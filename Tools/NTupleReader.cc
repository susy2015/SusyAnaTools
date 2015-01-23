#include "NTupleReader.h"

#include "TROOT.h"
#include "TObjArray.h"

NTupleReader::NTupleReader(TTree * tree)
{
    tree_ = tree;
    nEvtTotal_ = tree_->GetEntries();
    nevt_ = 0;
    isUpdateDisabled_ = false;
    clearTuple();

    gROOT->ProcessLine("#include <vector>");
    
    //vectors must be initialized to 0 here to avoid segfaults.  This cannot be done 
    //in clearTuple() as it will cause memory leaks.

    activateBranches();
}

void NTupleReader::activateBranches()
{
    tree_->SetBranchStatus("*", 0);

    // Add desired branches to branchMap_
    populateBranchList();

    for(auto& iMap : branchMap_)
    {
        tree_->SetBranchStatus(iMap.first.c_str(), 1);
        tree_->SetBranchAddress(iMap.first.c_str(), iMap.second);
    }

    for(auto& iMap : branchVecMap_)
    {
        tree_->SetBranchStatus(iMap.first.c_str(), 1);
        tree_->SetBranchAddress(iMap.first.c_str(), iMap.second);
    }
}

void NTupleReader::populateBranchList()
{
    TObjArray *lob = tree_->GetListOfBranches();
    TIter next(lob);
    TBranch *branch;
    while(branch = (TBranch*)next()) 
    {
        std::string type(branch->GetTitle());
        std::string name(branch->GetName());

        if(type.find("vector") != std::string::npos)
        {
            if(type.find("double")         != std::string::npos) registerVecBranch<double>(name);
            if(type.find("int")            != std::string::npos) registerVecBranch<int>(name);
            if(type.find("unsigned int")   != std::string::npos) registerVecBranch<unsigned int>(name);
            if(type.find("string")         != std::string::npos) registerVecBranch<std::string>(name);
            if(type.find("TLorentzVector") != std::string::npos) registerVecBranch<TLorentzVector>(name);
        }
        else
        {
            if(type.find("/D") != std::string::npos) registerBranch<double>(name);
            if(type.find("/I") != std::string::npos) registerBranch<int>(name);
            if(type.find("/i") != std::string::npos) registerBranch<unsigned int>(name);
        }
    }
}

bool NTupleReader::getNextEvent()
{
    if(nevt_ >= nEvtTotal_)
    {
        return false;
    }
    //clearTuple();
    int status = tree_->GetEntry(nevt_);
    nevt_++;
    if(!isUpdateDisabled_) updateTuple();
    calculateDerivedVariables();
    return status > 0;
}

void NTupleReader::clearTuple()
{
    // Do not initialize vector pointers here!

}

void NTupleReader::disableUpdate()
{
    isUpdateDisabled_ = true;
    printf("NTupleReader::disableUpdate(): You have disabled tuple updates.  You may therefore be using old variablre definitions.  Be sure you are ok with this!!!\n");
}

void NTupleReader::calculateDerivedVariables()
{
    //My apoligies for anyone looking at this totally cryptic line of code
    for(auto& var : derivedMap_)
    {
        var.second.first(*this, var.second.second);
    }

    for(auto& var : derivedVecMap_)
    {
        var.second.first(*this, var.second.second);
    }
}

void NTupleReader::updateTuple()
{
    
}
