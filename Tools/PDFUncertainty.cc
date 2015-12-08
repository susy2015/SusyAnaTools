#include "PDFUncertainty.h"

#include "LHAPDF/LHAPDF.h"
#include "NTupleReader.h"
#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include <iostream>
#include <cstdio>
#include <string>
#include <ctime>
#include <iomanip>
#include <cmath>

#include "TH1.h"

PDFUncertainty::PDFUncertainty()
{
        //ctor
        //Initialize all pdf sets
        for(int in = 0; in < 53; in++){

            pdf1.push_back(LHAPDF::mkPDF("CT10nlo", in));

        }
        for(int in = 0; in < 51; in++){

            pdf2.push_back(LHAPDF::mkPDF("MMHT2014nlo68cl", in));

        }
        for(int in = 0; in < 101; in++){

            pdf3.push_back(LHAPDF::mkPDF("NNPDF30_nlo_as_0118", in));

        }
}


PDFUncertainty::~PDFUncertainty()
{
    //dtor
}


void PDFUncertainty::getPDFUncertainty(NTupleReader& tr)
{
  //This is how we get variables from nTuple 
        const std::vector<double> &ScaleWeightsMiniAOD = tr.getVec<double>("ScaleWeightsMiniAOD");
        const double x1  =   tr.getVar<double>("x1");
        const double x2  =   tr.getVar<double>("x2");
        const double  Q  =   tr.getVar<double>("q");//q is stored variable in tuple
        const int id1    =   tr.getVar<int>("id1");
        const int id2    =   tr.getVar<int>("id2");

	//For Scale Variations
	std::vector<double> *ScaleWeights = new std::vector<double>();
        /*                                                                                                                         
          Compute the envelope of your observable for weight                                                                       
          indices 1,2,3,4,6,8 (index 0 corresponds to nominal                                                                      
          scale, indices 5 and 7 correspond  anti-correlated                                                                       
          variations).                                                                                                             
	*/
        (*ScaleWeights).push_back(ScaleWeightsMiniAOD.at(1));
        (*ScaleWeights).push_back(ScaleWeightsMiniAOD.at(2));
        (*ScaleWeights).push_back(ScaleWeightsMiniAOD.at(3));
        (*ScaleWeights).push_back(ScaleWeightsMiniAOD.at(4));
        (*ScaleWeights).push_back(ScaleWeightsMiniAOD.at(6));
        (*ScaleWeights).push_back(ScaleWeightsMiniAOD.at(8));

	auto biggest1 = std::max_element(std::begin(*ScaleWeights), std::end(*ScaleWeights));
        auto smallest1 = std::min_element(std::begin(*ScaleWeights), std::end(*ScaleWeights));

        double upperBound = *biggest1;
	double lowerBound = *smallest1;



        //Vector to be stored.
        std::vector<double> *pdfweights = new std::vector<double>();
        std::vector<int> *pdfids = new std::vector<int>();

        //Variables to be registered after calculation.
        double pdf_unc_sys;
        double pdf_unc_central;
        double pdf_unc_up;
        double pdf_unc_down;
	//Claculating PDF weights for each PDF Sets
        //Set 1
        int pdfid1 = -99;
        double xfx1_1 = pdf1.at(0)->xfxQ(id1,  x1, Q);
        double xfx2_1 = pdf1.at(0)->xfxQ(id2,  x2, Q);
        double w01 = xfx1_1*xfx2_1;
        double lhaweight1 = 0.0;
        for(int i = 0; i < pdf1.size(); i++){
          double      xfx_1_1 = pdf1.at(i)->xfxQ(id1, x1, Q);
          double      xfx_2_1 = pdf1.at(i)->xfxQ(id2, x2, Q);
          double w_new1 = xfx_1_1*xfx_2_1;
          lhaweight1  = w_new1/w01;
          pdfid1 = 1000 + i+1;
          (*pdfweights).push_back(lhaweight1);
          (*pdfids).push_back(pdfid1);
        }
        //Set 2
        int pdfid2 = -99;
        double xfx1_2 = pdf2.at(0)->xfxQ(id1,  x1, Q);
        double xfx2_2 = pdf2.at(0)->xfxQ(id2,  x2, Q);
        double w02 = xfx1_2*xfx2_2;
        double lhaweight2 = 0.0;
        for(int i = 0; i < pdf2.size(); i++){
          double      xfx_1_2 = pdf2.at(i)->xfxQ(id1, x1, Q);
          double      xfx_2_2 = pdf2.at(i)->xfxQ(id2, x2, Q);
          double w_new2 = xfx_1_2*xfx_2_2;
          lhaweight2  = w_new2/w02;
          pdfid2 = 2000 + i+1;
          (*pdfweights).push_back(lhaweight2);
          (*pdfids).push_back(pdfid2);
        }


        //Set 3
        int pdfid3 = -99;
        double xfx1_3 = pdf3.at(0)->xfxQ(id1,  x1, Q);
        double xfx2_3 = pdf3.at(0)->xfxQ(id2,  x2, Q);
        double w03 = xfx1_3*xfx2_3;
        double lhaweight3 = 0.0;
        for(int i = 0; i < pdf3.size(); i++){
          double      xfx_1_3 = pdf3.at(i)->xfxQ(id1, x1, Q);
          double      xfx_2_3 = pdf3.at(i)->xfxQ(id2, x2, Q);
          double w_new3 = xfx_1_3*xfx_2_3;
          lhaweight3  = w_new3/w03;
          pdfid3 = 3000 + i+1;
          (*pdfweights).push_back(lhaweight3);
          (*pdfids).push_back(pdfid3);
        }
	//Now calculate pdf uncertainities
           // All Variations Initialize to zero.
        double var_Weight[3];
        for(int i = 0; i < 3; i++) var_Weight[i] = 0.0;


        //PDF 1 CT10nlo....
        //PDF 2 MMHT2014
        //PDF 3 NNPDF...
        double wgh_CT10[53];
        double wgh_MMHT2014[51];
        double wgh_NNPDF[101];

        //Average central value for particular Pdf For each event.
        for(int ic =0; ic < 53; ic++){
            wgh_CT10[ic] = -1.0;
            if((*pdfids).at(ic) < 1200)  wgh_CT10[ic] = (*pdfweights).at(ic);
        }

        //***********************
        for(int im =0; im < 51; im++){
            wgh_MMHT2014[im] = -1.0;
            if((*pdfids).at(im+53) > 2000 && (*pdfids).at(im+53) < 2060)  wgh_MMHT2014[im] = (*pdfweights).at(im+53);
        }


        //*************************
        for(int in =0; in < 101; in++){
            wgh_NNPDF[in] = 0.0;
            if((*pdfids).at(in+104) > 2999) wgh_NNPDF[in] = (*pdfweights).at(in +104);

        }


	//********************************************************** 
       



        //**************************

        //PDF 1 CT10 ....
        for (int k= 1; k <= 26; k++){
            var_Weight[0] += 0.25*(wgh_CT10[2*k]-wgh_CT10[2*k-1])*(wgh_CT10[2*k]-wgh_CT10[2*k-1]);
        }

        //MMHT2014
        for (int k= 1; k <= 25; k++){
            var_Weight[1] += 0.25*(wgh_MMHT2014[2*k]-wgh_MMHT2014[2*k-1])*(wgh_MMHT2014[2*k]-wgh_MMHT2014[2*k-1]);
        }

        //NNPDF
        for (int k= 1; k <= 50; k++){
            var_Weight[2] += 0.25*(wgh_NNPDF[2*k]-wgh_NNPDF[2*k-1])*(wgh_NNPDF[2*k]-wgh_NNPDF[2*k-1]);
        }

      //Get std error
        for (int k=0;k<3;k++) var_Weight[k] = std::sqrt(var_Weight[k]);
        var_Weight[0] /= 1.645;            //----CTEQ6 variations correspond to 68% CL, so this factor extrapolates to 90%CL
        var_Weight[2]/= std::sqrt(50.0);   //----NNPDF variations are sampled from a Gaussian, so divide by sqrt(N)

        pdf_unc_central = 0.5*(TMath::Max(wgh_NNPDF[0]+var_Weight[2],TMath::Max(wgh_CT10[0]+var_Weight[0],wgh_MMHT2014[0]+var_Weight[1])) + TMath::Min(wgh_NNPDF[0]-var_Weight[2],TMath::Min(wgh_CT10[0]-var_Weight[0],wgh_MMHT2014[0]-var_Weight[1])));
        pdf_unc_sys = 0.5*(TMath::Max(wgh_NNPDF[0]+var_Weight[2],TMath::Max(wgh_CT10[0]+var_Weight[0],wgh_MMHT2014[0]+var_Weight[1])) - TMath::Min(wgh_NNPDF[0]-var_Weight[2],TMath::Min(wgh_CT10[0]-var_Weight[0],wgh_MMHT2014[0]-var_Weight[1])));

        pdf_unc_up = pdf_unc_central + pdf_unc_sys;
        pdf_unc_down = pdf_unc_central - pdf_unc_sys;
	//register derived variables back to ntuples
        tr.registerDerivedVar("PDF_Unc_Central", pdf_unc_central);
        tr.registerDerivedVar("PDF_Unc_Sys", pdf_unc_sys);
        tr.registerDerivedVar("PDF_Unc_Up", pdf_unc_up);
        tr.registerDerivedVar("PDF_Unc_Down", pdf_unc_down);

	//For Scale Variations
	tr.registerDerivedVar("Scaled_Variations_Up",upperBound);
	tr.registerDerivedVar("Scaled_Variations_Down",lowerBound);
}

void PDFUncertainty::operator()(NTupleReader& tr)
{
  //
    getPDFUncertainty(tr);
}


