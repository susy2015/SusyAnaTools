//create input files for different limit tools
//Christian Autermann, Universität Hamburg

//system
#include <string>
#include <vector>
#include <map>

#include "table.h"

struct point { //Data structre to store all event yield, uncertainty info etc.
        struct cMSSM{
	  double mzero;
	  double mhalf;
	  double azero;
	  double mu;
	  double tanbeta;
	  double xsec;
	  double xsecNLO;
	} cmssm;
	double lumi;
	
	struct Uncertainties{
	  std::string label;
	  std::string distribution;
	  double up;
	  double dn;
	};
	
	struct Sample{
	  std::string label;
	  double eventyield;
	  double contamination;
	  std::vector<Uncertainties> uncert;
	  double GetFactorialUncertainty(unsigned u){//Used by WriteHiggsInput
	    return (eventyield==0?1. : 1.0 + (fabs(uncert[u].up)+fabs(uncert[u].dn))/(2.*eventyield));
	  }
	  double GetFactorialUncertaintyUp(unsigned u){//Used by WriteHiggsInput
	    return (eventyield==0?1. : 1.0 + (fabs(uncert[u].up))/(eventyield));
	  }
	  double GetFactorialUncertaintyDn(unsigned u){//Used by WriteHiggsInput
	    return (eventyield==0?1. : 1.0 + (fabs(uncert[u].dn))/(eventyield));
	  }
	  double GetTotalUncertainty(){//Used by WriteTable
            double syst=0.;
	    for (std::vector<Uncertainties>::const_iterator it=uncert.begin();it!=uncert.end();++it)
	      syst+= pow( (fabs(it->up)+fabs(it->dn))/2.,2);
	    return sqrt(syst);  
	  }
	};
	
        struct Bin {
	  std::string label;
	  Sample data;
	  Sample signal;
	  std::vector<Sample> backgds;
	};
	std::vector<Bin> bins;
	unsigned n_bins;
	std::map<std::string,std::vector<std::string> > uncertainties; //list of uncertainties and what samples are affected
        std::map<std::string, double> syst, systUp, systDn, stat2;
};

class points { //class providing methods to reading in the data-cards and to write different kinds of output
public:
	void Read(std::string dat, std::vector<std::string> bkgs, std::vector<std::string> sigs);
	point* Get(double mzero, double mhalf, double tanbeta) {
		for (std::vector<point>::iterator it = p_.begin(); it != p_.end(); ++it)
			if (it->cmssm.mzero == mzero && it->cmssm.mhalf == mhalf && it->cmssm.tanbeta == tanbeta)
				return &(*it);
		return 0;
	}
	void Add(point p) {
		p_.push_back(p);
	}
	void Reset() {
		p_.clear();
	}
	//void WriteHiggsInput(const std::string dir);
	void WriteHiggsInputStatUncorr(const std::string dir, const std::string prefix);
	void WriteHiggsInputSingleChannel(const std::string dir, const	std::string prefix);
	void WriteTable(std::ostream& os, Table::TableStyle style = Table::Plain, unsigned point=0);

private:
	std::vector<point> p_;

        void AddData(point& p, ConfigFile * data, std::string data_name);
	void AddData(point& p, std::string dat);
	void AddBackground(point& p, ConfigFile * cfg, std::string name);
	void AddSignal(point& p, ConfigFile * cfg, std::string name);
	void AddBackgrounds(point& p, std::string bkg);
} Points;

