import FWCore.ParameterSet.Config as cms

# apply phi modulation correction to pfMetsPF
# As suggested by Christian Veeklen, take most recent parametrization from
# cvs up -r 1.6 JetMETCorrections/Type1MET/python/pfMETsysShiftCorrections_cfi.py
# XXX: However instead, I have the following parts copied from the above cfi.

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

patMETPhiCorrData = pfMEtSysShiftCorr.clone()
patMETPhiCorrMC = pfMEtSysShiftCorr.clone()

patMETPhiCorrData.parameter = pfMEtSysShiftCorrParameters_2012runABCvsNvtx_data
patMETPhiCorrMC.parameter = pfMEtSysShiftCorrParameters_2012runABCvsNvtx_mc

newMETwPhiCorrData = cms.EDProducer("CorrectedPATMETProducer",
                                  src = cms.InputTag('patMETsPF'),
                                  applyType0Corrections = cms.bool(False),
                                  srcCHSSums = cms.VInputTag(
                                     cms.InputTag('pfchsMETcorr', 'type0')
                                  ),
                                  type0Rsoft = cms.double(0.6),
                                  applyType1Corrections = cms.bool(True),
                                  srcType1Corrections = cms.VInputTag(
                                     cms.InputTag('patMETPhiCorrData')
                                  ),
                                  applyType2Corrections = cms.bool(False)
                          )

newMETwPhiCorrMC = newMETwPhiCorrData.clone()
newMETwPhiCorrMC.srcType1Corrections = cms.VInputTag(
                    cms.InputTag('patMETPhiCorrMC')
                 )

metphi_seq = cms.Sequence(
   selectedVerticesForMEtCorr * # needed for met-phi corr
   patMETPhiCorrData * patMETPhiCorrMC * # create met-phi correction
   newMETwPhiCorrData * newMETwPhiCorrMC # phi-corrected met correction   
)
