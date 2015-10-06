#include "../../SusyAnaTools/Tools/samples.h"

#include "TFile.h"
#include "TChain.h"

#include <iostream>

int main()
{
    AnaSamples::SampleSet        ss;
    AnaSamples::SampleCollection sc(ss);

//    for(auto& filelist : sc)
//    {
        for(auto& file : ss) 
        {
            TChain *t = new TChain(file.second.treePath.c_str());
            file.second.addFilesToChain(t);
     
            std::cout << "Processing file(s): " << file.second.filePath << "\t" << t->GetEntries() << std::endl;
        }
//    }

}
