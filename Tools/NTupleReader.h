#ifndef NTUPLE_READER_H
#define NTUPLE_READER_H

#include "SATException.h"

#include "TFile.h"
#include "TBranch.h"
#include "TTree.h"
#include "TLorentzVector.h"

#include <cstdio>
#include <vector>
#include <map>
#include <unordered_map>
#include <string>
#include <set>
#include <iostream>
#include <typeinfo>
#include <functional>
#include <cxxabi.h>

#ifdef __CINT__
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ class vector<TLorentzVector>+;
#endif

/* This class is designed to be a simple interface to reading stop NTuples
   
   To use this class simply open the desired Tree as a TTree or TChain and pass it 
   to the constructor.  Then the tuple contents may be accessed as follows

   NTupleReader tr(tree);
   while(tr.getNextEvent())
   {
       const int& run = tr.getVar<unsigned int>("run");
   }

   and so on.  
 */

class NTupleReader;

void baselineUpdate(NTupleReader& tr);

class NTupleReader
{
    friend void baselineUpdate(NTupleReader& tr);

private:

    //Machinery to allow object cleanup
    bool DuplicateFDVector();
    template <class Tfrom, class Tto> void CastVector(NTupleReader& tr, const std::string& var, const char typen);
    //Generic deleter base class to obfiscate template type
    class deleter_base
    {
    public:
        virtual void destroy(void *) = 0;
        virtual ~deleter_base() {}
    };

    //Templated class to create/store simple object deleter 
    template<typename T> 
    class deleter : public deleter_base
    {
    public:
        virtual void destroy(void *ptr)
        {
            delete static_cast<T*>(ptr);
        }
    };

    //Templated class to create/store vector object deleter 
    template<typename T> 
    class vec_deleter : public deleter_base
    {
    public:
        virtual void destroy(void *ptr)
        {
            //Delete vector
            T *vecptr = *static_cast<T**>(ptr);
            if(vecptr != nullptr) delete vecptr;

            //depete pointer to vector
            delete static_cast<T*>(ptr);
        }
    };

    //Handle class to hold pointer and deleter
    class Handle
    {
    public:
        void* ptr;
        deleter_base* deleter;

        Handle() : ptr(nullptr), deleter(nullptr) {}

        Handle(void* ptr, deleter_base* deleter = nullptr) :  ptr(ptr), deleter(deleter) {}

        void destroy()
        {
            if(deleter)
            {
                deleter->destroy(ptr);
                delete deleter;
            }
        }
    };

    //Helper to make simple Handle
    template<typename T>
    static inline Handle createHandle(T* ptr)
    {
        return Handle(ptr, new deleter<T>);
    }

    //Helper to make vector Handle
    template<typename T>
    static inline Handle createVecHandle(T* ptr)
    {
        return Handle(ptr, new vec_deleter<T>);
    }

    //function wrapper 
    //base class
    class FuncWrapper
    {
    public:
        virtual bool operator()(NTupleReader& tr) = 0;
    };

    //class for arbitrary return value
    template<typename T>
    class FuncWrapperImpl : public FuncWrapper
    {
    private:
        T func_;
    public:
        bool operator()(NTupleReader& tr)
        {
            func_(tr);
            return true;
        }

        FuncWrapperImpl(T f) : func_(f) {}
    };

public:

    NTupleReader(TTree * tree, const std::set<std::string>& activeBranches_);
    NTupleReader(TTree * tree);
    ~NTupleReader();

    std::string getFileName() const;

    int getEvtNum() const
    {
        return nevt_;
    }

    inline bool isFirstEvent() const
    {
        return evtProcessed_ <= 1;
    }

    int getNEntries() const;

    inline bool checkBranch(const std::string& name) const
    {
        return (typeMap_.find(name) != typeMap_.end());
    }
    inline bool hasVar(const std::string& name) const {return checkBranch(name); }

    bool goToEvent(int evt);
    bool getNextEvent();
    void disableUpdate();
    void printTupleMembers(FILE *f = stdout) const;

    std::vector<std::string> getTupleMembers() const;
    std::vector<std::string> getTupleSpecs(const std::string& varName) const;

