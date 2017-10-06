#include "NTupleReader.h"

#include "TROOT.h"
#include "TInterpreter.h"
#include "TObjArray.h"
#include "TLeaf.h"

#include <iostream>

NTupleReader::NTupleReader(TTree * tree, const std::set<std::string>& activeBranches) : activeBranches_(activeBranches)
{
    tree_ = tree;
    if(!tree_) THROW_SATEXCEPTION("NTupleReader(...): TTree " + std::string(tree_->GetName()) + " is invalid!!!!");
    init();
}

NTupleReader::NTupleReader(TTree * tree)
{
    tree_ = tree;
    init();
}

NTupleReader::~NTupleReader()
{
    //Clean up any remaining dynamic memory
    for(auto& branch : branchMap_)    if(branch.second.ptr) branch.second.destroy();
    for(auto& branch : branchVecMap_) if(branch.second.ptr) branch.second.destroy();
}

void NTupleReader::init()
{
    nevt_ = evtProcessed_ = 0;
    isUpdateDisabled_ = false;
    reThrow_ = true;

    tree_->SetBranchStatus("*", 0);

    // Add desired branches to branchMap_/branchVecMap
    populateBranchList();
}


std::string NTupleReader::getFileName() const
{
    return std::string( tree_->GetCurrentFile()->GetName() );
}

int NTupleReader::getNEntries() const
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

void NTupleReader::populateBranchList()
{
    TObjArray *lob = tree_->GetListOfBranches();
    TIter next(lob);
    TBranch *branch;

    while(branch = (TBranch*)next()) 
    {
        std::string name(branch->GetName());

        if(activeBranches_.size() > 0 && activeBranches_.count(name) == 0)
        {
            //allow typeMap_ to track that the branch exists without filling type
            typeMap_[name] = "";
            continue;
        }

        registerBranch(branch);
    }
}

void NTupleReader::registerBranch(TBranch * const branch) const
{
    std::string type(branch->GetTitle());
    std::string name(branch->GetName());

    if(type.compare(name) == 0)
    {
        TObjArray *lol = branch->GetListOfLeaves();
        if (lol->GetEntries() == 1) 
        {
            TLeaf *leaf = (TLeaf*)lol->UncheckedAt(0);
            type = leaf->GetTypeName();
        }
    }

    if(type.find("vector") != std::string::npos)
    {
        if     (type.find("double")         != std::string::npos) registerVecBranch<double>(name);
        else if(type.find("unsigned int")   != std::string::npos) registerVecBranch<unsigned int>(name);
        else if(type.find("unsigned long")   != std::string::npos) registerVecBranch<unsigned long>(name);
        else if(type.find("unsigned char")   != std::string::npos) registerVecBranch<unsigned char>(name);
        else if(type.find("unsigned short")   != std::string::npos) registerVecBranch<unsigned short>(name);
        else if(type.find("short")   != std::string::npos) registerVecBranch<short>(name);
        else if(type.find("char")   != std::string::npos) registerVecBranch<char>(name);
        else if(type.find("int")            != std::string::npos) registerVecBranch<int>(name);
        else if(type.find("bool")            != std::string::npos) registerVecBranch<bool>(name);
        else if(type.find("string")         != std::string::npos) registerVecBranch<std::string>(name);
        else if(type.find("TLorentzVector") != std::string::npos) registerVecBranch<TLorentzVector>(name);
        else if(type.find("float")          != std::string::npos) registerVecBranch<float>(name);
        else THROW_SATEXCEPTION("No type match for branch \"" + name + "\" with type \"" + type + "\"!!!");
    }
    else
    {
        if     (type.find("/D") != std::string::npos) registerBranch<double>(name);
        else if(type.find("/I") != std::string::npos) registerBranch<int>(name);
        else if(type.find("/i") != std::string::npos) registerBranch<unsigned int>(name);
        else if(type.find("/F") != std::string::npos) registerBranch<float>(name);
        else if(type.find("/C") != std::string::npos) registerBranch<char>(name);
        else if(type.find("/c") != std::string::npos) registerBranch<unsigned char>(name);
        else if(type.find("/S") != std::string::npos) registerBranch<short>(name);
        else if(type.find("/s") != std::string::npos) registerBranch<unsigned short>(name);
        else if(type.find("/O") != std::string::npos) registerBranch<bool>(name);
        else if(type.find("/L") != std::string::npos) registerBranch<unsigned long>(name);
        else if(type.find("/l") != std::string::npos) registerBranch<long>(name);
        else if(type.find("/b") != std::string::npos) registerBranch<bool>(name);
        else THROW_SATEXCEPTION("No type match for branch \"" + name + "\" with type \"" + type + "\"!!!");
    }
}

