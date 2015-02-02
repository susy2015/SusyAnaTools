# $Id: jesChange_cfi.py,v 1.2 2012/12/20 20:24:30 seema Exp $

import FWCore.ParameterSet.Config as cms

# Define CHS correctors
from JetMETCorrections.Configuration.DefaultJEC_cff import *
ak5PFchsL1Fastjet  = ak5PFL1Fastjet.clone ( algorithm = cms.string('AK5PFchs') )
ak5PFchsL2Relative = ak5PFL2Relative.clone( algorithm = cms.string('AK5PFchs') )
ak5PFchsL3Absolute = ak5PFL3Absolute.clone( algorithm = cms.string('AK5PFchs') )
ak5PFchsResidual   = ak5PFResidual.clone  ( algorithm = cms.string('AK5PFchs') )

ak5PFchsL1FastL2L3         = ak5PFL1FastL2L3Residual.clone( correctors = cms.vstring('ak5PFchsL1Fastjet', 'ak5PFchsL2Relative', 'ak5PFchsL3Absolute') )
ak5PFchsL1FastL2L3Residual  = ak5PFL1FastL2L3Residual.clone( correctors = cms.vstring('ak5PFchsL1Fastjet', 'ak5PFchsL2Relative', 'ak5PFchsL3Absolute', 'ak5PFchsResidual') )

jesChange = cms.EDProducer(
    'JESChange',
    Jets       = cms.InputTag('patJetsPF'), # The input jet collection
    METs       = cms.InputTag('patMETsPF'), # The input met collection
    JECLevel   = cms.string('ak5PFchsL1FastL2L3'),
    Debug      = cms.bool(False)
)

# used in defalut config
newJetsMET          = jesChange.clone()
newJetsMET.JECLevel = cms.string('ak5PFchsL1FastL2L3')

# JECLevels for MC
newJetsMETMC          = jesChange.clone()
newJetsMETMC.JECLevel = cms.string('ak5PFchsL1FastL2L3')

# JECLevels for data (include Residual corrections)
newJetsMETData          = jesChange.clone()
newJetsMETData.JECLevel = cms.string('ak5PFchsL1FastL2L3Residual')

# make default RA2Jet collections
from SusyAnaTools.Skims.basicJetSelector_cfi import *
from PhysicsTools.PatAlgos.selectionLayer1.jetCountFilter_cfi import *

newpatJetsPFchsPt30     = selectedPatJets.clone()
newpatJetsPFchsPt30.src = cms.InputTag("newJetsMET")
newpatJetsPFchsPt30.cut = cms.string('pt > 30')

newpatJetsPFchsPt50Eta25     = selectedPatJets.clone()
newpatJetsPFchsPt50Eta25.src = cms.InputTag('newpatJetsPFchsPt30')
newpatJetsPFchsPt50Eta25.cut = cms.string('pt > 50 & abs(eta) < 2.5')

# PFJets - filters
newcountJetsPFchsPt50Eta25           = countPatJets.clone()
newcountJetsPFchsPt50Eta25.src       = cms.InputTag('newpatJetsPFchsPt50Eta25')
newcountJetsPFchsPt50Eta25.minNumber = cms.uint32(3)


# apply phi modulation correction to pfMetsPF
# As suggested by Christian Veeklen, take most recent parametrization from
# cvs up -r 1.6 JetMETCorrections/Type1MET/python/pfMETsysShiftCorrections_cfi.py

pfMEtSysShiftCorrParameters_2012runABCvsNvtx_data = cms.PSet(
    px = cms.string("+0.2661 + 0.3217*Nvtx"),
    py = cms.string("-0.2251 - 0.1747*Nvtx")
)

pfMEtSysShiftCorrParameters_2012runABCvsNvtx_mc = cms.PSet(
    px = cms.string("+0.1166 + 0.0200*Nvtx"),
    py = cms.string("+0.2764 - 0.1280*Nvtx")
)

# default config
from JetMETCorrections.Type1MET.pfMETsysShiftCorrections_cfi import *

# default
patMETPhiCorr = pfMEtSysShiftCorr.clone()
patMETPhiCorr.parameter = pfMEtSysShiftCorrParameters_2012runABCvsNvtx_data

# for data
patMETPhiCorrData = pfMEtSysShiftCorr.clone()
patMETPhiCorrData.parameter = pfMEtSysShiftCorrParameters_2012runABCvsNvtx_data

# for mc
patMETPhiCorrMC = pfMEtSysShiftCorr.clone()
patMETPhiCorrMC.parameter = pfMEtSysShiftCorrParameters_2012runABCvsNvtx_mc

newMETwPhiCorr = cms.EDProducer("CorrectedPATMETProducer",
                     src = cms.InputTag('newJetsMET'),
                     applyType0Corrections = cms.bool(False),
                     srcCHSSums = cms.VInputTag(
                      cms.InputTag('pfchsMETcorr', 'type0')
                     ),
                     type0Rsoft = cms.double(0.6),
                     applyType1Corrections = cms.bool(True),
                     srcType1Corrections = cms.VInputTag(
                     cms.InputTag('patMETPhiCorr')
                     ),
                     applyType2Corrections = cms.bool(False)
                     ) 
  
# PF sequences
newra2PFchsJets = cms.Sequence(
  newJetsMET *
  selectedVerticesForMEtCorr *  # needed for met-phi corr
  patMETPhiCorr       * # create met-phi correction
  newMETwPhiCorr      * # phi-corrected met correction
  newpatJetsPFchsPt30 *
  newpatJetsPFchsPt50Eta25 
)

