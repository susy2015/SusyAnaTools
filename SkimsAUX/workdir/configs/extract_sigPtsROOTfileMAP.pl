#!/usr/bin/perl

if ($#ARGV < 0 || $#ARGV >=1){
   print "\nUsage: extract filename\n";
   print "extract and format information\n\n";

   exit;
}

$file = $ARGV[0];

if(!(-e $file)){
   print "\nplease check the file you give does really exist\n\n";
   exit;
}

open(ORI, "<$file");

@xparArray = ();
@yparArray = ();
@evtCntArray = ();

while(<ORI>){

   if(/.*INFO .* RA2SignalScan\s*bgn\s*xpar :\s*(\d*)\s*ypar :\s*(\d*)\s*nEvtPerPairPars :\s*(\d*)\s*-->\s*(\S*.root)/){
      $bgn_xpar = $1; $bgn_ypar = $2; $bgn_nEvt = $3; $bgn_rtName = $4;
#      print "bgn_xpar : $bgn_xpar  bgn_ypar : $bgn_ypar  bgn_nEvt : $bgn_nEvt  bgn_rtName : $bgn_rtName\n";
   }
   if(/.*INFO .* RA2SignalScan\s*end\s*xpar :\s*(\d*)\s*ypar :\s*(\d*)\s*nEvtPerPairPars :\s*(\d*)\s*-->\s*(\S*.root)/){
      $end_xpar = $1; $end_ypar = $2; $end_nEvt = $3; $end_rtName = $4;
#      print "end_xpar : $end_xpar  end_ypar : $end_ypar  end_nEvt : $end_nEvt  end_rtName : $end_rtName\n";

      if( $bgn_xpar != $end_xpar || $bgn_ypar != $end_ypar || $bgn_rtName ne $end_rtName ){
         print "ERROR ... bgn NOT equal to end  ==>  bgn_xpar : $bgn_xpar  bgn_ypar : $bgn_ypar  <-->  end_xpar : $end_xpar  end_ypar : $end_ypar\n";
      }else{
         $evtCnt = $end_nEvt - $bgn_nEvt + 1;
         $xpar = $bgn_xpar; $ypar = $bgn_ypar; $rtName = $bgn_rtName;
#         print "xpar : $xpar  ypar : $ypar  evtCnt : $evtCnt  rtName : $rtName\n";

         $found =0;
         for($i =0; $i<= $#evtCntArray; $i++){
            if($xparArray[$i] == $xpar && $yparArray[$i] == $ypar ){
               $evtCntArray[$i] += $evtCnt;
               $found =1;
            }
            if( $found ){ last; }
         }
         if( !$found ){
            push (@xparArray, $xpar);
            push (@yparArray, $ypar);
            push (@evtCntArray, $evtCnt);
         }
      }
   }
}

#for($i=0; $i<=$#evtCntArray; $i++){
#   printf ("xpar : %4d  ypar : %4d  evtCnt : %6d\n", $xparArray[$i], $yparArray[$i], $evtCntArray[$i]);
#}

for($i=0; $i<=$#evtCntArray; $i++){

   printf ("xpar : %4d  ypar : %4d  evtCnt : %6d\n", $xparArray[$i], $yparArray[$i], $evtCntArray[$i]);
   @rtNameArray = ();

   seek ORI, 0, 0;
   while(<ORI>){
      if(/.*INFO .* RA2SignalScan\s*bgn\s*xpar :\s*(\d*)\s*ypar :\s*(\d*)\s*nEvtPerPairPars :\s*(\d*)\s*-->\s*dcap.*(\/store\S*.root)/){
         $bgn_xpar = $1; $bgn_ypar = $2; $bgn_nEvt = $3; $bgn_rtName = $4;
         if( $bgn_xpar == $xparArray[$i] && $bgn_ypar == $yparArray[$i] ){
            $found =0;
            for($j =0; $j<=$#rtNameArray; $j++){
               if( $rtNameArray[$j] eq $bgn_rtName ){
                  $found =1;
               }
               if( $found ){ last; }
            }
            if( !$found ){
               push (@rtNameArray, $bgn_rtName);
            }
         }       
      }
   }

   for($j=0; $j<=$#rtNameArray; $j++){
         print "    $rtNameArray[$j]\n";
   }

   print "\n\n";
}

close(ORI);