bool NTupleReader::goToEvent(int evt)
{
    int status = tree_->GetEntry(evt);
    if (status == 0) return false;
    nevt_ = evt + 1;
    ++evtProcessed_;
    calculateDerivedVariables();
    return status > 0;
}

bool NTupleReader::getNextEvent()
{
    return goToEvent(nevt_);
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
}

void NTupleReader::registerFunction(void (*f)(NTupleReader&))
{
    if(isFirstEvent()) functionVec_.emplace_back(f);
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

void NTupleReader::setReThrow(const bool reThrow)
{
    reThrow_ = reThrow;
}

bool NTupleReader::getReThrow() const
{
    return reThrow_;
}

void NTupleReader::addAlias(const std::string& name, const std::string& alias)
{
    //Check that alias i not already used
    if(typeMap_.find(alias) == typeMap_.end())
    {
        //set type for alias
        typeMap_[alias] = typeMap_[name];

        auto branch_iter = branchMap_.find(name);
        auto branchVec_iter = branchVecMap_.find(name);
        
        //Check if this variable is already registered and register it if not 
        if(branch_iter == branchMap_.end() && branchVec_iter == branchVecMap_.end())
        {
            //If found in typeMap_, it can be added on the fly
            TBranch *branch = tree_->FindBranch(name.c_str());
        
            //If branch not found continue on to throw exception
            if(branch != nullptr)
            {
                registerBranch(branch);
        
                //force read just this branch if necessary
                if(evtProcessed_ >= 1) branch->GetEvent(nevt_ - 1);
            }
        }

        branch_iter = branchMap_.find(name);
        branchVec_iter = branchVecMap_.find(name);

        //Set the "fake" handle for the alias 
        //Check branchMap for "name"
        if(branch_iter != branchMap_.end())
        {
            branchMap_[alias] = Handle(branch_iter->second.ptr);
        }
        //If the variable name is not in branchMap, check branchVecMap
        else if(branchVec_iter != branchVecMap_.end())
        {
            branchVecMap_[alias] = Handle(branchVec_iter->second.ptr);
        }
    }
    else
    {
        THROW_SATEXCEPTION("Variable name \"" + alias + "\" already exists!!!");
    }
}

const void* NTupleReader::getPtr(const std::string& var) const
{
    //This function can be used to return the variable pointer
    try
    {
        auto tuple_iter = branchMap_.find(var);
        if(tuple_iter != branchMap_.end())
        {
            return tuple_iter->second.ptr;
        }

        THROW_SATEXCEPTION("NTupleReader::getPtr(...): Variable not found: " + var);
    }
    catch(const SATException& e)
    {
        e.print();
        if(reThrow_) throw;
    }
}

const void* NTupleReader::getVecPtr(const std::string& var) const
{
    //This function can be used to return the variable pointer

    try
    {
        auto tuple_iter = branchVecMap_.find(var);
        if(tuple_iter != branchVecMap_.end())
        {
            return tuple_iter->second.ptr;
        }

        THROW_SATEXCEPTION("NTupleReader::getVecPtr(...): Variable not found: " + var);
    }
    catch(const SATException& e)
    {
        e.print();
        if(reThrow_) throw;
    }
}

void NTupleReader::printTupleMembers(FILE *f) const
{
    for(auto& iVar : typeMap_)
    {
        fprintf(f, "%60s %s\n", iVar.second.c_str(), iVar.first.c_str());
    }
}

std::vector<std::string> NTupleReader::getTupleMembers() const
{
    std::vector<std::string> members;
    for(auto& iVar : typeMap_)
    {
        members.push_back(iVar.first);
    }
    return members;
}


std::vector<std::string> NTupleReader::getTupleSpecs(const std::string& varName) const
{
    std::vector<std::string> members = getTupleMembers();
    std::vector<std::string> specs;
    for(auto &member : members)
    {
        std::string::size_type t= member.find(varName);
        if (t != std::string::npos)
        {
            specs.push_back(member.erase(t, varName.length()));
        }
    }
  
    return specs;
}
