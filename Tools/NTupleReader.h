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
#include <typeinfo>
#include <cxxabi.h>

#ifdef __MAKECINT__
#pragma link C++ class vector<int>;
#pragma link C++ class vector<vector<int> >;
#pragma link C++ class vector<vector<vector<int> > >;
#pragma link C++ class vector<TLorentzVector>;
#pragma link C++ class vector<string>;
#pragma link C++ class vector<vector<TLorentzVector> >;
#endif


/* This class is designed to be a simple interface to reading stop NTuples
   
   To use this class simply open the desired Tree as a TTree or TChain and pass it 
   to the constructor.  Then the tuple contents may be accessed as follows

   NTupleReader tr(tree);
   while(tr.getNextEvent())
   {
       int run = tr.run;
   }
   
   
   To add a variable to the reader it should be declared in the public section of the class definition,
   and added to the branchMap_ map in the populateBranchList().  Vectors they must be declared
   as pointers and initialized to zero in the constructor but not in clearTuple().  For simple variables 
   they shoud be initialized to an approperiate value in clearTuple().  
 */

class NTupleReader
{

public:
    // List of all variables used in tuple

    NTupleReader(TTree * tree);

    int getEvtNum() const
    {
        return nevt_;
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
    void disableUpdate();
    void printTupleMembers(FILE *f = stdout) const;

    void registerFunction(void (*f)(NTupleReader&));

    template<typename T> void registerDerivedVar(const std::string name, T var)
    {
        if(isFirstEvent_)
        {
            if(branchMap_.find(name) != branchMap_.end())
            {
                printf("NTupleReader::registerDerivedVar(...): You are trying to redefine a base tuple var: \"%s\".  This is not allowed!  Please choose a unique name.\n", name.c_str());
                return;
            }
            branchMap_[name] = new T();

            std::string type;
            demangle<T>(type);
            typeVec_.push_back(std::make_pair(name, type));
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
                return;
            }
            branchVecMap_[name] = new T*();
            
            std::string type;
            demangle<T>(type);
            typeVec_.push_back(std::make_pair(name, type));
        }
        void * vecloc = branchVecMap_[name];
        T *vecptr = *static_cast<T**>(branchVecMap_[name]);
        if(vecptr != nullptr)
        {
            vecptr->~vector();
        }
        setDerived(var, vecloc);
    }

    template<typename T> T getVar(const std::string var) const
    {
        //This function can be used to return single variables

        return getTupleObj<T>(var, branchMap_);
    }

    template<typename T> const std::vector<T>& getVec(const std::string var) const
    {
        //This function can be used to return vectors

        return *getTupleObj<std::vector<T>*>(var, branchVecMap_);
    }
 
private:
    // private variables for internal use
    TTree *tree_;
    int nevt_, nEvtTotal_;
    bool isUpdateDisabled_, isFirstEvent_;

    // Maps to hold branch list 
    std::map<std::string, void *> branchMap_;
    std::map<std::string, void *> branchVecMap_;
    std::vector<void (*)(NTupleReader&)> functionVec_;
    std::vector<std::pair<std::string, std::string>> typeVec_;

    void activateBranches();
    void populateBranchList();
    
    void calculateDerivedVariables();

    void clearTuple();
    void updateTuple();

    template<typename T> void registerBranch(std::string name)
    {
        branchMap_[name] = new T();

        std::string type;
        demangle<T>(type);
        typeVec_.push_back(std::make_pair(name, type));
    }
    
    template<typename T> void registerVecBranch(std::string name)
    {
        branchVecMap_[name] = new std::vector<T>*();

        std::string type;
        demangle<std::vector<T>>(type);
        typeVec_.push_back(std::make_pair(name, type));
    }
    
    template<typename T, typename V> T getTupleObj(const std::string var, const V& v_tuple) const
    {
        auto tuple_iter = v_tuple.find(var);
        if(tuple_iter != v_tuple.end())
        {
            return *static_cast<T*>(tuple_iter->second);
        }

        printf("NTupleReader::getTupleObj(const std::string var):  Variable not found: \"%s\"!!!\n", var.c_str());
        return T(0);
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
