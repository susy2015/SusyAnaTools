#include "LHAPDF/LHAPDF.h"
#include "NTupleReader.h"
#include "baselineDef.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include"TH1.h"
#include <iostream>
#include <cstdio>
#include <string>
#include <ctime>
#include <iomanip>
#include <cmath>
#include "tdrstyle.h"





double pdf_central;
double pdf_symmetric;


class PDFweight
{

 public:
  PDFweight()
    {
      
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

  
  void getPDFWeight(NTupleReader& tr)
  {


    //*******************************************
    const std::vector<double> &ScaleWeightsMiniAOD = tr.getVec<double>("ScaleWeightsMiniAOD");
    const double x1  =   tr.getVar<double>("x1");
    const double x2  =   tr.getVar<double>("x2");
    const double  Q  =   tr.getVar<double>("q");
    const int id1    =   tr.getVar<int>("id1");
    const int id2    =   tr.getVar<int>("id2");



      
  //*****************
  //Vector to be stored.                                                                                                           
    std::vector<double> *pdfweights = new std::vector<double>();
    std::vector<int> *pdfids = new std::vector<int>();



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
      pdfid1 = 1000 + i +1;
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
      pdfid2 = 2000 + i +1;
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
      pdfid3 = 3000 + i +1;
      (*pdfweights).push_back(lhaweight3);
      (*pdfids).push_back(pdfid3);
    }


    tr.registerDerivedVec("PDFWeights", pdfweights);
    tr.registerDerivedVec("PDFIds", pdfids);

  }
  void operator()(NTupleReader& tr)
  {
    getPDFWeight(tr);
  }
 private:
  std::vector<LHAPDF::PDF*> pdf1;
  std::vector<LHAPDF::PDF*> pdf2;
  std::vector<LHAPDF::PDF*> pdf3;
};
//********************************************


