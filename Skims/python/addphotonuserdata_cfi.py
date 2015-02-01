import FWCore.ParameterSet.Config as cms

addphotonuserbasic = cms.EDProducer("AddPhotonUserData",
    debug          = cms.bool(False),
    debugString    = cms.string("addphotonuserdata"),
    photonLabel    = cms.InputTag("patPhotons"),
    floatLabels    = cms.VInputTag(cms.InputTag("kt6PFJetsForIsolation","rho")),
    floatNames     = cms.vstring("rho25"),
#    floatLabels    = cms.VInputTag(),
#    floatNames     = cms.vstring(),
    embedConversionInfo = cms.bool(True),
    gsfElectronLabel = cms.InputTag("gsfElectrons"),
    conversionsLabel = cms.InputTag("conversions"),
    beamspotLabel    = cms.InputTag("offlineBeamSpot"),
    useAlternateIsolations = cms.bool(True),
    vetoConeSize     = cms.double(0.4),
    candidateLabel   = cms.InputTag("particleFlow"),
    vertexLabel      = cms.InputTag("offlinePrimaryVertices"),
    userData = cms.PSet(
        userCands = cms.PSet(
            src = cms.VInputTag("")
        ),
        userInts = cms.PSet(
            src = cms.VInputTag("")
        ),
        userFloats = cms.PSet(
            src = cms.VInputTag("")
        ),
        userClasses = cms.PSet(
            src = cms.VInputTag("")
        ),
        userFunctionLabels = cms.vstring('hcalIsoConeDR03_2012','hcalIsoConeDR04_2012',
                                         'pfChargedEA','pfNeutralEA','pfGammaEA',
                                         'pfChargedEANew','pfNeutralEANew','pfGammaEANew',
                                         'hadTowOverEmTightCut' ,'showerShapeTightCut' ,'pfChargedRelTightCut' ,'pfNeutralRelTightCut' ,'pfGammaRelTightCut', 'pfChargedTightCut' ,'pfNeutralTightCut' ,'pfGammaTightCut',
                                         'hadTowOverEmMediumCut','showerShapeMediumCut','pfChargedRelMediumCut','pfNeutralRelMediumCut','pfGammaRelMediumCut','pfChargedMediumCut','pfNeutralMediumCut','pfGammaMediumCut',
                                         'hadTowOverEmLooseCut' ,'showerShapeLooseCut' ,'pfChargedRelLooseCut' ,'pfNeutralRelLooseCut' ,'pfGammaRelLooseCut', 'pfChargedLooseCut' ,'pfNeutralLooseCut' ,'pfGammaLooseCut'
        ),
        userFunctions = cms.vstring(
            """hcalTowerSumEtConeDR03 +
               (hadronicOverEm - hadTowOverEm)*superCluster.energy/cosh(superCluster.eta)""", #hcalIsoConeDR03_2012
            """hcalTowerSumEtConeDR04 +
               (hadronicOverEm - hadTowOverEm)*superCluster.energy/cosh(superCluster.eta)""", #hcalIsoConeDR04_2012
            """?0.0   <= abs(superCluster.eta) < 1.0   ? 0.002 :
               ?1.0   <= abs(superCluster.eta) < 1.479 ? 0.003 :
               ?1.479 <= abs(superCluster.eta) < 2.0   ? 0.004 :
               ?2.0   <= abs(superCluster.eta) < 2.2   ? 0.006 :
               ?2.2   <= abs(superCluster.eta) < 2.3   ? 0.006 :
               ?2.3   <= abs(superCluster.eta) < 2.4   ? 0.004 :
                                                         0.003""", #chargedEA
            """?0.0   <= abs(superCluster.eta) < 1.0   ? 0.024 :
               ?1.0   <= abs(superCluster.eta) < 1.479 ? 0.037 :
               ?1.479 <= abs(superCluster.eta) < 2.0   ? 0.037 :
               ?2.0   <= abs(superCluster.eta) < 2.2   ? 0.034 :
               ?2.2   <= abs(superCluster.eta) < 2.3   ? 0.043 :
               ?2.3   <= abs(superCluster.eta) < 2.4   ? 0.047 :
                                                         0.066""", #neutralEA
            """?0.0   <= abs(superCluster.eta) < 1.0   ? 0.053 :
               ?1.0   <= abs(superCluster.eta) < 1.479 ? 0.052 :
               ?1.479 <= abs(superCluster.eta) < 2.0   ? 0.037 :
               ?2.0   <= abs(superCluster.eta) < 2.2   ? 0.073 :
               ?2.2   <= abs(superCluster.eta) < 2.3   ? 0.107 :
               ?2.3   <= abs(superCluster.eta) < 2.4   ? 0.123 :
                                                         0.133""", #gammaEA

            """?0.0   <= abs(superCluster.eta) < 1.0   ? 0.012 :
               ?1.0   <= abs(superCluster.eta) < 1.479 ? 0.010 :
               ?1.479 <= abs(superCluster.eta) < 2.0   ? 0.014 :
               ?2.0   <= abs(superCluster.eta) < 2.2   ? 0.012 :
               ?2.2   <= abs(superCluster.eta) < 2.3   ? 0.016 :
               ?2.3   <= abs(superCluster.eta) < 2.4   ? 0.020 :
                                                         0.012""", #chargedEA
            """?0.0   <= abs(superCluster.eta) < 1.0   ? 0.030 :
               ?1.0   <= abs(superCluster.eta) < 1.479 ? 0.057 :
               ?1.479 <= abs(superCluster.eta) < 2.0   ? 0.029 :
               ?2.0   <= abs(superCluster.eta) < 2.2   ? 0.015 :
               ?2.2   <= abs(superCluster.eta) < 2.3   ? 0.024 :
               ?2.3   <= abs(superCluster.eta) < 2.4   ? 0.039 :
                                                         0.072""", #neutralEA
            """?0.0   <= abs(superCluster.eta) < 1.0   ? 0.148 :
               ?1.0   <= abs(superCluster.eta) < 1.479 ? 0.130 :
               ?1.479 <= abs(superCluster.eta) < 2.0   ? 0.112 :
               ?2.0   <= abs(superCluster.eta) < 2.2   ? 0.216 :
               ?2.2   <= abs(superCluster.eta) < 2.3   ? 0.262 :
               ?2.3   <= abs(superCluster.eta) < 2.4   ? 0.260 :
                                                         0.266""", #gammaEA

            ####Cut values:singleTowerHOverE,showerShape,pfChargedRel,pfNeutralRel,pfGammaRel
            ###Tight
            """?0.0   <= abs(superCluster.eta) < 1.4442? 0.05 :0.05 """,#singleTowerHOverE
            """?0.0   <= abs(superCluster.eta) < 1.4442? 0.011:0.030""",#showerShape
            """?0.0   <= abs(superCluster.eta) < 1.4442? 0.02 :0.02 """,#pfChargedRel
            """?0.0   <= abs(superCluster.eta) < 1.4442? 0.05 :0.05 """,#pfNeutralRel
            """?0.0   <= abs(superCluster.eta) < 1.4442? 0.09 :0.09 """,#pfGammaRel
            """?0.0   <= abs(superCluster.eta) < 1.4442? 0.7 :0.5 """,  #pfCharged
            """?0.0   <= abs(superCluster.eta) < 1.4442? 0.4 + 0.04*pt :1.5 + 0.04*pt """,  #pfNeutral
            """?0.0   <= abs(superCluster.eta) < 1.4442? 0.5 + 0.005*pt :1.0 + 0.005*pt """,#pfGamma
            ###Medium
            """?0.0   <= abs(superCluster.eta) < 1.4442? 0.05 :0.05 """,#singleTowerHOverE
            """?0.0   <= abs(superCluster.eta) < 1.4442? 0.011:0.031""",#showerShape      
            """?0.0   <= abs(superCluster.eta) < 1.4442? 0.03 :0.03 """,#pfChargedRel     
            """?0.0   <= abs(superCluster.eta) < 1.4442? 0.07 :0.07 """,#pfNeutralRel     
            """?0.0   <= abs(superCluster.eta) < 1.4442? 0.08 :0.09 """,#pfGammaRel       
            """?0.0   <= abs(superCluster.eta) < 1.4442? 1.5 :1.2 """,  #pfCharged
            """?0.0   <= abs(superCluster.eta) < 1.4442? 1.0 + 0.04*pt :1.5 + 0.04*pt """,  #pfNeutral
            """?0.0   <= abs(superCluster.eta) < 1.4442? 0.7 + 0.005*pt :1.0 + 0.005*pt """,#pfGamma
            ###Loose
            """?0.0   <= abs(superCluster.eta) < 1.4442? 0.06 :0.05 """,#singleTowerHOverE
            """?0.0   <= abs(superCluster.eta) < 1.4442? 0.011:0.034""",#showerShape      
            """?0.0   <= abs(superCluster.eta) < 1.4442? 0.06 :0.05 """,#pfChargedRel     
            """?0.0   <= abs(superCluster.eta) < 1.4442? 0.16 :0.10 """,#pfNeutralRel     
            """?0.0   <= abs(superCluster.eta) < 1.4442? 0.08 :0.12 """,#pfGammaRel       
            """?0.0   <= abs(superCluster.eta) < 1.4442? 2.6 :2.3 """,  #pfCharged
            """?0.0   <= abs(superCluster.eta) < 1.4442? 3.5 + 0.04*pt :2.9 + 0.04*pt """,#pfNeutral
            """?0.0   <= abs(superCluster.eta) < 1.4442? 1.3 + 0.005*pt : 10000000 """   #pfGamma
        )
    )
)

