### Instructions

## Updating a git repo to a new tag 

Commit all outstanding change (if any) to a new branch.
```
git checkout -b NEW_BRANCH_NAME
```

Fetch the remote changes and check out the new tag 
```
git fetch
git checkout NEW_TAG_NAME
```

If necessary merge your outstanding changes back into the working branch , note that there may be merge conflicts to resolve
```
git checkout -b ANOTHER_NEW_BRANCH_NAME
git merge NEW_BRANCH_NAME
```

You can then compile the SUSYAnaTools
```
cd $CMSSW_BASE/src/SusyAnaTools/Tools/
autoconf
#note if you are not on the LPC you will have to follow the instructions below to install OpenCV on your system
./configure OPENCVDIR=/uscms_data/d3/pastika/zinv/dev/CMSSW_7_4_8/src/opencv/
make
```

## Setup Tools Environment
Go to your working area, then do below
```
source $CMSSW_BASE/src/SusyAnaTools/Tools/setup.csh
```

## TopTagger Code

To Checkout TopTagger Code:

```
## Checkout OpenCV
cd $CMSSW_BASE/src
git clone git@github.com:susy2015/opencv.git
cd opencv
git checkout 3.1.1_BugFix
cmake .
make -j 8

## Checkout Tagtagger
cd $CMSSW_BASE/src
git clone -b HadStopAnaDevel_v8_Moriond2017_Mar27_2017 git@github.com:susy2015/TopTagger.git
scram b -j 8
cd TopTagger/TopTagger/test/
make -j 8 
```

## Check out tagger configuration 
By default SusyAnaTools/Tools/setup.csh will check out the default working point of the tagger configuration. To checkout another working point go to your working area where the code will be run from and run the following command 
```
$CMSSW_BASE/src/TopTagger/Tools/getTaggerCfg.sh -t DESIRED_TAG -d PATH_TO_DOWNLOAD_TO
```
If the legacy AK4 only version of the tagger is also needed run the following to checkout this configuration file
```
$CMSSW_BASE/src/TopTagger/Tools/getTaggerCfg.sh -t Legacy_AK4Only_v0.1.0 -f Legacy_TopTagger.cfg -d PATH_TO_DOWNLOAD_TO
```

Please note that this script will download the deired tagger configuration information in a seperate folder and softlink the necessary files into your corrent directory.  If you have multiple configuration files you work with in the same working directory use the "-f name" option to specify the name of the softlink to the tagger file (the default is TopTagger.cfg).  If you do not want this directory in your working directory you may add the "-d PATH_TO_DOWNLOAD_TO" option to specify where it should download these files.  This is particularly useful if you have several working directories as the script will then only check out each working point once and place softlinks as needed (the model files are rather large so this may save some space if you have many downloaded).  However, "-d" can be omitted and the files will be downloaded in your working directory.  Other configuration file releases can be found here https://github.com/susy2015/TopTaggerCfg/releases
