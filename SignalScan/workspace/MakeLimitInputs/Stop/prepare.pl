#!/usr/bin/perl
use Cwd;

if($#ARGV <0 ){
   print "\nUsage: prepare.pl topology dir_to_signalCards_topology dir_to_DataCards\n";
   print "topology: topology keyword\n";
   print "dir_to_signalCards_topology : /xxx/yyy/signalCards/topology\n";
   print "dir_to_DataCards : relative directory DataCards\n\n";
   exit;
}

if($#ARGV ==2 ){
  
   $topology = $ARGV[0];
   $signalCardsDir = $ARGV[1];
   $dataCardsDir = $ARGV[2];

   if( $signalCardsDir =~ m/(\S*)\/$/ ){
      $signalCardsDir = $1;
   }

   if( $dataCardsDir =~ m/(\S*)\/$/ ){
      $dataCardsDir = $1;
   }

   $cwd = getcwd;
   print "\ncwd : $cwd\n";

   $releaseDir = $ENV{'CMSSW_BASE'};

   print "topology : $topology  ==> signalCardsDir : $signalCardsDir\n";

   system("make");

   $oriDataCards = $topology."_ori_DataCards";
   $ln_signalCards = "ln -s $signalCardsDir $oriDataCards";
   system($ln_signalCards);

   opendir($DIR, $oriDataCards) || "can't opendir $oriDataCards: $!";
   @signalfiles = grep { !/^\.+$/ } readdir($DIR);
   closedir $DIR;

   open(BATCH, ">batch_$topology.txt");
   foreach $signal (@signalfiles){
      if($signal =~ m/(.*)(idx\d*)(.*dat)/){
         $prodInput = "./mkInputs -d $dataCardsDir/new_data.txt -q $dataCardsDir/new_qcd.txt -t $dataCardsDir/new_tau.txt -l $dataCardsDir/new_lostlepton.txt -z $dataCardsDir/new_zinvisible.txt -i $2 -s $oriDataCards/$1$2$3";
         system($prodInput);
         print BATCH "$prodInput\n";
      }
   }
   close(BATCH);
   system("mv limits limits_$topology");
   system("tar -czvf limits_$topology.tgz limits_$topology");

   open(SINGLE, ">single_point_batch_$topology.txt");
   foreach $signal (@signalfiles){
      if($signal =~ m/(.*)(idx\d*)(.*dat)/){
         $singleProdInput = "./mkInputs --single -d $dataCardsDir/new_data.txt -q $dataCardsDir/new_qcd.txt -t $dataCardsDir/new_tau.txt -l $dataCardsDir/new_lostlepton.txt -z $dataCardsDir/new_zinvisible.txt -i $2 -s $oriDataCards/$1$2$3";
         system($singleProdInput);
         print SINGLE "$singleProdInput\n";
      }
   }
   close(SINGLE);
   system("mv limits singleChannel_limits_$topology");
   system("tar -czvf singleChannel_limits_$topology.tgz singleChannel_limits_$topology");
   
   chdir($cwd);

   print "\n\n Please transfer the two tgz files to kodiak. Then continue the recipe from kodiak.\n";
   print "The singleChannel ones are going to be used to evaluate limits for each channel.\n";

}