int main()
{



  char nBase[] = "root://cmseos.fnal.gov//store/user/lpcsusyhad/Stop_production/Spring16_80X_Nov_2016_Ntp_v11X_new_IDs/TTJets_SingleLeptFromT_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/Spring16_80X_Nov_2016_Ntp_v11p0_new_IDs_TTJets_SingleLeptFromT/161111_230825/0000/stopFlatNtuples_1.root";

  TChain *ch = new TChain("stopTreeMaker/AUX");

    size_t t0 = clock();

    char chname[512];
    for(int i = 1; i <= 1; ++i)
    {
        sprintf(chname, nBase, i);
        ch->Add(chname);
    }
    /*
    TFile *hfile_Unc = new TFile("TTZ_PDF_Unc.root","RECREATE");
    TH1D *hCentral_PDF = new TH1D("h_Central_PDF", "Central Value of PDF", 50, 0.2,1.2);
    TH1D *hSys_PDF = new TH1D("h_Symmetric_PDF", "Symmetric Value of PDF", 50,0.,0.5);

    TH1D *TEST_central_NNPDF = new TH1D("TEST_hCentralNNPDF", "TESTCentral_Value_NNPDF", 50, 0.5, 1.5);
    TH1D *TEST_central_CT10 = new TH1D("TEST_hCentralCT10", "TESTCentral_Value_CT10", 50, 0.5, 1.2);
    TH1D *TEST_central_MMHT = new TH1D("TEST_hCentralMMHT", "TESTCentral_Value_MMHT", 50, 0.5, 1.2); 
    TH1D *central_NNPDF = new TH1D("hCentralNNPDF", "Central_Value_NNPDF", 50, 0.8, 1.2);
    TH1D *central_CT10 = new TH1D("hCentralCT10", "Central_Value_CT10", 50, 0.8, 1.2);
    TH1D *central_MMHT = new TH1D("hCentralMMHT", "Central_Value_MMHT", 50, 0.8, 1.2);

    TH1D *var_NNPDF = new TH1D("hVarNNPDF", "Uncertainity_NNPDF",50,0.,0.5);
    TH1D *var_CT10 = new TH1D("hVarCT10", "Uncertainity_CT10",50,0.,0.5);
    TH1D *var_MMHT = new TH1D("hVarMMHT", "Uncertainity_MMHT",50,0.,0.5);

    */
    NTupleReader tr(ch);
    tr.setReThrow(false);
    PDFweight pdfs;
    BaselineVessel blv(tr);
    tr.registerFunction(blv);
    tr.registerFunction(pdfs);
    std::cout << "NEVTS: " << tr.getNEntries() << std::endl;

    while(tr.getNextEvent())
    {
      if(tr.getEvtNum() == 1)
      {
        tr.printTupleMembers();
        FILE * fout = fopen("NTupleTypes.txt", "w");
        tr.printTupleMembers(fout);
        fclose(fout);
      }

      std::cout << "MET " << tr.getVar<double>("met")  << " nTop" << tr.getVar<int>("nTopCandSortedCnt") << std::endl;
    }
    ch->Reset();
	// All Variations Initialize to zero.
    /*
	double var_Weight[3];
	for(int i = 0; i < 3; i++) var_Weight[i] = 0.0;


	//PDF 1 NNPDF30.... 
	//PDF 2 CT10... 
	//PDF 3 MMHT2014... 
	double wgh_NNPDF[100];
	double wgh_CT10[52];
	double wgh_MMHT2014[50];

	//***************************

	//double cent_avg_NNPDF[100];//Average central value for particular Pdf For each event.
	for(int in =0; in < 100; in++){
	  wgh_NNPDF[in] = -1.0;

	  if(PDFIds.at(in) < 1200){
	    wgh_NNPDF[in] = PDFWeights.at(in);
	    //  cent_avg_NNPDF += wgh_NNPDF[in];
	    TEST_central_NNPDF->Fill(wgh_NNPDF[in]);
	  }
	}
	double 	cent_avg_NNPDF = wgh_NNPDF[50];
	central_NNPDF->Fill(cent_avg_NNPDF);

	//***********************
	//double cent_avg_CT10 =0.0;
	for(int ic =0; ic < 52; ic++){
	  wgh_CT10[ic] = -1.0;
	  if(PDFIds.at(ic+100) > 1200 && PDFIds.at(ic+100) < 2053){
	    wgh_CT10[ic] = PDFWeights.at(ic+100);
	    //  cent_avg_CT10 += wgh_CT10[ic];
	    TEST_central_CT10->Fill(wgh_CT10[ic]);
	    }
	  }
	double cent_avg_CT10 = wgh_CT10[0];
	central_CT10->Fill(cent_avg_CT10);
	//*************************
	//double cent_avg_MMHT = 0.0;
	for(int im =0; im < 50; im++){
	  wgh_MMHT2014[im] = 0.0;
	  if(PDFIds.at(im+152) > 2999){
	    wgh_MMHT2014[im] = PDFWeights.at(im+152);
	    //cent_avg_MMHT += wgh_MMHT2014[im];
	    TEST_central_MMHT->Fill(wgh_MMHT2014[im]);
	  }
	}
	double cent_avg_MMHT = wgh_MMHT2014[0];
	central_MMHT->Fill(cent_avg_MMHT);


	//**************************      

	//PDF 1 NNPDF30....
	for (int k= 1; k <= 49; k++){
	  var_Weight[0] += 0.25*(wgh_NNPDF[2*k]-wgh_NNPDF[2*k-1])*(wgh_NNPDF[2*k]-wgh_NNPDF[2*k-1]);
	}
	var_Weight[0] +=0.25*(wgh_NNPDF[99]-wgh_NNPDF[98])*(wgh_NNPDF[99]-wgh_NNPDF[98]);
	
	//var_NNPDF->Fill(var_Weight[0]);

	//CT10
	for (int k= 1; k <= 25; k++){
          var_Weight[1] += 0.25*(wgh_CT10[2*k]-wgh_CT10[2*k-1])*(wgh_CT10[2*k]-wgh_CT10[2*k-1]);
	}
	var_Weight[1] += 0.25*(wgh_CT10[51]-wgh_CT10[50])*(wgh_CT10[51]-wgh_CT10[50]);

	//	var_CT10->Fill(var_Weight[1]);
	//MMHT
	for (int k= 1; k <= 24; k++){
          var_Weight[2] += 0.25*(wgh_MMHT2014[2*k]-wgh_MMHT2014[2*k-1])*(wgh_MMHT2014[2*k]-wgh_MMHT2014[2*k-1]);
        }
	var_Weight[2] += 0.25*(wgh_MMHT2014[49]-wgh_MMHT2014[48])*(wgh_MMHT2014[49]-wgh_MMHT2014[48]);
	//var_MMHT->Fill(var_Weight[2]);

	//Get std error
	for (int k=0;k<3;k++) var_Weight[k] = std::sqrt(var_Weight[k]);
	var_Weight[1] /= 1.645;//----CTEQ6 variations correspond to 68% CL, so this factor extrapolates to 90%CL
	var_Weight[0]/= std::sqrt(50.0);//----NNPDF variations are sampled from a Gaussian, so divide by sqrt(N)

	pdf_central = 0.5*(TMath::Max(wgh_NNPDF[0]+var_Weight[0],TMath::Max(wgh_CT10[0]+var_Weight[1],wgh_MMHT2014[0]+var_Weight[2])) + TMath::Min(wgh_NNPDF[0]-var_Weight[0],TMath::Min(wgh_CT10[0]-var_Weight[1],wgh_MMHT2014[0]-var_Weight[2])));

	pdf_symmetric = 0.5*(TMath::Max(wgh_NNPDF[0]+var_Weight[0],TMath::Max(wgh_CT10[0]+var_Weight[1],wgh_MMHT2014[0]+var_Weight[2])) - TMath::Min(wgh_NNPDF[0]-var_Weight[0],TMath::Min(wgh_CT10[0]-var_Weight[1],wgh_MMHT2014[0]-var_Weight[2])));



	var_Weight[0] = var_Weight[0]/cent_avg_NNPDF;
        var_Weight[1] =var_Weight[1]/cent_avg_CT10;
        var_Weight[2] =var_Weight[2]/cent_avg_MMHT;

        //**********************                                                                       
        var_NNPDF->Fill(var_Weight[0]);
	var_CT10->Fill(var_Weight[1]);
	var_MMHT->Fill(var_Weight[2]);

	hCentral_PDF->Fill(pdf_central);
	//Relative uncertainity
	if(pdf_central > 0) pdf_symmetric = pdf_symmetric/pdf_central;
	hSys_PDF->Fill(pdf_symmetric);

	*/
    return 0;
}

