import FWCore.ParameterSet.Config as cms

printDecay = cms.EDFilter( "genDecayStringMaker",
   src = cms.InputTag( "genParticles" ),
#    untracked bool printP4 = true
#    untracked bool printPtEtaPhi = true
   status = cms.untracked.vint32( 3 ),
   keyDecayStrs = cms.vstring(),
)
