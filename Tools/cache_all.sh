#!/bin/bash

shopt -s expand_aliases
alias cachedir='echo "Signature: 8a477f597d28d172789f06886806bc55\n# This file is a cache directory tag.\n# For information about cache directory tags, see:\n#       http://www.brynosaurus.com/cachedir/" > CACHEDIR.TAG'

current=`pwd`

cd $CMSSW_BASE/tmp
cachedir
cd $CMSSW_BASE/src/.git
cachedir
cd $CMSSW_BASE/src/JMEAnalysis
cachedir
cd $CMSSW_BASE/src/PhysicsTools
cachedir
cd $CMSSW_BASE/src/HadronicTau/Tool/condorHadTauLL
cachedir
cd $CMSSW_BASE/src/HadronicTau/Tool/condorCS
cachedir
cd $CMSSW_BASE/src/HadronicTau/Tool/WorkDir
cachedir
cd $CMSSW_BASE/src/RecoMET
cachedir
cd $CMSSW_BASE/src/SusyAnaTools/.git
cachedir
cd $CMSSW_BASE/src/SusyAnaTools/DataFormats
cachedir
cd $CMSSW_BASE/src/SusyAnaTools/SignalScan
cachedir
cd $CMSSW_BASE/src/SusyAnaTools/Skims
cachedir
cd $CMSSW_BASE/src/SusyAnaTools/SkimsAUX
cachedir
cd $CMSSW_BASE/src/SusyAnaTools/StopTreeMaker
cachedir
cd $CMSSW_BASE/src/SusyAnaTools/TopTagger
cachedir
cd $CMSSW_BASE/src/SusyAnaTools/Tools/test
cachedir
cd $CMSSW_BASE/src/SusyAnaTools/Tools/condor
cachedir
cd $CMSSW_BASE/src/TopTagger/.git
cachedir
cd $CMSSW_BASE/src/TopTagger/CfgParser
cachedir
cd $CMSSW_BASE/src/TopTagger/Tools
cachedir
cd $CMSSW_BASE/src/TopTaggerCfgs
cachedir
cd $CMSSW_BASE/src/opencv/.git
cachedir
cd $CMSSW_BASE/src/opencv/3rdparty
cachedir
cd $CMSSW_BASE/src/opencv/CMakeFiles
cachedir
cd $CMSSW_BASE/src/opencv/apps
cachedir
cd $CMSSW_BASE/src/opencv/bin
cachedir
cd $CMSSW_BASE/src/opencv/cmake
cachedir
cd $CMSSW_BASE/src/opencv/data
cachedir
cd $CMSSW_BASE/src/opencv/doc
cachedir
cd $CMSSW_BASE/src/opencv/include
cachedir
cd $CMSSW_BASE/src/opencv/junk
cachedir
cd $CMSSW_BASE/src/opencv/modules
cachedir
cd $CMSSW_BASE/src/opencv/opencv2
cachedir
cd $CMSSW_BASE/src/opencv/platforms
cachedir
cd $CMSSW_BASE/src/opencv/samples
cachedir
cd $CMSSW_BASE/src/opencv/src
cachedir
cd $CMSSW_BASE/src/opencv/test-reports
cachedir
cd $CMSSW_BASE/src/opencv/unix-install
cachedir
cd $CMSSW_BASE/src/recipeAUX
cachedir

cd $current

