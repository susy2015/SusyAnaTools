import FWCore.ParameterSet.Config as cms

triggerProducer = cms.EDProducer('prodTriggerResults',
   trigTagSrc = cms.InputTag("TriggerResults"),
   triggerNameList = cms.vstring(
            'HLT_PFHT350_PFMET100_NoiseCleaned',
            'HLT_PFMET170_NoiseCleaned',
            'HLT_PFMET100_PFMHT100_IDTight',
            'HLT_PFMET110_PFMHT110_IDTight',
            'HLT_PFMET120_PFMHT120_IDTight',
            'HLT_PFHT350',
            'HLT_PFHT800',
            'HLT_PFHT900',
            'HLT_IsoMu16_eta2p1_CaloMET30',
            'HLT_Mu45_eta2p1',
            'HLT_Mu30_TkMu11',
            'HLT_PFHT350_PFMET120_NoiseCleaned',
            'HLT_Mu15_IsoVVVL_PFHT350_PFMET70',
            'HLT_Ele15_IsoVVVL_PFHT350_PFMET70',
            'HLT_Mu15_IsoVVVL_PFHT400_PFMET70',
            'HLT_Ele15_IsoVVVL_PFHT400_PFMET70',
   ),
   debug = cms.bool(False),
)
