//create input files for different limit tools
//Christian Autermann, Universität Hamburg

//system
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <time.h>
#include <stdlib.h>
#include <exception>
#include <algorithm>

#include <sys/stat.h>

#include "ConfigFile.h"
#include "mkInputs.h"
#include "anyoption.h"

std::string idxStr_;
const double trigger_uncertainty = 0.02; //2% trigger uncertainty

std::string ToString(double d, std::string s=""){
  if (d==0&&s!="") return s;
  std::stringstream ss;
  ss << d;
  return ss.str();
}

bool StrContains(std::string str,std::string str2){
  std::transform(str.begin(), str.end(),str.begin(), ::toupper);
  return (str.find(str2)!=string::npos);
}

void points::WriteHiggsInputSingleChannel(const std::string dir, const std::string prefix) {
    using namespace Table;
    using namespace std;
    for (vector<point>::iterator it = p_.begin(); it != p_.end(); ++it) {
       int n_nonzero_channels=0;
       for (int bin=0; bin<(int)it->bins.size(); ++bin) {
	 if (it->bins[bin].signal.eventyield - it->bins[bin].signal.contamination >0) ++n_nonzero_channels;
	 else continue;
	 unsigned b=bin;
  
         ofstream ofile;
         stringstream ss;
         struct stat stFile; if( stat(dir.c_str(), &stFile) == -1 ){ std::system(((string)"mkdir "+dir).c_str()); }
         ss << dir << "/"<<prefix<<"_" << it->cmssm.mzero << "_" << it->cmssm.mhalf << "_"
          	         << it->cmssm.tanbeta << "_" << it->cmssm.azero << "_" << it->cmssm.mu<<"_ch"<<b<<"_"<<idxStr_.c_str()<<".txt";

	 ofile.open(ss.str().c_str());
	 ofile << "# Mzero = " << it->cmssm.mzero << "\n";
	 ofile << "# Mhalf = " << it->cmssm.mhalf << "\n";
	 ofile << "# TanBeta = " << it->cmssm.tanbeta << "\n";
	 ofile << "# Xsection.LO = " << it->cmssm.xsec << "\n";
	 ofile << "# Xsection.NLO = " << it->cmssm.xsecNLO << "\n";
	 ofile << "# Luminosity = " << it-> lumi << "\n";
	 ofile << "# Channel = "<<b<<"\n";

//	 cout << "M0 = "<<it->cmssm.mzero<<", M1/2 ="<<it->cmssm.mhalf << ", Channel = "<< b <<endl;

	 int n_backgrounds = it->bins[0].backgds.size();
	 int n_nuisance	 = 0;
	 for (std::map<std::string,std::vector<std::string> >::const_iterator u=it->uncertainties.begin();
              u!=it->uncertainties.end(); ++u) 
	       ++n_nuisance;   

	 ofile << "# n_channels = 1 \n";
         ofile << "# "<<it->bins[b].data.label<<"_0 = "<<it->bins[b].data.eventyield<<"\n";

         ofile << "# "<<it->bins[b].signal.label<<"_0 = "<<it->bins[b].signal.eventyield<<"\n";
         ofile << "# "<<it->bins[b].signal.label<<"_contamination_0 = "<<it->bins[b].signal.contamination<<"\n";
	 for (int sample=0; sample<n_backgrounds;++sample) 
           ofile << "# "<<it->bins[b].backgds[sample].label<<"_0 = "
	         << it->bins[b].backgds[sample].eventyield<<"\n";


	 ofile << "imax  1  number of channels" << endl;
	 ofile << "jmax " << setw(2) << n_backgrounds << "  number of backgrounds" << endl;
	 ofile << "kmax " << setw(2) << n_nuisance    
               << "  number of nuisance parameters (sources of systematic uncertainties)" << endl;
	 ofile << "------------" << endl;  

	 //observed events in all channels
	 TTable observed("## observed events");
	 observed.SetStyle(Empty);
	 observed.SetDelimiter("  ");
	 observed.AddColumn<string>(""); 
	 observed.AddColumn<int>("");
	 observed << "bin" << 0;
	 observed << "observation" << (int)it->bins[b].data.eventyield;
	 ofile << observed << "------------\n" << endl;  

	 //expected events in all channels for signal and all backgrounds
	 TTable exp("## expected events");
	 exp.SetStyle(Empty);
	 exp.SetDelimiter("  ");
	 exp.AddColumn<string>(""); 
	 exp.SetMinumumWidth(20,0);//make first column at least 20 chars
         for (int sample=1; sample<=n_backgrounds+1; ++sample) 
               exp.AddColumn<string>("");
	 exp << "bin"; 
         for (int sample=1; sample<=n_backgrounds+1; ++sample) {
        	stringstream ss;
        	ss << 0;
        	exp << ss.str();
	 }   
	 exp << "process";
         exp << it->bins[b].signal.label;
	 for (int sample=0; sample<n_backgrounds;++sample) 
             exp << it->bins[b].backgds[sample].label;
	 exp << "process"; 
         for (int sample=1; sample<=n_backgrounds+1; ++sample) {
        	stringstream ss;
        	ss << (sample-1);
        	exp << ss.str();
         }
	 exp << "rate"; 
         exp << ToString(it->bins[b].signal.eventyield - it->bins[b].signal.contamination);
	 for (int sample=0; sample<n_backgrounds;++sample) 
              exp << ToString(it->bins[b].backgds[sample].eventyield);

	 ofile << exp << "------------" << std::endl;  
	 TTable sys("");
	 sys.SetStyle(Empty);
	 sys.SetDelimiter("  ");
	 sys.AddColumn<string>(""); 
	 sys.SetMinumumWidth(20,0);//make first column at least 20 chars
         for (int sample=1; sample<=n_backgrounds+1; ++sample) 
               sys.AddColumn<string>("");
	 for (std::map<std::string,std::vector<std::string> >::const_iterator u=it->uncertainties.begin();
              u!=it->uncertainties.end(); ++u) {
	    sys << u->first;
	      bool foundSyst=false;
              for (int unc=0; unc<(int)it->bins[b].signal.uncert.size();++unc){
		string unc_name( it->bins[b].signal.uncert[unc].label );
        	std::replace(unc_name.begin(), unc_name.end(), ' ', '_'); 
        	if (unc_name+" "+it->bins[b].signal.uncert[unc].distribution ==u->first){
	          sys << ToString(it->bins[b].signal.GetFactorialUncertainty(unc),"-"); // signal
                  it->syst[unc_name] += (it->bins[b].signal.GetFactorialUncertainty(unc)-1)*
		                     it->bins[b].signal.eventyield;

		  foundSyst=true;
        	}
	      }
	      if (!foundSyst) sys << "-";
	      for (int sample=0; sample<n_backgrounds; ++sample) { 
		foundSyst=false; 
        	for (int unc=0; unc<(int)it->bins[b].backgds[sample].uncert.size();++unc){
	          string unc_name( it->bins[b].backgds[sample].uncert[unc].label );
                  std::replace(unc_name.begin(), unc_name.end(), ' ', '_'); 
                  if (unc_name+" "+it->bins[b].backgds[sample].uncert[unc].distribution ==u->first){
	            sys << ToString(it->bins[b].backgds[sample].GetFactorialUncertainty(unc),"-"); // background 'sample'
                    it->syst[unc_name] += (it->bins[b].backgds[sample].GetFactorialUncertainty(unc)-1)*
		                           it->bins[b].backgds[sample].eventyield;
 		    foundSyst=true;
	          }
		}
		if (!foundSyst) sys << "-";
	      }
	    
	 }
	 ofile << sys << "------------" << std::endl;  

	 for (std::map<std::string, double>::const_iterator sys=it->syst.begin();sys!=it->syst.end();++sys){
           ofile << "# "<<sys->first<<" = "<<sys->second<<"\n";
	 } 
	 it->syst.clear();
	 //cout << "WRITE m0:" << it->cmssm.mzero << ", m1/2:" << it->cmssm.mhalf << endl;
	 ofile.close();
      }
   }
}


