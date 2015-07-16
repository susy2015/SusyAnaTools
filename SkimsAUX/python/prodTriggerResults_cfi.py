import FWCore.ParameterSet.Config as cms

triggerProducer = cms.EDProducer('prodTriggerResults',
   trigTagSrc = cms.InputTag("TriggerResults"),
   triggerNameList = cms.vstring(
            'HLT_PFHT350_PFMET100_NoiseCleaned',
            'HLT_PFMET170_NoiseCleaned',
            'HLT_PFHT350',
            'HLT_PFHT800',
            'HLT_PFHT900',
            'HLT_Ele27_eta2p1_WP85_Gsf',
            'HLT_IsoMu20_eta2p1_IterTrk02',
            'HLT_PFHT350_PFMET120_NoiseCleaned',
            'HLT_Mu15_IsoVVVL_PFHT350_PFMET70',
            'HLT_Ele15_IsoVVVL_PFHT350_PFMET70',
            'HLT_Mu15_IsoVVVL_PFHT400_PFMET70',
            'HLT_Ele15_IsoVVVL_PFHT400_PFMET70',
            'HLT_Photon90',
            'HLT_Photon75',
            'HLT_Photon90_CaloIdL_PFHT500',
            'HLT_Photon90_CaloIdL_PFHT600',
            'HLT_DoubleEle8_CaloIdM_Mass8_PFHT300',
            'HLT_DoubelMu8_Mass8_PFHT300'
            ),
   debug = cms.bool(False),
)
