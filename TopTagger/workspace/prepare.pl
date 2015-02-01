#!/usr/bin/perl
use Cwd;

$cwd = getcwd;
print "\ncwd : $cwd\n\n";

$releaseDir = $ENV{'CMSSW_BASE'};

$topheadH = $releaseDir."/src/SusyAnaTools/TopTagger/workspace/configs/tophead.h";
$tdrstyleC = $releaseDir."/src/SusyAnaTools/DataFormats/aux/tdrstyle.C";
$rootlogonC = $releaseDir."/src/SusyAnaTools/TopTagger/workspace/configs/rootlogon.C";
$combinationH = $releaseDir."/src/SusyAnaTools/TopTagger/interface/combination.h";
$Type3TopTaggerH = $releaseDir."/src/SusyAnaTools/TopTagger/interface/Type3TopTagger.h";
$indexSortH = $releaseDir."/src/SusyAnaTools/TopTagger/interface/indexSort.h";
$recipeAUX = $releaseDir."/src/recipeAUX";

$ln_topheadH = "ln -s $topheadH .";
$ln_tdrstyleC = "ln -s $tdrstyleC .";
$ln_rootlogonC = "ln -s $rootlogonC .";
$ln_combinationH = "ln -s $combinationH .";
$ln_Type3TopTaggerH = "ln -s $Type3TopTaggerH .";
$ln_indexSortH = "ln -s $indexSortH .";
$ln_recipeAUX = "ln -s $recipeAUX .";

system($ln_topheadH);
system($ln_tdrstyleC);
system($ln_rootlogonC);
system($ln_combinationH);
system($ln_Type3TopTaggerH);
system($ln_indexSortH);
system($ln_recipeAUX);