//====================================================================================================
//====================================================================================================
void points::WriteHiggsInputStatUncorr(const std::string dir, const std::string prefix) {
    using namespace Table;
    using namespace std;
    for (vector<point>::iterator it = p_.begin(); it != p_.end(); ++it) {
       //create result file
       ofstream ofile;
       stringstream ss;
       struct stat stFile; if( stat(dir.c_str(), &stFile) == -1 ){ std::system(((string)"mkdir "+dir).c_str()); }
       ss << dir << "/"<<prefix<<"_" << it->cmssm.mzero << "_" << it->cmssm.mhalf << "_"
        	       << it->cmssm.tanbeta << "_" << it->cmssm.azero << "_" << it->cmssm.mu<<"_"<<idxStr_.c_str()<<".txt";
       ofile.open(ss.str().c_str());
       //write general SUSY parameter information
       ofile << "# Mzero = " << it->cmssm.mzero << "\n";
       ofile << "# Mhalf = " << it->cmssm.mhalf << "\n";
       ofile << "# TanBeta = " << it->cmssm.tanbeta << "\n";
       ofile << "# Xsection.LO = " << it->cmssm.xsec << "\n";
       ofile << "# Xsection.NLO = " << it->cmssm.xsecNLO << "\n";
       ofile << "# Luminosity = " << it-> lumi << "\n";
       
       /*
       cout<<"Uncertainties among samples are correlated as follows:"<<endl;
       for (std::map<std::string,std::vector<std::string> >::const_iterator u=it->uncertainties.begin();
            u!=it->uncertainties.end(); ++u) {
	    cout<<"   "<<u->first<<": ";
	    for (std::vector<std::string>::const_iterator s=u->second.begin();s!=u->second.end();++s)
	      cout<<"  "<<*s;
	    cout<<endl;  
       }  
       */    
//       cout << "M0 = "<<it->cmssm.mzero<<", M1/2 ="<<it->cmssm.mhalf << endl;
       
       int n_channels	 = it->n_bins;
       int n_nonzero_channels=0;
       int n_backgrounds = it->bins[0].backgds.size();
       int n_nuisance	 = 0;
       for (int bin=0; bin<n_channels; ++bin) 
	   if (it->bins[bin].signal.eventyield - it->bins[bin].signal.contamination>0) ++n_nonzero_channels;
       for (std::map<std::string,std::vector<std::string> >::const_iterator u=it->uncertainties.begin();
            u!=it->uncertainties.end(); ++u) { 
	 if (StrContains(u->first, "SIGNAL_SYST_KFACTOR") || StrContains(u->first, "SIGNAL_SYST_XSEC"))
	     continue; 
          if (StrContains( u->first, "STAT") && !StrContains( u->first, "QCD"))
             n_nuisance += n_nonzero_channels;
	  else
	     ++n_nuisance;   
       }
	     
       ofile << "# n_channels = " << n_nonzero_channels << "\n";
       for (int bin=0; bin<n_channels; ++bin) {
         if (it->bins[bin].signal.eventyield - it->bins[bin].signal.contamination<=0) continue;
         ofile << "# "<<it->bins[bin].data.label<<"_" << bin<<" = "<<it->bins[bin].data.eventyield<<"\n";
         ofile << "# "<<it->bins[bin].signal.label<<"_" << bin<<" = "<<it->bins[bin].signal.eventyield<<"\n";
         ofile << "# "<<it->bins[bin].signal.label<<"_contamination_" << bin
	              <<" = "<<it->bins[bin].signal.contamination<<"\n";
	 for (int unc=0; unc<(int)it->bins[bin].signal.uncert.size(); ++unc){
           ofile << "# "<<it->bins[bin].signal.label<<"_" << bin<<"_"<<it->bins[bin].signal.uncert[unc].label 
	         <<"_UP = "<<it->bins[bin].signal.uncert[unc].up<<"\n";
           ofile << "# "<<it->bins[bin].signal.label<<"_" << bin<<"_"<<it->bins[bin].signal.uncert[unc].label 
	         <<"_DN = "<<it->bins[bin].signal.uncert[unc].dn<<"\n";
	 }	 
	 for (int sample=0; sample<n_backgrounds;++sample) {
           ofile << "# "<<it->bins[bin].backgds[sample].label<<"_" << bin<<" = "
	         << it->bins[bin].backgds[sample].eventyield<<"\n";
	   for (int unc=0; unc<(int)it->bins[bin].backgds[sample].uncert.size(); ++unc){
             ofile << "# "<<it->bins[bin].backgds[sample].label<<"_" << bin<<"_"<<it->bins[bin].backgds[sample].uncert[unc].label 
	           <<"_UP = "<<it->bins[bin].backgds[sample].uncert[unc].up<<"\n";
             ofile << "# "<<it->bins[bin].backgds[sample].label<<"_" << bin<<"_"<<it->bins[bin].backgds[sample].uncert[unc].label 
	           <<"_DN = "<<it->bins[bin].backgds[sample].uncert[unc].dn<<"\n";
	   }
	 }  	 
       }  
	     
	     
	     
       ofile << "imax " << setw(2) << n_nonzero_channels    << "  number of channels" << endl;
       ofile << "jmax " << setw(2) << n_backgrounds << "  number of backgrounds" << endl;
       ofile << "kmax " << setw(2) << n_nuisance    
             << "  number of nuisance parameters (sources of systematic uncertainties)" << endl;
       ofile << "------------" << endl;  

       //observed events in all channels
       TTable observed("## observed events");\
       observed.SetStyle(Empty);
       observed.SetDelimiter("  ");
       observed.AddColumn<string>(""); 
       for (int bin=1; bin<=n_channels; ++bin) {
	   if (it->bins[bin-1].signal.eventyield - it->bins[bin-1].signal.contamination<=0) continue;
	   observed.AddColumn<int>("");}
       observed << "bin";	  
       for (int bin=1; bin<=n_channels; ++bin) {
	 if (it->bins[bin-1].signal.eventyield - it->bins[bin-1].signal.contamination<=0) continue;
         observed << bin;
       }
       observed << "observation"; 
       for (int bin=1; bin<=n_channels; ++bin) {
         if (it->bins[bin-1].signal.eventyield - it->bins[bin-1].signal.contamination<=0) continue;
	 stringstream ss;
         ss << "n" << bin;
         observed << (int)it->bins[bin-1].data.eventyield;
       }  
       ofile << observed << "------------\n" << endl;  

       //expected events in all channels for signal and all backgrounds
       TTable exp("## expected events");
       exp.SetStyle(Empty);
       exp.SetDelimiter("  ");
       exp.AddColumn<string>(""); 
       exp.SetMinumumWidth(20,0);//make first column at least 20 chars
       for (int bin=1; bin<=n_channels; ++bin) {
           if (it->bins[bin-1].signal.eventyield - it->bins[bin-1].signal.contamination<=0) continue;
           for (int sample=1; sample<=n_backgrounds+1; ++sample) 
             exp.AddColumn<string>("");
       }     
       exp << "bin"; 
       for (int bin=1; bin<=n_channels; ++bin) {
           if (it->bins[bin-1].signal.eventyield - it->bins[bin-1].signal.contamination<=0) continue;
           for (int sample=1; sample<=n_backgrounds+1; ++sample) {
              stringstream ss;
              ss << bin;
              exp << ss.str();
           }
       }   
       exp << "process";
       for (int bin=0; bin<n_channels; ++bin) {
         if (it->bins[bin].signal.eventyield - it->bins[bin].signal.contamination<=0) continue;
         exp << it->bins[bin].signal.label;
	 for (int sample=0; sample<n_backgrounds;++sample) 
           exp << it->bins[bin].backgds[sample].label;
       }  
       exp << "process"; 
       for (int bin=1; bin<=n_channels; ++bin) {
           if (it->bins[bin-1].signal.eventyield - it->bins[bin-1].signal.contamination<=0) continue;
           for (int sample=1; sample<=n_backgrounds+1; ++sample) {
              stringstream ss;
              ss << (sample-1);
              exp << ss.str();
           }
       }	      
       exp << "rate"; 
       for (int bin=0; bin<n_channels; ++bin) {
         if (it->bins[bin].signal.eventyield - it->bins[bin].signal.contamination<=0) continue;
         exp << ToString(it->bins[bin].signal.eventyield - it->bins[bin].signal.contamination);
	 for (int sample=0; sample<n_backgrounds;++sample) 
            exp << ToString(it->bins[bin].backgds[sample].eventyield);

             
       }  
       ofile << exp << "------------" << std::endl;  
       TTable sys("");
       sys.SetStyle(Empty);
       sys.SetDelimiter("  ");
       sys.AddColumn<string>(""); 
       sys.SetMinumumWidth(20,0);//make first column at least 20 chars
       for (int bin=1; bin<=n_channels; ++bin) {
           if (it->bins[bin-1].signal.eventyield - it->bins[bin-1].signal.contamination<=0) continue;       
           for (int sample=1; sample<=n_backgrounds+1; ++sample) 
             sys.AddColumn<string>("");
       }     
       for (std::map<std::string,std::vector<std::string> >::const_iterator u=it->uncertainties.begin();
            u!=it->uncertainties.end(); ++u) {
	  if (StrContains(u->first, "SIGNAL_SYST_KFACTOR") || StrContains(u->first, "SIGNAL_SYST_XSEC")) {
	    for (int b=0; b<n_channels; ++b) {
	      if (it->bins[b].signal.eventyield - it->bins[b].signal.contamination<=0) continue;
                for (int unc=0; unc<(int)it->bins[b].signal.uncert.size();++unc){
 	          string unc_name( it->bins[b].signal.uncert[unc].label );
                  std::replace(unc_name.begin(), unc_name.end(), ' ', '_'); 
                  if (unc_name+" "+it->bins[b].signal.uncert[unc].distribution ==u->first){
                    it->syst[it->bins[b].signal.label+" "+unc_name] +=
                  (it->bins[b].signal.GetFactorialUncertainty(unc)-1)* it->bins[b].signal.eventyield;
		  }
		}
	     }  
	     continue; 
	  }	
  
	  //if (StrContains( u->first, "STAT") ){//&& !StrContains( u->first, "QCD")){//uncorrelated stat. uncertainties
	    if (StrContains( u->first, "STAT") && !StrContains( u->first, "QCD")){//uncorrelated stat. uncertainties
//	    cout << u->first << " is a statistical uncertainty!"<<endl;
	    for (int thisbin=0; thisbin<n_channels; ++thisbin){
              if (it->bins[thisbin].signal.eventyield - it->bins[thisbin].signal.contamination<=0) continue; 
              stringstream ss(u->first);
	      string Unc_name, Unc_distr;
	      ss >> Unc_name; 
	      ss >> Unc_distr;
	      stringstream ss1;
	      ss1 << Unc_name<<"_bin"<<thisbin+1<<" "<<Unc_distr;
	      sys << ss1.str();
	      for (int bin=0; bin<n_channels; ++bin){
                if (it->bins[bin].signal.eventyield - it->bins[bin].signal.contamination<=0) continue; 
		//signal:
		bool foundSyst=false;
        	for (int unc=0; unc<(int)it->bins[bin].signal.uncert.size();++unc){
		  string unc_name( it->bins[bin].signal.uncert[unc].label );
        	  std::replace(unc_name.begin(), unc_name.end(), ' ', '_'); 
        	  if (bin==thisbin && unc_name+" "+it->bins[bin].signal.uncert[unc].distribution ==u->first){
	            sys << ToString(it->bins[bin].signal.GetFactorialUncertainty(unc),"-"); // signal
		    it->stat2[it->bins[bin].signal.label+" "+unc_name] +=
		       pow((it->bins[bin].signal.GetFactorialUncertainty(unc)-1)*
		                              it->bins[bin].signal.eventyield,2);
		    foundSyst=true;
        	  }
	        }
                if (!foundSyst) sys << "-";
                //backgds:
		for (int sample=0; sample<n_backgrounds; ++sample) { 
		  foundSyst=false; 
        	  for (int unc=0; unc<(int)it->bins[bin].backgds[sample].uncert.size();++unc){
		    string unc_name( it->bins[bin].backgds[sample].uncert[unc].label );
        	    std::replace(unc_name.begin(), unc_name.end(), ' ', '_'); 
        	    if (bin==thisbin && unc_name+" "+it->bins[bin].backgds[sample].uncert[unc].distribution ==u->first){
	              sys << ToString(it->bins[bin].backgds[sample].GetFactorialUncertainty(unc),"-"); // background 'sample'
		      it->stat2[it->bins[bin].backgds[sample].label+" "+unc_name] +=
		         pow((it->bins[bin].backgds[sample].GetFactorialUncertainty(unc)-1)*
		                             it->bins[bin].backgds[sample].eventyield,2);
 		      foundSyst=true;
		    }
		  }
		  if (!foundSyst) sys << "-";
		}
	      }	
	    }
	  } else {  
	  sys << u->first;
	  for (int b=0; b<n_channels; ++b) {
	    if (it->bins[b].signal.eventyield - it->bins[b].signal.contamination<=0) continue;
	    bool foundSyst=false;
            for (int unc=0; unc<(int)it->bins[b].signal.uncert.size();++unc){
	      string unc_name( it->bins[b].signal.uncert[unc].label );
              std::replace(unc_name.begin(), unc_name.end(), ' ', '_'); 
              if (unc_name+" "+it->bins[b].signal.uncert[unc].distribution ==u->first){
	        sys << ToString(it->bins[b].signal.GetFactorialUncertainty(unc),"-"); // signal
                it->syst[it->bins[b].signal.label+" "+unc_name] += (it->bins[b].signal.GetFactorialUncertainty(unc)-1)* it->bins[b].signal.eventyield;
                it->systUp[it->bins[b].signal.label+" "+unc_name] += (it->bins[b].signal.GetFactorialUncertaintyUp(unc)-1)* it->bins[b].signal.eventyield;
                it->systDn[it->bins[b].signal.label+" "+unc_name] += (it->bins[b].signal.GetFactorialUncertaintyDn(unc)-1)* it->bins[b].signal.eventyield;

		foundSyst=true;
              }
	    }
	    if (!foundSyst) sys << "-";
	    for (int sample=0; sample<n_backgrounds; ++sample) { 
	      foundSyst=false; 
              for (int unc=0; unc<(int)it->bins[b].backgds[sample].uncert.size();++unc){
	        string unc_name( it->bins[b].backgds[sample].uncert[unc].label );
                std::replace(unc_name.begin(), unc_name.end(), ' ', '_'); 
                if (unc_name+" "+it->bins[b].backgds[sample].uncert[unc].distribution ==u->first){
	          sys << ToString(it->bins[b].backgds[sample].GetFactorialUncertainty(unc),"-"); // background 'sample'
                  it->syst[it->bins[b].backgds[sample].label+" "+unc_name] += (it->bins[b].backgds[sample].GetFactorialUncertainty(unc)-1)* it->bins[b].backgds[sample].eventyield;
                  it->systUp[it->bins[b].backgds[sample].label+" "+unc_name] += (it->bins[b].backgds[sample].GetFactorialUncertaintyUp(unc)-1)* it->bins[b].backgds[sample].eventyield;
                  it->systDn[it->bins[b].backgds[sample].label+" "+unc_name] += (it->bins[b].backgds[sample].GetFactorialUncertaintyDn(unc)-1)* it->bins[b].backgds[sample].eventyield;
 		  foundSyst=true;
	        }
	      }
	      if (!foundSyst) sys << "-";
	    }
	  }
	  }        
       }
       ofile << sys << "------------" << std::endl;  

       for (std::map<std::string, double>::const_iterator sys=it->syst.begin();sys!=it->syst.end();++sys){
         ofile << "# "<<sys->first<<" = "<<sys->second<<"\n";
       } 
       for (std::map<std::string, double>::const_iterator sys=it->systUp.begin();sys!=it->systUp.end();++sys){
         ofile << "# "<<sys->first<<"_Up = "<<sys->second<<"\n";
       } 
       for (std::map<std::string, double>::const_iterator sys=it->systDn.begin();sys!=it->systDn.end();++sys){
         ofile << "# "<<sys->first<<"_Dn = "<<sys->second<<"\n";
       } 
       for (std::map<std::string, double>::const_iterator sys=it->stat2.begin();sys!=it->stat2.end();++sys){
         ofile << "# "<<sys->first<<" = "<<sqrt(sys->second)<<"\n";
       } 
	
       //cout << "WRITE m0:" << it->cmssm.mzero << ", m1/2:" << it->cmssm.mhalf << endl;
       ofile.close();
    }
}
//=========================================================================================================
//=========================================================================================================
//=========================================================================================================
//=========================================================================================================
/*
void points::WriteHiggsInput(const std::string dir) {
    using namespace Table;
    using namespace std;
    for (vector<point>::iterator it = p_.begin(); it != p_.end(); ++it) {
       ofstream ofile;
       stringstream ss;
       ss << dir << "_" << it->cmssm.mzero << "_" << it->cmssm.mhalf << "_"
        	       << it->cmssm.tanbeta << "_" << it->cmssm.azero << "_" << it->cmssm.mu<< ".txt";
       ofile.open(ss.str().c_str());
       ofile << "# Mzero = " << it->cmssm.mzero << "\n";
       ofile << "# Mhalf = " << it->cmssm.mhalf << "\n";
       ofile << "# TanBeta = " << it->cmssm.tanbeta << "\n";
       ofile << "# Xsection.LO = " << it->cmssm.xsec << "\n";
       ofile << "# Xsection.NLO = " << it->cmssm.xsecNLO << "\n";
       ofile << "# Luminosity = " << it-> lumi << "\n";
       
       cout << "M0 = "<<it->cmssm.mzero<<", M1/2 ="<<it->cmssm.mhalf << endl;
       
       int n_channels	 = it->n_bins;
       int n_nonzero_channels=0;
       for (int bin=0; bin<n_channels; ++bin) 
	   if (it->bins[bin].signal.eventyield - it->bins[bin].signal.contamination>0) ++n_nonzero_channels;
       int n_backgrounds = it->bins[0].backgds.size();
       int n_nuisance	 = it->uncertainties.size();
       ofile << "imax " << setw(2) << n_nonzero_channels    << "  number of channels" << endl;
       ofile << "jmax " << setw(2) << n_backgrounds << "  number of backgrounds" << endl;
       ofile << "kmax " << setw(2) << n_nuisance    
             << "  number of nuisance parameters (sources of systematic uncertainties)" << endl;
       ofile << "------------" << endl;  

       //observed events in all channels
       TTable observed("# observed events");\
       observed.SetStyle(Empty);
       observed.SetDelimiter("  ");
       observed.AddColumn<string>(""); 
       for (int bin=1; bin<=n_channels; ++bin) {
	   if (it->bins[bin-1].signal.eventyield - it->bins[bin-1].signal.contamination<=0) continue;
	   observed.AddColumn<int>("");}
       observed << "bin";	  
       for (int bin=1; bin<=n_channels; ++bin) {
	 if (it->bins[bin-1].signal.eventyield - it->bins[bin-1].signal.contamination<=0) continue;
         observed << bin;
       }
       observed << "observation"; 
       for (int bin=1; bin<=n_channels; ++bin) {
         if (it->bins[bin-1].signal.eventyield - it->bins[bin-1].signal.contamination<=0) continue;
	 stringstream ss;
         ss << "n" << bin;
         observed << (int)it->bins[bin-1].data.eventyield;
       }  
       ofile << observed << "------------\n" << endl;  

       //expected events in all channels for signal and all backgrounds
       TTable exp("# expected events");
       exp.SetStyle(Empty);
       exp.SetDelimiter("  ");
       exp.AddColumn<string>(""); 
       exp.SetMinumumWidth(20,0);//make first column at least 20 chars
       for (int bin=1; bin<=n_channels; ++bin) {
           if (it->bins[bin-1].signal.eventyield - it->bins[bin-1].signal.contamination<=0) continue;
           for (int sample=1; sample<=n_backgrounds+1; ++sample) 
             exp.AddColumn<string>("");
       }     
       exp << "bin"; 
       for (int bin=1; bin<=n_channels; ++bin) {
           if (it->bins[bin-1].signal.eventyield - it->bins[bin-1].signal.contamination<=0) continue;
           for (int sample=1; sample<=n_backgrounds+1; ++sample) {
              stringstream ss;
              ss << bin;
              exp << ss.str();
           }
       }   
       exp << "process";
       for (int bin=0; bin<n_channels; ++bin) {
         if (it->bins[bin].signal.eventyield - it->bins[bin].signal.contamination<=0) continue;
         exp << it->bins[bin].signal.label;
	 for (int sample=0; sample<n_backgrounds;++sample) 
           exp << it->bins[bin].backgds[sample].label;
       }  
       exp << "process"; 
       for (int bin=1; bin<=n_channels; ++bin) {
           if (it->bins[bin-1].signal.eventyield - it->bins[bin-1].signal.contamination<=0) continue;
           for (int sample=1; sample<=n_backgrounds+1; ++sample) {
              stringstream ss;
              ss << (sample-1);
              exp << ss.str();
           }
       }	      
       exp << "rate"; 
       for (int bin=0; bin<n_channels; ++bin) {
         if (it->bins[bin].signal.eventyield - it->bins[bin].signal.contamination<=0) continue;
         exp << ToString(it->bins[bin].signal.eventyield - it->bins[bin].signal.contamination);
	 for (int sample=0; sample<n_backgrounds;++sample) 
            exp << ToString(it->bins[bin].backgds[sample].eventyield);

             
       }  
       ofile << exp << "------------" << std::endl;  
       TTable sys("");
       sys.SetStyle(Empty);
       sys.SetDelimiter("  ");
       sys.AddColumn<string>(""); 
       sys.SetMinumumWidth(20,0);//make first column at least 20 chars
       for (int bin=1; bin<=n_channels; ++bin) {
           if (it->bins[bin-1].signal.eventyield - it->bins[bin-1].signal.contamination<=0) continue;       
           for (int sample=1; sample<=n_backgrounds+1; ++sample) 
             sys.AddColumn<string>("");
       }     
       for (std::map<std::string,std::vector<std::string> >::const_iterator u=it->uncertainties.begin();
            u!=it->uncertainties.end(); ++u) {
	  sys << u->first;
	  for (int b=0; b<n_channels; ++b) {
	    if (it->bins[b].signal.eventyield - it->bins[b].signal.contamination<=0) continue;
	    bool foundSyst=false;
            for (int unc=0; unc<(int)it->bins[b].signal.uncert.size();++unc){
	      string unc_name( it->bins[b].signal.uncert[unc].label );
              std::replace(unc_name.begin(), unc_name.end(), ' ', '_'); 
              if (unc_name+" "+it->bins[b].signal.uncert[unc].distribution ==u->first){
	        sys << ToString(it->bins[b].signal.GetFactorialUncertainty(unc),"-"); // signal
		foundSyst=true;
              }
	    }
	    if (!foundSyst) sys << "-";
	    for (int sample=0; sample<n_backgrounds; ++sample) { 
	      foundSyst=false; 
              for (int unc=0; unc<(int)it->bins[b].backgds[sample].uncert.size();++unc){
	        string unc_name( it->bins[b].backgds[sample].uncert[unc].label );
                std::replace(unc_name.begin(), unc_name.end(), ' ', '_'); 
                if (unc_name+" "+it->bins[b].backgds[sample].uncert[unc].distribution ==u->first){
	          sys << ToString(it->bins[b].backgds[sample].GetFactorialUncertainty(unc),"-"); // background 'sample'
 		  foundSyst=true;
	        }
	      }
	      if (!foundSyst) sys << "-";
	    }
	  }        
       }
       ofile << sys << "------------" << std::endl;  

       //cout << "WRITE m0:" << it->cmssm.mzero << ", m1/2:" << it->cmssm.mhalf << endl;
       ofile.close();
	}
}
*/

