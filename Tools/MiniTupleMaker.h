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
    std::map<std::string, std::pair<void *, std::function<void()>>> tupleVars_;
    std::vector<std::function<void()>> cleanupVec_;

    template<typename T> void prepVar(const NTupleReader& tr, const std::string& name, std::pair<void *, std::function<void()>>& vs)
    {
        vs.first = new T;
        vs.second = std::bind(copyVar<T>, tr.getPtr(name), vs.first);
        cleanupVec_.emplace_back(std::bind(cleanup<T>, vs.first));
    }

    template<typename T> static void copyVar(const void* tupleVar, void* miniTupleVar)
    {
        *static_cast<T*>(miniTupleVar) = *static_cast<const T*>(tupleVar);
    }

    template<typename T> static void cleanup(void* miniTupleVar)
    {
        if(miniTupleVar) delete static_cast<T*>(miniTupleVar);
    }
};

#endif
