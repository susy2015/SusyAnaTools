#!/usr/bin/perl

if ($#ARGV <= 0 || $#ARGV >6){
   print "\nUsage: createCRAB.pl keyword fulldatasetname unitperjob pubname outdirBase crabTEMPname json\n\n";
   exit;
}

$keyword = $ARGV[0];
$dataset = $ARGV[1];
$unitperjob = $ARGV[2];
$pubname = $ARGV[3];
$outdirBase = $ARGV[4];
$crabtmp = $ARGV[5];
$json = $ARGV[6];

if(-e $keyword){
   print "\nplease check a directory exists with the keyword you give\n\n";
   exit;
}

system("mkdir $keyword");
system("cp treeMaker_stopRA2.py $keyword");
system("cp *.db $keyword");
system("cp Cert_*JSON.txt $keyword");

open(TMP, "<$crabtmp");

$outfile = $keyword."/crab3Confg.py";

open(OUT, ">$outfile");

while(<TMP>){

   if(/(.*) REQUESTNAME/){
      print OUT "$1 '$keyword'\n";
      next;
   }

   if(/(.*) DATASETNAME/){
      print OUT "$1 '$dataset'\n";
      next;
   }

   if(/(.*) UNITPERJOB/){
      print OUT "$1 $unitperjob\n";
      next;
   }

   if(/(.*) PUBDATANAME/){
      $fullpubname = $keyword."_".$pubname;
      print OUT "$1 '$fullpubname'\n";
      next;
   }

   if(/(.*) JSON/){
      print OUT "$1 '$json'\n";
      next;
   }

   if(/(.*) OUTLFNDIRBASE/){
      $outdirfull = '/store/group/lpcsusyhad/'.$outdirBase;
      print OUT "$1 '$outdirfull'\n";
      next;
   }

   print OUT $_;

}

close(TMP);
close(OUT);