void points::WriteTable(std::ostream& os, Table::TableStyle style, unsigned p)
{
   using Table::TTable;
   TTable out("Observed and expected event yields");
   out.SetCaption("Given systematics are simple quadric-sums, and not what is used for limit calculation where the correlations are considered.");
   out.SetStyle(style);
   out.SetDelimiter("|");
   out.AddColumn<string>( "Samples" );
   for (int bin=0; bin<(int)p_[p].bins.size(); ++bin)
      out.AddColumn<string>( p_[p].bins[bin].label );
   if (p>=0) { // add signal
     out << p_[p].bins[0].signal.label;
     for (int bin=0; bin<(int)p_[p].bins.size(); ++bin) {
       std::stringstream ss;
       ss<<std::fixed << std::setprecision(1) << p_[p].bins[bin].signal.eventyield;
       if (style!=Table::TeX) ss << " +- ";
       else ss <<" $\\pm$ ";
       ss<<std::fixed << std::setprecision(1)<<p_[p].bins[bin].signal.GetTotalUncertainty() << " ";
       out << ss.str();
     }
     //signal contamination  
     out << p_[p].bins[0].signal.label+" contamination";
     for (int bin=0; bin<(int)p_[p].bins.size(); ++bin) {
       std::stringstream ss;
       ss<<std::fixed << std::setprecision(1) << p_[p].bins[bin].signal.contamination;
       out << ss.str();
     }
   }
   //backgrounds
   std::vector<double> total_back, total_backuncert2;
   for (int bin=0; bin<(int)p_[p].bins.size(); ++bin) {total_back.push_back(0); total_backuncert2.push_back(0);}
   for (int sample=0; sample<(int)p_[p].bins[0].backgds.size(); ++sample) {
     out << p_[p].bins[0].backgds[sample].label;
     for (int bin=0; bin<(int)p_[p].bins.size(); ++bin) {
       total_back[bin] += p_[p].bins[bin].backgds[sample].eventyield;
       total_backuncert2[bin] += pow(p_[p].bins[bin].backgds[sample].GetTotalUncertainty(),2);
       std::stringstream ss;
       ss<<std::fixed << std::setprecision(1) << p_[p].bins[bin].backgds[sample].eventyield;
       if (style!=Table::TeX) ss << " +- ";
       else ss <<" $\\pm$ ";
       ss<<std::fixed << std::setprecision(1)<<p_[p].bins[bin].backgds[sample].GetTotalUncertainty() << " ";
       out << ss.str();
     }  
   }
   out << "total background";
   for (int bin=0; bin<(int)p_[p].bins.size(); ++bin) {
     std::stringstream ss;
     ss<<std::fixed << std::setprecision(1) << total_back[bin];
     if (style!=Table::TeX) ss << " +- ";
     else ss <<" $\\pm$ ";
     ss<<std::fixed << std::setprecision(1)<< sqrt(total_backuncert2[bin]) << " ";
     out << ss.str();
   }  
   out << p_[p].bins[0].data.label;
   for (int bin=0; bin<(int)p_[p].bins.size(); ++bin) {
     std::stringstream ss;
     ss<<std::fixed << std::setprecision(1) << p_[p].bins[bin].data.eventyield << " ";
     out << ss.str();
   }  
   os << out << std::endl;
}


