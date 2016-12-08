#ifndef NTUPLE_READER_H
#define NTUPLE_READER_H

#include "SATException.h"

#include "TFile.h"
#include "TBranch.h"
#include "TTree.h"
#include "TLorentzVector.h"

#include <vector>
#include <cstdio>
#include <iostream>
#include <map>
#include <string>
#include <limits>
#include <vector>
#include <set>
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
       const int& run = tr.getVar<int>("run");
   }

   and so on.  
 */

class NTupleReader;

void baselineUpdate(NTupleReader& tr);

class NTupleReader
{
    friend void baselineUpdate(NTupleReader& tr);
public:

    NTupleReader(TTree * tree, std::set<std::string>& activeBranches_);
    NTupleReader(TTree * tree);
    ~NTupleReader() {} ;

    int getEvtNum() const
    {
        return nevt_;
    }

    bool isFirstEvent() const
    {
        return isFirstEvent_;
    }

    int getNEntries() const
    {
        try
        {
            if(tree_) return tree_->GetEntries();
            else 
            {
                THROW_SATEXCEPTION("NO tree defined yet!!!");
            }
        }
        catch(const SATException& e)
        {
            e.print();
            if(reThrow_) throw;
        }
    }

    bool checkBranch(const std::string name) const
    {
        return (typeMap_.find(name) != typeMap_.end());
    }

    bool getNextEvent();
    void disableUpdate();
    void printTupleMembers(FILE *f = stdout) const;
    bool hasVar(std::string name) const;
    std::vector<std::string> GetTupleMembers() const;
    std::vector<std::string> GetTupleSpecs(std::string VarName = "cntNJetsPt30Eta24") const;

    template<typename T> void registerFunction(T f)
    {
        if(isFirstEvent_) functionVec_.emplace_back(f);
        else printf("NTupleReader::registerFunction(...): new functions cannot be registered after tuple reading begins!\n");
    }

    //Specialization for basic functions
    void registerFunction(void (*f)(NTupleReader&));

    void getType(const std::string& name, std::string& type) const;

    void setReThrow(const bool);
    bool getReThrow() const;

    template<typename T> void registerDerivedVar(const std::string name, T var)
    {
        try
        {
            if(isFirstEvent_)
            {
                if(branchMap_.find(name) != branchMap_.end())
                {
                    THROW_SATEXCEPTION("You are trying to redefine a base tuple var: \"" + name + "\".  This is not allowed!  Please choose a unique name.");
                }
                branchMap_[name] = new T();

                typeMap_[name] = demangle<T>();
            }
            setDerived(var, branchMap_[name]);
        }
        catch(const SATException& e)
        {
            e.print();
            if(reThrow_) throw;
        }
    }

    template<typename T> void registerDerivedVec(const std::string name, T* var)
    {
        try
        {
            if(isFirstEvent_)
            {
                if(branchVecMap_.find(name) != branchVecMap_.end())
                {
                    THROW_SATEXCEPTION("You are trying to redefine a base tuple var: \"" + name + "\".  This is not allowed!  Please choose a unique name.");
                }
                branchVecMap_[name] = new T*();
            
                typeMap_[name] = demangle<T>();
            }
            void * vecloc = branchVecMap_[name];
            T *vecptr = *static_cast<T**>(branchVecMap_[name]);
            if(vecptr != nullptr)
            {
                delete vecptr;
            }
            setDerived(var, vecloc);
        }
        catch(const SATException& e)
        {
            e.print();
            if(reThrow_) throw;
        }
    }

    const void* getPtr(const std::string var) const;
    const void* getVecPtr(const std::string var) const;

    template<typename T> const T& getVar(const std::string var) const
    {
        //This function can be used to return single variables

        try
        {
            return getTupleObj<T>(var, branchMap_);
        }
        catch(const SATException& e)
        {
            e.print();
            if(reThrow_) throw;
            return *static_cast<T*>(nullptr);
        }
    }

    template<typename T> const std::vector<T>& getVec(const std::string var) const
    {
        //This function can be used to return vectors

        try
        {
            return *getTupleObj<std::vector<T>*>(var, branchVecMap_);
        }
        catch(const SATException& e)
        {
            e.print();
            if(reThrow_) throw;
            return *static_cast<std::vector<T>*>(nullptr);
        }
    }

    template<typename T, typename V> const std::map<T, V>& getMap(const std::string var) const
    {
        //This function can be used to return maps

        try
        {
            return *getTupleObj<std::map<T, V>*>(var, branchVecMap_);
        }
        catch(const SATException& e)
        {
            e.print();
            if(reThrow_) throw;
            return *static_cast<std::map<T, V>*>(nullptr);
        }
    }
 
 
private:
    // private variables for internal use
    TTree *tree_;
    int nevt_;
    bool isUpdateDisabled_, isFirstEvent_, reThrow_;
    
    // stl collections to hold branch list and associated info
    mutable std::map<std::string, void *> branchMap_;
    mutable std::map<std::string, void *> branchVecMap_;
    std::vector<std::function<void(NTupleReader&)> > functionVec_;
    mutable std::map<std::string, std::string> typeMap_;
    std::set<std::string> activeBranches_;

    void init();

    void activateBranches();
    void populateBranchList();
    
    void registerBranch(TBranch * const branch) const;

    void calculateDerivedVariables();

    template<typename T> void registerBranch(const std::string name) const
    {
        branchMap_[name] = new T();

        typeMap_[name] = demangle<T>();
    }
    
    template<typename T> void registerVecBranch(const std::string name) const
    {
        branchVecMap_[name] = new std::vector<T>*();

        typeMap_[name] = demangle<std::vector<T>>();
    }

    template<typename T> void updateTupleVar(const std::string name, const T& var)
    {
        if(isFirstEvent_)
        {
            if(branchMap_.find(name) == branchMap_.end())
            {
                branchMap_[name] = new T();
                
                typeMap_[name] = demangle<T>();
            }
        }

        auto tuple_iter = branchMap_.find(name);
        if(tuple_iter != branchMap_.end())
        {
            *static_cast<T*>(tuple_iter->second) = var;
        }
        else printf("NTupleReader::updateTuple(...):  Variable not found: \"%s\"!!!\n", name.c_str());
    }

    template<typename T, typename V> T& getTupleObj(const std::string var, const V& v_tuple) const
    {
        auto tuple_iter = v_tuple.find(var);
        if(tuple_iter != v_tuple.end())
        {
            return *static_cast<T*>(tuple_iter->second);
        }
        else if(typeMap_.find(var) != typeMap_.end()) //If it is not found, check in typeMap_ 
        {
            //If found in typeMap_, it can be added on the fly
            TBranch *branch = tree_->FindBranch(var.c_str());

            //If branch not found continue on to throw exception
            if(branch != nullptr)
            {
                registerBranch(branch);

                //get iterator
                tuple_iter = v_tuple.find(var);

                tree_->SetBranchStatus(var.c_str(), 1);
                tree_->SetBranchAddress(var.c_str(), tuple_iter->second);

                //force read just this branch
                branch->GetEvent(nevt_ - 1);

                //return value
                return *static_cast<T*>(tuple_iter->second);
            }
        }

        //It really does not exist, throw exception 
        THROW_SATEXCEPTION("Variable not found: \"" + var + "\"!!!");
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
        delete [] demangled;
        return s;
    }
};

#endif
