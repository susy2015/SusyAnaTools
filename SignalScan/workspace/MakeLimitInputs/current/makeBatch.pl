#!/usr/bin/perl

if($#ARGV <0 ){
   print "\nUsage: makeBatch keyword\n";
   print "keyword: topology keyword, e.g., T1\n\n";
   exit;
}

if($#ARGV ==0 ){

   $keyword = $ARGV[0];

   $dir = $keyword."_ori_DataCards";
   $outfile = "batch_".$keyword.".txt";

   $command = "ls -l --color=never -v ".$dir."/*.dat | sed \"s:.*\\(T.*ori_DataCards/mSUGRA.*idx\\)\\([0-9][0-9]*\\).dat:./mkInputs -d DataCards/data.txt -q DataCards/qcd.txt -t DataCards/tau.txt -l DataCards/lostlepton.txt -z DataCards/zinvisible.txt -i idx\\2 -s \\1\\2.dat:\" > ".$outfile;

   print "command: $command\n";
   system($command);

}
