#ifndef MinTupleMaker_h
#define MinTupleMaker_h

#include "NTupleReader.h"

#include "TTree.h"

#include <vector>
#include <set>
#include <map>
#include <utility>
#include <string>
#include <functional>

#include <iostream>

class MiniTupleMaker
{
public:
    MiniTupleMaker(TTree *);

    ~MiniTupleMaker();

    void setTupleVars(const std::set<std::string>);

    //To use derived variables initBranches must be called after the first tuple event is read
    void initBranches(const NTupleReader&);

    void fill();

private:
    TTree* const tree_;
    std::set<std::string> tupleVars_;

    template<typename T> void prepVar(const NTupleReader& tr, const std::string& name)
    {
        TBranch *tb = tree_->GetBranch(name.c_str());
        if(!tb) tree_->Branch(name.c_str(), static_cast<T*>(const_cast<void*>(tr.getPtr(name))));
        else       tb->SetAddress(const_cast<void*>(tr.getPtr(name)));
    }

    template<typename T> void prepVec(const NTupleReader& tr, const std::string& name)
    {
        TBranch *tb = tree_->GetBranch(name.c_str());
        if(!tb) tree_->Branch(name.c_str(), static_cast<std::vector<T>**>(const_cast<void*>(tr.getVecPtr(name))));
        else       tb->SetAddress(const_cast<void*>(tr.getVecPtr(name)));
    }
};

#endif
