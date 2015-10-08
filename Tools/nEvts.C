#include "../../SusyAnaTools/Tools/samples.h"

#include "TFile.h"
#include "TTree.h"

#include <iostream>

int main()
{
    AnaSamples::SampleSet        ss;
    AnaSamples::SampleCollection sc(ss);

    for(auto& filelist : ss) 
    {
        auto fileVec = filelist.second.getFilelist();
        int nEvts = 0;
        for(auto& filename : fileVec)
        {
            TFile *f = TFile::Open(filename.c_str());
            TTree *t = (TTree*)f->Get(filelist.second.treePath.c_str());
            nEvts += t->GetEntries();
            f->Close();
        }
     
        std::cout << "Processing file(s): " << filelist.second.filePath << "\t" << nEvts << std::endl;
    }

}
