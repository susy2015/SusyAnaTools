#!/usr/bin/perl

if($#ARGV <0 ){
   print "\nUsage: processPickBins.pl input binsToPick\n";
   print "input: input file\n";
   print "binsToPick: input contains bins to be picked out (starting from 1)\n\n";
   exit;
}

if($#ARGV ==1 ){

   $input = $ARGV[0];
   $binsToPick = $ARGV[1];

   @inputsSplit = split(/\//, $input);

   $outfile = "new_".$inputsSplit[$#inputsSplit];
   print "outfile : $outfile\n";
 
   @binsArray = ();  
   open(PICK, "<$binsToPick");
   while(<PICK>){
#      my @values = split(/[\s;:,\s\/]+/, $_);
      my @values = $_=~/\d+/g;
      @binsArray = (@binsArray, @values); 
   }
   print "picking the following bins : @binsArray\n";

   @realBinsArray = ();

   open(OUT, ">$outfile");

   open(IN, "<$input");
   while(<IN>){
      if( /channels\s+=\s+(\d*)(\s*.*)/ ){
         foreach $perbin (@binsArray){
            if( $perbin <= $1 && $perbin > 0 ){
               push(@realBinsArray, $perbin);
            }
         }
         @realBinsArray = sort @realBinsArray;
         print "removing un-physical bins & sorting : @realBinsArray\n";
         my $realChn = $#realBinsArray+1;
         print OUT "channels = $realChn $2\n";
      }elsif( /channel\s+=\s+(.*)/ ){
         print OUT "channel =  ";
         for($i=0; $i<$#realBinsArray+1; $i++){
            $realBin = $i+1;
            if( $i != $#realBinsArray ){
               print OUT "bin$realBin;  ";
            }else{
               print OUT "bin$realBin";
            }
         }
         print OUT "\n";
      }elsif( /(\S*_events)\s+=\s+(.*)/ ){
         my @values = split(/[\s]+/, $2);
         print OUT "$1 =  ";
         foreach $id (@realBinsArray){
            print OUT "$values[$id-1]  ";
         }
         print OUT "\n";
      }elsif( /(\S*_uncertainty\S*_\S*)\s+=\s+(.*\d+.*)/ ){
         my @values = split(/[\s]+/, $2);
         print OUT "$1 =  ";
         foreach $id (@realBinsArray){
            print OUT "$values[$id-1]  ";
         }
         print OUT "\n";
      }else{
         print OUT "$_";
      }
   }

   close(OUT);
}
