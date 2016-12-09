setenv LD_LIBRARY_PATH ./:${CMSSW_BASE}/src/opencv/lib/:${CMSSW_BASE}/src/TopTagger/TopTagger/test/:${CMSSW_BASE}/src/SusyAnaTools/Tools/obj/:${LD_LIBRARY_PATH}

## Get the btagging file
if (! -f CSVv2_ichep.csv) then
  ln -s $CMSSW_BASE/src/SusyAnaTools/Tools/CSVv2_ichep.csv .
endif

if (! -f TTbarNoHad_bTagEff.root) then
  ln -s $CMSSW_BASE/src/SusyAnaTools/Tools/TTbarNoHad_bTagEff.root .
endif

## Pileup Reweighting
if (! -f PileupHistograms_Nov17.root) then
  ln -s $CMSSW_BASE/src/SusyAnaTools/Tools/PileupHistograms_Nov17.root .
endif

