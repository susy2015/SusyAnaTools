
import FWCore.ParameterSet.Config as cms

#goodVerticesRA2 = cms.EDFilter(
#  "VertexSelector",
#  filter = cms.bool(False),
#  src = cms.InputTag("offlinePrimaryVertices"),
#  cut = cms.string("!isFake && ndof > 4 && abs(z) <= 24 && position.rho < 2")
#)

oneGoodVertex = cms.EDFilter(
  "VertexCountFilter",
  src = cms.InputTag("goodVertices"),
  minNumber = cms.uint32(1),
  maxNumber = cms.uint32(999)
)