/*
    // Plotting
    setTDRStyle();

    //  For the Global title:
    tdrStyle->SetOptTitle(1);
    tdrStyle->SetTitleFont(42,"");
    tdrStyle->SetTitleColor(1);
    tdrStyle->SetTitleTextColor(1);
    tdrStyle->SetTitleFillColor(0);
    tdrStyle->SetTitleFontSize(0.1);
    tdrStyle->SetTitleAlign(13);
    tdrStyle->SetTitleX(0.6);
    tdrStyle->SetTitleH(0.05);
    tdrStyle->SetTitleBorderSize(0);
    tdrStyle->SetTitleAlign(13);
    tdrStyle->SetTitleX(0.19);
    tdrStyle->SetTitleH(0.038);
                                                                         
    //  For the frame
    tdrStyle->SetFrameBorderMode(0);
    tdrStyle->SetFrameBorderSize(1);
    tdrStyle->SetFrameFillColor(kBlack);
    tdrStyle->SetFrameFillStyle(0);
    tdrStyle->SetFrameLineColor(kBlack);
    tdrStyle->SetFrameLineStyle(0);
    tdrStyle->SetFrameLineWidth(1);
 
    //  For the Pad
    tdrStyle->SetPadBorderMode(0);
    tdrStyle->SetPadColor(kWhite);
    tdrStyle->SetPadGridX(false);
    tdrStyle->SetPadGridY(false);
    tdrStyle->SetGridColor(0);
    tdrStyle->SetGridStyle(3);
    tdrStyle->SetGridWidth(1);
 
    //  For the axis
    tdrStyle->SetAxisColor(1,"XYZ");
    tdrStyle->SetTickLength(0.03,"XYZ");
    tdrStyle->SetNdivisions(505,"XYZ");
    tdrStyle->SetPadTickX(1);
    tdrStyle->SetPadTickY(1);
    tdrStyle->SetStripDecimals(kFALSE);
 
    tdrStyle->SetLabelSize(0.050, "XYZ");
 
    tdrStyle->SetPadTopMargin(0.1); tdrStyle->SetPadBottomMargin(0.15);
    tdrStyle->SetPadLeftMargin(0.15); tdrStyle->SetPadRightMargin(0.15);
 
    tdrStyle->SetOptStat(1111);
 
    tdrStyle->SetHistLineWidth(1);
 
    tdrStyle->SetPaintTextFormat("4.2f");

    tdrStyle->SetTitleXOffset(5.50); tdrStyle->SetTitleYOffset(6.50);

    /*
    hCentral_PDF->Write();
    TEST_central_NNPDF->Write();
    TEST_central_CT10->Write();
    TEST_central_MMHT->Write();
    central_NNPDF->Write();
    central_CT10->Write();
    central_MMHT->Write();
    var_NNPDF->Write();
    var_CT10->Write();
    var_MMHT->Write();
    hSys_PDF->Write();
    */

    /*    TCanvas *c1 = new TCanvas("PDF_UNC", "PDF Uncertainties",10,10, 800, 800);
    c1->SetFillColor(41);
    c1->Divide(2,2);

    c1->cd(1);
    var_NNPDF->GetXaxis()->SetTitle("uncertainty");
    var_NNPDF->GetYaxis()->SetTitle("Events");
    var_NNPDF->Draw();
    c1->Draw();
    */
    //hfile_Unc->Close();
	  
 

