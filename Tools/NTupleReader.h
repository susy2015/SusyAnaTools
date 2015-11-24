#ifndef NTUPLE_READER_H
#define NTUPLE_READER_H

#include "TFile.h"
#include "TBranch.h"
#include "TTree.h"
#include "TLorentzVector.h"

#include <vector>
#include <cstdio>
#include <map>
#include <string>
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
    // List of all variables used in tuple

    NTupleReader(TTree * tree, std::set<std::string>& activeBranches_);
    NTupleReader(TTree * tree);

    int getEvtNum() const
    {
        return nevt_;
    }

    bool IsFirstEvent() const
    {
      return isFirstEvent_;
    }

    int getNEntries() const
    {
        if(tree_) return nEvtTotal_;
        else 
        {
            printf("NTupleReader::getNEntries() - NO tree defined yet!!!\n");
            return -1;
        }
    }

    bool getNextEvent();
    bool goToEvent(int evt_);
    void disableUpdate();
    void printTupleMembers(FILE *f = stdout) const;

    void registerFunction(std::function<void(NTupleReader&)>);

    void getType(const std::string& name, std::string& type) const;

    template<typename T> void registerDerivedVar(const std::string name, T var)
    {
        if(isFirstEvent_)
        {
            if(branchMap_.find(name) != branchMap_.end())
            {
                printf("NTupleReader::registerDerivedVar(...): You are trying to redefine a base tuple var: \"%s\".  This is not allowed!  Please choose a unique name.\n", name.c_str());
                throw name;
            }
            branchMap_[name] = new T();

            std::string type;
            demangle<T>(type);
            typeMap_[name] = type;
        }
        setDerived(var, branchMap_[name]);
    }

    template<typename T> void registerDerivedVec(const std::string name, T* var)
    {
        if(isFirstEvent_)
        {
            if(branchVecMap_.find(name) != branchVecMap_.end())
            {
                printf("NTupleReader::registerDerivedVar(...): You are trying to redefine a base tuple var: \"%s\".  This is not allowed!  Please choose a unique name.\n", name.c_str());
                throw name;
            }
            branchVecMap_[name] = new T*();
            
            std::string type;
            demangle<T>(type);
            typeMap_[name] = type;
        }
        void * vecloc = branchVecMap_[name];
        T *vecptr = *static_cast<T**>(branchVecMap_[name]);
        if(vecptr != nullptr)
        {
            delete vecptr;
        }
        setDerived(var, vecloc);
    }

    template<typename T> const T& getVar(const std::string var) const
    {
        //This function can be used to return single variables

        return getTupleObj<T>(var, branchMap_);
    }

    template<typename T> const std::vector<T>& getVec(const std::string var) const
    {
        //This function can be used to return vectors
        return *getTupleObj<std::vector<T>*>(var, branchVecMap_);
    }

    template<typename T, typename V> const std::map<T, V>& getMap(const std::string var) const
    {
        //This function can be used to return maps

        return *getTupleObj<std::map<T, V>*>(var, branchVecMap_);
    }
 
 
private:
    // private variables for internal use
    TTree *tree_;
    int nevt_, nEvtTotal_;
    bool isUpdateDisabled_, isFirstEvent_;
    
    // stl collections to hold branch list and associated info
    std::map<std::string, void *> branchMap_;
    std::map<std::string, void *> branchVecMap_;
    std::vector<std::function<void(NTupleReader&)> > functionVec_;
    std::map<std::string, std::string> typeMap_;
    std::set<std::string> activeBranches_;
    //Hack to get around segfault
    std::map<std::string, void *> inactiveBranchMap_;

    void activateBranches();
    void populateBranchList();
    
    void calculateDerivedVariables();

    void init();

    template<typename T> void registerBranch(const std::string name)
    {
        branchMap_[name] = new T();

        std::string type;
        demangle<T>(type);
        typeMap_[name] = type;
    }
    
    template<typename T> void registerVecBranch(const std::string name)
    {
        branchVecMap_[name] = new std::vector<T>*();

        std::string type;
        demangle<std::vector<T>>(type);
        typeMap_[name] = type;
    }

    template<typename T> void updateTupleVar(const std::string name, const T& var)
    {
        if(isFirstEvent_)
        {
            if(branchMap_.find(name) == branchMap_.end())
            {
                branchMap_[name] = new T();
                std::string type;
                demangle<T>(type);
                typeMap_[name] = type;
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

        if(isFirstEvent_) printf("NTupleReader::getTupleObj(const std::string var):  Variable not found: \"%s\"!!!\n", var.c_str());
        throw var;
    }

    template<typename T> inline static void setDerived(const T& retval, void* const loc)
    {
        *static_cast<T*>(loc) = retval;
    }

    template<typename T> void demangle(std::string& s)
    {
        // unmangled
        int status = 0;
        char* demangled = abi::__cxa_demangle(typeid(T).name(), 0, 0, &status);
        s = demangled;
        delete [] demangled;
    }
};

#endif
