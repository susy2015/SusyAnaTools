
from SusyAnaTools.Skims.RA2Cleaning_cff       import *
from SusyAnaTools.Skims.StopJets_cff           import *
from SusyAnaTools.Skims.StopLeptons_cff        import *
#from SusyAnaTools.Skims.StopPhotons_cff       import *
from SusyAnaTools.Skims.StopTrackIsolation_cff import *
from SusyAnaTools.Skims.StopTauJets_cff        import *
from SusyAnaTools.Skims.StopBTagJets_cff       import *

stopObjects = cms.Sequence(  
  stopPFJets *
  #stopMuons *     # use from RA2 for now, update soon
  #stopElectrons * # use from RA2 for now, update soon
  stopIsoTracks *
  stopTauJets *
  stopBJets
  #stopPhotons
)

stopObjectsOnPatTuples = cms.Sequence(  
  stopPFJets *
  #stopMuons *     # use from RA2 for now, update soon
  #stopElectrons * # use from RA2 for now, update soon
  stopIsoTracks *
  stopTauJets *
  stopBJets
  #stopPhotons
)

