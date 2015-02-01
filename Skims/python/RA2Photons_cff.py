
from PhysicsTools.PatAlgos.selectionLayer1.photonSelector_cfi import *
from PhysicsTools.PatAlgos.selectionLayer1.photonCountFilter_cfi import *

photonidcut = cms.string('pt > 70.0 && abs(eta) < 2.5 && hadronicOverEm < 0.05  && hasPixelSeed == 0 && ((isEB && sigmaIetaIeta < 0.01) || (isEE && sigmaIetaIeta < 0.03))')
photonIDCutTight  = cms.string('et > 50.0 && (abs(eta) < 1.4442 || (abs(eta) > 1.566 && abs(eta) < 2.5)) && '
                               'hadronicOverEm < 0.5  && userInt("passElectronConvVeto") > 0 && '
                               'hadTowOverEm < userFloat("hadTowOverEmTightCut") && '
                               'sigmaIetaIeta < userFloat("showerShapeTightCut")'
                              )

photonIDCutMedium = cms.string('et > 50.0 && (abs(eta) < 1.4442 || (abs(eta) > 1.566 && abs(eta) < 2.5)) && '
                               'hadronicOverEm < 0.5  && userInt("passElectronConvVeto") > 0 && '
                               'hadTowOverEm < userFloat("hadTowOverEmMediumCut") && '
                               'sigmaIetaIeta < userFloat("showerShapeMediumCut")'
                              )

photonIDCutLoose  = cms.string('et > 50.0 && (abs(eta) < 1.4442 || (abs(eta) > 1.566 && abs(eta) < 2.5)) && '
                               'hadronicOverEm < 0.5  && userInt("passElectronConvVeto") > 0 && '
                               'hadTowOverEm < userFloat("hadTowOverEmLooseCut") && '
                               'sigmaIetaIeta < userFloat("showerShapeLooseCut")'
                              )


from SusyAnaTools.Skims.addphotonuserdata_cfi import addphotonuserbasic
patPhotonsRA2    = addphotonuserbasic.clone()

patPhotonsID = cms.EDFilter(
  "PATPhotonSelector",
   src = cms.InputTag('patPhotonsRA2'),
   cut = photonIDCutLoose,
   filter = cms.bool(False),
)

patPhotonRefsID = cms.EDFilter(
  "PATPhotonRefSelector",
   src = cms.InputTag('patPhotonsRA2'),
   cut = photonIDCutLoose,
   filter = cms.bool(False)
)

ra2Photons = cms.Sequence(
    patPhotonsRA2
  + patPhotonsID
)

countPhotonsID = countPatPhotons.clone()
countPhotonsID.src = cms.InputTag("patPhotonsID")
countPhotonsID.minNumber = cms.uint32(1)

countPhotonRefsID = countPatPhotons.clone()
countPhotonRefsID.src = cms.InputTag("patPhotonsID")
countPhotonRefsID.minNumber = cms.uint32(1)
