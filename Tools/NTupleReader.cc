#include "NTupleReader.h"

#include "TROOT.h"
#include "TInterpreter.h"
#include "TObjArray.h"
#include "TLeaf.h"

NTupleReader::NTupleReader(TTree * tree, std::set<std::string>& activeBranches) : activeBranches_(activeBranches)
{
    tree_ = tree;
    init();
}

NTupleReader::NTupleReader(TTree * tree)
{
    tree_ = tree;
    init();
}

void NTupleReader::init()
{
    //gROOT->ProcessLine(".L TupleDict.h+");
    gInterpreter->GenerateDictionary("vector<TLorentzVector>","TLorentzVector.h;vector");

    nEvtTotal_ = tree_->GetEntries();
    nevt_ = 0;
    isUpdateDisabled_ = false;
    isFirstEvent_ = true;
    clearTuple();

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

        if(activeBranches_.size() > 0 && activeBranches_.count(name) == 0) continue;

        if(type.compare(name) == 0)
        {
            TObjArray *lol = branch->GetListOfLeaves();
            if (lol->GetEntries() == 1) 
            {
                TLeaf *leaf = (TLeaf*)lol->UncheckedAt(0);
                type = leaf->GetTypeName();
            }
            else continue;
        }

        if(type.find("vector") != std::string::npos)
        {
            if     (type.find("double")         != std::string::npos) registerVecBranch<double>(name);
            else if(type.find("unsigned int")   != std::string::npos) registerVecBranch<unsigned int>(name);
            else if(type.find("int")            != std::string::npos) registerVecBranch<int>(name);
            else if(type.find("string")         != std::string::npos) registerVecBranch<std::string>(name);
            else if(type.find("TLorentzVector") != std::string::npos) registerVecBranch<TLorentzVector>(name);
        }
        else
        {
            if     (type.find("/D") != std::string::npos) registerBranch<double>(name);
            else if(type.find("/I") != std::string::npos) registerBranch<int>(name);
            else if(type.find("/i") != std::string::npos) registerBranch<unsigned int>(name);
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
    for(auto& func : functionVec_)
    {
        func(*this);
    }

    isFirstEvent_ = false;
}

void NTupleReader::registerFunction(void (*f)(NTupleReader&))
{
    if(isFirstEvent_) functionVec_.push_back(f);
    else printf("NTupleReader::registerFunction(...): new functions cannot be registered after tuple reading begins!\n");
}

void NTupleReader::getType(const std::string& name, std::string& type) const
{
    auto typeIter = typeMap_.find(name);
    if(typeIter != typeMap_.end())
    {
        type = typeIter->second;
    }
}

void NTupleReader::updateTuple()
{
    
}

void NTupleReader::printTupleMembers(FILE *f) const
{
    for(auto& iVar : typeMap_)
    {
        fprintf(f, "%60s %s\n", iVar.second.c_str(), iVar.first.c_str());
    }
}