void points::AddData(point& p, ConfigFile * data, std::string data_name){
    p.lumi = data  ->read<double>("luminosity");
    std::vector<double> dat_yield = bag_of<double>(data  ->read<std::string>(data_name+"_events"));
    std::vector<std::string> bin_labels = bag_of_string(data->read<std::string>("channel"));
    for (int i=0; i<(int)p.bins.size(); ++i) {
      p.bins[i].label = bin_labels[i];
      p.bins[i].data.label = data_name;
      p.bins[i].data.eventyield = dat_yield[i];
    }
}
void points::AddData(point& p, std::string dat){
    ConfigFile * data   = new ConfigFile( dat );
    p.n_bins = data->read<double>("channels",1.);
    if (!p.bins.size()) for (int i=0; i<(int)p.n_bins; ++i) p.bins.push_back(point::Bin());
    else if (p.n_bins!=p.bins.size()){
      std::cerr << "ERROR: inconsistent number of channels! Check data = "<< dat 
                << " "<<p.n_bins<<"<->"<<p.bins.size()<<std::endl;
      exit(1);
    }
    std::string data_name = data->read<std::string>("sample","data");
    AddData(p,data,data_name);
}

void points::AddBackground(point& p, ConfigFile * cfg, std::string name){
    double lumiscale = p.lumi / cfg->read<double>("luminosity");
    std::vector<double> yield = bag_of<double>(cfg->read<std::string>(name+"_events"));
    int n_nuis = cfg->read<int>("nuisances",1);
    std::vector<string> nuisance_label = bag_of_string(cfg->read<std::string>("nuisance",""));
    std::vector<string> nuisance_distr;
    std::vector< std::vector<double> > nuisUP, nuisDN;
    for (int nuis=0; nuis<n_nuis; ++nuis) {
      std::stringstream ss, ssUP, ssDN, ssDistr;
      ss << name<<"_uncertainty_"<<nuis+1;
      ssUP << name<<"_uncertaintyUP_"<<nuis+1;
      ssDN << name<<"_uncertaintyDN_"<<nuis+1;
      ssDistr << name<<"_uncertaintyDistribution_"<<nuis+1;
      nuisance_distr.push_back(cfg->read<std::string>(ssDistr.str(),"lnN"));
      nuisDN.push_back(bag_of<double>(cfg->read<std::string>(ssDN.str(),
                           cfg->read<std::string>(ss.str(),""))) ); //use symmetric uncertainty as default
      nuisUP.push_back(bag_of<double>(cfg->read<std::string>(ssUP.str(),
                           cfg->read<std::string>(ss.str(),""))) );
      //if (nuisUP[nuis].size()!=p.bins.size() || nuisDN[nuis].size()!=p.bins.size() ){
      if (nuisUP[nuis].size()!=nuisDN[nuis].size() || nuisUP[nuis].size()<p.n_bins ){
        std::cerr << "ERROR: inconsistent number of channels for uncertainty '"<< ss.str() 
	          << " up="<< nuisUP[nuis].size()<<"<->"<<p.bins.size()<< std::endl;
	exit(1);
      }
      //keep track of all correlated and uncorrelated uncertainties
      string unc_name( nuisance_label[nuis] );
      std::replace(unc_name.begin(), unc_name.end(), ' ', '_');
      p.uncertainties[ unc_name+" "+nuisance_distr[nuis] ].push_back(name );
   }
   p.uncertainties[ name+"_trigger lnN"].push_back(name );
   

    for (int i=0; i<(int)p.bins.size(); ++i) {
      point::Sample s;
      s.label = name;
      s.eventyield = yield[i] * lumiscale;
      for (int u=0; u<n_nuis; ++u){
        point::Uncertainties uncert;
	uncert.label = nuisance_label[u];
	uncert.distribution = nuisance_distr[u];
	uncert.up = nuisUP[u][i] * lumiscale;
	uncert.dn = nuisDN[u][i] * lumiscale;
	
	//@@Check the impact of  errors: contrain them :
	//if ( //(name=="zinvis" ) && 
	//    (uncert.up>0.2*yield[i] || uncert.dn>0.2*yield[i])) {
	//   uncert.up = 0.2*yield[i];
	//   uncert.dn = 0.2*yield[i];
	//}
	//std::cout << "qcd? "<<(name=="qcd" && StrContains(uncert.label,"other")) 
	//          << " : "<<name<<", "<<uncert.label<< "; "
	//	  << yield[i]<<", "<< uncert.up<<"; "
	//	  << uncert.up/yield[i]<<std::endl;  
	//@@end: check

	
	s.uncert.push_back(uncert);
      }
      point::Uncertainties uncert;
      uncert.label = name+"_trigger";
      uncert.distribution = "lnN";
      uncert.up = trigger_uncertainty * yield[i] * lumiscale;
      uncert.dn = trigger_uncertainty * yield[i] * lumiscale;
      s.uncert.push_back(uncert);

      p.bins[i].backgds.push_back(s);      
    }

}
void points::AddSignal(point& p, ConfigFile * cfg, std::string name){
    double lumiscale = p.lumi / cfg->read<double>("luminosity");
    p.cmssm.mzero = cfg->read<double>("Mzero",-1.);
    p.cmssm.mhalf = cfg->read<double>("Mhalf",-1.);
    p.cmssm.azero = cfg->read<double>("Azero",-1.);
    p.cmssm.tanbeta = cfg->read<double>("TanBeta",-1.);
    p.cmssm.mu = cfg->read<double>("Mu",-1.);
    p.cmssm.xsec = cfg->read<double>("Xsection.LO",-1.);
    p.cmssm.xsecNLO = cfg->read<double>("Xsection.NLO",-1.);

    std::vector<double> yield = bag_of<double>(cfg->read<std::string>(name+"_events"));
    std::vector<double> contamination = bag_of<double>(cfg->read<std::string>(name+"_contamination"));
    int n_nuis = cfg->read<int>("nuisances",1);
    std::vector<string> nuisance_label = bag_of_string(cfg->read<std::string>("nuisance",""));
    std::vector<string> nuisance_distr;
    std::vector< std::vector<double> > nuisUP, nuisDN;
    for (int nuis=0; nuis<n_nuis; ++nuis) {
      std::stringstream ss, ssUP, ssDN, ssDistr;
      ss << name<<"_uncertainty_"<<nuis+1;
      ssUP << name<<"_uncertaintyUP_"<<nuis+1;
      ssDN << name<<"_uncertaintyDN_"<<nuis+1;
      ssDistr << name<<"_uncertaintyDistribution_"<<nuis+1;
      nuisance_distr.push_back(cfg->read<std::string>(ssDistr.str(),"lnN"));
      nuisDN.push_back(bag_of<double>(cfg->read<std::string>(ssDN.str(),
                           cfg->read<std::string>(ss.str(),""))) ); //use symmetric uncertainty as default
      nuisUP.push_back(bag_of<double>(cfg->read<std::string>(ssUP.str(),
                           cfg->read<std::string>(ss.str(),""))) );
      //if (nuisUP[nuis].size()!=p.bins.size() || nuisDN[nuis].size()!=p.bins.size() ){
      if (nuisUP[nuis].size()!=nuisDN[nuis].size() || nuisUP[nuis].size()<p.n_bins ){
         std::cerr << "ERROR: inconsistent number of channels for uncertainty '"<< ss.str() << std::endl;
	exit(1);
      }
      //keep track of all correlated and uncorrelated uncertainties
      string unc_name( nuisance_label[nuis] );
      std::replace(unc_name.begin(), unc_name.end(), ' ', '_');
      p.uncertainties[ unc_name+" "+nuisance_distr[nuis] ].push_back(name );
    }
    p.uncertainties[ name+"_trigger lnN"].push_back(name );
   

    for (int i=0; i<(int)p.bins.size(); ++i) {
      p.bins[i].signal.label = name;
      p.bins[i].signal.eventyield    = yield[i] * lumiscale;
      
      p.bins[i].signal.contamination = contamination[i] * lumiscale;
      //p.bins[i].signal.contamination = 0;
      
//kill bin 14: if (i==13)p.bins[i].signal.eventyield=0;
      for (int u=0; u<n_nuis; ++u){
        point::Uncertainties uncert;
	uncert.label = nuisance_label[u];
	uncert.distribution = nuisance_distr[u];
	uncert.up = nuisUP[u][i] * lumiscale;
	uncert.dn = nuisDN[u][i] * lumiscale;
	
	////@@Check the impact of hughe stat and JES/JER errors: contrain them to 20%:
	//if ((uncert.label=="signal stat" || uncert.label=="signal syst") && 
	 //   (uncert.up>0.2*p.bins[i].signal.eventyield || uncert.dn>0.2*p.bins[i].signal.eventyield)) {
	//   uncert.up = 0.2*p.bins[i].signal.eventyield;
	//   uncert.dn = 0.2*p.bins[i].signal.eventyield;
	//}
	//std::cout << "stat? "<<  (uncert.label=="signal stat") << " : "<<uncert.label<<std::endl;  
	//std::cout << "syst? "<<  (uncert.label=="signal syst") << " : "<<uncert.label<<std::endl;  
	//@@end: check
	
	if ((int)p.bins[i].signal.uncert.size()<=u ) 
	  p.bins[i].signal.uncert.push_back(uncert);
	else {        
	  p.bins[i].signal.uncert[u].label = uncert.label;
	  p.bins[i].signal.uncert[u].distribution = uncert.distribution;
	  p.bins[i].signal.uncert[u].up = uncert.up;
	  p.bins[i].signal.uncert[u].dn = uncert.dn;
	}  
      }
      point::Uncertainties uncert;
      uncert.label = name+"_trigger";
      uncert.distribution = "lnN";
      uncert.up = trigger_uncertainty * yield[i] * lumiscale;
      uncert.dn = trigger_uncertainty * yield[i] * lumiscale;
      if ((int)p.bins[i].signal.uncert.size()<=n_nuis ) p.bins[i].signal.uncert.push_back(uncert);
      else {	    
        p.bins[i].signal.uncert[n_nuis].label = uncert.label;
        p.bins[i].signal.uncert[n_nuis].distribution = uncert.distribution;
        p.bins[i].signal.uncert[n_nuis].up = uncert.up;
        p.bins[i].signal.uncert[n_nuis].dn = uncert.dn;
      }  
      
    }

}
void points::AddBackgrounds(point& p, std::string bkg){
    ConfigFile * cfg   = new ConfigFile( bkg );
    int bins = cfg->read<double>("channels",1.);
    if (!p.bins.size()) for (int i=0; i<bins; ++i) p.bins.push_back(point::Bin());
    else if (bins!=(int)p.bins.size()){
      std::cerr << "ERROR: inconsistent number of channels! Check background "<< bkg << std::endl;
      exit(1);
    }
    std::vector<std::string> names = bag_of_string(cfg->read<std::string>("sample"));
    for (std::vector<std::string>::const_iterator n=names.begin(); n!=names.end(); ++n)
      AddBackground(p,cfg,*n);
}