    template<typename T> void registerFunction(T f)
    {
        if(isFirstEvent()) functionVec_.emplace_back(new FuncWrapperImpl<T>(f));
        else THROW_SATEXCEPTION("New functions cannot be registered after tuple reading begins!\n");
    }

    //Specialization for basic functions
    void registerFunction(void (*f)(NTupleReader&));
    void registerFunction(bool (*f)(NTupleReader&));

    void getType(const std::string& name, std::string& type) const;

    void setReThrow(const bool);
    bool getReThrow() const;

    template<typename T> void registerDerivedVar(const std::string& name, T var)
    {
        try
        {
            auto handleItr = branchMap_.find(name);
            if(handleItr == branchMap_.end())
            {
                auto typeItr = typeMap_.find(name);
                if(typeItr != typeMap_.end())
                {
                    THROW_SATEXCEPTION("You are trying to redefine a tuple var: \"" + name + "\".  This is not allowed!  Please choose a unique name.");
                }
                handleItr = branchMap_.insert(std::make_pair(name, createHandle(new T()))).first;

                typeMap_[name] = demangle<T>();
            }
            setDerived(var, handleItr->second.ptr);
        }
        catch(const SATException& e)
        {
            e.print();
            if(reThrow_) throw;
        }
    }

    template<typename T> void registerDerivedVec(const std::string& name, T* var)
    {
        try
        {
            auto handleItr = branchVecMap_.find(name);
            if(handleItr == branchVecMap_.end())
            {
                auto typeItr = typeMap_.find(name);
                if(typeItr != typeMap_.end())
                {
                    THROW_SATEXCEPTION("You are trying to redefine a tuple var: \"" + name + "\".  This is not allowed!  Please choose a unique name.");
                }
                handleItr = branchVecMap_.insert(std::make_pair(name, createVecHandle(new T*()))).first;
            
                typeMap_[name] = demangle<T>();
            }
            T *vecptr = *static_cast<T**>(handleItr->second.ptr);
            if(vecptr != nullptr)
            {
                delete vecptr;
            }
            setDerived(var, handleItr->second.ptr);
        }
        catch(const SATException& e)
        {
            e.print();
            if(reThrow_) throw;
        }
    }

    void addAlias(const std::string& name, const std::string& alias);

    const void* getPtr(const std::string& var) const;
    const void* getVecPtr(const std::string& var) const;

    template<typename T> const T& getVar(const std::string& var) const
    {
        //This function can be used to return single variables

        try
        {
            return getTupleObj<T>(var, branchMap_);
        }
        catch(const SATException& e)
        {
            if(isFirstEvent()) e.print();
            if(reThrow_) throw;
            return *static_cast<T*>(nullptr);
        }
    }

    template<typename T> const std::vector<T>& getVec(const std::string& var) const
    {
        //This function can be used to return vectors

        try
        {
            return *getTupleObj<std::vector<T>*>(var, branchVecMap_);
        }
        catch(const SATException& e)
        {
            if(isFirstEvent()) e.print();
            if(reThrow_) throw;
            return *static_cast<std::vector<T>*>(nullptr);
        }
    }

    template<typename T, typename V> const std::map<T, V>& getMap(const std::string& var) const
    {
        //This function can be used to return maps

        try
        {
            return *getTupleObj<std::map<T, V>*>(var, branchVecMap_);
        }
        catch(const SATException& e)
        {
            if(isFirstEvent()) e.print();
            if(reThrow_) throw;
            return *static_cast<std::map<T, V>*>(nullptr);
        }
    }
 
 
private:
    // private variables for internal use
    TTree *tree_;
    int nevt_, evtProcessed_;
    bool isUpdateDisabled_, reThrow_;
    
    // stl collections to hold branch list and associated info
    mutable std::unordered_map<std::string, Handle> branchMap_;
    mutable std::unordered_map<std::string, Handle> branchVecMap_;
    std::vector<FuncWrapper*> functionVec_;
    mutable std::unordered_map<std::string, std::string> typeMap_;
    mutable std::unordered_map<std::string, std::string> VectypeMap_;
    std::set<std::string> activeBranches_;

    void init();

