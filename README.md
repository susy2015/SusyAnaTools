### Instructions

## TopTagger Code

To Checkout TopTagger Code:

```
cmsrel CMSSW_9_4_4
cd CMSSW_9_4_4
cmsenv

## Checkout Tagtagger
cd $CMSSW_BASE/src
git clone git@github.com:susy2015/TopTagger.git
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

<details> <summary> Checkout OpenCV </summary>

```bash
cd $CMSSW_BASE/src
git clone git@github.com:susy2015/opencv.git
cd opencv
git checkout 3.1.0_StopBugFix
cmake .
make -j 8
```

</details>


## Setup Tools Environment
Go to your working area, then do below
```
source $CMSSW_BASE/src/SusyAnaTools/Tools/setup.csh
```

Please note that this script will download the desired tagger configuration information in a seperate folder and softlink the necessary files into your corrent directory.
If you have multiple configuration files you work with in the same working directory use the "-f name" option to specify the name of the softlink to the tagger file (the default is TopTagger.cfg).
If you do not want this directory in your working directory you may add the "-d PATH_TO_DOWNLOAD_TO" option to specify where it should download these files.
This is particularly useful if you have several working directories as the script will then only check out each working point once and place softlinks as needed (the model files are rather large so this may save some space if you have many downloaded).
However, "-d" can be omitted and the files will be downloaded in your working directory.
Other configuration file releases can be found here https://github.com/susy2015/TopTaggerCfg/releases

## Check out stop config files

Here are the options for the script `SusyAnaTools/Tools/scripts/getStopCfg.sh`. The script is based on `TopTagger/Tools/getTaggerCfg.sh`, and thus has the same options and functionality.
```
$CMSSW_BASE/src/SusyAnaTools/Tools/scripts/getStopCfg.sh -h 

Usage:
    getStopCfg.sh -t RELEASE_TAG [-d checkout_directory] [-f cfg_filename] [-n]

Options:
    -t RELEASE_TAG :         This is the github release tag to check out (required option)
    -d checkout_directory :  This is the directory where the configuration files will be downloaded to (default: .)
    -s SETS_LINK_NAME :      Specify this option to name the softlink to "sampleSets.cfg" something other than "sampleSets.cfg"
    -c COLL_LINK_NAME :      Specify this option to name the softlink to "sampleCollections.cfg" something other than "sampleCollections.cfg"
    -o :                     Overwrite the softlinks if they already exist
    -n :                     Download files without producing softlinks

Description:
    This script automatically downloads the Stop search configuration files
    and produces a softlink to this files in your corrent directory. This script should
    be run from the directory where the stop code will be run from. Stop search 
    configuration releases can be browsed at https://github.com/susy2015/StopCfg/releases.
```

Here is a general example.
```
$CMSSW_BASE/src/SusyAnaTools/Tools/scripts/getStopCfg.sh -t DESIRED_TAG -d PATH_TO_DOWNLOAD_TO
```

Here is a specific example for the CMSSW8028_2016_v1.0.1 tag.
We create softlinks for sampleSets.cfg and sampleCollections.cfg in our current directory.
We use the -o flag, which means, "Overwrite the softlinks if they already exist."
The -f flag specifies the name of the softlink.
```
$CMSSW_BASE/src/SusyAnaTools/Tools/scripts/getStopCfg.sh -t CMSSW8028_2016_v1.0.1 -d . -s sampleSets.cfg -c sampleCollections.cfg -o
```
Actually, for our example, we could omit the "-d ." option as we are downloading the files in our current directory (which is the default).

Stop SUSY configuration releases can be browsed here: https://github.com/susy2015/StopCfg/releases