void points::Read(std::string dat, std::vector<std::string> bkgs, std::vector<std::string> sigs) {
  using namespace std;
  point p;
  AddData(p,dat);
  for (std::vector<std::string>::const_iterator bkg=bkgs.begin(); bkg!=bkgs.end(); ++bkg){
    AddBackgrounds(p,*bkg);
  }  
  for (std::vector<std::string>::const_iterator sig=sigs.begin(); sig!=sigs.end(); ++sig){
    ConfigFile * signal = new ConfigFile( *sig );
    int bins = signal->read<double>("channels",1.);
    if (bins != signal->read<double>("channels",1.) ){
        std::cerr << "ERROR: inconsistent number of channels! Skipping signal '"<<*sig<<"'" << std::endl;
	continue;
    }
    stringstream ss;
    ss << "signal"<<sig-sigs.begin();
    std::string signal_name = signal->read<std::string>("sample",ss.str());
    AddSignal(p,signal,signal_name);    
 
    Add(p);  
  }
}



int main( int argc, char* argv[] ) {
   Points.Reset();
   std::string data="";
   std::string out="limits";
   std::vector<std::string> bkgds;
   std::vector<std::string> signals;
           
   AnyOption *opt = new AnyOption();
   opt->addUsage( "Usage: " );
   opt->addUsage( "" );
   opt->addUsage( " -h  --help                     Prints this help " );
   opt->addUsage( " --tex                          Prints TeX table " );
   opt->addUsage( " --single                       Creates Single-Channel input files" );
   opt->addUsage( " -h  --help                     Prints this help " );
   opt->addUsage( " -d  --data [file]              Data data-card " );
   opt->addUsage( " -q  --qcd [file]               QCD data-card " );
   opt->addUsage( " -l  --lostlepton [file]        W/tt lostlepton data-card " );
   opt->addUsage( " -t  --tau [file]               W/tt->tau data-card " );
   opt->addUsage( " -z  --zinvisible [file]        Z->nu nu data-card " );
   opt->addUsage( " -o  --output [directory]       Output directory " );
   opt->addUsage( " -s  --signal [file(s) ...]     Signal data-card(s) " );
   opt->addUsage( " -i  --index [number]           index number " );
   opt->setFlag(  "help", 'h' );
   opt->setFlag(  "tex" );
   opt->setFlag(  "plain" );
   opt->setFlag(  "single" );
   opt->setOption(  "data", 'd' );
   opt->setOption(  "qcd", 'q' );
   opt->setOption(  "lostlepton", 'l' );
   opt->setOption(  "tau", 't' );
   opt->setOption(  "zinvisible", 'z' );
   opt->setOption(  "signal", 's' );
   opt->setOption(  "output", 'o' );
   opt->setOption(  "index", 'i' );
   opt->processCommandArgs( argc, argv );
   if( opt->getValue( 'i' ) != NULL || opt->getValue( "idx") != NULL )
      idxStr_ = opt->getValue( 'i' );
   if( opt->getValue( 'd' ) != NULL  || opt->getValue( "data" ) != NULL  )  
      data = opt->getValue( 'd' );
   if( opt->getValue( 'q' ) != NULL  || opt->getValue( "qcd"  ) != NULL  )  
      bkgds.push_back( (string)opt->getValue( 'q' ));
   if( opt->getValue( 'l' ) != NULL  || opt->getValue( "lostlepton"  ) != NULL  )  
      bkgds.push_back( (string)opt->getValue( 'l' ));
   if( opt->getValue( 't' ) != NULL  || opt->getValue( "tau"  ) != NULL  )  
      bkgds.push_back( (string)opt->getValue( 't' ));
   if( opt->getValue( 'z' ) != NULL  || opt->getValue( "zinvisible"  ) != NULL  )  
      bkgds.push_back( (string)opt->getValue( 'z' ));
   if( opt->getValue( 'o' ) != NULL  || opt->getValue( "output"  ) != NULL  )  
      out = (string)opt->getValue( 'o' );
   if( opt->getValue( 's' ) != NULL  || opt->getValue( "signal"  ) != NULL  )  
      signals.push_back( (string)opt->getValue( 's' ));
   for( int i = 0 ; i < opt->getArgc() ; i++ ) signals.push_back( (string)opt->getArgv( i ) );

   if( opt->getFlag( "help" ) || opt->getFlag( 'h' ) ) opt->printUsage();
   if (data==""||signals.size()<1||bkgds.size()<1){
      if (!opt->getFlag( "help" ) && !opt->getFlag( 'h' ) ) opt->printUsage();
      cerr<<"ERROR: Arguments missing!"<<endl;     
      return 1;
   }
  
   Points.Read(data,bkgds,signals);
   if (!opt->getFlag( "single" )) Points.WriteHiggsInputStatUncorr(out,"cMSSM");
   else                           Points.WriteHiggsInputSingleChannel(out,"cMSSM");
   if (opt->getFlag( "tex" ))     Points.WriteTable(std::cout,Table::TeX);
   if (opt->getFlag( "plain" ))   Points.WriteTable(std::cout);
   
   return 0;
}