    void populateBranchList();
    
    void registerBranch(TBranch * const branch) const;

    bool calculateDerivedVariables();

    bool goToEventInternal(int evt, const bool filter);

    template<typename T> void registerBranch(const std::string& name) const
    {
        branchMap_[name] = createHandle(new T());

        typeMap_[name] = demangle<T>();

        tree_->SetBranchStatus(name.c_str(), 1);
        tree_->SetBranchAddress(name.c_str(), branchMap_[name].ptr);
    }
    
    template<typename T> void registerVecBranch(const std::string& name) const
    {
        branchVecMap_[name] = createVecHandle(new std::vector<T>*());

        typeMap_[name] = demangle<std::vector<T>>();
        VectypeMap_[name] = typeid(T).name();

        tree_->SetBranchStatus(name.c_str(), 1);
        tree_->SetBranchAddress(name.c_str(), branchVecMap_[name].ptr);
    }

    template<typename T> void updateTupleVar(const std::string& name, const T& var)
    {
        if(isFirstEvent())
        {
            if(branchMap_.find(name) == branchMap_.end())
            {
                branchMap_[name] = createVecHandle(new T());
                
                typeMap_[name] = demangle<T>();
            }
        }

        auto tuple_iter = branchMap_.find(name);
        if(tuple_iter != branchMap_.end())
        {
            *static_cast<T*>(tuple_iter->second.ptr) = var;
        }
        else THROW_SATEXCEPTION("Variable not found: \"" + name + "\"!!!\n");
    }

    template<typename T, typename V> T& getTupleObj(const std::string& var, const V& v_tuple) const
    {
        auto tuple_iter = v_tuple.find(var);
        bool intuple = tuple_iter != v_tuple.end() ;
        auto typemap_iter = typeMap_.find(var);
        bool inTypeMap = typemap_iter != typeMap_.end();
        std::string requestType = demangle<T>();
        bool isSameType = false;
        if (inTypeMap)
        {
          isSameType  =  requestType.find(typeMap_[var]) != std::string::npos;
          // BAD IDea, but allow C++ to cost to other types
          if (requestType.find("vector") == std::string::npos)
            isSameType = true;
        }

        //std::cout << var << " intuple" << intuple <<" typemap " << inTypeMap <<" same type " << isSameType <<" request type" << requestType 
          //<< " store type " << typemap_iter->second << std::endl;

        if( intuple && inTypeMap && isSameType)
        {
          return *reinterpret_cast<T*>(tuple_iter->second.ptr);
        }
        else if( !intuple && inTypeMap && isSameType ) //If it is not found in tuple, but in branch
        {
            //If found in typeMap_, it can be added on the fly
            TBranch *branch = tree_->FindBranch(var.c_str());
        
            //If branch not found continue on to throw exception
            if(branch != nullptr)
            {
                registerBranch(branch);
        
                //get iterator
                tuple_iter = v_tuple.find(var);
        
                //force read just this branch
                branch->GetEvent(nevt_ - 1);
        
                //return value
                return *static_cast<T*>(tuple_iter->second.ptr);
            }
        } else if (intuple && inTypeMap && !isSameType) 
        {
            char typen;
            if( requestType == "std::vector<float, std::allocator<float> >*")
              typen='f';
            if( requestType == "std::vector<double, std::allocator<double> >*")
              typen='d';
            std::string newname = var+"___" + typen;
            //std::cout << newname << std::endl;
            auto tuple_iter = branchVecMap_.find(newname);
            if (tuple_iter != branchVecMap_.end())
              return *static_cast<T*>(tuple_iter->second.ptr);
        }

        //It really does not exist, throw exception 
        THROW_SATEXCEPTION("Variable not found: \"" + var + "\" with type " + requestType +"!!!");
    }

    template<typename T> inline static void setDerived(const T& retval, void* const loc)
    {
        *static_cast<T*>(loc) = retval;
    }

    template<typename T> std::string demangle() const
    {
        // unmangled
        int status = 0;
        char* demangled = abi::__cxa_demangle(typeid(T).name(), 0, 0, &status);
        std::string s = demangled;
        free(demangled);
        return s;
    }
};

#endif
