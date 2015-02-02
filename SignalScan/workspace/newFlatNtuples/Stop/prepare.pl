#!/usr/bin/perl
use Cwd;

if($#ARGV <0 ){
   print "\nUsage: prepare.pl topology fileDir\n";
   print "topology: topology keyword\n";
   print "fileDir : directory to the signal flat ntuples\n\n";
   exit;
}

if($#ARGV ==1 ){
  
   $topology = $ARGV[0];
   $fileDir = $ARGV[1];

   if( $fileDir =~ m/(\S*)\/$/ ){
      $fileDir = $1;
   }
  
   $cwd = getcwd;
   print "\ncwd : $cwd\n";

   $releaseDir = $ENV{'CMSSW_BASE'};
   $ln_topheadH = "ln -s $releaseDir/src/UserCode/SignalScan/workspace/configs/tophead.h .";
   $ln_tdrstyleC = "ln -s $releaseDir/src/UserCode/DataFormats/aux/tdrstyle.C .";
   $ln_PUdata = "ln -s $releaseDir/src/RA2Classic/AdditionalInputFiles/DataPileupHistogram_RA2Summer12_190456-203002_ABC.root PUData_dist.root";
   $ln_prodSignalHistC = "ln -s $releaseDir/src/UserCode/SignalScan/workspace/configs/prodSignalHist.C .";

   $ln_BTagEff_ROOT = "ln -s $releaseDir/src/UserCode/SignalScan/workspace/configs/BTagEfficiency_mother600_LSP25to150.root .";
   $ln_ISRWeights_ROOT = "ln -s $releaseDir/src/UserCode/SignalScan/workspace/configs/ISRWeightsUncFromRoberto.root .";

   print "topology : $topology  ==> fileDir : $fileDir\n";
   print "linking  tophead.h  --> $ln_topheadH\n";
   system($ln_topheadH);
   print "linking  tdrstyle.C  --> $ln_tdrstyleC\n";
   system($ln_tdrstyleC);
   print "linking  PUdata  --> $ln_PUdata\n";
   system($ln_PUdata);
   print "linking  prodSignalHist.C  --> $ln_prodSignalHistC\n";
   system($ln_prodSignalHistC);

   $topoDir = $cwd."/topo_".$topology;
   print "making directory $topoDir\n";
   system("mkdir $topoDir");
   chdir($topoDir);

   $ln_local_topheadH = "ln -s ../tophead.h .";
   system($ln_local_topheadH);
   $ln_local_tdrstyleC = "ln -s ../tdrstyle.C .";
   system($ln_local_tdrstyleC);
   $ln_local_prodSignalHistC = "ln -s ../prodSignalHist.C .";
   system($ln_local_prodSignalHistC);
   $ln_local_PUdata = "ln -s ../PUData_dist.root .";
   system($ln_local_PUdata);
   
   $ln_fileDir = "ln -s $fileDir .";
   system($ln_fileDir);
   
   opendir($DIR, $fileDir) || "can't opendir $fileDir: $!";
   @flatNtuples = grep { !/^\.+$/ } readdir($DIR);
   closedir $DIR;
   
   $splitPerNumFiles = 50;
   $nSplit = int(($#flatNtuples+1)/$splitPerNumFiles);
   if( $#flatNtuples+1 != $nSplit*$splitPerNumFiles ){ $nSplit++; }
   $nFlatFiles = $#flatNtuples+1;
   print "nFlatFiles : $nFlatFiles  splitPerNumFiles : $splitPerNumFiles   nSplit : $nSplit\n";

   $outfilename = $topology."_GOOD.txt";
   open(OUT, ">$outfilename");
   foreach $file (@flatNtuples){
      if( $fileDir =~ m/.*pnfs(\S*)/ ){
         $readableFile = "dcap://cmsdca.fnal.gov:24136/pnfs/fnal.gov/usr/$1/".$file;
      }else{
         $readableFile = $fileDir."/".$file;
      }
      print OUT "$readableFile\n";
   }
   close(OUT);

   $splitCommand = "split -d -l $splitPerNumFiles $outfilename $outfilename.";
   print "splitCommand : $splitCommand\n";
   system($splitCommand);
   
   for($is=0; $is<$nSplit; $is++){
      $splitDir = sprintf("$topoDir/split%02d", $is);
      system("mkdir $splitDir");
      chdir($splitDir);
      $ln_rootlist = sprintf("ln -s ../$outfilename.%02d rootlist.txt", $is);
      system($ln_rootlist);
      system($ln_local_topheadH);
      system($ln_local_tdrstyleC);
      system($ln_local_prodSignalHistC);
      system($ln_local_PUdata);
      system($ln_BTagEff_ROOT);
      system($ln_ISRWeights_ROOT);
      chdir($topoDir);
   }

   chdir($cwd);

   print "\n\n1. Enter into each split directory and run : root -q -b -l prodSignalHist.C++ >& log_run.lg&\n";
   print "2. hadd all the files in each split directory into one single file : hadd prodSignal_merged.root split*/prodSignal.root\n";

}
