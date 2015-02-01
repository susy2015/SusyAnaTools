#!/usr/bin/perl
use Cwd;

if($#ARGV <0 ){
   print "\nUsage: prepare.pl topology dir_to_prodSignal_merged_root_file\n";
   print "topology: topology keyword\n";
   print "dir_to_prodSignal_merged_root_file : /xxx/yyy/zzz/prodSignal_merged.root\n\n";
   exit;
}

if($#ARGV ==1 ){
  
   $topology = $ARGV[0];
   $fileDir = $ARGV[1];

   $cwd = getcwd;
   print "\ncwd : $cwd\n";

   $releaseDir = $ENV{'CMSSW_BASE'};
   $ln_topheadH = "ln -s $releaseDir/src/UserCode/SignalScan/workspace/configs/tophead.h .";
   $ln_tdrstyleC = "ln -s $releaseDir/src/UserCode/DataFormats/aux/tdrstyle.C .";
   $ln_PUdata = "ln -s $releaseDir/src/RA2Classic/AdditionalInputFiles/DataPileupHistogram_RA2Summer12_190456-203002_ABC.root PUData_dist.root";
   $ln_reference_xSecROOT = "ln -s $releaseDir/src/UserCode/SignalScan/workspace/configs/reference_xSec.root .";
   $ln_prodLimitInputsC = "ln -s $releaseDir/src/UserCode/SignalScan/workspace/configs/prodLimitInputs.C .";
   $ln_plotTopologies_T1T2C = "ln -s $releaseDir/src/UserCode/SignalScan/workspace/configs/plotTopologies_T1T2.C .";

   print "topology : $topology  ==> fileDir : $fileDir\n";
   print "linking  tophead.h  --> $ln_topheadH\n";
   system($ln_topheadH);
   print "linking  tdrstyle.C  --> $ln_tdrstyleC\n";
   system($ln_tdrstyleC);
   print "linking  PUdata  --> $ln_PUdata\n";
   system($ln_PUdata);
   print "linking reference_xSec.root  --> $ln_reference_xSecROOT\n";
   system($ln_reference_xSecROOT);
   print "linking  prodLimitInputs.C  --> $ln_prodLimitInputsC\n";
   system($ln_prodLimitInputsC);
   print "linking  plotTopologies_T1T2.C  --> $ln_plotTopologies_T1T2C\n";
   system($ln_plotTopologies_T1T2C);

   $ln_topoFile = sprintf("ln -s $fileDir RA2SMHists_Topology%s_EffJesJerPdfIsr.root", $topology);
   print "linking  prodSignal_merged.root  --> $ln_topoFile";
   system($ln_topoFile);

   system("mkdir Unc_$topology");

   chdir($cwd);

   print "\n\n1. Modify the plotTopologies_T1T2.C file, adding plotAll$topology() function if it does not exist\n";
   print "2. root -l \n";
   print "3. In root command line: .L plotTopologies_T1T2.C\n";
   print "4. In root command line:  plotAll$topology()\n";
   print "5. cd Unc_$topology\n";
   print "6. hadd all the EffUnc_*.root to one single file: hadd EffUnc_SMS_$topology.root EffUnc_*NJet*_*HT*_*MHT*_$topology.root >& log_merged.lg&\n";
   print "7. cd ../\n";
   print "8. ln -s Unc_$topology/EffUnc_SMS_$topology.root .\n";
   print "9. Manually change one line in prodLimitInputs.C file --> void prodLimitInputs(const std::string topo=\"$topology\"){\n";
   print "10. root -q -b -l prodLimitInputs.C++\n";
   print "   !!!!!!The signal cards will be in $topology directory\n";

}
