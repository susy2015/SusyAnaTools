import FWCore.ParameterSet.Config as cms

myGenParticlesForJets = cms.EDProducer("myGenJetsPartSelector",
     src = cms.InputTag("genParticles"),
     ignoreParticleIDs = cms.vuint32(
          1000022,
          1000012, 1000014, 1000016,
          2000012, 2000014, 2000016,
          1000039, 5100039,
          4000012, 4000014, 4000016,
          9900012, 9900014, 9900016,
          39),
     partonicFinalState = cms.bool(False),
     excludeResonances = cms.bool(True),
     excludeFromResonancePids = cms.vuint32(12, 13, 14, 16),
     tausAsJets = cms.bool(False),
     ignoreParticleDecayIDs = cms.vint32(),
     invertignoreParticleDecay = cms.bool(False),
)
