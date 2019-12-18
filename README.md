# Instructions


## Setup CMSSW

First setup a CMSSW release. We recommend CMSSW_10_2_9, which has support for uproot. You should do this from a working directory in your ~/nobackup area, which has more storage space on cmslpc (for example, ~/nobackup/SusyAnalysis).

```
mkdir ~/nobackup/SusyAnalysis
cd ~/nobackup/SusyAnalysis
cmsrel CMSSW_10_2_9
cd CMSSW_10_2_9
cmsenv
```

## Get Dependencies

Get json library to use json files in C++.

```
cd $CMSSW_BASE/src
git clone git@github.com:nlohmann/json.git
cd json
git checkout v3.7.3
```


## TopTagger Repos

Checkout and compile the TopTagger and TopTaggerTools repositories. The configure and make commands are also required after merging new TopTagger changes.

```
cd $CMSSW_BASE/src
git clone git@github.com:susy2015/TopTagger.git
git clone git@github.com:susy2015/TopTaggerTools.git
cd TopTagger/TopTagger/test/
./configure
make clean
make -j 8 
```

## SusyAnaTools Repo

Checkout and compile the SusyAnaTools repository. The autoconf, configure, and make commands are also required after merging new SusyAnaTools changes.

```
cd $CMSSW_BASE/src
git clone git@github.com:susy2015/SusyAnaTools.git
cd SusyAnaTools/Tools/
autoconf
./configure
make clean
make -j 8 
```

## Get Configuration Files

### TopTaggerCfg Files

Checkout the top tagger configuration files using the following tag.

Current tag for NanoSUSY ntuples: DeepCombined_fromNanoAOD_RES_T_DeepAK8_T_v1.0.1

You may see all TopTaggerCfg releases/tags with release notes at https://github.com/susy2015/TopTaggerCfg/releases. 

Run this command from your working area, i.e. the directory where softlinks to the configuration and supplementary files will be placed. You can use "-d" to provide the directory where the folder containing the original files will be downloaded. You can use "-o" to overwrite existing softlinks and files. You can provide the full path to TopTagger/TopTagger/scripts/getTaggerCfg.sh, or you can add `TopTagger/TopTagger/scripts` to your PATH.

Command using full path:
```
$CMSSW_BASE/src/TopTagger/TopTagger/scripts/getTaggerCfg.sh -t DeepCombined_fromNanoAOD_RES_T_DeepAK8_T_v1.0.1
```

Command if `TopTagger/TopTagger/scripts` is in your PATH:
```
getTaggerCfg.sh -t DeepCombined_fromNanoAOD_RES_T_DeepAK8_T_v1.0.1
```

Please note that this script will download the desired configuration information in a seperate folder and softlink the necessary files into your corrent directory.
If you do not want this directory in your working directory you may add the "-d PATH_TO_DOWNLOAD_TO" option to specify where it should download these files.
This is particularly useful if you have several working directories as the script will then only check out each working point once and place softlinks as needed.
The model files are rather large so this may save some space if you have many downloaded.
However, "-d" can be omitted and the files will be downloaded in your working directory.

### StopCfg Files

Checkout the stop search configuration files using the following tag.

Current tag for NanoSUSY ntuples: PostProcessed_StopNtuple_V2.9.3

You may see all StopCfg releases/tags with release notes at https://github.com/susy2015/StopCfg/releases. 

Run this command from your working area, i.e. the directory where softlinks to the configuration and supplementary files will be placed. You can use "-d" to provide the directory where the folder containing the original files will be downloaded. You can use "-o" to overwrite existing softlinks and files. You can provide the full path to SusyAnaTools/Tools/scripts/getStopCfg.sh, or you can add `SusyAnaTools/Tools/scripts` to your PATH.

Command using full path:
```
$CMSSW_BASE/src/SusyAnaTools/Tools/scripts/getStopCfg.sh -t PostProcessed_StopNtuple_V2.9.3
```

Command if `SusyAnaTools/Tools/scripts` is in your PATH:
```
getStopCfg.sh -t PostProcessed_StopNtuple_V2.9.3
```

Please note that this script will download the desired configuration information in a seperate folder and softlink the necessary files into your corrent directory.
If you do not want this directory in your working directory you may add the "-d PATH_TO_DOWNLOAD_TO" option to specify where it should download these files.
This is particularly useful if you have several working directories as the script will then only check out each working point once and place softlinks as needed.
However, "-d" can be omitted and the files will be downloaded in your working directory.

## Setup Environment

Go to your working area and then setup the TopTagger environment. This will need to be run after running `cmsenv` in every new terminal session. Use the command for your shell (bash or tcsh). Use `echo $SHELL` to check your shell if you don't know it.

Go to working area (replace myWorkingArea with your working area path):
```
cd $CMSSW_BASE/src/myWorkingArea
```

For bash users:
```
source $CMSSW_BASE/src/TopTagger/TopTagger/test/taggerSetup.sh
```

For tcsh users:
```
source $CMSSW_BASE/src/TopTagger/TopTagger/test/taggerSetup.csh
```


## Check out stop config files

Top checkout StopCfg files, use the script `SusyAnaTools/Tools/scripts/getStopCfg.sh`. Here are the options.
```
Usage:
    getStopCfg.sh -t RELEASE_TAG [-d checkout_directory] [-f cfg_filename] [-o] [-n] [-v]

Options:
    -t RELEASE_TAG :         This is the github release tag to check out (required option)
    -d checkout_directory :  This is the directory where the configuration files will be downloaded to (default: .)
    -a SETS_LINK_NAME_2016 : Specify this option to name the softlink to "sampleSets_PostProcessed_2016.cfg" something other than "sampleSets_PostProcessed_2016.cfg"
    -b SETS_LINK_NAME_2017 : Specify this option to name the softlink to "sampleSets_PostProcessed_2017.cfg" something other than "sampleSets_PostProcessed_2017.cfg"
    -x COLL_LINK_NAME_2016 : Specify this option to name the softlink to "sampleCollections_2016.cfg" something other than "sampleCollections_2016.cfg"
    -y COLL_LINK_NAME_2017 : Specify this option to name the softlink to "sampleCollections_2017.cfg" something other than "sampleCollections_2017.cfg"
    -o :                     Overwrite the softlinks if they already exist
    -n :                     Download files without producing softlinks
    -v :                     increase verbosity: print more stuff... for those who like stuff

Description:
    This script automatically downloads the Stop search configuration files
    and produces softlinks to these files in your current directory. This script should
    be run from the directory where the stop code will be run from. Stop search 
    configuration releases can be browsed at https://github.com/susy2015/StopCfg/releases

```

Stop SUSY configuration releases can be browsed here: https://github.com/susy2015/StopCfg/releases.

Here is the general usage
```
$CMSSW_BASE/src/SusyAnaTools/Tools/scripts/getStopCfg.sh -t DESIRED_TAG -d PATH_TO_DOWNLOAD_TO -o
```
Here is a specific example.

```
mkdir myStopCfgs
$CMSSW_BASE/src/SusyAnaTools/Tools/scripts/getStopCfg.sh -t PostProcess_StopNtuple_v1.0.3 -d myStopCfgs -o
```
The release will be downloaded to the directory provided with the "-d" option (myStopCfgs in this example). The softlinks will be created in the directory where the script is run. We use the -o flag, which means, "Overwrite softlinks and files in my current directory if they already exist."


