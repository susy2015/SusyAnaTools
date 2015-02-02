import FWCore.ParameterSet.Config as cms

goldenTauSelector = cms.EDFilter(
  "GoldenTauSelector",
  jetSrc             = cms.InputTag("patJetsAK5PF"),
  pfJetSelection     = cms.string("pt>0"),
  metSrc             = cms.InputTag("patMETsPF"),
  vtxSrc             = cms.InputTag("goodVertices"),

  MTCut              = cms.double(90.),
  dPhiJetMETCut      = cms.double(0.500),

  nTracksCut         = cms.vint32(4, 5),
  ptDivFornTracksCut = cms.vdouble(0, 80, -1),

  maxD0              = cms.double(0.2),
  maxDz              = cms.double(0.5),

  deltaRTrkCone      = cms.double(0.3),
  deltaRleadingChgPartToJetCut = cms.double(0.15),

  doTauVeto          = cms.bool(False),
  doFillTree         = cms.bool(False),
)
