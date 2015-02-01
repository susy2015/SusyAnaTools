import FWCore.ParameterSet.Config as cms

smsModelFilter = cms.EDFilter("SMSModelFilter",
                              SusyScanTopology   = cms.string("T2tt"),
                              SusyScanMotherMass = cms.double(1175),
                              SusyScanLSPMass    = cms.double(750),
                              SusyScanFracLSP    = cms.double(0.0),
                              Debug              = cms.bool(False)
                              )
