setenv LD_LIBRARY_PATH ./:${CMSSW_BASE}/src/opencv/lib/:${CMSSW_BASE}/src/TopTagger/TopTagger/test/:${CMSSW_BASE}/src/SusyAnaTools/Tools/obj/:${LD_LIBRARY_PATH}

## Get the btagging file
if (! -f CSVv2_ichep.csv) then
  cp $CMSSW_BASE/src/SusyAnaTools/Tools/CSVv2_ichep.csv .
endif

if (! -f TTbarNoHad_bTagEff.root) then
  cp $CMSSW_BASE/src/SusyAnaTools/Tools/TTbarNoHad_bTagEff.root .
endif

## Pileup Reweighting
if (! -f PileupHistograms_Nov17.root) then
  cp $CMSSW_BASE/src/SusyAnaTools/Tools/PileupHistograms_Nov17.root .
endif

## Get the Top tagger files
if (! -f Example_TopTagger.cfg) then
  cp  $CMSSW_BASE/src/TopTagger/TopTagger/test/Example_TopTagger.cfg .
endif

if (! -f Example_Legacy_TopTagger.cfg) then
  cp  $CMSSW_BASE/src/TopTagger/TopTagger/test/Example_Legacy_TopTagger.cfg .
endif

if (! -f TrainingOutput.model) then
  if (`hostname` =~ *fnal.gov) then
    cp /uscms_data/d3/mandal13/MVATraining/TrainingOutput_v1.model TrainingOutput.model
  else
    scp cmslpc41.fnal.gov:/uscms_data/d3/mandal13/MVATraining/TrainingOutput_v1.model TrainingOutput.model
  endif
endif