addphotonuserdata1 = cms.EDProducer("AddPhotonUserData",
    debug          = cms.bool(False),
    debugString    = cms.string("addphotonuserdata"),
    photonLabel    = cms.InputTag("patPhotonsRA2"),
    floatLabels    = cms.VInputTag(),
    floatNames     = cms.vstring(),
    embedConversionInfo = cms.bool(False),
    gsfElectronLabel = cms.InputTag("gsfElectrons"),
    conversionsLabel = cms.InputTag("conversions"),
    beamspotLabel    = cms.InputTag("offlineBeamSpot"),
    useAlternateIsolations = cms.bool(False),
    vetoConeSize     = cms.double(0.4),
    candidateLabel   = cms.InputTag("particleFlow"),
    vertexLabel      = cms.InputTag("offlinePrimaryVertices"),
    userData = cms.PSet(
        userCands = cms.PSet(
            src = cms.VInputTag("")
        ),
        userInts = cms.PSet(
            src = cms.VInputTag("")
        ),
        userFloats = cms.PSet(
            src = cms.VInputTag("")
        ),
        userClasses = cms.PSet(
            src = cms.VInputTag("")
        ),
        userFunctionLabels = cms.vstring('pfChargedRel','pfNeutralRel','pfGammaRel',
                                         'combIsoR03','combIsoR04'),
        userFunctions = cms.vstring(
            'userFloat("pfChargedIsoAlt")/et',
            'userFloat("pfNeutralIsoAlt")/et',
            'userFloat("pfGammaIsoAlt")/et',
            'trkSumPtSolidConeDR03 + ecalRecHitSumEtConeDR03 + userFloat("hcalIsoConeDR03_2012")',
            'trkSumPtSolidConeDR04 + ecalRecHitSumEtConeDR04 + userFloat("hcalIsoConeDR04_2012")'
        )
    )
)
addphotonuserdata2 = cms.EDProducer("AddPhotonUserData",
    debug          = cms.bool(False),
    debugString    = cms.string("addphotonuserdata"),
    photonLabel    = cms.InputTag("patPhotonsUser1"),
    floatLabels    = cms.VInputTag(),
    floatNames     = cms.vstring(),
    embedConversionInfo = cms.bool(False),
    gsfElectronLabel = cms.InputTag("gsfElectrons"),
    conversionsLabel = cms.InputTag("conversions"),
    beamspotLabel    = cms.InputTag("offlineBeamSpot"),
    useAlternateIsolations = cms.bool(False),
    vetoConeSize     = cms.double(0.4),
    candidateLabel   = cms.InputTag("particleFlow"),
    vertexLabel      = cms.InputTag("offlinePrimaryVertices"),
    userData = cms.PSet(
        userCands = cms.PSet(
            src = cms.VInputTag("")
        ),
        userInts = cms.PSet(
            src = cms.VInputTag("")
        ),
        userFloats = cms.PSet(
            src = cms.VInputTag("")
        ),
        userClasses = cms.PSet(
            src = cms.VInputTag("")
        ),
        userFunctionLabels = cms.vstring('pfChargedPUSub','pfNeutralPUSub','pfGammaPUSub',
                                         'pfChargedPURel','pfNeutralPURel','pfGammaPURel',
                                         'pfChargedPU','pfNeutralPU','pfGammaPU',
                                         'combIsoR03PU','combIsoR04PU'),
        userFunctions = cms.vstring(
            'userFloat("pfChargedEANew")*userFloat("rho25")', #chargedSub
            'userFloat("pfNeutralEANew")*userFloat("rho25")', #neutralSub
            'userFloat("pfGammaEANew")  *userFloat("rho25")', #gammaSub
            'max((userFloat("pfChargedIsoAlt") - userFloat("pfChargedEANew")*userFloat("rho25"))/et,0.)',
            'max((userFloat("pfNeutralIsoAlt") - userFloat("pfNeutralEANew")*userFloat("rho25"))/et,0.)',
            'max((userFloat("pfGammaIsoAlt")   - userFloat("pfGammaEANew")  *userFloat("rho25"))/et,0.)',
            'max((userFloat("pfChargedIsoAlt") - userFloat("pfChargedEANew")*userFloat("rho25")),0.)',
            'max((userFloat("pfNeutralIsoAlt") - userFloat("pfNeutralEANew")*userFloat("rho25")),0.)',
            'max((userFloat("pfGammaIsoAlt")   - userFloat("pfGammaEANew")  *userFloat("rho25")),0.)',
            'trkSumPtSolidConeDR03 + ecalRecHitSumEtConeDR03 + userFloat("hcalIsoConeDR03_2012") - 3.141593*0.3*0.3*userFloat("rho25")',
            'trkSumPtSolidConeDR04 + ecalRecHitSumEtConeDR04 + userFloat("hcalIsoConeDR04_2012") - 3.141593*0.4*0.4*userFloat("rho25")'
        )
    )
)
