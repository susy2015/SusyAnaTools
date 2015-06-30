import FWCore.ParameterSet.Config as cms

triggerProducer = cms.EDProducer('prodTriggerResults',
trigTagArg1  = cms.string('TriggerResults'),
trigTagArg2  = cms.string(''),
trigTagArg3  = cms.string('HLT'),
VertexCollection  = cms.InputTag('offlineSlimmedPrimaryVertices'),
triggerNameList  =   cms.string(#space-delimited list of trigger names
            'HLT_PFHT350_PFMET100_NoiseCleaned_v1 '\
            'HLT_PFMET170_NoiseCleaned_v1 '\
            'HLT_PFMET170_NoiseCleaned_v2 '\
            'HLT_PFHT350_v1 '\
            'HLT_PFHT800_v1 '\
            'HLT_PFHT900_v1 '\
            'HLT_Ele27_eta2p1_WP85_Gsf_v1 '\
            'HLT_IsoMu20_eta2p1_IterTrk02_v1 '\
            'HLT_PFHT350_PFMET120_NoiseCleaned_v1 '\
            'HLT_Mu15_IsoVVVL_PFHT350_PFMET70_v1 '\
            'HLT_Ele15_IsoVVVL_PFHT350_PFMET70_v1 '\
            'HLT_Mu15_IsoVVVL_PFHT400_PFMET70_v1 '\
            'HLT_Ele15_IsoVVVL_PFHT400_PFMET70_v1 '\
            'HLT_Photon90_v1 '\
            'HLT_Photon75_v1 '\
            'HLT_Photon90_CaloIdL_PFHT500_v1 '\
            'HLT_Photon90_CaloIdL_PFHT600_v1 '\
            'HLT_DoubleEle8_CaloIdM_Mass8_PFHT300_v2 '\
            'HLT_DoubelMu8_Mass8_PFHT300_v2 '
            )
)
