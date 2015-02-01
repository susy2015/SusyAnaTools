from SusyAnaTools.TopTagger.topTagger_cfi import *

type3topTagger = topTagger.clone()
type3topTagger.taggingMode = cms.untracked.bool(True)

stopType3TopTagger = cms.Sequence( type3topTagger )

from SusyAnaTools.Skims.filterDoubles_cfi import *

topMassCuttype3topTagger = doublesFilter.clone()
topMassCuttype3topTagger.ResultSource = cms.InputTag("stopType3TopTagger:bestTopJetMass")
topMassCuttype3topTagger.lowCut = cms.double(80)
topMassCuttype3topTagger.highCut = cms.double(270)

mTbmTtCuttye3topTagger = doublesFilter.clone()
mTbmTtCuttye3topTagger.ResultSource = cms.InputTag("stopType3TopTagger:linearCombmTbJetPlusmTbestTopJet")
mTbmTtCuttye3topTagger.lowCut = cms.double(500)

MT2Cuttye3topTagger = doublesFilter.clone()
MT2Cuttye3topTagger.ResultSource = cms.InputTag("stopType3TopTagger:MT2")
MT2Cuttye3topTagger.lowCut = cms.double(300)

stopType3topTaggerAllCuts = cms.Sequence(
   topMassCuttype3topTagger *
   mTbmTtCuttye3topTagger *
   MT2Cuttye3topTagger
)
