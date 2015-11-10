#include "../../SusyAnaTools/Tools/samples.h"

#include <string>

extern "C" {
    AnaSamples::SampleCollection* SC_new(){ return new AnaSamples::SampleCollection(*(new AnaSamples::SampleSet())); }
    int SC_samples_size(AnaSamples::SampleCollection* sc, char *scn){ return (*sc)[std::string(scn)].size(); }
    char const ** SC_samples(AnaSamples::SampleCollection* sc, char *scn)
    {
        auto& sampleVec = (*sc)[std::string(scn)];
        const char **array = new const char*[sampleVec.size()];
        int i = 0;
        for(auto& sample : sampleVec)
        {
            array[i++] = sample.filePath.c_str();
        }
        return array;
    }
    char const ** SC_samples_names(AnaSamples::SampleCollection* sc, char *scn)
    {
        auto& sampleVec = sc->getSampleLabels(std::string(scn));
        const char **array = new const char*[sampleVec.size()];
        int i = 0;
        for(auto& sample : sampleVec)
        {
            array[i++] = sample.c_str();
        }
        return array;
    }
}
