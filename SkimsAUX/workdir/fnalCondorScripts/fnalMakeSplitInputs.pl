#!/usr/bin/perl

if ($#ARGV < 0){
   print "\nUsage: splitInput numFilePerSnippet\n";
   print "split the inputList.txt into files with numFilePerSnippet each. \n\n";
   exit;
}

if($#ARGV ==0 ){
   
   $perNumFile = $ARGV[0];
   print "perNumFile : $perNumFile\n";

   open(IN, "inputList.txt");
   $cnt = -1;
   $appendix = 0;

   while(<IN>){
      $cnt ++;
      if( $cnt % $perNumFile ==0 ){
         if( $cnt !=0 ){ close(OUT); }
         $outFileName = sprintf("inputList_%d.txt", $appendix);
         open(OUT, ">$outFileName");
         $appendix++;
      }
      print OUT $_;
   }
   close(OUT);
}
