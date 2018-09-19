### Instructions

## TopTagger Code

To Checkout TopTagger Code:

```
cmsrel CMSSW_9_4_4
cd CMSSW_9_4_4
cmsenv

## Checkout Tagtagger
cd $CMSSW_BASE/src
git clone -b d1840ab83aa3e8e81ba1a463e22ec7c003f1e355 git@github.com:susy2015/TopTagger.git
cd TopTagger/TopTagger/test/
./configure OPENCVDIR=/uscms_data/d3/pastika/zinv/dev/CMSSW_7_4_8/src/opencv/
make -j 8 
```

## SUSYAnaTools Code

You can then compile the SUSYAnaTools
```
cd $CMSSW_BASE/src
git clone -b AddNewTagger git@github.com:susy2015/SusyAnaTools.git

cd $CMSSW_BASE/src/SusyAnaTools/Tools/
autoconf
#note if you are not on the LPC you will have to follow the instructions below to install OpenCV on your system
./configure OPENCVDIR=/uscms_data/d3/pastika/zinv/dev/CMSSW_7_4_8/src/opencv/
make -j 8 
```

## Setup Tools Environment
Go to your working area, then do below
```
source $CMSSW_BASE/src/SusyAnaTools/Tools/setup.csh
```


