#!/cvmfs/cms.cern.ch/slc6_amd64_gcc491/cms/cmssw/CMSSW_7_4_8/external/slc6_amd64_gcc491/bin/python

from optparse import OptionParser

import array, sys
# Check if we want the help, if not import ROOT (otherwise ROOT overwrites the help)
if '-h' not in sys.argv and '--help' not in sys.argv:
    from ROOT import TH1D, TMath, TFile, TCanvas, TF1, TH2D

def rebin1D(h, bins):
    """Rebin histo h to bins and recompute the errors."""
    new_h = TH1D("%s"%(h.GetName()), "%s"%(h.GetName()),
                 len(bins)-1, array.array('d', bins))
    new_h.Sumw2()
    for i in xrange(h.GetNbinsX()):
        bin_to_fill = new_h.FindBin(h.GetBinCenter(i+1))
        new_h.SetBinContent(bin_to_fill, h.GetBinContent(i+1) + new_h.GetBinContent(bin_to_fill))
        new_h.SetBinError(bin_to_fill, TMath.Sqrt(h.GetBinError(i+1)*h.GetBinError(i+1) + new_h.GetBinError(bin_to_fill)*new_h.GetBinError(bin_to_fill) ) )
    new_h.SetBinContent(new_h.GetNbinsX(), new_h.GetBinContent(new_h.GetNbinsX()) + new_h.GetBinContent(new_h.GetNbinsX()+1))
    return new_h


def calcBeffs(filePath):

    fileNames = ["bTagEfficiency_TTbarSingleLepT.root",
                 "bTagEfficiency_TTbarDiLep.root",
                 "bTagEfficiency_TTbarSingleLepTbar.root",
                 "bTagEfficiency_DYJetsToLL_HT_100to200.root",
                 "bTagEfficiency_DYJetsToLL_HT_200to400.root",
                 "bTagEfficiency_DYJetsToLL_HT_400to600.root",
                 "bTagEfficiency_DYJetsToLL_HT_600toInf.root",
                 "bTagEfficiency_ZJetsToNuNu_HT_600toInf.root",
                 "bTagEfficiency_ZJetsToNuNu_HT_400to600.root",
                 "bTagEfficiency_ZJetsToNuNu_HT_200to400.root",
                 "bTagEfficiency_ZJetsToNuNu_HT_100to200.root",
                 "bTagEfficiency_ttHJetTobb.root",
                 "bTagEfficiency_ttHJetToNonbb.root",
                 "bTagEfficiency_tW_top.root",
                 "bTagEfficiency_tW_antitop.root",
                 "bTagEfficiency_ZZ.root",
                 "bTagEfficiency_WZ.root",
                 "bTagEfficiency_WW.root",
                 "bTagEfficiency_WWZ.root",
                 "bTagEfficiency_WZZ.root",
                 "bTagEfficiency_ZZZ.root",
                 "bTagEfficiency_TTZToQQ.root",
                 "bTagEfficiency_TTZToLLNuNu.root",
                 "bTagEfficiency_TTWJetsToQQ.root",
                 "bTagEfficiency_TTWJetsToLNu.root",
                 "bTagEfficiency_TTGJets.root",
                 "bTagEfficiency_DYJetsToLL_Inc.root"]

    fout = TFile.Open("bTagEffHists.root", "RECREATE")

    for filename in fileNames:

        f = TFile.Open(filePath + filename)

        if not f is None:

            n_eff_b = f.Get("h_eff_b")
            n_eff_c = f.Get("h_eff_c")
            n_eff_udsg = f.Get("h_eff_udsg")
            d_eff_b = f.Get("d_eff_b")
            d_eff_c = f.Get("d_eff_c")
            d_eff_udsg = f.Get("d_eff_udsg")

            n_eff_b.Sumw2()
            n_eff_c.Sumw2()
            n_eff_udsg.Sumw2()
            d_eff_b.Sumw2()
            d_eff_c.Sumw2()
            d_eff_udsg.Sumw2()

            dataset = filename.replace(".root", "").replace("bTagEfficiency_", "")
            eff_b = n_eff_b.Clone("eff_b_" + dataset)
            eff_c = n_eff_c.Clone("eff_c_" + dataset)
            eff_udsg = n_eff_udsg.Clone("eff_udsg_" + dataset)

            eff_b.Divide(d_eff_b)
            eff_c.Divide(d_eff_c)
            eff_udsg.Divide(d_eff_udsg)

            fout.cd()
            eff_b.Write()
            eff_c.Write()
            eff_udsg.Write()

        f.Close()

    fout.Close()


def calcISR(filePath):

    fileNames = [
        "DYJetsToLL_HT_100to200_ISRJet.root",
        "DYJetsToLL_HT_1200to2500_ISRJets.root",
        "DYJetsToLL_HT_200to400_ISRJets.root",
        "DYJetsToLL_HT_2500toInf_ISRJets.root",
        "DYJetsToLL_HT_400to600_ISRJets.root",
        "DYJetsToLL_HT_600to800_ISRJets.root",
        "DYJetsToLL_HT_800to1200_ISRJets.root",
        "DYJetsToLL_Inc_ISRJets.root",
        "TTbarDiLep_ISRJets.root",
        "TTbarSingleLepT_ISRJets.root",
        "TTbarSingleLepTbar_ISRJets.root",
        "ZJetsToNuNu_HT_100to200_ISRJets.root",
        "ZJetsToNuNu_HT_1200to2500_ISRJets.root",
        "ZJetsToNuNu_HT_200to400_ISRJets.root",
        "ZJetsToNuNu_HT_2500toInf_ISRJets.root",
        "ZJetsToNuNu_HT_400to600_ISRJets.root",
        "ZJetsToNuNu_HT_600to800_ISRJets.root",
        "ZJetsToNuNu_HT_800to1200_ISRJets.root",
        "ZZTo2L2Nu_ISRJets.root",
        "ZZTo2L2Q_ISRJets.root",
        "ZZTo2Q2Nu_ISRJets.root",
        "ZZTo4L_ISRJets.root",
        "ZZTo4Q_ISRJets.root",
        "ZZZ_ISRJets_0.root",
        "tW_antitop_NoHad_ISRJets.root",
        "tW_top_NoHad_ISRJets.root",
        ]

    fout = TFile.Open("ISRhistos.root", "RECREATE")

    for filename in fileNames:

        f = TFile.Open(filePath + "/" + filename)

        if not f is None:

            h = f.Get("NJetsISR")

            h.Sumw2()

            dataset = filename.replace("_ISRJets.root", "")
            if filename ==  "ZZZ_ISRJets_0.root":
                dataset = filename.replace("_ISRJets_0.root", "")
            if filename ==  "DYJetsToLL_HT_100to200_ISRJet.root":
                dataset = filename.replace("_ISRJet.root", "")
                
            h2 = h.Clone("NJetsISR_" + dataset)
            bins = [0,1,2,3,4,5,6,7]
            h2 = rebin1D(h2, bins)
            fout.cd()
            h2.Write()


        f.Close()

    fout.Close()



if __name__ ==  "__main__":

    parser = OptionParser()
    parser.add_option("-d", "--dir", dest="directory",
                      help="Grab histogram from directory", metavar="DIR")
    parser.add_option("--btag", dest="btag", default=False,
                      help="Aggregate the b-tag files.", action='store_true')
    parser.add_option("--isr", dest="isr", default=False,
                      help="Aggregate the ISR files.", action='store_true')

    (options, args) = parser.parse_args()

    if options.btag:
        calcBeffs(options.directory)
    if options.isr:
        calcISR(options.directory)

