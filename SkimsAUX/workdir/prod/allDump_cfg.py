import FWCore.ParameterSet.Config as cms

process = cms.Process("SUSY")

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('/store/mc/Phys14DR/TTJets_MSDecaysCKM_central_Tune4C_13TeV-madgraph-tauola/MINIAODSIM/PU20bx25_PHYS14_25_V1-v1/00000/7CCE517B-0575-E411-877E-002590DB9214.root')
)
process.AK4PFchsForAK4PFCHSCandMETcorr = cms.EDProducer("PFCandMETcorrInputProducer",
    src = cms.InputTag("AK4PFchsForAK4PFCHSpfCandsNotInJetsForMetCorr")
)


process.AK4PFchsForAK4PFCHSJetMETcorr = cms.EDProducer("PFJetMETcorrInputProducer",
    src = cms.InputTag("ak4PFJetsCHS"),
    type1JetPtThreshold = cms.double(10.0),
    skipEMfractionThreshold = cms.double(0.9),
    skipEM = cms.bool(True),
    offsetCorrLabel = cms.string('AK4PFchsL1FastJet'),
    skipMuons = cms.bool(True),
    skipMuonSelection = cms.string('isGlobalMuon | isStandAloneMuon'),
    jetCorrEtaMax = cms.double(9.9),
    jetCorrLabel = cms.string('ak4PFL1FastL2L3')
)


process.AK4PFchsForAK4PFCHSNoMuCandMETcorr = cms.EDProducer("PFCandMETcorrInputProducer",
    src = cms.InputTag("AK4PFchsForAK4PFCHSNoMupfCandsNotInJetsForMetCorr")
)


process.AK4PFchsForAK4PFCHSNoMuJetMETcorr = cms.EDProducer("PFJetMETcorrInputProducer",
    src = cms.InputTag("ak4PFJetsCHSNoMu"),
    type1JetPtThreshold = cms.double(10.0),
    skipEMfractionThreshold = cms.double(0.9),
    skipEM = cms.bool(True),
    offsetCorrLabel = cms.string('AK4PFchsL1FastJet'),
    skipMuons = cms.bool(True),
    skipMuonSelection = cms.string('isGlobalMuon | isStandAloneMuon'),
    jetCorrEtaMax = cms.double(9.9),
    jetCorrLabel = cms.string('ak4PFL1FastL2L3')
)


process.AK4PFchsForAK4PFCHSNoMuType1CorMet = cms.EDProducer("AddCorrectionsToPFMET",
    src = cms.InputTag("pfMet"),
    srcCorrections = cms.VInputTag(cms.InputTag("AK4PFchsForAK4PFCHSNoMuJetMETcorr","type1"))
)


process.AK4PFchsForAK4PFCHSNoMuType1p2CorMet = cms.EDProducer("AddCorrectionsToPFMET",
    src = cms.InputTag("pfMet"),
    srcCorrections = cms.VInputTag(cms.InputTag("AK4PFchsForAK4PFCHSNoMuJetMETcorr","type1"), "AK4PFchsForAK4PFCHSNoMucorrPfMetType2")
)


process.AK4PFchsForAK4PFCHSNoMucorrPfMetType2 = cms.EDProducer("Type2CorrectionProducer",
    type2CorrFormula = cms.string('A'),
    type2CorrParameter = cms.PSet(
        A = cms.double(1.4)
    ),
    srcUnclEnergySums = cms.VInputTag(cms.InputTag("AK4PFchsForAK4PFCHSNoMuJetMETcorr","type2"), cms.InputTag("AK4PFchsForAK4PFCHSNoMuJetMETcorr","offset"), cms.InputTag("AK4PFchsForAK4PFCHSNoMuCandMETcorr"))
)


process.AK4PFchsForAK4PFCHSNoMupfCandsNotInJetsForMetCorr = cms.EDProducer("PFCandidateFromFwdPtrProducer",
    src = cms.InputTag("AK4PFchsForAK4PFCHSNoMupfCandsNotInJetsPtrForMetCorr")
)


process.AK4PFchsForAK4PFCHSNoMupfCandsNotInJetsPtrForMetCorr = cms.EDProducer("TPPFJetsOnPFCandidates",
    bottomCollection = cms.InputTag("particleFlowPtrs"),
    enable = cms.bool(True),
    topCollection = cms.InputTag("AK4PFchsForAK4PFCHSNoMupfJetsPtrForMetCorr"),
    name = cms.untracked.string('noJet'),
    verbose = cms.untracked.bool(False)
)


process.AK4PFchsForAK4PFCHSNoMupfJetsPtrForMetCorr = cms.EDProducer("PFJetFwdPtrProducer",
    src = cms.InputTag("ak4PFJetsCHSNoMu")
)


process.AK4PFchsForAK4PFCHSType1CorMet = cms.EDProducer("AddCorrectionsToPFMET",
    src = cms.InputTag("pfMet"),
    srcCorrections = cms.VInputTag(cms.InputTag("AK4PFchsForAK4PFCHSJetMETcorr","type1"))
)


process.AK4PFchsForAK4PFCHSType1p2CorMet = cms.EDProducer("AddCorrectionsToPFMET",
    src = cms.InputTag("pfMet"),
    srcCorrections = cms.VInputTag(cms.InputTag("AK4PFchsForAK4PFCHSJetMETcorr","type1"), "AK4PFchsForAK4PFCHScorrPfMetType2")
)


process.AK4PFchsForAK4PFCHScorrPfMetType2 = cms.EDProducer("Type2CorrectionProducer",
    type2CorrFormula = cms.string('A'),
    type2CorrParameter = cms.PSet(
        A = cms.double(1.4)
    ),
    srcUnclEnergySums = cms.VInputTag(cms.InputTag("AK4PFchsForAK4PFCHSJetMETcorr","type2"), cms.InputTag("AK4PFchsForAK4PFCHSJetMETcorr","offset"), cms.InputTag("AK4PFchsForAK4PFCHSCandMETcorr"))
)


process.AK4PFchsForAK4PFCHSpfCandsNotInJetsForMetCorr = cms.EDProducer("PFCandidateFromFwdPtrProducer",
    src = cms.InputTag("AK4PFchsForAK4PFCHSpfCandsNotInJetsPtrForMetCorr")
)


process.AK4PFchsForAK4PFCHSpfCandsNotInJetsPtrForMetCorr = cms.EDProducer("TPPFJetsOnPFCandidates",
    bottomCollection = cms.InputTag("particleFlowPtrs"),
    enable = cms.bool(True),
    topCollection = cms.InputTag("AK4PFchsForAK4PFCHSpfJetsPtrForMetCorr"),
    name = cms.untracked.string('noJet'),
    verbose = cms.untracked.bool(False)
)


process.AK4PFchsForAK4PFCHSpfJetsPtrForMetCorr = cms.EDProducer("PFJetFwdPtrProducer",
    src = cms.InputTag("ak4PFJetsCHS")
)


process.ak4GenJets = cms.EDProducer("FastjetJetProducer",
    Active_Area_Repeats = cms.int32(5),
    doAreaFastjet = cms.bool(False),
    Ghost_EtaMax = cms.double(6.0),
    maxBadHcalCells = cms.uint32(9999999),
    maxRecoveredEcalCells = cms.uint32(9999999),
    jetType = cms.string('GenJet'),
    minSeed = cms.uint32(14327),
    doRhoFastjet = cms.bool(False),
    jetAlgorithm = cms.string('AntiKt'),
    nSigmaPU = cms.double(1.0),
    GhostArea = cms.double(0.01),
    Rho_EtaMax = cms.double(4.5),
    maxBadEcalCells = cms.uint32(9999999),
    useDeterministicSeed = cms.bool(True),
    doPVCorrection = cms.bool(False),
    maxRecoveredHcalCells = cms.uint32(9999999),
    rParam = cms.double(0.4),
    maxProblematicHcalCells = cms.uint32(9999999),
    src = cms.InputTag("packedGenParticles"),
    inputEtMin = cms.double(0.0),
    srcPVs = cms.InputTag(""),
    jetPtMin = cms.double(3.0),
    radiusPU = cms.double(0.5),
    maxProblematicEcalCells = cms.uint32(9999999),
    doPUOffsetCorr = cms.bool(False),
    inputEMin = cms.double(0.0)
)


process.ak4PFJetsCHS = cms.EDProducer("FastjetJetProducer",
    Active_Area_Repeats = cms.int32(1),
    doAreaFastjet = cms.bool(True),
    voronoiRfact = cms.double(-0.9),
    maxBadHcalCells = cms.uint32(9999999),
    doAreaDiskApprox = cms.bool(False),
    maxRecoveredEcalCells = cms.uint32(9999999),
    jetType = cms.string('PFJet'),
    minSeed = cms.uint32(14327),
    Ghost_EtaMax = cms.double(5.0),
    doRhoFastjet = cms.bool(False),
    jetAlgorithm = cms.string('AntiKt'),
    nSigmaPU = cms.double(1.0),
    GhostArea = cms.double(0.01),
    Rho_EtaMax = cms.double(4.4),
    maxBadEcalCells = cms.uint32(9999999),
    useDeterministicSeed = cms.bool(True),
    doPVCorrection = cms.bool(False),
    maxRecoveredHcalCells = cms.uint32(9999999),
    rParam = cms.double(0.4),
    maxProblematicHcalCells = cms.uint32(9999999),
    doOutputJets = cms.bool(True),
    src = cms.InputTag("pfCHS"),
    inputEtMin = cms.double(0.0),
    srcPVs = cms.InputTag(""),
    jetPtMin = cms.double(3.0),
    radiusPU = cms.double(0.5),
    maxProblematicEcalCells = cms.uint32(9999999),
    doPUOffsetCorr = cms.bool(False),
    inputEMin = cms.double(0.0)
)


process.ak4PFJetsCHSNoMu = cms.EDProducer("FastjetJetProducer",
    Active_Area_Repeats = cms.int32(1),
    doAreaFastjet = cms.bool(True),
    voronoiRfact = cms.double(-0.9),
    maxBadHcalCells = cms.uint32(9999999),
    doAreaDiskApprox = cms.bool(False),
    maxRecoveredEcalCells = cms.uint32(9999999),
    jetType = cms.string('PFJet'),
    minSeed = cms.uint32(14327),
    Ghost_EtaMax = cms.double(5.0),
    doRhoFastjet = cms.bool(False),
    jetAlgorithm = cms.string('AntiKt'),
    nSigmaPU = cms.double(1.0),
    GhostArea = cms.double(0.01),
    Rho_EtaMax = cms.double(4.4),
    maxBadEcalCells = cms.uint32(9999999),
    useDeterministicSeed = cms.bool(True),
    doPVCorrection = cms.bool(False),
    maxRecoveredHcalCells = cms.uint32(9999999),
    rParam = cms.double(0.4),
    maxProblematicHcalCells = cms.uint32(9999999),
    doOutputJets = cms.bool(True),
    src = cms.InputTag("pfNoMuonCHSNoMu"),
    inputEtMin = cms.double(0.0),
    srcPVs = cms.InputTag(""),
    jetPtMin = cms.double(3.0),
    radiusPU = cms.double(0.5),
    maxProblematicEcalCells = cms.uint32(9999999),
    doPUOffsetCorr = cms.bool(False),
    inputEMin = cms.double(0.0)
)


process.ak4PFJetschsL1FastL2L3 = cms.EDProducer("PFJetCorrectionProducer",
    src = cms.InputTag("slimmedJets"),
    correctors = cms.vstring('ak4PFL1FastL2L3'),
    algorithm = cms.string('AK4PFchs')
)


process.ak4PFJetschsL1FastL2L3Residual = cms.EDProducer("PFJetCorrectionProducer",
    src = cms.InputTag("slimmedJets"),
    correctors = cms.vstring('ak4PFL1FastL2L3Residual'),
    algorithm = cms.string('AK4PFchs')
)


process.ak4stophtPFchs = cms.EDProducer("HTProducer",
    JetCollection = cms.InputTag("ak4stopJetsPFchsPt50Eta24"),
    MaxJetEta = cms.double(2.5),
    MinJetPt = cms.double(50)
)


process.ak4stopmhtPFchs = cms.EDProducer("MHTProducer",
    JetCollection = cms.InputTag("ak4stopJetsPFchsPt30"),
    MaxJetEta = cms.double(5),
    MinJetPt = cms.double(30)
)


process.bToCharmDecayVertexMerged = cms.EDProducer("BtoCharmDecayVertexMerger",
    minvecSumIMifsmallDRUnique = cms.untracked.double(5.5),
    minDRUnique = cms.untracked.double(0.4),
    minCosPAtomerge = cms.untracked.double(0.99),
    primaryVertices = cms.InputTag("offlinePrimaryVertices"),
    maxPtreltomerge = cms.untracked.double(7777.0),
    secondaryVertices = cms.InputTag("inclusiveSecondaryVerticesFiltered")
)


process.caloMetT1 = cms.EDProducer("AddCorrectionsToCaloMET",
    src = cms.InputTag("caloMetM"),
    srcCorrections = cms.VInputTag(cms.InputTag("corrCaloMetType1","type1"))
)


process.caloMetT1T2 = cms.EDProducer("AddCorrectionsToCaloMET",
    src = cms.InputTag("caloMetM"),
    srcCorrections = cms.VInputTag(cms.InputTag("corrCaloMetType1","type1"), cms.InputTag("corrCaloMetType2"))
)


process.combinedInclusiveSecondaryVertexBJetTags = cms.EDProducer("JetTagProducer",
    jetTagComputer = cms.string('combinedSecondaryVertex'),
    tagInfos = cms.VInputTag(cms.InputTag("impactParameterTagInfos"), cms.InputTag("inclusiveSecondaryVertexFinderTagInfos"))
)


process.combinedInclusiveSecondaryVertexNegativeBJetTags = cms.EDProducer("JetTagProducer",
    jetTagComputer = cms.string('combinedSecondaryVertexNegative'),
    tagInfos = cms.VInputTag(cms.InputTag("impactParameterTagInfos"), cms.InputTag("inclusiveSecondaryVertexFinderNegativeTagInfos"))
)


process.combinedInclusiveSecondaryVertexPositiveBJetTags = cms.EDProducer("JetTagProducer",
    jetTagComputer = cms.string('combinedSecondaryVertexPositive'),
    tagInfos = cms.VInputTag(cms.InputTag("impactParameterTagInfos"), cms.InputTag("inclusiveSecondaryVertexFinderTagInfos"))
)


process.combinedInclusiveSecondaryVertexV2BJetTags = cms.EDProducer("JetTagProducer",
    jetTagComputer = cms.string('combinedSecondaryVertexV2'),
    tagInfos = cms.VInputTag(cms.InputTag("impactParameterTagInfos"), cms.InputTag("inclusiveSecondaryVertexFinderTagInfos"))
)


process.combinedInclusiveSecondaryVertexV2BJetTagsAK4PFCHS = cms.EDProducer("JetTagProducer",
    jetTagComputer = cms.string('combinedSecondaryVertexV2'),
    tagInfos = cms.VInputTag(cms.InputTag("impactParameterTagInfosAK4PFCHS"), cms.InputTag("inclusiveSecondaryVertexFinderTagInfosAK4PFCHS"))
)


process.combinedInclusiveSecondaryVertexV2BJetTagsAK4PFCHSNoMu = cms.EDProducer("JetTagProducer",
    jetTagComputer = cms.string('combinedSecondaryVertexV2'),
    tagInfos = cms.VInputTag(cms.InputTag("impactParameterTagInfosAK4PFCHSNoMu"), cms.InputTag("inclusiveSecondaryVertexFinderTagInfosAK4PFCHSNoMu"))
)


process.combinedInclusiveSecondaryVertexV2NegativeBJetTags = cms.EDProducer("JetTagProducer",
    jetTagComputer = cms.string('combinedSecondaryVertexV2Negative'),
    tagInfos = cms.VInputTag(cms.InputTag("impactParameterTagInfos"), cms.InputTag("inclusiveSecondaryVertexFinderNegativeTagInfos"))
)


process.combinedInclusiveSecondaryVertexV2PositiveBJetTags = cms.EDProducer("JetTagProducer",
    jetTagComputer = cms.string('combinedSecondaryVertexV2Positive'),
    tagInfos = cms.VInputTag(cms.InputTag("impactParameterTagInfos"), cms.InputTag("inclusiveSecondaryVertexFinderTagInfos"))
)


process.combinedMVABJetTags = cms.EDProducer("JetTagProducer",
    jetTagComputer = cms.string('combinedMVA'),
    tagInfos = cms.VInputTag(cms.InputTag("impactParameterTagInfos"), cms.InputTag("secondaryVertexTagInfos"), cms.InputTag("softPFMuonsTagInfos"), cms.InputTag("softPFElectronsTagInfos"))
)


process.combinedSecondaryVertexBJetTags = cms.EDProducer("JetTagProducer",
    jetTagComputer = cms.string('combinedSecondaryVertex'),
    tagInfos = cms.VInputTag(cms.InputTag("impactParameterTagInfos"), cms.InputTag("secondaryVertexTagInfos"))
)


process.combinedSecondaryVertexMVABJetTags = cms.EDProducer("JetTagProducer",
    jetTagComputer = cms.string('combinedSecondaryVertexMVA'),
    tagInfos = cms.VInputTag(cms.InputTag("impactParameterTagInfos"), cms.InputTag("secondaryVertexTagInfos"))
)


process.combinedSecondaryVertexNegativeBJetTags = cms.EDProducer("JetTagProducer",
    jetTagComputer = cms.string('combinedSecondaryVertexNegative'),
    tagInfos = cms.VInputTag(cms.InputTag("impactParameterTagInfos"), cms.InputTag("secondaryVertexNegativeTagInfos"))
)


process.combinedSecondaryVertexPositiveBJetTags = cms.EDProducer("JetTagProducer",
    jetTagComputer = cms.string('combinedSecondaryVertexPositive'),
    tagInfos = cms.VInputTag(cms.InputTag("impactParameterTagInfos"), cms.InputTag("secondaryVertexTagInfos"))
)


process.combinedSecondaryVertexSoftLeptonBJetTags = cms.EDProducer("JetTagProducer",
    jetTagComputer = cms.string('combinedSecondaryVertexSoftLepton'),
    tagInfos = cms.VInputTag(cms.InputTag("impactParameterTagInfos"), cms.InputTag("secondaryVertexTagInfos"), cms.InputTag("softPFMuonsTagInfos"), cms.InputTag("softPFElectronsTagInfos"))
)


process.corrCaloMetType1 = cms.EDProducer("CaloJetMETcorrInputProducer",
    src = cms.InputTag("ak4CaloJets"),
    type1JetPtThreshold = cms.double(20.0),
    skipEMfractionThreshold = cms.double(0.9),
    skipEM = cms.bool(True),
    jetCorrEtaMax = cms.double(9.9),
    srcMET = cms.InputTag("caloMetM"),
    jetCorrLabel = cms.string('ak4CaloL2L3')
)


process.corrCaloMetType2 = cms.EDProducer("Type2CorrectionProducer",
    type2CorrFormula = cms.string('A + B*TMath::Exp(-C*x)'),
    type2CorrParameter = cms.PSet(
        A = cms.double(2.0),
        C = cms.double(0.1),
        B = cms.double(1.3)
    ),
    srcUnclEnergySums = cms.VInputTag(cms.InputTag("corrCaloMetType1","type2"), cms.InputTag("muCaloMetCorr"))
)


process.corrPfMetType1 = cms.EDProducer("PFJetMETcorrInputProducer",
    src = cms.InputTag("ak4PFJets"),
    type1JetPtThreshold = cms.double(10.0),
    skipEMfractionThreshold = cms.double(0.9),
    skipEM = cms.bool(True),
    offsetCorrLabel = cms.string('ak4PFL1Fastjet'),
    skipMuons = cms.bool(True),
    skipMuonSelection = cms.string('isGlobalMuon | isStandAloneMuon'),
    jetCorrEtaMax = cms.double(9.9),
    jetCorrLabel = cms.string('ak4PFL1FastL2L3')
)


process.corrPfMetType2 = cms.EDProducer("Type2CorrectionProducer",
    type2CorrFormula = cms.string('A'),
    type2CorrParameter = cms.PSet(
        A = cms.double(1.4)
    ),
    srcUnclEnergySums = cms.VInputTag(cms.InputTag("corrPfMetType1","type2"), cms.InputTag("corrPfMetType1","offset"), cms.InputTag("pfCandMETcorr"))
)


process.doubleSecondaryVertexHighEffBJetTags = cms.EDProducer("JetTagProducer",
    jetTagComputer = cms.string('doubleVertex2Trk'),
    tagInfos = cms.VInputTag(cms.InputTag("inclusiveSecondaryVertexFinderFilteredTagInfos"))
)


process.dummyCounter = cms.EDProducer("EventCountProducer")


process.elPFIsoDepositCharged = cms.EDProducer("CandIsoDepositProducer",
    src = cms.InputTag("gedGsfElectrons"),
    MultipleDepositsFlag = cms.bool(False),
    trackType = cms.string('candidate'),
    ExtractorPSet = cms.PSet(
        Diff_z = cms.double(99999.99),
        ComponentName = cms.string('CandViewExtractor'),
        DR_Max = cms.double(1.0),
        Diff_r = cms.double(99999.99),
        inputCandView = cms.InputTag("pfAllChargedHadrons"),
        DR_Veto = cms.double(0),
        DepositLabel = cms.untracked.string('')
    )
)


process.elPFIsoDepositChargedAll = cms.EDProducer("CandIsoDepositProducer",
    src = cms.InputTag("gedGsfElectrons"),
    MultipleDepositsFlag = cms.bool(False),
    trackType = cms.string('candidate'),
    ExtractorPSet = cms.PSet(
        Diff_z = cms.double(99999.99),
        ComponentName = cms.string('CandViewExtractor'),
        DR_Max = cms.double(1.0),
        Diff_r = cms.double(99999.99),
        inputCandView = cms.InputTag("pfAllChargedParticles"),
        DR_Veto = cms.double(0),
        DepositLabel = cms.untracked.string('')
    )
)


process.elPFIsoDepositGamma = cms.EDProducer("CandIsoDepositProducer",
    src = cms.InputTag("gedGsfElectrons"),
    MultipleDepositsFlag = cms.bool(False),
    trackType = cms.string('candidate'),
    ExtractorPSet = cms.PSet(
        Diff_z = cms.double(99999.99),
        SCMatch_Veto = cms.bool(False),
        ComponentName = cms.string('PFCandWithSuperClusterExtractor'),
        DR_Max = cms.double(1.0),
        Diff_r = cms.double(99999.99),
        inputCandView = cms.InputTag("pfAllPhotons"),
        MissHitSCMatch_Veto = cms.bool(True),
        DR_Veto = cms.double(0),
        DepositLabel = cms.untracked.string('')
    )
)


process.elPFIsoDepositNeutral = cms.EDProducer("CandIsoDepositProducer",
    src = cms.InputTag("gedGsfElectrons"),
    MultipleDepositsFlag = cms.bool(False),
    trackType = cms.string('candidate'),
    ExtractorPSet = cms.PSet(
        Diff_z = cms.double(99999.99),
        ComponentName = cms.string('CandViewExtractor'),
        DR_Max = cms.double(1.0),
        Diff_r = cms.double(99999.99),
        inputCandView = cms.InputTag("pfAllNeutralHadrons"),
        DR_Veto = cms.double(0),
        DepositLabel = cms.untracked.string('')
    )
)


process.elPFIsoDepositPU = cms.EDProducer("CandIsoDepositProducer",
    src = cms.InputTag("gedGsfElectrons"),
    MultipleDepositsFlag = cms.bool(False),
    trackType = cms.string('candidate'),
    ExtractorPSet = cms.PSet(
        Diff_z = cms.double(99999.99),
        ComponentName = cms.string('CandViewExtractor'),
        DR_Max = cms.double(1.0),
        Diff_r = cms.double(99999.99),
        inputCandView = cms.InputTag("pfPileUpAllChargedParticles"),
        DR_Veto = cms.double(0),
        DepositLabel = cms.untracked.string('')
    )
)


process.elPFIsoValueCharged03NoPFId = cms.EDProducer("PFCandIsolatorFromDeposits",
    deposits = cms.VPSet(cms.PSet(
        src = cms.InputTag("elPFIsoDepositCharged"),
        deltaR = cms.double(0.3),
        PivotCoordinatesForEBEE = cms.bool(True),
        weight = cms.string('1'),
        vetos = cms.vstring('EcalEndcaps:ConeVeto(0.015)'),
        skipDefaultVeto = cms.bool(True),
        mode = cms.string('sum')
    ))
)


process.elPFIsoValueCharged03PFId = cms.EDProducer("PFCandIsolatorFromDeposits",
    deposits = cms.VPSet(cms.PSet(
        src = cms.InputTag("elPFIsoDepositCharged"),
        deltaR = cms.double(0.3),
        PivotCoordinatesForEBEE = cms.bool(True),
        weight = cms.string('1'),
        vetos = cms.vstring('EcalEndcaps:ConeVeto(0.015)'),
        skipDefaultVeto = cms.bool(True),
        mode = cms.string('sum')
    ))
)


process.elPFIsoValueCharged04NoPFId = cms.EDProducer("PFCandIsolatorFromDeposits",
    deposits = cms.VPSet(cms.PSet(
        src = cms.InputTag("elPFIsoDepositCharged"),
        deltaR = cms.double(0.4),
        PivotCoordinatesForEBEE = cms.bool(True),
        weight = cms.string('1'),
        vetos = cms.vstring('EcalEndcaps:ConeVeto(0.015)'),
        skipDefaultVeto = cms.bool(True),
        mode = cms.string('sum')
    ))
)


process.elPFIsoValueCharged04PFId = cms.EDProducer("PFCandIsolatorFromDeposits",
    deposits = cms.VPSet(cms.PSet(
        src = cms.InputTag("elPFIsoDepositCharged"),
        deltaR = cms.double(0.4),
        PivotCoordinatesForEBEE = cms.bool(True),
        weight = cms.string('1'),
        vetos = cms.vstring('EcalEndcaps:ConeVeto(0.015)'),
        skipDefaultVeto = cms.bool(True),
        mode = cms.string('sum')
    ))
)


process.elPFIsoValueChargedAll03NoPFId = cms.EDProducer("PFCandIsolatorFromDeposits",
    deposits = cms.VPSet(cms.PSet(
        src = cms.InputTag("elPFIsoDepositChargedAll"),
        deltaR = cms.double(0.3),
        PivotCoordinatesForEBEE = cms.bool(True),
        weight = cms.string('1'),
        vetos = cms.vstring('EcalEndcaps:ConeVeto(0.015)'),
        skipDefaultVeto = cms.bool(True),
        mode = cms.string('sum')
    ))
)


process.elPFIsoValueChargedAll03PFId = cms.EDProducer("PFCandIsolatorFromDeposits",
    deposits = cms.VPSet(cms.PSet(
        src = cms.InputTag("elPFIsoDepositChargedAll"),
        deltaR = cms.double(0.3),
        PivotCoordinatesForEBEE = cms.bool(True),
        weight = cms.string('1'),
        vetos = cms.vstring('EcalEndcaps:ConeVeto(0.015)'),
        skipDefaultVeto = cms.bool(True),
        mode = cms.string('sum')
    ))
)


process.elPFIsoValueChargedAll04NoPFId = cms.EDProducer("PFCandIsolatorFromDeposits",
    deposits = cms.VPSet(cms.PSet(
        src = cms.InputTag("elPFIsoDepositChargedAll"),
        deltaR = cms.double(0.4),
        PivotCoordinatesForEBEE = cms.bool(True),
        weight = cms.string('1'),
        vetos = cms.vstring('EcalEndcaps:ConeVeto(0.015)'),
        skipDefaultVeto = cms.bool(True),
        mode = cms.string('sum')
    ))
)


process.elPFIsoValueChargedAll04PFId = cms.EDProducer("PFCandIsolatorFromDeposits",
    deposits = cms.VPSet(cms.PSet(
        src = cms.InputTag("elPFIsoDepositChargedAll"),
        deltaR = cms.double(0.4),
        PivotCoordinatesForEBEE = cms.bool(True),
        weight = cms.string('1'),
        vetos = cms.vstring('EcalEndcaps:ConeVeto(0.015)'),
        skipDefaultVeto = cms.bool(True),
        mode = cms.string('sum')
    ))
)


process.elPFIsoValueGamma03NoPFId = cms.EDProducer("PFCandIsolatorFromDeposits",
    deposits = cms.VPSet(cms.PSet(
        src = cms.InputTag("elPFIsoDepositGamma"),
        deltaR = cms.double(0.3),
        PivotCoordinatesForEBEE = cms.bool(True),
        weight = cms.string('1'),
        vetos = cms.vstring('EcalEndcaps:ConeVeto(0.08)'),
        skipDefaultVeto = cms.bool(True),
        mode = cms.string('sum')
    ))
)


process.elPFIsoValueGamma03PFId = cms.EDProducer("PFCandIsolatorFromDeposits",
    deposits = cms.VPSet(cms.PSet(
        src = cms.InputTag("elPFIsoDepositGamma"),
        deltaR = cms.double(0.3),
        PivotCoordinatesForEBEE = cms.bool(True),
        weight = cms.string('1'),
        vetos = cms.vstring('EcalEndcaps:ConeVeto(0.08)'),
        skipDefaultVeto = cms.bool(True),
        mode = cms.string('sum')
    ))
)


process.elPFIsoValueGamma04NoPFId = cms.EDProducer("PFCandIsolatorFromDeposits",
    deposits = cms.VPSet(cms.PSet(
        src = cms.InputTag("elPFIsoDepositGamma"),
        deltaR = cms.double(0.4),
        PivotCoordinatesForEBEE = cms.bool(True),
        weight = cms.string('1'),
        vetos = cms.vstring('EcalEndcaps:ConeVeto(0.08)'),
        skipDefaultVeto = cms.bool(True),
        mode = cms.string('sum')
    ))
)


process.elPFIsoValueGamma04PFId = cms.EDProducer("PFCandIsolatorFromDeposits",
    deposits = cms.VPSet(cms.PSet(
        src = cms.InputTag("elPFIsoDepositGamma"),
        deltaR = cms.double(0.4),
        PivotCoordinatesForEBEE = cms.bool(True),
        weight = cms.string('1'),
        vetos = cms.vstring('EcalEndcaps:ConeVeto(0.08)'),
        skipDefaultVeto = cms.bool(True),
        mode = cms.string('sum')
    ))
)


process.elPFIsoValueNeutral03NoPFId = cms.EDProducer("PFCandIsolatorFromDeposits",
    deposits = cms.VPSet(cms.PSet(
        src = cms.InputTag("elPFIsoDepositNeutral"),
        deltaR = cms.double(0.3),
        PivotCoordinatesForEBEE = cms.bool(True),
        weight = cms.string('1'),
        vetos = cms.vstring(),
        skipDefaultVeto = cms.bool(True),
        mode = cms.string('sum')
    ))
)


process.elPFIsoValueNeutral03PFId = cms.EDProducer("PFCandIsolatorFromDeposits",
    deposits = cms.VPSet(cms.PSet(
        src = cms.InputTag("elPFIsoDepositNeutral"),
        deltaR = cms.double(0.3),
        PivotCoordinatesForEBEE = cms.bool(True),
        weight = cms.string('1'),
        vetos = cms.vstring(),
        skipDefaultVeto = cms.bool(True),
        mode = cms.string('sum')
    ))
)


process.elPFIsoValueNeutral04NoPFId = cms.EDProducer("PFCandIsolatorFromDeposits",
    deposits = cms.VPSet(cms.PSet(
        src = cms.InputTag("elPFIsoDepositNeutral"),
        deltaR = cms.double(0.4),
        PivotCoordinatesForEBEE = cms.bool(True),
        weight = cms.string('1'),
        vetos = cms.vstring(),
        skipDefaultVeto = cms.bool(True),
        mode = cms.string('sum')
    ))
)


process.elPFIsoValueNeutral04PFId = cms.EDProducer("PFCandIsolatorFromDeposits",
    deposits = cms.VPSet(cms.PSet(
        src = cms.InputTag("elPFIsoDepositNeutral"),
        deltaR = cms.double(0.4),
        PivotCoordinatesForEBEE = cms.bool(True),
        weight = cms.string('1'),
        vetos = cms.vstring(),
        skipDefaultVeto = cms.bool(True),
        mode = cms.string('sum')
    ))
)


process.elPFIsoValuePU03NoPFId = cms.EDProducer("PFCandIsolatorFromDeposits",
    deposits = cms.VPSet(cms.PSet(
        src = cms.InputTag("elPFIsoDepositPU"),
        deltaR = cms.double(0.3),
        PivotCoordinatesForEBEE = cms.bool(True),
        weight = cms.string('1'),
        vetos = cms.vstring('EcalEndcaps:ConeVeto(0.015)'),
        skipDefaultVeto = cms.bool(True),
        mode = cms.string('sum')
    ))
)


process.elPFIsoValuePU03PFId = cms.EDProducer("PFCandIsolatorFromDeposits",
    deposits = cms.VPSet(cms.PSet(
        src = cms.InputTag("elPFIsoDepositPU"),
        deltaR = cms.double(0.3),
        PivotCoordinatesForEBEE = cms.bool(True),
        weight = cms.string('1'),
        vetos = cms.vstring('EcalEndcaps:ConeVeto(0.015)'),
        skipDefaultVeto = cms.bool(True),
        mode = cms.string('sum')
    ))
)


process.elPFIsoValuePU04NoPFId = cms.EDProducer("PFCandIsolatorFromDeposits",
    deposits = cms.VPSet(cms.PSet(
        src = cms.InputTag("elPFIsoDepositPU"),
        deltaR = cms.double(0.4),
        PivotCoordinatesForEBEE = cms.bool(True),
        weight = cms.string('1'),
        vetos = cms.vstring('EcalEndcaps:ConeVeto(0.015)'),
        skipDefaultVeto = cms.bool(True),
        mode = cms.string('sum')
    ))
)


process.elPFIsoValuePU04PFId = cms.EDProducer("PFCandIsolatorFromDeposits",
    deposits = cms.VPSet(cms.PSet(
        src = cms.InputTag("elPFIsoDepositPU"),
        deltaR = cms.double(0.4),
        PivotCoordinatesForEBEE = cms.bool(True),
        weight = cms.string('1'),
        vetos = cms.vstring('EcalEndcaps:ConeVeto(0.015)'),
        skipDefaultVeto = cms.bool(True),
        mode = cms.string('sum')
    ))
)


process.electronMatch = cms.EDProducer("MCMatcher",
    src = cms.InputTag("gedGsfElectrons"),
    maxDPtRel = cms.double(0.5),
    mcPdgId = cms.vint32(11),
    mcStatus = cms.vint32(1),
    resolveByMatchQuality = cms.bool(False),
    maxDeltaR = cms.double(0.5),
    checkCharge = cms.bool(True),
    resolveAmbiguities = cms.bool(True),
    matched = cms.InputTag("genParticles")
)


process.genParticlesForJets = cms.EDProducer("InputGenJetsParticleSelector",
    src = cms.InputTag("prunedGenParticles"),
    ignoreParticleIDs = cms.vuint32(1000022, 1000012, 1000014, 1000016, 2000012, 
        2000014, 2000016, 1000039, 5100039, 4000012, 
        4000014, 4000016, 9900012, 9900014, 9900016, 
        39),
    partonicFinalState = cms.bool(False),
    excludeResonances = cms.bool(False),
    excludeFromResonancePids = cms.vuint32(12, 13, 14, 16),
    tausAsJets = cms.bool(False)
)


process.genParticlesForJetsNoMuNoNu = cms.EDProducer("InputGenJetsParticleSelector",
    src = cms.InputTag("genParticles"),
    ignoreParticleIDs = cms.vuint32(1000022, 1000012, 1000014, 1000016, 2000012, 
        2000014, 2000016, 1000039, 5100039, 4000012, 
        4000014, 4000016, 9900012, 9900014, 9900016, 
        39, 12, 13, 14, 16),
    partonicFinalState = cms.bool(False),
    excludeResonances = cms.bool(False),
    excludeFromResonancePids = cms.vuint32(12, 13, 14, 16),
    tausAsJets = cms.bool(False)
)


process.genParticlesForJetsNoNu = cms.EDProducer("InputGenJetsParticleSelector",
    src = cms.InputTag("genParticles"),
    ignoreParticleIDs = cms.vuint32(1000022, 1000012, 1000014, 1000016, 2000012, 
        2000014, 2000016, 1000039, 5100039, 4000012, 
        4000014, 4000016, 9900012, 9900014, 9900016, 
        39, 12, 14, 16),
    partonicFinalState = cms.bool(False),
    excludeResonances = cms.bool(False),
    excludeFromResonancePids = cms.vuint32(12, 13, 14, 16),
    tausAsJets = cms.bool(False)
)


process.ghostTrackBJetTags = cms.EDProducer("JetTagProducer",
    jetTagComputer = cms.string('ghostTrack'),
    tagInfos = cms.VInputTag(cms.InputTag("impactParameterTagInfos"), cms.InputTag("ghostTrackVertexTagInfos"))
)


process.ghostTrackVertexTagInfos = cms.EDProducer("SecondaryVertexProducer",
    extSVDeltaRToJet = cms.double(0.3),
    beamSpotTag = cms.InputTag("offlineBeamSpot"),
    vertexReco = cms.PSet(
        primcut = cms.double(2.0),
        seccut = cms.double(4.0),
        maxFitChi2 = cms.double(10.0),
        fitType = cms.string('RefitGhostTrackWithVertices'),
        mergeThreshold = cms.double(3.0),
        finder = cms.string('gtvr')
    ),
    vertexSelection = cms.PSet(
        sortCriterium = cms.string('dist3dError')
    ),
    constraint = cms.string('BeamSpot'),
    trackIPTagInfos = cms.InputTag("impactParameterTagInfos"),
    vertexCuts = cms.PSet(
        distSig3dMax = cms.double(99999.9),
        fracPV = cms.double(0.65),
        distVal2dMax = cms.double(2.5),
        useTrackWeights = cms.bool(True),
        maxDeltaRToJetAxis = cms.double(0.5),
        v0Filter = cms.PSet(
            k0sMassWindow = cms.double(0.05)
        ),
        distSig2dMin = cms.double(3.0),
        multiplicityMin = cms.uint32(1),
        massMax = cms.double(6.5),
        distSig2dMax = cms.double(99999.9),
        distVal3dMax = cms.double(99999.9),
        minimumTrackWeight = cms.double(0.5),
        distVal3dMin = cms.double(-99999.9),
        distVal2dMin = cms.double(0.01),
        distSig3dMin = cms.double(-99999.9)
    ),
    useExternalSV = cms.bool(False),
    minimumTrackWeight = cms.double(0.5),
    usePVError = cms.bool(True),
    trackSelection = cms.PSet(
        b_pT = cms.double(0.3684),
        a_dR = cms.double(-0.001053),
        min_pT = cms.double(120),
        max_pT = cms.double(500),
        min_pT_dRcut = cms.double(0.5),
        max_pT_trackPTcut = cms.double(3),
        max_pT_dRcut = cms.double(0.1),
        b_dR = cms.double(0.6263),
        a_pT = cms.double(0.005263),
        totalHitsMin = cms.uint32(8),
        jetDeltaRMax = cms.double(0.3),
        qualityClass = cms.string('any'),
        pixelHitsMin = cms.uint32(2),
        sip3dSigMin = cms.double(-99999.9),
        sip3dSigMax = cms.double(99999.9),
        maxDistToAxis = cms.double(0.2),
        useVariableJTA = cms.bool(False),
        sip2dValMax = cms.double(99999.9),
        maxDecayLen = cms.double(99999.9),
        ptMin = cms.double(1.0),
        sip2dSigMax = cms.double(99999.9),
        sip2dSigMin = cms.double(-99999.9),
        sip3dValMax = cms.double(99999.9),
        sip3dValMin = cms.double(-99999.9),
        sip2dValMin = cms.double(-99999.9),
        normChi2Max = cms.double(99999.9)
    ),
    trackSort = cms.string('sip3dSig'),
    extSVCollection = cms.InputTag("secondaryVertices")
)


process.hiGenParticlesForJets = cms.EDProducer("InputGenJetsParticleSelector",
    src = cms.InputTag("hiGenParticles"),
    ignoreParticleIDs = cms.vuint32(1000022, 1000012, 1000014, 1000016, 2000012, 
        2000014, 2000016, 1000039, 5100039, 4000012, 
        4000014, 4000016, 9900012, 9900014, 9900016, 
        39),
    partonicFinalState = cms.bool(False),
    excludeResonances = cms.bool(False),
    excludeFromResonancePids = cms.vuint32(12, 13, 14, 16),
    tausAsJets = cms.bool(False)
)


process.ht = cms.EDProducer("HTProducer",
    JetCollection = cms.InputTag("patJets"),
    MaxJetEta = cms.double(2.5),
    MinJetPt = cms.double(50)
)


process.htPFchs = cms.EDProducer("HTProducer",
    JetCollection = cms.InputTag("ak4patJetsPFchsPt50Eta25"),
    MaxJetEta = cms.double(2.5),
    MinJetPt = cms.double(50)
)


process.impactParameterMVABJetTags = cms.EDProducer("JetTagProducer",
    jetTagComputer = cms.string('impactParameterMVAComputer'),
    tagInfos = cms.VInputTag(cms.InputTag("impactParameterTagInfos"))
)


process.impactParameterTagInfos = cms.EDProducer("TrackIPProducer",
    maximumTransverseImpactParameter = cms.double(0.2),
    minimumNumberOfHits = cms.int32(8),
    minimumTransverseMomentum = cms.double(1.0),
    primaryVertex = cms.InputTag("offlinePrimaryVertices"),
    maximumLongitudinalImpactParameter = cms.double(17.0),
    computeProbabilities = cms.bool(True),
    ghostTrackPriorDeltaR = cms.double(0.03),
    jetTracks = cms.InputTag("ak4JetTracksAssociatorAtVertexPF"),
    jetDirectionUsingGhostTrack = cms.bool(False),
    minimumNumberOfPixelHits = cms.int32(2),
    jetDirectionUsingTracks = cms.bool(False),
    computeGhostTrack = cms.bool(True),
    useTrackQuality = cms.bool(False),
    maximumChiSquared = cms.double(5.0)
)


process.impactParameterTagInfosAK4PFCHS = cms.EDProducer("TrackIPProducer",
    maximumTransverseImpactParameter = cms.double(0.2),
    minimumNumberOfHits = cms.int32(8),
    minimumTransverseMomentum = cms.double(1.0),
    primaryVertex = cms.InputTag("unpackedTracksAndVertices"),
    maximumLongitudinalImpactParameter = cms.double(17.0),
    computeGhostTrack = cms.bool(True),
    ghostTrackPriorDeltaR = cms.double(0.03),
    jetTracks = cms.InputTag("jetTracksAssociatorAtVertexAK4PFCHS"),
    jetDirectionUsingGhostTrack = cms.bool(False),
    minimumNumberOfPixelHits = cms.int32(2),
    jetDirectionUsingTracks = cms.bool(False),
    computeProbabilities = cms.bool(True),
    useTrackQuality = cms.bool(False),
    maximumChiSquared = cms.double(5.0)
)


process.impactParameterTagInfosAK4PFCHSNoMu = cms.EDProducer("TrackIPProducer",
    maximumTransverseImpactParameter = cms.double(0.2),
    minimumNumberOfHits = cms.int32(8),
    minimumTransverseMomentum = cms.double(1.0),
    primaryVertex = cms.InputTag("unpackedTracksAndVertices"),
    maximumLongitudinalImpactParameter = cms.double(17.0),
    computeGhostTrack = cms.bool(True),
    ghostTrackPriorDeltaR = cms.double(0.03),
    jetTracks = cms.InputTag("jetTracksAssociatorAtVertexAK4PFCHSNoMu"),
    jetDirectionUsingGhostTrack = cms.bool(False),
    minimumNumberOfPixelHits = cms.int32(2),
    jetDirectionUsingTracks = cms.bool(False),
    computeProbabilities = cms.bool(True),
    useTrackQuality = cms.bool(False),
    maximumChiSquared = cms.double(5.0)
)


process.inclusiveSecondaryVertexFinderFilteredNegativeTagInfos = cms.EDProducer("SecondaryVertexProducer",
    extSVDeltaRToJet = cms.double(-0.5),
    beamSpotTag = cms.InputTag("offlineBeamSpot"),
    vertexReco = cms.PSet(
        seccut = cms.double(6.0),
        primcut = cms.double(1.8),
        smoothing = cms.bool(False),
        weightthreshold = cms.double(0.001),
        minweight = cms.double(0.5),
        finder = cms.string('avr')
    ),
    vertexSelection = cms.PSet(
        sortCriterium = cms.string('dist3dError')
    ),
    constraint = cms.string('BeamSpot'),
    trackIPTagInfos = cms.InputTag("impactParameterTagInfos"),
    vertexCuts = cms.PSet(
        distSig3dMax = cms.double(99999.9),
        fracPV = cms.double(0.79),
        distVal2dMax = cms.double(-0.01),
        useTrackWeights = cms.bool(True),
        maxDeltaRToJetAxis = cms.double(-0.5),
        v0Filter = cms.PSet(
            k0sMassWindow = cms.double(0.05)
        ),
        distSig2dMin = cms.double(-99999.9),
        multiplicityMin = cms.uint32(2),
        massMax = cms.double(6.5),
        distSig2dMax = cms.double(-2.0),
        distVal3dMax = cms.double(99999.9),
        minimumTrackWeight = cms.double(0.5),
        distVal3dMin = cms.double(-99999.9),
        distVal2dMin = cms.double(-2.5),
        distSig3dMin = cms.double(-99999.9)
    ),
    useExternalSV = cms.bool(True),
    minimumTrackWeight = cms.double(0.5),
    usePVError = cms.bool(True),
    trackSelection = cms.PSet(
        b_pT = cms.double(0.3684),
        a_dR = cms.double(-0.001053),
        min_pT = cms.double(120),
        max_pT = cms.double(500),
        min_pT_dRcut = cms.double(0.5),
        max_pT_trackPTcut = cms.double(3),
        max_pT_dRcut = cms.double(0.1),
        b_dR = cms.double(0.6263),
        a_pT = cms.double(0.005263),
        totalHitsMin = cms.uint32(8),
        jetDeltaRMax = cms.double(0.3),
        qualityClass = cms.string('any'),
        pixelHitsMin = cms.uint32(2),
        sip3dSigMin = cms.double(-99999.9),
        sip3dSigMax = cms.double(99999.9),
        maxDistToAxis = cms.double(0.2),
        useVariableJTA = cms.bool(False),
        sip2dValMax = cms.double(99999.9),
        maxDecayLen = cms.double(99999.9),
        ptMin = cms.double(1.0),
        sip2dSigMax = cms.double(99999.9),
        sip2dSigMin = cms.double(-99999.9),
        sip3dValMax = cms.double(99999.9),
        sip3dValMin = cms.double(-99999.9),
        sip2dValMin = cms.double(-99999.9),
        normChi2Max = cms.double(99999.9)
    ),
    trackSort = cms.string('sip3dSig'),
    extSVCollection = cms.InputTag("bToCharmDecayVertexMerged")
)


process.inclusiveSecondaryVertexFinderFilteredTagInfos = cms.EDProducer("SecondaryVertexProducer",
    extSVDeltaRToJet = cms.double(0.5),
    beamSpotTag = cms.InputTag("offlineBeamSpot"),
    vertexReco = cms.PSet(
        seccut = cms.double(6.0),
        primcut = cms.double(1.8),
        smoothing = cms.bool(False),
        weightthreshold = cms.double(0.001),
        minweight = cms.double(0.5),
        finder = cms.string('avr')
    ),
    vertexSelection = cms.PSet(
        sortCriterium = cms.string('dist3dError')
    ),
    constraint = cms.string('BeamSpot'),
    trackIPTagInfos = cms.InputTag("impactParameterTagInfos"),
    vertexCuts = cms.PSet(
        distSig3dMax = cms.double(99999.9),
        fracPV = cms.double(0.79),
        distVal2dMax = cms.double(2.5),
        useTrackWeights = cms.bool(True),
        maxDeltaRToJetAxis = cms.double(0.5),
        v0Filter = cms.PSet(
            k0sMassWindow = cms.double(0.05)
        ),
        distSig2dMin = cms.double(2.0),
        multiplicityMin = cms.uint32(2),
        massMax = cms.double(6.5),
        distSig2dMax = cms.double(99999.9),
        distVal3dMax = cms.double(99999.9),
        minimumTrackWeight = cms.double(0.5),
        distVal3dMin = cms.double(-99999.9),
        distVal2dMin = cms.double(0.01),
        distSig3dMin = cms.double(-99999.9)
    ),
    useExternalSV = cms.bool(True),
    minimumTrackWeight = cms.double(0.5),
    usePVError = cms.bool(True),
    trackSelection = cms.PSet(
        b_pT = cms.double(0.3684),
        a_dR = cms.double(-0.001053),
        min_pT = cms.double(120),
        max_pT = cms.double(500),
        min_pT_dRcut = cms.double(0.5),
        max_pT_trackPTcut = cms.double(3),
        max_pT_dRcut = cms.double(0.1),
        b_dR = cms.double(0.6263),
        a_pT = cms.double(0.005263),
        totalHitsMin = cms.uint32(8),
        jetDeltaRMax = cms.double(0.3),
        qualityClass = cms.string('any'),
        pixelHitsMin = cms.uint32(2),
        sip3dSigMin = cms.double(-99999.9),
        sip3dSigMax = cms.double(99999.9),
        maxDistToAxis = cms.double(0.2),
        useVariableJTA = cms.bool(False),
        sip2dValMax = cms.double(99999.9),
        maxDecayLen = cms.double(99999.9),
        ptMin = cms.double(1.0),
        sip2dSigMax = cms.double(99999.9),
        sip2dSigMin = cms.double(-99999.9),
        sip3dValMax = cms.double(99999.9),
        sip3dValMin = cms.double(-99999.9),
        sip2dValMin = cms.double(-99999.9),
        normChi2Max = cms.double(99999.9)
    ),
    trackSort = cms.string('sip3dSig'),
    extSVCollection = cms.InputTag("bToCharmDecayVertexMerged")
)


process.inclusiveSecondaryVertexFinderNegativeTagInfos = cms.EDProducer("SecondaryVertexProducer",
    extSVDeltaRToJet = cms.double(-0.3),
    beamSpotTag = cms.InputTag("offlineBeamSpot"),
    vertexReco = cms.PSet(
        seccut = cms.double(6.0),
        primcut = cms.double(1.8),
        smoothing = cms.bool(False),
        weightthreshold = cms.double(0.001),
        minweight = cms.double(0.5),
        finder = cms.string('avr')
    ),
    vertexSelection = cms.PSet(
        sortCriterium = cms.string('dist3dError')
    ),
    constraint = cms.string('BeamSpot'),
    trackIPTagInfos = cms.InputTag("impactParameterTagInfos"),
    vertexCuts = cms.PSet(
        distSig3dMax = cms.double(99999.9),
        fracPV = cms.double(0.79),
        distVal2dMax = cms.double(-0.01),
        useTrackWeights = cms.bool(True),
        maxDeltaRToJetAxis = cms.double(-0.5),
        v0Filter = cms.PSet(
            k0sMassWindow = cms.double(0.05)
        ),
        distSig2dMin = cms.double(-99999.9),
        multiplicityMin = cms.uint32(2),
        massMax = cms.double(6.5),
        distSig2dMax = cms.double(-2.0),
        distVal3dMax = cms.double(99999.9),
        minimumTrackWeight = cms.double(0.5),
        distVal3dMin = cms.double(-99999.9),
        distVal2dMin = cms.double(-2.5),
        distSig3dMin = cms.double(-99999.9)
    ),
    useExternalSV = cms.bool(True),
    minimumTrackWeight = cms.double(0.5),
    usePVError = cms.bool(True),
    trackSelection = cms.PSet(
        b_pT = cms.double(0.3684),
        a_dR = cms.double(-0.001053),
        min_pT = cms.double(120),
        max_pT = cms.double(500),
        min_pT_dRcut = cms.double(0.5),
        max_pT_trackPTcut = cms.double(3),
        max_pT_dRcut = cms.double(0.1),
        b_dR = cms.double(0.6263),
        a_pT = cms.double(0.005263),
        totalHitsMin = cms.uint32(8),
        jetDeltaRMax = cms.double(0.3),
        qualityClass = cms.string('any'),
        pixelHitsMin = cms.uint32(2),
        sip3dSigMin = cms.double(-99999.9),
        sip3dSigMax = cms.double(99999.9),
        maxDistToAxis = cms.double(0.2),
        useVariableJTA = cms.bool(False),
        sip2dValMax = cms.double(99999.9),
        maxDecayLen = cms.double(99999.9),
        ptMin = cms.double(1.0),
        sip2dSigMax = cms.double(99999.9),
        sip2dSigMin = cms.double(-99999.9),
        sip3dValMax = cms.double(99999.9),
        sip3dValMin = cms.double(-99999.9),
        sip2dValMin = cms.double(-99999.9),
        normChi2Max = cms.double(99999.9)
    ),
    trackSort = cms.string('sip3dSig'),
    extSVCollection = cms.InputTag("inclusiveSecondaryVertices")
)


process.inclusiveSecondaryVertexFinderTagInfos = cms.EDProducer("SecondaryVertexProducer",
    extSVDeltaRToJet = cms.double(0.3),
    beamSpotTag = cms.InputTag("offlineBeamSpot"),
    vertexReco = cms.PSet(
        seccut = cms.double(6.0),
        primcut = cms.double(1.8),
        smoothing = cms.bool(False),
        weightthreshold = cms.double(0.001),
        minweight = cms.double(0.5),
        finder = cms.string('avr')
    ),
    vertexSelection = cms.PSet(
        sortCriterium = cms.string('dist3dError')
    ),
    constraint = cms.string('BeamSpot'),
    trackIPTagInfos = cms.InputTag("impactParameterTagInfos"),
    vertexCuts = cms.PSet(
        distSig3dMax = cms.double(99999.9),
        fracPV = cms.double(0.79),
        distVal2dMax = cms.double(2.5),
        useTrackWeights = cms.bool(True),
        maxDeltaRToJetAxis = cms.double(0.5),
        v0Filter = cms.PSet(
            k0sMassWindow = cms.double(0.05)
        ),
        distSig2dMin = cms.double(2.0),
        multiplicityMin = cms.uint32(2),
        massMax = cms.double(6.5),
        distSig2dMax = cms.double(99999.9),
        distVal3dMax = cms.double(99999.9),
        minimumTrackWeight = cms.double(0.5),
        distVal3dMin = cms.double(-99999.9),
        distVal2dMin = cms.double(0.01),
        distSig3dMin = cms.double(-99999.9)
    ),
    useExternalSV = cms.bool(True),
    minimumTrackWeight = cms.double(0.5),
    usePVError = cms.bool(True),
    trackSelection = cms.PSet(
        b_pT = cms.double(0.3684),
        a_dR = cms.double(-0.001053),
        min_pT = cms.double(120),
        max_pT = cms.double(500),
        min_pT_dRcut = cms.double(0.5),
        max_pT_trackPTcut = cms.double(3),
        max_pT_dRcut = cms.double(0.1),
        b_dR = cms.double(0.6263),
        a_pT = cms.double(0.005263),
        totalHitsMin = cms.uint32(8),
        jetDeltaRMax = cms.double(0.3),
        qualityClass = cms.string('any'),
        pixelHitsMin = cms.uint32(2),
        sip3dSigMin = cms.double(-99999.9),
        sip3dSigMax = cms.double(99999.9),
        maxDistToAxis = cms.double(0.2),
        useVariableJTA = cms.bool(False),
        sip2dValMax = cms.double(99999.9),
        maxDecayLen = cms.double(99999.9),
        ptMin = cms.double(1.0),
        sip2dSigMax = cms.double(99999.9),
        sip2dSigMin = cms.double(-99999.9),
        sip3dValMax = cms.double(99999.9),
        sip3dValMin = cms.double(-99999.9),
        sip2dValMin = cms.double(-99999.9),
        normChi2Max = cms.double(99999.9)
    ),
    trackSort = cms.string('sip3dSig'),
    extSVCollection = cms.InputTag("inclusiveSecondaryVertices")
)


process.inclusiveSecondaryVertexFinderTagInfosAK4PFCHS = cms.EDProducer("SecondaryVertexProducer",
    extSVDeltaRToJet = cms.double(0.3),
    beamSpotTag = cms.InputTag("offlineBeamSpot"),
    vertexReco = cms.PSet(
        seccut = cms.double(6.0),
        primcut = cms.double(1.8),
        smoothing = cms.bool(False),
        weightthreshold = cms.double(0.001),
        minweight = cms.double(0.5),
        finder = cms.string('avr')
    ),
    constraint = cms.string('BeamSpot'),
    vertexSelection = cms.PSet(
        sortCriterium = cms.string('dist3dError')
    ),
    useExternalSV = cms.bool(True),
    vertexCuts = cms.PSet(
        distSig3dMax = cms.double(99999.9),
        fracPV = cms.double(0.79),
        distVal2dMax = cms.double(2.5),
        useTrackWeights = cms.bool(True),
        maxDeltaRToJetAxis = cms.double(0.5),
        v0Filter = cms.PSet(
            k0sMassWindow = cms.double(0.05)
        ),
        distSig2dMin = cms.double(2.0),
        multiplicityMin = cms.uint32(2),
        massMax = cms.double(6.5),
        distSig2dMax = cms.double(99999.9),
        distVal3dMax = cms.double(99999.9),
        minimumTrackWeight = cms.double(0.5),
        distVal3dMin = cms.double(-99999.9),
        distVal2dMin = cms.double(0.01),
        distSig3dMin = cms.double(-99999.9)
    ),
    trackIPTagInfos = cms.InputTag("impactParameterTagInfosAK4PFCHS"),
    minimumTrackWeight = cms.double(0.5),
    usePVError = cms.bool(True),
    trackSelection = cms.PSet(
        b_pT = cms.double(0.3684),
        a_dR = cms.double(-0.001053),
        min_pT = cms.double(120),
        max_pT = cms.double(500),
        min_pT_dRcut = cms.double(0.5),
        max_pT_trackPTcut = cms.double(3),
        max_pT_dRcut = cms.double(0.1),
        b_dR = cms.double(0.6263),
        a_pT = cms.double(0.005263),
        totalHitsMin = cms.uint32(8),
        jetDeltaRMax = cms.double(0.3),
        qualityClass = cms.string('any'),
        pixelHitsMin = cms.uint32(2),
        sip3dSigMin = cms.double(-99999.9),
        sip3dSigMax = cms.double(99999.9),
        maxDistToAxis = cms.double(0.2),
        useVariableJTA = cms.bool(False),
        sip2dValMax = cms.double(99999.9),
        maxDecayLen = cms.double(99999.9),
        ptMin = cms.double(1.0),
        sip2dSigMax = cms.double(99999.9),
        sip2dSigMin = cms.double(-99999.9),
        sip3dValMax = cms.double(99999.9),
        sip3dValMin = cms.double(-99999.9),
        sip2dValMin = cms.double(-99999.9),
        normChi2Max = cms.double(99999.9)
    ),
    trackSort = cms.string('sip3dSig'),
    extSVCollection = cms.InputTag("unpackedTracksAndVertices","secondary")
)


process.inclusiveSecondaryVertexFinderTagInfosAK4PFCHSNoMu = cms.EDProducer("SecondaryVertexProducer",
    extSVDeltaRToJet = cms.double(0.3),
    beamSpotTag = cms.InputTag("offlineBeamSpot"),
    vertexReco = cms.PSet(
        seccut = cms.double(6.0),
        primcut = cms.double(1.8),
        smoothing = cms.bool(False),
        weightthreshold = cms.double(0.001),
        minweight = cms.double(0.5),
        finder = cms.string('avr')
    ),
    constraint = cms.string('BeamSpot'),
    vertexSelection = cms.PSet(
        sortCriterium = cms.string('dist3dError')
    ),
    useExternalSV = cms.bool(True),
    vertexCuts = cms.PSet(
        distSig3dMax = cms.double(99999.9),
        fracPV = cms.double(0.79),
        distVal2dMax = cms.double(2.5),
        useTrackWeights = cms.bool(True),
        maxDeltaRToJetAxis = cms.double(0.5),
        v0Filter = cms.PSet(
            k0sMassWindow = cms.double(0.05)
        ),
        distSig2dMin = cms.double(2.0),
        multiplicityMin = cms.uint32(2),
        massMax = cms.double(6.5),
        distSig2dMax = cms.double(99999.9),
        distVal3dMax = cms.double(99999.9),
        minimumTrackWeight = cms.double(0.5),
        distVal3dMin = cms.double(-99999.9),
        distVal2dMin = cms.double(0.01),
        distSig3dMin = cms.double(-99999.9)
    ),
    trackIPTagInfos = cms.InputTag("impactParameterTagInfosAK4PFCHSNoMu"),
    minimumTrackWeight = cms.double(0.5),
    usePVError = cms.bool(True),
    trackSelection = cms.PSet(
        b_pT = cms.double(0.3684),
        a_dR = cms.double(-0.001053),
        min_pT = cms.double(120),
        max_pT = cms.double(500),
        min_pT_dRcut = cms.double(0.5),
        max_pT_trackPTcut = cms.double(3),
        max_pT_dRcut = cms.double(0.1),
        b_dR = cms.double(0.6263),
        a_pT = cms.double(0.005263),
        totalHitsMin = cms.uint32(8),
        jetDeltaRMax = cms.double(0.3),
        qualityClass = cms.string('any'),
        pixelHitsMin = cms.uint32(2),
        sip3dSigMin = cms.double(-99999.9),
        sip3dSigMax = cms.double(99999.9),
        maxDistToAxis = cms.double(0.2),
        useVariableJTA = cms.bool(False),
        sip2dValMax = cms.double(99999.9),
        maxDecayLen = cms.double(99999.9),
        ptMin = cms.double(1.0),
        sip2dSigMax = cms.double(99999.9),
        sip2dSigMin = cms.double(-99999.9),
        sip3dValMax = cms.double(99999.9),
        sip3dValMin = cms.double(-99999.9),
        sip2dValMin = cms.double(-99999.9),
        normChi2Max = cms.double(99999.9)
    ),
    trackSort = cms.string('sip3dSig'),
    extSVCollection = cms.InputTag("unpackedTracksAndVertices","secondary")
)


process.inclusiveSecondaryVertices = cms.EDProducer("VertexMerger",
    minSignificance = cms.double(10.0),
    secondaryVertices = cms.InputTag("trackVertexArbitrator"),
    maxFraction = cms.double(0.2)
)


process.inclusiveVertexFinder = cms.EDProducer("InclusiveVertexFinder",
    fitterSigmacut = cms.double(3),
    vertexReco = cms.PSet(
        seccut = cms.double(3),
        primcut = cms.double(1.0),
        finder = cms.string('avr'),
        smoothing = cms.bool(True)
    ),
    fitterTini = cms.double(256),
    fitterRatio = cms.double(0.25),
    vertexMinDLen2DSig = cms.double(2.5),
    maximumLongitudinalImpactParameter = cms.double(0.3),
    vertexMinAngleCosine = cms.double(0.95),
    primaryVertices = cms.InputTag("offlinePrimaryVertices"),
    tracks = cms.InputTag("generalTracks"),
    minPt = cms.double(0.8),
    maxNTracks = cms.uint32(30),
    useVertexReco = cms.bool(True),
    vertexMinDLenSig = cms.double(0.5),
    useDirectVertexFitter = cms.bool(True),
    minHits = cms.uint32(8),
    beamSpot = cms.InputTag("offlineBeamSpot"),
    clusterizer = cms.PSet(
        seedMin3DIPValue = cms.double(0.005),
        clusterMaxDistance = cms.double(0.05),
        seedMin3DIPSignificance = cms.double(1.2),
        seedMax3DIPSignificance = cms.double(9999.0),
        distanceRatio = cms.double(20),
        clusterMaxSignificance = cms.double(4.5),
        clusterMinAngleCosine = cms.double(0.5),
        seedMax3DIPValue = cms.double(9999.0)
    )
)


process.isoDeposits = cms.EDProducer("CandIsoDepositProducer",
    src = cms.InputTag(""),
    MultipleDepositsFlag = cms.bool(False),
    trackType = cms.string('candidate'),
    ExtractorPSet = cms.PSet(
        Diff_z = cms.double(99999.99),
        ComponentName = cms.string('CandViewExtractor'),
        DR_Max = cms.double(1.0),
        Diff_r = cms.double(99999.99),
        inputCandView = cms.InputTag(""),
        DR_Veto = cms.double(1e-05),
        DepositLabel = cms.untracked.string('')
    )
)


process.jetBProbabilityBJetTags = cms.EDProducer("JetTagProducer",
    jetTagComputer = cms.string('jetBProbability'),
    tagInfos = cms.VInputTag(cms.InputTag("impactParameterTagInfos"))
)


process.jetProbabilityBJetTags = cms.EDProducer("JetTagProducer",
    jetTagComputer = cms.string('jetProbability'),
    tagInfos = cms.VInputTag(cms.InputTag("impactParameterTagInfos"))
)


process.jetTracksAssociatorAtVertexAK4PFCHS = cms.EDProducer("JetTracksAssociatorAtVertex",
    jets = cms.InputTag("ak4PFJetsCHS"),
    tracks = cms.InputTag("unpackedTracksAndVertices"),
    useAssigned = cms.bool(False),
    coneSize = cms.double(0.4),
    pvSrc = cms.InputTag("unpackedTracksAndVertices")
)


process.jetTracksAssociatorAtVertexAK4PFCHSNoMu = cms.EDProducer("JetTracksAssociatorAtVertex",
    jets = cms.InputTag("ak4PFJetsCHSNoMu"),
    tracks = cms.InputTag("unpackedTracksAndVertices"),
    useAssigned = cms.bool(False),
    coneSize = cms.double(0.4),
    pvSrc = cms.InputTag("unpackedTracksAndVertices")
)


process.mht = cms.EDProducer("MHTProducer",
    JetCollection = cms.InputTag("patJets"),
    MaxJetEta = cms.double(5),
    MinJetPt = cms.double(30)
)


process.mhtPFchs = cms.EDProducer("MHTProducer",
    JetCollection = cms.InputTag("ak4patJetsPFchsPt30"),
    MaxJetEta = cms.double(5),
    MinJetPt = cms.double(30)
)


process.muCaloMetCorr = cms.EDProducer("MuonMETcorrInputProducer",
    srcMuonCorrections = cms.InputTag("muonMETValueMapProducer","muCorrData"),
    src = cms.InputTag("muons")
)


process.muPFIsoDepositCharged = cms.EDProducer("CandIsoDepositProducer",
    src = cms.InputTag("muons"),
    MultipleDepositsFlag = cms.bool(False),
    trackType = cms.string('candidate'),
    ExtractorPSet = cms.PSet(
        Diff_z = cms.double(99999.99),
        ComponentName = cms.string('CandViewExtractor'),
        DR_Max = cms.double(1.0),
        Diff_r = cms.double(99999.99),
        inputCandView = cms.InputTag("pfAllChargedHadrons"),
        DR_Veto = cms.double(1e-05),
        DepositLabel = cms.untracked.string('')
    )
)


process.muPFIsoDepositChargedAll = cms.EDProducer("CandIsoDepositProducer",
    src = cms.InputTag("muons"),
    MultipleDepositsFlag = cms.bool(False),
    trackType = cms.string('candidate'),
    ExtractorPSet = cms.PSet(
        Diff_z = cms.double(99999.99),
        ComponentName = cms.string('CandViewExtractor'),
        DR_Max = cms.double(1.0),
        Diff_r = cms.double(99999.99),
        inputCandView = cms.InputTag("pfAllChargedParticles"),
        DR_Veto = cms.double(1e-05),
        DepositLabel = cms.untracked.string('')
    )
)


process.muPFIsoDepositGamma = cms.EDProducer("CandIsoDepositProducer",
    src = cms.InputTag("muons"),
    MultipleDepositsFlag = cms.bool(False),
    trackType = cms.string('candidate'),
    ExtractorPSet = cms.PSet(
        Diff_z = cms.double(99999.99),
        ComponentName = cms.string('CandViewExtractor'),
        DR_Max = cms.double(1.0),
        Diff_r = cms.double(99999.99),
        inputCandView = cms.InputTag("pfAllPhotons"),
        DR_Veto = cms.double(1e-05),
        DepositLabel = cms.untracked.string('')
    )
)


process.muPFIsoDepositNeutral = cms.EDProducer("CandIsoDepositProducer",
    src = cms.InputTag("muons"),
    MultipleDepositsFlag = cms.bool(False),
    trackType = cms.string('candidate'),
    ExtractorPSet = cms.PSet(
        Diff_z = cms.double(99999.99),
        ComponentName = cms.string('CandViewExtractor'),
        DR_Max = cms.double(1.0),
        Diff_r = cms.double(99999.99),
        inputCandView = cms.InputTag("pfAllNeutralHadrons"),
        DR_Veto = cms.double(1e-05),
        DepositLabel = cms.untracked.string('')
    )
)


process.muPFIsoDepositPU = cms.EDProducer("CandIsoDepositProducer",
    src = cms.InputTag("muons"),
    MultipleDepositsFlag = cms.bool(False),
    trackType = cms.string('candidate'),
    ExtractorPSet = cms.PSet(
        Diff_z = cms.double(99999.99),
        ComponentName = cms.string('CandViewExtractor'),
        DR_Max = cms.double(1.0),
        Diff_r = cms.double(99999.99),
        inputCandView = cms.InputTag("pfPileUpAllChargedParticles"),
        DR_Veto = cms.double(1e-05),
        DepositLabel = cms.untracked.string('')
    )
)


process.muPFIsoValueCharged03 = cms.EDProducer("CandIsolatorFromDeposits",
    deposits = cms.VPSet(cms.PSet(
        src = cms.InputTag("muPFIsoDepositCharged"),
        deltaR = cms.double(0.3),
        weight = cms.string('1'),
        vetos = cms.vstring('0.0001', 
            'Threshold(0.0)'),
        skipDefaultVeto = cms.bool(True),
        mode = cms.string('sum')
    ))
)


process.muPFIsoValueCharged04 = cms.EDProducer("CandIsolatorFromDeposits",
    deposits = cms.VPSet(cms.PSet(
        src = cms.InputTag("muPFIsoDepositCharged"),
        deltaR = cms.double(0.4),
        weight = cms.string('1'),
        vetos = cms.vstring('0.0001', 
            'Threshold(0.0)'),
        skipDefaultVeto = cms.bool(True),
        mode = cms.string('sum')
    ))
)


process.muPFIsoValueChargedAll03 = cms.EDProducer("CandIsolatorFromDeposits",
    deposits = cms.VPSet(cms.PSet(
        src = cms.InputTag("muPFIsoDepositChargedAll"),
        deltaR = cms.double(0.3),
        weight = cms.string('1'),
        vetos = cms.vstring('0.0001', 
            'Threshold(0.0)'),
        skipDefaultVeto = cms.bool(True),
        mode = cms.string('sum')
    ))
)


process.muPFIsoValueChargedAll04 = cms.EDProducer("CandIsolatorFromDeposits",
    deposits = cms.VPSet(cms.PSet(
        src = cms.InputTag("muPFIsoDepositChargedAll"),
        deltaR = cms.double(0.4),
        weight = cms.string('1'),
        vetos = cms.vstring('0.0001', 
            'Threshold(0.0)'),
        skipDefaultVeto = cms.bool(True),
        mode = cms.string('sum')
    ))
)


process.muPFIsoValueGamma03 = cms.EDProducer("CandIsolatorFromDeposits",
    deposits = cms.VPSet(cms.PSet(
        src = cms.InputTag("muPFIsoDepositGamma"),
        deltaR = cms.double(0.3),
        weight = cms.string('1'),
        vetos = cms.vstring('0.01', 
            'Threshold(0.5)'),
        skipDefaultVeto = cms.bool(True),
        mode = cms.string('sum')
    ))
)


process.muPFIsoValueGamma04 = cms.EDProducer("CandIsolatorFromDeposits",
    deposits = cms.VPSet(cms.PSet(
        src = cms.InputTag("muPFIsoDepositGamma"),
        deltaR = cms.double(0.4),
        weight = cms.string('1'),
        vetos = cms.vstring('0.01', 
            'Threshold(0.5)'),
        skipDefaultVeto = cms.bool(True),
        mode = cms.string('sum')
    ))
)


process.muPFIsoValueGammaHighThreshold03 = cms.EDProducer("CandIsolatorFromDeposits",
    deposits = cms.VPSet(cms.PSet(
        src = cms.InputTag("muPFIsoDepositGamma"),
        deltaR = cms.double(0.3),
        weight = cms.string('1'),
        vetos = cms.vstring('0.01', 
            'Threshold(1.0)'),
        skipDefaultVeto = cms.bool(True),
        mode = cms.string('sum')
    ))
)


process.muPFIsoValueGammaHighThreshold04 = cms.EDProducer("CandIsolatorFromDeposits",
    deposits = cms.VPSet(cms.PSet(
        src = cms.InputTag("muPFIsoDepositGamma"),
        deltaR = cms.double(0.4),
        weight = cms.string('1'),
        vetos = cms.vstring('0.01', 
            'Threshold(1.0)'),
        skipDefaultVeto = cms.bool(True),
        mode = cms.string('sum')
    ))
)


process.muPFIsoValueNeutral03 = cms.EDProducer("CandIsolatorFromDeposits",
    deposits = cms.VPSet(cms.PSet(
        src = cms.InputTag("muPFIsoDepositNeutral"),
        deltaR = cms.double(0.3),
        weight = cms.string('1'),
        vetos = cms.vstring('0.01', 
            'Threshold(0.5)'),
        skipDefaultVeto = cms.bool(True),
        mode = cms.string('sum')
    ))
)


process.muPFIsoValueNeutral04 = cms.EDProducer("CandIsolatorFromDeposits",
    deposits = cms.VPSet(cms.PSet(
        src = cms.InputTag("muPFIsoDepositNeutral"),
        deltaR = cms.double(0.4),
        weight = cms.string('1'),
        vetos = cms.vstring('0.01', 
            'Threshold(0.5)'),
        skipDefaultVeto = cms.bool(True),
        mode = cms.string('sum')
    ))
)


process.muPFIsoValueNeutralHighThreshold03 = cms.EDProducer("CandIsolatorFromDeposits",
    deposits = cms.VPSet(cms.PSet(
        src = cms.InputTag("muPFIsoDepositNeutral"),
        deltaR = cms.double(0.3),
        weight = cms.string('1'),
        vetos = cms.vstring('0.01', 
            'Threshold(1.0)'),
        skipDefaultVeto = cms.bool(True),
        mode = cms.string('sum')
    ))
)


process.muPFIsoValueNeutralHighThreshold04 = cms.EDProducer("CandIsolatorFromDeposits",
    deposits = cms.VPSet(cms.PSet(
        src = cms.InputTag("muPFIsoDepositNeutral"),
        deltaR = cms.double(0.4),
        weight = cms.string('1'),
        vetos = cms.vstring('0.01', 
            'Threshold(1.0)'),
        skipDefaultVeto = cms.bool(True),
        mode = cms.string('sum')
    ))
)


process.muPFIsoValuePU03 = cms.EDProducer("CandIsolatorFromDeposits",
    deposits = cms.VPSet(cms.PSet(
        src = cms.InputTag("muPFIsoDepositPU"),
        deltaR = cms.double(0.3),
        weight = cms.string('1'),
        vetos = cms.vstring('0.01', 
            'Threshold(0.5)'),
        skipDefaultVeto = cms.bool(True),
        mode = cms.string('sum')
    ))
)


process.muPFIsoValuePU04 = cms.EDProducer("CandIsolatorFromDeposits",
    deposits = cms.VPSet(cms.PSet(
        src = cms.InputTag("muPFIsoDepositPU"),
        deltaR = cms.double(0.4),
        weight = cms.string('1'),
        vetos = cms.vstring('0.01', 
            'Threshold(0.5)'),
        skipDefaultVeto = cms.bool(True),
        mode = cms.string('sum')
    ))
)


process.muPFMeanDRIsoValueCharged03 = cms.EDProducer("CandIsolatorFromDeposits",
    deposits = cms.VPSet(cms.PSet(
        src = cms.InputTag("muPFIsoDepositCharged"),
        deltaR = cms.double(0.3),
        weight = cms.string('1'),
        vetos = cms.vstring('0.0001', 
            'Threshold(0.0)'),
        skipDefaultVeto = cms.bool(True),
        mode = cms.string('meanDR')
    ))
)


process.muPFMeanDRIsoValueCharged04 = cms.EDProducer("CandIsolatorFromDeposits",
    deposits = cms.VPSet(cms.PSet(
        src = cms.InputTag("muPFIsoDepositCharged"),
        deltaR = cms.double(0.4),
        weight = cms.string('1'),
        vetos = cms.vstring('0.0001', 
            'Threshold(0.0)'),
        skipDefaultVeto = cms.bool(True),
        mode = cms.string('meanDR')
    ))
)


process.muPFMeanDRIsoValueChargedAll03 = cms.EDProducer("CandIsolatorFromDeposits",
    deposits = cms.VPSet(cms.PSet(
        src = cms.InputTag("muPFIsoDepositChargedAll"),
        deltaR = cms.double(0.3),
        weight = cms.string('1'),
        vetos = cms.vstring('0.0001', 
            'Threshold(0.0)'),
        skipDefaultVeto = cms.bool(True),
        mode = cms.string('meanDR')
    ))
)


process.muPFMeanDRIsoValueChargedAll04 = cms.EDProducer("CandIsolatorFromDeposits",
    deposits = cms.VPSet(cms.PSet(
        src = cms.InputTag("muPFIsoDepositChargedAll"),
        deltaR = cms.double(0.4),
        weight = cms.string('1'),
        vetos = cms.vstring('0.0001', 
            'Threshold(0.0)'),
        skipDefaultVeto = cms.bool(True),
        mode = cms.string('meanDR')
    ))
)


process.muPFMeanDRIsoValueGamma03 = cms.EDProducer("CandIsolatorFromDeposits",
    deposits = cms.VPSet(cms.PSet(
        src = cms.InputTag("muPFIsoDepositGamma"),
        deltaR = cms.double(0.3),
        weight = cms.string('1'),
        vetos = cms.vstring('0.01', 
            'Threshold(0.5)'),
        skipDefaultVeto = cms.bool(True),
        mode = cms.string('meanDR')
    ))
)


process.muPFMeanDRIsoValueGamma04 = cms.EDProducer("CandIsolatorFromDeposits",
    deposits = cms.VPSet(cms.PSet(
        src = cms.InputTag("muPFIsoDepositGamma"),
        deltaR = cms.double(0.4),
        weight = cms.string('1'),
        vetos = cms.vstring('0.01', 
            'Threshold(0.5)'),
        skipDefaultVeto = cms.bool(True),
        mode = cms.string('meanDR')
    ))
)


process.muPFMeanDRIsoValueGammaHighThreshold03 = cms.EDProducer("CandIsolatorFromDeposits",
    deposits = cms.VPSet(cms.PSet(
        src = cms.InputTag("muPFIsoDepositGamma"),
        deltaR = cms.double(0.3),
        weight = cms.string('1'),
        vetos = cms.vstring('0.01', 
            'Threshold(1.0)'),
        skipDefaultVeto = cms.bool(True),
        mode = cms.string('meanDR')
    ))
)


process.muPFMeanDRIsoValueGammaHighThreshold04 = cms.EDProducer("CandIsolatorFromDeposits",
    deposits = cms.VPSet(cms.PSet(
        src = cms.InputTag("muPFIsoDepositGamma"),
        deltaR = cms.double(0.4),
        weight = cms.string('1'),
        vetos = cms.vstring('0.01', 
            'Threshold(1.0)'),
        skipDefaultVeto = cms.bool(True),
        mode = cms.string('meanDR')
    ))
)


process.muPFMeanDRIsoValueNeutral03 = cms.EDProducer("CandIsolatorFromDeposits",
    deposits = cms.VPSet(cms.PSet(
        src = cms.InputTag("muPFIsoDepositNeutral"),
        deltaR = cms.double(0.3),
        weight = cms.string('1'),
        vetos = cms.vstring('0.01', 
            'Threshold(0.5)'),
        skipDefaultVeto = cms.bool(True),
        mode = cms.string('meanDR')
    ))
)


process.muPFMeanDRIsoValueNeutral04 = cms.EDProducer("CandIsolatorFromDeposits",
    deposits = cms.VPSet(cms.PSet(
        src = cms.InputTag("muPFIsoDepositNeutral"),
        deltaR = cms.double(0.4),
        weight = cms.string('1'),
        vetos = cms.vstring('0.01', 
            'Threshold(0.5)'),
        skipDefaultVeto = cms.bool(True),
        mode = cms.string('meanDR')
    ))
)


process.muPFMeanDRIsoValueNeutralHighThreshold03 = cms.EDProducer("CandIsolatorFromDeposits",
    deposits = cms.VPSet(cms.PSet(
        src = cms.InputTag("muPFIsoDepositNeutral"),
        deltaR = cms.double(0.3),
        weight = cms.string('1'),
        vetos = cms.vstring('0.01', 
            'Threshold(1.0)'),
        skipDefaultVeto = cms.bool(True),
        mode = cms.string('meanDR')
    ))
)


process.muPFMeanDRIsoValueNeutralHighThreshold04 = cms.EDProducer("CandIsolatorFromDeposits",
    deposits = cms.VPSet(cms.PSet(
        src = cms.InputTag("muPFIsoDepositNeutral"),
        deltaR = cms.double(0.4),
        weight = cms.string('1'),
        vetos = cms.vstring('0.01', 
            'Threshold(1.0)'),
        skipDefaultVeto = cms.bool(True),
        mode = cms.string('meanDR')
    ))
)


process.muPFMeanDRIsoValuePU03 = cms.EDProducer("CandIsolatorFromDeposits",
    deposits = cms.VPSet(cms.PSet(
        src = cms.InputTag("muPFIsoDepositPU"),
        deltaR = cms.double(0.3),
        weight = cms.string('1'),
        vetos = cms.vstring('0.01', 
            'Threshold(0.5)'),
        skipDefaultVeto = cms.bool(True),
        mode = cms.string('meanDR')
    ))
)


process.muPFMeanDRIsoValuePU04 = cms.EDProducer("CandIsolatorFromDeposits",
    deposits = cms.VPSet(cms.PSet(
        src = cms.InputTag("muPFIsoDepositPU"),
        deltaR = cms.double(0.4),
        weight = cms.string('1'),
        vetos = cms.vstring('0.01', 
            'Threshold(0.5)'),
        skipDefaultVeto = cms.bool(True),
        mode = cms.string('meanDR')
    ))
)


process.muPFSumDRIsoValueCharged03 = cms.EDProducer("CandIsolatorFromDeposits",
    deposits = cms.VPSet(cms.PSet(
        src = cms.InputTag("muPFIsoDepositCharged"),
        deltaR = cms.double(0.3),
        weight = cms.string('1'),
        vetos = cms.vstring('0.0001', 
            'Threshold(0.0)'),
        skipDefaultVeto = cms.bool(True),
        mode = cms.string('sumDR')
    ))
)


process.muPFSumDRIsoValueCharged04 = cms.EDProducer("CandIsolatorFromDeposits",
    deposits = cms.VPSet(cms.PSet(
        src = cms.InputTag("muPFIsoDepositCharged"),
        deltaR = cms.double(0.4),
        weight = cms.string('1'),
        vetos = cms.vstring('0.0001', 
            'Threshold(0.0)'),
        skipDefaultVeto = cms.bool(True),
        mode = cms.string('sumDR')
    ))
)


process.muPFSumDRIsoValueChargedAll03 = cms.EDProducer("CandIsolatorFromDeposits",
    deposits = cms.VPSet(cms.PSet(
        src = cms.InputTag("muPFIsoDepositChargedAll"),
        deltaR = cms.double(0.3),
        weight = cms.string('1'),
        vetos = cms.vstring('0.0001', 
            'Threshold(0.0)'),
        skipDefaultVeto = cms.bool(True),
        mode = cms.string('sumDR')
    ))
)


process.muPFSumDRIsoValueChargedAll04 = cms.EDProducer("CandIsolatorFromDeposits",
    deposits = cms.VPSet(cms.PSet(
        src = cms.InputTag("muPFIsoDepositChargedAll"),
        deltaR = cms.double(0.4),
        weight = cms.string('1'),
        vetos = cms.vstring('0.0001', 
            'Threshold(0.0)'),
        skipDefaultVeto = cms.bool(True),
        mode = cms.string('sumDR')
    ))
)


process.muPFSumDRIsoValueGamma03 = cms.EDProducer("CandIsolatorFromDeposits",
    deposits = cms.VPSet(cms.PSet(
        src = cms.InputTag("muPFIsoDepositGamma"),
        deltaR = cms.double(0.3),
        weight = cms.string('1'),
        vetos = cms.vstring('0.01', 
            'Threshold(0.5)'),
        skipDefaultVeto = cms.bool(True),
        mode = cms.string('sumDR')
    ))
)


process.muPFSumDRIsoValueGamma04 = cms.EDProducer("CandIsolatorFromDeposits",
    deposits = cms.VPSet(cms.PSet(
        src = cms.InputTag("muPFIsoDepositGamma"),
        deltaR = cms.double(0.4),
        weight = cms.string('1'),
        vetos = cms.vstring('0.01', 
            'Threshold(0.5)'),
        skipDefaultVeto = cms.bool(True),
        mode = cms.string('sumDR')
    ))
)


process.muPFSumDRIsoValueGammaHighThreshold03 = cms.EDProducer("CandIsolatorFromDeposits",
    deposits = cms.VPSet(cms.PSet(
        src = cms.InputTag("muPFIsoDepositGamma"),
        deltaR = cms.double(0.3),
        weight = cms.string('1'),
        vetos = cms.vstring('0.01', 
            'Threshold(1.0)'),
        skipDefaultVeto = cms.bool(True),
        mode = cms.string('sumDR')
    ))
)


process.muPFSumDRIsoValueGammaHighThreshold04 = cms.EDProducer("CandIsolatorFromDeposits",
    deposits = cms.VPSet(cms.PSet(
        src = cms.InputTag("muPFIsoDepositGamma"),
        deltaR = cms.double(0.4),
        weight = cms.string('1'),
        vetos = cms.vstring('0.01', 
            'Threshold(1.0)'),
        skipDefaultVeto = cms.bool(True),
        mode = cms.string('sumDR')
    ))
)


process.muPFSumDRIsoValueNeutral03 = cms.EDProducer("CandIsolatorFromDeposits",
    deposits = cms.VPSet(cms.PSet(
        src = cms.InputTag("muPFIsoDepositNeutral"),
        deltaR = cms.double(0.3),
        weight = cms.string('1'),
        vetos = cms.vstring('0.01', 
            'Threshold(0.5)'),
        skipDefaultVeto = cms.bool(True),
        mode = cms.string('sumDR')
    ))
)


process.muPFSumDRIsoValueNeutral04 = cms.EDProducer("CandIsolatorFromDeposits",
    deposits = cms.VPSet(cms.PSet(
        src = cms.InputTag("muPFIsoDepositNeutral"),
        deltaR = cms.double(0.4),
        weight = cms.string('1'),
        vetos = cms.vstring('0.01', 
            'Threshold(0.5)'),
        skipDefaultVeto = cms.bool(True),
        mode = cms.string('sumDR')
    ))
)


process.muPFSumDRIsoValueNeutralHighThreshold03 = cms.EDProducer("CandIsolatorFromDeposits",
    deposits = cms.VPSet(cms.PSet(
        src = cms.InputTag("muPFIsoDepositNeutral"),
        deltaR = cms.double(0.3),
        weight = cms.string('1'),
        vetos = cms.vstring('0.01', 
            'Threshold(1.0)'),
        skipDefaultVeto = cms.bool(True),
        mode = cms.string('sumDR')
    ))
)


process.muPFSumDRIsoValueNeutralHighThreshold04 = cms.EDProducer("CandIsolatorFromDeposits",
    deposits = cms.VPSet(cms.PSet(
        src = cms.InputTag("muPFIsoDepositNeutral"),
        deltaR = cms.double(0.4),
        weight = cms.string('1'),
        vetos = cms.vstring('0.01', 
            'Threshold(1.0)'),
        skipDefaultVeto = cms.bool(True),
        mode = cms.string('sumDR')
    ))
)


process.muPFSumDRIsoValuePU03 = cms.EDProducer("CandIsolatorFromDeposits",
    deposits = cms.VPSet(cms.PSet(
        src = cms.InputTag("muPFIsoDepositPU"),
        deltaR = cms.double(0.3),
        weight = cms.string('1'),
        vetos = cms.vstring('0.01', 
            'Threshold(0.5)'),
        skipDefaultVeto = cms.bool(True),
        mode = cms.string('sumDR')
    ))
)


process.muPFSumDRIsoValuePU04 = cms.EDProducer("CandIsolatorFromDeposits",
    deposits = cms.VPSet(cms.PSet(
        src = cms.InputTag("muPFIsoDepositPU"),
        deltaR = cms.double(0.4),
        weight = cms.string('1'),
        vetos = cms.vstring('0.01', 
            'Threshold(0.5)'),
        skipDefaultVeto = cms.bool(True),
        mode = cms.string('sumDR')
    ))
)


process.muonMatch = cms.EDProducer("MCMatcher",
    src = cms.InputTag("muons"),
    maxDPtRel = cms.double(0.5),
    mcPdgId = cms.vint32(13),
    mcStatus = cms.vint32(1),
    resolveByMatchQuality = cms.bool(False),
    maxDeltaR = cms.double(0.5),
    checkCharge = cms.bool(True),
    resolveAmbiguities = cms.bool(True),
    matched = cms.InputTag("genParticles")
)


process.myGenParticlesForJets = cms.EDProducer("myGenJetsPartSelector",
    src = cms.InputTag("packedGenParticles"),
    ignoreParticleDecayIDs = cms.vint32(),
    ignoreParticleIDs = cms.vuint32(1000022, 1000012, 1000014, 1000016, 2000012, 
        2000014, 2000016, 1000039, 5100039, 4000012, 
        4000014, 4000016, 9900012, 9900014, 9900016, 
        39),
    partonicFinalState = cms.bool(False),
    excludeResonances = cms.bool(True),
    excludeFromResonancePids = cms.vuint32(12, 13, 14, 16),
    invertignoreParticleDecay = cms.bool(False),
    tausAsJets = cms.bool(False)
)


process.myGenParticlesForJetsNoNu = cms.EDProducer("myGenJetsPartSelector",
    src = cms.InputTag("packedGenParticles"),
    ignoreParticleDecayIDs = cms.vint32(),
    ignoreParticleIDs = cms.vuint32(1000022, 1000012, 1000014, 1000016, 2000012, 
        2000014, 2000016, 1000039, 5100039, 4000012, 
        4000014, 4000016, 9900012, 9900014, 9900016, 
        39, 12, 14, 16),
    partonicFinalState = cms.bool(False),
    excludeResonances = cms.bool(True),
    excludeFromResonancePids = cms.vuint32(12, 13, 14, 16),
    invertignoreParticleDecay = cms.bool(False),
    tausAsJets = cms.bool(False)
)


process.myGenParticlesForJetsNoNuNoStopDecays = cms.EDProducer("myGenJetsPartSelector",
    src = cms.InputTag("packedGenParticles"),
    ignoreParticleDecayIDs = cms.vint32(1000006, -1000006),
    ignoreParticleIDs = cms.vuint32(1000022, 1000012, 1000014, 1000016, 2000012, 
        2000014, 2000016, 1000039, 5100039, 4000012, 
        4000014, 4000016, 9900012, 9900014, 9900016, 
        39, 12, 14, 16),
    partonicFinalState = cms.bool(False),
    excludeResonances = cms.bool(True),
    excludeFromResonancePids = cms.vuint32(12, 13, 14, 16),
    invertignoreParticleDecay = cms.bool(False),
    tausAsJets = cms.bool(False)
)


process.myGenParticlesForJetsNoNuOnlyStopDecays = cms.EDProducer("myGenJetsPartSelector",
    src = cms.InputTag("packedGenParticles"),
    ignoreParticleDecayIDs = cms.vint32(1000006, -1000006),
    ignoreParticleIDs = cms.vuint32(1000022, 1000012, 1000014, 1000016, 2000012, 
        2000014, 2000016, 1000039, 5100039, 4000012, 
        4000014, 4000016, 9900012, 9900014, 9900016, 
        39, 12, 14, 16),
    partonicFinalState = cms.bool(False),
    excludeResonances = cms.bool(True),
    excludeFromResonancePids = cms.vuint32(12, 13, 14, 16),
    invertignoreParticleDecay = cms.bool(True),
    tausAsJets = cms.bool(False)
)


process.myak4GenJetsNoNu = cms.EDProducer("FastjetJetProducer",
    Active_Area_Repeats = cms.int32(5),
    doAreaFastjet = cms.bool(False),
    Ghost_EtaMax = cms.double(6.0),
    maxBadHcalCells = cms.uint32(9999999),
    maxRecoveredEcalCells = cms.uint32(9999999),
    jetType = cms.string('GenJet'),
    minSeed = cms.uint32(14327),
    doRhoFastjet = cms.bool(False),
    jetAlgorithm = cms.string('AntiKt'),
    nSigmaPU = cms.double(1.0),
    GhostArea = cms.double(0.01),
    Rho_EtaMax = cms.double(4.5),
    maxBadEcalCells = cms.uint32(9999999),
    useDeterministicSeed = cms.bool(True),
    doPVCorrection = cms.bool(False),
    maxRecoveredHcalCells = cms.uint32(9999999),
    rParam = cms.double(0.4),
    maxProblematicHcalCells = cms.uint32(9999999),
    src = cms.InputTag("myGenParticlesForJetsNoNu"),
    inputEtMin = cms.double(0.0),
    srcPVs = cms.InputTag(""),
    jetPtMin = cms.double(3.0),
    radiusPU = cms.double(0.5),
    maxProblematicEcalCells = cms.uint32(9999999),
    doPUOffsetCorr = cms.bool(False),
    inputEMin = cms.double(0.0)
)


process.myak4GenJetsNoNuNoStopDecays = cms.EDProducer("FastjetJetProducer",
    Active_Area_Repeats = cms.int32(5),
    doAreaFastjet = cms.bool(False),
    Ghost_EtaMax = cms.double(6.0),
    maxBadHcalCells = cms.uint32(9999999),
    maxRecoveredEcalCells = cms.uint32(9999999),
    jetType = cms.string('GenJet'),
    minSeed = cms.uint32(14327),
    doRhoFastjet = cms.bool(False),
    jetAlgorithm = cms.string('AntiKt'),
    nSigmaPU = cms.double(1.0),
    GhostArea = cms.double(0.01),
    Rho_EtaMax = cms.double(4.5),
    maxBadEcalCells = cms.uint32(9999999),
    useDeterministicSeed = cms.bool(True),
    doPVCorrection = cms.bool(False),
    maxRecoveredHcalCells = cms.uint32(9999999),
    rParam = cms.double(0.4),
    maxProblematicHcalCells = cms.uint32(9999999),
    src = cms.InputTag("myGenParticlesForJetsNoNuNoStopDecays"),
    inputEtMin = cms.double(0.0),
    srcPVs = cms.InputTag(""),
    jetPtMin = cms.double(3.0),
    radiusPU = cms.double(0.5),
    maxProblematicEcalCells = cms.uint32(9999999),
    doPUOffsetCorr = cms.bool(False),
    inputEMin = cms.double(0.0)
)


process.myak4GenJetsNoNuOnlyStopDecays = cms.EDProducer("FastjetJetProducer",
    Active_Area_Repeats = cms.int32(5),
    doAreaFastjet = cms.bool(False),
    Ghost_EtaMax = cms.double(6.0),
    maxBadHcalCells = cms.uint32(9999999),
    maxRecoveredEcalCells = cms.uint32(9999999),
    jetType = cms.string('GenJet'),
    minSeed = cms.uint32(14327),
    doRhoFastjet = cms.bool(False),
    jetAlgorithm = cms.string('AntiKt'),
    nSigmaPU = cms.double(1.0),
    GhostArea = cms.double(0.01),
    Rho_EtaMax = cms.double(4.5),
    maxBadEcalCells = cms.uint32(9999999),
    useDeterministicSeed = cms.bool(True),
    doPVCorrection = cms.bool(False),
    maxRecoveredHcalCells = cms.uint32(9999999),
    rParam = cms.double(0.4),
    maxProblematicHcalCells = cms.uint32(9999999),
    src = cms.InputTag("myGenParticlesForJetsNoNuOnlyStopDecays"),
    inputEtMin = cms.double(0.0),
    srcPVs = cms.InputTag(""),
    jetPtMin = cms.double(3.0),
    radiusPU = cms.double(0.5),
    maxProblematicEcalCells = cms.uint32(9999999),
    doPUOffsetCorr = cms.bool(False),
    inputEMin = cms.double(0.0)
)


process.negativeCombinedMVABJetTags = cms.EDProducer("JetTagProducer",
    jetTagComputer = cms.string('negativeCombinedMVA'),
    tagInfos = cms.VInputTag(cms.InputTag("impactParameterTagInfos"), cms.InputTag("secondaryVertexNegativeTagInfos"), cms.InputTag("softPFMuonsTagInfos"), cms.InputTag("softPFElectronsTagInfos"))
)


process.negativeOnlyJetBProbabilityJetTags = cms.EDProducer("JetTagProducer",
    jetTagComputer = cms.string('negativeOnlyJetBProbability'),
    tagInfos = cms.VInputTag(cms.InputTag("impactParameterTagInfos"))
)


process.negativeOnlyJetProbabilityJetTags = cms.EDProducer("JetTagProducer",
    jetTagComputer = cms.string('negativeOnlyJetProbability'),
    tagInfos = cms.VInputTag(cms.InputTag("impactParameterTagInfos"))
)


process.negativeSoftPFElectronBJetTags = cms.EDProducer("JetTagProducer",
    jetTagComputer = cms.string('negativeSoftPFElectron'),
    tagInfos = cms.VInputTag(cms.InputTag("softPFElectronsTagInfos"))
)


process.negativeSoftPFElectronByIP2dBJetTags = cms.EDProducer("JetTagProducer",
    jetTagComputer = cms.string('negativeSoftPFElectronByIP2d'),
    tagInfos = cms.VInputTag(cms.InputTag("softPFElectronsTagInfos"))
)


process.negativeSoftPFElectronByIP3dBJetTags = cms.EDProducer("JetTagProducer",
    jetTagComputer = cms.string('negativeSoftPFElectronByIP3d'),
    tagInfos = cms.VInputTag(cms.InputTag("softPFElectronsTagInfos"))
)


process.negativeSoftPFElectronByPtBJetTags = cms.EDProducer("JetTagProducer",
    jetTagComputer = cms.string('negativeSoftPFElectronByPt'),
    tagInfos = cms.VInputTag(cms.InputTag("softPFElectronsTagInfos"))
)


process.negativeSoftPFMuonBJetTags = cms.EDProducer("JetTagProducer",
    jetTagComputer = cms.string('negativeSoftPFMuon'),
    tagInfos = cms.VInputTag(cms.InputTag("softPFMuonsTagInfos"))
)


process.negativeSoftPFMuonByIP2dBJetTags = cms.EDProducer("JetTagProducer",
    jetTagComputer = cms.string('negativeSoftPFMuonByIP2d'),
    tagInfos = cms.VInputTag(cms.InputTag("softPFMuonsTagInfos"))
)


process.negativeSoftPFMuonByIP3dBJetTags = cms.EDProducer("JetTagProducer",
    jetTagComputer = cms.string('negativeSoftPFMuonByIP3d'),
    tagInfos = cms.VInputTag(cms.InputTag("softPFMuonsTagInfos"))
)


process.negativeSoftPFMuonByPtBJetTags = cms.EDProducer("JetTagProducer",
    jetTagComputer = cms.string('negativeSoftPFMuonByPt'),
    tagInfos = cms.VInputTag(cms.InputTag("softPFMuonsTagInfos"))
)


process.negativeTrackCountingHighEffJetTags = cms.EDProducer("JetTagProducer",
    trackQualityClass = cms.string('any'),
    jetTagComputer = cms.string('negativeTrackCounting3D2nd'),
    tagInfos = cms.VInputTag(cms.InputTag("impactParameterTagInfos"))
)


process.negativeTrackCountingHighPurJetTags = cms.EDProducer("JetTagProducer",
    trackQualityClass = cms.string('any'),
    jetTagComputer = cms.string('negativeTrackCounting3D3rd'),
    tagInfos = cms.VInputTag(cms.InputTag("impactParameterTagInfos"))
)


process.particleFlowPtrs = cms.EDProducer("PFCandidateFwdPtrProducer",
    src = cms.InputTag("particleFlow")
)


process.patElectrons = cms.EDProducer("PATElectronProducer",
    embedPreshowerClusters = cms.bool(True),
    embedHighLevelSelection = cms.bool(True),
    embedRecHits = cms.bool(True),
    embedGsfElectronCore = cms.bool(True),
    electronSource = cms.InputTag("gedGsfElectrons"),
    resolutions = cms.PSet(

    ),
    pfElectronSource = cms.InputTag("particleFlow"),
    userIsolation = cms.PSet(

    ),
    embedPflowBasicClusters = cms.bool(True),
    embedSuperCluster = cms.bool(True),
    embedSeedCluster = cms.bool(True),
    embedPFCandidate = cms.bool(True),
    pfCandidateMap = cms.InputTag("particleFlow","electrons"),
    addElectronID = cms.bool(True),
    efficiencies = cms.PSet(

    ),
    reducedBarrelRecHitCollection = cms.InputTag("reducedEcalRecHitsEB"),
    embedPflowPreshowerClusters = cms.bool(True),
    embedGsfTrack = cms.bool(True),
    usePV = cms.bool(True),
    useParticleFlow = cms.bool(False),
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
        userFunctionLabels = cms.vstring(),
        userFunctions = cms.vstring()
    ),
    embedTrack = cms.bool(True),
    addEfficiencies = cms.bool(False),
    embedPflowSuperCluster = cms.bool(True),
    reducedEndcapRecHitCollection = cms.InputTag("reducedEcalRecHitsEE"),
    pvSrc = cms.InputTag("offlinePrimaryVertices"),
    electronIDSources = cms.PSet(
        eidTight = cms.InputTag("eidTight"),
        eidLoose = cms.InputTag("eidLoose"),
        eidRobustTight = cms.InputTag("eidRobustTight"),
        eidRobustHighEnergy = cms.InputTag("eidRobustHighEnergy"),
        eidRobustLoose = cms.InputTag("eidRobustLoose")
    ),
    genParticleMatch = cms.InputTag("electronMatch"),
    beamLineSrc = cms.InputTag("offlineBeamSpot"),
    addGenMatch = cms.bool(True),
    addResolutions = cms.bool(False),
    isoDeposits = cms.PSet(
        pfNeutralHadrons = cms.InputTag("elPFIsoDepositNeutral"),
        pfChargedAll = cms.InputTag("elPFIsoDepositChargedAll"),
        pfPUChargedHadrons = cms.InputTag("elPFIsoDepositPU"),
        pfPhotons = cms.InputTag("elPFIsoDepositGamma"),
        pfChargedHadrons = cms.InputTag("elPFIsoDepositCharged")
    ),
    embedGenMatch = cms.bool(True),
    embedBasicClusters = cms.bool(True),
    isolationValues = cms.PSet(
        pfNeutralHadrons = cms.InputTag("elPFIsoValueNeutral04PFId"),
        pfChargedAll = cms.InputTag("elPFIsoValueChargedAll04PFId"),
        pfPUChargedHadrons = cms.InputTag("elPFIsoValuePU04PFId"),
        pfPhotons = cms.InputTag("elPFIsoValueGamma04PFId"),
        pfChargedHadrons = cms.InputTag("elPFIsoValueCharged04PFId")
    ),
    isolationValuesNoPFId = cms.PSet(
        pfNeutralHadrons = cms.InputTag("elPFIsoValueNeutral04NoPFId"),
        pfChargedAll = cms.InputTag("elPFIsoValueChargedAll04NoPFId"),
        pfPUChargedHadrons = cms.InputTag("elPFIsoValuePU04NoPFId"),
        pfPhotons = cms.InputTag("elPFIsoValueGamma04NoPFId"),
        pfChargedHadrons = cms.InputTag("elPFIsoValueCharged04NoPFId")
    )
)


process.patJetCharge = cms.EDProducer("JetChargeProducer",
    var = cms.string('Pt'),
    src = cms.InputTag("ak4JetTracksAssociatorAtVertexPF"),
    exp = cms.double(1.0)
)


process.patJetChargeAK4PFCHS = cms.EDProducer("JetChargeProducer",
    var = cms.string('Pt'),
    src = cms.InputTag("jetTracksAssociatorAtVertexAK4PFCHS"),
    exp = cms.double(1.0)
)


process.patJetChargeAK4PFCHSNoMu = cms.EDProducer("JetChargeProducer",
    var = cms.string('Pt'),
    src = cms.InputTag("jetTracksAssociatorAtVertexAK4PFCHSNoMu"),
    exp = cms.double(1.0)
)


process.patJetCorrFactors = cms.EDProducer("JetCorrFactorsProducer",
    src = cms.InputTag("ak4PFJetsCHS"),
    emf = cms.bool(False),
    extraJPTOffset = cms.string('L1FastJet'),
    primaryVertices = cms.InputTag("offlinePrimaryVertices"),
    levels = cms.vstring('L1FastJet', 
        'L2Relative', 
        'L3Absolute'),
    useNPV = cms.bool(True),
    rho = cms.InputTag("fixedGridRhoFastjetAll"),
    useRho = cms.bool(True),
    payload = cms.string('AK4PFchs'),
    flavorType = cms.string('J')
)


process.patJetCorrFactorsAK4PFCHS = cms.EDProducer("JetCorrFactorsProducer",
    src = cms.InputTag("ak4PFJetsCHS"),
    emf = cms.bool(False),
    extraJPTOffset = cms.string('L1FastJet'),
    primaryVertices = cms.InputTag("offlineSlimmedPrimaryVertices"),
    levels = cms.vstring('L1FastJet', 
        'L2Relative', 
        'L3Absolute'),
    useNPV = cms.bool(True),
    rho = cms.InputTag("fixedGridRhoFastjetAll"),
    useRho = cms.bool(True),
    payload = cms.string('AK4PFchs'),
    flavorType = cms.string('J')
)


process.patJetCorrFactorsAK4PFCHSNoMu = cms.EDProducer("JetCorrFactorsProducer",
    src = cms.InputTag("ak4PFJetsCHSNoMu"),
    emf = cms.bool(False),
    extraJPTOffset = cms.string('L1FastJet'),
    primaryVertices = cms.InputTag("offlineSlimmedPrimaryVertices"),
    levels = cms.vstring('L1FastJet', 
        'L2Relative', 
        'L3Absolute'),
    useNPV = cms.bool(True),
    rho = cms.InputTag("fixedGridRhoFastjetAll"),
    useRho = cms.bool(True),
    payload = cms.string('AK4PFchs'),
    flavorType = cms.string('J')
)


process.patJetFlavourAssociation = cms.EDProducer("JetFlavourClustering",
    ghostRescaling = cms.double(1e-18),
    partons = cms.InputTag("patJetPartons","partons"),
    jetAlgorithm = cms.string('AntiKt'),
    bHadrons = cms.InputTag("patJetPartons","bHadrons"),
    rParam = cms.double(0.4),
    jets = cms.InputTag("ak4PFJetsCHS"),
    hadronFlavourHasPriority = cms.bool(True),
    cHadrons = cms.InputTag("patJetPartons","cHadrons")
)


process.patJetFlavourAssociationAK4PFCHS = cms.EDProducer("JetFlavourClustering",
    ghostRescaling = cms.double(1e-18),
    partons = cms.InputTag("patJetPartons","partons"),
    jetAlgorithm = cms.string('AntiKt'),
    bHadrons = cms.InputTag("patJetPartons","bHadrons"),
    rParam = cms.double(0.4),
    jets = cms.InputTag("ak4PFJetsCHS"),
    hadronFlavourHasPriority = cms.bool(True),
    cHadrons = cms.InputTag("patJetPartons","cHadrons")
)


process.patJetFlavourAssociationAK4PFCHSNoMu = cms.EDProducer("JetFlavourClustering",
    ghostRescaling = cms.double(1e-18),
    partons = cms.InputTag("patJetPartons","partons"),
    jetAlgorithm = cms.string('AntiKt'),
    bHadrons = cms.InputTag("patJetPartons","bHadrons"),
    rParam = cms.double(0.4),
    jets = cms.InputTag("ak4PFJetsCHSNoMu"),
    hadronFlavourHasPriority = cms.bool(True),
    cHadrons = cms.InputTag("patJetPartons","cHadrons")
)


process.patJetFlavourAssociationLegacy = cms.EDProducer("JetFlavourIdentifier",
    srcByReference = cms.InputTag("patJetPartonAssociationLegacy"),
    physicsDefinition = cms.bool(False)
)


process.patJetFlavourAssociationLegacyAK4PFCHS = cms.EDProducer("JetFlavourIdentifier",
    srcByReference = cms.InputTag("patJetPartonAssociationLegacyAK4PFCHS"),
    physicsDefinition = cms.bool(False)
)


process.patJetFlavourAssociationLegacyAK4PFCHSNoMu = cms.EDProducer("JetFlavourIdentifier",
    srcByReference = cms.InputTag("patJetPartonAssociationLegacyAK4PFCHSNoMu"),
    physicsDefinition = cms.bool(False)
)


process.patJetGenJetMatch = cms.EDProducer("GenJetMatcher",
    src = cms.InputTag("ak4PFJetsCHS"),
    mcPdgId = cms.vint32(),
    mcStatus = cms.vint32(),
    resolveByMatchQuality = cms.bool(False),
    maxDeltaR = cms.double(0.4),
    checkCharge = cms.bool(False),
    resolveAmbiguities = cms.bool(True),
    matched = cms.InputTag("ak4GenJets")
)


process.patJetGenJetMatchAK4PFCHS = cms.EDProducer("GenJetMatcher",
    src = cms.InputTag("ak4PFJetsCHS"),
    mcPdgId = cms.vint32(),
    mcStatus = cms.vint32(),
    resolveByMatchQuality = cms.bool(False),
    maxDeltaR = cms.double(0.4),
    checkCharge = cms.bool(False),
    resolveAmbiguities = cms.bool(True),
    matched = cms.InputTag("ak4GenJets")
)


process.patJetGenJetMatchAK4PFCHSNoMu = cms.EDProducer("GenJetMatcher",
    src = cms.InputTag("ak4PFJetsCHSNoMu"),
    mcPdgId = cms.vint32(),
    mcStatus = cms.vint32(),
    resolveByMatchQuality = cms.bool(False),
    maxDeltaR = cms.double(0.4),
    checkCharge = cms.bool(False),
    resolveAmbiguities = cms.bool(True),
    matched = cms.InputTag("ak4GenJets")
)


process.patJetPartonAssociationLegacy = cms.EDProducer("JetPartonMatcher",
    jets = cms.InputTag("ak4PFJetsCHS"),
    coneSizeToAssociate = cms.double(0.3),
    partons = cms.InputTag("patJetPartonsLegacy")
)


process.patJetPartonAssociationLegacyAK4PFCHS = cms.EDProducer("JetPartonMatcher",
    jets = cms.InputTag("ak4PFJetsCHS"),
    coneSizeToAssociate = cms.double(0.3),
    partons = cms.InputTag("patJetPartonsLegacy")
)


process.patJetPartonAssociationLegacyAK4PFCHSNoMu = cms.EDProducer("JetPartonMatcher",
    jets = cms.InputTag("ak4PFJetsCHSNoMu"),
    coneSizeToAssociate = cms.double(0.3),
    partons = cms.InputTag("patJetPartonsLegacy")
)


process.patJetPartonMatch = cms.EDProducer("MCMatcher",
    src = cms.InputTag("ak4PFJetsCHS"),
    maxDPtRel = cms.double(3.0),
    mcPdgId = cms.vint32(1, 2, 3, 4, 5, 
        21),
    mcStatus = cms.vint32(3),
    resolveByMatchQuality = cms.bool(False),
    maxDeltaR = cms.double(0.4),
    checkCharge = cms.bool(False),
    resolveAmbiguities = cms.bool(True),
    matched = cms.InputTag("genParticles")
)


process.patJetPartonMatchAK4PFCHS = cms.EDProducer("MCMatcher",
    src = cms.InputTag("ak4PFJetsCHS"),
    maxDPtRel = cms.double(3.0),
    mcPdgId = cms.vint32(1, 2, 3, 4, 5, 
        21),
    mcStatus = cms.vint32(3),
    resolveByMatchQuality = cms.bool(False),
    maxDeltaR = cms.double(0.4),
    checkCharge = cms.bool(False),
    resolveAmbiguities = cms.bool(True),
    matched = cms.InputTag("prunedGenParticles")
)


process.patJetPartonMatchAK4PFCHSNoMu = cms.EDProducer("MCMatcher",
    src = cms.InputTag("ak4PFJetsCHSNoMu"),
    maxDPtRel = cms.double(3.0),
    mcPdgId = cms.vint32(1, 2, 3, 4, 5, 
        21),
    mcStatus = cms.vint32(3),
    resolveByMatchQuality = cms.bool(False),
    maxDeltaR = cms.double(0.4),
    checkCharge = cms.bool(False),
    resolveAmbiguities = cms.bool(True),
    matched = cms.InputTag("prunedGenParticles")
)


process.patJetPartons = cms.EDProducer("HadronAndPartonSelector",
    particles = cms.InputTag("prunedGenParticles"),
    src = cms.InputTag("generator"),
    partonMode = cms.string('Auto'),
    skipFirstN = cms.uint32(0),
    acceptNoDaughters = cms.bool(True)
)


process.patJetPartonsLegacy = cms.EDProducer("PartonSelector",
    src = cms.InputTag("genParticles"),
    withLeptons = cms.bool(False)
)


process.patJets = cms.EDProducer("PATJetProducer",
    addJetCharge = cms.bool(True),
    addGenJetMatch = cms.bool(True),
    embedPFCandidates = cms.bool(False),
    embedGenJetMatch = cms.bool(True),
    addAssociatedTracks = cms.bool(True),
    partonJetSource = cms.InputTag("NOT_IMPLEMENTED"),
    addGenPartonMatch = cms.bool(True),
    JetPartonMapSource = cms.InputTag("patJetFlavourAssociationLegacy"),
    resolutions = cms.PSet(

    ),
    genPartonMatch = cms.InputTag("patJetPartonMatch"),
    addTagInfos = cms.bool(False),
    addPartonJetMatch = cms.bool(False),
    embedGenPartonMatch = cms.bool(True),
    efficiencies = cms.PSet(

    ),
    genJetMatch = cms.InputTag("patJetGenJetMatch"),
    addEfficiencies = cms.bool(False),
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
        userFunctionLabels = cms.vstring(),
        userFunctions = cms.vstring()
    ),
    jetSource = cms.InputTag("ak4PFJetsCHS"),
    useLegacyJetMCFlavour = cms.bool(False),
    discriminatorSources = cms.VInputTag(cms.InputTag("jetBProbabilityBJetTags"), cms.InputTag("jetProbabilityBJetTags"), cms.InputTag("trackCountingHighPurBJetTags"), cms.InputTag("trackCountingHighEffBJetTags"), cms.InputTag("simpleSecondaryVertexHighEffBJetTags"), 
        cms.InputTag("simpleSecondaryVertexHighPurBJetTags"), cms.InputTag("combinedInclusiveSecondaryVertexV2BJetTags"), cms.InputTag("pfCombinedSecondaryVertexBJetTags"), cms.InputTag("combinedMVABJetTags")),
    trackAssociationSource = cms.InputTag("ak4JetTracksAssociatorAtVertexPF"),
    tagInfoSources = cms.VInputTag(),
    jetCorrFactorsSource = cms.VInputTag(cms.InputTag("patJetCorrFactors")),
    addBTagInfo = cms.bool(True),
    addJetFlavourInfo = cms.bool(False),
    addResolutions = cms.bool(False),
    getJetMCFlavour = cms.bool(True),
    addDiscriminators = cms.bool(True),
    jetChargeSource = cms.InputTag("patJetCharge"),
    JetFlavourInfoSource = cms.InputTag("patJetFlavourAssociation"),
    addJetCorrFactors = cms.bool(True),
    jetIDMap = cms.InputTag("ak4JetID"),
    addJetID = cms.bool(False)
)


process.patJetsAK4PFCHS = cms.EDProducer("PATJetProducer",
    addJetCharge = cms.bool(True),
    addGenJetMatch = cms.bool(True),
    embedGenJetMatch = cms.bool(True),
    addAssociatedTracks = cms.bool(True),
    addBTagInfo = cms.bool(True),
    partonJetSource = cms.InputTag("NOT_IMPLEMENTED"),
    addGenPartonMatch = cms.bool(True),
    JetPartonMapSource = cms.InputTag("patJetFlavourAssociationLegacyAK4PFCHS"),
    resolutions = cms.PSet(

    ),
    genPartonMatch = cms.InputTag("patJetPartonMatchAK4PFCHS"),
    addTagInfos = cms.bool(False),
    addPartonJetMatch = cms.bool(False),
    embedGenPartonMatch = cms.bool(True),
    efficiencies = cms.PSet(

    ),
    genJetMatch = cms.InputTag("patJetGenJetMatchAK4PFCHS"),
    useLegacyJetMCFlavour = cms.bool(False),
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
        userFunctionLabels = cms.vstring(),
        userFunctions = cms.vstring()
    ),
    jetSource = cms.InputTag("ak4PFJetsCHS"),
    addEfficiencies = cms.bool(False),
    discriminatorSources = cms.VInputTag(cms.InputTag("combinedInclusiveSecondaryVertexV2BJetTagsAK4PFCHS")),
    trackAssociationSource = cms.InputTag("jetTracksAssociatorAtVertexAK4PFCHS"),
    tagInfoSources = cms.VInputTag(cms.InputTag("impactParameterTagInfosAK4PFCHS"), cms.InputTag("inclusiveSecondaryVertexFinderTagInfosAK4PFCHS")),
    jetCorrFactorsSource = cms.VInputTag(cms.InputTag("patJetCorrFactorsAK4PFCHS")),
    embedPFCandidates = cms.bool(False),
    addJetFlavourInfo = cms.bool(False),
    addResolutions = cms.bool(False),
    getJetMCFlavour = cms.bool(True),
    addDiscriminators = cms.bool(True),
    jetChargeSource = cms.InputTag("patJetChargeAK4PFCHS"),
    JetFlavourInfoSource = cms.InputTag("patJetFlavourAssociationAK4PFCHS"),
    addJetCorrFactors = cms.bool(True),
    jetIDMap = cms.InputTag("ak4JetID"),
    addJetID = cms.bool(False)
)


process.patJetsAK4PFCHSNoMu = cms.EDProducer("PATJetProducer",
    addJetCharge = cms.bool(True),
    addGenJetMatch = cms.bool(True),
    embedGenJetMatch = cms.bool(True),
    addAssociatedTracks = cms.bool(True),
    addBTagInfo = cms.bool(True),
    partonJetSource = cms.InputTag("NOT_IMPLEMENTED"),
    addGenPartonMatch = cms.bool(True),
    JetPartonMapSource = cms.InputTag("patJetFlavourAssociationLegacyAK4PFCHSNoMu"),
    resolutions = cms.PSet(

    ),
    genPartonMatch = cms.InputTag("patJetPartonMatchAK4PFCHSNoMu"),
    addTagInfos = cms.bool(False),
    addPartonJetMatch = cms.bool(False),
    embedGenPartonMatch = cms.bool(True),
    efficiencies = cms.PSet(

    ),
    genJetMatch = cms.InputTag("patJetGenJetMatchAK4PFCHSNoMu"),
    useLegacyJetMCFlavour = cms.bool(False),
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
        userFunctionLabels = cms.vstring(),
        userFunctions = cms.vstring()
    ),
    jetSource = cms.InputTag("ak4PFJetsCHSNoMu"),
    addEfficiencies = cms.bool(False),
    discriminatorSources = cms.VInputTag(cms.InputTag("combinedInclusiveSecondaryVertexV2BJetTagsAK4PFCHSNoMu")),
    trackAssociationSource = cms.InputTag("jetTracksAssociatorAtVertexAK4PFCHSNoMu"),
    tagInfoSources = cms.VInputTag(cms.InputTag("impactParameterTagInfosAK4PFCHSNoMu"), cms.InputTag("inclusiveSecondaryVertexFinderTagInfosAK4PFCHSNoMu")),
    jetCorrFactorsSource = cms.VInputTag(cms.InputTag("patJetCorrFactorsAK4PFCHSNoMu")),
    embedPFCandidates = cms.bool(False),
    addJetFlavourInfo = cms.bool(False),
    addResolutions = cms.bool(False),
    getJetMCFlavour = cms.bool(True),
    addDiscriminators = cms.bool(True),
    jetChargeSource = cms.InputTag("patJetChargeAK4PFCHSNoMu"),
    JetFlavourInfoSource = cms.InputTag("patJetFlavourAssociationAK4PFCHSNoMu"),
    addJetCorrFactors = cms.bool(True),
    jetIDMap = cms.InputTag("ak4JetID"),
    addJetID = cms.bool(False)
)


process.patMETs = cms.EDProducer("PATMETProducer",
    metSource = cms.InputTag("pfMetT1"),
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
        userFunctionLabels = cms.vstring(),
        userFunctions = cms.vstring()
    ),
    addResolutions = cms.bool(False),
    addEfficiencies = cms.bool(False),
    genMETSource = cms.InputTag("genMetTrue"),
    efficiencies = cms.PSet(

    ),
    addGenMET = cms.bool(False),
    addMuonCorrections = cms.bool(False),
    muonSource = cms.InputTag("muons"),
    resolutions = cms.PSet(

    )
)


process.patMETsAK4PFCHS = cms.EDProducer("PATMETProducer",
    metSource = cms.InputTag("AK4PFchsForAK4PFCHSType1p2CorMet"),
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
        userFunctionLabels = cms.vstring(),
        userFunctions = cms.vstring()
    ),
    addResolutions = cms.bool(False),
    muonSource = cms.InputTag("muons"),
    addEfficiencies = cms.bool(False),
    genMETSource = cms.InputTag("genMetTrue"),
    efficiencies = cms.PSet(

    ),
    addGenMET = cms.bool(False),
    addMuonCorrections = cms.bool(False),
    resolutions = cms.PSet(

    )
)


process.patMETsAK4PFCHSNoMu = cms.EDProducer("PATMETProducer",
    metSource = cms.InputTag("AK4PFchsForAK4PFCHSNoMuType1p2CorMet"),
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
        userFunctionLabels = cms.vstring(),
        userFunctions = cms.vstring()
    ),
    addResolutions = cms.bool(False),
    muonSource = cms.InputTag("muons"),
    addEfficiencies = cms.bool(False),
    genMETSource = cms.InputTag("genMetTrue"),
    efficiencies = cms.PSet(

    ),
    addGenMET = cms.bool(False),
    addMuonCorrections = cms.bool(False),
    resolutions = cms.PSet(

    )
)


process.patMuons = cms.EDProducer("PATMuonProducer",
    embedTpfmsMuon = cms.bool(True),
    embedHighLevelSelection = cms.bool(True),
    embedCaloMETMuonCorrs = cms.bool(True),
    caloMETMuonCorrs = cms.InputTag("muonMETValueMapProducer","muCorrData"),
    addResolutions = cms.bool(False),
    resolutions = cms.PSet(

    ),
    embedDytMuon = cms.bool(True),
    userIsolation = cms.PSet(

    ),
    embedPFCandidate = cms.bool(True),
    pfMuonSource = cms.InputTag("particleFlow"),
    efficiencies = cms.PSet(

    ),
    embedStandAloneMuon = cms.bool(True),
    useParticleFlow = cms.bool(False),
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
        userFunctionLabels = cms.vstring(),
        userFunctions = cms.vstring()
    ),
    embedTrack = cms.bool(False),
    addEfficiencies = cms.bool(False),
    usePV = cms.bool(True),
    embedTcMETMuonCorrs = cms.bool(False),
    pvSrc = cms.InputTag("offlinePrimaryVertices"),
    embedTunePMuonBestTrack = cms.bool(True),
    embedMuonBestTrack = cms.bool(True),
    muonSource = cms.InputTag("muons"),
    embedCombinedMuon = cms.bool(True),
    genParticleMatch = cms.InputTag("muonMatch"),
    beamLineSrc = cms.InputTag("offlineBeamSpot"),
    addGenMatch = cms.bool(True),
    forceBestTrackEmbedding = cms.bool(False),
    isoDeposits = cms.PSet(
        pfNeutralHadrons = cms.InputTag("muPFIsoDepositNeutral"),
        pfChargedAll = cms.InputTag("muPFIsoDepositChargedAll"),
        pfPUChargedHadrons = cms.InputTag("muPFIsoDepositPU"),
        pfPhotons = cms.InputTag("muPFIsoDepositGamma"),
        pfChargedHadrons = cms.InputTag("muPFIsoDepositCharged")
    ),
    embedGenMatch = cms.bool(True),
    tcMETMuonCorrs = cms.InputTag("muonTCMETValueMapProducer","muCorrData"),
    embedPickyMuon = cms.bool(True),
    isolationValues = cms.PSet(
        pfNeutralHadrons = cms.InputTag("muPFIsoValueNeutral04"),
        pfChargedAll = cms.InputTag("muPFIsoValueChargedAll04"),
        pfPUChargedHadrons = cms.InputTag("muPFIsoValuePU04"),
        pfPhotons = cms.InputTag("muPFIsoValueGamma04"),
        pfChargedHadrons = cms.InputTag("muPFIsoValueCharged04")
    )
)


process.patPhotons = cms.EDProducer("PATPhotonProducer",
    beamLineSrc = cms.InputTag("offlineBeamSpot"),
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
        userFunctionLabels = cms.vstring(),
        userFunctions = cms.vstring()
    ),
    addGenMatch = cms.bool(True),
    addResolutions = cms.bool(False),
    embedPreshowerClusters = cms.bool(True),
    embedRecHits = cms.bool(True),
    photonIDSources = cms.PSet(
        PhotonCutBasedIDTight = cms.InputTag("PhotonIDProdGED","PhotonCutBasedIDTight"),
        PhotonCutBasedIDLoose = cms.InputTag("PhotonIDProdGED","PhotonCutBasedIDLoose")
    ),
    isoDeposits = cms.PSet(
        pfNeutralHadrons = cms.InputTag("phPFIsoDepositNeutral"),
        pfChargedAll = cms.InputTag("phPFIsoDepositChargedAll"),
        pfPUChargedHadrons = cms.InputTag("phPFIsoDepositPU"),
        pfPhotons = cms.InputTag("phPFIsoDepositGamma"),
        pfChargedHadrons = cms.InputTag("phPFIsoDepositCharged")
    ),
    reducedBarrelRecHitCollection = cms.InputTag("reducedEcalRecHitsEB"),
    photonSource = cms.InputTag("gedPhotons"),
    reducedEndcapRecHitCollection = cms.InputTag("reducedEcalRecHitsEE"),
    efficiencies = cms.PSet(

    ),
    addEfficiencies = cms.bool(False),
    embedGenMatch = cms.bool(True),
    embedSuperCluster = cms.bool(True),
    resolutions = cms.PSet(

    ),
    addPhotonID = cms.bool(True),
    embedBasicClusters = cms.bool(True),
    embedSeedCluster = cms.bool(True),
    userIsolation = cms.PSet(

    ),
    genParticleMatch = cms.InputTag("photonMatch"),
    isolationValues = cms.PSet(
        pfNeutralHadrons = cms.InputTag("phPFIsoValueNeutral04PFId"),
        pfChargedAll = cms.InputTag("phPFIsoValueChargedAll04PFId"),
        pfPUChargedHadrons = cms.InputTag("phPFIsoValuePU04PFId"),
        pfPhotons = cms.InputTag("phPFIsoValueGamma04PFId"),
        pfChargedHadrons = cms.InputTag("phPFIsoValueCharged04PFId")
    )
)


process.patTaus = cms.EDProducer("PATTauProducer",
    tauIDSources = cms.PSet(
        byVLooseIsolationMVA3oldDMwLT = cms.InputTag("hpsPFTauDiscriminationByVLooseIsolationMVA3oldDMwLT"),
        byCombinedIsolationDeltaBetaCorrRaw3Hits = cms.InputTag("hpsPFTauDiscriminationByRawCombinedIsolationDBSumPtCorr3Hits"),
        againstMuonMedium = cms.InputTag("hpsPFTauDiscriminationByMediumMuonRejection"),
        byTightIsolationMVA3newDMwoLT = cms.InputTag("hpsPFTauDiscriminationByTightIsolationMVA3newDMwoLT"),
        againstElectronTightMVA5 = cms.InputTag("hpsPFTauDiscriminationByMVA5TightElectronRejection"),
        byVLooseIsolationMVA3newDMwoLT = cms.InputTag("hpsPFTauDiscriminationByVLooseIsolationMVA3newDMwoLT"),
        againstElectronTight = cms.InputTag("hpsPFTauDiscriminationByTightElectronRejection"),
        neutralIsoPtSum = cms.InputTag("hpsPFTauMVA3IsolationNeutralIsoPtSum"),
        puCorrPtSum = cms.InputTag("hpsPFTauMVA3IsolationPUcorrPtSum"),
        againstMuonTight = cms.InputTag("hpsPFTauDiscriminationByTightMuonRejection"),
        againstMuonMediumMVA = cms.InputTag("hpsPFTauDiscriminationByMVAMediumMuonRejection"),
        byTightIsolationMVA3oldDMwoLT = cms.InputTag("hpsPFTauDiscriminationByTightIsolationMVA3oldDMwoLT"),
        againstMuonLoose3 = cms.InputTag("hpsPFTauDiscriminationByLooseMuonRejection3"),
        againstMuonLoose2 = cms.InputTag("hpsPFTauDiscriminationByLooseMuonRejection2"),
        againstElectronVTightMVA5 = cms.InputTag("hpsPFTauDiscriminationByMVA5VTightElectronRejection"),
        againstElectronLooseMVA5 = cms.InputTag("hpsPFTauDiscriminationByMVA5LooseElectronRejection"),
        byIsolationMVA3newDMwoLTraw = cms.InputTag("hpsPFTauDiscriminationByIsolationMVA3newDMwoLTraw"),
        againstElectronVLooseMVA5 = cms.InputTag("hpsPFTauDiscriminationByMVA5VLooseElectronRejection"),
        byTightIsolationMVA3newDMwLT = cms.InputTag("hpsPFTauDiscriminationByTightIsolationMVA3newDMwLT"),
        decayModeFinding = cms.InputTag("hpsPFTauDiscriminationByDecayModeFinding"),
        againstElectronMediumMVA5 = cms.InputTag("hpsPFTauDiscriminationByMVA5MediumElectronRejection"),
        againstMuonLoose = cms.InputTag("hpsPFTauDiscriminationByLooseMuonRejection"),
        againstMuonTight2 = cms.InputTag("hpsPFTauDiscriminationByTightMuonRejection2"),
        byMediumIsolationMVA3newDMwLT = cms.InputTag("hpsPFTauDiscriminationByMediumIsolationMVA3newDMwLT"),
        againstMuonMVAraw = cms.InputTag("hpsPFTauDiscriminationByMVArawMuonRejection"),
        againstElectronMedium = cms.InputTag("hpsPFTauDiscriminationByMediumElectronRejection"),
        againstElectronMVA5category = cms.InputTag("hpsPFTauDiscriminationByMVA5rawElectronRejection","category"),
        againstElectronLoose = cms.InputTag("hpsPFTauDiscriminationByLooseElectronRejection"),
        byMediumIsolationMVA3oldDMwLT = cms.InputTag("hpsPFTauDiscriminationByMediumIsolationMVA3oldDMwLT"),
        byMediumCombinedIsolationDeltaBetaCorr3Hits = cms.InputTag("hpsPFTauDiscriminationByMediumCombinedIsolationDBSumPtCorr3Hits"),
        byVLooseIsolationMVA3newDMwLT = cms.InputTag("hpsPFTauDiscriminationByVLooseIsolationMVA3newDMwLT"),
        byLooseIsolationMVA3oldDMwoLT = cms.InputTag("hpsPFTauDiscriminationByLooseIsolationMVA3oldDMwoLT"),
        againstMuonLooseMVA = cms.InputTag("hpsPFTauDiscriminationByMVALooseMuonRejection"),
        byVVTightIsolationMVA3newDMwoLT = cms.InputTag("hpsPFTauDiscriminationByVVTightIsolationMVA3newDMwoLT"),
        byVTightIsolationMVA3newDMwLT = cms.InputTag("hpsPFTauDiscriminationByVTightIsolationMVA3newDMwLT"),
        byVLooseIsolationMVA3oldDMwoLT = cms.InputTag("hpsPFTauDiscriminationByVLooseIsolationMVA3oldDMwoLT"),
        byVVTightIsolationMVA3oldDMwoLT = cms.InputTag("hpsPFTauDiscriminationByVVTightIsolationMVA3oldDMwoLT"),
        byIsolationMVA3oldDMwoLTraw = cms.InputTag("hpsPFTauDiscriminationByIsolationMVA3oldDMwoLTraw"),
        chargedIsoPtSum = cms.InputTag("hpsPFTauMVA3IsolationChargedIsoPtSum"),
        byVTightIsolationMVA3oldDMwoLT = cms.InputTag("hpsPFTauDiscriminationByVTightIsolationMVA3oldDMwoLT"),
        byLooseCombinedIsolationDeltaBetaCorr3Hits = cms.InputTag("hpsPFTauDiscriminationByLooseCombinedIsolationDBSumPtCorr3Hits"),
        byLooseIsolationMVA3newDMwoLT = cms.InputTag("hpsPFTauDiscriminationByLooseIsolationMVA3newDMwoLT"),
        byMediumIsolationMVA3newDMwoLT = cms.InputTag("hpsPFTauDiscriminationByMediumIsolationMVA3newDMwoLT"),
        againstMuonMedium2 = cms.InputTag("hpsPFTauDiscriminationByMediumMuonRejection2"),
        byVVTightIsolationMVA3oldDMwLT = cms.InputTag("hpsPFTauDiscriminationByVVTightIsolationMVA3oldDMwLT"),
        byLooseIsolationMVA3newDMwLT = cms.InputTag("hpsPFTauDiscriminationByLooseIsolationMVA3newDMwLT"),
        byVTightIsolationMVA3newDMwoLT = cms.InputTag("hpsPFTauDiscriminationByVTightIsolationMVA3newDMwoLT"),
        byTightIsolationMVA3oldDMwLT = cms.InputTag("hpsPFTauDiscriminationByTightIsolationMVA3oldDMwLT"),
        againstMuonTightMVA = cms.InputTag("hpsPFTauDiscriminationByMVATightMuonRejection"),
        byIsolationMVA3newDMwLTraw = cms.InputTag("hpsPFTauDiscriminationByIsolationMVA3newDMwLTraw"),
        byVVTightIsolationMVA3newDMwLT = cms.InputTag("hpsPFTauDiscriminationByVVTightIsolationMVA3newDMwLT"),
        againstMuonTight3 = cms.InputTag("hpsPFTauDiscriminationByTightMuonRejection3"),
        byVTightIsolationMVA3oldDMwLT = cms.InputTag("hpsPFTauDiscriminationByVTightIsolationMVA3oldDMwLT"),
        byTightCombinedIsolationDeltaBetaCorr3Hits = cms.InputTag("hpsPFTauDiscriminationByTightCombinedIsolationDBSumPtCorr3Hits"),
        byIsolationMVA3oldDMwLTraw = cms.InputTag("hpsPFTauDiscriminationByIsolationMVA3oldDMwLTraw"),
        againstElectronMVA5raw = cms.InputTag("hpsPFTauDiscriminationByMVA5rawElectronRejection"),
        byMediumIsolationMVA3oldDMwoLT = cms.InputTag("hpsPFTauDiscriminationByMediumIsolationMVA3oldDMwoLT"),
        decayModeFindingNewDMs = cms.InputTag("hpsPFTauDiscriminationByDecayModeFindingNewDMs"),
        byLooseIsolationMVA3oldDMwLT = cms.InputTag("hpsPFTauDiscriminationByLooseIsolationMVA3oldDMwLT")
    ),
    addGenJetMatch = cms.bool(True),
    embedGenJetMatch = cms.bool(True),
    embedLeadTrack = cms.bool(False),
    embedLeadPFCand = cms.bool(False),
    embedSignalPFChargedHadrCands = cms.bool(False),
    addTauJetCorrFactors = cms.bool(False),
    resolutions = cms.PSet(

    ),
    userIsolation = cms.PSet(
        pfAllParticles = cms.PSet(
            threshold = cms.double(0.0),
            src = cms.InputTag("tauIsoDepositPFCandidates"),
            deltaR = cms.double(0.5)
        ),
        pfNeutralHadron = cms.PSet(
            threshold = cms.double(0.0),
            src = cms.InputTag("tauIsoDepositPFNeutralHadrons"),
            deltaR = cms.double(0.5)
        ),
        pfChargedHadron = cms.PSet(
            threshold = cms.double(0.0),
            src = cms.InputTag("tauIsoDepositPFChargedHadrons"),
            deltaR = cms.double(0.5)
        ),
        pfGamma = cms.PSet(
            threshold = cms.double(0.0),
            src = cms.InputTag("tauIsoDepositPFGammas"),
            deltaR = cms.double(0.5)
        )
    ),
    embedIsolationPFGammaCands = cms.bool(False),
    embedSignalPFGammaCands = cms.bool(False),
    efficiencies = cms.PSet(

    ),
    genJetMatch = cms.InputTag("tauGenJetMatch"),
    embedIsolationPFCands = cms.bool(False),
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
        userFunctionLabels = cms.vstring(),
        userFunctions = cms.vstring()
    ),
    embedSignalPFCands = cms.bool(False),
    addEfficiencies = cms.bool(False),
    embedSignalTracks = cms.bool(False),
    tauSource = cms.InputTag("hpsPFTauProducer"),
    tauJetCorrFactorsSource = cms.VInputTag(cms.InputTag("patTauJetCorrFactors")),
    embedIsolationPFNeutralHadrCands = cms.bool(False),
    addTauID = cms.bool(True),
    genParticleMatch = cms.InputTag("tauMatch"),
    addGenMatch = cms.bool(True),
    addResolutions = cms.bool(False),
    embedIsolationPFChargedHadrCands = cms.bool(False),
    embedIsolationTracks = cms.bool(False),
    embedSignalPFNeutralHadrCands = cms.bool(False),
    isoDeposits = cms.PSet(
        pfAllParticles = cms.InputTag("tauIsoDepositPFCandidates"),
        pfNeutralHadron = cms.InputTag("tauIsoDepositPFNeutralHadrons"),
        pfChargedHadron = cms.InputTag("tauIsoDepositPFChargedHadrons"),
        pfGamma = cms.InputTag("tauIsoDepositPFGammas")
    ),
    embedLeadPFChargedHadrCand = cms.bool(False),
    tauTransverseImpactParameterSource = cms.InputTag("hpsPFTauTransverseImpactParameters"),
    embedGenMatch = cms.bool(True),
    embedLeadPFNeutralCand = cms.bool(False)
)


process.pfAllElectronsClones = cms.EDProducer("PFCandidateProductFromFwdPtrProducer",
    src = cms.InputTag("pfAllElectrons")
)


process.pfAllMuonsClones = cms.EDProducer("PFCandidateProductFromFwdPtrProducer",
    src = cms.InputTag("pfAllMuons")
)


process.pfCandMETcorr = cms.EDProducer("PFCandMETcorrInputProducer",
    src = cms.InputTag("pfCandsNotInJetsForMetCorr")
)


process.pfCandsNotInJetsForMetCorr = cms.EDProducer("PFCandidateFromFwdPtrProducer",
    src = cms.InputTag("pfCandsNotInJetsPtrForMetCorr")
)


process.pfCandsNotInJetsPtrForMetCorr = cms.EDProducer("TPPFJetsOnPFCandidates",
    bottomCollection = cms.InputTag("particleFlowPtrs"),
    enable = cms.bool(True),
    topCollection = cms.InputTag("pfJetsPtrForMetCorr"),
    name = cms.untracked.string('noJet'),
    verbose = cms.untracked.bool(False)
)


process.pfCombinedSecondaryVertexBJetTags = cms.EDProducer("JetTagProducer",
    jetTagComputer = cms.string('candidateCombinedSecondaryVertex'),
    tagInfos = cms.VInputTag(cms.InputTag("pfImpactParameterTagInfos"), cms.InputTag("pfSecondaryVertexTagInfos"))
)


process.pfImpactParameterTagInfos = cms.EDProducer("CandIPProducer",
    maximumTransverseImpactParameter = cms.double(0.2),
    minimumNumberOfHits = cms.int32(8),
    minimumTransverseMomentum = cms.double(1.0),
    primaryVertex = cms.InputTag("offlinePrimaryVertices"),
    maximumLongitudinalImpactParameter = cms.double(17.0),
    computeProbabilities = cms.bool(True),
    maxDeltaR = cms.double(0.4),
    candidates = cms.InputTag("particleFlow"),
    jetDirectionUsingGhostTrack = cms.bool(False),
    minimumNumberOfPixelHits = cms.int32(2),
    jetDirectionUsingTracks = cms.bool(False),
    computeGhostTrack = cms.bool(True),
    useTrackQuality = cms.bool(False),
    jets = cms.InputTag("ak4PFJetsCHS"),
    ghostTrackPriorDeltaR = cms.double(0.03),
    maximumChiSquared = cms.double(5.0)
)


process.pfJetsPtrForMetCorr = cms.EDProducer("PFJetFwdPtrProducer",
    src = cms.InputTag("ak4PFJets")
)


process.pfMet = cms.EDProducer("PFMETProducer",
    globalThreshold = cms.double(0.0),
    calculateSignificance = cms.bool(False),
    alias = cms.string('pfMet'),
    src = cms.InputTag("packedPFCandidates")
)


process.pfMetT0pc = cms.EDProducer("AddCorrectionsToPFMET",
    src = cms.InputTag("pfMet"),
    srcCorrections = cms.VInputTag(cms.InputTag("corrPfMetType0PfCand"))
)


process.pfMetT0pcT1 = cms.EDProducer("AddCorrectionsToPFMET",
    src = cms.InputTag("pfMet"),
    srcCorrections = cms.VInputTag(cms.InputTag("corrPfMetType0PfCand"), cms.InputTag("corrPfMetType1","type1"))
)


process.pfMetT0pcT1Txy = cms.EDProducer("AddCorrectionsToPFMET",
    src = cms.InputTag("pfMet"),
    srcCorrections = cms.VInputTag(cms.InputTag("corrPfMetType0PfCand"), cms.InputTag("corrPfMetType1","type1"), cms.InputTag("corrPfMetShiftXY"))
)


process.pfMetT0pcTxy = cms.EDProducer("AddCorrectionsToPFMET",
    src = cms.InputTag("pfMet"),
    srcCorrections = cms.VInputTag(cms.InputTag("corrPfMetType0PfCand"), cms.InputTag("corrPfMetShiftXY"))
)


process.pfMetT0rt = cms.EDProducer("AddCorrectionsToPFMET",
    src = cms.InputTag("pfMet"),
    srcCorrections = cms.VInputTag(cms.InputTag("corrPfMetType0RecoTrack"))
)


process.pfMetT0rtT1 = cms.EDProducer("AddCorrectionsToPFMET",
    src = cms.InputTag("pfMet"),
    srcCorrections = cms.VInputTag(cms.InputTag("corrPfMetType0RecoTrack"), cms.InputTag("corrPfMetType1","type1"))
)


process.pfMetT0rtT1T2 = cms.EDProducer("AddCorrectionsToPFMET",
    src = cms.InputTag("pfMet"),
    srcCorrections = cms.VInputTag(cms.InputTag("corrPfMetType0RecoTrackForType2"), cms.InputTag("corrPfMetType1","type1"), cms.InputTag("corrPfMetType2"))
)


process.pfMetT0rtT1T2Txy = cms.EDProducer("AddCorrectionsToPFMET",
    src = cms.InputTag("pfMet"),
    srcCorrections = cms.VInputTag(cms.InputTag("corrPfMetType0RecoTrackForType2"), cms.InputTag("corrPfMetType1","type1"), cms.InputTag("corrPfMetType2"), cms.InputTag("corrPfMetShiftXY"))
)


process.pfMetT0rtT1Txy = cms.EDProducer("AddCorrectionsToPFMET",
    src = cms.InputTag("pfMet"),
    srcCorrections = cms.VInputTag(cms.InputTag("corrPfMetType0RecoTrack"), cms.InputTag("corrPfMetType1","type1"), cms.InputTag("corrPfMetShiftXY"))
)


process.pfMetT0rtT2 = cms.EDProducer("AddCorrectionsToPFMET",
    src = cms.InputTag("pfMet"),
    srcCorrections = cms.VInputTag(cms.InputTag("corrPfMetType0RecoTrackForType2"), cms.InputTag("corrPfMetType2"))
)


process.pfMetT0rtT2Txy = cms.EDProducer("AddCorrectionsToPFMET",
    src = cms.InputTag("pfMet"),
    srcCorrections = cms.VInputTag(cms.InputTag("corrPfMetType0RecoTrackForType2"), cms.InputTag("corrPfMetType2"), cms.InputTag("corrPfMetShiftXY"))
)


process.pfMetT0rtTxy = cms.EDProducer("AddCorrectionsToPFMET",
    src = cms.InputTag("pfMet"),
    srcCorrections = cms.VInputTag(cms.InputTag("corrPfMetType0RecoTrack"), cms.InputTag("corrPfMetShiftXY"))
)


process.pfMetT1 = cms.EDProducer("AddCorrectionsToPFMET",
    src = cms.InputTag("pfMet"),
    srcCorrections = cms.VInputTag(cms.InputTag("corrPfMetType1","type1"))
)


process.pfMetT1T2 = cms.EDProducer("AddCorrectionsToPFMET",
    src = cms.InputTag("pfMet"),
    srcCorrections = cms.VInputTag(cms.InputTag("corrPfMetType1","type1"), cms.InputTag("corrPfMetType2"))
)


process.pfMetT1T2Txy = cms.EDProducer("AddCorrectionsToPFMET",
    src = cms.InputTag("pfMet"),
    srcCorrections = cms.VInputTag(cms.InputTag("corrPfMetType1","type1"), cms.InputTag("corrPfMetType2"), cms.InputTag("corrPfMetShiftXY"))
)


process.pfMetT1Txy = cms.EDProducer("AddCorrectionsToPFMET",
    src = cms.InputTag("pfMet"),
    srcCorrections = cms.VInputTag(cms.InputTag("corrPfMetType1","type1"), cms.InputTag("corrPfMetShiftXY"))
)


process.pfNoElectrons = cms.EDProducer("CandPtrProjector",
    veto = cms.InputTag("selectedElectrons"),
    src = cms.InputTag("pfNoMuon")
)


process.pfNoElectronsCHS = cms.EDProducer("CandPtrProjector",
    veto = cms.InputTag("selectedElectrons"),
    src = cms.InputTag("pfNoMuonCHS")
)


process.pfNoJet = cms.EDProducer("TPPFJetsOnPFCandidates",
    bottomCollection = cms.InputTag("pfNoElectronJME"),
    enable = cms.bool(True),
    topCollection = cms.InputTag("pfJetsPtrs"),
    name = cms.untracked.string('noJet'),
    verbose = cms.untracked.bool(False)
)


process.pfNoMuon = cms.EDProducer("CandPtrProjector",
    veto = cms.InputTag("selectedMuons"),
    src = cms.InputTag("packedPFCandidates")
)


process.pfNoMuonCHS = cms.EDProducer("CandPtrProjector",
    veto = cms.InputTag("selectedMuons"),
    src = cms.InputTag("pfCHS")
)


process.pfNoMuonCHSNoMu = cms.EDProducer("CandPtrProjector",
    veto = cms.InputTag("prodMuons","mu2Clean"),
    src = cms.InputTag("pfCHS")
)


process.pfNoPileUp = cms.EDProducer("TPPFCandidatesOnPFCandidates",
    bottomCollection = cms.InputTag("particleFlowPtrs"),
    enable = cms.bool(True),
    topCollection = cms.InputTag("pfPileUp"),
    name = cms.untracked.string('pileUpOnPFCandidates'),
    verbose = cms.untracked.bool(False)
)


process.pfNoPileUpIso = cms.EDProducer("TPPFCandidatesOnPFCandidates",
    bottomCollection = cms.InputTag("particleFlowPtrs"),
    enable = cms.bool(True),
    topCollection = cms.InputTag("pfPileUpIso"),
    name = cms.untracked.string('pileUpOnPFCandidates'),
    verbose = cms.untracked.bool(False)
)


process.pfNoPileUpJME = cms.EDProducer("TPPFCandidatesOnPFCandidates",
    bottomCollection = cms.InputTag("particleFlowPtrs"),
    enable = cms.bool(True),
    topCollection = cms.InputTag("pfPileUpJME"),
    name = cms.untracked.string('pileUpOnPFCandidates'),
    verbose = cms.untracked.bool(False)
)


process.pfPileUp = cms.EDProducer("PFPileUp",
    PFCandidates = cms.InputTag("particleFlowPtrs"),
    Enable = cms.bool(True),
    checkClosestZVertex = cms.bool(True),
    verbose = cms.untracked.bool(False),
    Vertices = cms.InputTag("offlinePrimaryVertices")
)


process.pfPileUpIso = cms.EDProducer("PFPileUp",
    checkClosestZVertex = cms.bool(True),
    Enable = cms.bool(True),
    PFCandidates = cms.InputTag("particleFlowPtrs"),
    verbose = cms.untracked.bool(False),
    Vertices = cms.InputTag("offlinePrimaryVertices")
)


process.pfPileUpJME = cms.EDProducer("PFPileUp",
    checkClosestZVertex = cms.bool(False),
    Enable = cms.bool(True),
    PFCandidates = cms.InputTag("particleFlowPtrs"),
    verbose = cms.untracked.bool(False),
    Vertices = cms.InputTag("goodOfflinePrimaryVertices")
)


process.pfSecondaryVertexTagInfos = cms.EDProducer("CandSecondaryVertexProducer",
    trackSelection = cms.PSet(
        b_pT = cms.double(0.3684),
        a_dR = cms.double(-0.001053),
        min_pT = cms.double(120),
        max_pT = cms.double(500),
        min_pT_dRcut = cms.double(0.5),
        max_pT_trackPTcut = cms.double(3),
        max_pT_dRcut = cms.double(0.1),
        b_dR = cms.double(0.6263),
        a_pT = cms.double(0.005263),
        totalHitsMin = cms.uint32(8),
        jetDeltaRMax = cms.double(0.3),
        qualityClass = cms.string('any'),
        pixelHitsMin = cms.uint32(2),
        sip3dSigMin = cms.double(-99999.9),
        sip3dSigMax = cms.double(99999.9),
        maxDistToAxis = cms.double(0.2),
        useVariableJTA = cms.bool(False),
        sip2dValMax = cms.double(99999.9),
        maxDecayLen = cms.double(99999.9),
        ptMin = cms.double(1.0),
        sip2dSigMax = cms.double(99999.9),
        sip2dSigMin = cms.double(-99999.9),
        sip3dValMax = cms.double(99999.9),
        sip3dValMin = cms.double(-99999.9),
        sip2dValMin = cms.double(-99999.9),
        normChi2Max = cms.double(99999.9)
    ),
    vertexSelection = cms.PSet(
        sortCriterium = cms.string('dist3dError')
    ),
    vertexCuts = cms.PSet(
        distSig3dMax = cms.double(99999.9),
        fracPV = cms.double(0.65),
        distVal2dMax = cms.double(2.5),
        useTrackWeights = cms.bool(True),
        maxDeltaRToJetAxis = cms.double(0.5),
        v0Filter = cms.PSet(
            k0sMassWindow = cms.double(0.05)
        ),
        distSig2dMin = cms.double(3.0),
        multiplicityMin = cms.uint32(2),
        massMax = cms.double(6.5),
        distSig2dMax = cms.double(99999.9),
        distVal3dMax = cms.double(99999.9),
        minimumTrackWeight = cms.double(0.5),
        distVal3dMin = cms.double(-99999.9),
        distVal2dMin = cms.double(0.01),
        distSig3dMin = cms.double(-99999.9)
    ),
    vertexReco = cms.PSet(
        seccut = cms.double(6.0),
        primcut = cms.double(1.8),
        smoothing = cms.bool(False),
        weightthreshold = cms.double(0.001),
        minweight = cms.double(0.5),
        finder = cms.string('avr')
    ),
    extSVDeltaRToJet = cms.double(0.3),
    beamSpotTag = cms.InputTag("offlineBeamSpot"),
    constraint = cms.string('BeamSpot'),
    useExternalSV = cms.bool(False),
    trackIPTagInfos = cms.InputTag("pfImpactParameterTagInfos"),
    minimumTrackWeight = cms.double(0.5),
    usePVError = cms.bool(True),
    trackSort = cms.string('sip3dSig'),
    extSVCollection = cms.InputTag("secondaryVertices")
)


process.phPFIsoDepositCharged = cms.EDProducer("CandIsoDepositProducer",
    src = cms.InputTag("gedPhotons"),
    MultipleDepositsFlag = cms.bool(False),
    trackType = cms.string('candidate'),
    ExtractorPSet = cms.PSet(
        Diff_z = cms.double(99999.99),
        ComponentName = cms.string('CandViewExtractor'),
        DR_Max = cms.double(1.0),
        Diff_r = cms.double(99999.99),
        inputCandView = cms.InputTag("pfAllChargedHadrons"),
        DR_Veto = cms.double(0),
        DepositLabel = cms.untracked.string('')
    )
)


process.phPFIsoDepositChargedAll = cms.EDProducer("CandIsoDepositProducer",
    src = cms.InputTag("gedPhotons"),
    MultipleDepositsFlag = cms.bool(False),
    trackType = cms.string('candidate'),
    ExtractorPSet = cms.PSet(
        Diff_z = cms.double(99999.99),
        ComponentName = cms.string('CandViewExtractor'),
        DR_Max = cms.double(1.0),
        Diff_r = cms.double(99999.99),
        inputCandView = cms.InputTag("pfAllChargedParticles"),
        DR_Veto = cms.double(0),
        DepositLabel = cms.untracked.string('')
    )
)


process.phPFIsoDepositGamma = cms.EDProducer("CandIsoDepositProducer",
    src = cms.InputTag("gedPhotons"),
    MultipleDepositsFlag = cms.bool(False),
    trackType = cms.string('candidate'),
    ExtractorPSet = cms.PSet(
        Diff_z = cms.double(99999.99),
        SCMatch_Veto = cms.bool(True),
        ComponentName = cms.string('PFCandWithSuperClusterExtractor'),
        DR_Max = cms.double(1.0),
        Diff_r = cms.double(99999.99),
        inputCandView = cms.InputTag("pfAllPhotons"),
        MissHitSCMatch_Veto = cms.bool(False),
        DR_Veto = cms.double(0),
        DepositLabel = cms.untracked.string('')
    )
)


process.phPFIsoDepositNeutral = cms.EDProducer("CandIsoDepositProducer",
    src = cms.InputTag("gedPhotons"),
    MultipleDepositsFlag = cms.bool(False),
    trackType = cms.string('candidate'),
    ExtractorPSet = cms.PSet(
        Diff_z = cms.double(99999.99),
        ComponentName = cms.string('CandViewExtractor'),
        DR_Max = cms.double(1.0),
        Diff_r = cms.double(99999.99),
        inputCandView = cms.InputTag("pfAllNeutralHadrons"),
        DR_Veto = cms.double(0),
        DepositLabel = cms.untracked.string('')
    )
)


process.phPFIsoDepositPU = cms.EDProducer("CandIsoDepositProducer",
    src = cms.InputTag("gedPhotons"),
    MultipleDepositsFlag = cms.bool(False),
    trackType = cms.string('candidate'),
    ExtractorPSet = cms.PSet(
        Diff_z = cms.double(99999.99),
        ComponentName = cms.string('CandViewExtractor'),
        DR_Max = cms.double(1.0),
        Diff_r = cms.double(99999.99),
        inputCandView = cms.InputTag("pfPileUpAllChargedParticles"),
        DR_Veto = cms.double(0),
        DepositLabel = cms.untracked.string('')
    )
)


process.phPFIsoValueCharged03PFId = cms.EDProducer("PFCandIsolatorFromDeposits",
    deposits = cms.VPSet(cms.PSet(
        src = cms.InputTag("phPFIsoDepositCharged"),
        deltaR = cms.double(0.3),
        PivotCoordinatesForEBEE = cms.bool(True),
        weight = cms.string('1'),
        vetos = cms.vstring(),
        skipDefaultVeto = cms.bool(True),
        mode = cms.string('sum')
    ))
)


process.phPFIsoValueCharged04PFId = cms.EDProducer("PFCandIsolatorFromDeposits",
    deposits = cms.VPSet(cms.PSet(
        src = cms.InputTag("phPFIsoDepositCharged"),
        deltaR = cms.double(0.4),
        PivotCoordinatesForEBEE = cms.bool(True),
        weight = cms.string('1'),
        vetos = cms.vstring(),
        skipDefaultVeto = cms.bool(True),
        mode = cms.string('sum')
    ))
)


process.phPFIsoValueChargedAll03PFId = cms.EDProducer("PFCandIsolatorFromDeposits",
    deposits = cms.VPSet(cms.PSet(
        src = cms.InputTag("phPFIsoDepositChargedAll"),
        deltaR = cms.double(0.3),
        PivotCoordinatesForEBEE = cms.bool(True),
        weight = cms.string('1'),
        vetos = cms.vstring(),
        skipDefaultVeto = cms.bool(True),
        mode = cms.string('sum')
    ))
)


process.phPFIsoValueChargedAll04PFId = cms.EDProducer("PFCandIsolatorFromDeposits",
    deposits = cms.VPSet(cms.PSet(
        src = cms.InputTag("phPFIsoDepositChargedAll"),
        deltaR = cms.double(0.4),
        PivotCoordinatesForEBEE = cms.bool(True),
        weight = cms.string('1'),
        vetos = cms.vstring(),
        skipDefaultVeto = cms.bool(True),
        mode = cms.string('sum')
    ))
)


process.phPFIsoValueGamma03PFId = cms.EDProducer("PFCandIsolatorFromDeposits",
    deposits = cms.VPSet(cms.PSet(
        src = cms.InputTag("phPFIsoDepositGamma"),
        deltaR = cms.double(0.3),
        PivotCoordinatesForEBEE = cms.bool(True),
        weight = cms.string('1'),
        vetos = cms.vstring('EcalEndcaps:ConeVeto(0.05)'),
        skipDefaultVeto = cms.bool(True),
        mode = cms.string('sum')
    ))
)


process.phPFIsoValueGamma04PFId = cms.EDProducer("PFCandIsolatorFromDeposits",
    deposits = cms.VPSet(cms.PSet(
        src = cms.InputTag("phPFIsoDepositGamma"),
        deltaR = cms.double(0.4),
        PivotCoordinatesForEBEE = cms.bool(True),
        weight = cms.string('1'),
        vetos = cms.vstring('EcalEndcaps:ConeVeto(0.05)'),
        skipDefaultVeto = cms.bool(True),
        mode = cms.string('sum')
    ))
)


process.phPFIsoValueNeutral03PFId = cms.EDProducer("PFCandIsolatorFromDeposits",
    deposits = cms.VPSet(cms.PSet(
        src = cms.InputTag("phPFIsoDepositNeutral"),
        deltaR = cms.double(0.3),
        PivotCoordinatesForEBEE = cms.bool(True),
        weight = cms.string('1'),
        vetos = cms.vstring(),
        skipDefaultVeto = cms.bool(True),
        mode = cms.string('sum')
    ))
)


process.phPFIsoValueNeutral04PFId = cms.EDProducer("PFCandIsolatorFromDeposits",
    deposits = cms.VPSet(cms.PSet(
        src = cms.InputTag("phPFIsoDepositNeutral"),
        deltaR = cms.double(0.4),
        PivotCoordinatesForEBEE = cms.bool(True),
        weight = cms.string('1'),
        vetos = cms.vstring(),
        skipDefaultVeto = cms.bool(True),
        mode = cms.string('sum')
    ))
)


process.phPFIsoValuePU03PFId = cms.EDProducer("PFCandIsolatorFromDeposits",
    deposits = cms.VPSet(cms.PSet(
        src = cms.InputTag("phPFIsoDepositPU"),
        deltaR = cms.double(0.3),
        PivotCoordinatesForEBEE = cms.bool(True),
        weight = cms.string('1'),
        vetos = cms.vstring(),
        skipDefaultVeto = cms.bool(True),
        mode = cms.string('sum')
    ))
)


process.phPFIsoValuePU04PFId = cms.EDProducer("PFCandIsolatorFromDeposits",
    deposits = cms.VPSet(cms.PSet(
        src = cms.InputTag("phPFIsoDepositPU"),
        deltaR = cms.double(0.4),
        PivotCoordinatesForEBEE = cms.bool(True),
        weight = cms.string('1'),
        vetos = cms.vstring(),
        skipDefaultVeto = cms.bool(True),
        mode = cms.string('sum')
    ))
)


process.photonMatch = cms.EDProducer("MCMatcher",
    src = cms.InputTag("gedPhotons"),
    maxDPtRel = cms.double(1.0),
    mcPdgId = cms.vint32(22),
    mcStatus = cms.vint32(1),
    resolveByMatchQuality = cms.bool(False),
    maxDeltaR = cms.double(0.2),
    checkCharge = cms.bool(True),
    resolveAmbiguities = cms.bool(True),
    matched = cms.InputTag("genParticles")
)


process.positiveCombinedMVABJetTags = cms.EDProducer("JetTagProducer",
    jetTagComputer = cms.string('positiveCombinedMVA'),
    tagInfos = cms.VInputTag(cms.InputTag("impactParameterTagInfos"), cms.InputTag("secondaryVertexTagInfos"), cms.InputTag("softPFMuonsTagInfos"), cms.InputTag("softPFElectronsTagInfos"))
)


process.positiveOnlyJetBProbabilityJetTags = cms.EDProducer("JetTagProducer",
    jetTagComputer = cms.string('positiveOnlyJetBProbability'),
    tagInfos = cms.VInputTag(cms.InputTag("impactParameterTagInfos"))
)


process.positiveOnlyJetProbabilityJetTags = cms.EDProducer("JetTagProducer",
    jetTagComputer = cms.string('positiveOnlyJetProbability'),
    tagInfos = cms.VInputTag(cms.InputTag("impactParameterTagInfos"))
)


process.positiveSoftPFElectronBJetTags = cms.EDProducer("JetTagProducer",
    jetTagComputer = cms.string('positiveSoftPFElectron'),
    tagInfos = cms.VInputTag(cms.InputTag("softPFElectronsTagInfos"))
)


process.positiveSoftPFElectronByIP2dBJetTags = cms.EDProducer("JetTagProducer",
    jetTagComputer = cms.string('positiveSoftPFElectronByIP2d'),
    tagInfos = cms.VInputTag(cms.InputTag("softPFElectronsTagInfos"))
)


process.positiveSoftPFElectronByIP3dBJetTags = cms.EDProducer("JetTagProducer",
    jetTagComputer = cms.string('positiveSoftPFElectronByIP3d'),
    tagInfos = cms.VInputTag(cms.InputTag("softPFElectronsTagInfos"))
)


process.positiveSoftPFElectronByPtBJetTags = cms.EDProducer("JetTagProducer",
    jetTagComputer = cms.string('positiveSoftPFElectronByPt'),
    tagInfos = cms.VInputTag(cms.InputTag("softPFElectronsTagInfos"))
)


process.positiveSoftPFMuonBJetTags = cms.EDProducer("JetTagProducer",
    jetTagComputer = cms.string('positiveSoftPFMuon'),
    tagInfos = cms.VInputTag(cms.InputTag("softPFMuonsTagInfos"))
)


process.positiveSoftPFMuonByIP2dBJetTags = cms.EDProducer("JetTagProducer",
    jetTagComputer = cms.string('positiveSoftPFMuonByIP2d'),
    tagInfos = cms.VInputTag(cms.InputTag("softPFMuonsTagInfos"))
)


process.positiveSoftPFMuonByIP3dBJetTags = cms.EDProducer("JetTagProducer",
    jetTagComputer = cms.string('positiveSoftPFMuonByIP3d'),
    tagInfos = cms.VInputTag(cms.InputTag("softPFMuonsTagInfos"))
)


process.positiveSoftPFMuonByPtBJetTags = cms.EDProducer("JetTagProducer",
    jetTagComputer = cms.string('positiveSoftPFMuonByPt'),
    tagInfos = cms.VInputTag(cms.InputTag("softPFMuonsTagInfos"))
)


process.postStdCleaningCounter = cms.EDProducer("EventCountProducer")


process.prefilterCounter = cms.EDProducer("EventCountProducer")


process.secondaryVertexNegativeTagInfos = cms.EDProducer("SecondaryVertexProducer",
    trackSelection = cms.PSet(
        b_pT = cms.double(0.3684),
        a_dR = cms.double(-0.001053),
        min_pT = cms.double(120),
        max_pT = cms.double(500),
        min_pT_dRcut = cms.double(0.5),
        max_pT_trackPTcut = cms.double(3),
        max_pT_dRcut = cms.double(0.1),
        b_dR = cms.double(0.6263),
        a_pT = cms.double(0.005263),
        totalHitsMin = cms.uint32(8),
        jetDeltaRMax = cms.double(0.3),
        qualityClass = cms.string('any'),
        pixelHitsMin = cms.uint32(2),
        sip3dSigMin = cms.double(-99999.9),
        sip3dSigMax = cms.double(99999.9),
        maxDistToAxis = cms.double(0.2),
        useVariableJTA = cms.bool(False),
        sip2dValMax = cms.double(99999.9),
        maxDecayLen = cms.double(99999.9),
        ptMin = cms.double(1.0),
        sip2dSigMax = cms.double(99999.9),
        sip2dSigMin = cms.double(-99999.9),
        sip3dValMax = cms.double(99999.9),
        sip3dValMin = cms.double(-99999.9),
        sip2dValMin = cms.double(-99999.9),
        normChi2Max = cms.double(99999.9)
    ),
    vertexSelection = cms.PSet(
        sortCriterium = cms.string('dist3dError')
    ),
    vertexCuts = cms.PSet(
        distSig3dMax = cms.double(99999.9),
        fracPV = cms.double(0.65),
        distVal2dMax = cms.double(-0.01),
        useTrackWeights = cms.bool(True),
        maxDeltaRToJetAxis = cms.double(-0.5),
        v0Filter = cms.PSet(
            k0sMassWindow = cms.double(0.05)
        ),
        distSig2dMin = cms.double(-99999.9),
        multiplicityMin = cms.uint32(2),
        massMax = cms.double(6.5),
        distSig2dMax = cms.double(-3.0),
        distVal3dMax = cms.double(99999.9),
        minimumTrackWeight = cms.double(0.5),
        distVal3dMin = cms.double(-99999.9),
        distVal2dMin = cms.double(-2.5),
        distSig3dMin = cms.double(-99999.9)
    ),
    vertexReco = cms.PSet(
        seccut = cms.double(6.0),
        primcut = cms.double(1.8),
        smoothing = cms.bool(False),
        weightthreshold = cms.double(0.001),
        minweight = cms.double(0.5),
        finder = cms.string('avr')
    ),
    extSVDeltaRToJet = cms.double(0.3),
    beamSpotTag = cms.InputTag("offlineBeamSpot"),
    constraint = cms.string('BeamSpot'),
    useExternalSV = cms.bool(False),
    trackIPTagInfos = cms.InputTag("impactParameterTagInfos"),
    minimumTrackWeight = cms.double(0.5),
    usePVError = cms.bool(True),
    trackSort = cms.string('sip3dSig'),
    extSVCollection = cms.InputTag("secondaryVertices")
)


process.secondaryVertexTagInfos = cms.EDProducer("SecondaryVertexProducer",
    trackSelection = cms.PSet(
        b_pT = cms.double(0.3684),
        a_dR = cms.double(-0.001053),
        min_pT = cms.double(120),
        max_pT = cms.double(500),
        min_pT_dRcut = cms.double(0.5),
        max_pT_trackPTcut = cms.double(3),
        max_pT_dRcut = cms.double(0.1),
        b_dR = cms.double(0.6263),
        a_pT = cms.double(0.005263),
        totalHitsMin = cms.uint32(8),
        jetDeltaRMax = cms.double(0.3),
        qualityClass = cms.string('any'),
        pixelHitsMin = cms.uint32(2),
        sip3dSigMin = cms.double(-99999.9),
        sip3dSigMax = cms.double(99999.9),
        maxDistToAxis = cms.double(0.2),
        useVariableJTA = cms.bool(False),
        sip2dValMax = cms.double(99999.9),
        maxDecayLen = cms.double(99999.9),
        ptMin = cms.double(1.0),
        sip2dSigMax = cms.double(99999.9),
        sip2dSigMin = cms.double(-99999.9),
        sip3dValMax = cms.double(99999.9),
        sip3dValMin = cms.double(-99999.9),
        sip2dValMin = cms.double(-99999.9),
        normChi2Max = cms.double(99999.9)
    ),
    vertexSelection = cms.PSet(
        sortCriterium = cms.string('dist3dError')
    ),
    vertexCuts = cms.PSet(
        distSig3dMax = cms.double(99999.9),
        fracPV = cms.double(0.65),
        distVal2dMax = cms.double(2.5),
        useTrackWeights = cms.bool(True),
        maxDeltaRToJetAxis = cms.double(0.5),
        v0Filter = cms.PSet(
            k0sMassWindow = cms.double(0.05)
        ),
        distSig2dMin = cms.double(3.0),
        multiplicityMin = cms.uint32(2),
        massMax = cms.double(6.5),
        distSig2dMax = cms.double(99999.9),
        distVal3dMax = cms.double(99999.9),
        minimumTrackWeight = cms.double(0.5),
        distVal3dMin = cms.double(-99999.9),
        distVal2dMin = cms.double(0.01),
        distSig3dMin = cms.double(-99999.9)
    ),
    vertexReco = cms.PSet(
        seccut = cms.double(6.0),
        primcut = cms.double(1.8),
        smoothing = cms.bool(False),
        weightthreshold = cms.double(0.001),
        minweight = cms.double(0.5),
        finder = cms.string('avr')
    ),
    extSVDeltaRToJet = cms.double(0.3),
    beamSpotTag = cms.InputTag("offlineBeamSpot"),
    constraint = cms.string('BeamSpot'),
    useExternalSV = cms.bool(False),
    trackIPTagInfos = cms.InputTag("impactParameterTagInfos"),
    minimumTrackWeight = cms.double(0.5),
    usePVError = cms.bool(True),
    trackSort = cms.string('sip3dSig'),
    extSVCollection = cms.InputTag("secondaryVertices")
)


process.simpleInclusiveSecondaryVertexHighEffBJetTags = cms.EDProducer("JetTagProducer",
    jetTagComputer = cms.string('simpleSecondaryVertex2Trk'),
    tagInfos = cms.VInputTag(cms.InputTag("inclusiveSecondaryVertexFinderFilteredTagInfos"))
)


process.simpleInclusiveSecondaryVertexHighPurBJetTags = cms.EDProducer("JetTagProducer",
    jetTagComputer = cms.string('simpleSecondaryVertex3Trk'),
    tagInfos = cms.VInputTag(cms.InputTag("inclusiveSecondaryVertexFinderFilteredTagInfos"))
)


process.simpleSecondaryVertexBJetTags = cms.EDProducer("JetTagProducer",
    jetTagComputer = cms.string('simpleSecondaryVertex2Trk'),
    tagInfos = cms.VInputTag(cms.InputTag("secondaryVertexTagInfos"))
)


process.simpleSecondaryVertexHighEffBJetTags = cms.EDProducer("JetTagProducer",
    jetTagComputer = cms.string('simpleSecondaryVertex2Trk'),
    tagInfos = cms.VInputTag(cms.InputTag("secondaryVertexTagInfos"))
)


process.simpleSecondaryVertexHighPurBJetTags = cms.EDProducer("JetTagProducer",
    jetTagComputer = cms.string('simpleSecondaryVertex3Trk'),
    tagInfos = cms.VInputTag(cms.InputTag("secondaryVertexTagInfos"))
)


process.simpleSecondaryVertexNegativeHighEffBJetTags = cms.EDProducer("JetTagProducer",
    jetTagComputer = cms.string('simpleSecondaryVertex2Trk'),
    tagInfos = cms.VInputTag(cms.InputTag("secondaryVertexNegativeTagInfos"))
)


process.simpleSecondaryVertexNegativeHighPurBJetTags = cms.EDProducer("JetTagProducer",
    jetTagComputer = cms.string('simpleSecondaryVertex3Trk'),
    tagInfos = cms.VInputTag(cms.InputTag("secondaryVertexNegativeTagInfos"))
)


process.softMuonTagInfos = cms.EDProducer("SoftLepton",
    muonSelection = cms.uint32(1),
    leptons = cms.InputTag("muons"),
    primaryVertex = cms.InputTag("offlinePrimaryVertices"),
    leptonCands = cms.InputTag(""),
    leptonId = cms.InputTag(""),
    refineJetAxis = cms.uint32(0),
    jets = cms.InputTag("ak4PFJetsCHS"),
    leptonDeltaRCut = cms.double(0.4),
    leptonChi2Cut = cms.double(9999.0)
)


process.softPFElectronBJetTags = cms.EDProducer("JetTagProducer",
    jetTagComputer = cms.string('softPFElectron'),
    tagInfos = cms.VInputTag(cms.InputTag("softPFElectronsTagInfos"))
)


process.softPFElectronByIP2dBJetTags = cms.EDProducer("JetTagProducer",
    jetTagComputer = cms.string('softPFElectronByIP2d'),
    tagInfos = cms.VInputTag(cms.InputTag("softPFElectronsTagInfos"))
)


process.softPFElectronByIP3dBJetTags = cms.EDProducer("JetTagProducer",
    jetTagComputer = cms.string('softPFElectronByIP3d'),
    tagInfos = cms.VInputTag(cms.InputTag("softPFElectronsTagInfos"))
)


process.softPFElectronByPtBJetTags = cms.EDProducer("JetTagProducer",
    jetTagComputer = cms.string('softPFElectronByPt'),
    tagInfos = cms.VInputTag(cms.InputTag("softPFElectronsTagInfos"))
)


process.softPFElectronsTagInfos = cms.EDProducer("SoftPFElectronTagInfoProducer",
    jets = cms.InputTag("ak4PFJetsCHS"),
    primaryVertex = cms.InputTag("offlinePrimaryVertices")
)


process.softPFMuonBJetTags = cms.EDProducer("JetTagProducer",
    jetTagComputer = cms.string('softPFMuon'),
    tagInfos = cms.VInputTag(cms.InputTag("softPFMuonsTagInfos"))
)


process.softPFMuonByIP2dBJetTags = cms.EDProducer("JetTagProducer",
    jetTagComputer = cms.string('softPFMuonByIP2d'),
    tagInfos = cms.VInputTag(cms.InputTag("softPFMuonsTagInfos"))
)


process.softPFMuonByIP3dBJetTags = cms.EDProducer("JetTagProducer",
    jetTagComputer = cms.string('softPFMuonByIP3d'),
    tagInfos = cms.VInputTag(cms.InputTag("softPFMuonsTagInfos"))
)


process.softPFMuonByPtBJetTags = cms.EDProducer("JetTagProducer",
    jetTagComputer = cms.string('softPFMuonByPt'),
    tagInfos = cms.VInputTag(cms.InputTag("softPFMuonsTagInfos"))
)


process.softPFMuonsTagInfos = cms.EDProducer("SoftPFMuonTagInfoProducer",
    MuonId = cms.int32(0),
    jets = cms.InputTag("ak4PFJetsCHS"),
    primaryVertex = cms.InputTag("offlinePrimaryVertices")
)


process.stopTreeMaker = cms.EDProducer("stopTreeMaker",
    varsTLorentzVector = cms.VInputTag(),
    vectorBoolNamesInTree = cms.vstring(),
    TreeName = cms.string('AUX'),
    varsStringNamesInTree = cms.vstring(),
    vectorTLorentzVectorNamesInTree = cms.vstring('prodIsoTrks:looseisoTrksLVec|loose_isoTrksLVec'),
    varsDouble = cms.VInputTag(cms.InputTag("ak4stopmhtPFchs","mht"), cms.InputTag("ak4stopmhtPFchs","mhtphi"), cms.InputTag("ak4stophtPFchs"), cms.InputTag("prodMET","met"), cms.InputTag("prodMET","metphi"), 
        cms.InputTag("ak4jetMHTDPhiForSkimsStop","dPhi0"), cms.InputTag("ak4jetMHTDPhiForSkimsStop","dPhi1"), cms.InputTag("ak4jetMHTDPhiForSkimsStop","dPhi2"), cms.InputTag("prodEventInfo","trunpv"), cms.InputTag("prodEventInfo","avgnpv"), 
        cms.InputTag("type3topTagger","bestTopJetMass"), cms.InputTag("type3topTagger","MT2"), cms.InputTag("type3topTagger","mTbestTopJet"), cms.InputTag("type3topTagger","mTbJet"), cms.InputTag("type3topTagger","linearCombmTbJetPlusmTbestTopJet"), 
        cms.InputTag("type3topTagger","mTbestWJet"), cms.InputTag("type3topTagger","mTbestbJet"), cms.InputTag("type3topTagger","mTremainingTopJet"), cms.InputTag("weightProducer","weight")),
    vectorDoubleNamesInTree = cms.vstring('prodJets:recoJetsBtag|recoJetsBtag_0', 
        'prodIsoTrks:trksForIsoVetocharge|trksForIsoVeto_charge', 
        'prodIsoTrks:trksForIsoVetodz|trksForIsoVeto_dz', 
        'prodIsoTrks:looseisoTrkscharge|loose_isoTrks_charge', 
        'prodIsoTrks:looseisoTrksdz|loose_isoTrks_dz', 
        'prodIsoTrks:looseisoTrksiso|loose_isoTrks_iso', 
        'prodIsoTrks:looseisoTrksmtw|loose_isoTrks_mtw'),
    varsBool = cms.VInputTag(cms.InputTag("type3topTagger","remainPassCSVS")),
    varsString = cms.VInputTag(),
    varsBoolNamesInTree = cms.vstring(),
    vectorInt = cms.VInputTag(cms.InputTag("prodJets","recoJetsFlavor"), cms.InputTag("prodJets","muMatchedJetIdx"), cms.InputTag("prodJets","eleMatchedJetIdx"), cms.InputTag("prodGenInfo","genDecayIdxVec"), cms.InputTag("prodGenInfo","genDecayPdgIdVec"), 
        cms.InputTag("prodGenInfo","genDecayMomIdxVec"), cms.InputTag("prodGenInfo","genDecayMomRefVec"), cms.InputTag("prodGenInfo","WemuVec"), cms.InputTag("prodGenInfo","WtauVec"), cms.InputTag("prodGenInfo","WtauemuVec"), 
        cms.InputTag("prodGenInfo","WtauprongsVec"), cms.InputTag("prodGenInfo","WtaunuVec"), cms.InputTag("prodIsoTrks","trksForIsoVetopdgId"), cms.InputTag("prodIsoTrks","trksForIsoVetoidx"), cms.InputTag("prodIsoTrks","looseisoTrkspdgId"), 
        cms.InputTag("prodIsoTrks","looseisoTrksidx"), cms.InputTag("prodIsoTrks","forVetoIsoTrksidx")),
    vectorString = cms.VInputTag(cms.InputTag("prodGenInfo","genDecayStrVec")),
    vectorTLorentzVector = cms.VInputTag(cms.InputTag("prodMuonsNoIso","muonsLVec"), cms.InputTag("prodElectronsNoIso","elesLVec"), cms.InputTag("prodJets","jetsLVec"), cms.InputTag("prodJetsNoMu","jetsLVec"), cms.InputTag("prodJets","lowptjetsLVec"), 
        cms.InputTag("prodGenInfo","genDecayLVec"), cms.InputTag("prodIsoTrks","trksForIsoVetoLVec"), cms.InputTag("prodIsoTrks","looseisoTrksLVec")),
    vectorIntNamesInTree = cms.vstring('prodGenInfo:WemuVec|W_emuVec', 
        'prodGenInfo:WtauVec|W_tauVec', 
        'prodGenInfo:WtauemuVec|W_tau_emuVec', 
        'prodGenInfo:WtauprongsVec|W_tau_prongsVec', 
        'prodGenInfo:WtaunuVec|W_tau_nuVec', 
        'prodIsoTrks:trksForIsoVetopdgId|trksForIsoVeto_pdgId', 
        'prodIsoTrks:trksForIsoVetoidx|trksForIsoVeto_idx', 
        'prodIsoTrks:looseisoTrkspdgId|loose_isoTrks_pdgId', 
        'prodIsoTrks:looseisoTrksidx|loose_isoTrks_idx'),
    varsDoubleNamesInTree = cms.vstring('ak4stophtPFchs|ht', 
        'ak4jetMHTDPhiForSkimsStop:dPhi0|dPhi0_CUT', 
        'ak4jetMHTDPhiForSkimsStop:dPhi1|dPhi1_CUT', 
        'ak4jetMHTDPhiForSkimsStop:dPhi2|dPhi2_CUT', 
        'prodEventInfo:trunpv|tru_npv', 
        'prodEventInfo:avgnpv|avg_npv', 
        'weightProducer:weight|evtWeight'),
    varsIntNamesInTree = cms.vstring('prodMuons:nMuons|nMuons_CUT', 
        'prodElectrons:nElectrons|nElectrons_CUT', 
        'prodIsoTrks:loosenIsoTrks|loose_nIsoTrks', 
        'prodIsoTrks:nIsoTrksForVeto|nIsoTrks_CUT', 
        'ak4nJetsForSkimsStop:nJets|nJets_CUT'),
    vectorDouble = cms.VInputTag(cms.InputTag("prodMuonsNoIso","muonsCharge"), cms.InputTag("prodMuonsNoIso","muonsMtw"), cms.InputTag("prodMuonsNoIso","muonsRelIso"), cms.InputTag("prodMuonsNoIso","muonsMiniIso"), cms.InputTag("prodElectronsNoIso","elesCharge"), 
        cms.InputTag("prodElectronsNoIso","elesMtw"), cms.InputTag("prodElectronsNoIso","elesRelIso"), cms.InputTag("prodElectronsNoIso","elesMiniIso"), cms.InputTag("prodJets","recoJetschargedHadronEnergyFraction"), cms.InputTag("prodJets","recoJetschargedEmEnergyFraction"), 
        cms.InputTag("prodJets","recoJetsBtag"), cms.InputTag("prodIsoTrks","trksForIsoVetocharge"), cms.InputTag("prodIsoTrks","trksForIsoVetodz"), cms.InputTag("prodIsoTrks","looseisoTrkscharge"), cms.InputTag("prodIsoTrks","looseisoTrksdz"), 
        cms.InputTag("prodIsoTrks","looseisoTrksiso"), cms.InputTag("prodIsoTrks","looseisoTrksmtw")),
    vectorStringNamesInTree = cms.vstring(),
    varsTLorentzVectorNamesInTree = cms.vstring('prodJetsNoMu_jetsLVec|jetsLVecMuCleaned'),
    vectorBool = cms.VInputTag(cms.InputTag("prodElectronsNoIso","elesisEB")),
    debug = cms.bool(False),
    varsInt = cms.VInputTag(cms.InputTag("prodMuons","nMuons"), cms.InputTag("prodMuonsNoIso","nMuons"), cms.InputTag("prodElectrons","nElectrons"), cms.InputTag("prodElectronsNoIso","nElectrons"), cms.InputTag("prodJets","nJets"), 
        cms.InputTag("prodIsoTrks","loosenIsoTrks"), cms.InputTag("prodIsoTrks","nIsoTrksForVeto"), cms.InputTag("ak4nJetsForSkimsStop","nJets"), cms.InputTag("prodEventInfo","vtxSize"), cms.InputTag("prodEventInfo","npv"), 
        cms.InputTag("prodEventInfo","nm1"), cms.InputTag("prodEventInfo","n0"), cms.InputTag("prodEventInfo","np1"), cms.InputTag("type3topTagger","bestTopJetIdx"), cms.InputTag("type3topTagger","pickedRemainingCombfatJetIdx"))
)


process.tauGenJetMatch = cms.EDProducer("GenJetMatcher",
    src = cms.InputTag("hpsPFTauProducer"),
    maxDPtRel = cms.double(3.0),
    mcPdgId = cms.vint32(),
    mcStatus = cms.vint32(),
    resolveByMatchQuality = cms.bool(False),
    maxDeltaR = cms.double(0.1),
    checkCharge = cms.bool(False),
    resolveAmbiguities = cms.bool(True),
    matched = cms.InputTag("tauGenJetsSelectorAllHadrons")
)


process.tauGenJets = cms.EDProducer("TauGenJetProducer",
    includeNeutrinos = cms.bool(False),
    GenParticles = cms.InputTag("genParticles"),
    verbose = cms.untracked.bool(False)
)


process.tauIsoDepositPFCandidates = cms.EDProducer("CandIsoDepositProducer",
    src = cms.InputTag("hpsPFTauProducer"),
    MultipleDepositsFlag = cms.bool(False),
    trackType = cms.string('candidate'),
    ExtractorPSet = cms.PSet(
        Diff_z = cms.double(10000.0),
        ComponentName = cms.string('PFTauExtractor'),
        DR_Max = cms.double(1.0),
        Diff_r = cms.double(10000.0),
        dRvetoPFTauSignalConeConstituents = cms.double(0.01),
        tauSource = cms.InputTag("hpsPFTauProducer"),
        DR_Veto = cms.double(0.0),
        DepositLabel = cms.untracked.string(''),
        candidateSource = cms.InputTag("particleFlow"),
        dRmatchPFTau = cms.double(0.1)
    )
)


process.tauIsoDepositPFChargedHadrons = cms.EDProducer("CandIsoDepositProducer",
    src = cms.InputTag("hpsPFTauProducer"),
    MultipleDepositsFlag = cms.bool(False),
    trackType = cms.string('candidate'),
    ExtractorPSet = cms.PSet(
        Diff_z = cms.double(0.2),
        ComponentName = cms.string('PFTauExtractor'),
        DR_Max = cms.double(1.0),
        Diff_r = cms.double(0.1),
        dRvetoPFTauSignalConeConstituents = cms.double(0.01),
        tauSource = cms.InputTag("hpsPFTauProducer"),
        DR_Veto = cms.double(0.0),
        DepositLabel = cms.untracked.string(''),
        candidateSource = cms.InputTag("pfAllChargedHadrons"),
        dRmatchPFTau = cms.double(0.1)
    )
)


process.tauIsoDepositPFGammas = cms.EDProducer("CandIsoDepositProducer",
    src = cms.InputTag("hpsPFTauProducer"),
    MultipleDepositsFlag = cms.bool(False),
    trackType = cms.string('candidate'),
    ExtractorPSet = cms.PSet(
        Diff_z = cms.double(10000.0),
        ComponentName = cms.string('PFTauExtractor'),
        DR_Max = cms.double(1.0),
        Diff_r = cms.double(10000.0),
        dRvetoPFTauSignalConeConstituents = cms.double(0.01),
        tauSource = cms.InputTag("hpsPFTauProducer"),
        DR_Veto = cms.double(0.0),
        DepositLabel = cms.untracked.string(''),
        candidateSource = cms.InputTag("pfAllPhotons"),
        dRmatchPFTau = cms.double(0.1)
    )
)


process.tauIsoDepositPFNeutralHadrons = cms.EDProducer("CandIsoDepositProducer",
    src = cms.InputTag("hpsPFTauProducer"),
    MultipleDepositsFlag = cms.bool(False),
    trackType = cms.string('candidate'),
    ExtractorPSet = cms.PSet(
        Diff_z = cms.double(10000.0),
        ComponentName = cms.string('PFTauExtractor'),
        DR_Max = cms.double(1.0),
        Diff_r = cms.double(10000.0),
        dRvetoPFTauSignalConeConstituents = cms.double(0.01),
        tauSource = cms.InputTag("hpsPFTauProducer"),
        DR_Veto = cms.double(0.0),
        DepositLabel = cms.untracked.string(''),
        candidateSource = cms.InputTag("pfAllNeutralHadrons"),
        dRmatchPFTau = cms.double(0.1)
    )
)


process.tauMatch = cms.EDProducer("MCMatcher",
    src = cms.InputTag("hpsPFTauProducer"),
    maxDPtRel = cms.double(999.9),
    mcPdgId = cms.vint32(15),
    mcStatus = cms.vint32(2),
    resolveByMatchQuality = cms.bool(False),
    maxDeltaR = cms.double(999.9),
    checkCharge = cms.bool(True),
    resolveAmbiguities = cms.bool(True),
    matched = cms.InputTag("genParticles")
)


process.trackCountingHighEffBJetTags = cms.EDProducer("JetTagProducer",
    jetTagComputer = cms.string('trackCounting3D2nd'),
    tagInfos = cms.VInputTag(cms.InputTag("impactParameterTagInfos"))
)


process.trackCountingHighPurBJetTags = cms.EDProducer("JetTagProducer",
    jetTagComputer = cms.string('trackCounting3D3rd'),
    tagInfos = cms.VInputTag(cms.InputTag("impactParameterTagInfos"))
)


process.trackVertexArbitrator = cms.EDProducer("TrackVertexArbitrator",
    fitterSigmacut = cms.double(3),
    beamSpot = cms.InputTag("offlineBeamSpot"),
    fitterTini = cms.double(256),
    trackMinLayers = cms.int32(4),
    fitterRatio = cms.double(0.25),
    secondaryVertices = cms.InputTag("vertexMerger"),
    sigCut = cms.double(5),
    distCut = cms.double(0.04),
    trackMinPt = cms.double(0.4),
    primaryVertices = cms.InputTag("offlinePrimaryVertices"),
    tracks = cms.InputTag("generalTracks"),
    dLenFraction = cms.double(0.333),
    trackMinPixels = cms.int32(1),
    dRCut = cms.double(0.4)
)


process.unpackedTracksAndVertices = cms.EDProducer("PATTrackAndVertexUnpacker",
    slimmedVertices = cms.InputTag("offlineSlimmedPrimaryVertices"),
    slimmedSecondaryVertices = cms.InputTag("slimmedSecondaryVertices"),
    packedCandidates = cms.InputTag("packedPFCandidates"),
    additionalTracks = cms.InputTag("lostTracks")
)


process.vertexMerger = cms.EDProducer("VertexMerger",
    minSignificance = cms.double(2),
    secondaryVertices = cms.InputTag("inclusiveVertexFinder"),
    maxFraction = cms.double(0.7)
)


process.weightProducer = cms.EDProducer("WeightProducer",
    Exponent = cms.double(-4.5),
    weight = cms.double(-1.0),
    weightName = cms.InputTag("weight"),
    ratioWARNingThreshold = cms.double(1e-10),
    Method = cms.string(''),
    Lumi = cms.double(100.0),
    weightWARNingUpThreshold = cms.double(1e+32),
    NumberEvts = cms.double(-1.0),
    XS = cms.double(49000000000.0),
    LumiScale = cms.double(1.0),
    weightWARNingLowThreshold = cms.double(0.0),
    inputPUfileMC = cms.untracked.string(''),
    inputPUfileData = cms.untracked.string('')
)


process.MT2Cuttye3topTagger = cms.EDFilter("FilterDoubles",
    ResultSource = cms.InputTag("stopType3TopTagger","MT2"),
    lowCut = cms.double(300)
)


process.ak4jetMHTDPhiForSkimsStop = cms.EDFilter("JetMHTDPhiForSkimsRA2",
    MHTSource = cms.InputTag("slimmedMETs"),
    JetSource = cms.InputTag("ak4stopJetsPFchsPt30")
)


process.ak4jetMHTPFchsDPhiFilter = cms.EDFilter("JetMHTDPhiFilter",
    MHTSource = cms.InputTag("mhtPFchs"),
    JetSource = cms.InputTag("ak4patJetsPFchsPt30")
)


process.ak4nJetsForSkimsStop = cms.EDFilter("nJetsForSkimsRA2",
    MaxJetEta = cms.double(2.5),
    MinJetPt = cms.double(50),
    JetSource = cms.InputTag("ak4stopJetsPFchsPt30")
)


process.ak4patJetsPFchsPt10 = cms.EDFilter("simpleJetSelector",
    jetSrc = cms.InputTag("slimmedJets"),
    pfJetCut = cms.string('pt > 10')
)


process.ak4patJetsPFchsPt30 = cms.EDFilter("simpleJetSelector",
    jetSrc = cms.InputTag("slimmedJets"),
    pfJetCut = cms.string('pt > 30')
)


process.ak4patJetsPFchsPt50Eta25 = cms.EDFilter("simpleJetSelector",
    jetSrc = cms.InputTag("slimmedJets"),
    pfJetCut = cms.string('pt > 50 & abs(eta) < 2.5')
)


process.ak4stopJetsPFchsPt30 = cms.EDFilter("simpleJetSelector",
    pfJetCut = cms.string('pt > 30'),
    jetSrc = cms.InputTag("slimmedJets")
)


process.ak4stopJetsPFchsPt50Eta24 = cms.EDFilter("simpleJetSelector",
    pfJetCut = cms.string('pt>50 && abs(eta)<2.4'),
    jetSrc = cms.InputTag("slimmedJets")
)


process.bVertexFilter = cms.EDFilter("BVertexFilter",
    primaryVertices = cms.InputTag("offlinePrimaryVertices"),
    minVertices = cms.int32(0),
    useVertexKinematicAsJetAxis = cms.bool(True),
    vertexFilter = cms.PSet(
        distSig3dMax = cms.double(99999.9),
        fracPV = cms.double(0.65),
        distVal2dMax = cms.double(2.5),
        useTrackWeights = cms.bool(True),
        maxDeltaRToJetAxis = cms.double(0.1),
        v0Filter = cms.PSet(
            k0sMassWindow = cms.double(0.05)
        ),
        distSig2dMin = cms.double(3.0),
        multiplicityMin = cms.uint32(2),
        massMax = cms.double(6.5),
        distSig2dMax = cms.double(99999.9),
        distVal3dMax = cms.double(99999.9),
        minimumTrackWeight = cms.double(0.5),
        distVal3dMin = cms.double(-99999.9),
        distVal2dMin = cms.double(0.01),
        distSig3dMin = cms.double(-99999.9)
    ),
    secondaryVertices = cms.InputTag("secondaryVertices")
)


process.btagJetsSelector = cms.EDFilter("BJetsSelector",
    JetEtaMax = cms.double(2.4),
    JetPtMin = cms.double(30.0),
    bTagValue = cms.double(0.679),
    JetSrc = cms.InputTag("patJetsPF"),
    SelecOnBJets = cms.bool(False),
    bTagName = cms.string('combinedSecondaryVertexBJetTags')
)


process.count2JetsPFchsPt70Eta24Std = cms.EDFilter("PATCandViewCountFilter",
    maxNumber = cms.uint32(999999),
    src = cms.InputTag("stopJetsPFchsPt70Eta24"),
    minNumber = cms.uint32(2)
)


process.count4JetsPFchsPt50Eta24Std = cms.EDFilter("PATCandViewCountFilter",
    maxNumber = cms.uint32(999999),
    src = cms.InputTag("stopJetsPFchsPt50Eta24"),
    minNumber = cms.uint32(4)
)


process.count5JetsPFchsPt30Eta24Std = cms.EDFilter("PATCandViewCountFilter",
    maxNumber = cms.uint32(999999),
    src = cms.InputTag("stopJetsPFchsPt30Eta24"),
    minNumber = cms.uint32(5)
)


process.count5JetsPFchsPt30Std = cms.EDFilter("PATCandViewCountFilter",
    maxNumber = cms.uint32(999999),
    src = cms.InputTag("stopJetsPFchsPt30"),
    minNumber = cms.uint32(5)
)


process.countPFElectronsIDIso = cms.EDFilter("PATCandViewCountFilter",
    maxNumber = cms.uint32(999999),
    src = cms.InputTag("selectedIDIsoElectrons"),
    minNumber = cms.uint32(1)
)


process.countPFMuonsIDIso = cms.EDFilter("PATCandViewCountFilter",
    maxNumber = cms.uint32(999999),
    src = cms.InputTag("selectedIDIsoMuons"),
    minNumber = cms.uint32(1)
)


process.countPatElectrons = cms.EDFilter("PATCandViewCountFilter",
    maxNumber = cms.uint32(999999),
    src = cms.InputTag("cleanPatElectrons"),
    minNumber = cms.uint32(0)
)


process.countPatJets = cms.EDFilter("PATCandViewCountFilter",
    maxNumber = cms.uint32(999999),
    src = cms.InputTag("cleanPatJets"),
    minNumber = cms.uint32(0)
)


process.countPatMuons = cms.EDFilter("PATCandViewCountFilter",
    maxNumber = cms.uint32(999999),
    src = cms.InputTag("cleanPatMuons"),
    minNumber = cms.uint32(0)
)


process.countak4JetsPFchsPt50Eta25 = cms.EDFilter("PATCandViewCountFilter",
    maxNumber = cms.uint32(999999),
    src = cms.InputTag("ak4patJetsPFchsPt50Eta25"),
    minNumber = cms.uint32(3)
)


process.dPhiFilter = cms.EDFilter("DPhiFilter",
    metSrc = cms.InputTag("patMETsPF"),
    jetSrc = cms.InputTag("patJetsAK5PFPt30")
)


process.doublesFilter = cms.EDFilter("FilterDoubles",
    ResultSource = cms.InputTag("topTagger")
)


process.goodOfflinePrimaryVertices = cms.EDFilter("VertexSelector",
    filter = cms.bool(False),
    src = cms.InputTag("offlinePrimaryVertices"),
    cut = cms.string('!isFake && ndof >= 4.0 && abs(z) <= 24.0 && abs(position.Rho) <= 2.0')
)


process.goodVertices = cms.EDFilter("VertexSelector",
    filter = cms.bool(False),
    src = cms.InputTag("offlineSlimmedPrimaryVertices"),
    cut = cms.string('!isFake && ndof > 4 && abs(z) <= 24 && position.rho < 2')
)


process.groomProd = cms.EDFilter("groomProd",
    jetSrc = cms.InputTag("patJetsPFchsPt10")
)


process.groomProdak4 = cms.EDFilter("groomProd",
    jetSrc = cms.InputTag("ak4patJetsPFchsPt10"),
    groomingOpt = cms.untracked.int32(1)
)


process.hltFilter = cms.EDFilter("HLTHighLevel",
    eventSetupPathsKey = cms.string(''),
    andOr = cms.bool(True),
    HLTPaths = cms.vstring('*'),
    throw = cms.bool(True),
    TriggerResultsTag = cms.InputTag("TriggerResults","","HLT")
)


process.htFilter = cms.EDFilter("HTFilter",
    HTSource = cms.InputTag("ht"),
    MinHT = cms.double(350.0)
)


process.htPFchsFilter = cms.EDFilter("HTFilter",
    HTSource = cms.InputTag("htPFchs"),
    MinHT = cms.double(350.0)
)


process.inclusiveSecondaryVerticesFiltered = cms.EDFilter("BVertexFilter",
    primaryVertices = cms.InputTag("offlinePrimaryVertices"),
    minVertices = cms.int32(0),
    useVertexKinematicAsJetAxis = cms.bool(True),
    vertexFilter = cms.PSet(
        distSig3dMax = cms.double(99999.9),
        fracPV = cms.double(0.65),
        distVal2dMax = cms.double(2.5),
        useTrackWeights = cms.bool(True),
        maxDeltaRToJetAxis = cms.double(0.1),
        v0Filter = cms.PSet(
            k0sMassWindow = cms.double(0.05)
        ),
        distSig2dMin = cms.double(3.0),
        multiplicityMin = cms.uint32(2),
        massMax = cms.double(6.5),
        distSig2dMax = cms.double(99999.9),
        distVal3dMax = cms.double(99999.9),
        minimumTrackWeight = cms.double(0.5),
        distVal3dMin = cms.double(-99999.9),
        distVal2dMin = cms.double(0.01),
        distSig3dMin = cms.double(-99999.9)
    ),
    secondaryVertices = cms.InputTag("inclusiveSecondaryVertices")
)


process.jetIDFailure = cms.EDFilter("JetIDFailureFilter",
    JetSource = cms.InputTag("slimmedJets"),
    taggingMode = cms.bool(False),
    MinJetPt = cms.double(30.0),
    MaxNeutralHadFrac = cms.double(0.9),
    debug = cms.bool(False),
    MaxNeutralEMFrac = cms.double(0.95),
    MaxJetEta = cms.double(999.0),
    JECLevel = cms.untracked.string('ak4PFJetsL1FastL2L3')
)


process.jetMHTDPhiFilter = cms.EDFilter("JetMHTDPhiFilter",
    MHTSource = cms.InputTag("mht"),
    JetSource = cms.InputTag("patJets")
)


process.jetMHTDPhiForSkimsRA2 = cms.EDFilter("JetMHTDPhiForSkimsRA2",
    MHTSource = cms.InputTag("mht"),
    JetSource = cms.InputTag("patJets")
)


process.jetsMETDPhiFilter = cms.EDFilter("DPhiFilter",
    metSrc = cms.InputTag("slimmedMETs"),
    jetSrc = cms.InputTag("stopJetsPFchsPt30"),
    dPhiCuts = cms.untracked.vdouble(0.5, 0.5, 0.3)
)


process.loosetrackIsolation = cms.EDFilter("TrackIsolationFilter",
    dz_CutValue = cms.double(0.05),
    isoCut = cms.double(0.5),
    dR_ConeSize = cms.double(0.3),
    minPt_PFCandidate = cms.double(5.0),
    doTrkIsoVeto = cms.bool(False),
    vertexInputTag = cms.InputTag("goodVertices"),
    pfCandidatesTag = cms.InputTag("packedPFCandidates")
)


process.mTbmTtCuttye3topTagger = cms.EDFilter("FilterDoubles",
    ResultSource = cms.InputTag("stopType3TopTagger","linearCombmTbJetPlusmTbestTopJet"),
    lowCut = cms.double(500)
)


process.met175PFchsFilter = cms.EDFilter("MHTFilter",
    MinMHT = cms.double(175),
    MHTSource = cms.InputTag("slimmedMETs")
)


process.met200PFchsFilter = cms.EDFilter("MHTFilter",
    MinMHT = cms.double(200),
    MHTSource = cms.InputTag("slimmedMETs")
)


process.met350PFchsFilter = cms.EDFilter("MHTFilter",
    MinMHT = cms.double(350),
    MHTSource = cms.InputTag("slimmedMETs")
)


process.metPFchsFilter = cms.EDFilter("MHTFilter",
    MinMHT = cms.double(200.0),
    MHTSource = cms.InputTag("slimmedMETs")
)


process.mhtFilter = cms.EDFilter("MHTFilter",
    MinMHT = cms.double(200.0),
    MHTSource = cms.InputTag("mht")
)


process.mhtPFchsFilter = cms.EDFilter("MHTFilter",
    MinMHT = cms.double(200.0),
    MHTSource = cms.InputTag("mhtPFchs")
)


process.nJetsForSkimsRA2 = cms.EDFilter("nJetsForSkimsRA2",
    MaxJetEta = cms.double(2.5),
    MinJetPt = cms.double(50),
    JetSource = cms.InputTag("patJets")
)


process.noscraping = cms.EDFilter("FilterOutScraping",
    debugOn = cms.untracked.bool(False),
    thresh = cms.untracked.double(0.25),
    numtrack = cms.untracked.uint32(10),
    applyfilter = cms.untracked.bool(True)
)


process.oneGoodVertex = cms.EDFilter("VertexCountFilter",
    maxNumber = cms.uint32(999),
    src = cms.InputTag("goodVertices"),
    minNumber = cms.uint32(1)
)


process.patJetsAK4PFCHSPt10 = cms.EDFilter("simpleJetSelector",
    jetSrc = cms.InputTag("patJetsAK4PFCHS"),
    pfJetCut = cms.string('pt >= 10')
)


process.patJetsAK4PFCHSPt10NoMu = cms.EDFilter("simpleJetSelector",
    jetSrc = cms.InputTag("patJetsAK4PFCHSNoMu"),
    pfJetCut = cms.string('pt >= 10')
)


process.pfAllChargedHadrons = cms.EDFilter("PFCandidateFwdPtrCollectionPdgIdFilter",
    pdgId = cms.vint32(211, -211, 321, -321, 999211, 
        2212, -2212),
    makeClones = cms.bool(True),
    src = cms.InputTag("pfNoPileUpIso")
)


process.pfAllChargedParticles = cms.EDFilter("PFCandidateFwdPtrCollectionPdgIdFilter",
    pdgId = cms.vint32(211, -211, 321, -321, 999211, 
        2212, -2212, 11, -11, 13, 
        -13),
    makeClones = cms.bool(True),
    src = cms.InputTag("pfNoPileUpIso")
)


process.pfAllElectrons = cms.EDFilter("PFCandidateFwdPtrCollectionPdgIdFilter",
    pdgId = cms.vint32(11, -11),
    makeClones = cms.bool(True),
    src = cms.InputTag("pfNoMuon")
)


process.pfAllMuons = cms.EDFilter("PFCandidateFwdPtrCollectionPdgIdFilter",
    pdgId = cms.vint32(-13, 13),
    makeClones = cms.bool(True),
    src = cms.InputTag("pfNoPileUp")
)


process.pfAllNeutralHadrons = cms.EDFilter("PFCandidateFwdPtrCollectionPdgIdFilter",
    pdgId = cms.vint32(111, 130, 310, 2112),
    makeClones = cms.bool(True),
    src = cms.InputTag("pfNoPileUpIso")
)


process.pfAllNeutralHadronsAndPhotons = cms.EDFilter("PFCandidateFwdPtrCollectionPdgIdFilter",
    pdgId = cms.vint32(22, 111, 130, 310, 2112),
    makeClones = cms.bool(True),
    src = cms.InputTag("pfNoPileUpIso")
)


process.pfAllPhotons = cms.EDFilter("PFCandidateFwdPtrCollectionPdgIdFilter",
    pdgId = cms.vint32(22),
    makeClones = cms.bool(True),
    src = cms.InputTag("pfNoPileUpIso")
)


process.pfCHS = cms.EDFilter("CandPtrSelector",
    src = cms.InputTag("packedPFCandidates"),
    cut = cms.string('fromPV')
)


process.pfPileUpAllChargedParticles = cms.EDFilter("PFCandidateFwdPtrCollectionPdgIdFilter",
    pdgId = cms.vint32(211, -211, 321, -321, 999211, 
        2212, -2212, 11, -11, 13, 
        -13),
    makeClones = cms.bool(True),
    src = cms.InputTag("pfPileUpIso")
)


process.printDecay = cms.EDFilter("genDecayStringMaker",
    status = cms.untracked.vint32(3),
    keyDecayStrs = cms.vstring('t', 
        'tbar', 
        '~chi_1+', 
        '~chi_1-'),
    src = cms.InputTag("prunedGenParticles"),
    printDecay = cms.untracked.bool(False)
)


process.printDecayPythia8 = cms.EDFilter("genDecayStringMakerPythia8",
    status = cms.untracked.vint32(3),
    keyDecayStrs = cms.vstring('t', 
        'tbar', 
        '~chi_1+', 
        '~chi_1-'),
    src = cms.InputTag("prunedGenParticles"),
    printDecay = cms.untracked.bool(False)
)


process.prodElectrons = cms.EDFilter("prodElectrons",
    metSource = cms.InputTag("slimmedMETs"),
    ConversionsSource = cms.InputTag("reducedEgamma","reducedConversions"),
    BeamSpotSource = cms.InputTag("offlineBeamSpot"),
    MinElePt = cms.double(5),
    DoElectronIsolation = cms.bool(True),
    DoElectronVtxAssociation = cms.bool(True),
    MaxEleEta = cms.double(2.5),
    DoElectronID = cms.bool(True),
    DoElectronVeto = cms.bool(False),
    PFCandSource = cms.InputTag("packedPFCandidates"),
    Debug = cms.bool(False),
    ElectronSource = cms.InputTag("slimmedElectrons"),
    VertexSource = cms.InputTag("goodVertices")
)


process.prodElectronsNoIso = cms.EDFilter("prodElectrons",
    metSource = cms.InputTag("slimmedMETs"),
    ConversionsSource = cms.InputTag("reducedEgamma","reducedConversions"),
    BeamSpotSource = cms.InputTag("offlineBeamSpot"),
    DoElectronIsolation = cms.bool(False),
    MinElePt = cms.double(5),
    DoElectronVtxAssociation = cms.bool(True),
    MaxEleEta = cms.double(2.5),
    DoElectronID = cms.bool(True),
    DoElectronVeto = cms.bool(False),
    PFCandSource = cms.InputTag("packedPFCandidates"),
    Debug = cms.bool(False),
    ElectronSource = cms.InputTag("slimmedElectrons"),
    VertexSource = cms.InputTag("goodVertices")
)


process.prodEventInfo = cms.EDFilter("prodEventInfo",
    debug = cms.bool(False),
    vtxSrc = cms.InputTag("goodVertices")
)


process.prodGenInfo = cms.EDFilter("prodGenInfo",
    debug = cms.bool(False),
    genParticleSrc = cms.InputTag("prunedGenParticles"),
    genDecayStrVecSrc = cms.InputTag("printDecayPythia8","decayStr"),
    genDecayChainPartIdxVecSrc = cms.InputTag("printDecayPythia8","decayChainPartIdxVec")
)


process.prodIsoTrks = cms.EDFilter("prodIsoTrks",
    W_tau_emuVec = cms.InputTag("prodGenInfo","WtauemuVec"),
    isotrk_dz = cms.double(0.15),
    W_tau_prongsVec = cms.InputTag("prodGenInfo","WtauprongsVec"),
    isotrk_dR = cms.double(0.5),
    forVetoIsoTrkSrc = cms.InputTag("trackIsolation"),
    vtxSrc = cms.InputTag("goodVertices"),
    loose_isotrk_dzpvVecSrc = cms.InputTag("loosetrackIsolation","pfcandsdzpv"),
    metSrc = cms.InputTag("slimmedMETs"),
    W_emuVec = cms.InputTag("prodGenInfo","WemuVec"),
    loose_isoTrkSrc = cms.InputTag("loosetrackIsolation"),
    debug = cms.bool(False),
    loose_isotrk_isoVecSrc = cms.InputTag("loosetrackIsolation","pfcandstrkiso"),
    genDecayLVec = cms.InputTag("prodGenInfo","genDecayLVec"),
    pfCandSrc = cms.InputTag("packedPFCandidates")
)


process.prodJets = cms.EDFilter("prodJets",
    W_tau_emuVec = cms.InputTag("prodGenInfo","WtauemuVec"),
    muLVec = cms.InputTag("prodMuonsNoIso","muonsLVec"),
    W_tau_prongsVec = cms.InputTag("prodGenInfo","WtauprongsVec"),
    jetOtherSrc = cms.InputTag("patJetsAK4PFCHSNoMu"),
    eleLVec = cms.InputTag("prodElectronsNoIso","elesLVec"),
    vtxSrc = cms.InputTag("goodVertices"),
    genDecayMomRefVec = cms.InputTag("prodGenInfo","genDecayMomRefVec"),
    metSrc = cms.InputTag("slimmedMETs"),
    W_emuVec = cms.InputTag("prodGenInfo","WemuVec"),
    W_tauVec = cms.InputTag("prodGenInfo","WtauVec"),
    jetSrc = cms.InputTag("slimmedJets"),
    debug = cms.bool(False),
    W_tau_nuVec = cms.InputTag("prodGenInfo","WtaunuVec"),
    bTagKeyString = cms.string('combinedInclusiveSecondaryVertexV2BJetTags'),
    genDecayLVec = cms.InputTag("prodGenInfo","genDecayLVec")
)


process.prodJetsNoMu = cms.EDFilter("prodJets",
    W_tau_emuVec = cms.InputTag("prodGenInfo","WtauemuVec"),
    muLVec = cms.InputTag("prodMuonsNoIso","muonsLVec"),
    W_tau_prongsVec = cms.InputTag("prodGenInfo","WtauprongsVec"),
    jetOtherSrc = cms.InputTag("patJetsAK4PFCHS"),
    eleLVec = cms.InputTag("prodElectronsNoIso","elesLVec"),
    vtxSrc = cms.InputTag("goodVertices"),
    genDecayMomRefVec = cms.InputTag("prodGenInfo","genDecayMomRefVec"),
    metSrc = cms.InputTag("slimmedMETs"),
    W_emuVec = cms.InputTag("prodGenInfo","WemuVec"),
    W_tauVec = cms.InputTag("prodGenInfo","WtauVec"),
    jetSrc = cms.InputTag("patJetsAK4PFCHSPt10NoMu"),
    debug = cms.bool(False),
    W_tau_nuVec = cms.InputTag("prodGenInfo","WtaunuVec"),
    bTagKeyString = cms.string('combinedInclusiveSecondaryVertexV2BJetTags'),
    genDecayLVec = cms.InputTag("prodGenInfo","genDecayLVec")
)


process.prodMET = cms.EDFilter("prodMET",
    debug = cms.bool(False),
    metSrc = cms.InputTag("slimmedMETs")
)


process.prodMuons = cms.EDFilter("prodMuons",
    metSource = cms.InputTag("slimmedMETs"),
    DoMuonID = cms.bool(True),
    MinMuNumHit = cms.double(11),
    DoMuonVeto = cms.bool(False),
    DoMuonVtxAssociation = cms.bool(True),
    DoMuonIsolation = cms.bool(True),
    PFCandSource = cms.InputTag("packedPFCandidates"),
    MaxMuEta = cms.double(2.4),
    VertexSource = cms.InputTag("goodVertices"),
    MaxMuD0 = cms.double(0.2),
    Debug = cms.bool(False),
    MaxMuMiniIso = cms.double(0.2),
    MaxMuRelIso = cms.double(0.2),
    MaxMuDz = cms.double(0.5),
    MuonSource = cms.InputTag("slimmedMuons"),
    MinMuPt = cms.double(5)
)


process.prodMuonsNoIso = cms.EDFilter("prodMuons",
    metSource = cms.InputTag("slimmedMETs"),
    DoMuonID = cms.bool(True),
    MinMuNumHit = cms.double(11),
    DoMuonVeto = cms.bool(False),
    DoMuonVtxAssociation = cms.bool(True),
    DoMuonIsolation = cms.bool(False),
    MaxMuD0 = cms.double(0.2),
    MaxMuEta = cms.double(2.4),
    MaxMuDz = cms.double(0.5),
    PFCandSource = cms.InputTag("packedPFCandidates"),
    Debug = cms.bool(False),
    MaxMuMiniIso = cms.double(0.2),
    MaxMuRelIso = cms.double(0.2),
    VertexSource = cms.InputTag("goodVertices"),
    MuonSource = cms.InputTag("slimmedMuons"),
    MinMuPt = cms.double(5)
)


process.selectedElectrons = cms.EDFilter("CandPtrSelector",
    src = cms.InputTag("slimmedElectrons"),
    cut = cms.string("abs(eta)<2.5 && pt>5. &&\n    gsfTrack.isAvailable() &&\n    gsfTrack.hitPattern().numberOfLostHits(\'MISSING_INNER_HITS\')<2 && \n    (pfIsolationVariables().sumChargedHadronPt+\n    max(0.,pfIsolationVariables().sumNeutralHadronEt+\n    pfIsolationVariables().sumPhotonEt-\n    0.5*pfIsolationVariables().sumPUPt))/pt < 0.20")
)


process.selectedIDElectrons = cms.EDFilter("CandPtrSelector",
    src = cms.InputTag("slimmedElectrons"),
    cut = cms.string("abs(eta)<2.5 && pt>5. &&\n   gsfTrack.isAvailable() &&\n   gsfTrack.hitPattern().numberOfLostHits(\'MISSING_INNER_HITS\')<2")
)


process.selectedIDIsoElectrons = cms.EDFilter("CandPtrSelector",
    src = cms.InputTag("slimmedElectrons"),
    cut = cms.string("abs(eta)<2.5 && pt>5. &&\n   gsfTrack.isAvailable() &&\n   gsfTrack.hitPattern().numberOfLostHits(\'MISSING_INNER_HITS\')<2 &&\n   (pfIsolationVariables().sumChargedHadronPt+\n   max(0.,pfIsolationVariables().sumNeutralHadronEt+\n   pfIsolationVariables().sumPhotonEt-\n   0.5*pfIsolationVariables().sumPUPt))/pt < 0.20")
)


process.selectedIDIsoMuons = cms.EDFilter("CandPtrSelector",
    src = cms.InputTag("slimmedMuons"),
    cut = cms.string('abs(eta)<2.5 && pt>5. &&\n   (pfIsolationR04().sumChargedHadronPt+\n   max(0.,pfIsolationR04().sumNeutralHadronEt+\n   pfIsolationR04().sumPhotonEt-\n   0.50*pfIsolationR04().sumPUPt))/pt < 0.20 &&\n   (isPFMuon && (isGlobalMuon || isTrackerMuon) )')
)


process.selectedIDMuons = cms.EDFilter("CandPtrSelector",
    src = cms.InputTag("slimmedMuons"),
    cut = cms.string('abs(eta)<2.5 && pt>5. &&\n   (isPFMuon && (isGlobalMuon || isTrackerMuon) )')
)


process.selectedMuons = cms.EDFilter("CandPtrSelector",
    src = cms.InputTag("slimmedMuons"),
    cut = cms.string('abs(eta)<2.5 && pt>5. &&\n   (pfIsolationR04().sumChargedHadronPt+\n    max(0.,pfIsolationR04().sumNeutralHadronEt+\n    pfIsolationR04().sumPhotonEt-\n    0.50*pfIsolationR04().sumPUPt))/pt < 0.20 && \n    (isPFMuon && (isGlobalMuon || isTrackerMuon) )')
)


process.selectedPatJetsAK4PFCHS = cms.EDFilter("PATJetSelector",
    src = cms.InputTag("patJetsAK4PFCHS"),
    cut = cms.string('')
)


process.selectedPatJetsAK4PFCHSNoMu = cms.EDFilter("PATJetSelector",
    src = cms.InputTag("patJetsAK4PFCHSNoMu"),
    cut = cms.string('')
)


process.selectedPatJetsRA2 = cms.EDFilter("simpleJetSelector",
    jetSrc = cms.InputTag("patJetsAK5PFPt30")
)


process.simpleJetSelector = cms.EDFilter("simpleJetSelector",
    jetSrc = cms.InputTag("patJetsAK5PFPt30")
)


process.smsModelFilter = cms.EDFilter("SMSModelFilter",
    Debug = cms.bool(False),
    SusyScanTopology = cms.string('T1tttt'),
    SusyScanFracLSP = cms.double(0.0),
    SusyScanMotherMass = cms.double(-1),
    SusyScanLSPMass = cms.double(-1)
)


process.stopBJets = cms.EDFilter("BJetsSelector",
    JetEtaMax = cms.double(2.4),
    JetPtMin = cms.double(30.0),
    bTagValue = cms.double(0.679),
    JetSrc = cms.InputTag("stopJetsPFchsPt30"),
    SelecOnBJets = cms.bool(False),
    bTagName = cms.string('combinedSecondaryVertexBJetTags')
)


process.stopCount1BJets = cms.EDFilter("PATCandViewCountFilter",
    maxNumber = cms.uint32(999999),
    src = cms.InputTag("stopBJets"),
    minNumber = cms.uint32(1)
)


process.stopCount2BJets = cms.EDFilter("PATCandViewCountFilter",
    maxNumber = cms.uint32(999999),
    src = cms.InputTag("stopBJets"),
    minNumber = cms.uint32(2)
)


process.stopCountBJets = cms.EDFilter("PATCandViewCountFilter",
    maxNumber = cms.uint32(999999),
    src = cms.InputTag("stopBJets"),
    minNumber = cms.uint32(1)
)


process.stopCountPFchsJetsPt30 = cms.EDFilter("PATCandViewCountFilter",
    maxNumber = cms.uint32(999999),
    src = cms.InputTag("stopJetsPFchsPt30"),
    minNumber = cms.uint32(6)
)


process.stopCountPFchsJetsPt70eta2p5 = cms.EDFilter("PATCandViewCountFilter",
    maxNumber = cms.uint32(999999),
    src = cms.InputTag("stopJetsPFchsPt70eta2p5"),
    minNumber = cms.uint32(2)
)


process.stopJetsPFchsPt30 = cms.EDFilter("simpleJetSelector",
    jetSrc = cms.InputTag("slimmedJets"),
    pfJetCut = cms.string('pt > 30')
)


process.stopJetsPFchsPt30Eta24 = cms.EDFilter("simpleJetSelector",
    jetSrc = cms.InputTag("slimmedJets"),
    pfJetCut = cms.string('pt>30 && abs(eta)<2.4')
)


process.stopJetsPFchsPt50Eta24 = cms.EDFilter("simpleJetSelector",
    jetSrc = cms.InputTag("slimmedJets"),
    pfJetCut = cms.string('pt>50 && abs(eta)<2.4')
)


process.stopJetsPFchsPt70Eta24 = cms.EDFilter("simpleJetSelector",
    jetSrc = cms.InputTag("slimmedJets"),
    pfJetCut = cms.string('pt>70 && abs(eta)<2.4')
)


process.stopJetsPFchsPt70eta2p5 = cms.EDFilter("simpleJetSelector",
    jetSrc = cms.InputTag("slimmedJets"),
    pfJetCut = cms.string('pt > 70 & abs(eta) < 2.5')
)


process.tauGenJetsSelectorAllHadrons = cms.EDFilter("TauGenJetDecayModeSelector",
    filter = cms.bool(False),
    src = cms.InputTag("tauGenJets"),
    select = cms.vstring('oneProng0Pi0', 
        'oneProng1Pi0', 
        'oneProng2Pi0', 
        'oneProngOther', 
        'threeProng0Pi0', 
        'threeProng1Pi0', 
        'threeProngOther', 
        'rare')
)


process.topMassCuttype3topTagger = cms.EDFilter("FilterDoubles",
    ResultSource = cms.InputTag("stopType3TopTagger","bestTopJetMass"),
    lowCut = cms.double(80),
    highCut = cms.double(270)
)


process.topTagger = cms.EDFilter("TopTagger",
    vtxSrc = cms.InputTag("goodVertices"),
    evtWeightInput = cms.InputTag(""),
    metSrc = cms.InputTag("patMETsPF"),
    jetSrc = cms.InputTag("patJetsAK5PFPt30")
)


process.trackIsolation = cms.EDFilter("TrackIsolationFilter",
    dz_CutValue = cms.double(0.05),
    isoCut = cms.double(0.1),
    dR_ConeSize = cms.double(0.3),
    minPt_PFCandidate = cms.double(10.0),
    doTrkIsoVeto = cms.bool(False),
    vertexInputTag = cms.InputTag("goodVertices"),
    pfCandidatesTag = cms.InputTag("packedPFCandidates")
)


process.trackIsolationCounter = cms.EDFilter("PATCandViewCountFilter",
    maxNumber = cms.uint32(999999),
    src = cms.InputTag("trackIsolationFilter"),
    minNumber = cms.uint32(0)
)


process.trackIsolationFilter = cms.EDFilter("TrackIsolationFilter",
    dz_CutValue = cms.double(0.05),
    isoCut = cms.double(0.1),
    dR_ConeSize = cms.double(0.3),
    minPt_PFCandidate = cms.double(10.0),
    doTrkIsoVeto = cms.bool(True),
    vertexInputTag = cms.InputTag("goodVertices"),
    pfCandidatesTag = cms.InputTag("packedPFCandidates")
)


process.type3topTagger = cms.EDFilter("TopTagger",
    vtxSrc = cms.InputTag("goodVertices"),
    evtWeightInput = cms.InputTag(""),
    metSrc = cms.InputTag("slimmedMETs"),
    jetSrc = cms.InputTag("stopJetsPFchsPt30"),
    taggingMode = cms.untracked.bool(True)
)


process.patCandidateSummary = cms.EDAnalyzer("CandidateSummaryTable",
    logName = cms.untracked.string('patCandidates|PATSummaryTables'),
    candidates = cms.VInputTag(cms.InputTag("patElectrons"), cms.InputTag("patMuons"), cms.InputTag("patTaus"), cms.InputTag("patPhotons"), cms.InputTag("patJets"), 
        cms.InputTag("patMETs"))
)


process.pfNoPileUpJMESequence = cms.Sequence(process.goodOfflinePrimaryVertices+process.pfPileUpJME+process.pfNoPileUpJME)


process.photonPFIsolationDepositsSequence = cms.Sequence(process.phPFIsoDepositCharged+process.phPFIsoDepositChargedAll+process.phPFIsoDepositGamma+process.phPFIsoDepositNeutral+process.phPFIsoDepositPU)


process.correctionTermsPfMetType1Type2 = cms.Sequence(process.pfJetsPtrForMetCorr+process.particleFlowPtrs+process.pfCandsNotInJetsPtrForMetCorr+process.pfCandsNotInJetsForMetCorr+process.pfCandMETcorr+process.corrPfMetType1+process.corrPfMetType2)


process.pfSortByTypeSequence = cms.Sequence(process.pfAllNeutralHadrons+process.pfAllChargedHadrons+process.pfAllPhotons+process.pfAllChargedParticles+process.pfPileUpAllChargedParticles+process.pfAllNeutralHadronsAndPhotons)


process.ra2PFchsJets = cms.Sequence(process.ak4patJetsPFchsPt10+process.ak4patJetsPFchsPt30+process.ak4patJetsPFchsPt50Eta25)


process.pfPhotonIsolationSequence = cms.Sequence(process.photonPFIsolationDepositsSequence+process.phPFIsoValueCharged03PFId+process.phPFIsoValueChargedAll03PFId+process.phPFIsoValueGamma03PFId+process.phPFIsoValueNeutral03PFId+process.phPFIsoValuePU03PFId+process.phPFIsoValueCharged04PFId+process.phPFIsoValueChargedAll04PFId+process.phPFIsoValueGamma04PFId+process.phPFIsoValueNeutral04PFId+process.phPFIsoValuePU04PFId)


process.patJetFlavourId = cms.Sequence(process.patJetPartons+process.patJetFlavourAssociation)


process.stopPFJets = cms.Sequence(process.stopJetsPFchsPt30+process.stopJetsPFchsPt30Eta24+process.stopJetsPFchsPt50Eta24+process.stopJetsPFchsPt70Eta24+process.stopJetsPFchsPt70eta2p5)


process.btagging = cms.Sequence(process.impactParameterTagInfos+process.trackCountingHighEffBJetTags+process.trackCountingHighPurBJetTags+process.jetProbabilityBJetTags+process.jetBProbabilityBJetTags+process.secondaryVertexTagInfos+process.simpleSecondaryVertexHighEffBJetTags+process.simpleSecondaryVertexHighPurBJetTags+process.combinedSecondaryVertexBJetTags+process.inclusiveSecondaryVertexFinderTagInfos+process.combinedInclusiveSecondaryVertexV2BJetTags+process.ghostTrackVertexTagInfos+process.ghostTrackBJetTags+process.softPFMuonsTagInfos+process.softPFMuonBJetTags+process.softPFElectronsTagInfos+process.softPFElectronBJetTags+process.combinedMVABJetTags+process.pfImpactParameterTagInfos+process.pfSecondaryVertexTagInfos+process.pfCombinedSecondaryVertexBJetTags)


process.patPFTauIsolation = cms.Sequence(process.tauIsoDepositPFCandidates+process.tauIsoDepositPFChargedHadrons+process.tauIsoDepositPFNeutralHadrons+process.tauIsoDepositPFGammas)


process.inclusiveVertexing = cms.Sequence(process.inclusiveVertexFinder+process.vertexMerger+process.trackVertexArbitrator+process.inclusiveSecondaryVertices)


process.stopType3TopTagger = cms.Sequence(process.type3topTagger)


process.pfNoPileUpSequence = cms.Sequence(process.pfPileUp+process.pfNoPileUp)


process.ra2PFMuonVeto = cms.Sequence(~process.countPFMuonsIDIso)


process.prepareCutvars_seq = cms.Sequence(process.ak4stopJetsPFchsPt30+process.ak4stopJetsPFchsPt50Eta24+process.ak4nJetsForSkimsStop+process.ak4jetMHTDPhiForSkimsStop+process.ak4stophtPFchs+process.ak4stopmhtPFchs)


process.correctionTermsCaloMet = cms.Sequence(process.corrCaloMetType1+process.muCaloMetCorr+process.corrCaloMetType2)


process.ra2StdCleaning = cms.Sequence(process.oneGoodVertex)


process.pfNoPileUpIsoSequence = cms.Sequence(process.pfPileUpIso+process.pfNoPileUpIso)


process.patJetCorrections = cms.Sequence(process.patJetCorrFactors)


process.patFixedConePFTauDiscrimination = cms.Sequence()


process.patJetFlavourIdLegacy = cms.Sequence(process.patJetPartonsLegacy+process.patJetPartonAssociationLegacy+process.patJetFlavourAssociationLegacy)


process.genJetParticles = cms.Sequence(process.genParticlesForJets)


process.stopType3topTaggerAllCuts = cms.Sequence(process.topMassCuttype3topTagger+process.mTbmTtCuttye3topTagger+process.MT2Cuttye3topTagger)


process.updateHPSPFTaus = cms.Sequence()


process.patCaloTauDiscrimination = cms.Sequence()


process.ra2PFElectronVeto = cms.Sequence(~process.countPFElectronsIDIso)


process.ra2Objects = cms.Sequence(process.ra2PFchsJets+process.htPFchs+process.mhtPFchs+process.selectedMuons+process.selectedElectrons+process.selectedIDIsoMuons+process.selectedIDMuons+process.selectedIDIsoElectrons+process.selectedIDElectrons)


process.muonPFIsolationDepositsSequence = cms.Sequence(process.muPFIsoDepositCharged+process.muPFIsoDepositChargedAll+process.muPFIsoDepositGamma+process.muPFIsoDepositNeutral+process.muPFIsoDepositPU)


process.electronPFIsolationDepositsSequence = cms.Sequence(process.elPFIsoDepositCharged+process.elPFIsoDepositChargedAll+process.elPFIsoDepositGamma+process.elPFIsoDepositNeutral+process.elPFIsoDepositPU)


process.patShrinkingConePFTauDiscrimination = cms.Sequence()


process.pfElectronIsolationSequence = cms.Sequence(process.electronPFIsolationDepositsSequence+process.elPFIsoValueCharged03PFId+process.elPFIsoValueChargedAll03PFId+process.elPFIsoValueGamma03PFId+process.elPFIsoValueNeutral03PFId+process.elPFIsoValuePU03PFId+process.elPFIsoValueCharged04PFId+process.elPFIsoValueChargedAll04PFId+process.elPFIsoValueGamma04PFId+process.elPFIsoValueNeutral04PFId+process.elPFIsoValuePU04PFId+process.elPFIsoValueCharged03NoPFId+process.elPFIsoValueChargedAll03NoPFId+process.elPFIsoValueGamma03NoPFId+process.elPFIsoValueNeutral03NoPFId+process.elPFIsoValuePU03NoPFId+process.elPFIsoValueCharged04NoPFId+process.elPFIsoValueChargedAll04NoPFId+process.elPFIsoValueGamma04NoPFId+process.elPFIsoValueNeutral04NoPFId+process.elPFIsoValuePU04NoPFId)


process.patMETCorrections = cms.Sequence(process.correctionTermsCaloMet+process.caloMetT1+process.caloMetT1T2+process.correctionTermsPfMetType1Type2+process.pfMetT1+process.pfMetT1T2)


process.pfParticleSelectionSequence = cms.Sequence(process.pfNoPileUpIsoSequence+process.pfNoPileUpSequence+process.pfSortByTypeSequence)


process.patHPSPFTauDiscrimination = cms.Sequence(process.updateHPSPFTaus)


process.cleanpatseq = cms.Sequence(process.postStdCleaningCounter+process.ra2Objects)


process.makePatMETs = cms.Sequence(process.patMETCorrections+process.patMETs)


process.pfParticleSelectionForIsoSequence = cms.Sequence(process.particleFlowPtrs+process.pfNoPileUpIsoSequence+process.pfParticleSelectionSequence)


process.muonPFIsolationSequence = cms.Sequence(process.muonPFIsolationDepositsSequence+process.muPFIsoValueCharged03+process.muPFMeanDRIsoValueCharged03+process.muPFSumDRIsoValueCharged03+process.muPFIsoValueChargedAll03+process.muPFMeanDRIsoValueChargedAll03+process.muPFSumDRIsoValueChargedAll03+process.muPFIsoValueGamma03+process.muPFMeanDRIsoValueGamma03+process.muPFSumDRIsoValueGamma03+process.muPFIsoValueNeutral03+process.muPFMeanDRIsoValueNeutral03+process.muPFSumDRIsoValueNeutral03+process.muPFIsoValueGammaHighThreshold03+process.muPFMeanDRIsoValueGammaHighThreshold03+process.muPFSumDRIsoValueGammaHighThreshold03+process.muPFIsoValueNeutralHighThreshold03+process.muPFMeanDRIsoValueNeutralHighThreshold03+process.muPFSumDRIsoValueNeutralHighThreshold03+process.muPFIsoValuePU03+process.muPFMeanDRIsoValuePU03+process.muPFSumDRIsoValuePU03+process.muPFIsoValueCharged04+process.muPFMeanDRIsoValueCharged04+process.muPFSumDRIsoValueCharged04+process.muPFIsoValueChargedAll04+process.muPFMeanDRIsoValueChargedAll04+process.muPFSumDRIsoValueChargedAll04+process.muPFIsoValueGamma04+process.muPFMeanDRIsoValueGamma04+process.muPFSumDRIsoValueGamma04+process.muPFIsoValueNeutral04+process.muPFMeanDRIsoValueNeutral04+process.muPFSumDRIsoValueNeutral04+process.muPFIsoValueGammaHighThreshold04+process.muPFMeanDRIsoValueGammaHighThreshold04+process.muPFSumDRIsoValueGammaHighThreshold04+process.muPFIsoValueNeutralHighThreshold04+process.muPFMeanDRIsoValueNeutralHighThreshold04+process.muPFSumDRIsoValueNeutralHighThreshold04+process.muPFIsoValuePU04+process.muPFMeanDRIsoValuePU04+process.muPFSumDRIsoValuePU04)


process.comb_seq = cms.Sequence(process.cleanpatseq+process.hltFilter+process.weightProducer+process.trackIsolation+process.loosetrackIsolation+process.stopPFJets+process.stopBJets+process.prepareCutvars_seq)


process.patPFCandidateIsoDepositSelection = cms.Sequence(process.pfNoPileUpIsoSequence+process.pfSortByTypeSequence)


process.makePatTaus = cms.Sequence(process.patHPSPFTauDiscrimination+process.patPFCandidateIsoDepositSelection+process.patPFTauIsolation+process.tauMatch+process.tauGenJets+process.tauGenJetsSelectorAllHadrons+process.tauGenJetMatch+process.patTaus)


process.makePatJets = cms.Sequence(process.patJetCorrections+process.patJetCharge+process.patJetPartonMatch+process.patJetGenJetMatch+process.patJetFlavourIdLegacy+process.patJetFlavourId+process.patJets)


process.makePatElectrons = cms.Sequence(process.pfParticleSelectionForIsoSequence+process.pfElectronIsolationSequence+process.electronMatch+process.patElectrons)


process.makePatPhotons = cms.Sequence(process.pfParticleSelectionForIsoSequence+process.pfPhotonIsolationSequence+process.photonMatch+process.patPhotons)


process.pfMuonIsolationSequence = cms.Sequence(process.muonPFIsolationSequence)


process.makePatMuons = cms.Sequence(process.pfParticleSelectionForIsoSequence+process.pfMuonIsolationSequence+process.muonMatch+process.patMuons)


process.patCandidates = cms.Sequence(process.makePatElectrons+process.makePatMuons+process.makePatTaus+process.makePatPhotons+process.makePatJets+process.makePatMETs+process.patCandidateSummary)


process.ak4Stop_Path = cms.Path(process.comb_seq+process.printDecayPythia8+process.prodGenInfo+process.prodMuons+process.prodMuonsNoIso+process.prodElectrons+process.prodElectronsNoIso+process.prodJets+process.prodMET+process.prodIsoTrks+process.prodEventInfo+process.type3topTagger+process.stopTreeMaker)


process.MessageLogger = cms.Service("MessageLogger",
    suppressInfo = cms.untracked.vstring(),
    debugs = cms.untracked.PSet(
        placeholder = cms.untracked.bool(True)
    ),
    suppressDebug = cms.untracked.vstring(),
    cout = cms.untracked.PSet(
        placeholder = cms.untracked.bool(True)
    ),
    cerr_stats = cms.untracked.PSet(
        threshold = cms.untracked.string('WARNING'),
        output = cms.untracked.string('cerr'),
        optionalPSet = cms.untracked.bool(True)
    ),
    warnings = cms.untracked.PSet(
        placeholder = cms.untracked.bool(True)
    ),
    default = cms.untracked.PSet(

    ),
    statistics = cms.untracked.vstring('cerr_stats'),
    cerr = cms.untracked.PSet(
        INFO = cms.untracked.PSet(
            limit = cms.untracked.int32(0)
        ),
        noTimeStamps = cms.untracked.bool(False),
        FwkReport = cms.untracked.PSet(
            reportEvery = cms.untracked.int32(100),
            optionalPSet = cms.untracked.bool(True),
            limit = cms.untracked.int32(10000000)
        ),
        default = cms.untracked.PSet(
            limit = cms.untracked.int32(10000000)
        ),
        Root_NoDictionary = cms.untracked.PSet(
            optionalPSet = cms.untracked.bool(True),
            limit = cms.untracked.int32(0)
        ),
        threshold = cms.untracked.string('INFO'),
        FwkJob = cms.untracked.PSet(
            optionalPSet = cms.untracked.bool(True),
            limit = cms.untracked.int32(0)
        ),
        FwkSummary = cms.untracked.PSet(
            reportEvery = cms.untracked.int32(1),
            optionalPSet = cms.untracked.bool(True),
            limit = cms.untracked.int32(10000000)
        ),
        optionalPSet = cms.untracked.bool(True)
    ),
    FrameworkJobReport = cms.untracked.PSet(
        default = cms.untracked.PSet(
            limit = cms.untracked.int32(0)
        ),
        optionalPSet = cms.untracked.bool(True),
        FwkJob = cms.untracked.PSet(
            optionalPSet = cms.untracked.bool(True),
            limit = cms.untracked.int32(10000000)
        )
    ),
    suppressWarning = cms.untracked.vstring('ecalLaserCorrFilter', 
        'manystripclus53X', 
        'toomanystripclus53X'),
    errors = cms.untracked.PSet(
        placeholder = cms.untracked.bool(True)
    ),
    destinations = cms.untracked.vstring('warnings', 
        'errors', 
        'infos', 
        'debugs', 
        'cout', 
        'cerr'),
    debugModules = cms.untracked.vstring(),
    infos = cms.untracked.PSet(
        optionalPSet = cms.untracked.bool(True),
        Root_NoDictionary = cms.untracked.PSet(
            optionalPSet = cms.untracked.bool(True),
            limit = cms.untracked.int32(0)
        ),
        placeholder = cms.untracked.bool(True)
    ),
    categories = cms.untracked.vstring('FwkJob', 
        'FwkReport', 
        'FwkSummary', 
        'Root_NoDictionary'),
    fwkJobReports = cms.untracked.vstring('FrameworkJobReport')
)


process.TFileService = cms.Service("TFileService",
    fileName = cms.string('stopFlatNtuples.root')
)


process.AK4PFchsCombinedCorrector = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('AK4PFchsL1FastJet', 
        'AK4PFchsL2Relative', 
        'AK4PFchsL3Absolute')
)


process.AK4PFchsL1FastJet = cms.ESProducer("L1FastjetCorrectionESProducer",
    srcRho = cms.InputTag("fixedGridRhoFastjetAll"),
    algorithm = cms.string('AK4PFchs'),
    level = cms.string('L1FastJet')
)


process.AK4PFchsL1Offset = cms.ESProducer("L1OffsetCorrectionESProducer",
    minVtxNdof = cms.int32(4),
    vertexCollection = cms.string('offlinePrimaryVertices'),
    algorithm = cms.string('AK4PFchs'),
    level = cms.string('L1Offset')
)


process.AK4PFchsL2L3Residual = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK4PFchs'),
    level = cms.string('L2L3Residual')
)


process.AK4PFchsL2Relative = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK4PFchs'),
    level = cms.string('L2Relative')
)


process.AK4PFchsL3Absolute = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK4PFchs'),
    level = cms.string('L3Absolute')
)


process.CSCGeometryESModule = cms.ESProducer("CSCGeometryESModule",
    appendToDataLabel = cms.string(''),
    useDDD = cms.bool(True),
    debugV = cms.untracked.bool(False),
    useGangedStripsInME1a = cms.bool(True),
    alignmentsLabel = cms.string(''),
    useOnlyWiresInME1a = cms.bool(False),
    useRealWireGeometry = cms.bool(True),
    useCentreTIOffsets = cms.bool(False),
    applyAlignment = cms.bool(True)
)


process.CaloGeometryBuilder = cms.ESProducer("CaloGeometryBuilder",
    SelectedCalos = cms.vstring('HCAL', 
        'ZDC', 
        'CASTOR', 
        'EcalBarrel', 
        'EcalEndcap', 
        'EcalPreshower', 
        'TOWER')
)


process.CaloTopologyBuilder = cms.ESProducer("CaloTopologyBuilder")


process.CaloTowerHardcodeGeometryEP = cms.ESProducer("CaloTowerHardcodeGeometryEP")


process.CastorDbProducer = cms.ESProducer("CastorDbProducer")


process.CastorHardcodeGeometryEP = cms.ESProducer("CastorHardcodeGeometryEP")


process.DTGeometryESModule = cms.ESProducer("DTGeometryESModule",
    appendToDataLabel = cms.string(''),
    fromDDD = cms.bool(True),
    applyAlignment = cms.bool(True),
    alignmentsLabel = cms.string('')
)


process.EcalBarrelGeometryEP = cms.ESProducer("EcalBarrelGeometryEP",
    applyAlignment = cms.bool(False)
)


process.EcalElectronicsMappingBuilder = cms.ESProducer("EcalElectronicsMappingBuilder")


process.EcalEndcapGeometryEP = cms.ESProducer("EcalEndcapGeometryEP",
    applyAlignment = cms.bool(False)
)


process.EcalLaserCorrectionService = cms.ESProducer("EcalLaserCorrectionService")


process.EcalPreshowerGeometryEP = cms.ESProducer("EcalPreshowerGeometryEP",
    applyAlignment = cms.bool(False)
)


process.EcalTrigTowerConstituentsMapBuilder = cms.ESProducer("EcalTrigTowerConstituentsMapBuilder",
    MapFile = cms.untracked.string('Geometry/EcalMapping/data/EndCap_TTMap.txt')
)


process.GlobalTrackingGeometryESProducer = cms.ESProducer("GlobalTrackingGeometryESProducer")


process.HcalHardcodeGeometryEP = cms.ESProducer("HcalHardcodeGeometryEP",
    HcalReLabel = cms.PSet(
        RelabelRules = cms.untracked.PSet(
            Eta16 = cms.untracked.vint32(1, 1, 2, 2, 2, 
                2, 2, 2, 2, 3, 
                3, 3, 3, 3, 3, 
                3, 3, 3, 3),
            Eta17 = cms.untracked.vint32(1, 1, 2, 2, 3, 
                3, 3, 4, 4, 4, 
                4, 4, 5, 5, 5, 
                5, 5, 5, 5),
            Eta1 = cms.untracked.vint32(1, 2, 2, 2, 3, 
                3, 3, 3, 3, 3, 
                3, 3, 3, 3, 3, 
                3, 3, 3, 3),
            CorrectPhi = cms.untracked.bool(False)
        ),
        RelabelHits = cms.untracked.bool(False)
    )
)


process.MuonDetLayerGeometryESProducer = cms.ESProducer("MuonDetLayerGeometryESProducer")


process.MuonNumberingInitialization = cms.ESProducer("MuonNumberingInitialization")


process.ParabolicParametrizedMagneticFieldProducer = cms.ESProducer("ParametrizedMagneticFieldProducer",
    version = cms.string('Parabolic'),
    parameters = cms.PSet(

    ),
    label = cms.untracked.string('ParabolicMf')
)


process.ParametrizedMagneticFieldProducer = cms.ESProducer("ParametrizedMagneticFieldProducer",
    version = cms.string('OAE_1103l_071212'),
    parameters = cms.PSet(
        BValue = cms.string('3_8T')
    ),
    label = cms.untracked.string('parametrizedField')
)


process.RPCGeometryESModule = cms.ESProducer("RPCGeometryESModule",
    useDDD = cms.untracked.bool(True),
    compatibiltyWith11 = cms.untracked.bool(True)
)


process.SiStripRecHitMatcherESProducer = cms.ESProducer("SiStripRecHitMatcherESProducer",
    PreFilter = cms.bool(False),
    ComponentName = cms.string('StandardMatcher'),
    NSigmaInside = cms.double(3.0)
)


process.StripCPEfromTrackAngleESProducer = cms.ESProducer("StripCPEESProducer",
    ComponentType = cms.string('StripCPEfromTrackAngle'),
    ComponentName = cms.string('StripCPEfromTrackAngle'),
    parameters = cms.PSet(
        mLC_P2 = cms.double(0.3),
        mLC_P1 = cms.double(0.618),
        mLC_P0 = cms.double(-0.326),
        useLegacyError = cms.bool(True),
        mTEC_P1 = cms.double(0.471),
        mTEC_P0 = cms.double(-1.885),
        mTOB_P0 = cms.double(-1.026),
        mTOB_P1 = cms.double(0.253),
        mTIB_P0 = cms.double(-0.742),
        mTIB_P1 = cms.double(0.202),
        mTID_P0 = cms.double(-1.427),
        mTID_P1 = cms.double(0.433)
    )
)


process.TrackerRecoGeometryESProducer = cms.ESProducer("TrackerRecoGeometryESProducer")


process.TransientTrackBuilderESProducer = cms.ESProducer("TransientTrackBuilderESProducer",
    ComponentName = cms.string('TransientTrackBuilder')
)


process.VolumeBasedMagneticFieldESProducer = cms.ESProducer("VolumeBasedMagneticFieldESProducer",
    scalingVolumes = cms.vint32(14100, 14200, 17600, 17800, 17900, 
        18100, 18300, 18400, 18600, 23100, 
        23300, 23400, 23600, 23800, 23900, 
        24100, 28600, 28800, 28900, 29100, 
        29300, 29400, 29600, 28609, 28809, 
        28909, 29109, 29309, 29409, 29609, 
        28610, 28810, 28910, 29110, 29310, 
        29410, 29610, 28611, 28811, 28911, 
        29111, 29311, 29411, 29611),
    scalingFactors = cms.vdouble(1, 1, 0.994, 1.004, 1.004, 
        1.005, 1.004, 1.004, 0.994, 0.965, 
        0.958, 0.958, 0.953, 0.958, 0.958, 
        0.965, 0.918, 0.924, 0.924, 0.906, 
        0.924, 0.924, 0.918, 0.991, 0.998, 
        0.998, 0.978, 0.998, 0.998, 0.991, 
        0.991, 0.998, 0.998, 0.978, 0.998, 
        0.998, 0.991, 0.991, 0.998, 0.998, 
        0.978, 0.998, 0.998, 0.991),
    useParametrizedTrackerField = cms.bool(True),
    label = cms.untracked.string(''),
    version = cms.string('grid_1103l_090322_3_8t'),
    debugBuilder = cms.untracked.bool(False),
    paramLabel = cms.string('parametrizedField'),
    geometryVersion = cms.int32(90322),
    gridFiles = cms.VPSet(cms.PSet(
        path = cms.string('grid.[v].bin'),
        master = cms.int32(1),
        volumes = cms.string('1-312'),
        sectors = cms.string('0')
    ), 
        cms.PSet(
            path = cms.string('S3/grid.[v].bin'),
            master = cms.int32(3),
            volumes = cms.string('176-186,231-241,286-296'),
            sectors = cms.string('3')
        ), 
        cms.PSet(
            path = cms.string('S4/grid.[v].bin'),
            master = cms.int32(4),
            volumes = cms.string('176-186,231-241,286-296'),
            sectors = cms.string('4')
        ), 
        cms.PSet(
            path = cms.string('S9/grid.[v].bin'),
            master = cms.int32(9),
            volumes = cms.string('14,15,20,21,24-27,32,33,40,41,48,49,56,57,62,63,70,71,286-296'),
            sectors = cms.string('9')
        ), 
        cms.PSet(
            path = cms.string('S10/grid.[v].bin'),
            master = cms.int32(10),
            volumes = cms.string('14,15,20,21,24-27,32,33,40,41,48,49,56,57,62,63,70,71,286-296'),
            sectors = cms.string('10')
        ), 
        cms.PSet(
            path = cms.string('S11/grid.[v].bin'),
            master = cms.int32(11),
            volumes = cms.string('14,15,20,21,24-27,32,33,40,41,48,49,56,57,62,63,70,71,286-296'),
            sectors = cms.string('11')
        )),
    cacheLastVolume = cms.untracked.bool(True)
)


process.ZdcHardcodeGeometryEP = cms.ESProducer("ZdcHardcodeGeometryEP")


process.ak10PFCHSL1Fastjet = cms.ESProducer("L1FastjetCorrectionESProducer",
    srcRho = cms.InputTag("fixedGridRhoFastjetAll"),
    algorithm = cms.string('AK4PFchs'),
    level = cms.string('L1FastJet')
)


process.ak10PFCHSL1FastjetL2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak10PFCHSL1Fastjet', 
        'ak10PFCHSL2Relative', 
        'ak10PFCHSL3Absolute', 
        'ak10PFCHSResidual')
)


process.ak10PFCHSL1L2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak10PFCHSL1Offset', 
        'ak10PFCHSL2Relative', 
        'ak10PFCHSL3Absolute', 
        'ak10PFCHSResidual')
)


process.ak10PFCHSL1Offset = cms.ESProducer("L1OffsetCorrectionESProducer",
    minVtxNdof = cms.int32(4),
    vertexCollection = cms.string('offlinePrimaryVertices'),
    algorithm = cms.string('AK4PFchs'),
    level = cms.string('L1Offset')
)


process.ak10PFCHSL2L3 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak10PFCHSL2Relative', 
        'ak10PFCHSL3Absolute')
)


process.ak10PFCHSL2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak10PFCHSL2Relative', 
        'ak10PFCHSL3Absolute', 
        'ak10PFCHSResidual')
)


process.ak10PFCHSL2Relative = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK10PFchs'),
    level = cms.string('L2Relative')
)


process.ak10PFCHSL3Absolute = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK10PFchs'),
    level = cms.string('L3Absolute')
)


process.ak10PFCHSResidual = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK4PFchs'),
    level = cms.string('L2L3Residual')
)


process.ak10PFL1Fastjet = cms.ESProducer("L1FastjetCorrectionESProducer",
    srcRho = cms.InputTag("fixedGridRhoFastjetAll"),
    algorithm = cms.string('AK4PF'),
    level = cms.string('L1FastJet')
)


process.ak10PFL1FastjetL2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak10PFL1Fastjet', 
        'ak10PFL2Relative', 
        'ak10PFL3Absolute', 
        'ak10PFResidual')
)


process.ak10PFL1L2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak10PFL1Offset', 
        'ak10PFL2Relative', 
        'ak10PFL3Absolute', 
        'ak10PFResidual')
)


process.ak10PFL1Offset = cms.ESProducer("L1OffsetCorrectionESProducer",
    minVtxNdof = cms.int32(4),
    vertexCollection = cms.string('offlinePrimaryVertices'),
    algorithm = cms.string('AK4PF'),
    level = cms.string('L1Offset')
)


process.ak10PFL2L3 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak10PFL2Relative', 
        'ak10PFL3Absolute')
)


process.ak10PFL2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak10PFL2Relative', 
        'ak10PFL3Absolute', 
        'ak10PFResidual')
)


process.ak10PFL2Relative = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK10PF'),
    level = cms.string('L2Relative')
)


process.ak10PFL3Absolute = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK10PF'),
    level = cms.string('L3Absolute')
)


process.ak10PFResidual = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK4PF'),
    level = cms.string('L2L3Residual')
)


process.ak1PFCHSL1Fastjet = cms.ESProducer("L1FastjetCorrectionESProducer",
    srcRho = cms.InputTag("fixedGridRhoFastjetAll"),
    algorithm = cms.string('AK4PFchs'),
    level = cms.string('L1FastJet')
)


process.ak1PFCHSL1FastjetL2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak1PFCHSL1Fastjet', 
        'ak1PFCHSL2Relative', 
        'ak1PFCHSL3Absolute', 
        'ak1PFCHSResidual')
)


process.ak1PFCHSL1L2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak1PFCHSL1Offset', 
        'ak1PFCHSL2Relative', 
        'ak1PFCHSL3Absolute', 
        'ak1PFCHSResidual')
)


process.ak1PFCHSL1Offset = cms.ESProducer("L1OffsetCorrectionESProducer",
    minVtxNdof = cms.int32(4),
    vertexCollection = cms.string('offlinePrimaryVertices'),
    algorithm = cms.string('AK4PFchs'),
    level = cms.string('L1Offset')
)


process.ak1PFCHSL2L3 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak1PFCHSL2Relative', 
        'ak1PFCHSL3Absolute')
)


process.ak1PFCHSL2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak1PFCHSL2Relative', 
        'ak1PFCHSL3Absolute', 
        'ak1PFCHSResidual')
)


process.ak1PFCHSL2Relative = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK1PFchs'),
    level = cms.string('L2Relative')
)


process.ak1PFCHSL3Absolute = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK1PFchs'),
    level = cms.string('L3Absolute')
)


process.ak1PFCHSResidual = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK4PFchs'),
    level = cms.string('L2L3Residual')
)


process.ak1PFL1Fastjet = cms.ESProducer("L1FastjetCorrectionESProducer",
    srcRho = cms.InputTag("fixedGridRhoFastjetAll"),
    algorithm = cms.string('AK4PF'),
    level = cms.string('L1FastJet')
)


process.ak1PFL1FastjetL2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak1PFL1Fastjet', 
        'ak1PFL2Relative', 
        'ak1PFL3Absolute', 
        'ak1PFResidual')
)


process.ak1PFL1L2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak1PFL1Offset', 
        'ak1PFL2Relative', 
        'ak1PFL3Absolute', 
        'ak1PFResidual')
)


process.ak1PFL1Offset = cms.ESProducer("L1OffsetCorrectionESProducer",
    minVtxNdof = cms.int32(4),
    vertexCollection = cms.string('offlinePrimaryVertices'),
    algorithm = cms.string('AK4PF'),
    level = cms.string('L1Offset')
)


process.ak1PFL2L3 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak1PFL2Relative', 
        'ak1PFL3Absolute')
)


process.ak1PFL2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak1PFL2Relative', 
        'ak1PFL3Absolute', 
        'ak1PFResidual')
)


process.ak1PFL2Relative = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK1PF'),
    level = cms.string('L2Relative')
)


process.ak1PFL3Absolute = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK1PF'),
    level = cms.string('L3Absolute')
)


process.ak1PFResidual = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK4PF'),
    level = cms.string('L2L3Residual')
)


process.ak2PFCHSL1Fastjet = cms.ESProducer("L1FastjetCorrectionESProducer",
    srcRho = cms.InputTag("fixedGridRhoFastjetAll"),
    algorithm = cms.string('AK4PFchs'),
    level = cms.string('L1FastJet')
)


process.ak2PFCHSL1FastjetL2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak2PFCHSL1Fastjet', 
        'ak2PFCHSL2Relative', 
        'ak2PFCHSL3Absolute', 
        'ak2PFCHSResidual')
)


process.ak2PFCHSL1L2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak2PFCHSL1Offset', 
        'ak2PFCHSL2Relative', 
        'ak2PFCHSL3Absolute', 
        'ak2PFCHSResidual')
)


process.ak2PFCHSL1Offset = cms.ESProducer("L1OffsetCorrectionESProducer",
    minVtxNdof = cms.int32(4),
    vertexCollection = cms.string('offlinePrimaryVertices'),
    algorithm = cms.string('AK4PFchs'),
    level = cms.string('L1Offset')
)


process.ak2PFCHSL2L3 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak2PFCHSL2Relative', 
        'ak2PFCHSL3Absolute')
)


process.ak2PFCHSL2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak2PFCHSL2Relative', 
        'ak2PFCHSL3Absolute', 
        'ak2PFCHSResidual')
)


process.ak2PFCHSL2Relative = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK2PFchs'),
    level = cms.string('L2Relative')
)


process.ak2PFCHSL3Absolute = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK2PFchs'),
    level = cms.string('L3Absolute')
)


process.ak2PFCHSResidual = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK4PFchs'),
    level = cms.string('L2L3Residual')
)


process.ak2PFL1Fastjet = cms.ESProducer("L1FastjetCorrectionESProducer",
    srcRho = cms.InputTag("fixedGridRhoFastjetAll"),
    algorithm = cms.string('AK4PF'),
    level = cms.string('L1FastJet')
)


process.ak2PFL1FastjetL2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak2PFL1Fastjet', 
        'ak2PFL2Relative', 
        'ak2PFL3Absolute', 
        'ak2PFResidual')
)


process.ak2PFL1L2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak2PFL1Offset', 
        'ak2PFL2Relative', 
        'ak2PFL3Absolute', 
        'ak2PFResidual')
)


process.ak2PFL1Offset = cms.ESProducer("L1OffsetCorrectionESProducer",
    minVtxNdof = cms.int32(4),
    vertexCollection = cms.string('offlinePrimaryVertices'),
    algorithm = cms.string('AK4PF'),
    level = cms.string('L1Offset')
)


process.ak2PFL2L3 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak2PFL2Relative', 
        'ak2PFL3Absolute')
)


process.ak2PFL2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak2PFL2Relative', 
        'ak2PFL3Absolute', 
        'ak2PFResidual')
)


process.ak2PFL2Relative = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK2PF'),
    level = cms.string('L2Relative')
)


process.ak2PFL3Absolute = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK2PF'),
    level = cms.string('L3Absolute')
)


process.ak2PFResidual = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK4PF'),
    level = cms.string('L2L3Residual')
)


process.ak3PFCHSL1Fastjet = cms.ESProducer("L1FastjetCorrectionESProducer",
    srcRho = cms.InputTag("fixedGridRhoFastjetAll"),
    algorithm = cms.string('AK4PFchs'),
    level = cms.string('L1FastJet')
)


process.ak3PFCHSL1FastjetL2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak3PFCHSL1Fastjet', 
        'ak3PFCHSL2Relative', 
        'ak3PFCHSL3Absolute', 
        'ak3PFCHSResidual')
)


process.ak3PFCHSL1L2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak3PFCHSL1Offset', 
        'ak3PFCHSL2Relative', 
        'ak3PFCHSL3Absolute', 
        'ak3PFCHSResidual')
)


process.ak3PFCHSL1Offset = cms.ESProducer("L1OffsetCorrectionESProducer",
    minVtxNdof = cms.int32(4),
    vertexCollection = cms.string('offlinePrimaryVertices'),
    algorithm = cms.string('AK4PFchs'),
    level = cms.string('L1Offset')
)


process.ak3PFCHSL2L3 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak3PFCHSL2Relative', 
        'ak3PFCHSL3Absolute')
)


process.ak3PFCHSL2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak3PFCHSL2Relative', 
        'ak3PFCHSL3Absolute', 
        'ak3PFCHSResidual')
)


process.ak3PFCHSL2Relative = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK3PFchs'),
    level = cms.string('L2Relative')
)


process.ak3PFCHSL3Absolute = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK3PFchs'),
    level = cms.string('L3Absolute')
)


process.ak3PFCHSResidual = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK4PFchs'),
    level = cms.string('L2L3Residual')
)


process.ak3PFL1Fastjet = cms.ESProducer("L1FastjetCorrectionESProducer",
    srcRho = cms.InputTag("fixedGridRhoFastjetAll"),
    algorithm = cms.string('AK4PF'),
    level = cms.string('L1FastJet')
)


process.ak3PFL1FastjetL2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak3PFL1Fastjet', 
        'ak3PFL2Relative', 
        'ak3PFL3Absolute', 
        'ak3PFResidual')
)


process.ak3PFL1L2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak3PFL1Offset', 
        'ak3PFL2Relative', 
        'ak3PFL3Absolute', 
        'ak3PFResidual')
)


process.ak3PFL1Offset = cms.ESProducer("L1OffsetCorrectionESProducer",
    minVtxNdof = cms.int32(4),
    vertexCollection = cms.string('offlinePrimaryVertices'),
    algorithm = cms.string('AK4PF'),
    level = cms.string('L1Offset')
)


process.ak3PFL2L3 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak3PFL2Relative', 
        'ak3PFL3Absolute')
)


process.ak3PFL2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak3PFL2Relative', 
        'ak3PFL3Absolute', 
        'ak3PFResidual')
)


process.ak3PFL2Relative = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK3PF'),
    level = cms.string('L2Relative')
)


process.ak3PFL3Absolute = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK3PF'),
    level = cms.string('L3Absolute')
)


process.ak3PFResidual = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK4PF'),
    level = cms.string('L2L3Residual')
)


process.ak4CaloL1FastL2L3 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak4CaloL1Fastjet', 
        'ak4CaloL2Relative', 
        'ak4CaloL3Absolute')
)


process.ak4CaloL1FastL2L3L6 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak4CaloL1Fastjet', 
        'ak4CaloL2Relative', 
        'ak4CaloL3Absolute', 
        'ak4CaloL6SLB')
)


process.ak4CaloL1FastL2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak4CaloL1Fastjet', 
        'ak4CaloL2Relative', 
        'ak4CaloL3Absolute', 
        'ak4CaloResidual')
)


process.ak4CaloL1Fastjet = cms.ESProducer("L1FastjetCorrectionESProducer",
    srcRho = cms.InputTag("fixedGridRhoFastjetAllCalo"),
    algorithm = cms.string('AK5Calo'),
    level = cms.string('L1FastJet')
)


process.ak4CaloL1L2L3 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak4CaloL1Offset', 
        'ak4CaloL2Relative', 
        'ak4CaloL3Absolute')
)


process.ak4CaloL1L2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak4CaloL1Offset', 
        'ak4CaloL2Relative', 
        'ak4CaloL3Absolute', 
        'ak4CaloResidual')
)


process.ak4CaloL1Offset = cms.ESProducer("L1OffsetCorrectionESProducer",
    minVtxNdof = cms.int32(4),
    vertexCollection = cms.string('offlinePrimaryVertices'),
    algorithm = cms.string('AK5Calo'),
    level = cms.string('L1Offset')
)


process.ak4CaloL2L3 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak4CaloL2Relative', 
        'ak4CaloL3Absolute')
)


process.ak4CaloL2L3L6 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak4CaloL2Relative', 
        'ak4CaloL3Absolute', 
        'ak4CaloL6SLB')
)


process.ak4CaloL2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak4CaloL2Relative', 
        'ak4CaloL3Absolute', 
        'ak4CaloResidual')
)


process.ak4CaloL2Relative = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK5Calo'),
    level = cms.string('L2Relative')
)


process.ak4CaloL3Absolute = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK5Calo'),
    level = cms.string('L3Absolute')
)


process.ak4CaloL6SLB = cms.ESProducer("L6SLBCorrectionESProducer",
    srcBTagInfoElectron = cms.InputTag("ak4CaloJetsSoftElectronTagInfos"),
    srcBTagInfoMuon = cms.InputTag("ak4CaloJetsSoftMuonTagInfos"),
    addMuonToJet = cms.bool(True),
    algorithm = cms.string(''),
    level = cms.string('L6SLB')
)


process.ak4CaloResidual = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK5Calo'),
    level = cms.string('L2L3Residual')
)


process.ak4JPTL1FastL2L3 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak4JPTL1Fastjet', 
        'ak4JPTL2Relative', 
        'ak4JPTL3Absolute')
)


process.ak4JPTL1FastL2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak4JPTL1Fastjet', 
        'ak4JPTL2Relative', 
        'ak4JPTL3Absolute', 
        'ak4JPTResidual')
)


process.ak4JPTL1Fastjet = cms.ESProducer("L1FastjetCorrectionESProducer",
    srcRho = cms.InputTag("fixedGridRhoFastjetAllCalo"),
    algorithm = cms.string('AK5Calo'),
    level = cms.string('L1FastJet')
)


process.ak4JPTL1L2L3 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak4L1JPTOffset', 
        'ak4JPTL2Relative', 
        'ak4JPTL3Absolute')
)


process.ak4JPTL1L2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak4L1JPTOffset', 
        'ak4JPTL2Relative', 
        'ak4JPTL3Absolute', 
        'ak4JPTResidual')
)


process.ak4JPTL1Offset = cms.ESProducer("L1OffsetCorrectionESProducer",
    minVtxNdof = cms.int32(4),
    vertexCollection = cms.string('offlinePrimaryVertices'),
    algorithm = cms.string('AK5JPT'),
    level = cms.string('L1Offset')
)


process.ak4JPTL2L3 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak4L1JPTOffset', 
        'ak4JPTL2Relative', 
        'ak4JPTL3Absolute')
)


process.ak4JPTL2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak4L1JPTOffset', 
        'ak4JPTL2Relative', 
        'ak4JPTL3Absolute', 
        'ak4JPTResidual')
)


process.ak4JPTL2Relative = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK5JPT'),
    level = cms.string('L2Relative')
)


process.ak4JPTL3Absolute = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK5JPT'),
    level = cms.string('L3Absolute')
)


process.ak4JPTResidual = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK5JPT'),
    level = cms.string('L2L3Residual')
)


process.ak4L1JPTOffset = cms.ESProducer("L1JPTOffsetCorrectionESProducer",
    offsetService = cms.string('ak4CaloL1Offset'),
    algorithm = cms.string('AK5JPT'),
    level = cms.string('L1JPTOffset')
)


process.ak4PFCHSL1FastL2L3 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak4PFCHSL1Fastjet', 
        'ak4PFCHSL2Relative', 
        'ak4PFCHSL3Absolute')
)


process.ak4PFCHSL1FastL2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak4PFCHSL1Fastjet', 
        'ak4PFCHSL2Relative', 
        'ak4PFCHSL3Absolute', 
        'ak4PFCHSResidual')
)


process.ak4PFCHSL1Fastjet = cms.ESProducer("L1FastjetCorrectionESProducer",
    srcRho = cms.InputTag("fixedGridRhoFastjetAll"),
    algorithm = cms.string('AK4PFchs'),
    level = cms.string('L1FastJet')
)


process.ak4PFCHSL1L2L3 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak4PFCHSL1Offset', 
        'ak4PFCHSL2Relative', 
        'ak4PFCHSL3Absolute')
)


process.ak4PFCHSL1L2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak4PFCHSL1Offset', 
        'ak4PFCHSL2Relative', 
        'ak4PFCHSL3Absolute', 
        'ak4PFCHSResidual')
)


process.ak4PFCHSL1Offset = cms.ESProducer("L1OffsetCorrectionESProducer",
    minVtxNdof = cms.int32(4),
    vertexCollection = cms.string('offlinePrimaryVertices'),
    algorithm = cms.string('AK4PFchs'),
    level = cms.string('L1Offset')
)


process.ak4PFCHSL2L3 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak4PFCHSL2Relative', 
        'ak4PFCHSL3Absolute')
)


process.ak4PFCHSL2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak4PFCHSL2Relative', 
        'ak4PFCHSL3Absolute', 
        'ak4PFCHSResidual')
)


process.ak4PFCHSL2Relative = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK4PFchs'),
    level = cms.string('L2Relative')
)


process.ak4PFCHSL3Absolute = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK4PFchs'),
    level = cms.string('L3Absolute')
)


process.ak4PFCHSResidual = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK4PFchs'),
    level = cms.string('L2L3Residual')
)


process.ak4PFL1FastL2L3 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak4PFL1Fastjet', 
        'ak4PFL2Relative', 
        'ak4PFL3Absolute')
)


process.ak4PFL1FastL2L3L6 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak4PFL1Fastjet', 
        'ak4PFL2Relative', 
        'ak4PFL3Absolute', 
        'ak4PFL6SLB')
)


process.ak4PFL1FastL2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak4PFL1Fastjet', 
        'ak4PFL2Relative', 
        'ak4PFL3Absolute', 
        'ak4PFResidual')
)


process.ak4PFL1Fastjet = cms.ESProducer("L1FastjetCorrectionESProducer",
    srcRho = cms.InputTag("fixedGridRhoFastjetAll"),
    algorithm = cms.string('AK4PF'),
    level = cms.string('L1FastJet')
)


process.ak4PFL1L2L3 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak4PFL1Offset', 
        'ak4PFL2Relative', 
        'ak4PFL3Absolute')
)


process.ak4PFL1L2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak4PFL1Offset', 
        'ak4PFL2Relative', 
        'ak4PFL3Absolute', 
        'ak4PFResidual')
)


process.ak4PFL1Offset = cms.ESProducer("L1OffsetCorrectionESProducer",
    minVtxNdof = cms.int32(4),
    vertexCollection = cms.string('offlinePrimaryVertices'),
    algorithm = cms.string('AK4PF'),
    level = cms.string('L1Offset')
)


process.ak4PFL2L3 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak4PFL2Relative', 
        'ak4PFL3Absolute')
)


process.ak4PFL2L3L6 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak4PFL2Relative', 
        'ak4PFL3Absolute', 
        'ak4PFL6SLB')
)


process.ak4PFL2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak4PFL2Relative', 
        'ak4PFL3Absolute', 
        'ak4PFResidual')
)


process.ak4PFL2Relative = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK4PF'),
    level = cms.string('L2Relative')
)


process.ak4PFL3Absolute = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK4PF'),
    level = cms.string('L3Absolute')
)


process.ak4PFL6SLB = cms.ESProducer("L6SLBCorrectionESProducer",
    srcBTagInfoElectron = cms.InputTag("ak4PFJetsSoftElectronTagInfos"),
    srcBTagInfoMuon = cms.InputTag("ak4PFJetsSoftMuonTagInfos"),
    addMuonToJet = cms.bool(False),
    algorithm = cms.string(''),
    level = cms.string('L6SLB')
)


process.ak4PFResidual = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK4PF'),
    level = cms.string('L2L3Residual')
)


process.ak4TrackL1FastL2L3 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak4CaloL1Fastjet', 
        'ak4TrackL2Relative', 
        'ak4TrackL3Absolute')
)


process.ak4TrackL2L3 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak4TrackL2Relative', 
        'ak4TrackL3Absolute')
)


process.ak4TrackL2Relative = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK5TRK'),
    level = cms.string('L2Relative')
)


process.ak4TrackL3Absolute = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK5TRK'),
    level = cms.string('L3Absolute')
)


process.ak5PFCHSL1Fastjet = cms.ESProducer("L1FastjetCorrectionESProducer",
    srcRho = cms.InputTag("fixedGridRhoFastjetAll"),
    algorithm = cms.string('AK4PFchs'),
    level = cms.string('L1FastJet')
)


process.ak5PFCHSL1FastjetL2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak5PFCHSL1Fastjet', 
        'ak5PFCHSL2Relative', 
        'ak5PFCHSL3Absolute', 
        'ak5PFCHSResidual')
)


process.ak5PFCHSL1L2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak5PFCHSL1Offset', 
        'ak5PFCHSL2Relative', 
        'ak5PFCHSL3Absolute', 
        'ak5PFCHSResidual')
)


process.ak5PFCHSL1Offset = cms.ESProducer("L1OffsetCorrectionESProducer",
    minVtxNdof = cms.int32(4),
    vertexCollection = cms.string('offlinePrimaryVertices'),
    algorithm = cms.string('AK4PFchs'),
    level = cms.string('L1Offset')
)


process.ak5PFCHSL2L3 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak5PFCHSL2Relative', 
        'ak5PFCHSL3Absolute')
)


process.ak5PFCHSL2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak5PFCHSL2Relative', 
        'ak5PFCHSL3Absolute', 
        'ak5PFCHSResidual')
)


process.ak5PFCHSL2Relative = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK5PFchs'),
    level = cms.string('L2Relative')
)


process.ak5PFCHSL3Absolute = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK5PFchs'),
    level = cms.string('L3Absolute')
)


process.ak5PFCHSResidual = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK4PFchs'),
    level = cms.string('L2L3Residual')
)


process.ak5PFL1Fastjet = cms.ESProducer("L1FastjetCorrectionESProducer",
    srcRho = cms.InputTag("fixedGridRhoFastjetAll"),
    algorithm = cms.string('AK4PF'),
    level = cms.string('L1FastJet')
)


process.ak5PFL1FastjetL2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak5PFL1Fastjet', 
        'ak5PFL2Relative', 
        'ak5PFL3Absolute', 
        'ak5PFResidual')
)


process.ak5PFL1L2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak5PFL1Offset', 
        'ak5PFL2Relative', 
        'ak5PFL3Absolute', 
        'ak5PFResidual')
)


process.ak5PFL1Offset = cms.ESProducer("L1OffsetCorrectionESProducer",
    minVtxNdof = cms.int32(4),
    vertexCollection = cms.string('offlinePrimaryVertices'),
    algorithm = cms.string('AK4PF'),
    level = cms.string('L1Offset')
)


process.ak5PFL2L3 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak5PFL2Relative', 
        'ak5PFL3Absolute')
)


process.ak5PFL2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak5PFL2Relative', 
        'ak5PFL3Absolute', 
        'ak5PFResidual')
)


process.ak5PFL2Relative = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK5PF'),
    level = cms.string('L2Relative')
)


process.ak5PFL3Absolute = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK5PF'),
    level = cms.string('L3Absolute')
)


process.ak5PFResidual = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK4PF'),
    level = cms.string('L2L3Residual')
)


process.ak6PFCHSL1Fastjet = cms.ESProducer("L1FastjetCorrectionESProducer",
    srcRho = cms.InputTag("fixedGridRhoFastjetAll"),
    algorithm = cms.string('AK4PFchs'),
    level = cms.string('L1FastJet')
)


process.ak6PFCHSL1FastjetL2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak6PFCHSL1Fastjet', 
        'ak6PFCHSL2Relative', 
        'ak6PFCHSL3Absolute', 
        'ak6PFCHSResidual')
)


process.ak6PFCHSL1L2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak6PFCHSL1Offset', 
        'ak6PFCHSL2Relative', 
        'ak6PFCHSL3Absolute', 
        'ak6PFCHSResidual')
)


process.ak6PFCHSL1Offset = cms.ESProducer("L1OffsetCorrectionESProducer",
    minVtxNdof = cms.int32(4),
    vertexCollection = cms.string('offlinePrimaryVertices'),
    algorithm = cms.string('AK4PFchs'),
    level = cms.string('L1Offset')
)


process.ak6PFCHSL2L3 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak6PFCHSL2Relative', 
        'ak6PFCHSL3Absolute')
)


process.ak6PFCHSL2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak6PFCHSL2Relative', 
        'ak6PFCHSL3Absolute', 
        'ak6PFCHSResidual')
)


process.ak6PFCHSL2Relative = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK6PFchs'),
    level = cms.string('L2Relative')
)


process.ak6PFCHSL3Absolute = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK6PFchs'),
    level = cms.string('L3Absolute')
)


process.ak6PFCHSResidual = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK4PFchs'),
    level = cms.string('L2L3Residual')
)


process.ak6PFL1Fastjet = cms.ESProducer("L1FastjetCorrectionESProducer",
    srcRho = cms.InputTag("fixedGridRhoFastjetAll"),
    algorithm = cms.string('AK4PF'),
    level = cms.string('L1FastJet')
)


process.ak6PFL1FastjetL2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak6PFL1Fastjet', 
        'ak6PFL2Relative', 
        'ak6PFL3Absolute', 
        'ak6PFResidual')
)


process.ak6PFL1L2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak6PFL1Offset', 
        'ak6PFL2Relative', 
        'ak6PFL3Absolute', 
        'ak6PFResidual')
)


process.ak6PFL1Offset = cms.ESProducer("L1OffsetCorrectionESProducer",
    minVtxNdof = cms.int32(4),
    vertexCollection = cms.string('offlinePrimaryVertices'),
    algorithm = cms.string('AK4PF'),
    level = cms.string('L1Offset')
)


process.ak6PFL2L3 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak6PFL2Relative', 
        'ak6PFL3Absolute')
)


process.ak6PFL2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak6PFL2Relative', 
        'ak6PFL3Absolute', 
        'ak6PFResidual')
)


process.ak6PFL2Relative = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK6PF'),
    level = cms.string('L2Relative')
)


process.ak6PFL3Absolute = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK6PF'),
    level = cms.string('L3Absolute')
)


process.ak6PFResidual = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK4PF'),
    level = cms.string('L2L3Residual')
)


process.ak7CaloL1FastL2L3 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak4CaloL1Fastjet', 
        'ak7CaloL2Relative', 
        'ak7CaloL3Absolute')
)


process.ak7CaloL1FastL2L3L6 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak7CaloL1Offset', 
        'ak7CaloL2Relative', 
        'ak7CaloL3Absolute', 
        'ak7CaloL6SLB')
)


process.ak7CaloL1FastL2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak7CaloL1Fastjet', 
        'ak7CaloL2Relative', 
        'ak7CaloL3Absolute', 
        'ak7CaloResidual')
)


process.ak7CaloL1Fastjet = cms.ESProducer("L1FastjetCorrectionESProducer",
    srcRho = cms.InputTag("fixedGridRhoFastjetAllCalo"),
    algorithm = cms.string('AK5Calo'),
    level = cms.string('L1FastJet')
)


process.ak7CaloL1L2L3 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak7CaloL1Offset', 
        'ak7CaloL2Relative', 
        'ak7CaloL3Absolute')
)


process.ak7CaloL1L2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak7CaloL1Offset', 
        'ak7CaloL2Relative', 
        'ak7CaloL3Absolute', 
        'ak7CaloResidual')
)


process.ak7CaloL1Offset = cms.ESProducer("L1OffsetCorrectionESProducer",
    minVtxNdof = cms.int32(4),
    vertexCollection = cms.string('offlinePrimaryVertices'),
    algorithm = cms.string('AK5Calo'),
    level = cms.string('L1Offset')
)


process.ak7CaloL2L3 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak7CaloL2Relative', 
        'ak7CaloL3Absolute')
)


process.ak7CaloL2L3L6 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak7CaloL2Relative', 
        'ak7CaloL3Absolute', 
        'ak7CaloL6SLB')
)


process.ak7CaloL2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak7CaloL2Relative', 
        'ak7CaloL3Absolute', 
        'ak7CaloResidual')
)


process.ak7CaloL2Relative = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK7Calo'),
    level = cms.string('L2Relative')
)


process.ak7CaloL3Absolute = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK7Calo'),
    level = cms.string('L3Absolute')
)


process.ak7CaloL6SLB = cms.ESProducer("L6SLBCorrectionESProducer",
    srcBTagInfoElectron = cms.InputTag("ak7CaloJetsSoftElectronTagInfos"),
    srcBTagInfoMuon = cms.InputTag("ak7CaloJetsSoftMuonTagInfos"),
    addMuonToJet = cms.bool(True),
    algorithm = cms.string(''),
    level = cms.string('L6SLB')
)


process.ak7CaloResidual = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK5Calo'),
    level = cms.string('L2L3Residual')
)


process.ak7JPTL1FastL2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak7JPTL1Fastjet', 
        'ak7L1JPTOffset', 
        'ak7JPTL2Relative', 
        'ak7JPTL3Absolute', 
        'ak7JPTResidual')
)


process.ak7JPTL1Fastjet = cms.ESProducer("L1FastjetCorrectionESProducer",
    srcRho = cms.InputTag("fixedGridRhoFastjetAllCalo"),
    algorithm = cms.string('AK5Calo'),
    level = cms.string('L1FastJet')
)


process.ak7JPTL1L2L3 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak7JPTL1Offset', 
        'ak7L1JPTOffset', 
        'ak7JPTL2Relative', 
        'ak7JPTL3Absolute')
)


process.ak7JPTL1L2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak7JPTL1Offset', 
        'ak7L1JPTOffset', 
        'ak7JPTL2Relative', 
        'ak7JPTL3Absolute', 
        'ak7JPTResidual')
)


process.ak7JPTL1Offset = cms.ESProducer("L1OffsetCorrectionESProducer",
    minVtxNdof = cms.int32(4),
    vertexCollection = cms.string('offlinePrimaryVertices'),
    algorithm = cms.string('AK5Calo'),
    level = cms.string('L1Offset')
)


process.ak7JPTL2L3 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak7L1JPTOffset', 
        'ak7JPTL2Relative', 
        'ak7JPTL3Absolute')
)


process.ak7L1JPTOffset = cms.ESProducer("L1JPTOffsetCorrectionESProducer",
    offsetService = cms.string('ak4CaloL1Offset'),
    algorithm = cms.string('AK5JPT'),
    level = cms.string('L1JPTOffset')
)


process.ak7PFCHSL1FastL2L3 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak4PFCHSL1Fastjet', 
        'ak7PFCHSL2Relative', 
        'ak7PFCHSL3Absolute')
)


process.ak7PFCHSL1Fastjet = cms.ESProducer("L1FastjetCorrectionESProducer",
    srcRho = cms.InputTag("fixedGridRhoFastjetAll"),
    algorithm = cms.string('AK4PFchs'),
    level = cms.string('L1FastJet')
)


process.ak7PFCHSL1FastjetL2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak7PFCHSL1Fastjet', 
        'ak7PFCHSL2Relative', 
        'ak7PFCHSL3Absolute', 
        'ak7PFCHSResidual')
)


process.ak7PFCHSL1L2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak7PFCHSL1Offset', 
        'ak7PFCHSL2Relative', 
        'ak7PFCHSL3Absolute', 
        'ak7PFCHSResidual')
)


process.ak7PFCHSL1Offset = cms.ESProducer("L1OffsetCorrectionESProducer",
    minVtxNdof = cms.int32(4),
    vertexCollection = cms.string('offlinePrimaryVertices'),
    algorithm = cms.string('AK4PFchs'),
    level = cms.string('L1Offset')
)


process.ak7PFCHSL2L3 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak7PFCHSL2Relative', 
        'ak7PFCHSL3Absolute')
)


process.ak7PFCHSL2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak7PFCHSL2Relative', 
        'ak7PFCHSL3Absolute', 
        'ak7PFCHSResidual')
)


process.ak7PFCHSL2Relative = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK7PFchs'),
    level = cms.string('L2Relative')
)


process.ak7PFCHSL3Absolute = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK7PFchs'),
    level = cms.string('L3Absolute')
)


process.ak7PFCHSResidual = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK4PFchs'),
    level = cms.string('L2L3Residual')
)


process.ak7PFL1FastL2L3 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak4PFL1Fastjet', 
        'ak7PFL2Relative', 
        'ak7PFL3Absolute')
)


process.ak7PFL1FastL2L3L6 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak4PFL1Fastjet', 
        'ak7PFL2Relative', 
        'ak7PFL3Absolute', 
        'ak7PFL6SLB')
)


process.ak7PFL1Fastjet = cms.ESProducer("L1FastjetCorrectionESProducer",
    srcRho = cms.InputTag("fixedGridRhoFastjetAll"),
    algorithm = cms.string('AK4PF'),
    level = cms.string('L1FastJet')
)


process.ak7PFL1FastjetL2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak7PFL1Fastjet', 
        'ak7PFL2Relative', 
        'ak7PFL3Absolute', 
        'ak7PFResidual')
)


process.ak7PFL1L2L3 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak7PFL1Offset', 
        'ak7PFL2Relative', 
        'ak7PFL3Absolute')
)


process.ak7PFL1L2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak7PFL1Offset', 
        'ak7PFL2Relative', 
        'ak7PFL3Absolute', 
        'ak7PFResidual')
)


process.ak7PFL1Offset = cms.ESProducer("L1OffsetCorrectionESProducer",
    minVtxNdof = cms.int32(4),
    vertexCollection = cms.string('offlinePrimaryVertices'),
    algorithm = cms.string('AK4PF'),
    level = cms.string('L1Offset')
)


process.ak7PFL2L3 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak7PFL2Relative', 
        'ak7PFL3Absolute')
)


process.ak7PFL2L3L6 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak7PFL2Relative', 
        'ak7PFL3Absolute', 
        'ak7PFL6SLB')
)


process.ak7PFL2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak7PFL2Relative', 
        'ak7PFL3Absolute', 
        'ak7PFResidual')
)


process.ak7PFL2Relative = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK7PF'),
    level = cms.string('L2Relative')
)


process.ak7PFL3Absolute = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK7PF'),
    level = cms.string('L3Absolute')
)


process.ak7PFL6SLB = cms.ESProducer("L6SLBCorrectionESProducer",
    srcBTagInfoElectron = cms.InputTag("ak7PFJetsSoftElectronTagInfos"),
    srcBTagInfoMuon = cms.InputTag("ak7PFJetsSoftMuonTagInfos"),
    addMuonToJet = cms.bool(False),
    algorithm = cms.string(''),
    level = cms.string('L6SLB')
)


process.ak7PFResidual = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK4PF'),
    level = cms.string('L2L3Residual')
)


process.ak8PFCHSL1Fastjet = cms.ESProducer("L1FastjetCorrectionESProducer",
    srcRho = cms.InputTag("fixedGridRhoFastjetAll"),
    algorithm = cms.string('AK4PFchs'),
    level = cms.string('L1FastJet')
)


process.ak8PFCHSL1FastjetL2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak8PFCHSL1Fastjet', 
        'ak8PFCHSL2Relative', 
        'ak8PFCHSL3Absolute', 
        'ak8PFCHSResidual')
)


process.ak8PFCHSL1L2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak8PFCHSL1Offset', 
        'ak8PFCHSL2Relative', 
        'ak8PFCHSL3Absolute', 
        'ak8PFCHSResidual')
)


process.ak8PFCHSL1Offset = cms.ESProducer("L1OffsetCorrectionESProducer",
    minVtxNdof = cms.int32(4),
    vertexCollection = cms.string('offlinePrimaryVertices'),
    algorithm = cms.string('AK4PFchs'),
    level = cms.string('L1Offset')
)


process.ak8PFCHSL2L3 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak8PFCHSL2Relative', 
        'ak8PFCHSL3Absolute')
)


process.ak8PFCHSL2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak8PFCHSL2Relative', 
        'ak8PFCHSL3Absolute', 
        'ak8PFCHSResidual')
)


process.ak8PFCHSL2Relative = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK8PFchs'),
    level = cms.string('L2Relative')
)


process.ak8PFCHSL3Absolute = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK8PFchs'),
    level = cms.string('L3Absolute')
)


process.ak8PFCHSResidual = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK4PFchs'),
    level = cms.string('L2L3Residual')
)


process.ak8PFL1Fastjet = cms.ESProducer("L1FastjetCorrectionESProducer",
    srcRho = cms.InputTag("fixedGridRhoFastjetAll"),
    algorithm = cms.string('AK4PF'),
    level = cms.string('L1FastJet')
)


process.ak8PFL1FastjetL2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak8PFL1Fastjet', 
        'ak8PFL2Relative', 
        'ak8PFL3Absolute', 
        'ak8PFResidual')
)


process.ak8PFL1L2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak8PFL1Offset', 
        'ak8PFL2Relative', 
        'ak8PFL3Absolute', 
        'ak8PFResidual')
)


process.ak8PFL1Offset = cms.ESProducer("L1OffsetCorrectionESProducer",
    minVtxNdof = cms.int32(4),
    vertexCollection = cms.string('offlinePrimaryVertices'),
    algorithm = cms.string('AK4PF'),
    level = cms.string('L1Offset')
)


process.ak8PFL2L3 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak8PFL2Relative', 
        'ak8PFL3Absolute')
)


process.ak8PFL2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak8PFL2Relative', 
        'ak8PFL3Absolute', 
        'ak8PFResidual')
)


process.ak8PFL2Relative = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK8PF'),
    level = cms.string('L2Relative')
)


process.ak8PFL3Absolute = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK8PF'),
    level = cms.string('L3Absolute')
)


process.ak8PFResidual = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK4PF'),
    level = cms.string('L2L3Residual')
)


process.ak9PFCHSL1Fastjet = cms.ESProducer("L1FastjetCorrectionESProducer",
    srcRho = cms.InputTag("fixedGridRhoFastjetAll"),
    algorithm = cms.string('AK4PFchs'),
    level = cms.string('L1FastJet')
)


process.ak9PFCHSL1FastjetL2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak9PFCHSL1Fastjet', 
        'ak9PFCHSL2Relative', 
        'ak9PFCHSL3Absolute', 
        'ak9PFCHSResidual')
)


process.ak9PFCHSL1L2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak9PFCHSL1Offset', 
        'ak9PFCHSL2Relative', 
        'ak9PFCHSL3Absolute', 
        'ak9PFCHSResidual')
)


process.ak9PFCHSL1Offset = cms.ESProducer("L1OffsetCorrectionESProducer",
    minVtxNdof = cms.int32(4),
    vertexCollection = cms.string('offlinePrimaryVertices'),
    algorithm = cms.string('AK4PFchs'),
    level = cms.string('L1Offset')
)


process.ak9PFCHSL2L3 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak9PFCHSL2Relative', 
        'ak9PFCHSL3Absolute')
)


process.ak9PFCHSL2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak9PFCHSL2Relative', 
        'ak9PFCHSL3Absolute', 
        'ak9PFCHSResidual')
)


process.ak9PFCHSL2Relative = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK9PFchs'),
    level = cms.string('L2Relative')
)


process.ak9PFCHSL3Absolute = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK9PFchs'),
    level = cms.string('L3Absolute')
)


process.ak9PFCHSResidual = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK4PFchs'),
    level = cms.string('L2L3Residual')
)


process.ak9PFL1Fastjet = cms.ESProducer("L1FastjetCorrectionESProducer",
    srcRho = cms.InputTag("fixedGridRhoFastjetAll"),
    algorithm = cms.string('AK4PF'),
    level = cms.string('L1FastJet')
)


process.ak9PFL1FastjetL2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak9PFL1Fastjet', 
        'ak9PFL2Relative', 
        'ak9PFL3Absolute', 
        'ak9PFResidual')
)


process.ak9PFL1L2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak9PFL1Offset', 
        'ak9PFL2Relative', 
        'ak9PFL3Absolute', 
        'ak9PFResidual')
)


process.ak9PFL1Offset = cms.ESProducer("L1OffsetCorrectionESProducer",
    minVtxNdof = cms.int32(4),
    vertexCollection = cms.string('offlinePrimaryVertices'),
    algorithm = cms.string('AK4PF'),
    level = cms.string('L1Offset')
)


process.ak9PFL2L3 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak9PFL2Relative', 
        'ak9PFL3Absolute')
)


process.ak9PFL2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak9PFL2Relative', 
        'ak9PFL3Absolute', 
        'ak9PFResidual')
)


process.ak9PFL2Relative = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK9PF'),
    level = cms.string('L2Relative')
)


process.ak9PFL3Absolute = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK9PF'),
    level = cms.string('L3Absolute')
)


process.ak9PFResidual = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK4PF'),
    level = cms.string('L2L3Residual')
)


process.candidateCombinedSecondaryVertex = cms.ESProducer("CandidateCombinedSecondaryVertexESProducer",
    useTrackWeights = cms.bool(True),
    pseudoMultiplicityMin = cms.uint32(2),
    correctVertexMass = cms.bool(True),
    trackSelection = cms.PSet(
        b_pT = cms.double(0.3684),
        a_dR = cms.double(-0.001053),
        min_pT = cms.double(120),
        max_pT = cms.double(500),
        min_pT_dRcut = cms.double(0.5),
        max_pT_trackPTcut = cms.double(3),
        max_pT_dRcut = cms.double(0.1),
        b_dR = cms.double(0.6263),
        a_pT = cms.double(0.005263),
        totalHitsMin = cms.uint32(0),
        jetDeltaRMax = cms.double(0.3),
        qualityClass = cms.string('any'),
        pixelHitsMin = cms.uint32(0),
        sip3dSigMin = cms.double(-99999.9),
        sip3dSigMax = cms.double(99999.9),
        maxDistToAxis = cms.double(0.07),
        useVariableJTA = cms.bool(False),
        sip2dValMax = cms.double(99999.9),
        maxDecayLen = cms.double(5),
        ptMin = cms.double(0.0),
        sip2dSigMax = cms.double(99999.9),
        sip2dSigMin = cms.double(-99999.9),
        sip3dValMax = cms.double(99999.9),
        sip3dValMin = cms.double(-99999.9),
        sip2dValMin = cms.double(-99999.9),
        normChi2Max = cms.double(99999.9)
    ),
    pseudoVertexV0Filter = cms.PSet(
        k0sMassWindow = cms.double(0.05)
    ),
    charmCut = cms.double(1.5),
    vertexFlip = cms.bool(False),
    minimumTrackWeight = cms.double(0.5),
    trackPairV0Filter = cms.PSet(
        k0sMassWindow = cms.double(0.03)
    ),
    trackMultiplicityMin = cms.uint32(2),
    trackPseudoSelection = cms.PSet(
        b_pT = cms.double(0.3684),
        a_dR = cms.double(-0.001053),
        min_pT = cms.double(120),
        max_pT = cms.double(500),
        min_pT_dRcut = cms.double(0.5),
        max_pT_trackPTcut = cms.double(3),
        max_pT_dRcut = cms.double(0.1),
        b_dR = cms.double(0.6263),
        a_pT = cms.double(0.005263),
        totalHitsMin = cms.uint32(0),
        jetDeltaRMax = cms.double(0.3),
        qualityClass = cms.string('any'),
        pixelHitsMin = cms.uint32(0),
        sip3dSigMin = cms.double(-99999.9),
        sip3dSigMax = cms.double(99999.9),
        maxDistToAxis = cms.double(0.07),
        useVariableJTA = cms.bool(False),
        sip2dValMax = cms.double(99999.9),
        maxDecayLen = cms.double(5),
        ptMin = cms.double(0.0),
        sip2dSigMax = cms.double(99999.9),
        sip2dSigMin = cms.double(2.0),
        sip3dValMax = cms.double(99999.9),
        sip3dValMin = cms.double(-99999.9),
        sip2dValMin = cms.double(-99999.9),
        normChi2Max = cms.double(99999.9)
    ),
    trackSort = cms.string('sip2dSig'),
    trackFlip = cms.bool(False),
    calibrationRecords = cms.vstring('CombinedSVRecoVertex', 
        'CombinedSVPseudoVertex', 
        'CombinedSVNoVertex'),
    useCategories = cms.bool(True),
    categoryVariableName = cms.string('vertexCategory')
)


process.combinedMVA = cms.ESProducer("CombinedMVAJetTagESProducer",
    useCategories = cms.bool(False),
    calibrationRecord = cms.string('CombinedMVA'),
    jetTagComputers = cms.VPSet(cms.PSet(
        discriminator = cms.bool(True),
        variables = cms.bool(False),
        jetTagComputer = cms.string('jetProbability')
    ), 
        cms.PSet(
            discriminator = cms.bool(True),
            variables = cms.bool(False),
            jetTagComputer = cms.string('combinedSecondaryVertex')
        ), 
        cms.PSet(
            discriminator = cms.bool(True),
            variables = cms.bool(False),
            jetTagComputer = cms.string('softPFMuon')
        ), 
        cms.PSet(
            discriminator = cms.bool(True),
            variables = cms.bool(False),
            jetTagComputer = cms.string('softPFElectron')
        ))
)


process.combinedSecondaryVertex = cms.ESProducer("CombinedSecondaryVertexESProducer",
    useTrackWeights = cms.bool(True),
    pseudoMultiplicityMin = cms.uint32(2),
    correctVertexMass = cms.bool(True),
    trackSelection = cms.PSet(
        b_pT = cms.double(0.3684),
        a_dR = cms.double(-0.001053),
        min_pT = cms.double(120),
        max_pT = cms.double(500),
        min_pT_dRcut = cms.double(0.5),
        max_pT_trackPTcut = cms.double(3),
        max_pT_dRcut = cms.double(0.1),
        b_dR = cms.double(0.6263),
        a_pT = cms.double(0.005263),
        totalHitsMin = cms.uint32(0),
        jetDeltaRMax = cms.double(0.3),
        qualityClass = cms.string('any'),
        pixelHitsMin = cms.uint32(0),
        sip3dSigMin = cms.double(-99999.9),
        sip3dSigMax = cms.double(99999.9),
        maxDistToAxis = cms.double(0.07),
        useVariableJTA = cms.bool(False),
        sip2dValMax = cms.double(99999.9),
        maxDecayLen = cms.double(5),
        ptMin = cms.double(0.0),
        sip2dSigMax = cms.double(99999.9),
        sip2dSigMin = cms.double(-99999.9),
        sip3dValMax = cms.double(99999.9),
        sip3dValMin = cms.double(-99999.9),
        sip2dValMin = cms.double(-99999.9),
        normChi2Max = cms.double(99999.9)
    ),
    pseudoVertexV0Filter = cms.PSet(
        k0sMassWindow = cms.double(0.05)
    ),
    charmCut = cms.double(1.5),
    vertexFlip = cms.bool(False),
    minimumTrackWeight = cms.double(0.5),
    trackPairV0Filter = cms.PSet(
        k0sMassWindow = cms.double(0.03)
    ),
    trackMultiplicityMin = cms.uint32(2),
    trackPseudoSelection = cms.PSet(
        b_pT = cms.double(0.3684),
        a_dR = cms.double(-0.001053),
        min_pT = cms.double(120),
        max_pT = cms.double(500),
        min_pT_dRcut = cms.double(0.5),
        max_pT_trackPTcut = cms.double(3),
        max_pT_dRcut = cms.double(0.1),
        b_dR = cms.double(0.6263),
        a_pT = cms.double(0.005263),
        totalHitsMin = cms.uint32(0),
        jetDeltaRMax = cms.double(0.3),
        qualityClass = cms.string('any'),
        pixelHitsMin = cms.uint32(0),
        sip3dSigMin = cms.double(-99999.9),
        sip3dSigMax = cms.double(99999.9),
        maxDistToAxis = cms.double(0.07),
        useVariableJTA = cms.bool(False),
        sip2dValMax = cms.double(99999.9),
        maxDecayLen = cms.double(5),
        ptMin = cms.double(0.0),
        sip2dSigMax = cms.double(99999.9),
        sip2dSigMin = cms.double(2.0),
        sip3dValMax = cms.double(99999.9),
        sip3dValMin = cms.double(-99999.9),
        sip2dValMin = cms.double(-99999.9),
        normChi2Max = cms.double(99999.9)
    ),
    trackSort = cms.string('sip2dSig'),
    trackFlip = cms.bool(False),
    calibrationRecords = cms.vstring('CombinedSVRecoVertex', 
        'CombinedSVPseudoVertex', 
        'CombinedSVNoVertex'),
    useCategories = cms.bool(True),
    categoryVariableName = cms.string('vertexCategory')
)


process.combinedSecondaryVertexMVA = cms.ESProducer("CombinedSecondaryVertexESProducer",
    useTrackWeights = cms.bool(True),
    pseudoMultiplicityMin = cms.uint32(2),
    correctVertexMass = cms.bool(True),
    trackSelection = cms.PSet(
        b_pT = cms.double(0.3684),
        a_dR = cms.double(-0.001053),
        min_pT = cms.double(120),
        max_pT = cms.double(500),
        min_pT_dRcut = cms.double(0.5),
        max_pT_trackPTcut = cms.double(3),
        max_pT_dRcut = cms.double(0.1),
        b_dR = cms.double(0.6263),
        a_pT = cms.double(0.005263),
        totalHitsMin = cms.uint32(0),
        jetDeltaRMax = cms.double(0.3),
        qualityClass = cms.string('any'),
        pixelHitsMin = cms.uint32(0),
        sip3dSigMin = cms.double(-99999.9),
        sip3dSigMax = cms.double(99999.9),
        maxDistToAxis = cms.double(0.07),
        useVariableJTA = cms.bool(False),
        sip2dValMax = cms.double(99999.9),
        maxDecayLen = cms.double(5),
        ptMin = cms.double(0.0),
        sip2dSigMax = cms.double(99999.9),
        sip2dSigMin = cms.double(-99999.9),
        sip3dValMax = cms.double(99999.9),
        sip3dValMin = cms.double(-99999.9),
        sip2dValMin = cms.double(-99999.9),
        normChi2Max = cms.double(99999.9)
    ),
    pseudoVertexV0Filter = cms.PSet(
        k0sMassWindow = cms.double(0.05)
    ),
    charmCut = cms.double(1.5),
    vertexFlip = cms.bool(False),
    minimumTrackWeight = cms.double(0.5),
    trackPairV0Filter = cms.PSet(
        k0sMassWindow = cms.double(0.03)
    ),
    trackMultiplicityMin = cms.uint32(2),
    trackPseudoSelection = cms.PSet(
        b_pT = cms.double(0.3684),
        a_dR = cms.double(-0.001053),
        min_pT = cms.double(120),
        max_pT = cms.double(500),
        min_pT_dRcut = cms.double(0.5),
        max_pT_trackPTcut = cms.double(3),
        max_pT_dRcut = cms.double(0.1),
        b_dR = cms.double(0.6263),
        a_pT = cms.double(0.005263),
        totalHitsMin = cms.uint32(0),
        jetDeltaRMax = cms.double(0.3),
        qualityClass = cms.string('any'),
        pixelHitsMin = cms.uint32(0),
        sip3dSigMin = cms.double(-99999.9),
        sip3dSigMax = cms.double(99999.9),
        maxDistToAxis = cms.double(0.07),
        useVariableJTA = cms.bool(False),
        sip2dValMax = cms.double(99999.9),
        maxDecayLen = cms.double(5),
        ptMin = cms.double(0.0),
        sip2dSigMax = cms.double(99999.9),
        sip2dSigMin = cms.double(2.0),
        sip3dValMax = cms.double(99999.9),
        sip3dValMin = cms.double(-99999.9),
        sip2dValMin = cms.double(-99999.9),
        normChi2Max = cms.double(99999.9)
    ),
    trackSort = cms.string('sip2dSig'),
    trackFlip = cms.bool(False),
    calibrationRecords = cms.vstring('CombinedSVMVARecoVertex', 
        'CombinedSVMVAPseudoVertex', 
        'CombinedSVMVANoVertex'),
    useCategories = cms.bool(True),
    categoryVariableName = cms.string('vertexCategory')
)


process.combinedSecondaryVertexNegative = cms.ESProducer("CombinedSecondaryVertexESProducer",
    charmCut = cms.double(1.5),
    useTrackWeights = cms.bool(True),
    useCategories = cms.bool(True),
    pseudoMultiplicityMin = cms.uint32(2),
    categoryVariableName = cms.string('vertexCategory'),
    trackSelection = cms.PSet(
        b_pT = cms.double(0.3684),
        a_dR = cms.double(-0.001053),
        min_pT = cms.double(120),
        max_pT = cms.double(500),
        min_pT_dRcut = cms.double(0.5),
        max_pT_trackPTcut = cms.double(3),
        max_pT_dRcut = cms.double(0.1),
        b_dR = cms.double(0.6263),
        a_pT = cms.double(0.005263),
        totalHitsMin = cms.uint32(0),
        jetDeltaRMax = cms.double(0.3),
        qualityClass = cms.string('any'),
        pixelHitsMin = cms.uint32(0),
        sip3dSigMin = cms.double(-99999.9),
        sip3dSigMax = cms.double(0),
        maxDistToAxis = cms.double(0.07),
        useVariableJTA = cms.bool(False),
        sip2dValMax = cms.double(99999.9),
        maxDecayLen = cms.double(5),
        ptMin = cms.double(0.0),
        sip2dSigMax = cms.double(99999.9),
        sip2dSigMin = cms.double(-99999.9),
        sip3dValMax = cms.double(99999.9),
        sip3dValMin = cms.double(-99999.9),
        sip2dValMin = cms.double(-99999.9),
        normChi2Max = cms.double(99999.9)
    ),
    calibrationRecords = cms.vstring('CombinedSVRecoVertex', 
        'CombinedSVPseudoVertex', 
        'CombinedSVNoVertex'),
    pseudoVertexV0Filter = cms.PSet(
        k0sMassWindow = cms.double(0.05)
    ),
    correctVertexMass = cms.bool(True),
    vertexFlip = cms.bool(True),
    minimumTrackWeight = cms.double(0.5),
    trackPairV0Filter = cms.PSet(
        k0sMassWindow = cms.double(0.03)
    ),
    trackMultiplicityMin = cms.uint32(2),
    trackPseudoSelection = cms.PSet(
        b_pT = cms.double(0.3684),
        a_dR = cms.double(-0.001053),
        min_pT = cms.double(120),
        max_pT = cms.double(500),
        min_pT_dRcut = cms.double(0.5),
        max_pT_trackPTcut = cms.double(3),
        max_pT_dRcut = cms.double(0.1),
        b_dR = cms.double(0.6263),
        a_pT = cms.double(0.005263),
        totalHitsMin = cms.uint32(0),
        jetDeltaRMax = cms.double(0.3),
        qualityClass = cms.string('any'),
        pixelHitsMin = cms.uint32(0),
        sip3dSigMin = cms.double(-99999.9),
        sip3dSigMax = cms.double(0),
        maxDistToAxis = cms.double(0.07),
        useVariableJTA = cms.bool(False),
        sip2dValMax = cms.double(99999.9),
        maxDecayLen = cms.double(5),
        ptMin = cms.double(0.0),
        sip2dSigMax = cms.double(-2.0),
        sip2dSigMin = cms.double(-99999.9),
        sip3dValMax = cms.double(99999.9),
        sip3dValMin = cms.double(-99999.9),
        sip2dValMin = cms.double(-99999.9),
        normChi2Max = cms.double(99999.9)
    ),
    trackSort = cms.string('sip2dSig'),
    trackFlip = cms.bool(True)
)


process.combinedSecondaryVertexPositive = cms.ESProducer("CombinedSecondaryVertexESProducer",
    charmCut = cms.double(1.5),
    useTrackWeights = cms.bool(True),
    useCategories = cms.bool(True),
    pseudoMultiplicityMin = cms.uint32(2),
    categoryVariableName = cms.string('vertexCategory'),
    trackSelection = cms.PSet(
        b_pT = cms.double(0.3684),
        a_dR = cms.double(-0.001053),
        min_pT = cms.double(120),
        max_pT = cms.double(500),
        min_pT_dRcut = cms.double(0.5),
        max_pT_trackPTcut = cms.double(3),
        max_pT_dRcut = cms.double(0.1),
        b_dR = cms.double(0.6263),
        a_pT = cms.double(0.005263),
        totalHitsMin = cms.uint32(0),
        jetDeltaRMax = cms.double(0.3),
        qualityClass = cms.string('any'),
        pixelHitsMin = cms.uint32(0),
        sip3dSigMin = cms.double(0),
        sip3dSigMax = cms.double(99999.9),
        maxDistToAxis = cms.double(0.07),
        useVariableJTA = cms.bool(False),
        sip2dValMax = cms.double(99999.9),
        maxDecayLen = cms.double(5),
        ptMin = cms.double(0.0),
        sip2dSigMax = cms.double(99999.9),
        sip2dSigMin = cms.double(-99999.9),
        sip3dValMax = cms.double(99999.9),
        sip3dValMin = cms.double(-99999.9),
        sip2dValMin = cms.double(-99999.9),
        normChi2Max = cms.double(99999.9)
    ),
    calibrationRecords = cms.vstring('CombinedSVRecoVertex', 
        'CombinedSVPseudoVertex', 
        'CombinedSVNoVertex'),
    pseudoVertexV0Filter = cms.PSet(
        k0sMassWindow = cms.double(0.05)
    ),
    correctVertexMass = cms.bool(True),
    vertexFlip = cms.bool(False),
    minimumTrackWeight = cms.double(0.5),
    trackPairV0Filter = cms.PSet(
        k0sMassWindow = cms.double(0.03)
    ),
    trackMultiplicityMin = cms.uint32(2),
    trackPseudoSelection = cms.PSet(
        b_pT = cms.double(0.3684),
        a_dR = cms.double(-0.001053),
        min_pT = cms.double(120),
        max_pT = cms.double(500),
        min_pT_dRcut = cms.double(0.5),
        max_pT_trackPTcut = cms.double(3),
        max_pT_dRcut = cms.double(0.1),
        b_dR = cms.double(0.6263),
        a_pT = cms.double(0.005263),
        totalHitsMin = cms.uint32(0),
        jetDeltaRMax = cms.double(0.3),
        qualityClass = cms.string('any'),
        pixelHitsMin = cms.uint32(0),
        sip3dSigMin = cms.double(0),
        sip3dSigMax = cms.double(99999.9),
        maxDistToAxis = cms.double(0.07),
        useVariableJTA = cms.bool(False),
        sip2dValMax = cms.double(99999.9),
        maxDecayLen = cms.double(5),
        ptMin = cms.double(0.0),
        sip2dSigMax = cms.double(99999.9),
        sip2dSigMin = cms.double(2.0),
        sip3dValMax = cms.double(99999.9),
        sip3dValMin = cms.double(-99999.9),
        sip2dValMin = cms.double(-99999.9),
        normChi2Max = cms.double(99999.9)
    ),
    trackSort = cms.string('sip2dSig'),
    trackFlip = cms.bool(False)
)


process.combinedSecondaryVertexSoftLepton = cms.ESProducer("CombinedSecondaryVertexSoftLeptonESProducer",
    useTrackWeights = cms.bool(True),
    pseudoMultiplicityMin = cms.uint32(2),
    correctVertexMass = cms.bool(True),
    trackSelection = cms.PSet(
        b_pT = cms.double(0.3684),
        a_dR = cms.double(-0.001053),
        min_pT = cms.double(120),
        max_pT = cms.double(500),
        min_pT_dRcut = cms.double(0.5),
        max_pT_trackPTcut = cms.double(3),
        max_pT_dRcut = cms.double(0.1),
        b_dR = cms.double(0.6263),
        a_pT = cms.double(0.005263),
        totalHitsMin = cms.uint32(0),
        jetDeltaRMax = cms.double(0.3),
        qualityClass = cms.string('any'),
        pixelHitsMin = cms.uint32(0),
        sip3dSigMin = cms.double(-99999.9),
        sip3dSigMax = cms.double(99999.9),
        maxDistToAxis = cms.double(0.07),
        useVariableJTA = cms.bool(False),
        sip2dValMax = cms.double(99999.9),
        maxDecayLen = cms.double(5),
        ptMin = cms.double(0.0),
        sip2dSigMax = cms.double(99999.9),
        sip2dSigMin = cms.double(-99999.9),
        sip3dValMax = cms.double(99999.9),
        sip3dValMin = cms.double(-99999.9),
        sip2dValMin = cms.double(-99999.9),
        normChi2Max = cms.double(99999.9)
    ),
    pseudoVertexV0Filter = cms.PSet(
        k0sMassWindow = cms.double(0.05)
    ),
    charmCut = cms.double(1.5),
    vertexFlip = cms.bool(False),
    minimumTrackWeight = cms.double(0.5),
    trackPairV0Filter = cms.PSet(
        k0sMassWindow = cms.double(0.03)
    ),
    trackMultiplicityMin = cms.uint32(2),
    trackPseudoSelection = cms.PSet(
        b_pT = cms.double(0.3684),
        a_dR = cms.double(-0.001053),
        min_pT = cms.double(120),
        max_pT = cms.double(500),
        min_pT_dRcut = cms.double(0.5),
        max_pT_trackPTcut = cms.double(3),
        max_pT_dRcut = cms.double(0.1),
        b_dR = cms.double(0.6263),
        a_pT = cms.double(0.005263),
        totalHitsMin = cms.uint32(0),
        jetDeltaRMax = cms.double(0.3),
        qualityClass = cms.string('any'),
        pixelHitsMin = cms.uint32(0),
        sip3dSigMin = cms.double(-99999.9),
        sip3dSigMax = cms.double(99999.9),
        maxDistToAxis = cms.double(0.07),
        useVariableJTA = cms.bool(False),
        sip2dValMax = cms.double(99999.9),
        maxDecayLen = cms.double(5),
        ptMin = cms.double(0.0),
        sip2dSigMax = cms.double(99999.9),
        sip2dSigMin = cms.double(2.0),
        sip3dValMax = cms.double(99999.9),
        sip3dValMin = cms.double(-99999.9),
        sip2dValMin = cms.double(-99999.9),
        normChi2Max = cms.double(99999.9)
    ),
    trackSort = cms.string('sip2dSig'),
    trackFlip = cms.bool(False),
    calibrationRecords = cms.vstring('CombinedSVRecoVertexNoSoftLepton', 
        'CombinedSVPseudoVertexNoSoftLepton', 
        'CombinedSVNoVertexNoSoftLepton', 
        'CombinedSVRecoVertexSoftMuon', 
        'CombinedSVPseudoVertexSoftMuon', 
        'CombinedSVNoVertexSoftMuon', 
        'CombinedSVRecoVertexSoftElectron', 
        'CombinedSVPseudoVertexSoftElectron', 
        'CombinedSVNoVertexSoftElectron'),
    useCategories = cms.bool(True),
    categoryVariableName = cms.string('vertexLeptonCategory')
)


process.combinedSecondaryVertexV2 = cms.ESProducer("CombinedSecondaryVertexESProducerV2",
    useTrackWeights = cms.bool(True),
    pseudoMultiplicityMin = cms.uint32(2),
    correctVertexMass = cms.bool(True),
    trackSelection = cms.PSet(
        b_pT = cms.double(0.3684),
        a_dR = cms.double(-0.001053),
        min_pT = cms.double(120),
        max_pT = cms.double(500),
        min_pT_dRcut = cms.double(0.5),
        max_pT_trackPTcut = cms.double(3),
        max_pT_dRcut = cms.double(0.1),
        b_dR = cms.double(0.6263),
        a_pT = cms.double(0.005263),
        totalHitsMin = cms.uint32(0),
        jetDeltaRMax = cms.double(0.3),
        qualityClass = cms.string('any'),
        pixelHitsMin = cms.uint32(0),
        sip3dSigMin = cms.double(-99999.9),
        sip3dSigMax = cms.double(99999.9),
        maxDistToAxis = cms.double(0.07),
        useVariableJTA = cms.bool(False),
        sip2dValMax = cms.double(99999.9),
        maxDecayLen = cms.double(5),
        ptMin = cms.double(0.0),
        sip2dSigMax = cms.double(99999.9),
        sip2dSigMin = cms.double(-99999.9),
        sip3dValMax = cms.double(99999.9),
        sip3dValMin = cms.double(-99999.9),
        sip2dValMin = cms.double(-99999.9),
        normChi2Max = cms.double(99999.9)
    ),
    pseudoVertexV0Filter = cms.PSet(
        k0sMassWindow = cms.double(0.05)
    ),
    charmCut = cms.double(1.5),
    vertexFlip = cms.bool(False),
    minimumTrackWeight = cms.double(0.5),
    trackPairV0Filter = cms.PSet(
        k0sMassWindow = cms.double(0.03)
    ),
    trackMultiplicityMin = cms.uint32(2),
    trackPseudoSelection = cms.PSet(
        b_pT = cms.double(0.3684),
        a_dR = cms.double(-0.001053),
        min_pT = cms.double(120),
        max_pT = cms.double(500),
        min_pT_dRcut = cms.double(0.5),
        max_pT_trackPTcut = cms.double(3),
        max_pT_dRcut = cms.double(0.1),
        b_dR = cms.double(0.6263),
        a_pT = cms.double(0.005263),
        totalHitsMin = cms.uint32(0),
        jetDeltaRMax = cms.double(0.3),
        qualityClass = cms.string('any'),
        pixelHitsMin = cms.uint32(0),
        sip3dSigMin = cms.double(-99999.9),
        sip3dSigMax = cms.double(99999.9),
        maxDistToAxis = cms.double(0.07),
        useVariableJTA = cms.bool(False),
        sip2dValMax = cms.double(99999.9),
        maxDecayLen = cms.double(5),
        ptMin = cms.double(0.0),
        sip2dSigMax = cms.double(99999.9),
        sip2dSigMin = cms.double(2.0),
        sip3dValMax = cms.double(99999.9),
        sip3dValMin = cms.double(-99999.9),
        sip2dValMin = cms.double(-99999.9),
        normChi2Max = cms.double(99999.9)
    ),
    trackSort = cms.string('sip2dSig'),
    trackFlip = cms.bool(False),
    calibrationRecords = cms.vstring('CombinedSVIVFV2RecoVertex', 
        'CombinedSVIVFV2PseudoVertex', 
        'CombinedSVIVFV2NoVertex'),
    useCategories = cms.bool(True),
    categoryVariableName = cms.string('vertexCategory')
)


process.combinedSecondaryVertexV2Negative = cms.ESProducer("CombinedSecondaryVertexESProducerV2",
    charmCut = cms.double(1.5),
    useTrackWeights = cms.bool(True),
    useCategories = cms.bool(True),
    pseudoMultiplicityMin = cms.uint32(2),
    categoryVariableName = cms.string('vertexCategory'),
    trackSelection = cms.PSet(
        b_pT = cms.double(0.3684),
        a_dR = cms.double(-0.001053),
        min_pT = cms.double(120),
        max_pT = cms.double(500),
        min_pT_dRcut = cms.double(0.5),
        max_pT_trackPTcut = cms.double(3),
        max_pT_dRcut = cms.double(0.1),
        b_dR = cms.double(0.6263),
        a_pT = cms.double(0.005263),
        totalHitsMin = cms.uint32(0),
        jetDeltaRMax = cms.double(0.3),
        qualityClass = cms.string('any'),
        pixelHitsMin = cms.uint32(0),
        sip3dSigMin = cms.double(-99999.9),
        sip3dSigMax = cms.double(0),
        maxDistToAxis = cms.double(0.07),
        useVariableJTA = cms.bool(False),
        sip2dValMax = cms.double(99999.9),
        maxDecayLen = cms.double(5),
        ptMin = cms.double(0.0),
        sip2dSigMax = cms.double(99999.9),
        sip2dSigMin = cms.double(-99999.9),
        sip3dValMax = cms.double(99999.9),
        sip3dValMin = cms.double(-99999.9),
        sip2dValMin = cms.double(-99999.9),
        normChi2Max = cms.double(99999.9)
    ),
    calibrationRecords = cms.vstring('CombinedSVIVFV2RecoVertex', 
        'CombinedSVIVFV2PseudoVertex', 
        'CombinedSVIVFV2NoVertex'),
    pseudoVertexV0Filter = cms.PSet(
        k0sMassWindow = cms.double(0.05)
    ),
    correctVertexMass = cms.bool(True),
    vertexFlip = cms.bool(True),
    minimumTrackWeight = cms.double(0.5),
    trackPairV0Filter = cms.PSet(
        k0sMassWindow = cms.double(0.03)
    ),
    trackMultiplicityMin = cms.uint32(2),
    trackPseudoSelection = cms.PSet(
        b_pT = cms.double(0.3684),
        a_dR = cms.double(-0.001053),
        min_pT = cms.double(120),
        max_pT = cms.double(500),
        min_pT_dRcut = cms.double(0.5),
        max_pT_trackPTcut = cms.double(3),
        max_pT_dRcut = cms.double(0.1),
        b_dR = cms.double(0.6263),
        a_pT = cms.double(0.005263),
        totalHitsMin = cms.uint32(0),
        jetDeltaRMax = cms.double(0.3),
        qualityClass = cms.string('any'),
        pixelHitsMin = cms.uint32(0),
        sip3dSigMin = cms.double(-99999.9),
        sip3dSigMax = cms.double(0),
        maxDistToAxis = cms.double(0.07),
        useVariableJTA = cms.bool(False),
        sip2dValMax = cms.double(99999.9),
        maxDecayLen = cms.double(5),
        ptMin = cms.double(0.0),
        sip2dSigMax = cms.double(-2.0),
        sip2dSigMin = cms.double(-99999.9),
        sip3dValMax = cms.double(99999.9),
        sip3dValMin = cms.double(-99999.9),
        sip2dValMin = cms.double(-99999.9),
        normChi2Max = cms.double(99999.9)
    ),
    trackSort = cms.string('sip2dSig'),
    trackFlip = cms.bool(True)
)


process.combinedSecondaryVertexV2Positive = cms.ESProducer("CombinedSecondaryVertexESProducerV2",
    charmCut = cms.double(1.5),
    useTrackWeights = cms.bool(True),
    useCategories = cms.bool(True),
    pseudoMultiplicityMin = cms.uint32(2),
    categoryVariableName = cms.string('vertexCategory'),
    trackSelection = cms.PSet(
        b_pT = cms.double(0.3684),
        a_dR = cms.double(-0.001053),
        min_pT = cms.double(120),
        max_pT = cms.double(500),
        min_pT_dRcut = cms.double(0.5),
        max_pT_trackPTcut = cms.double(3),
        max_pT_dRcut = cms.double(0.1),
        b_dR = cms.double(0.6263),
        a_pT = cms.double(0.005263),
        totalHitsMin = cms.uint32(0),
        jetDeltaRMax = cms.double(0.3),
        qualityClass = cms.string('any'),
        pixelHitsMin = cms.uint32(0),
        sip3dSigMin = cms.double(0),
        sip3dSigMax = cms.double(99999.9),
        maxDistToAxis = cms.double(0.07),
        useVariableJTA = cms.bool(False),
        sip2dValMax = cms.double(99999.9),
        maxDecayLen = cms.double(5),
        ptMin = cms.double(0.0),
        sip2dSigMax = cms.double(99999.9),
        sip2dSigMin = cms.double(-99999.9),
        sip3dValMax = cms.double(99999.9),
        sip3dValMin = cms.double(-99999.9),
        sip2dValMin = cms.double(-99999.9),
        normChi2Max = cms.double(99999.9)
    ),
    calibrationRecords = cms.vstring('CombinedSVIVFV2RecoVertex', 
        'CombinedSVIVFV2PseudoVertex', 
        'CombinedSVIVFV2NoVertex'),
    pseudoVertexV0Filter = cms.PSet(
        k0sMassWindow = cms.double(0.05)
    ),
    correctVertexMass = cms.bool(True),
    vertexFlip = cms.bool(False),
    minimumTrackWeight = cms.double(0.5),
    trackPairV0Filter = cms.PSet(
        k0sMassWindow = cms.double(0.03)
    ),
    trackMultiplicityMin = cms.uint32(2),
    trackPseudoSelection = cms.PSet(
        b_pT = cms.double(0.3684),
        a_dR = cms.double(-0.001053),
        min_pT = cms.double(120),
        max_pT = cms.double(500),
        min_pT_dRcut = cms.double(0.5),
        max_pT_trackPTcut = cms.double(3),
        max_pT_dRcut = cms.double(0.1),
        b_dR = cms.double(0.6263),
        a_pT = cms.double(0.005263),
        totalHitsMin = cms.uint32(0),
        jetDeltaRMax = cms.double(0.3),
        qualityClass = cms.string('any'),
        pixelHitsMin = cms.uint32(0),
        sip3dSigMin = cms.double(0),
        sip3dSigMax = cms.double(99999.9),
        maxDistToAxis = cms.double(0.07),
        useVariableJTA = cms.bool(False),
        sip2dValMax = cms.double(99999.9),
        maxDecayLen = cms.double(5),
        ptMin = cms.double(0.0),
        sip2dSigMax = cms.double(99999.9),
        sip2dSigMin = cms.double(2.0),
        sip3dValMax = cms.double(99999.9),
        sip3dValMin = cms.double(-99999.9),
        sip2dValMin = cms.double(-99999.9),
        normChi2Max = cms.double(99999.9)
    ),
    trackSort = cms.string('sip2dSig'),
    trackFlip = cms.bool(False)
)


process.doubleVertex2Trk = cms.ESProducer("SimpleSecondaryVertexESProducer",
    minTracks = cms.uint32(2),
    minVertices = cms.uint32(2),
    unBoost = cms.bool(False),
    useSignificance = cms.bool(True),
    use3d = cms.bool(True)
)


process.fakeForIdealAlignment = cms.ESProducer("FakeAlignmentProducer",
    appendToDataLabel = cms.string('fakeForIdeal')
)


process.ghostTrack = cms.ESProducer("GhostTrackESProducer",
    trackPairV0Filter = cms.PSet(
        k0sMassWindow = cms.double(0.03)
    ),
    charmCut = cms.double(1.5),
    trackSort = cms.string('sip2dSig'),
    minimumTrackWeight = cms.double(0.5),
    trackSelection = cms.PSet(
        b_pT = cms.double(0.3684),
        a_dR = cms.double(-0.001053),
        min_pT = cms.double(120),
        max_pT = cms.double(500),
        min_pT_dRcut = cms.double(0.5),
        max_pT_trackPTcut = cms.double(3),
        max_pT_dRcut = cms.double(0.1),
        b_dR = cms.double(0.6263),
        a_pT = cms.double(0.005263),
        totalHitsMin = cms.uint32(0),
        jetDeltaRMax = cms.double(0.3),
        qualityClass = cms.string('any'),
        pixelHitsMin = cms.uint32(0),
        sip3dSigMin = cms.double(-99999.9),
        sip3dSigMax = cms.double(99999.9),
        maxDistToAxis = cms.double(0.07),
        useVariableJTA = cms.bool(False),
        sip2dValMax = cms.double(99999.9),
        maxDecayLen = cms.double(5),
        ptMin = cms.double(0.0),
        sip2dSigMax = cms.double(99999.9),
        sip2dSigMin = cms.double(-99999.9),
        sip3dValMax = cms.double(99999.9),
        sip3dValMin = cms.double(-99999.9),
        sip2dValMin = cms.double(-99999.9),
        normChi2Max = cms.double(99999.9)
    ),
    calibrationRecords = cms.vstring('GhostTrackRecoVertex', 
        'GhostTrackPseudoVertex', 
        'GhostTrackNoVertex'),
    useCategories = cms.bool(True),
    categoryVariableName = cms.string('vertexCategory')
)


process.hcalTopologyIdeal = cms.ESProducer("HcalTopologyIdealEP",
    Exclude = cms.untracked.string(''),
    appendToDataLabel = cms.string(''),
    hcalTopologyConstants = cms.PSet(
        maxDepthHE = cms.int32(3),
        maxDepthHB = cms.int32(2),
        mode = cms.string('HcalTopologyMode::LHC')
    )
)


process.hcal_db_producer = cms.ESProducer("HcalDbProducer",
    file = cms.untracked.string(''),
    dump = cms.untracked.vstring('')
)


process.ic5CaloL1FastL2L3 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak4CaloL1Fastjet', 
        'ic5CaloL2Relative', 
        'ic5CaloL3Absolute')
)


process.ic5CaloL1FastL2L3L6 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ic5CaloL1Offset', 
        'ic5CaloL2Relative', 
        'ic5CaloL3Absolute', 
        'ic5CaloL6SLB')
)


process.ic5CaloL1FastL2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ic5CaloL1Fastjet', 
        'ic5CaloL2Relative', 
        'ic5CaloL3Absolute', 
        'ic5CaloResidual')
)


process.ic5CaloL1Fastjet = cms.ESProducer("L1FastjetCorrectionESProducer",
    srcRho = cms.InputTag("fixedGridRhoFastjetAllCalo"),
    algorithm = cms.string('AK5Calo'),
    level = cms.string('L1FastJet')
)


process.ic5CaloL1L2L3 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ic5CaloL1Offset', 
        'ic5CaloL2Relative', 
        'ic5CaloL3Absolute')
)


process.ic5CaloL1L2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ic5CaloL1Offset', 
        'ic5CaloL2Relative', 
        'ic5CaloL3Absolute', 
        'ic5CaloResidual')
)


process.ic5CaloL1Offset = cms.ESProducer("L1OffsetCorrectionESProducer",
    minVtxNdof = cms.int32(4),
    vertexCollection = cms.string('offlinePrimaryVertices'),
    algorithm = cms.string('AK5Calo'),
    level = cms.string('L1Offset')
)


process.ic5CaloL2L3 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ic5CaloL2Relative', 
        'ic5CaloL3Absolute')
)


process.ic5CaloL2L3L6 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ic5CaloL2Relative', 
        'ic5CaloL3Absolute', 
        'ic5CaloL6SLB')
)


process.ic5CaloL2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ic5CaloL2Relative', 
        'ic5CaloL3Absolute', 
        'ic5CaloResidual')
)


process.ic5CaloL2Relative = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('IC5Calo'),
    level = cms.string('L2Relative')
)


process.ic5CaloL3Absolute = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('IC5Calo'),
    level = cms.string('L3Absolute')
)


process.ic5CaloL6SLB = cms.ESProducer("L6SLBCorrectionESProducer",
    srcBTagInfoElectron = cms.InputTag("ic5CaloJetsSoftElectronTagInfos"),
    srcBTagInfoMuon = cms.InputTag("ic5CaloJetsSoftMuonTagInfos"),
    addMuonToJet = cms.bool(True),
    algorithm = cms.string(''),
    level = cms.string('L6SLB')
)


process.ic5CaloResidual = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK5Calo'),
    level = cms.string('L2L3Residual')
)


process.ic5PFL1FastL2L3 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak4PFL1Fastjet', 
        'ic5PFL2Relative', 
        'ic5PFL3Absolute')
)


process.ic5PFL1FastL2L3L6 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak4PFL1Fastjet', 
        'ic5PFL2Relative', 
        'ic5PFL3Absolute', 
        'ic5PFL6SLB')
)


process.ic5PFL1FastL2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ic5PFL1Fastjet', 
        'ic5PFL2Relative', 
        'ic5PFL3Absolute', 
        'ic5PFResidual')
)


process.ic5PFL1Fastjet = cms.ESProducer("L1FastjetCorrectionESProducer",
    srcRho = cms.InputTag("fixedGridRhoFastjetAll"),
    algorithm = cms.string('AK4PF'),
    level = cms.string('L1FastJet')
)


process.ic5PFL1L2L3 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ic5PFL1Offset', 
        'ic5PFL2Relative', 
        'ic5PFL3Absolute')
)


process.ic5PFL1L2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ic5PFL1Offset', 
        'ic5PFL2Relative', 
        'ic5PFL3Absolute', 
        'ic5PFResidual')
)


process.ic5PFL1Offset = cms.ESProducer("L1OffsetCorrectionESProducer",
    minVtxNdof = cms.int32(4),
    vertexCollection = cms.string('offlinePrimaryVertices'),
    algorithm = cms.string('AK4PF'),
    level = cms.string('L1Offset')
)


process.ic5PFL2L3 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ic5PFL2Relative', 
        'ic5PFL3Absolute')
)


process.ic5PFL2L3L6 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ic5PFL2Relative', 
        'ic5PFL3Absolute', 
        'ic5PFL6SLB')
)


process.ic5PFL2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ic5PFL2Relative', 
        'ic5PFL3Absolute', 
        'ic5PFResidual')
)


process.ic5PFL2Relative = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('IC5PF'),
    level = cms.string('L2Relative')
)


process.ic5PFL3Absolute = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('IC5PF'),
    level = cms.string('L3Absolute')
)


process.ic5PFL6SLB = cms.ESProducer("L6SLBCorrectionESProducer",
    srcBTagInfoElectron = cms.InputTag("ic5PFJetsSoftElectronTagInfos"),
    srcBTagInfoMuon = cms.InputTag("ic5PFJetsSoftMuonTagInfos"),
    addMuonToJet = cms.bool(False),
    algorithm = cms.string(''),
    level = cms.string('L6SLB')
)


process.ic5PFResidual = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK4PF'),
    level = cms.string('L2L3Residual')
)


process.idealForDigiCSCGeometry = cms.ESProducer("CSCGeometryESModule",
    appendToDataLabel = cms.string('idealForDigi'),
    useDDD = cms.bool(True),
    debugV = cms.untracked.bool(False),
    useGangedStripsInME1a = cms.bool(True),
    alignmentsLabel = cms.string('fakeForIdeal'),
    useOnlyWiresInME1a = cms.bool(False),
    useRealWireGeometry = cms.bool(True),
    useCentreTIOffsets = cms.bool(False),
    applyAlignment = cms.bool(False)
)


process.idealForDigiDTGeometry = cms.ESProducer("DTGeometryESModule",
    appendToDataLabel = cms.string('idealForDigi'),
    fromDDD = cms.bool(True),
    applyAlignment = cms.bool(False),
    alignmentsLabel = cms.string('fakeForIdeal')
)


process.idealForDigiTrackerGeometry = cms.ESProducer("TrackerDigiGeometryESModule",
    appendToDataLabel = cms.string('idealForDigi'),
    fromDDD = cms.bool(True),
    trackerGeometryConstants = cms.PSet(
        ROCS_X = cms.int32(0),
        ROCS_Y = cms.int32(0),
        upgradeGeometry = cms.bool(False),
        BIG_PIX_PER_ROC_Y = cms.int32(2),
        BIG_PIX_PER_ROC_X = cms.int32(1),
        ROWS_PER_ROC = cms.int32(80),
        COLS_PER_ROC = cms.int32(52)
    ),
    applyAlignment = cms.bool(False),
    alignmentsLabel = cms.string('fakeForIdeal')
)


process.impactParameterMVAComputer = cms.ESProducer("GenericMVAJetTagESProducer",
    useCategories = cms.bool(False),
    calibrationRecord = cms.string('ImpactParameterMVA')
)


process.jetBProbability = cms.ESProducer("JetBProbabilityESProducer",
    b_pT = cms.double(0.3684),
    a_dR = cms.double(-0.001053),
    min_pT = cms.double(120),
    max_pT = cms.double(500),
    min_pT_dRcut = cms.double(0.5),
    max_pT_trackPTcut = cms.double(3),
    max_pT_dRcut = cms.double(0.1),
    b_dR = cms.double(0.6263),
    a_pT = cms.double(0.005263),
    deltaR = cms.double(-1.0),
    maximumDistanceToJetAxis = cms.double(0.07),
    impactParameterType = cms.int32(0),
    trackQualityClass = cms.string('any'),
    useVariableJTA = cms.bool(False),
    trackIpSign = cms.int32(1),
    minimumProbability = cms.double(0.005),
    numberOfBTracks = cms.uint32(4),
    maximumDecayLength = cms.double(5.0)
)


process.jetProbability = cms.ESProducer("JetProbabilityESProducer",
    b_pT = cms.double(0.3684),
    a_dR = cms.double(-0.001053),
    min_pT = cms.double(120),
    max_pT = cms.double(500),
    min_pT_dRcut = cms.double(0.5),
    max_pT_trackPTcut = cms.double(3),
    max_pT_dRcut = cms.double(0.1),
    b_dR = cms.double(0.6263),
    a_pT = cms.double(0.005263),
    deltaR = cms.double(0.3),
    maximumDistanceToJetAxis = cms.double(0.07),
    impactParameterType = cms.int32(0),
    trackQualityClass = cms.string('any'),
    useVariableJTA = cms.bool(False),
    trackIpSign = cms.int32(1),
    minimumProbability = cms.double(0.005),
    maximumDecayLength = cms.double(5.0)
)


process.kt4CaloL1FastL2L3 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak4CaloL1Fastjet', 
        'kt4CaloL2Relative', 
        'kt4CaloL3Absolute')
)


process.kt4CaloL1FastL2L3L6 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('kt4CaloL1Offset', 
        'kt4CaloL2Relative', 
        'kt4CaloL3Absolute', 
        'kt4CaloL6SLB')
)


process.kt4CaloL1FastL2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('kt4CaloL1Fastjet', 
        'kt4CaloL2Relative', 
        'kt4CaloL3Absolute', 
        'kt4CaloResidual')
)


process.kt4CaloL1Fastjet = cms.ESProducer("L1FastjetCorrectionESProducer",
    srcRho = cms.InputTag("fixedGridRhoFastjetAllCalo"),
    algorithm = cms.string('AK5Calo'),
    level = cms.string('L1FastJet')
)


process.kt4CaloL1L2L3 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('kt4CaloL1Offset', 
        'kt4CaloL2Relative', 
        'kt4CaloL3Absolute')
)


process.kt4CaloL1L2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('kt4CaloL1Offset', 
        'kt4CaloL2Relative', 
        'kt4CaloL3Absolute', 
        'kt4CaloResidual')
)


process.kt4CaloL1Offset = cms.ESProducer("L1OffsetCorrectionESProducer",
    minVtxNdof = cms.int32(4),
    vertexCollection = cms.string('offlinePrimaryVertices'),
    algorithm = cms.string('AK5Calo'),
    level = cms.string('L1Offset')
)


process.kt4CaloL2L3 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('kt4CaloL2Relative', 
        'kt4CaloL3Absolute')
)


process.kt4CaloL2L3L6 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('kt4CaloL2Relative', 
        'kt4CaloL3Absolute', 
        'kt4CaloL6SLB')
)


process.kt4CaloL2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('kt4CaloL2Relative', 
        'kt4CaloL3Absolute', 
        'kt4CaloResidual')
)


process.kt4CaloL2Relative = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('KT4Calo'),
    level = cms.string('L2Relative')
)


process.kt4CaloL3Absolute = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('KT4Calo'),
    level = cms.string('L3Absolute')
)


process.kt4CaloL6SLB = cms.ESProducer("L6SLBCorrectionESProducer",
    srcBTagInfoElectron = cms.InputTag("kt4CaloJetsSoftElectronTagInfos"),
    srcBTagInfoMuon = cms.InputTag("kt4CaloJetsSoftMuonTagInfos"),
    addMuonToJet = cms.bool(True),
    algorithm = cms.string(''),
    level = cms.string('L6SLB')
)


process.kt4CaloResidual = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK5Calo'),
    level = cms.string('L2L3Residual')
)


process.kt4PFL1FastL2L3 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak4PFL1Fastjet', 
        'kt4PFL2Relative', 
        'kt4PFL3Absolute')
)


process.kt4PFL1FastL2L3L6 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak4PFL1Fastjet', 
        'kt4PFL2Relative', 
        'kt4PFL3Absolute', 
        'kt4PFL6SLB')
)


process.kt4PFL1FastL2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('kt4PFL1Fastjet', 
        'kt4PFL2Relative', 
        'kt4PFL3Absolute', 
        'kt4PFResidual')
)


process.kt4PFL1Fastjet = cms.ESProducer("L1FastjetCorrectionESProducer",
    srcRho = cms.InputTag("fixedGridRhoFastjetAll"),
    algorithm = cms.string('AK4PF'),
    level = cms.string('L1FastJet')
)


process.kt4PFL1L2L3 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('kt4PFL1Offset', 
        'kt4PFL2Relative', 
        'kt4PFL3Absolute')
)


process.kt4PFL1L2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('kt4PFL1Offset', 
        'kt4PFL2Relative', 
        'kt4PFL3Absolute', 
        'kt4PFResidual')
)


process.kt4PFL1Offset = cms.ESProducer("L1OffsetCorrectionESProducer",
    minVtxNdof = cms.int32(4),
    vertexCollection = cms.string('offlinePrimaryVertices'),
    algorithm = cms.string('AK4PF'),
    level = cms.string('L1Offset')
)


process.kt4PFL2L3 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('kt4PFL2Relative', 
        'kt4PFL3Absolute')
)


process.kt4PFL2L3L6 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('kt4PFL2Relative', 
        'kt4PFL3Absolute', 
        'kt4PFL6SLB')
)


process.kt4PFL2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('kt4PFL2Relative', 
        'kt4PFL3Absolute', 
        'kt4PFResidual')
)


process.kt4PFL2Relative = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('KT4PF'),
    level = cms.string('L2Relative')
)


process.kt4PFL3Absolute = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('KT4PF'),
    level = cms.string('L3Absolute')
)


process.kt4PFL6SLB = cms.ESProducer("L6SLBCorrectionESProducer",
    srcBTagInfoElectron = cms.InputTag("kt4PFJetsSoftElectronTagInfos"),
    srcBTagInfoMuon = cms.InputTag("kt4PFJetsSoftMuonTagInfos"),
    addMuonToJet = cms.bool(False),
    algorithm = cms.string(''),
    level = cms.string('L6SLB')
)


process.kt4PFResidual = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK4PF'),
    level = cms.string('L2L3Residual')
)


process.kt6CaloL1FastL2L3 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak4CaloL1Fastjet', 
        'kt6CaloL2Relative', 
        'kt6CaloL3Absolute')
)


process.kt6CaloL1FastL2L3L6 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('kt6CaloL1Offset', 
        'kt6CaloL2Relative', 
        'kt6CaloL3Absolute', 
        'kt6CaloL6SLB')
)


process.kt6CaloL1FastL2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('kt6CaloL1Fastjet', 
        'kt6CaloL2Relative', 
        'kt6CaloL3Absolute', 
        'kt6CaloResidual')
)


process.kt6CaloL1Fastjet = cms.ESProducer("L1FastjetCorrectionESProducer",
    srcRho = cms.InputTag("fixedGridRhoFastjetAllCalo"),
    algorithm = cms.string('AK5Calo'),
    level = cms.string('L1FastJet')
)


process.kt6CaloL1L2L3 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('kt6CaloL1Offset', 
        'kt6CaloL2Relative', 
        'kt6CaloL3Absolute')
)


process.kt6CaloL1L2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('kt6CaloL1Offset', 
        'kt6CaloL2Relative', 
        'kt6CaloL3Absolute', 
        'kt6CaloResidual')
)


process.kt6CaloL1Offset = cms.ESProducer("L1OffsetCorrectionESProducer",
    minVtxNdof = cms.int32(4),
    vertexCollection = cms.string('offlinePrimaryVertices'),
    algorithm = cms.string('AK5Calo'),
    level = cms.string('L1Offset')
)


process.kt6CaloL2L3 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('kt6CaloL2Relative', 
        'kt6CaloL3Absolute')
)


process.kt6CaloL2L3L6 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('kt6CaloL2Relative', 
        'kt6CaloL3Absolute', 
        'kt6CaloL6SLB')
)


process.kt6CaloL2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('kt6CaloL2Relative', 
        'kt6CaloL3Absolute', 
        'kt6CaloResidual')
)


process.kt6CaloL2Relative = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('KT6Calo'),
    level = cms.string('L2Relative')
)


process.kt6CaloL3Absolute = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('KT6Calo'),
    level = cms.string('L3Absolute')
)


process.kt6CaloL6SLB = cms.ESProducer("L6SLBCorrectionESProducer",
    srcBTagInfoElectron = cms.InputTag("kt6CaloJetsSoftElectronTagInfos"),
    srcBTagInfoMuon = cms.InputTag("kt6CaloJetsSoftMuonTagInfos"),
    addMuonToJet = cms.bool(True),
    algorithm = cms.string(''),
    level = cms.string('L6SLB')
)


process.kt6CaloResidual = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK5Calo'),
    level = cms.string('L2L3Residual')
)


process.kt6PFL1FastL2L3 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak4PFL1Fastjet', 
        'kt6PFL2Relative', 
        'kt6PFL3Absolute')
)


process.kt6PFL1FastL2L3L6 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak4PFL1Fastjet', 
        'kt6PFL2Relative', 
        'kt6PFL3Absolute', 
        'kt6PFL6SLB')
)


process.kt6PFL1FastL2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('kt6PFL1Fastjet', 
        'kt6PFL2Relative', 
        'kt6PFL3Absolute', 
        'kt6PFResidual')
)


process.kt6PFL1Fastjet = cms.ESProducer("L1FastjetCorrectionESProducer",
    srcRho = cms.InputTag("fixedGridRhoFastjetAll"),
    algorithm = cms.string('AK4PF'),
    level = cms.string('L1FastJet')
)


process.kt6PFL1L2L3 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('kt6PFL1Offset', 
        'kt6PFL2Relative', 
        'kt6PFL3Absolute')
)


process.kt6PFL1L2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('kt6PFL1Offset', 
        'kt6PFL2Relative', 
        'kt6PFL3Absolute', 
        'kt6PFResidual')
)


process.kt6PFL1Offset = cms.ESProducer("L1OffsetCorrectionESProducer",
    minVtxNdof = cms.int32(4),
    vertexCollection = cms.string('offlinePrimaryVertices'),
    algorithm = cms.string('AK4PF'),
    level = cms.string('L1Offset')
)


process.kt6PFL2L3 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('kt6PFL2Relative', 
        'kt6PFL3Absolute')
)


process.kt6PFL2L3L6 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('kt6PFL2Relative', 
        'kt6PFL3Absolute', 
        'kt6PFL6SLB')
)


process.kt6PFL2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('kt6PFL2Relative', 
        'kt6PFL3Absolute', 
        'kt6PFResidual')
)


process.kt6PFL2Relative = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('KT6PF'),
    level = cms.string('L2Relative')
)


process.kt6PFL3Absolute = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('KT6PF'),
    level = cms.string('L3Absolute')
)


process.kt6PFL6SLB = cms.ESProducer("L6SLBCorrectionESProducer",
    srcBTagInfoElectron = cms.InputTag("kt6PFJetsSoftElectronTagInfos"),
    srcBTagInfoMuon = cms.InputTag("kt6PFJetsSoftMuonTagInfos"),
    addMuonToJet = cms.bool(False),
    algorithm = cms.string(''),
    level = cms.string('L6SLB')
)


process.kt6PFResidual = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK4PF'),
    level = cms.string('L2L3Residual')
)


process.negativeCombinedMVA = cms.ESProducer("CombinedMVAJetTagESProducer",
    useCategories = cms.bool(False),
    calibrationRecord = cms.string('CombinedMVA'),
    jetTagComputers = cms.VPSet(cms.PSet(
        discriminator = cms.bool(True),
        variables = cms.bool(False),
        jetTagComputer = cms.string('negativeOnlyJetProbability')
    ), 
        cms.PSet(
            discriminator = cms.bool(True),
            variables = cms.bool(False),
            jetTagComputer = cms.string('combinedSecondaryVertexNegative')
        ), 
        cms.PSet(
            discriminator = cms.bool(True),
            variables = cms.bool(False),
            jetTagComputer = cms.string('negativeSoftPFMuon')
        ), 
        cms.PSet(
            discriminator = cms.bool(True),
            variables = cms.bool(False),
            jetTagComputer = cms.string('negativeSoftPFElectron')
        ))
)


process.negativeOnlyJetBProbability = cms.ESProducer("JetBProbabilityESProducer",
    b_pT = cms.double(0.3684),
    a_dR = cms.double(-0.001053),
    min_pT = cms.double(120),
    max_pT = cms.double(500),
    min_pT_dRcut = cms.double(0.5),
    max_pT_trackPTcut = cms.double(3),
    max_pT_dRcut = cms.double(0.1),
    b_dR = cms.double(0.6263),
    a_pT = cms.double(0.005263),
    deltaR = cms.double(-1.0),
    maximumDistanceToJetAxis = cms.double(0.07),
    impactParameterType = cms.int32(0),
    trackQualityClass = cms.string('any'),
    useVariableJTA = cms.bool(False),
    trackIpSign = cms.int32(-1),
    minimumProbability = cms.double(0.005),
    numberOfBTracks = cms.uint32(4),
    maximumDecayLength = cms.double(5.0)
)


process.negativeOnlyJetProbability = cms.ESProducer("JetProbabilityESProducer",
    b_pT = cms.double(0.3684),
    a_dR = cms.double(-0.001053),
    min_pT = cms.double(120),
    max_pT = cms.double(500),
    min_pT_dRcut = cms.double(0.5),
    max_pT_trackPTcut = cms.double(3),
    max_pT_dRcut = cms.double(0.1),
    b_dR = cms.double(0.6263),
    a_pT = cms.double(0.005263),
    deltaR = cms.double(0.3),
    maximumDistanceToJetAxis = cms.double(0.07),
    impactParameterType = cms.int32(0),
    trackQualityClass = cms.string('any'),
    useVariableJTA = cms.bool(False),
    trackIpSign = cms.int32(-1),
    minimumProbability = cms.double(0.005),
    maximumDecayLength = cms.double(5.0)
)


process.negativeSoftPFElectron = cms.ESProducer("ElectronTaggerESProducer",
    ipSign = cms.string('negative')
)


process.negativeSoftPFElectronByIP2d = cms.ESProducer("LeptonTaggerByIPESProducer",
    use3d = cms.bool(False),
    ipSign = cms.string('negative')
)


process.negativeSoftPFElectronByIP3d = cms.ESProducer("LeptonTaggerByIPESProducer",
    use3d = cms.bool(True),
    ipSign = cms.string('negative')
)


process.negativeSoftPFElectronByPt = cms.ESProducer("LeptonTaggerByPtESProducer",
    ipSign = cms.string('negative')
)


process.negativeSoftPFMuon = cms.ESProducer("MuonTaggerESProducer",
    ipSign = cms.string('negative')
)


process.negativeSoftPFMuonByIP2d = cms.ESProducer("LeptonTaggerByIPESProducer",
    use3d = cms.bool(False),
    ipSign = cms.string('negative')
)


process.negativeSoftPFMuonByIP3d = cms.ESProducer("LeptonTaggerByIPESProducer",
    use3d = cms.bool(True),
    ipSign = cms.string('negative')
)


process.negativeSoftPFMuonByPt = cms.ESProducer("LeptonTaggerByPtESProducer",
    ipSign = cms.string('negative')
)


process.negativeTrackCounting3D2nd = cms.ESProducer("NegativeTrackCountingESProducer",
    b_pT = cms.double(0.3684),
    a_dR = cms.double(-0.001053),
    min_pT = cms.double(120),
    max_pT = cms.double(500),
    min_pT_dRcut = cms.double(0.5),
    max_pT_trackPTcut = cms.double(3),
    max_pT_dRcut = cms.double(0.1),
    b_dR = cms.double(0.6263),
    a_pT = cms.double(0.005263),
    deltaR = cms.double(-1.0),
    maximumDecayLength = cms.double(5.0),
    impactParameterType = cms.int32(0),
    trackQualityClass = cms.string('any'),
    useVariableJTA = cms.bool(False),
    maximumDistanceToJetAxis = cms.double(0.07),
    nthTrack = cms.int32(2)
)


process.negativeTrackCounting3D3rd = cms.ESProducer("NegativeTrackCountingESProducer",
    b_pT = cms.double(0.3684),
    a_dR = cms.double(-0.001053),
    min_pT = cms.double(120),
    max_pT = cms.double(500),
    min_pT_dRcut = cms.double(0.5),
    max_pT_trackPTcut = cms.double(3),
    max_pT_dRcut = cms.double(0.1),
    b_dR = cms.double(0.6263),
    a_pT = cms.double(0.005263),
    deltaR = cms.double(-1.0),
    maximumDecayLength = cms.double(5.0),
    impactParameterType = cms.int32(0),
    trackQualityClass = cms.string('any'),
    useVariableJTA = cms.bool(False),
    maximumDistanceToJetAxis = cms.double(0.07),
    nthTrack = cms.int32(3)
)


process.positiveCombinedMVA = cms.ESProducer("CombinedMVAJetTagESProducer",
    useCategories = cms.bool(False),
    calibrationRecord = cms.string('CombinedMVA'),
    jetTagComputers = cms.VPSet(cms.PSet(
        discriminator = cms.bool(True),
        variables = cms.bool(False),
        jetTagComputer = cms.string('positiveOnlyJetProbability')
    ), 
        cms.PSet(
            discriminator = cms.bool(True),
            variables = cms.bool(False),
            jetTagComputer = cms.string('combinedSecondaryVertexPositive')
        ), 
        cms.PSet(
            discriminator = cms.bool(True),
            variables = cms.bool(False),
            jetTagComputer = cms.string('positiveSoftPFMuon')
        ), 
        cms.PSet(
            discriminator = cms.bool(True),
            variables = cms.bool(False),
            jetTagComputer = cms.string('positiveSoftPFElectron')
        ))
)


process.positiveOnlyJetBProbability = cms.ESProducer("JetBProbabilityESProducer",
    b_pT = cms.double(0.3684),
    a_dR = cms.double(-0.001053),
    min_pT = cms.double(120),
    max_pT = cms.double(500),
    min_pT_dRcut = cms.double(0.5),
    max_pT_trackPTcut = cms.double(3),
    max_pT_dRcut = cms.double(0.1),
    b_dR = cms.double(0.6263),
    a_pT = cms.double(0.005263),
    deltaR = cms.double(-1.0),
    maximumDistanceToJetAxis = cms.double(0.07),
    impactParameterType = cms.int32(0),
    trackQualityClass = cms.string('any'),
    useVariableJTA = cms.bool(False),
    trackIpSign = cms.int32(1),
    minimumProbability = cms.double(0.005),
    numberOfBTracks = cms.uint32(4),
    maximumDecayLength = cms.double(5.0)
)


process.positiveOnlyJetProbability = cms.ESProducer("JetProbabilityESProducer",
    b_pT = cms.double(0.3684),
    a_dR = cms.double(-0.001053),
    min_pT = cms.double(120),
    max_pT = cms.double(500),
    min_pT_dRcut = cms.double(0.5),
    max_pT_trackPTcut = cms.double(3),
    max_pT_dRcut = cms.double(0.1),
    b_dR = cms.double(0.6263),
    a_pT = cms.double(0.005263),
    deltaR = cms.double(0.3),
    maximumDistanceToJetAxis = cms.double(0.07),
    impactParameterType = cms.int32(0),
    trackQualityClass = cms.string('any'),
    useVariableJTA = cms.bool(False),
    trackIpSign = cms.int32(1),
    minimumProbability = cms.double(0.005),
    maximumDecayLength = cms.double(5.0)
)


process.positiveSoftPFElectron = cms.ESProducer("ElectronTaggerESProducer",
    ipSign = cms.string('positive')
)


process.positiveSoftPFElectronByIP2d = cms.ESProducer("LeptonTaggerByIPESProducer",
    use3d = cms.bool(False),
    ipSign = cms.string('positive')
)


process.positiveSoftPFElectronByIP3d = cms.ESProducer("LeptonTaggerByIPESProducer",
    use3d = cms.bool(True),
    ipSign = cms.string('positive')
)


process.positiveSoftPFElectronByPt = cms.ESProducer("LeptonTaggerByPtESProducer",
    ipSign = cms.string('positive')
)


process.positiveSoftPFMuon = cms.ESProducer("MuonTaggerESProducer",
    ipSign = cms.string('positive')
)


process.positiveSoftPFMuonByIP2d = cms.ESProducer("LeptonTaggerByIPESProducer",
    use3d = cms.bool(False),
    ipSign = cms.string('positive')
)


process.positiveSoftPFMuonByIP3d = cms.ESProducer("LeptonTaggerByIPESProducer",
    use3d = cms.bool(True),
    ipSign = cms.string('positive')
)


process.positiveSoftPFMuonByPt = cms.ESProducer("LeptonTaggerByPtESProducer",
    ipSign = cms.string('positive')
)


process.siPixelQualityESProducer = cms.ESProducer("SiPixelQualityESProducer",
    ListOfRecordToMerge = cms.VPSet(cms.PSet(
        record = cms.string('SiPixelQualityFromDbRcd'),
        tag = cms.string('')
    ), 
        cms.PSet(
            record = cms.string('SiPixelDetVOffRcd'),
            tag = cms.string('')
        ))
)


process.siStripBackPlaneCorrectionDepESProducer = cms.ESProducer("SiStripBackPlaneCorrectionDepESProducer",
    LatencyRecord = cms.PSet(
        record = cms.string('SiStripLatencyRcd'),
        label = cms.untracked.string('')
    ),
    BackPlaneCorrectionDeconvMode = cms.PSet(
        record = cms.string('SiStripBackPlaneCorrectionRcd'),
        label = cms.untracked.string('deconvolution')
    ),
    BackPlaneCorrectionPeakMode = cms.PSet(
        record = cms.string('SiStripBackPlaneCorrectionRcd'),
        label = cms.untracked.string('peak')
    )
)


process.siStripGainESProducer = cms.ESProducer("SiStripGainESProducer",
    printDebug = cms.untracked.bool(False),
    appendToDataLabel = cms.string(''),
    APVGain = cms.VPSet(cms.PSet(
        Record = cms.string('SiStripApvGainRcd'),
        NormalizationFactor = cms.untracked.double(1.0),
        Label = cms.untracked.string('')
    ), 
        cms.PSet(
            Record = cms.string('SiStripApvGain2Rcd'),
            NormalizationFactor = cms.untracked.double(1.0),
            Label = cms.untracked.string('')
        )),
    AutomaticNormalization = cms.bool(False)
)


process.siStripLorentzAngleDepESProducer = cms.ESProducer("SiStripLorentzAngleDepESProducer",
    LatencyRecord = cms.PSet(
        record = cms.string('SiStripLatencyRcd'),
        label = cms.untracked.string('')
    ),
    LorentzAngleDeconvMode = cms.PSet(
        record = cms.string('SiStripLorentzAngleRcd'),
        label = cms.untracked.string('deconvolution')
    ),
    LorentzAnglePeakMode = cms.PSet(
        record = cms.string('SiStripLorentzAngleRcd'),
        label = cms.untracked.string('peak')
    )
)


process.siStripQualityESProducer = cms.ESProducer("SiStripQualityESProducer",
    appendToDataLabel = cms.string(''),
    PrintDebugOutput = cms.bool(False),
    ThresholdForReducedGranularity = cms.double(0.3),
    UseEmptyRunInfo = cms.bool(False),
    ReduceGranularity = cms.bool(False),
    ListOfRecordToMerge = cms.VPSet(cms.PSet(
        record = cms.string('SiStripDetVOffRcd'),
        tag = cms.string('')
    ), 
        cms.PSet(
            record = cms.string('SiStripDetCablingRcd'),
            tag = cms.string('')
        ), 
        cms.PSet(
            record = cms.string('RunInfoRcd'),
            tag = cms.string('')
        ), 
        cms.PSet(
            record = cms.string('SiStripBadChannelRcd'),
            tag = cms.string('')
        ), 
        cms.PSet(
            record = cms.string('SiStripBadFiberRcd'),
            tag = cms.string('')
        ), 
        cms.PSet(
            record = cms.string('SiStripBadModuleRcd'),
            tag = cms.string('')
        ), 
        cms.PSet(
            record = cms.string('SiStripBadStripRcd'),
            tag = cms.string('')
        ))
)


process.simpleSecondaryVertex2Trk = cms.ESProducer("SimpleSecondaryVertexESProducer",
    minTracks = cms.uint32(2),
    unBoost = cms.bool(False),
    useSignificance = cms.bool(True),
    use3d = cms.bool(True)
)


process.simpleSecondaryVertex3Trk = cms.ESProducer("SimpleSecondaryVertexESProducer",
    minTracks = cms.uint32(3),
    unBoost = cms.bool(False),
    useSignificance = cms.bool(True),
    use3d = cms.bool(True)
)


process.sistripconn = cms.ESProducer("SiStripConnectivity")


process.softPFElectron = cms.ESProducer("ElectronTaggerESProducer",
    ipSign = cms.string('any')
)


process.softPFElectronByIP2d = cms.ESProducer("LeptonTaggerByIPESProducer",
    use3d = cms.bool(False),
    ipSign = cms.string('any')
)


process.softPFElectronByIP3d = cms.ESProducer("LeptonTaggerByIPESProducer",
    use3d = cms.bool(True),
    ipSign = cms.string('any')
)


process.softPFElectronByPt = cms.ESProducer("LeptonTaggerByPtESProducer",
    ipSign = cms.string('any')
)


process.softPFMuon = cms.ESProducer("MuonTaggerESProducer",
    ipSign = cms.string('any')
)


process.softPFMuonByIP2d = cms.ESProducer("LeptonTaggerByIPESProducer",
    use3d = cms.bool(False),
    ipSign = cms.string('any')
)


process.softPFMuonByIP3d = cms.ESProducer("LeptonTaggerByIPESProducer",
    use3d = cms.bool(True),
    ipSign = cms.string('any')
)


process.softPFMuonByPt = cms.ESProducer("LeptonTaggerByPtESProducer",
    ipSign = cms.string('any')
)


process.stripCPEESProducer = cms.ESProducer("StripCPEESProducer",
    ComponentType = cms.string('SimpleStripCPE'),
    ComponentName = cms.string('stripCPE'),
    parameters = cms.PSet(

    )
)


process.trackCounting3D2nd = cms.ESProducer("TrackCountingESProducer",
    b_pT = cms.double(0.3684),
    a_dR = cms.double(-0.001053),
    min_pT = cms.double(120),
    max_pT = cms.double(500),
    min_pT_dRcut = cms.double(0.5),
    max_pT_trackPTcut = cms.double(3),
    max_pT_dRcut = cms.double(0.1),
    b_dR = cms.double(0.6263),
    a_pT = cms.double(0.005263),
    deltaR = cms.double(-1.0),
    maximumDecayLength = cms.double(5.0),
    impactParameterType = cms.int32(0),
    trackQualityClass = cms.string('any'),
    useVariableJTA = cms.bool(False),
    maximumDistanceToJetAxis = cms.double(0.07),
    nthTrack = cms.int32(2)
)


process.trackCounting3D3rd = cms.ESProducer("TrackCountingESProducer",
    b_pT = cms.double(0.3684),
    a_dR = cms.double(-0.001053),
    min_pT = cms.double(120),
    max_pT = cms.double(500),
    min_pT_dRcut = cms.double(0.5),
    max_pT_trackPTcut = cms.double(3),
    max_pT_dRcut = cms.double(0.1),
    b_dR = cms.double(0.6263),
    a_pT = cms.double(0.005263),
    deltaR = cms.double(-1.0),
    maximumDecayLength = cms.double(5.0),
    impactParameterType = cms.int32(0),
    trackQualityClass = cms.string('any'),
    useVariableJTA = cms.bool(False),
    maximumDistanceToJetAxis = cms.double(0.07),
    nthTrack = cms.int32(3)
)


process.trackerGeometry = cms.ESProducer("TrackerDigiGeometryESModule",
    appendToDataLabel = cms.string(''),
    fromDDD = cms.bool(True),
    trackerGeometryConstants = cms.PSet(
        ROCS_X = cms.int32(0),
        ROCS_Y = cms.int32(0),
        upgradeGeometry = cms.bool(False),
        BIG_PIX_PER_ROC_Y = cms.int32(2),
        BIG_PIX_PER_ROC_X = cms.int32(1),
        ROWS_PER_ROC = cms.int32(80),
        COLS_PER_ROC = cms.int32(52)
    ),
    applyAlignment = cms.bool(True),
    alignmentsLabel = cms.string('')
)


process.trackerNumberingGeometry = cms.ESProducer("TrackerGeometricDetESModule",
    appendToDataLabel = cms.string(''),
    fromDDD = cms.bool(True),
    layerNumberPXB = cms.uint32(16),
    totalBlade = cms.uint32(24)
)


process.trackerTopologyConstants = cms.ESProducer("TrackerTopologyEP",
    tob_rodStartBit = cms.uint32(5),
    tib_str_int_extStartBit = cms.uint32(10),
    tib_layerMask = cms.uint32(7),
    pxf_bladeMask = cms.uint32(63),
    appendToDataLabel = cms.string(''),
    pxb_ladderStartBit = cms.uint32(8),
    pxb_layerStartBit = cms.uint32(16),
    tec_wheelStartBit = cms.uint32(14),
    tib_str_int_extMask = cms.uint32(3),
    tec_ringStartBit = cms.uint32(5),
    tib_moduleStartBit = cms.uint32(2),
    tib_sterMask = cms.uint32(3),
    tid_sideStartBit = cms.uint32(13),
    tid_module_fw_bwStartBit = cms.uint32(7),
    tid_ringMask = cms.uint32(3),
    tob_sterMask = cms.uint32(3),
    tec_petal_fw_bwStartBit = cms.uint32(12),
    tec_ringMask = cms.uint32(7),
    tib_strMask = cms.uint32(63),
    tec_sterMask = cms.uint32(3),
    tec_wheelMask = cms.uint32(15),
    tec_sideStartBit = cms.uint32(18),
    pxb_moduleMask = cms.uint32(63),
    pxf_panelStartBit = cms.uint32(8),
    tid_sideMask = cms.uint32(3),
    tob_moduleMask = cms.uint32(7),
    tid_ringStartBit = cms.uint32(9),
    pxf_sideMask = cms.uint32(3),
    pxb_moduleStartBit = cms.uint32(2),
    pxf_diskStartBit = cms.uint32(16),
    tib_str_fw_bwMask = cms.uint32(3),
    tec_moduleMask = cms.uint32(7),
    tid_sterMask = cms.uint32(3),
    tob_rod_fw_bwMask = cms.uint32(3),
    tob_layerStartBit = cms.uint32(14),
    tec_petal_fw_bwMask = cms.uint32(3),
    tib_strStartBit = cms.uint32(4),
    tec_sterStartBit = cms.uint32(0),
    tid_moduleMask = cms.uint32(31),
    tib_sterStartBit = cms.uint32(0),
    tid_sterStartBit = cms.uint32(0),
    pxf_moduleStartBit = cms.uint32(2),
    pxf_diskMask = cms.uint32(15),
    tob_moduleStartBit = cms.uint32(2),
    tid_wheelStartBit = cms.uint32(11),
    tob_layerMask = cms.uint32(7),
    tid_module_fw_bwMask = cms.uint32(3),
    tob_rod_fw_bwStartBit = cms.uint32(12),
    tec_petalMask = cms.uint32(15),
    pxb_ladderMask = cms.uint32(255),
    tec_moduleStartBit = cms.uint32(2),
    tob_rodMask = cms.uint32(127),
    tec_sideMask = cms.uint32(3),
    pxf_sideStartBit = cms.uint32(23),
    pxb_layerMask = cms.uint32(15),
    tib_layerStartBit = cms.uint32(14),
    pxf_panelMask = cms.uint32(3),
    tib_moduleMask = cms.uint32(3),
    pxf_bladeStartBit = cms.uint32(10),
    tid_wheelMask = cms.uint32(3),
    tob_sterStartBit = cms.uint32(0),
    tid_moduleStartBit = cms.uint32(2),
    tec_petalStartBit = cms.uint32(8),
    tib_str_fw_bwStartBit = cms.uint32(12),
    pxf_moduleMask = cms.uint32(63)
)


process.BTagRecord = cms.ESSource("EmptyESSource",
    iovIsRunNotTime = cms.bool(True),
    recordName = cms.string('JetTagComputerRecord'),
    firstValid = cms.vuint32(1)
)


process.GlobalTag = cms.ESSource("PoolDBESSource",
    DBParameters = cms.PSet(
        authenticationPath = cms.untracked.string(''),
        enableReadOnlySessionOnUpdateConnection = cms.untracked.bool(False),
        idleConnectionCleanupPeriod = cms.untracked.int32(10),
        messageLevel = cms.untracked.int32(0),
        enablePoolAutomaticCleanUp = cms.untracked.bool(False),
        enableConnectionSharing = cms.untracked.bool(True),
        connectionRetrialTimeOut = cms.untracked.int32(60),
        connectionTimeOut = cms.untracked.int32(60),
        authenticationSystem = cms.untracked.int32(0),
        connectionRetrialPeriod = cms.untracked.int32(10)
    ),
    BlobStreamerName = cms.untracked.string('TBufferBlobStreamingService'),
    toGet = cms.VPSet(),
    connect = cms.string('frontier://FrontierProd/CMS_COND_31X_GLOBALTAG'),
    globaltag = cms.string('PHYS14_25_V1::All')
)


process.HepPDTESSource = cms.ESSource("HepPDTESSource",
    pdtFileName = cms.FileInPath('SimGeneral/HepPDTESSource/data/pythiaparticle.tbl')
)


process.XMLIdealGeometryESSource = cms.ESSource("XMLIdealGeometryESSource",
    geomXMLFiles = cms.vstring('Geometry/CMSCommonData/data/materials.xml', 
        'Geometry/CMSCommonData/data/rotations.xml', 
        'Geometry/CMSCommonData/data/normal/cmsextent.xml', 
        'Geometry/CMSCommonData/data/cms.xml', 
        'Geometry/CMSCommonData/data/cmsMother.xml', 
        'Geometry/CMSCommonData/data/cmsTracker.xml', 
        'Geometry/CMSCommonData/data/caloBase.xml', 
        'Geometry/CMSCommonData/data/cmsCalo.xml', 
        'Geometry/CMSCommonData/data/muonBase.xml', 
        'Geometry/CMSCommonData/data/cmsMuon.xml', 
        'Geometry/CMSCommonData/data/mgnt.xml', 
        'Geometry/CMSCommonData/data/beampipe.xml', 
        'Geometry/CMSCommonData/data/cmsBeam.xml', 
        'Geometry/CMSCommonData/data/muonMB.xml', 
        'Geometry/CMSCommonData/data/muonMagnet.xml', 
        'Geometry/TrackerCommonData/data/pixfwdMaterials.xml', 
        'Geometry/TrackerCommonData/data/pixfwdCommon.xml', 
        'Geometry/TrackerCommonData/data/pixfwdPlaq.xml', 
        'Geometry/TrackerCommonData/data/pixfwdPlaq1x2.xml', 
        'Geometry/TrackerCommonData/data/pixfwdPlaq1x5.xml', 
        'Geometry/TrackerCommonData/data/pixfwdPlaq2x3.xml', 
        'Geometry/TrackerCommonData/data/pixfwdPlaq2x4.xml', 
        'Geometry/TrackerCommonData/data/pixfwdPlaq2x5.xml', 
        'Geometry/TrackerCommonData/data/pixfwdPanelBase.xml', 
        'Geometry/TrackerCommonData/data/pixfwdPanel.xml', 
        'Geometry/TrackerCommonData/data/pixfwdBlade.xml', 
        'Geometry/TrackerCommonData/data/pixfwdNipple.xml', 
        'Geometry/TrackerCommonData/data/pixfwdDisk.xml', 
        'Geometry/TrackerCommonData/data/pixfwdCylinder.xml', 
        'Geometry/TrackerCommonData/data/pixfwd.xml', 
        'Geometry/TrackerCommonData/data/pixbarmaterial.xml', 
        'Geometry/TrackerCommonData/data/pixbarladder.xml', 
        'Geometry/TrackerCommonData/data/pixbarladderfull.xml', 
        'Geometry/TrackerCommonData/data/pixbarladderhalf.xml', 
        'Geometry/TrackerCommonData/data/pixbarlayer.xml', 
        'Geometry/TrackerCommonData/data/pixbarlayer0.xml', 
        'Geometry/TrackerCommonData/data/pixbarlayer1.xml', 
        'Geometry/TrackerCommonData/data/pixbarlayer2.xml', 
        'Geometry/TrackerCommonData/data/pixbar.xml', 
        'Geometry/TrackerCommonData/data/tibtidcommonmaterial.xml', 
        'Geometry/TrackerCommonData/data/tibmaterial.xml', 
        'Geometry/TrackerCommonData/data/tibmodpar.xml', 
        'Geometry/TrackerCommonData/data/tibmodule0.xml', 
        'Geometry/TrackerCommonData/data/tibmodule0a.xml', 
        'Geometry/TrackerCommonData/data/tibmodule0b.xml', 
        'Geometry/TrackerCommonData/data/tibmodule2.xml', 
        'Geometry/TrackerCommonData/data/tibstringpar.xml', 
        'Geometry/TrackerCommonData/data/tibstring0ll.xml', 
        'Geometry/TrackerCommonData/data/tibstring0lr.xml', 
        'Geometry/TrackerCommonData/data/tibstring0ul.xml', 
        'Geometry/TrackerCommonData/data/tibstring0ur.xml', 
        'Geometry/TrackerCommonData/data/tibstring0.xml', 
        'Geometry/TrackerCommonData/data/tibstring1ll.xml', 
        'Geometry/TrackerCommonData/data/tibstring1lr.xml', 
        'Geometry/TrackerCommonData/data/tibstring1ul.xml', 
        'Geometry/TrackerCommonData/data/tibstring1ur.xml', 
        'Geometry/TrackerCommonData/data/tibstring1.xml', 
        'Geometry/TrackerCommonData/data/tibstring2ll.xml', 
        'Geometry/TrackerCommonData/data/tibstring2lr.xml', 
        'Geometry/TrackerCommonData/data/tibstring2ul.xml', 
        'Geometry/TrackerCommonData/data/tibstring2ur.xml', 
        'Geometry/TrackerCommonData/data/tibstring2.xml', 
        'Geometry/TrackerCommonData/data/tibstring3ll.xml', 
        'Geometry/TrackerCommonData/data/tibstring3lr.xml', 
        'Geometry/TrackerCommonData/data/tibstring3ul.xml', 
        'Geometry/TrackerCommonData/data/tibstring3ur.xml', 
        'Geometry/TrackerCommonData/data/tibstring3.xml', 
        'Geometry/TrackerCommonData/data/tiblayerpar.xml', 
        'Geometry/TrackerCommonData/data/tiblayer0.xml', 
        'Geometry/TrackerCommonData/data/tiblayer1.xml', 
        'Geometry/TrackerCommonData/data/tiblayer2.xml', 
        'Geometry/TrackerCommonData/data/tiblayer3.xml', 
        'Geometry/TrackerCommonData/data/tib.xml', 
        'Geometry/TrackerCommonData/data/tidmaterial.xml', 
        'Geometry/TrackerCommonData/data/tidmodpar.xml', 
        'Geometry/TrackerCommonData/data/tidmodule0.xml', 
        'Geometry/TrackerCommonData/data/tidmodule0r.xml', 
        'Geometry/TrackerCommonData/data/tidmodule0l.xml', 
        'Geometry/TrackerCommonData/data/tidmodule1.xml', 
        'Geometry/TrackerCommonData/data/tidmodule1r.xml', 
        'Geometry/TrackerCommonData/data/tidmodule1l.xml', 
        'Geometry/TrackerCommonData/data/tidmodule2.xml', 
        'Geometry/TrackerCommonData/data/tidringpar.xml', 
        'Geometry/TrackerCommonData/data/tidring0.xml', 
        'Geometry/TrackerCommonData/data/tidring0f.xml', 
        'Geometry/TrackerCommonData/data/tidring0b.xml', 
        'Geometry/TrackerCommonData/data/tidring1.xml', 
        'Geometry/TrackerCommonData/data/tidring1f.xml', 
        'Geometry/TrackerCommonData/data/tidring1b.xml', 
        'Geometry/TrackerCommonData/data/tidring2.xml', 
        'Geometry/TrackerCommonData/data/tid.xml', 
        'Geometry/TrackerCommonData/data/tidf.xml', 
        'Geometry/TrackerCommonData/data/tidb.xml', 
        'Geometry/TrackerCommonData/data/tibtidservices.xml', 
        'Geometry/TrackerCommonData/data/tibtidservicesf.xml', 
        'Geometry/TrackerCommonData/data/tibtidservicesb.xml', 
        'Geometry/TrackerCommonData/data/tobmaterial.xml', 
        'Geometry/TrackerCommonData/data/tobmodpar.xml', 
        'Geometry/TrackerCommonData/data/tobmodule0.xml', 
        'Geometry/TrackerCommonData/data/tobmodule2.xml', 
        'Geometry/TrackerCommonData/data/tobmodule4.xml', 
        'Geometry/TrackerCommonData/data/tobrodpar.xml', 
        'Geometry/TrackerCommonData/data/tobrod0c.xml', 
        'Geometry/TrackerCommonData/data/tobrod0l.xml', 
        'Geometry/TrackerCommonData/data/tobrod0h.xml', 
        'Geometry/TrackerCommonData/data/tobrod0.xml', 
        'Geometry/TrackerCommonData/data/tobrod1l.xml', 
        'Geometry/TrackerCommonData/data/tobrod1h.xml', 
        'Geometry/TrackerCommonData/data/tobrod1.xml', 
        'Geometry/TrackerCommonData/data/tobrod2c.xml', 
        'Geometry/TrackerCommonData/data/tobrod2l.xml', 
        'Geometry/TrackerCommonData/data/tobrod2h.xml', 
        'Geometry/TrackerCommonData/data/tobrod2.xml', 
        'Geometry/TrackerCommonData/data/tobrod3l.xml', 
        'Geometry/TrackerCommonData/data/tobrod3h.xml', 
        'Geometry/TrackerCommonData/data/tobrod3.xml', 
        'Geometry/TrackerCommonData/data/tobrod4c.xml', 
        'Geometry/TrackerCommonData/data/tobrod4l.xml', 
        'Geometry/TrackerCommonData/data/tobrod4h.xml', 
        'Geometry/TrackerCommonData/data/tobrod4.xml', 
        'Geometry/TrackerCommonData/data/tobrod5l.xml', 
        'Geometry/TrackerCommonData/data/tobrod5h.xml', 
        'Geometry/TrackerCommonData/data/tobrod5.xml', 
        'Geometry/TrackerCommonData/data/tob.xml', 
        'Geometry/TrackerCommonData/data/tecmaterial.xml', 
        'Geometry/TrackerCommonData/data/tecmodpar.xml', 
        'Geometry/TrackerCommonData/data/tecmodule0.xml', 
        'Geometry/TrackerCommonData/data/tecmodule0r.xml', 
        'Geometry/TrackerCommonData/data/tecmodule0s.xml', 
        'Geometry/TrackerCommonData/data/tecmodule1.xml', 
        'Geometry/TrackerCommonData/data/tecmodule1r.xml', 
        'Geometry/TrackerCommonData/data/tecmodule1s.xml', 
        'Geometry/TrackerCommonData/data/tecmodule2.xml', 
        'Geometry/TrackerCommonData/data/tecmodule3.xml', 
        'Geometry/TrackerCommonData/data/tecmodule4.xml', 
        'Geometry/TrackerCommonData/data/tecmodule4r.xml', 
        'Geometry/TrackerCommonData/data/tecmodule4s.xml', 
        'Geometry/TrackerCommonData/data/tecmodule5.xml', 
        'Geometry/TrackerCommonData/data/tecmodule6.xml', 
        'Geometry/TrackerCommonData/data/tecpetpar.xml', 
        'Geometry/TrackerCommonData/data/tecring0.xml', 
        'Geometry/TrackerCommonData/data/tecring1.xml', 
        'Geometry/TrackerCommonData/data/tecring2.xml', 
        'Geometry/TrackerCommonData/data/tecring3.xml', 
        'Geometry/TrackerCommonData/data/tecring4.xml', 
        'Geometry/TrackerCommonData/data/tecring5.xml', 
        'Geometry/TrackerCommonData/data/tecring6.xml', 
        'Geometry/TrackerCommonData/data/tecring0f.xml', 
        'Geometry/TrackerCommonData/data/tecring1f.xml', 
        'Geometry/TrackerCommonData/data/tecring2f.xml', 
        'Geometry/TrackerCommonData/data/tecring3f.xml', 
        'Geometry/TrackerCommonData/data/tecring4f.xml', 
        'Geometry/TrackerCommonData/data/tecring5f.xml', 
        'Geometry/TrackerCommonData/data/tecring6f.xml', 
        'Geometry/TrackerCommonData/data/tecring0b.xml', 
        'Geometry/TrackerCommonData/data/tecring1b.xml', 
        'Geometry/TrackerCommonData/data/tecring2b.xml', 
        'Geometry/TrackerCommonData/data/tecring3b.xml', 
        'Geometry/TrackerCommonData/data/tecring4b.xml', 
        'Geometry/TrackerCommonData/data/tecring5b.xml', 
        'Geometry/TrackerCommonData/data/tecring6b.xml', 
        'Geometry/TrackerCommonData/data/tecpetalf.xml', 
        'Geometry/TrackerCommonData/data/tecpetalb.xml', 
        'Geometry/TrackerCommonData/data/tecpetal0.xml', 
        'Geometry/TrackerCommonData/data/tecpetal0f.xml', 
        'Geometry/TrackerCommonData/data/tecpetal0b.xml', 
        'Geometry/TrackerCommonData/data/tecpetal3.xml', 
        'Geometry/TrackerCommonData/data/tecpetal3f.xml', 
        'Geometry/TrackerCommonData/data/tecpetal3b.xml', 
        'Geometry/TrackerCommonData/data/tecpetal6f.xml', 
        'Geometry/TrackerCommonData/data/tecpetal6b.xml', 
        'Geometry/TrackerCommonData/data/tecpetal8f.xml', 
        'Geometry/TrackerCommonData/data/tecpetal8b.xml', 
        'Geometry/TrackerCommonData/data/tecwheel.xml', 
        'Geometry/TrackerCommonData/data/tecwheela.xml', 
        'Geometry/TrackerCommonData/data/tecwheelb.xml', 
        'Geometry/TrackerCommonData/data/tecwheelc.xml', 
        'Geometry/TrackerCommonData/data/tecwheeld.xml', 
        'Geometry/TrackerCommonData/data/tecwheel6.xml', 
        'Geometry/TrackerCommonData/data/tecservices.xml', 
        'Geometry/TrackerCommonData/data/tecbackplate.xml', 
        'Geometry/TrackerCommonData/data/tec.xml', 
        'Geometry/TrackerCommonData/data/trackermaterial.xml', 
        'Geometry/TrackerCommonData/data/tracker.xml', 
        'Geometry/TrackerCommonData/data/trackerpixbar.xml', 
        'Geometry/TrackerCommonData/data/trackerpixfwd.xml', 
        'Geometry/TrackerCommonData/data/trackertibtidservices.xml', 
        'Geometry/TrackerCommonData/data/trackertib.xml', 
        'Geometry/TrackerCommonData/data/trackertid.xml', 
        'Geometry/TrackerCommonData/data/trackertob.xml', 
        'Geometry/TrackerCommonData/data/trackertec.xml', 
        'Geometry/TrackerCommonData/data/trackerbulkhead.xml', 
        'Geometry/TrackerCommonData/data/trackerother.xml', 
        'Geometry/EcalCommonData/data/eregalgo.xml', 
        'Geometry/EcalCommonData/data/ebalgo.xml', 
        'Geometry/EcalCommonData/data/ebcon.xml', 
        'Geometry/EcalCommonData/data/ebrot.xml', 
        'Geometry/EcalCommonData/data/eecon.xml', 
        'Geometry/EcalCommonData/data/eefixed.xml', 
        'Geometry/EcalCommonData/data/eehier.xml', 
        'Geometry/EcalCommonData/data/eealgo.xml', 
        'Geometry/EcalCommonData/data/escon.xml', 
        'Geometry/EcalCommonData/data/esalgo.xml', 
        'Geometry/EcalCommonData/data/eeF.xml', 
        'Geometry/EcalCommonData/data/eeB.xml', 
        'Geometry/HcalCommonData/data/hcalrotations.xml', 
        'Geometry/HcalCommonData/data/hcalalgo.xml', 
        'Geometry/HcalCommonData/data/hcalbarrelalgo.xml', 
        'Geometry/HcalCommonData/data/hcalendcapalgo.xml', 
        'Geometry/HcalCommonData/data/hcalouteralgo.xml', 
        'Geometry/HcalCommonData/data/hcalforwardalgo.xml', 
        'Geometry/HcalCommonData/data/average/hcalforwardmaterial.xml', 
        'Geometry/MuonCommonData/data/mbCommon.xml', 
        'Geometry/MuonCommonData/data/mb1.xml', 
        'Geometry/MuonCommonData/data/mb2.xml', 
        'Geometry/MuonCommonData/data/mb3.xml', 
        'Geometry/MuonCommonData/data/mb4.xml', 
        'Geometry/MuonCommonData/data/muonYoke.xml', 
        'Geometry/MuonCommonData/data/mf.xml', 
        'Geometry/ForwardCommonData/data/forward.xml', 
        'Geometry/ForwardCommonData/data/bundle/forwardshield.xml', 
        'Geometry/ForwardCommonData/data/brmrotations.xml', 
        'Geometry/ForwardCommonData/data/brm.xml', 
        'Geometry/ForwardCommonData/data/totemMaterials.xml', 
        'Geometry/ForwardCommonData/data/totemRotations.xml', 
        'Geometry/ForwardCommonData/data/totemt1.xml', 
        'Geometry/ForwardCommonData/data/totemt2.xml', 
        'Geometry/ForwardCommonData/data/ionpump.xml', 
        'Geometry/MuonCommonData/data/muonNumbering.xml', 
        'Geometry/TrackerCommonData/data/trackerStructureTopology.xml', 
        'Geometry/TrackerSimData/data/trackersens.xml', 
        'Geometry/TrackerRecoData/data/trackerRecoMaterial.xml', 
        'Geometry/EcalSimData/data/ecalsens.xml', 
        'Geometry/HcalCommonData/data/hcalsenspmf.xml', 
        'Geometry/HcalSimData/data/hf.xml', 
        'Geometry/HcalSimData/data/hfpmt.xml', 
        'Geometry/HcalSimData/data/hffibrebundle.xml', 
        'Geometry/HcalSimData/data/CaloUtil.xml', 
        'Geometry/MuonSimData/data/muonSens.xml', 
        'Geometry/DTGeometryBuilder/data/dtSpecsFilter.xml', 
        'Geometry/CSCGeometryBuilder/data/cscSpecsFilter.xml', 
        'Geometry/CSCGeometryBuilder/data/cscSpecs.xml', 
        'Geometry/RPCGeometryBuilder/data/RPCSpecs.xml', 
        'Geometry/ForwardCommonData/data/brmsens.xml', 
        'Geometry/HcalSimData/data/HcalProdCuts.xml', 
        'Geometry/EcalSimData/data/EcalProdCuts.xml', 
        'Geometry/EcalSimData/data/ESProdCuts.xml', 
        'Geometry/TrackerSimData/data/trackerProdCuts.xml', 
        'Geometry/TrackerSimData/data/trackerProdCutsBEAM.xml', 
        'Geometry/MuonSimData/data/muonProdCuts.xml', 
        'Geometry/ForwardSimData/data/ForwardShieldProdCuts.xml', 
        'Geometry/CMSCommonData/data/FieldParameters.xml'),
    rootNodeName = cms.string('cms:OCMS')
)


process.eegeom = cms.ESSource("EmptyESSource",
    iovIsRunNotTime = cms.bool(True),
    recordName = cms.string('EcalMappingRcd'),
    firstValid = cms.vuint32(1)
)


process.es_hardcode = cms.ESSource("HcalHardcodeCalibrations",
    HcalReLabel = cms.PSet(
        RelabelRules = cms.untracked.PSet(
            Eta16 = cms.untracked.vint32(1, 1, 2, 2, 2, 
                2, 2, 2, 2, 3, 
                3, 3, 3, 3, 3, 
                3, 3, 3, 3),
            Eta17 = cms.untracked.vint32(1, 1, 2, 2, 3, 
                3, 3, 4, 4, 4, 
                4, 4, 5, 5, 5, 
                5, 5, 5, 5),
            Eta1 = cms.untracked.vint32(1, 2, 2, 2, 3, 
                3, 3, 3, 3, 3, 
                3, 3, 3, 3, 3, 
                3, 3, 3, 3),
            CorrectPhi = cms.untracked.bool(False)
        ),
        RelabelHits = cms.untracked.bool(False)
    ),
    HERecalibration = cms.bool(False),
    toGet = cms.untracked.vstring('GainWidths'),
    GainWidthsForTrigPrims = cms.bool(False),
    HEreCalibCutoff = cms.double(20.0),
    HFRecalibration = cms.bool(False),
    iLumi = cms.double(-1.0),
    hcalTopologyConstants = cms.PSet(
        maxDepthHE = cms.int32(3),
        maxDepthHB = cms.int32(2),
        mode = cms.string('HcalTopologyMode::LHC')
    )
)


process.magfield = cms.ESSource("XMLIdealGeometryESSource",
    geomXMLFiles = cms.vstring('Geometry/CMSCommonData/data/normal/cmsextent.xml', 
        'Geometry/CMSCommonData/data/cms.xml', 
        'Geometry/CMSCommonData/data/cmsMagneticField.xml', 
        'MagneticField/GeomBuilder/data/MagneticFieldVolumes_1103l.xml', 
        'Geometry/CMSCommonData/data/materials.xml'),
    rootNodeName = cms.string('cmsMagneticField:MAGF')
)


process.prefer("es_hardcode")

process.prefer("magfield")

process.ALCARECOEventContent = cms.PSet(
    splitLevel = cms.untracked.int32(0),
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
    outputCommands = cms.untracked.vstring( ('drop *', 
        'keep edmTriggerResults_*_*_*', 
        'keep *_ALCARECOTkAlZMuMu_*_*', 
        'keep L1AcceptBunchCrossings_*_*_*', 
        'keep L1GlobalTriggerReadoutRecord_gtDigis_*_*', 
        'keep *_TriggerResults_*_*', 
        'keep DcsStatuss_scalersRawToDigi_*_*', 
        'keep *_offlinePrimaryVertices_*_*', 
        'keep *_ALCARECOTkAlCosmicsInCollisions_*_*', 
        'keep siStripDigis_DetIdCollection_*_*', 
        'keep L1AcceptBunchCrossings_*_*_*', 
        'keep L1GlobalTriggerReadoutRecord_gtDigis_*_*', 
        'keep *_TriggerResults_*_*', 
        'keep DcsStatuss_scalersRawToDigi_*_*', 
        'keep Si*Cluster*_si*Clusters_*_*', 
        'keep recoMuons_muons1Leg_*_*', 
        'keep *_ALCARECOTkAlCosmicsCTF_*_*', 
        'keep *_ALCARECOTkAlCosmicsCosmicTF_*_*', 
        'keep *_ALCARECOTkAlCosmicsRegional_*_*', 
        'keep siStripDigis_DetIdCollection_*_*', 
        'keep L1AcceptBunchCrossings_*_*_*', 
        'keep L1GlobalTriggerReadoutRecord_gtDigis_*_*', 
        'keep *_TriggerResults_*_*', 
        'keep DcsStatuss_scalersRawToDigi_*_*', 
        'keep Si*Cluster*_si*Clusters_*_*', 
        'keep recoMuons_muons1Leg_*_*', 
        'keep *_ALCARECOTkAlCosmicsCTF_*_*', 
        'keep *_ALCARECOTkAlCosmicsCosmicTF_*_*', 
        'keep *_ALCARECOTkAlCosmicsRegional_*_*', 
        'keep siStripDigis_DetIdCollection_*_*', 
        'keep L1AcceptBunchCrossings_*_*_*', 
        'keep L1GlobalTriggerReadoutRecord_gtDigis_*_*', 
        'keep *_TriggerResults_*_*', 
        'keep DcsStatuss_scalersRawToDigi_*_*', 
        'keep Si*Cluster*_si*Clusters_*_*', 
        'keep recoMuons_muons1Leg_*_*', 
        'keep *_ALCARECOTkAlCosmics*0T_*_*', 
        'keep siStripDigis_DetIdCollection_*_*', 
        'keep L1AcceptBunchCrossings_*_*_*', 
        'keep L1GlobalTriggerReadoutRecord_gtDigis_*_*', 
        'keep *_TriggerResults_*_*', 
        'keep DcsStatuss_scalersRawToDigi_*_*', 
        'keep Si*Cluster*_si*Clusters_*_*', 
        'keep recoMuons_muons1Leg_*_*', 
        'keep *_ALCARECOTkAlCosmics*0T_*_*', 
        'keep siStripDigis_DetIdCollection_*_*', 
        'keep L1AcceptBunchCrossings_*_*_*', 
        'keep L1GlobalTriggerReadoutRecord_gtDigis_*_*', 
        'keep *_TriggerResults_*_*', 
        'keep DcsStatuss_scalersRawToDigi_*_*', 
        'keep Si*Cluster*_si*Clusters_*_*', 
        'keep recoMuons_muons1Leg_*_*', 
        'keep *_ALCARECOTkAlLAST0Producer_*_*', 
        'keep DcsStatuss_scalersRawToDigi_*_*', 
        'keep *_ALCARECOTkAlMuonIsolated_*_*', 
        'keep L1AcceptBunchCrossings_*_*_*', 
        'keep L1GlobalTriggerReadoutRecord_gtDigis_*_*', 
        'keep *_TriggerResults_*_*', 
        'keep DcsStatuss_scalersRawToDigi_*_*', 
        'keep *_offlinePrimaryVertices_*_*', 
        'keep *_ALCARECOTkAlMuonIsolatedPA_*_*', 
        'keep L1AcceptBunchCrossings_*_*_*', 
        'keep L1GlobalTriggerReadoutRecord_gtDigis_*_*', 
        'keep *_TriggerResults_*_*', 
        'keep DcsStatuss_scalersRawToDigi_*_*', 
        'keep *_offlinePrimaryVertices_*_*', 
        'keep *_ALCARECOTkAlJpsiMuMu_*_*', 
        'keep L1AcceptBunchCrossings_*_*_*', 
        'keep L1GlobalTriggerReadoutRecord_gtDigis_*_*', 
        'keep *_TriggerResults_*_*', 
        'keep DcsStatuss_scalersRawToDigi_*_*', 
        'keep *_offlinePrimaryVertices_*_*', 
        'keep *_ALCARECOTkAlUpsilonMuMu_*_*', 
        'keep L1AcceptBunchCrossings_*_*_*', 
        'keep L1GlobalTriggerReadoutRecord_gtDigis_*_*', 
        'keep *_TriggerResults_*_*', 
        'keep DcsStatuss_scalersRawToDigi_*_*', 
        'keep *_offlinePrimaryVertices_*_*', 
        'keep *_ALCARECOTkAlMinBias_*_*', 
        'keep L1AcceptBunchCrossings_*_*_*', 
        'keep L1GlobalTriggerReadoutRecord_gtDigis_*_*', 
        'keep *_TriggerResults_*_*', 
        'keep DcsStatuss_scalersRawToDigi_*_*', 
        'keep *_offlinePrimaryVertices_*_*', 
        'keep *_offlineBeamSpot_*_*', 
        'keep *_ALCARECOTkAlBeamHalo_*_*', 
        'keep L1AcceptBunchCrossings_*_*_*', 
        'keep L1GlobalTriggerReadoutRecord_gtDigis_*_*', 
        'keep *_TriggerResults_*_*', 
        'keep DcsStatuss_scalersRawToDigi_*_*', 
        'keep *_ALCARECOSiStripCalZeroBias_*_*', 
        'keep *_calZeroBiasClusters_*_*', 
        'keep L1AcceptBunchCrossings_*_*_*', 
        'keep *_TriggerResults_*_*', 
        'keep *_ALCARECOSiStripCalMinBias_*_*', 
        'keep *_siStripClusters_*_*', 
        'keep *_siPixelClusters_*_*', 
        'keep DetIdedmEDCollection_siStripDigis_*_*', 
        'keep L1AcceptBunchCrossings_*_*_*', 
        'keep L1GlobalTriggerReadoutRecord_gtDigis_*_*', 
        'keep *_TriggerResults_*_*', 
        'keep *_ecalPhiSymCorrected_phiSymEcalRecHitsEB_*', 
        'keep *_ecalPhiSymCorrected_phiSymEcalRecHitsEE_*', 
        'keep L1GlobalTriggerReadoutRecord_hltGtDigis_*_*', 
        'keep *_pfMet_*_*', 
        'keep *_kt6PFJetsForRhoCorrection_rho_*', 
        'keep *_kt6PFJets_rho_*', 
        'keep *_offlinePrimaryVerticesWithBS_*_*', 
        'keep *BeamSpot_offlineBeamSpot_*_*', 
        'keep *_allConversions_*_*', 
        'keep *_conversions_*_*', 
        'keep *GsfTrack*_*_*_*', 
        'keep *_generator_*_*', 
        'keep *_addPileupInfo_*_*', 
        'keep *_genParticles_*_*', 
        'keep recoGsfElectron*_gsfElectron*_*_*', 
        'keep recoCaloClusters_*_*_*', 
        'keep recoSuperClusters_*_*_*', 
        'keep recoPreshowerCluster*_*_*_*', 
        'drop reco*Clusters_hfEMClusters_*_*', 
        'drop reco*Clusters_pfPhotonTranslator_*_*', 
        'drop *_*Unclean*_*_*', 
        'drop *_*unclean*_*_*', 
        'drop *_*_*Unclean*_*', 
        'drop *_*_*unclean*_*', 
        'keep *_*_*_HLT', 
        'keep recoTracks_generalTracks_*_*', 
        'drop *EcalRecHit*_ecalRecHit_*_*', 
        'drop *EcalrecHit*_*ecalPreshowerRecHit*_*EcalRecHitsES*_*', 
        'drop *EcalRecHit*_reducedEcalRecHitsE*_*_*', 
        'keep *EcalRecHit*_alCaIsolatedElectrons_*_*', 
        'keep *EcalRecHit*_reducedEcalRecHitsES_alCaRecHitsES_*', 
        'keep *_ecalPi0Corrected_pi0EcalRecHitsEB_*', 
        'keep *_ecalPi0Corrected_pi0EcalRecHitsEE_*', 
        'keep L1GlobalTriggerReadoutRecord_hltGtDigis_*_*', 
        'keep *_hltAlCaPi0RecHitsFilter_pi0EcalRecHitsES_*', 
        'keep *_ecalEtaCorrected_etaEcalRecHitsEB_*', 
        'keep *_ecalEtaCorrected_etaEcalRecHitsEE_*', 
        'keep L1GlobalTriggerReadoutRecord_hltGtDigis_*_*', 
        'keep *_hltAlCaEtaRecHitsFilter_etaEcalRecHitsES_*', 
        'keep *_DiJProd_*_*', 
        'keep triggerTriggerEvent_*_*_*', 
        'keep *_GammaJetProd_*_*', 
        'keep *_IsoProd_*_*', 
        'keep *_TkAlIsoProd_*_*', 
        'keep *_offlinePrimaryVertices_*_*', 
        'keep edmTriggerResults_*_*_*', 
        'keep triggerTriggerEvent_*_*_*', 
        'keep *_IsoProd_*_*', 
        'keep *_gtDigisAlCaMB_*_*', 
        'keep HBHERecHitsSorted_hbherecoMB_*_*', 
        'keep HORecHitsSorted_horecoMB_*_*', 
        'keep HFRecHitsSorted_hfrecoMB_*_*', 
        'keep HFRecHitsSorted_hfrecoMBspecial_*_*', 
        'keep HBHERecHitsSorted_hbherecoNoise_*_*', 
        'keep HORecHitsSorted_horecoNoise_*_*', 
        'keep HFRecHitsSorted_hfrecoNoise_*_*', 
        'keep *_hoCalibProducer_*_*', 
        'keep HOCalibVariabless_*_*_*', 
        'keep *_HcalNoiseProd_*_*', 
        'keep edmTriggerResults_*_*_HLT', 
        'keep *_ALCARECOMuAlStandAloneCosmics_*_*', 
        'keep *_muonCSCDigis_*_*', 
        'keep *_muonDTDigis_*_*', 
        'keep *_muonRPCDigis_*_*', 
        'keep *_dt1DRecHits_*_*', 
        'keep *_dt2DSegments_*_*', 
        'keep *_dt4DSegments_*_*', 
        'keep *_csc2DRecHits_*_*', 
        'keep *_cscSegments_*_*', 
        'keep *_rpcRecHits_*_*', 
        'keep L1AcceptBunchCrossings_*_*_*', 
        'keep L1GlobalTriggerReadoutRecord_gtDigis_*_*', 
        'keep *_TriggerResults_*_*', 
        'keep DcsStatuss_scalersRawToDigi_*_*', 
        'keep *_ALCARECOMuAlGlobalCosmicsInCollisions_*_*', 
        'keep *_muonCSCDigis_*_*', 
        'keep *_muonDTDigis_*_*', 
        'keep *_muonRPCDigis_*_*', 
        'keep *_dt1DRecHits_*_*', 
        'keep *_dt2DSegments_*_*', 
        'keep *_dt4DSegments_*_*', 
        'keep *_csc2DRecHits_*_*', 
        'keep *_cscSegments_*_*', 
        'keep *_rpcRecHits_*_*', 
        'keep L1AcceptBunchCrossings_*_*_*', 
        'keep L1GlobalTriggerReadoutRecord_gtDigis_*_*', 
        'keep *_TriggerResults_*_*', 
        'keep DcsStatuss_scalersRawToDigi_*_*', 
        'keep *_ALCARECOMuAlGlobalCosmics_*_*', 
        'keep *_muonCSCDigis_*_*', 
        'keep *_muonDTDigis_*_*', 
        'keep *_muonRPCDigis_*_*', 
        'keep *_dt1DRecHits_*_*', 
        'keep *_dt2DSegments_*_*', 
        'keep *_dt4DSegments_*_*', 
        'keep *_csc2DRecHits_*_*', 
        'keep *_cscSegments_*_*', 
        'keep *_rpcRecHits_*_*', 
        'keep L1AcceptBunchCrossings_*_*_*', 
        'keep L1GlobalTriggerReadoutRecord_gtDigis_*_*', 
        'keep *_TriggerResults_*_*', 
        'keep DcsStatuss_scalersRawToDigi_*_*', 
        'keep *_ALCARECOMuAlCalIsolatedMu_*_*', 
        'keep *_muonCSCDigis_*_*', 
        'keep *_muonDTDigis_*_*', 
        'keep *_muonRPCDigis_*_*', 
        'keep *_dt1DRecHits_*_*', 
        'keep *_dt2DSegments_*_*', 
        'keep *_dt4DSegments_*_*', 
        'keep *_csc2DRecHits_*_*', 
        'keep *_cscSegments_*_*', 
        'keep *_rpcRecHits_*_*', 
        'keep L1AcceptBunchCrossings_*_*_*', 
        'keep L1GlobalTriggerReadoutRecord_gtDigis_*_*', 
        'keep *_TriggerResults_*_*', 
        'keep DcsStatuss_scalersRawToDigi_*_*', 
        'keep *_ALCARECOMuAlZMuMu_*_*', 
        'keep *_muonCSCDigis_*_*', 
        'keep *_muonDTDigis_*_*', 
        'keep *_muonRPCDigis_*_*', 
        'keep *_dt1DRecHits_*_*', 
        'keep *_dt2DSegments_*_*', 
        'keep *_dt4DSegments_*_*', 
        'keep *_csc2DRecHits_*_*', 
        'keep *_cscSegments_*_*', 
        'keep *_rpcRecHits_*_*', 
        'keep L1AcceptBunchCrossings_*_*_*', 
        'keep L1GlobalTriggerReadoutRecord_gtDigis_*_*', 
        'keep *_TriggerResults_*_*', 
        'keep DcsStatuss_scalersRawToDigi_*_*', 
        'keep *_ALCARECOMuAlOverlaps_*_*', 
        'keep L1AcceptBunchCrossings_*_*_*', 
        'keep L1GlobalTriggerReadoutRecord_gtDigis_*_*', 
        'keep *_TriggerResults_*_*', 
        'keep DcsStatuss_scalersRawToDigi_*_*', 
        'keep *_ALCARECOMuAlBeamHaloOverlaps_*_*', 
        'keep *_muonCSCDigis_*_*', 
        'keep *_csc2DRecHits_*_*', 
        'keep *_cscSegments_*_*', 
        'keep L1AcceptBunchCrossings_*_*_*', 
        'keep L1GlobalTriggerReadoutRecord_gtDigis_*_*', 
        'keep *_TriggerResults_*_*', 
        'keep DcsStatuss_scalersRawToDigi_*_*', 
        'keep *_ALCARECOMuAlBeamHalo_*_*', 
        'keep *_muonCSCDigis_*_*', 
        'keep *_csc2DRecHits_*_*', 
        'keep *_cscSegments_*_*', 
        'keep L1AcceptBunchCrossings_*_*_*', 
        'keep L1GlobalTriggerReadoutRecord_gtDigis_*_*', 
        'keep *_TriggerResults_*_*', 
        'keep DcsStatuss_scalersRawToDigi_*_*', 
        'keep *_muonDTDigis_*_*', 
        'keep CSCDetIdCSCWireDigiMuonDigiCollection_*_*_*', 
        'keep CSCDetIdCSCStripDigiMuonDigiCollection_*_*_*', 
        'keep DTLayerIdDTDigiMuonDigiCollection_*_*_*', 
        'keep *_dt4DSegments_*_*', 
        'keep *_cscSegments_*_*', 
        'keep *_rpcRecHits_*_*', 
        'keep RPCDetIdRPCDigiMuonDigiCollection_*_*_*', 
        'keep recoMuons_muonsNoRPC_*_*', 
        'keep L1MuRegionalCands_*_RPCb_*', 
        'keep L1MuRegionalCands_*_RPCf_*', 
        'keep L1MuGMTCands_*_*_*', 
        'keep L1MuGMTReadoutCollection_*_*_*', 
        'keep *_dt4DSegments_*_*', 
        'keep *_dt4DSegmentsNoWire_*_*', 
        'keep *_muonDTDigis_*_*', 
        'keep *_dttfDigis_*_*', 
        'keep *_gtDigis_*_*', 
        'keep *_TriggerResults_*_*', 
        'keep recoMuons_muons_*_*', 
        'keep booledmValueMap_muid*_*_*', 
        'keep *_MEtoEDMConvertSiStrip_*_*', 
        'keep *_siPixelClustersForLumi_*_*', 
        'keep *_TriggerResults_*_HLT', 
        'drop *_MEtoEDMConverter_*_*' ) )
)

process.AODEventContent = cms.PSet(
    compressionLevel = cms.untracked.int32(4),
    compressionAlgorithm = cms.untracked.string('LZMA'),
    outputCommands = cms.untracked.vstring('drop *', 
        'keep *_clusterSummaryProducer_*_*', 
        'keep *_castorreco_*_*', 
        'keep *_reducedHcalRecHits_*_*', 
        'keep HcalUnpackerReport_*_*_*', 
        'keep *_selectDigi_*_*', 
        'keep EcalRecHitsSorted_reducedEcalRecHitsEB_*_*', 
        'keep EcalRecHitsSorted_reducedEcalRecHitsEE_*_*', 
        'keep EcalRecHitsSorted_reducedEcalRecHitsES_*_*', 
        'keep recoSuperClusters_correctedHybridSuperClusters_*_*', 
        'keep recoCaloClusters_hybridSuperClusters_*_*', 
        'keep recoSuperClusters_hybridSuperClusters_uncleanOnlyHybridSuperClusters_*', 
        'keep recoCaloClusters_multi5x5SuperClusters_multi5x5EndcapBasicClusters_*', 
        'keep recoSuperClusters_correctedMulti5x5SuperClustersWithPreshower_*_*', 
        'keep recoPreshowerClusters_multi5x5SuperClustersWithPreshower_*_*', 
        'keep recoPreshowerClusterShapes_multi5x5PreshowerClusterShape_*_*', 
        'keep recoSuperClusters_particleFlowSuperClusterECAL_*_*', 
        'keep recoCaloClusters_particleFlowSuperClusterECAL_*_*', 
        'keep recoTracks_GsfGlobalElectronTest_*_*', 
        'keep recoGsfTracks_electronGsfTracks_*_*', 
        'keep recoTracks_generalTracks_*_*', 
        'keep recoTracks_rsWithMaterialTracks_*_*', 
        'keep recoTracks_conversionStepTracks_*_*', 
        'keep recoTracks_beamhaloTracks_*_*', 
        'keep recoTracks_regionalCosmicTracks_*_*', 
        'keep recoTracks_ctfPixelLess_*_*', 
        'keep *_dedxHarmonic2_*_*', 
        'keep *_dedxDiscrimASmi_*_*', 
        'keep *_trackExtrapolator_*_*', 
        'keep floatedmValueMap_generalTracks_*_*', 
        'keep *_ak4CaloJets_*_*', 
        'keep *_ak5CaloJets_*_*', 
        'keep *_ak4PFJetsCHS_*_*', 
        'keep *_ak5PFJetsCHS_*_*', 
        'keep *_ak8PFJetsCHS_*_*', 
        'keep *_ca8PFJetsCHS_*_*', 
        'keep *_ca8PFJetsCHSPruned_*_*', 
        'keep *_cmsTopTagPFJetsCHS_*_*', 
        'keep double_kt6PFJets_rho_*', 
        'keep *_ak4PFJets_*_*', 
        'keep *_ak5PFJets_*_*', 
        'keep *_ak8PFJets_*_*', 
        'keep *_JetPlusTrackZSPCorJetAntiKt4_*_*', 
        'keep *_ak4TrackJets_*_*', 
        'keep recoRecoChargedRefCandidates_trackRefsForJets_*_*', 
        'keep *_caloTowers_*_*', 
        'keep *_CastorTowerReco_*_*', 
        'keep *_ak4JetTracksAssociatorAtVertex_*_*', 
        'keep *_ak4JetTracksAssociatorAtVertexPF_*_*', 
        'keep *_ak4JetTracksAssociatorExplicit_*_*', 
        'keep *_ak4JetExtender_*_*', 
        'keep *_ak4JetID_*_*', 
        'keep double_kt6CaloJetsCentral_*_*', 
        'keep double_kt6PFJetsCentralChargedPileUp_*_*', 
        'keep double_kt6PFJetsCentralNeutral_*_*', 
        'keep double_kt6PFJetsCentralNeutralTight_*_*', 
        'keep *_fixedGridRho*_*_*', 
        'drop doubles_*Jets_rhos_*', 
        'drop doubles_*Jets_sigmas_*', 
        'keep *_ca8PFJetsCHSPrunedLinks_*_*', 
        'keep recoCaloMETs_caloMet_*_*', 
        'keep recoCaloMETs_caloMetBE_*_*', 
        'keep recoCaloMETs_caloMetBEFO_*_*', 
        'keep recoCaloMETs_caloMetM_*_*', 
        'keep recoPFMETs_pfMet_*_*', 
        'keep recoPFMETs_pfChMet_*_*', 
        'keep recoMuonMETCorrectionDataedmValueMap_muonMETValueMapProducer_*_*', 
        'drop recoHcalNoiseRBXs_*_*_*', 
        'keep HcalNoiseSummary_hcalnoise_*_*', 
        'keep *GlobalHaloData_*_*_*', 
        'keep *BeamHaloSummary_BeamHaloSummary_*_*', 
        'keep *_muons_*_*', 
        'keep *_*_muons_*', 
        'drop *_muons_muons1stStep2muonsMap_*', 
        'drop recoIsoDepositedmValueMap_muons_*_*', 
        'drop doubleedmValueMap_muons_muPFIso*_*', 
        'keep recoTracks_standAloneMuons_*_*', 
        'keep recoTrackExtras_standAloneMuons_*_*', 
        'keep TrackingRecHitsOwned_standAloneMuons_*_*', 
        'keep recoTracks_globalMuons_*_*', 
        'keep recoTrackExtras_globalMuons_*_*', 
        'keep recoTracks_tevMuons_*_*', 
        'keep recoTrackExtras_tevMuons_*_*', 
        'keep recoTracks_generalTracks_*_*', 
        'keep recoTracksToOnerecoTracksAssociation_tevMuons_*_*', 
        'keep recoTracks_cosmicMuons_*_*', 
        'keep recoTracks_globalCosmicMuons_*_*', 
        'keep recoMuons_muonsFromCosmics_*_*', 
        'keep recoTracks_cosmicMuons1Leg_*_*', 
        'keep recoTracks_globalCosmicMuons1Leg_*_*', 
        'keep recoMuons_muonsFromCosmics1Leg_*_*', 
        'keep recoTracks_refittedStandAloneMuons_*_*', 
        'keep recoTrackExtras_refittedStandAloneMuons_*_*', 
        'keep TrackingRecHitsOwned_refittedStandAloneMuons_*_*', 
        'keep recoTracks_displacedStandAloneMuons__*', 
        'keep recoTrackExtras_displacedStandAloneMuons_*_*', 
        'keep TrackingRecHitsOwned_displacedStandAloneMuons_*_*', 
        'keep *_trackCountingHighEffBJetTags_*_*', 
        'keep *_trackCountingHighPurBJetTags_*_*', 
        'keep *_jetProbabilityBJetTags_*_*', 
        'keep *_jetBProbabilityBJetTags_*_*', 
        'keep *_simpleSecondaryVertexBJetTags_*_*', 
        'keep *_simpleSecondaryVertexHighEffBJetTags_*_*', 
        'keep *_simpleSecondaryVertexHighPurBJetTags_*_*', 
        'keep *_combinedSecondaryVertexBJetTags_*_*', 
        'keep *_combinedInclusiveSecondaryVertexV2BJetTags_*_*', 
        'keep *_ghostTrackBJetTags_*_*', 
        'keep *_softPFElectronBJetTags_*_*', 
        'keep *_softPFMuonBJetTags_*_*', 
        'keep *_softMuonBJetTags_*_*', 
        'keep *_softMuonByIP3dBJetTags_*_*', 
        'keep *_softMuonByPtBJetTags_*_*', 
        'keep *_combinedMVABJetTags_*_*', 
        'keep *_pfCombinedSecondaryVertexBJetTags_*_*', 
        'keep *_ak4PFJetsRecoTauPiZeros_*_*', 
        'keep *_hpsPFTauProducer_*_*', 
        'keep *_hpsPFTauDiscrimination*_*_*', 
        'keep *_hpsPFTau*PtSum_*_*', 
        'keep *_hpsPFTauTransverseImpactParameters_*_*', 
        'keep  *_offlinePrimaryVertices__*', 
        'keep  *_offlinePrimaryVerticesWithBS_*_*', 
        'keep  *_offlinePrimaryVerticesFromCosmicTracks_*_*', 
        'keep  *_nuclearInteractionMaker_*_*', 
        'keep *_generalV0Candidates_*_*', 
        'keep *_inclusiveSecondaryVertices_*_*', 
        'keep recoGsfElectronCores_gsfElectronCores_*_*', 
        'keep recoGsfElectrons_gsfElectrons_*_*', 
        'keep recoGsfElectronCores_uncleanedOnlyGsfElectronCores_*_*', 
        'keep recoGsfElectrons_uncleanedOnlyGsfElectrons_*_*', 
        'keep floatedmValueMap_eidRobustLoose_*_*', 
        'keep floatedmValueMap_eidRobustTight_*_*', 
        'keep floatedmValueMap_eidRobustHighEnergy_*_*', 
        'keep floatedmValueMap_eidLoose_*_*', 
        'keep floatedmValueMap_eidTight_*_*', 
        'keep *_egmGedGsfElectronPFIsolation_*_*', 
        'keep *_egmGsfElectronIDs_*_*', 
        'keep recoPhotonCores_gedPhotonCore_*_*', 
        'keep recoPhotons_gedPhotons_*_*', 
        'keep *_particleBasedIsolation_*_*', 
        'drop *_gedPhotons_valMapPFEgammaCandToPhoton_*', 
        'keep recoPhotonCores_photonCore_*_*', 
        'keep recoPhotons_photons_*_*', 
        'keep recoConversions_conversions_*_*', 
        'keep recoConversions_mustacheConversions_*_*', 
        'drop *_conversions_uncleanedConversions_*', 
        'keep recoConversions_allConversions_*_*', 
        'keep recoConversions_allConversionsOldEG_*_*', 
        'keep recoTracks_ckfOutInTracksFrom*Conversions_*_*', 
        'keep recoTracks_ckfInOutTracksFrom*Conversions_*_*', 
        'keep recoConversions_uncleanedOnlyAllConversions_*_*', 
        'keep recoTracks_uncleanedOnlyCkfOutInTracksFromConversions_*_*', 
        'keep recoTracks_uncleanedOnlyCkfInOutTracksFromConversions_*_*', 
        'keep *_PhotonIDProd_*_*', 
        'keep *_PhotonIDProdGED_*_*', 
        'keep *_hfRecoEcalCandidate_*_*', 
        'keep *_hfEMClusters_*_*', 
        'keep *_gedGsfElectronCores_*_*', 
        'keep *_gedGsfElectrons_*_*', 
        'drop CaloTowersSorted_towerMakerPF_*_*', 
        'drop *_pfElectronTranslator_*_*', 
        'keep recoPFRecHits_particleFlowClusterECAL_Cleaned_*', 
        'keep recoPFRecHits_particleFlowClusterHCAL_Cleaned_*', 
        'keep recoPFRecHits_particleFlowClusterHO_Cleaned_*', 
        'keep recoPFRecHits_particleFlowClusterHFEM_Cleaned_*', 
        'keep recoPFRecHits_particleFlowClusterHFHAD_Cleaned_*', 
        'keep recoPFRecHits_particleFlowClusterPS_Cleaned_*', 
        'keep recoPFRecHits_particleFlowRecHitECAL_Cleaned_*', 
        'keep recoPFRecHits_particleFlowRecHitHCAL_Cleaned_*', 
        'keep recoPFRecHits_particleFlowRecHitHO_Cleaned_*', 
        'keep recoPFRecHits_particleFlowRecHitPS_Cleaned_*', 
        'keep recoCaloClusters_particleFlowEGamma_*_*', 
        'keep recoSuperClusters_particleFlowEGamma_*_*', 
        'keep recoConversions_particleFlowEGamma_*_*', 
        'keep recoPFCandidates_particleFlow_*_*', 
        'keep recoPFCandidates_particleFlowTmp_*_*', 
        'drop recoPFCandidates_particleFlowTmp__*', 
        'keep *_particleFlow_electrons_*', 
        'keep *_particleFlow_photons_*', 
        'keep recoCaloClusters_pfElectronTranslator_*_*', 
        'keep recoPreshowerClusters_pfElectronTranslator_*_*', 
        'keep recoSuperClusters_pfElectronTranslator_*_*', 
        'keep recoCaloClusters_pfPhotonTranslator_*_*', 
        'keep recoPreshowerClusters_pfPhotonTranslator_*_*', 
        'keep recoSuperClusters_pfPhotonTranslator_*_*', 
        'keep recoPhotons_pfPhotonTranslator_*_*', 
        'keep recoPhotonCores_pfPhotonTranslator_*_*', 
        'keep recoConversions_pfPhotonTranslator_*_*', 
        'keep *_particleFlowPtrs_*_*', 
        'keep *_particleFlowTmpPtrs_*_*', 
        'keep *_offlineBeamSpot_*_*', 
        'keep L1GlobalTriggerReadoutRecord_gtDigis_*_*', 
        'keep *_l1GtRecord_*_*', 
        'keep *_l1GtTriggerMenuLite_*_*', 
        'keep *_conditionsInEdm_*_*', 
        'keep *_l1extraParticles_*_*', 
        'keep *_l1L1GtObjectMap_*_*', 
        'keep LumiSummary_lumiProducer_*_*', 
        'drop *_hlt*_*_*', 
        'keep *_hltL1GtObjectMap_*_*', 
        'drop L1GlobalTriggerObjectMapRecord_hltL1GtObjectMap_*_*', 
        'keep edmTriggerResults_*_*_*', 
        'keep triggerTriggerEvent_*_*_*', 
        'keep L1AcceptBunchCrossings_*_*_*', 
        'keep L1TriggerScalerss_*_*_*', 
        'keep Level1TriggerScalerss_*_*_*', 
        'keep LumiScalerss_*_*_*', 
        'keep BeamSpotOnlines_*_*_*', 
        'keep DcsStatuss_*_*_*', 
        'keep *_logErrorHarvester_*_*', 
        'keep PileupSummaryInfos_*_*_*', 
        'keep *_pfIsolatedElectronsEI_*_*', 
        'keep *_pfIsolatedMuonsEI_*_*', 
        'keep recoPFJets_pfJetsEI_*_*', 
        'keep *_pfJetTrackAssociatorEI_*_*', 
        'keep *_impactParameterTagInfosEI_*_*', 
        'keep *_secondaryVertexTagInfosEI_*_*', 
        'keep *_combinedSecondaryVertexBJetTagsEI_*_*', 
        'keep recoPFTaus_pfTausEI_*_*', 
        'keep recoPFTauDiscriminator_pfTausDiscrimination*_*_*', 
        'keep *_pfMetEI_*_*'),
    eventAutoFlushCompressedSize = cms.untracked.int32(15728640)
)

process.AODSIMEventContent = cms.PSet(
    compressionLevel = cms.untracked.int32(4),
    compressionAlgorithm = cms.untracked.string('LZMA'),
    outputCommands = cms.untracked.vstring('drop *', 
        'drop *', 
        'keep *_clusterSummaryProducer_*_*', 
        'keep *_castorreco_*_*', 
        'keep *_reducedHcalRecHits_*_*', 
        'keep HcalUnpackerReport_*_*_*', 
        'keep *_selectDigi_*_*', 
        'keep EcalRecHitsSorted_reducedEcalRecHitsEB_*_*', 
        'keep EcalRecHitsSorted_reducedEcalRecHitsEE_*_*', 
        'keep EcalRecHitsSorted_reducedEcalRecHitsES_*_*', 
        'keep recoSuperClusters_correctedHybridSuperClusters_*_*', 
        'keep recoCaloClusters_hybridSuperClusters_*_*', 
        'keep recoSuperClusters_hybridSuperClusters_uncleanOnlyHybridSuperClusters_*', 
        'keep recoCaloClusters_multi5x5SuperClusters_multi5x5EndcapBasicClusters_*', 
        'keep recoSuperClusters_correctedMulti5x5SuperClustersWithPreshower_*_*', 
        'keep recoPreshowerClusters_multi5x5SuperClustersWithPreshower_*_*', 
        'keep recoPreshowerClusterShapes_multi5x5PreshowerClusterShape_*_*', 
        'keep recoSuperClusters_particleFlowSuperClusterECAL_*_*', 
        'keep recoCaloClusters_particleFlowSuperClusterECAL_*_*', 
        'keep recoTracks_GsfGlobalElectronTest_*_*', 
        'keep recoGsfTracks_electronGsfTracks_*_*', 
        'keep recoTracks_generalTracks_*_*', 
        'keep recoTracks_rsWithMaterialTracks_*_*', 
        'keep recoTracks_conversionStepTracks_*_*', 
        'keep recoTracks_beamhaloTracks_*_*', 
        'keep recoTracks_regionalCosmicTracks_*_*', 
        'keep recoTracks_ctfPixelLess_*_*', 
        'keep *_dedxHarmonic2_*_*', 
        'keep *_dedxDiscrimASmi_*_*', 
        'keep *_trackExtrapolator_*_*', 
        'keep floatedmValueMap_generalTracks_*_*', 
        'keep *_ak4CaloJets_*_*', 
        'keep *_ak5CaloJets_*_*', 
        'keep *_ak4PFJetsCHS_*_*', 
        'keep *_ak5PFJetsCHS_*_*', 
        'keep *_ak8PFJetsCHS_*_*', 
        'keep *_ca8PFJetsCHS_*_*', 
        'keep *_ca8PFJetsCHSPruned_*_*', 
        'keep *_cmsTopTagPFJetsCHS_*_*', 
        'keep double_kt6PFJets_rho_*', 
        'keep *_ak4PFJets_*_*', 
        'keep *_ak5PFJets_*_*', 
        'keep *_ak8PFJets_*_*', 
        'keep *_JetPlusTrackZSPCorJetAntiKt4_*_*', 
        'keep *_ak4TrackJets_*_*', 
        'keep recoRecoChargedRefCandidates_trackRefsForJets_*_*', 
        'keep *_caloTowers_*_*', 
        'keep *_CastorTowerReco_*_*', 
        'keep *_ak4JetTracksAssociatorAtVertex_*_*', 
        'keep *_ak4JetTracksAssociatorAtVertexPF_*_*', 
        'keep *_ak4JetTracksAssociatorExplicit_*_*', 
        'keep *_ak4JetExtender_*_*', 
        'keep *_ak4JetID_*_*', 
        'keep double_kt6CaloJetsCentral_*_*', 
        'keep double_kt6PFJetsCentralChargedPileUp_*_*', 
        'keep double_kt6PFJetsCentralNeutral_*_*', 
        'keep double_kt6PFJetsCentralNeutralTight_*_*', 
        'keep *_fixedGridRho*_*_*', 
        'drop doubles_*Jets_rhos_*', 
        'drop doubles_*Jets_sigmas_*', 
        'keep *_ca8PFJetsCHSPrunedLinks_*_*', 
        'keep recoCaloMETs_caloMet_*_*', 
        'keep recoCaloMETs_caloMetBE_*_*', 
        'keep recoCaloMETs_caloMetBEFO_*_*', 
        'keep recoCaloMETs_caloMetM_*_*', 
        'keep recoPFMETs_pfMet_*_*', 
        'keep recoPFMETs_pfChMet_*_*', 
        'keep recoMuonMETCorrectionDataedmValueMap_muonMETValueMapProducer_*_*', 
        'drop recoHcalNoiseRBXs_*_*_*', 
        'keep HcalNoiseSummary_hcalnoise_*_*', 
        'keep *GlobalHaloData_*_*_*', 
        'keep *BeamHaloSummary_BeamHaloSummary_*_*', 
        'keep *_muons_*_*', 
        'keep *_*_muons_*', 
        'drop *_muons_muons1stStep2muonsMap_*', 
        'drop recoIsoDepositedmValueMap_muons_*_*', 
        'drop doubleedmValueMap_muons_muPFIso*_*', 
        'keep recoTracks_standAloneMuons_*_*', 
        'keep recoTrackExtras_standAloneMuons_*_*', 
        'keep TrackingRecHitsOwned_standAloneMuons_*_*', 
        'keep recoTracks_globalMuons_*_*', 
        'keep recoTrackExtras_globalMuons_*_*', 
        'keep recoTracks_tevMuons_*_*', 
        'keep recoTrackExtras_tevMuons_*_*', 
        'keep recoTracks_generalTracks_*_*', 
        'keep recoTracksToOnerecoTracksAssociation_tevMuons_*_*', 
        'keep recoTracks_cosmicMuons_*_*', 
        'keep recoTracks_globalCosmicMuons_*_*', 
        'keep recoMuons_muonsFromCosmics_*_*', 
        'keep recoTracks_cosmicMuons1Leg_*_*', 
        'keep recoTracks_globalCosmicMuons1Leg_*_*', 
        'keep recoMuons_muonsFromCosmics1Leg_*_*', 
        'keep recoTracks_refittedStandAloneMuons_*_*', 
        'keep recoTrackExtras_refittedStandAloneMuons_*_*', 
        'keep TrackingRecHitsOwned_refittedStandAloneMuons_*_*', 
        'keep recoTracks_displacedStandAloneMuons__*', 
        'keep recoTrackExtras_displacedStandAloneMuons_*_*', 
        'keep TrackingRecHitsOwned_displacedStandAloneMuons_*_*', 
        'keep *_trackCountingHighEffBJetTags_*_*', 
        'keep *_trackCountingHighPurBJetTags_*_*', 
        'keep *_jetProbabilityBJetTags_*_*', 
        'keep *_jetBProbabilityBJetTags_*_*', 
        'keep *_simpleSecondaryVertexBJetTags_*_*', 
        'keep *_simpleSecondaryVertexHighEffBJetTags_*_*', 
        'keep *_simpleSecondaryVertexHighPurBJetTags_*_*', 
        'keep *_combinedSecondaryVertexBJetTags_*_*', 
        'keep *_combinedInclusiveSecondaryVertexV2BJetTags_*_*', 
        'keep *_ghostTrackBJetTags_*_*', 
        'keep *_softPFElectronBJetTags_*_*', 
        'keep *_softPFMuonBJetTags_*_*', 
        'keep *_softMuonBJetTags_*_*', 
        'keep *_softMuonByIP3dBJetTags_*_*', 
        'keep *_softMuonByPtBJetTags_*_*', 
        'keep *_combinedMVABJetTags_*_*', 
        'keep *_pfCombinedSecondaryVertexBJetTags_*_*', 
        'keep *_ak4PFJetsRecoTauPiZeros_*_*', 
        'keep *_hpsPFTauProducer_*_*', 
        'keep *_hpsPFTauDiscrimination*_*_*', 
        'keep *_hpsPFTau*PtSum_*_*', 
        'keep *_hpsPFTauTransverseImpactParameters_*_*', 
        'keep  *_offlinePrimaryVertices__*', 
        'keep  *_offlinePrimaryVerticesWithBS_*_*', 
        'keep  *_offlinePrimaryVerticesFromCosmicTracks_*_*', 
        'keep  *_nuclearInteractionMaker_*_*', 
        'keep *_generalV0Candidates_*_*', 
        'keep *_inclusiveSecondaryVertices_*_*', 
        'keep recoGsfElectronCores_gsfElectronCores_*_*', 
        'keep recoGsfElectrons_gsfElectrons_*_*', 
        'keep recoGsfElectronCores_uncleanedOnlyGsfElectronCores_*_*', 
        'keep recoGsfElectrons_uncleanedOnlyGsfElectrons_*_*', 
        'keep floatedmValueMap_eidRobustLoose_*_*', 
        'keep floatedmValueMap_eidRobustTight_*_*', 
        'keep floatedmValueMap_eidRobustHighEnergy_*_*', 
        'keep floatedmValueMap_eidLoose_*_*', 
        'keep floatedmValueMap_eidTight_*_*', 
        'keep *_egmGedGsfElectronPFIsolation_*_*', 
        'keep *_egmGsfElectronIDs_*_*', 
        'keep recoPhotonCores_gedPhotonCore_*_*', 
        'keep recoPhotons_gedPhotons_*_*', 
        'keep *_particleBasedIsolation_*_*', 
        'drop *_gedPhotons_valMapPFEgammaCandToPhoton_*', 
        'keep recoPhotonCores_photonCore_*_*', 
        'keep recoPhotons_photons_*_*', 
        'keep recoConversions_conversions_*_*', 
        'keep recoConversions_mustacheConversions_*_*', 
        'drop *_conversions_uncleanedConversions_*', 
        'keep recoConversions_allConversions_*_*', 
        'keep recoConversions_allConversionsOldEG_*_*', 
        'keep recoTracks_ckfOutInTracksFrom*Conversions_*_*', 
        'keep recoTracks_ckfInOutTracksFrom*Conversions_*_*', 
        'keep recoConversions_uncleanedOnlyAllConversions_*_*', 
        'keep recoTracks_uncleanedOnlyCkfOutInTracksFromConversions_*_*', 
        'keep recoTracks_uncleanedOnlyCkfInOutTracksFromConversions_*_*', 
        'keep *_PhotonIDProd_*_*', 
        'keep *_PhotonIDProdGED_*_*', 
        'keep *_hfRecoEcalCandidate_*_*', 
        'keep *_hfEMClusters_*_*', 
        'keep *_gedGsfElectronCores_*_*', 
        'keep *_gedGsfElectrons_*_*', 
        'drop CaloTowersSorted_towerMakerPF_*_*', 
        'drop *_pfElectronTranslator_*_*', 
        'keep recoPFRecHits_particleFlowClusterECAL_Cleaned_*', 
        'keep recoPFRecHits_particleFlowClusterHCAL_Cleaned_*', 
        'keep recoPFRecHits_particleFlowClusterHO_Cleaned_*', 
        'keep recoPFRecHits_particleFlowClusterHFEM_Cleaned_*', 
        'keep recoPFRecHits_particleFlowClusterHFHAD_Cleaned_*', 
        'keep recoPFRecHits_particleFlowClusterPS_Cleaned_*', 
        'keep recoPFRecHits_particleFlowRecHitECAL_Cleaned_*', 
        'keep recoPFRecHits_particleFlowRecHitHCAL_Cleaned_*', 
        'keep recoPFRecHits_particleFlowRecHitHO_Cleaned_*', 
        'keep recoPFRecHits_particleFlowRecHitPS_Cleaned_*', 
        'keep recoCaloClusters_particleFlowEGamma_*_*', 
        'keep recoSuperClusters_particleFlowEGamma_*_*', 
        'keep recoConversions_particleFlowEGamma_*_*', 
        'keep recoPFCandidates_particleFlow_*_*', 
        'keep recoPFCandidates_particleFlowTmp_*_*', 
        'drop recoPFCandidates_particleFlowTmp__*', 
        'keep *_particleFlow_electrons_*', 
        'keep *_particleFlow_photons_*', 
        'keep recoCaloClusters_pfElectronTranslator_*_*', 
        'keep recoPreshowerClusters_pfElectronTranslator_*_*', 
        'keep recoSuperClusters_pfElectronTranslator_*_*', 
        'keep recoCaloClusters_pfPhotonTranslator_*_*', 
        'keep recoPreshowerClusters_pfPhotonTranslator_*_*', 
        'keep recoSuperClusters_pfPhotonTranslator_*_*', 
        'keep recoPhotons_pfPhotonTranslator_*_*', 
        'keep recoPhotonCores_pfPhotonTranslator_*_*', 
        'keep recoConversions_pfPhotonTranslator_*_*', 
        'keep *_particleFlowPtrs_*_*', 
        'keep *_particleFlowTmpPtrs_*_*', 
        'keep *_offlineBeamSpot_*_*', 
        'keep L1GlobalTriggerReadoutRecord_gtDigis_*_*', 
        'keep *_l1GtRecord_*_*', 
        'keep *_l1GtTriggerMenuLite_*_*', 
        'keep *_conditionsInEdm_*_*', 
        'keep *_l1extraParticles_*_*', 
        'keep *_l1L1GtObjectMap_*_*', 
        'keep LumiSummary_lumiProducer_*_*', 
        'drop *_hlt*_*_*', 
        'keep *_hltL1GtObjectMap_*_*', 
        'drop L1GlobalTriggerObjectMapRecord_hltL1GtObjectMap_*_*', 
        'keep edmTriggerResults_*_*_*', 
        'keep triggerTriggerEvent_*_*_*', 
        'keep L1AcceptBunchCrossings_*_*_*', 
        'keep L1TriggerScalerss_*_*_*', 
        'keep Level1TriggerScalerss_*_*_*', 
        'keep LumiScalerss_*_*_*', 
        'keep BeamSpotOnlines_*_*_*', 
        'keep DcsStatuss_*_*_*', 
        'keep *_logErrorHarvester_*_*', 
        'keep PileupSummaryInfos_*_*_*', 
        'keep *_pfIsolatedElectronsEI_*_*', 
        'keep *_pfIsolatedMuonsEI_*_*', 
        'keep recoPFJets_pfJetsEI_*_*', 
        'keep *_pfJetTrackAssociatorEI_*_*', 
        'keep *_impactParameterTagInfosEI_*_*', 
        'keep *_secondaryVertexTagInfosEI_*_*', 
        'keep *_combinedSecondaryVertexBJetTagsEI_*_*', 
        'keep recoPFTaus_pfTausEI_*_*', 
        'keep recoPFTauDiscriminator_pfTausDiscrimination*_*_*', 
        'keep *_pfMetEI_*_*', 
        'keep LHERunInfoProduct_*_*_*', 
        'keep LHEEventProduct_*_*_*', 
        'keep GenRunInfoProduct_generator_*_*', 
        'keep GenLumiInfoProduct_generator_*_*', 
        'keep GenEventInfoProduct_generator_*_*', 
        'keep GenFilterInfo_*_*_*', 
        'keep *_genParticles_*_*', 
        'keep *_allTrackMCMatch_*_*', 
        'keep *_kt4GenJets_*_*', 
        'keep *_kt6GenJets_*_*', 
        'keep *_ak4GenJets_*_*', 
        'keep *_ak5GenJets_*_*', 
        'keep *_ak8GenJets_*_*', 
        'keep *_genParticle_*_*', 
        'keep recoGenMETs_*_*_*', 
        'keep PileupSummaryInfos_*_*_*'),
    eventAutoFlushCompressedSize = cms.untracked.int32(15728640)
)

process.BeamSpotAOD = cms.PSet(
    outputCommands = cms.untracked.vstring('keep *_offlineBeamSpot_*_*')
)

process.BeamSpotFEVT = cms.PSet(
    outputCommands = cms.untracked.vstring('keep *_offlineBeamSpot_*_*')
)

process.BeamSpotRECO = cms.PSet(
    outputCommands = cms.untracked.vstring('keep *_offlineBeamSpot_*_*')
)

process.CommonEventContent = cms.PSet(
    outputCommands = cms.untracked.vstring('keep *_logErrorHarvester_*_*')
)

process.CondDBSetup = cms.PSet(
    DBParameters = cms.PSet(
        authenticationPath = cms.untracked.string(''),
        enableReadOnlySessionOnUpdateConnection = cms.untracked.bool(False),
        idleConnectionCleanupPeriod = cms.untracked.int32(10),
        messageLevel = cms.untracked.int32(0),
        enablePoolAutomaticCleanUp = cms.untracked.bool(False),
        enableConnectionSharing = cms.untracked.bool(True),
        connectionRetrialTimeOut = cms.untracked.int32(60),
        connectionTimeOut = cms.untracked.int32(60),
        authenticationSystem = cms.untracked.int32(0),
        connectionRetrialPeriod = cms.untracked.int32(10)
    )
)

process.DATAMIXEREventContent = cms.PSet(
    splitLevel = cms.untracked.int32(0),
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
    outputCommands = cms.untracked.vstring('drop *', 
        'keep CSCDetIdCSCALCTDigiMuonDigiCollection_muonCSCDigis_MuonCSCALCTDigi_*', 
        'keep CSCDetIdCSCCLCTDigiMuonDigiCollection_muonCSCDigis_MuonCSCCLCTDigi_*', 
        'keep CSCDetIdCSCComparatorDigiMuonDigiCollection_muonCSCDigis_MuonCSCComparatorDigi_*', 
        'keep CSCDetIdCSCCorrelatedLCTDigiMuonDigiCollection_csctfDigis_*_*', 
        'keep CSCDetIdCSCCorrelatedLCTDigiMuonDigiCollection_muonCSCDigis_MuonCSCCorrelatedLCTDigi_*', 
        'keep CSCDetIdCSCRPCDigiMuonDigiCollection_muonCSCDigis_MuonCSCRPCDigi_*', 
        'keep CSCDetIdCSCStripDigiMuonDigiCollection_muonCSCDigis_MuonCSCStripDigi_*', 
        'keep CSCDetIdCSCWireDigiMuonDigiCollection_muonCSCDigis_MuonCSCWireDigi_*', 
        'keep DTLayerIdDTDigiMuonDigiCollection_muonDTDigis_*_*', 
        'keep PixelDigiedmDetSetVector_siPixelDigis_*_*', 
        'keep SiStripDigiedmDetSetVector_siStripDigis_*_*', 
        'keep RPCDetIdRPCDigiMuonDigiCollection_muonRPCDigis_*_*', 
        'keep HBHEDataFramesSorted_hcalDigis_*_*', 
        'keep HFDataFramesSorted_hcalDigis_*_*', 
        'keep HODataFramesSorted_hcalDigis_*_*', 
        'keep ZDCDataFramesSorted_hcalDigis_*_*', 
        'keep CastorDataFramesSorted_castorDigis_*_*', 
        'keep EBDigiCollection_ecalDigis_*_*', 
        'keep EEDigiCollection_ecalDigis_*_*', 
        'keep ESDigiCollection_ecalPreshowerDigis_*_*')
)

process.DQMEventContent = cms.PSet(
    splitLevel = cms.untracked.int32(0),
    outputCommands = cms.untracked.vstring('drop *', 
        'keep *_MEtoEDMConverter_*_*')
)

process.DigiToRawFEVT = cms.PSet(
    outputCommands = cms.untracked.vstring('keep FEDRawDataCollection_source_*_*', 
        'keep FEDRawDataCollection_rawDataCollector_*_*')
)

process.EITopPAGEventContent = cms.PSet(
    outputCommands = cms.untracked.vstring('keep *_pfIsolatedElectronsEI_*_*', 
        'keep *_pfIsolatedMuonsEI_*_*', 
        'keep recoPFJets_pfJetsEI_*_*', 
        'keep *_pfJetTrackAssociatorEI_*_*', 
        'keep *_impactParameterTagInfosEI_*_*', 
        'keep *_secondaryVertexTagInfosEI_*_*', 
        'keep *_combinedSecondaryVertexBJetTagsEI_*_*', 
        'keep recoPFTaus_pfTausEI_*_*', 
        'keep recoPFTauDiscriminator_pfTausDiscrimination*_*_*', 
        'keep *_pfMetEI_*_*')
)

process.EvtScalersAOD = cms.PSet(
    outputCommands = cms.untracked.vstring('keep L1AcceptBunchCrossings_*_*_*', 
        'keep L1TriggerScalerss_*_*_*', 
        'keep Level1TriggerScalerss_*_*_*', 
        'keep LumiScalerss_*_*_*', 
        'keep BeamSpotOnlines_*_*_*', 
        'keep DcsStatuss_*_*_*')
)

process.EvtScalersRECO = cms.PSet(
    outputCommands = cms.untracked.vstring('keep L1AcceptBunchCrossings_*_*_*', 
        'keep L1TriggerScalerss_*_*_*', 
        'keep Level1TriggerScalerss_*_*_*', 
        'keep LumiScalerss_*_*_*', 
        'keep BeamSpotOnlines_*_*_*', 
        'keep DcsStatuss_*_*_*')
)

process.FEVTDEBUGEventContent = cms.PSet(
    splitLevel = cms.untracked.int32(0),
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
    outputCommands = cms.untracked.vstring( ('drop *', 
        'drop *', 
        'drop *', 
        'keep  FEDRawDataCollection_rawDataCollector_*_*', 
        'keep  FEDRawDataCollection_source_*_*', 
        'keep  FEDRawDataCollection_rawDataCollector_*_*', 
        'keep  FEDRawDataCollection_source_*_*', 
        'drop *_hlt*_*_*', 
        'keep *_hltL1GtObjectMap_*_*', 
        'keep FEDRawDataCollection_rawDataCollector_*_*', 
        'keep FEDRawDataCollection_source_*_*', 
        'keep edmTriggerResults_*_*_*', 
        'keep triggerTriggerEvent_*_*_*', 
        'keep *_g4SimHits_*_*', 
        'keep edmHepMCProduct_source_*_*', 
        'keep *_allTrackMCMatch_*_*', 
        'keep StripDigiSimLinkedmDetSetVector_simMuonCSCDigis_*_*', 
        'keep CSCDetIdCSCComparatorDigiMuonDigiCollection_simMuonCSCDigis_*_*', 
        'keep DTLayerIdDTDigiSimLinkMuonDigiCollection_simMuonDTDigis_*_*', 
        'keep RPCDigiSimLinkedmDetSetVector_simMuonRPCDigis_*_*', 
        'keep EBSrFlagsSorted_simEcalDigis_*_*', 
        'keep EESrFlagsSorted_simEcalDigis_*_*', 
        'keep CrossingFramePlaybackInfoExtended_*_*_*', 
        'keep PileupSummaryInfos_*_*_*', 
        'keep LHERunInfoProduct_*_*_*', 
        'keep LHEEventProduct_*_*_*', 
        'keep GenRunInfoProduct_generator_*_*', 
        'keep GenLumiInfoProduct_generator_*_*', 
        'keep GenEventInfoProduct_generator_*_*', 
        'keep edmHepMCProduct_generator_*_*', 
        'keep GenFilterInfo_*_*_*', 
        'keep *_genParticles_*_*', 
        'keep recoGenJets_*_*_*', 
        'keep *_genParticle_*_*', 
        'keep recoGenMETs_*_*_*', 
        'keep FEDRawDataCollection_source_*_*', 
        'keep FEDRawDataCollection_rawDataCollector_*_*', 
        'keep *_MEtoEDMConverter_*_*', 
        'keep *_randomEngineStateProducer_*_*', 
        'keep DetIdedmEDCollection_siStripDigis_*_*', 
        'keep DetIdedmEDCollection_siPixelDigis_*_*', 
        'keep *_siPixelClusters_*_*', 
        'keep *_siStripClusters_*_*', 
        'keep *_clusterSummaryProducer_*_*', 
        'keep *_dt1DRecHits_*_*', 
        'keep *_dt4DSegments_*_*', 
        'keep *_dt1DCosmicRecHits_*_*', 
        'keep *_dt4DCosmicSegments_*_*', 
        'keep *_csc2DRecHits_*_*', 
        'keep *_cscSegments_*_*', 
        'keep *_rpcRecHits_*_*', 
        'keep *_hbhereco_*_*', 
        'keep *_hbheprereco_*_*', 
        'keep *_hfreco_*_*', 
        'keep *_horeco_*_*', 
        'keep HBHERecHitsSorted_hbherecoMB_*_*', 
        'keep HORecHitsSorted_horecoMB_*_*', 
        'keep HFRecHitsSorted_hfrecoMB_*_*', 
        'keep ZDCDataFramesSorted_*Digis_*_*', 
        'keep ZDCRecHitsSorted_*_*_*', 
        'keep *_reducedHcalRecHits_*_*', 
        'keep *_castorreco_*_*', 
        'keep HcalUnpackerReport_*_*_*', 
        'keep *_ecalPreshowerRecHit_*_*', 
        'keep *_ecalRecHit_*_*', 
        'keep *_ecalCompactTrigPrim_*_*', 
        'keep *_ecalTPSkim_*_*', 
        'keep *_selectDigi_*_*', 
        'keep EcalRecHitsSorted_reducedEcalRecHitsEE_*_*', 
        'keep EcalRecHitsSorted_reducedEcalRecHitsEB_*_*', 
        'keep EcalRecHitsSorted_reducedEcalRecHitsES_*_*', 
        'keep *_hybridSuperClusters_*_*', 
        'keep recoSuperClusters_correctedHybridSuperClusters_*_*', 
        'keep *_multi5x5SuperClusters_*_*', 
        'keep recoSuperClusters_multi5x5SuperClusters_*_*', 
        'keep recoSuperClusters_multi5x5SuperClustersWithPreshower_*_*', 
        'keep recoSuperClusters_correctedMulti5x5SuperClustersWithPreshower_*_*', 
        'keep recoPreshowerClusters_multi5x5SuperClustersWithPreshower_*_*', 
        'keep recoPreshowerClusterShapes_multi5x5PreshowerClusterShape_*_*', 
        'keep *_particleFlowSuperClusterECAL_*_*', 
        'drop recoClusterShapes_*_*_*', 
        'drop recoBasicClustersToOnerecoClusterShapesAssociation_*_*_*', 
        'drop recoBasicClusters_multi5x5BasicClusters_multi5x5BarrelBasicClusters_*', 
        'drop recoSuperClusters_multi5x5SuperClusters_multi5x5BarrelSuperClusters_*', 
        'keep *_CkfElectronCandidates_*_*', 
        'keep *_GsfGlobalElectronTest_*_*', 
        'keep *_electronMergedSeeds_*_*', 
        'keep recoGsfTracks_electronGsfTracks_*_*', 
        'keep recoGsfTrackExtras_electronGsfTracks_*_*', 
        'keep recoTrackExtras_electronGsfTracks_*_*', 
        'keep TrackingRecHitsOwned_electronGsfTracks_*_*', 
        'keep recoTracks_generalTracks_*_*', 
        'keep recoTrackExtras_generalTracks_*_*', 
        'keep TrackingRecHitsOwned_extraFromSeeds_*_*', 
        'keep uints_extraFromSeeds_*_*', 
        'keep TrackingRecHitsOwned_generalTracks_*_*', 
        'keep recoTracks_beamhaloTracks_*_*', 
        'keep recoTrackExtras_beamhaloTracks_*_*', 
        'keep TrackingRecHitsOwned_beamhaloTracks_*_*', 
        'keep recoTracks_regionalCosmicTracks_*_*', 
        'keep recoTrackExtras_regionalCosmicTracks_*_*', 
        'keep TrackingRecHitsOwned_regionalCosmicTracks_*_*', 
        'keep recoTracks_rsWithMaterialTracks_*_*', 
        'keep recoTrackExtras_rsWithMaterialTracks_*_*', 
        'keep TrackingRecHitsOwned_rsWithMaterialTracks_*_*', 
        'keep recoTracks_conversionStepTracks_*_*', 
        'keep recoTrackExtras_conversionStepTracks_*_*', 
        'keep TrackingRecHitsOwned_conversionStepTracks_*_*', 
        'keep *_ctfPixelLess_*_*', 
        'keep *_dedxTruncated40_*_*', 
        'keep *_dedxDiscrimASmi_*_*', 
        'keep *_dedxHarmonic2_*_*', 
        'keep *_trackExtrapolator_*_*', 
        'keep floatedmValueMap_generalTracks_*_*', 
        'keep *_kt6CaloJets_*_*', 
        'keep *_ak4CaloJets_*_*', 
        'keep *_ak5CaloJets_*_*', 
        'keep double_kt6PFJets_rho_*', 
        'keep *_ak4PFJets_*_*', 
        'keep *_ak5PFJets_*_*', 
        'keep *_ak8PFJets_*_*', 
        'keep *_ak4PFJetsCHS_*_*', 
        'keep *_ak5PFJetsCHS_*_*', 
        'keep *_ak8PFJetsCHS_*_*', 
        'keep *_ca8PFJetsCHS_*_*', 
        'keep *_ca8PFJetsCHSPruned_*_*', 
        'keep *_cmsTopTagPFJetsCHS_*_*', 
        'keep *_JetPlusTrackZSPCorJetAntiKt4_*_*', 
        'keep *_ak4TrackJets_*_*', 
        'keep recoRecoChargedRefCandidates_trackRefsForJets_*_*', 
        'keep *_caloTowers_*_*', 
        'keep *_towerMaker_*_*', 
        'keep *_CastorTowerReco_*_*', 
        'keep *_kt4JetTracksAssociatorAtVertex_*_*', 
        'keep *_kt4JetTracksAssociatorAtCaloFace_*_*', 
        'keep *_kt4JetExtender_*_*', 
        'keep *_ak4JetTracksAssociatorAtVertex_*_*', 
        'keep *_ak4JetTracksAssociatorAtVertexPF_*_*', 
        'keep *_ak4JetTracksAssociatorAtCaloFace_*_*', 
        'keep *_ak4JetTracksAssociatorExplicit_*_*', 
        'keep *_ak4JetExtender_*_*', 
        'keep *_ak7JetTracksAssociatorAtVertex*_*_*', 
        'keep *_ak7JetTracksAssociatorAtCaloFace*_*_*', 
        'keep *_ak7JetExtender_*_*', 
        'keep *_ak4JetID_*_*', 
        'keep *_ak5JetID_*_*', 
        'keep *_ak7JetID_*_*', 
        'keep *_ic5JetID_*_*', 
        'keep *_kt4JetID_*_*', 
        'keep *_kt6JetID_*_*', 
        'keep *_ak7BasicJets_*_*', 
        'keep *_ak7CastorJetID_*_*', 
        'keep double_kt6CaloJetsCentral_*_*', 
        'keep double_kt6PFJetsCentralChargedPileUp_*_*', 
        'keep double_kt6PFJetsCentralNeutral_*_*', 
        'keep double_kt6PFJetsCentralNeutralTight_*_*', 
        'keep *_fixedGridRho*_*_*', 
        'keep *_ca8PFJetsCHSPrunedLinks_*_*', 
        'keep recoCaloMETs_caloMet_*_*', 
        'keep recoCaloMETs_caloMetBE_*_*', 
        'keep recoCaloMETs_caloMetBEFO_*_*', 
        'keep recoCaloMETs_caloMetM_*_*', 
        'keep recoPFMETs_pfMet_*_*', 
        'keep recoPFMETs_pfChMet_*_*', 
        'keep recoMuonMETCorrectionDataedmValueMap_muonMETValueMapProducer_*_*', 
        'keep recoHcalNoiseRBXs_hcalnoise_*_*', 
        'keep HcalNoiseSummary_hcalnoise_*_*', 
        'keep *HaloData_*_*_*', 
        'keep *BeamHaloSummary_BeamHaloSummary_*_*', 
        'keep *_MuonSeed_*_*', 
        'keep *_ancientMuonSeed_*_*', 
        'keep *_displacedMuonSeeds_*_*', 
        'keep *_mergedStandAloneMuonSeeds_*_*', 
        'keep TrackingRecHitsOwned_globalMuons_*_*', 
        'keep TrackingRecHitsOwned_tevMuons_*_*', 
        'keep recoCaloMuons_calomuons_*_*', 
        'keep *_CosmicMuonSeed_*_*', 
        'keep recoTrackExtras_cosmicMuons_*_*', 
        'keep TrackingRecHitsOwned_cosmicMuons_*_*', 
        'keep recoTrackExtras_globalCosmicMuons_*_*', 
        'keep TrackingRecHitsOwned_globalCosmicMuons_*_*', 
        'keep recoTrackExtras_cosmicMuons1Leg_*_*', 
        'keep TrackingRecHitsOwned_cosmicMuons1Leg_*_*', 
        'keep recoTrackExtras_globalCosmicMuons1Leg_*_*', 
        'keep TrackingRecHitsOwned_globalCosmicMuons1Leg_*_*', 
        'keep recoTracks_cosmicsVetoTracks_*_*', 
        'keep *_SETMuonSeed_*_*', 
        'keep recoTracks_standAloneSETMuons_*_*', 
        'keep recoTrackExtras_standAloneSETMuons_*_*', 
        'keep TrackingRecHitsOwned_standAloneSETMuons_*_*', 
        'keep recoTracks_globalSETMuons_*_*', 
        'keep recoTrackExtras_globalSETMuons_*_*', 
        'keep TrackingRecHitsOwned_globalSETMuons_*_*', 
        'keep recoMuons_muonsWithSET_*_*', 
        'keep *_muons_*_*', 
        'keep *_*_muons_*', 
        'drop *_muons_muons1stStep2muonsMap_*', 
        'drop recoIsoDepositedmValueMap_muons_*_*', 
        'drop doubleedmValueMap_muons_muPFIso*_*', 
        'keep recoTracks_standAloneMuons_*_*', 
        'keep recoTrackExtras_standAloneMuons_*_*', 
        'keep TrackingRecHitsOwned_standAloneMuons_*_*', 
        'keep recoTracks_globalMuons_*_*', 
        'keep recoTrackExtras_globalMuons_*_*', 
        'keep recoTracks_tevMuons_*_*', 
        'keep recoTrackExtras_tevMuons_*_*', 
        'keep recoTracks_generalTracks_*_*', 
        'keep recoTracksToOnerecoTracksAssociation_tevMuons_*_*', 
        'keep recoTracks_cosmicMuons_*_*', 
        'keep recoTracks_globalCosmicMuons_*_*', 
        'keep recoMuons_muonsFromCosmics_*_*', 
        'keep recoTracks_cosmicMuons1Leg_*_*', 
        'keep recoTracks_globalCosmicMuons1Leg_*_*', 
        'keep recoMuons_muonsFromCosmics1Leg_*_*', 
        'keep recoTracks_refittedStandAloneMuons_*_*', 
        'keep recoTrackExtras_refittedStandAloneMuons_*_*', 
        'keep TrackingRecHitsOwned_refittedStandAloneMuons_*_*', 
        'keep recoTracks_displacedStandAloneMuons__*', 
        'keep recoTrackExtras_displacedStandAloneMuons_*_*', 
        'keep TrackingRecHitsOwned_displacedStandAloneMuons_*_*', 
        'keep *_muIsoDepositTk_*_*', 
        'keep *_muIsoDepositCalByAssociatorTowers_*_*', 
        'keep *_muIsoDepositCalByAssociatorHits_*_*', 
        'keep *_muIsoDepositJets_*_*', 
        'keep *_muGlobalIsoDepositCtfTk_*_*', 
        'keep *_muGlobalIsoDepositCalByAssociatorTowers_*_*', 
        'keep *_muGlobalIsoDepositCalByAssociatorHits_*_*', 
        'keep *_muGlobalIsoDepositJets_*_*', 
        'keep *_impactParameterTagInfos_*_*', 
        'keep *_trackCountingHighEffBJetTags_*_*', 
        'keep *_trackCountingHighPurBJetTags_*_*', 
        'keep *_jetProbabilityBJetTags_*_*', 
        'keep *_jetBProbabilityBJetTags_*_*', 
        'keep *_secondaryVertexTagInfos_*_*', 
        'keep *_inclusiveSecondaryVertexFinderTagInfos_*_*', 
        'keep *_ghostTrackVertexTagInfos_*_*', 
        'keep *_simpleSecondaryVertexBJetTags_*_*', 
        'keep *_simpleSecondaryVertexHighEffBJetTags_*_*', 
        'keep *_simpleSecondaryVertexHighPurBJetTags_*_*', 
        'keep *_combinedSecondaryVertexBJetTags_*_*', 
        'keep *_combinedInclusiveSecondaryVertexV2BJetTags_*_*', 
        'keep *_ghostTrackBJetTags_*_*', 
        'keep *_softPFMuonsTagInfos_*_*', 
        'keep *_softPFElectronsTagInfos_*_*', 
        'keep *_softPFElectronBJetTags_*_*', 
        'keep *_softPFMuonBJetTags_*_*', 
        'keep *_softMuonTagInfos_*_*', 
        'keep *_softMuonBJetTags_*_*', 
        'keep *_softMuonByIP3dBJetTags_*_*', 
        'keep *_softMuonByPtBJetTags_*_*', 
        'keep *_combinedMVABJetTags_*_*', 
        'keep *_pfImpactParameterTagInfos_*_*', 
        'keep *_pfSecondaryVertexTagInfos_*_*', 
        'keep *_pfCombinedSecondaryVertexBJetTags_*_*', 
        'keep *_ak4PFJetsRecoTauPiZeros_*_*', 
        'keep *_hpsPFTauProducer_*_*', 
        'keep *_hpsPFTauDiscrimination*_*_*', 
        'keep *_hpsPFTau*PtSum_*_*', 
        'keep *_hpsPFTauTransverseImpactParameters_*_*', 
        'keep  *_offlinePrimaryVertices__*', 
        'keep  *_offlinePrimaryVerticesWithBS_*_*', 
        'keep  *_offlinePrimaryVerticesFromCosmicTracks_*_*', 
        'keep  *_nuclearInteractionMaker_*_*', 
        'keep *_generalV0Candidates_*_*', 
        'keep *_inclusiveSecondaryVertices_*_*', 
        'keep recoGsfElectronCores_gsfElectronCores_*_*', 
        'keep recoGsfElectrons_gsfElectrons_*_*', 
        'keep recoGsfElectronCores_uncleanedOnlyGsfElectronCores_*_*', 
        'keep recoGsfElectrons_uncleanedOnlyGsfElectrons_*_*', 
        'keep floatedmValueMap_eidRobustLoose_*_*', 
        'keep floatedmValueMap_eidRobustTight_*_*', 
        'keep floatedmValueMap_eidRobustHighEnergy_*_*', 
        'keep floatedmValueMap_eidLoose_*_*', 
        'keep floatedmValueMap_eidTight_*_*', 
        'keep *_egmGedGsfElectronPFIsolation_*_*', 
        'keep *_egmGsfElectronIDs_*_*', 
        'keep *_gedPhotonCore_*_*', 
        'keep *_gedPhotons_*_*', 
        'keep *_particleBasedIsolation_*_*', 
        'keep recoPhotons_mustachePhotons_*_*', 
        'keep recoPhotonCores_mustachePhotonCore_*_*', 
        'keep recoPhotons_photons_*_*', 
        'keep recoPhotonCores_photonCore_*_*', 
        'keep recoConversions_conversions_*_*', 
        'keep recoConversions_mustacheConversions_*_*', 
        'drop *_conversions_uncleanedConversions_*', 
        'drop *_gedPhotonsTmp_valMapPFEgammaCandToPhoton_*', 
        'keep recoConversions_allConversions_*_*', 
        'keep recoConversions_allConversionsOldEG_*_*', 
        'keep recoTracks_ckfOutInTracksFrom*Conversions_*_*', 
        'keep recoTracks_ckfInOutTracksFrom*Conversions_*_*', 
        'keep recoTrackExtras_ckfOutInTracksFrom*Conversions_*_*', 
        'keep recoTrackExtras_ckfInOutTracksFrom*Conversions_*_*', 
        'keep TrackingRecHitsOwned_ckfOutInTracksFrom*Conversions_*_*', 
        'keep TrackingRecHitsOwned_ckfInOutTracksFrom*Conversions_*_*', 
        'keep recoConversions_uncleanedOnlyAllConversions_*_*', 
        'keep recoTracks_uncleanedOnlyCkfOutInTracksFromConversions_*_*', 
        'keep recoTracks_uncleanedOnlyCkfInOutTracksFromConversions_*_*', 
        'keep recoTrackExtras_uncleanedOnlyCkfOutInTracksFromConversions_*_*', 
        'keep recoTrackExtras_uncleanedOnlyCkfInOutTracksFromConversions_*_*', 
        'keep TrackingRecHitsOwned_uncleanedOnlyCkfOutInTracksFromConversions_*_*', 
        'keep TrackingRecHitsOwned_uncleanedOnlyCkfInOutTracksFromConversions_*_*', 
        'keep *_PhotonIDProd_*_*', 
        'keep *_PhotonIDProdGED_*_*', 
        'keep *_hfRecoEcalCandidate_*_*', 
        'keep *_hfEMClusters_*_*', 
        'keep *_gedGsfElectronCores_*_*', 
        'keep *_gedGsfElectrons_*_*', 
        'keep *_pixelTracks_*_*', 
        'keep *_pixelVertices_*_*', 
        'drop CaloTowersSorted_towerMakerPF_*_*', 
        'keep recoPFRecHits_particleFlowClusterECAL_Cleaned_*', 
        'keep recoPFRecHits_particleFlowClusterHCAL_Cleaned_*', 
        'keep recoPFRecHits_particleFlowClusterHO_Cleaned_*', 
        'keep recoPFRecHits_particleFlowClusterHFEM_Cleaned_*', 
        'keep recoPFRecHits_particleFlowClusterHFHAD_Cleaned_*', 
        'keep recoPFRecHits_particleFlowClusterPS_Cleaned_*', 
        'keep recoPFRecHits_particleFlowRecHitECAL_Cleaned_*', 
        'keep recoPFRecHits_particleFlowRecHitHCAL_Cleaned_*', 
        'keep recoPFRecHits_particleFlowRecHitHO_Cleaned_*', 
        'keep recoPFRecHits_particleFlowRecHitPS_Cleaned_*', 
        'keep recoPFClusters_particleFlowClusterECAL_*_*', 
        'keep recoPFClusters_particleFlowClusterHCAL_*_*', 
        'keep recoPFClusters_particleFlowClusterHO_*_*', 
        'keep recoPFClusters_particleFlowClusterPS_*_*', 
        'keep recoPFBlocks_particleFlowBlock_*_*', 
        'keep recoPFCandidates_particleFlowEGamma_*_*', 
        'keep recoCaloClusters_particleFlowEGamma_*_*', 
        'keep recoSuperClusters_particleFlowEGamma_*_*', 
        'keep recoConversions_particleFlowEGamma_*_*', 
        'keep recoPFCandidates_particleFlow_*_*', 
        'keep recoPFCandidates_particleFlowTmp_electrons_*', 
        'keep recoPFCandidates_particleFlowTmp_*_*', 
        'drop recoPFCandidates_particleFlowTmp__*', 
        'keep recoPFDisplacedVertexs_particleFlowDisplacedVertex_*_*', 
        'keep *_pfElectronTranslator_*_*', 
        'keep *_pfPhotonTranslator_*_*', 
        'keep *_particleFlow_electrons_*', 
        'keep *_particleFlow_photons_*', 
        'keep *_trackerDrivenElectronSeeds_preid_*', 
        'keep *_particleFlowPtrs_*_*', 
        'keep *_particleFlowTmpPtrs_*_*', 
        'keep *_offlineBeamSpot_*_*', 
        'keep L1GlobalTriggerReadoutRecord_gtDigis_*_*', 
        'keep *_l1GtRecord_*_*', 
        'keep *_l1GtTriggerMenuLite_*_*', 
        'keep *_conditionsInEdm_*_*', 
        'keep *_l1extraParticles_*_*', 
        'keep *_l1L1GtObjectMap_*_*', 
        'keep L1MuGMTReadoutCollection_gtDigis_*_*', 
        'keep L1GctEmCand*_gctDigis_*_*', 
        'keep L1GctJetCand*_gctDigis_*_*', 
        'keep L1GctEtHad*_gctDigis_*_*', 
        'keep L1GctEtMiss*_gctDigis_*_*', 
        'keep L1GctEtTotal*_gctDigis_*_*', 
        'keep L1GctHtMiss*_gctDigis_*_*', 
        'keep L1GctJetCounts*_gctDigis_*_*', 
        'keep L1GctHFRingEtSums*_gctDigis_*_*', 
        'keep L1GctHFBitCounts*_gctDigis_*_*', 
        'keep LumiDetails_lumiProducer_*_*', 
        'keep LumiSummary_lumiProducer_*_*', 
        'drop *_hlt*_*_*', 
        'keep *_hltL1GtObjectMap_*_*', 
        'keep edmTriggerResults_*_*_*', 
        'keep triggerTriggerEvent_*_*_*', 
        'keep LHERunInfoProduct_*_*_*', 
        'keep LHEEventProduct_*_*_*', 
        'keep GenRunInfoProduct_generator_*_*', 
        'keep GenLumiInfoProduct_generator_*_*', 
        'keep GenEventInfoProduct_generator_*_*', 
        'keep edmHepMCProduct_generator_*_*', 
        'keep GenFilterInfo_*_*_*', 
        'keep *_genParticles_*_*', 
        'keep recoGenMETs_*_*_*', 
        'keep *_kt4GenJets_*_*', 
        'keep *_kt6GenJets_*_*', 
        'keep *_ak4GenJets_*_*', 
        'keep *_ak5GenJets_*_*', 
        'keep *_ak7GenJets_*_*', 
        'keep *_iterativeCone5GenJets_*_*', 
        'keep *_genParticle_*_*', 
        'keep edmHepMCProduct_source_*_*', 
        'keep SimTracks_g4SimHits_*_*', 
        'keep SimVertexs_g4SimHits_*_*', 
        'keep *_allTrackMCMatch_*_*', 
        'keep StripDigiSimLinkedmDetSetVector_simMuonCSCDigis_*_*', 
        'keep DTLayerIdDTDigiSimLinkMuonDigiCollection_simMuonDTDigis_*_*', 
        'keep RPCDigiSimLinkedmDetSetVector_simMuonRPCDigis_*_*', 
        'keep PileupSummaryInfos_*_*_*', 
        'keep L1AcceptBunchCrossings_*_*_*', 
        'keep L1TriggerScalerss_*_*_*', 
        'keep Level1TriggerScalerss_*_*_*', 
        'keep LumiScalerss_*_*_*', 
        'keep BeamSpotOnlines_*_*_*', 
        'keep DcsStatuss_*_*_*', 
        'keep *_logErrorHarvester_*_*', 
        'keep *_pfIsolatedElectronsEI_*_*', 
        'keep *_pfIsolatedMuonsEI_*_*', 
        'keep recoPFJets_pfJetsEI_*_*', 
        'keep *_pfJetTrackAssociatorEI_*_*', 
        'keep *_impactParameterTagInfosEI_*_*', 
        'keep *_secondaryVertexTagInfosEI_*_*', 
        'keep *_combinedSecondaryVertexBJetTagsEI_*_*', 
        'keep recoPFTaus_pfTausEI_*_*', 
        'keep recoPFTauDiscriminator_pfTausDiscrimination*_*_*', 
        'keep *_pfMetEI_*_*', 
        'keep *_simCscTriggerPrimitiveDigis_*_*', 
        'keep *_simDtTriggerPrimitiveDigis_*_*', 
        'keep *_simRpcTriggerDigis_*_*', 
        'keep *_simRctDigis_*_*', 
        'keep *_simCsctfDigis_*_*', 
        'keep *_simCsctfTrackDigis_*_*', 
        'keep *_simDttfDigis_*_*', 
        'keep *_simGctDigis_*_*', 
        'keep *_simGmtDigis_*_*', 
        'keep *_simGtDigis_*_*', 
        'keep *_cscTriggerPrimitiveDigis_*_*', 
        'keep *_dtTriggerPrimitiveDigis_*_*', 
        'keep *_rpcTriggerDigis_*_*', 
        'keep *_rctDigis_*_*', 
        'keep *_csctfDigis_*_*', 
        'keep *_csctfTrackDigis_*_*', 
        'keep *_dttfDigis_*_*', 
        'keep *_gctDigis_*_*', 
        'keep *_gmtDigis_*_*', 
        'keep *_gtDigis_*_*', 
        'keep *_gtEvmDigis_*_*', 
        'keep *_l1GtRecord_*_*', 
        'keep *_l1GtTriggerMenuLite_*_*', 
        'keep *_conditionsInEdm_*_*', 
        'keep *_l1extraParticles_*_*', 
        'keep *_l1L1GtObjectMap_*_*', 
        'keep LumiDetails_lumiProducer_*_*', 
        'keep LumiSummary_lumiProducer_*_*', 
        'drop *_trackingtruthprod_*_*', 
        'drop *_electrontruth_*_*', 
        'keep *_mix_MergedTrackTruth_*', 
        'keep CrossingFramePlaybackInfoExtended_*_*_*', 
        'keep *_simSiPixelDigis_*_*', 
        'keep *_simSiStripDigis_*_*', 
        'drop *_mix_simSiPixelDigis*_*', 
        'drop *_mix_simSiStripDigis*_*', 
        'keep *_allTrackMCMatch_*_*', 
        'keep *_trackingParticleRecoTrackAsssociation_*_*', 
        'keep *_assoc2secStepTk_*_*', 
        'keep *_assoc2thStepTk_*_*', 
        'keep *_assoc2GsfTracks_*_*', 
        'keep *_assocOutInConversionTracks_*_*', 
        'keep *_assocInOutConversionTracks_*_*', 
        'keep *_simMuonCSCDigis_*_*', 
        'keep *_simMuonDTDigis_*_*', 
        'keep *_simMuonRPCDigis_*_*', 
        'keep *_simEcalDigis_*_*', 
        'keep *_simEcalPreshowerDigis_*_*', 
        'keep *_simEcalTriggerPrimitiveDigis_*_*', 
        'keep *_simHcalDigis_*_*', 
        'keep ZDCDataFramesSorted_simHcalUnsuppressedDigis_*_*', 
        'drop ZDCDataFramesSorted_mix_simHcalUnsuppressedDigis*_*', 
        'keep *_simHcalTriggerPrimitiveDigis_*_*' ) )
)

process.FEVTDEBUGHLTEventContent = cms.PSet(
    splitLevel = cms.untracked.int32(0),
    eventAutoFlushCompressedSize = cms.untracked.int32(1048576),
    outputCommands = cms.untracked.vstring( ('drop *', 
        'drop *', 
        'drop *', 
        'drop *', 
        'keep  FEDRawDataCollection_rawDataCollector_*_*', 
        'keep  FEDRawDataCollection_source_*_*', 
        'keep  FEDRawDataCollection_rawDataCollector_*_*', 
        'keep  FEDRawDataCollection_source_*_*', 
        'drop *_hlt*_*_*', 
        'keep *_hltL1GtObjectMap_*_*', 
        'keep FEDRawDataCollection_rawDataCollector_*_*', 
        'keep FEDRawDataCollection_source_*_*', 
        'keep edmTriggerResults_*_*_*', 
        'keep triggerTriggerEvent_*_*_*', 
        'keep *_g4SimHits_*_*', 
        'keep edmHepMCProduct_source_*_*', 
        'keep *_allTrackMCMatch_*_*', 
        'keep StripDigiSimLinkedmDetSetVector_simMuonCSCDigis_*_*', 
        'keep CSCDetIdCSCComparatorDigiMuonDigiCollection_simMuonCSCDigis_*_*', 
        'keep DTLayerIdDTDigiSimLinkMuonDigiCollection_simMuonDTDigis_*_*', 
        'keep RPCDigiSimLinkedmDetSetVector_simMuonRPCDigis_*_*', 
        'keep EBSrFlagsSorted_simEcalDigis_*_*', 
        'keep EESrFlagsSorted_simEcalDigis_*_*', 
        'keep CrossingFramePlaybackInfoExtended_*_*_*', 
        'keep PileupSummaryInfos_*_*_*', 
        'keep LHERunInfoProduct_*_*_*', 
        'keep LHEEventProduct_*_*_*', 
        'keep GenRunInfoProduct_generator_*_*', 
        'keep GenLumiInfoProduct_generator_*_*', 
        'keep GenEventInfoProduct_generator_*_*', 
        'keep edmHepMCProduct_generator_*_*', 
        'keep GenFilterInfo_*_*_*', 
        'keep *_genParticles_*_*', 
        'keep recoGenJets_*_*_*', 
        'keep *_genParticle_*_*', 
        'keep recoGenMETs_*_*_*', 
        'keep FEDRawDataCollection_source_*_*', 
        'keep FEDRawDataCollection_rawDataCollector_*_*', 
        'keep *_MEtoEDMConverter_*_*', 
        'keep *_randomEngineStateProducer_*_*', 
        'keep DetIdedmEDCollection_siStripDigis_*_*', 
        'keep DetIdedmEDCollection_siPixelDigis_*_*', 
        'keep *_siPixelClusters_*_*', 
        'keep *_siStripClusters_*_*', 
        'keep *_clusterSummaryProducer_*_*', 
        'keep *_dt1DRecHits_*_*', 
        'keep *_dt4DSegments_*_*', 
        'keep *_dt1DCosmicRecHits_*_*', 
        'keep *_dt4DCosmicSegments_*_*', 
        'keep *_csc2DRecHits_*_*', 
        'keep *_cscSegments_*_*', 
        'keep *_rpcRecHits_*_*', 
        'keep *_hbhereco_*_*', 
        'keep *_hbheprereco_*_*', 
        'keep *_hfreco_*_*', 
        'keep *_horeco_*_*', 
        'keep HBHERecHitsSorted_hbherecoMB_*_*', 
        'keep HORecHitsSorted_horecoMB_*_*', 
        'keep HFRecHitsSorted_hfrecoMB_*_*', 
        'keep ZDCDataFramesSorted_*Digis_*_*', 
        'keep ZDCRecHitsSorted_*_*_*', 
        'keep *_reducedHcalRecHits_*_*', 
        'keep *_castorreco_*_*', 
        'keep HcalUnpackerReport_*_*_*', 
        'keep *_ecalPreshowerRecHit_*_*', 
        'keep *_ecalRecHit_*_*', 
        'keep *_ecalCompactTrigPrim_*_*', 
        'keep *_ecalTPSkim_*_*', 
        'keep *_selectDigi_*_*', 
        'keep EcalRecHitsSorted_reducedEcalRecHitsEE_*_*', 
        'keep EcalRecHitsSorted_reducedEcalRecHitsEB_*_*', 
        'keep EcalRecHitsSorted_reducedEcalRecHitsES_*_*', 
        'keep *_hybridSuperClusters_*_*', 
        'keep recoSuperClusters_correctedHybridSuperClusters_*_*', 
        'keep *_multi5x5SuperClusters_*_*', 
        'keep recoSuperClusters_multi5x5SuperClusters_*_*', 
        'keep recoSuperClusters_multi5x5SuperClustersWithPreshower_*_*', 
        'keep recoSuperClusters_correctedMulti5x5SuperClustersWithPreshower_*_*', 
        'keep recoPreshowerClusters_multi5x5SuperClustersWithPreshower_*_*', 
        'keep recoPreshowerClusterShapes_multi5x5PreshowerClusterShape_*_*', 
        'keep *_particleFlowSuperClusterECAL_*_*', 
        'drop recoClusterShapes_*_*_*', 
        'drop recoBasicClustersToOnerecoClusterShapesAssociation_*_*_*', 
        'drop recoBasicClusters_multi5x5BasicClusters_multi5x5BarrelBasicClusters_*', 
        'drop recoSuperClusters_multi5x5SuperClusters_multi5x5BarrelSuperClusters_*', 
        'keep *_CkfElectronCandidates_*_*', 
        'keep *_GsfGlobalElectronTest_*_*', 
        'keep *_electronMergedSeeds_*_*', 
        'keep recoGsfTracks_electronGsfTracks_*_*', 
        'keep recoGsfTrackExtras_electronGsfTracks_*_*', 
        'keep recoTrackExtras_electronGsfTracks_*_*', 
        'keep TrackingRecHitsOwned_electronGsfTracks_*_*', 
        'keep recoTracks_generalTracks_*_*', 
        'keep recoTrackExtras_generalTracks_*_*', 
        'keep TrackingRecHitsOwned_extraFromSeeds_*_*', 
        'keep uints_extraFromSeeds_*_*', 
        'keep TrackingRecHitsOwned_generalTracks_*_*', 
        'keep recoTracks_beamhaloTracks_*_*', 
        'keep recoTrackExtras_beamhaloTracks_*_*', 
        'keep TrackingRecHitsOwned_beamhaloTracks_*_*', 
        'keep recoTracks_regionalCosmicTracks_*_*', 
        'keep recoTrackExtras_regionalCosmicTracks_*_*', 
        'keep TrackingRecHitsOwned_regionalCosmicTracks_*_*', 
        'keep recoTracks_rsWithMaterialTracks_*_*', 
        'keep recoTrackExtras_rsWithMaterialTracks_*_*', 
        'keep TrackingRecHitsOwned_rsWithMaterialTracks_*_*', 
        'keep recoTracks_conversionStepTracks_*_*', 
        'keep recoTrackExtras_conversionStepTracks_*_*', 
        'keep TrackingRecHitsOwned_conversionStepTracks_*_*', 
        'keep *_ctfPixelLess_*_*', 
        'keep *_dedxTruncated40_*_*', 
        'keep *_dedxDiscrimASmi_*_*', 
        'keep *_dedxHarmonic2_*_*', 
        'keep *_trackExtrapolator_*_*', 
        'keep floatedmValueMap_generalTracks_*_*', 
        'keep *_kt6CaloJets_*_*', 
        'keep *_ak4CaloJets_*_*', 
        'keep *_ak5CaloJets_*_*', 
        'keep double_kt6PFJets_rho_*', 
        'keep *_ak4PFJets_*_*', 
        'keep *_ak5PFJets_*_*', 
        'keep *_ak8PFJets_*_*', 
        'keep *_ak4PFJetsCHS_*_*', 
        'keep *_ak5PFJetsCHS_*_*', 
        'keep *_ak8PFJetsCHS_*_*', 
        'keep *_ca8PFJetsCHS_*_*', 
        'keep *_ca8PFJetsCHSPruned_*_*', 
        'keep *_cmsTopTagPFJetsCHS_*_*', 
        'keep *_JetPlusTrackZSPCorJetAntiKt4_*_*', 
        'keep *_ak4TrackJets_*_*', 
        'keep recoRecoChargedRefCandidates_trackRefsForJets_*_*', 
        'keep *_caloTowers_*_*', 
        'keep *_towerMaker_*_*', 
        'keep *_CastorTowerReco_*_*', 
        'keep *_kt4JetTracksAssociatorAtVertex_*_*', 
        'keep *_kt4JetTracksAssociatorAtCaloFace_*_*', 
        'keep *_kt4JetExtender_*_*', 
        'keep *_ak4JetTracksAssociatorAtVertex_*_*', 
        'keep *_ak4JetTracksAssociatorAtVertexPF_*_*', 
        'keep *_ak4JetTracksAssociatorAtCaloFace_*_*', 
        'keep *_ak4JetTracksAssociatorExplicit_*_*', 
        'keep *_ak4JetExtender_*_*', 
        'keep *_ak7JetTracksAssociatorAtVertex*_*_*', 
        'keep *_ak7JetTracksAssociatorAtCaloFace*_*_*', 
        'keep *_ak7JetExtender_*_*', 
        'keep *_ak4JetID_*_*', 
        'keep *_ak5JetID_*_*', 
        'keep *_ak7JetID_*_*', 
        'keep *_ic5JetID_*_*', 
        'keep *_kt4JetID_*_*', 
        'keep *_kt6JetID_*_*', 
        'keep *_ak7BasicJets_*_*', 
        'keep *_ak7CastorJetID_*_*', 
        'keep double_kt6CaloJetsCentral_*_*', 
        'keep double_kt6PFJetsCentralChargedPileUp_*_*', 
        'keep double_kt6PFJetsCentralNeutral_*_*', 
        'keep double_kt6PFJetsCentralNeutralTight_*_*', 
        'keep *_fixedGridRho*_*_*', 
        'keep *_ca8PFJetsCHSPrunedLinks_*_*', 
        'keep recoCaloMETs_caloMet_*_*', 
        'keep recoCaloMETs_caloMetBE_*_*', 
        'keep recoCaloMETs_caloMetBEFO_*_*', 
        'keep recoCaloMETs_caloMetM_*_*', 
        'keep recoPFMETs_pfMet_*_*', 
        'keep recoPFMETs_pfChMet_*_*', 
        'keep recoMuonMETCorrectionDataedmValueMap_muonMETValueMapProducer_*_*', 
        'keep recoHcalNoiseRBXs_hcalnoise_*_*', 
        'keep HcalNoiseSummary_hcalnoise_*_*', 
        'keep *HaloData_*_*_*', 
        'keep *BeamHaloSummary_BeamHaloSummary_*_*', 
        'keep *_MuonSeed_*_*', 
        'keep *_ancientMuonSeed_*_*', 
        'keep *_displacedMuonSeeds_*_*', 
        'keep *_mergedStandAloneMuonSeeds_*_*', 
        'keep TrackingRecHitsOwned_globalMuons_*_*', 
        'keep TrackingRecHitsOwned_tevMuons_*_*', 
        'keep recoCaloMuons_calomuons_*_*', 
        'keep *_CosmicMuonSeed_*_*', 
        'keep recoTrackExtras_cosmicMuons_*_*', 
        'keep TrackingRecHitsOwned_cosmicMuons_*_*', 
        'keep recoTrackExtras_globalCosmicMuons_*_*', 
        'keep TrackingRecHitsOwned_globalCosmicMuons_*_*', 
        'keep recoTrackExtras_cosmicMuons1Leg_*_*', 
        'keep TrackingRecHitsOwned_cosmicMuons1Leg_*_*', 
        'keep recoTrackExtras_globalCosmicMuons1Leg_*_*', 
        'keep TrackingRecHitsOwned_globalCosmicMuons1Leg_*_*', 
        'keep recoTracks_cosmicsVetoTracks_*_*', 
        'keep *_SETMuonSeed_*_*', 
        'keep recoTracks_standAloneSETMuons_*_*', 
        'keep recoTrackExtras_standAloneSETMuons_*_*', 
        'keep TrackingRecHitsOwned_standAloneSETMuons_*_*', 
        'keep recoTracks_globalSETMuons_*_*', 
        'keep recoTrackExtras_globalSETMuons_*_*', 
        'keep TrackingRecHitsOwned_globalSETMuons_*_*', 
        'keep recoMuons_muonsWithSET_*_*', 
        'keep *_muons_*_*', 
        'keep *_*_muons_*', 
        'drop *_muons_muons1stStep2muonsMap_*', 
        'drop recoIsoDepositedmValueMap_muons_*_*', 
        'drop doubleedmValueMap_muons_muPFIso*_*', 
        'keep recoTracks_standAloneMuons_*_*', 
        'keep recoTrackExtras_standAloneMuons_*_*', 
        'keep TrackingRecHitsOwned_standAloneMuons_*_*', 
        'keep recoTracks_globalMuons_*_*', 
        'keep recoTrackExtras_globalMuons_*_*', 
        'keep recoTracks_tevMuons_*_*', 
        'keep recoTrackExtras_tevMuons_*_*', 
        'keep recoTracks_generalTracks_*_*', 
        'keep recoTracksToOnerecoTracksAssociation_tevMuons_*_*', 
        'keep recoTracks_cosmicMuons_*_*', 
        'keep recoTracks_globalCosmicMuons_*_*', 
        'keep recoMuons_muonsFromCosmics_*_*', 
        'keep recoTracks_cosmicMuons1Leg_*_*', 
        'keep recoTracks_globalCosmicMuons1Leg_*_*', 
        'keep recoMuons_muonsFromCosmics1Leg_*_*', 
        'keep recoTracks_refittedStandAloneMuons_*_*', 
        'keep recoTrackExtras_refittedStandAloneMuons_*_*', 
        'keep TrackingRecHitsOwned_refittedStandAloneMuons_*_*', 
        'keep recoTracks_displacedStandAloneMuons__*', 
        'keep recoTrackExtras_displacedStandAloneMuons_*_*', 
        'keep TrackingRecHitsOwned_displacedStandAloneMuons_*_*', 
        'keep *_muIsoDepositTk_*_*', 
        'keep *_muIsoDepositCalByAssociatorTowers_*_*', 
        'keep *_muIsoDepositCalByAssociatorHits_*_*', 
        'keep *_muIsoDepositJets_*_*', 
        'keep *_muGlobalIsoDepositCtfTk_*_*', 
        'keep *_muGlobalIsoDepositCalByAssociatorTowers_*_*', 
        'keep *_muGlobalIsoDepositCalByAssociatorHits_*_*', 
        'keep *_muGlobalIsoDepositJets_*_*', 
        'keep *_impactParameterTagInfos_*_*', 
        'keep *_trackCountingHighEffBJetTags_*_*', 
        'keep *_trackCountingHighPurBJetTags_*_*', 
        'keep *_jetProbabilityBJetTags_*_*', 
        'keep *_jetBProbabilityBJetTags_*_*', 
        'keep *_secondaryVertexTagInfos_*_*', 
        'keep *_inclusiveSecondaryVertexFinderTagInfos_*_*', 
        'keep *_ghostTrackVertexTagInfos_*_*', 
        'keep *_simpleSecondaryVertexBJetTags_*_*', 
        'keep *_simpleSecondaryVertexHighEffBJetTags_*_*', 
        'keep *_simpleSecondaryVertexHighPurBJetTags_*_*', 
        'keep *_combinedSecondaryVertexBJetTags_*_*', 
        'keep *_combinedInclusiveSecondaryVertexV2BJetTags_*_*', 
        'keep *_ghostTrackBJetTags_*_*', 
        'keep *_softPFMuonsTagInfos_*_*', 
        'keep *_softPFElectronsTagInfos_*_*', 
        'keep *_softPFElectronBJetTags_*_*', 
        'keep *_softPFMuonBJetTags_*_*', 
        'keep *_softMuonTagInfos_*_*', 
        'keep *_softMuonBJetTags_*_*', 
        'keep *_softMuonByIP3dBJetTags_*_*', 
        'keep *_softMuonByPtBJetTags_*_*', 
        'keep *_combinedMVABJetTags_*_*', 
        'keep *_pfImpactParameterTagInfos_*_*', 
        'keep *_pfSecondaryVertexTagInfos_*_*', 
        'keep *_pfCombinedSecondaryVertexBJetTags_*_*', 
        'keep *_ak4PFJetsRecoTauPiZeros_*_*', 
        'keep *_hpsPFTauProducer_*_*', 
        'keep *_hpsPFTauDiscrimination*_*_*', 
        'keep *_hpsPFTau*PtSum_*_*', 
        'keep *_hpsPFTauTransverseImpactParameters_*_*', 
        'keep  *_offlinePrimaryVertices__*', 
        'keep  *_offlinePrimaryVerticesWithBS_*_*', 
        'keep  *_offlinePrimaryVerticesFromCosmicTracks_*_*', 
        'keep  *_nuclearInteractionMaker_*_*', 
        'keep *_generalV0Candidates_*_*', 
        'keep *_inclusiveSecondaryVertices_*_*', 
        'keep recoGsfElectronCores_gsfElectronCores_*_*', 
        'keep recoGsfElectrons_gsfElectrons_*_*', 
        'keep recoGsfElectronCores_uncleanedOnlyGsfElectronCores_*_*', 
        'keep recoGsfElectrons_uncleanedOnlyGsfElectrons_*_*', 
        'keep floatedmValueMap_eidRobustLoose_*_*', 
        'keep floatedmValueMap_eidRobustTight_*_*', 
        'keep floatedmValueMap_eidRobustHighEnergy_*_*', 
        'keep floatedmValueMap_eidLoose_*_*', 
        'keep floatedmValueMap_eidTight_*_*', 
        'keep *_egmGedGsfElectronPFIsolation_*_*', 
        'keep *_egmGsfElectronIDs_*_*', 
        'keep *_gedPhotonCore_*_*', 
        'keep *_gedPhotons_*_*', 
        'keep *_particleBasedIsolation_*_*', 
        'keep recoPhotons_mustachePhotons_*_*', 
        'keep recoPhotonCores_mustachePhotonCore_*_*', 
        'keep recoPhotons_photons_*_*', 
        'keep recoPhotonCores_photonCore_*_*', 
        'keep recoConversions_conversions_*_*', 
        'keep recoConversions_mustacheConversions_*_*', 
        'drop *_conversions_uncleanedConversions_*', 
        'drop *_gedPhotonsTmp_valMapPFEgammaCandToPhoton_*', 
        'keep recoConversions_allConversions_*_*', 
        'keep recoConversions_allConversionsOldEG_*_*', 
        'keep recoTracks_ckfOutInTracksFrom*Conversions_*_*', 
        'keep recoTracks_ckfInOutTracksFrom*Conversions_*_*', 
        'keep recoTrackExtras_ckfOutInTracksFrom*Conversions_*_*', 
        'keep recoTrackExtras_ckfInOutTracksFrom*Conversions_*_*', 
        'keep TrackingRecHitsOwned_ckfOutInTracksFrom*Conversions_*_*', 
        'keep TrackingRecHitsOwned_ckfInOutTracksFrom*Conversions_*_*', 
        'keep recoConversions_uncleanedOnlyAllConversions_*_*', 
        'keep recoTracks_uncleanedOnlyCkfOutInTracksFromConversions_*_*', 
        'keep recoTracks_uncleanedOnlyCkfInOutTracksFromConversions_*_*', 
        'keep recoTrackExtras_uncleanedOnlyCkfOutInTracksFromConversions_*_*', 
        'keep recoTrackExtras_uncleanedOnlyCkfInOutTracksFromConversions_*_*', 
        'keep TrackingRecHitsOwned_uncleanedOnlyCkfOutInTracksFromConversions_*_*', 
        'keep TrackingRecHitsOwned_uncleanedOnlyCkfInOutTracksFromConversions_*_*', 
        'keep *_PhotonIDProd_*_*', 
        'keep *_PhotonIDProdGED_*_*', 
        'keep *_hfRecoEcalCandidate_*_*', 
        'keep *_hfEMClusters_*_*', 
        'keep *_gedGsfElectronCores_*_*', 
        'keep *_gedGsfElectrons_*_*', 
        'keep *_pixelTracks_*_*', 
        'keep *_pixelVertices_*_*', 
        'drop CaloTowersSorted_towerMakerPF_*_*', 
        'keep recoPFRecHits_particleFlowClusterECAL_Cleaned_*', 
        'keep recoPFRecHits_particleFlowClusterHCAL_Cleaned_*', 
        'keep recoPFRecHits_particleFlowClusterHO_Cleaned_*', 
        'keep recoPFRecHits_particleFlowClusterHFEM_Cleaned_*', 
        'keep recoPFRecHits_particleFlowClusterHFHAD_Cleaned_*', 
        'keep recoPFRecHits_particleFlowClusterPS_Cleaned_*', 
        'keep recoPFRecHits_particleFlowRecHitECAL_Cleaned_*', 
        'keep recoPFRecHits_particleFlowRecHitHCAL_Cleaned_*', 
        'keep recoPFRecHits_particleFlowRecHitHO_Cleaned_*', 
        'keep recoPFRecHits_particleFlowRecHitPS_Cleaned_*', 
        'keep recoPFClusters_particleFlowClusterECAL_*_*', 
        'keep recoPFClusters_particleFlowClusterHCAL_*_*', 
        'keep recoPFClusters_particleFlowClusterHO_*_*', 
        'keep recoPFClusters_particleFlowClusterPS_*_*', 
        'keep recoPFBlocks_particleFlowBlock_*_*', 
        'keep recoPFCandidates_particleFlowEGamma_*_*', 
        'keep recoCaloClusters_particleFlowEGamma_*_*', 
        'keep recoSuperClusters_particleFlowEGamma_*_*', 
        'keep recoConversions_particleFlowEGamma_*_*', 
        'keep recoPFCandidates_particleFlow_*_*', 
        'keep recoPFCandidates_particleFlowTmp_electrons_*', 
        'keep recoPFCandidates_particleFlowTmp_*_*', 
        'drop recoPFCandidates_particleFlowTmp__*', 
        'keep recoPFDisplacedVertexs_particleFlowDisplacedVertex_*_*', 
        'keep *_pfElectronTranslator_*_*', 
        'keep *_pfPhotonTranslator_*_*', 
        'keep *_particleFlow_electrons_*', 
        'keep *_particleFlow_photons_*', 
        'keep *_trackerDrivenElectronSeeds_preid_*', 
        'keep *_particleFlowPtrs_*_*', 
        'keep *_particleFlowTmpPtrs_*_*', 
        'keep *_offlineBeamSpot_*_*', 
        'keep L1GlobalTriggerReadoutRecord_gtDigis_*_*', 
        'keep *_l1GtRecord_*_*', 
        'keep *_l1GtTriggerMenuLite_*_*', 
        'keep *_conditionsInEdm_*_*', 
        'keep *_l1extraParticles_*_*', 
        'keep *_l1L1GtObjectMap_*_*', 
        'keep L1MuGMTReadoutCollection_gtDigis_*_*', 
        'keep L1GctEmCand*_gctDigis_*_*', 
        'keep L1GctJetCand*_gctDigis_*_*', 
        'keep L1GctEtHad*_gctDigis_*_*', 
        'keep L1GctEtMiss*_gctDigis_*_*', 
        'keep L1GctEtTotal*_gctDigis_*_*', 
        'keep L1GctHtMiss*_gctDigis_*_*', 
        'keep L1GctJetCounts*_gctDigis_*_*', 
        'keep L1GctHFRingEtSums*_gctDigis_*_*', 
        'keep L1GctHFBitCounts*_gctDigis_*_*', 
        'keep LumiDetails_lumiProducer_*_*', 
        'keep LumiSummary_lumiProducer_*_*', 
        'drop *_hlt*_*_*', 
        'keep *_hltL1GtObjectMap_*_*', 
        'keep edmTriggerResults_*_*_*', 
        'keep triggerTriggerEvent_*_*_*', 
        'keep LHERunInfoProduct_*_*_*', 
        'keep LHEEventProduct_*_*_*', 
        'keep GenRunInfoProduct_generator_*_*', 
        'keep GenLumiInfoProduct_generator_*_*', 
        'keep GenEventInfoProduct_generator_*_*', 
        'keep edmHepMCProduct_generator_*_*', 
        'keep GenFilterInfo_*_*_*', 
        'keep *_genParticles_*_*', 
        'keep recoGenMETs_*_*_*', 
        'keep *_kt4GenJets_*_*', 
        'keep *_kt6GenJets_*_*', 
        'keep *_ak4GenJets_*_*', 
        'keep *_ak5GenJets_*_*', 
        'keep *_ak7GenJets_*_*', 
        'keep *_iterativeCone5GenJets_*_*', 
        'keep *_genParticle_*_*', 
        'keep edmHepMCProduct_source_*_*', 
        'keep SimTracks_g4SimHits_*_*', 
        'keep SimVertexs_g4SimHits_*_*', 
        'keep *_allTrackMCMatch_*_*', 
        'keep StripDigiSimLinkedmDetSetVector_simMuonCSCDigis_*_*', 
        'keep DTLayerIdDTDigiSimLinkMuonDigiCollection_simMuonDTDigis_*_*', 
        'keep RPCDigiSimLinkedmDetSetVector_simMuonRPCDigis_*_*', 
        'keep PileupSummaryInfos_*_*_*', 
        'keep L1AcceptBunchCrossings_*_*_*', 
        'keep L1TriggerScalerss_*_*_*', 
        'keep Level1TriggerScalerss_*_*_*', 
        'keep LumiScalerss_*_*_*', 
        'keep BeamSpotOnlines_*_*_*', 
        'keep DcsStatuss_*_*_*', 
        'keep *_logErrorHarvester_*_*', 
        'keep *_pfIsolatedElectronsEI_*_*', 
        'keep *_pfIsolatedMuonsEI_*_*', 
        'keep recoPFJets_pfJetsEI_*_*', 
        'keep *_pfJetTrackAssociatorEI_*_*', 
        'keep *_impactParameterTagInfosEI_*_*', 
        'keep *_secondaryVertexTagInfosEI_*_*', 
        'keep *_combinedSecondaryVertexBJetTagsEI_*_*', 
        'keep recoPFTaus_pfTausEI_*_*', 
        'keep recoPFTauDiscriminator_pfTausDiscrimination*_*_*', 
        'keep *_pfMetEI_*_*', 
        'keep *_simCscTriggerPrimitiveDigis_*_*', 
        'keep *_simDtTriggerPrimitiveDigis_*_*', 
        'keep *_simRpcTriggerDigis_*_*', 
        'keep *_simRctDigis_*_*', 
        'keep *_simCsctfDigis_*_*', 
        'keep *_simCsctfTrackDigis_*_*', 
        'keep *_simDttfDigis_*_*', 
        'keep *_simGctDigis_*_*', 
        'keep *_simGmtDigis_*_*', 
        'keep *_simGtDigis_*_*', 
        'keep *_cscTriggerPrimitiveDigis_*_*', 
        'keep *_dtTriggerPrimitiveDigis_*_*', 
        'keep *_rpcTriggerDigis_*_*', 
        'keep *_rctDigis_*_*', 
        'keep *_csctfDigis_*_*', 
        'keep *_csctfTrackDigis_*_*', 
        'keep *_dttfDigis_*_*', 
        'keep *_gctDigis_*_*', 
        'keep *_gmtDigis_*_*', 
        'keep *_gtDigis_*_*', 
        'keep *_gtEvmDigis_*_*', 
        'keep *_l1GtRecord_*_*', 
        'keep *_l1GtTriggerMenuLite_*_*', 
        'keep *_conditionsInEdm_*_*', 
        'keep *_l1extraParticles_*_*', 
        'keep *_l1L1GtObjectMap_*_*', 
        'keep LumiDetails_lumiProducer_*_*', 
        'keep LumiSummary_lumiProducer_*_*', 
        'drop *_trackingtruthprod_*_*', 
        'drop *_electrontruth_*_*', 
        'keep *_mix_MergedTrackTruth_*', 
        'keep CrossingFramePlaybackInfoExtended_*_*_*', 
        'keep *_simSiPixelDigis_*_*', 
        'keep *_simSiStripDigis_*_*', 
        'drop *_mix_simSiPixelDigis*_*', 
        'drop *_mix_simSiStripDigis*_*', 
        'keep *_allTrackMCMatch_*_*', 
        'keep *_trackingParticleRecoTrackAsssociation_*_*', 
        'keep *_assoc2secStepTk_*_*', 
        'keep *_assoc2thStepTk_*_*', 
        'keep *_assoc2GsfTracks_*_*', 
        'keep *_assocOutInConversionTracks_*_*', 
        'keep *_assocInOutConversionTracks_*_*', 
        'keep *_simMuonCSCDigis_*_*', 
        'keep *_simMuonDTDigis_*_*', 
        'keep *_simMuonRPCDigis_*_*', 
        'keep *_simEcalDigis_*_*', 
        'keep *_simEcalPreshowerDigis_*_*', 
        'keep *_simEcalTriggerPrimitiveDigis_*_*', 
        'keep *_simHcalDigis_*_*', 
        'keep ZDCDataFramesSorted_simHcalUnsuppressedDigis_*_*', 
        'drop ZDCDataFramesSorted_mix_simHcalUnsuppressedDigis*_*', 
        'keep *_simHcalTriggerPrimitiveDigis_*_*', 
        'drop *_hlt*_*_*', 
        'keep *_hltAK4CaloJetsCorrectedIDPassed_*_*', 
        'keep *_hltAK4CaloJetsIDPassed_*_*', 
        'keep *_hltAK4CaloJets_*_*', 
        'keep *_hltAK4PFJetsCorrected_*_*', 
        'keep *_hltAK4PFJetsForTaus_*_*', 
        'keep *_hltAK4PFJets_*_*', 
        'keep *_hltAlCaEtaEBUncalibrator_*_*', 
        'keep *_hltAlCaEtaEEUncalibrator_*_*', 
        'keep *_hltAlCaEtaRecHitsFilterEBonly_*_*', 
        'keep *_hltAlCaEtaRecHitsFilterEEonly_*_*', 
        'keep *_hltAlCaEtaRecHitsFilterEEonly_etaEcalRecHitsES_*', 
        'keep *_hltAlCaEtaRecHitsFilter_*_*', 
        'keep *_hltAlCaPhiSymStream_*_*', 
        'keep *_hltAlCaPhiSymUncalibrator_*_*', 
        'keep *_hltAlCaPi0EBUncalibrator_*_*', 
        'keep *_hltAlCaPi0EEUncalibrator_*_*', 
        'keep *_hltAlCaPi0RecHitsFilterEBonly_*_*', 
        'keep *_hltAlCaPi0RecHitsFilterEEonly_*_*', 
        'keep *_hltAlCaPi0RecHitsFilterEEonly_pi0EcalRecHitsES_*', 
        'keep *_hltAlCaPi0RecHitsFilter_*_*', 
        'keep *_hltBLifetimeL25AssociatorbbPhiL1FastJetFastPV_*_*', 
        'keep *_hltBLifetimeL25BJetTagsbbPhiL1FastJetFastPV_*_*', 
        'keep *_hltBLifetimeL25TagInfosbbPhiL1FastJetFastPV_*_*', 
        'keep *_hltBLifetimeL3AssociatorbbPhiL1FastJetFastPV_*_*', 
        'keep *_hltBLifetimeL3BJetTagsbbPhiL1FastJetFastPV_*_*', 
        'keep *_hltBLifetimeL3TagInfosbbPhiL1FastJetFastPV_*_*', 
        'keep *_hltBLifetimeRegionalCtfWithMaterialTracksbbPhiL1FastJetFastPV_*_*', 
        'keep *_hltBSoftMuonDiJet110Mu5L3FilterByDR_*_*', 
        'keep *_hltBSoftMuonDiJet110Mu5SelL3BJetTagsByDR_*_*', 
        'keep *_hltBSoftMuonDiJet20Mu5L3FilterByDR_*_*', 
        'keep *_hltBSoftMuonDiJet20Mu5SelL3BJetTagsByDR_*_*', 
        'keep *_hltBSoftMuonDiJet40Mu5L3FilterByDR_*_*', 
        'keep *_hltBSoftMuonDiJet40Mu5SelL3BJetTagsByDR_*_*', 
        'keep *_hltBSoftMuonDiJet70Mu5L3FilterByDR_*_*', 
        'keep *_hltBSoftMuonDiJet70Mu5SelL3BJetTagsByDR_*_*', 
        'keep *_hltBSoftMuonMu5L3_*_*', 
        'keep *_hltCSVJetTagSingleTopEle27_*_*', 
        'keep *_hltCSVJetTagSingleTopIsoMu24_*_*', 
        'keep *_hltCaloJetCorrectedRegional_*_*', 
        'keep *_hltCaloJetCorrected_*_*', 
        'keep *_hltCaloJetL1FastJetCorrected_*_*', 
        'keep *_hltCleanedCorrectedIslandBarrelSuperClustersHI_*_*', 
        'keep *_hltCleanedHiCorrectedIslandBarrelSuperClustersHI_*_*', 
        'keep *_hltConvPFTausTightIsoTrackFindingIsolation_*_*', 
        'keep *_hltConvPFTausTightIsoTrackFinding_*_*', 
        'keep *_hltConvPFTausTightIsoTrackPt5Isolation_*_*', 
        'keep *_hltConvPFTausTightIsoTrackPt5_*_*', 
        'keep *_hltConvPFTausTightIso_*_*', 
        'keep *_hltConvPFTausTrackFindingLooseIsolation_*_*', 
        'keep *_hltConvPFTausTrackFinding_*_*', 
        'keep *_hltConvPFTaus_*_*', 
        'keep *_hltCorrectedIslandBarrelSuperClustersHI_*_*', 
        'keep *_hltCorrectedIslandEndcapSuperClustersHI_*_*', 
        'keep *_hltCsc2DRecHits_*_*', 
        'keep *_hltCscSegments_*_*', 
        'keep *_hltDoublePFTau25TrackPt5MediumIsolationProng4L1HLTMatched_*_*', 
        'keep *_hltDoublePFTau25TrackPt5MediumIsolationProng4_*_*', 
        'keep *_hltDoublePFTau25TrackPt5MediumIsolation_*_*', 
        'keep *_hltDoublePFTau25TrackPt5_*_*', 
        'keep *_hltDoublePFTau25_*_*', 
        'keep *_hltDoublePFTauTightIso45Track5_*_*', 
        'keep *_hltDoublePFTauTightIso45Track_*_*', 
        'keep *_hltDt4DSegments_*_*', 
        'keep *_hltEcalRecHitAll_*_*', 
        'keep *_hltEle20CaloIdVTCaloIsoTTrkIdTTrkIsoTTrackIsoFilterL1SingleEG18orL1SingleEG20_*_*', 
        'keep *_hltEle20CaloIdVTTrkIdTDphiFilter_*_*', 
        'keep *_hltEle27WP85PixelMatchElectronsL1Seeded_*_*', 
        'keep *_hltFastPVPixelTracksMerger_*_*', 
        'keep *_hltFastPVPixelTracksRecover_*_*', 
        'keep *_hltFastPVPixelTracks_*_*', 
        'keep *_hltFastPVPixelVertices3D_*_*', 
        'keep *_hltFastPVPixelVertices_*_*', 
        'keep *_hltFastPixelBLifetimeL3TagInfos_*_*', 
        'keep *_hltFastPrimaryVertex_*_*', 
        'keep *_hltFilterDoubleIsoPFTau45Trk5LeadTrack5IsolationL1HLTMatched_*_*', 
        'keep *_hltFilterL2EtCutDoublePFIsoTau45Trk5_*_*', 
        'keep *_hltFilterL2EtCutSingleIsoPFTau35Trk20MET70_*_*', 
        'keep *_hltFilterSingleIsoPFTau35Trk20LeadTrackPt20_*_*', 
        'keep *_hltFilterSingleIsoPFTau35Trk20MET60LeadTrack20IsolationL1HLTMatched_*_*', 
        'keep *_hltFilterSingleIsoPFTau35Trk20MET70LeadTrack20IsolationL1HLTMatched_*_*', 
        'keep *_hltGoodOnlinePVs_*_*', 
        'keep *_hltHICaloJetCorrected_*_*', 
        'keep *_hltHICaloJetIDPassed_*_*', 
        'keep *_hltHIGoodLooseTracks_*_*', 
        'keep *_hltHIPixel3PrimTracks_*_*', 
        'keep *_hltHISelectedVertex_*_*', 
        'keep *_hltHISiPixelClusters_*_*', 
        'keep *_hltHITIPTCorrectorHB_*_*', 
        'keep *_hltHITIPTCorrectorHE_*_*', 
        'keep *_hltHiCorrectedIslandBarrelSuperClustersHI_*_*', 
        'keep *_hltHiCorrectedIslandEndcapSuperClustersHI_*_*', 
        'keep *_hltHiIslandSuperClustersHI_*_*', 
        'keep *_hltIsolPixelTrackProdHB_*_*', 
        'keep *_hltIsolPixelTrackProdHE_*_*', 
        'keep *_hltIter0PFlowTrackSelectionHighPurity_*_*', 
        'keep *_hltIter1Merged_*_*', 
        'keep *_hltIter1PFlowTrackSelectionHighPurity_*_*', 
        'keep *_hltIter2Merged_*_*', 
        'keep *_hltIter2PFlowTrackSelectionHighPurity_*_*', 
        'keep *_hltIter3Merged_*_*', 
        'keep *_hltIter4Merged_*_*', 
        'keep *_hltIterativeCone5PileupSubtractionCaloJets_*_*', 
        'keep *_hltL1GtObjectMap_*_*', 
        'keep *_hltL1HLTSingleIsoPFTau35Trk20Met60JetsMatch_*_*', 
        'keep *_hltL1IsoElectronTrackIsol_*_*', 
        'keep *_hltL1NonIsoElectronTrackIsol_*_*', 
        'keep *_hltL1SeededRecoEcalCandidate_*_*', 
        'keep *_hltL1extraParticlesCentral_*_*', 
        'keep *_hltL1extraParticlesNonIsolated_*_*', 
        'keep *_hltL1extraParticlesTau_*_*', 
        'keep *_hltL1extraParticles_*_*', 
        'keep *_hltL1sDoubleTauJet44Eta2p17orDoubleJet64Central_*_*', 
        'keep *_hltL1sDoubleTauJet44erorDoubleJetC64_*_*', 
        'keep *_hltL1sL1EG18er_*_*', 
        'keep *_hltL1sL1ETM36ORETM40_*_*', 
        'keep *_hltL1sL1Jet52ETM30_*_*', 
        'keep *_hltL1sL1SingleEG12_*_*', 
        'keep *_hltL1sL1SingleEG15_*_*', 
        'keep *_hltL1sL1SingleEG18orL1SingleEG20_*_*', 
        'keep *_hltL1sL1SingleMu10_*_*', 
        'keep *_hltL1sL1SingleMu14Eta2p1_*_*', 
        'keep *_hltL1sMu16Eta2p1_*_*', 
        'keep *_hltL2MuonCandidatesNoVtx_*_*', 
        'keep *_hltL2MuonCandidates_*_*', 
        'keep *_hltL2MuonSeeds_*_*', 
        'keep *_hltL2Muons_*_*', 
        'keep *_hltL2TauJets_*_*', 
        'keep *_hltL3CombinedSecondaryVertexBJetTags_*_*', 
        'keep *_hltL3MuonCandidates_*_*', 
        'keep *_hltL3MuonsIOHit_*_*', 
        'keep *_hltL3MuonsLinksCombination_*_*', 
        'keep *_hltL3MuonsOIHit_*_*', 
        'keep *_hltL3MuonsOIState_*_*', 
        'keep *_hltL3Muons_*_*', 
        'keep *_hltL3NoFiltersNoVtxMuonCandidates_*_*', 
        'keep *_hltL3NoFiltersNoVtxMuons_*_*', 
        'keep *_hltL3SecondaryVertexTagInfos_*_*', 
        'keep *_hltL3TkFromL2OICombination_*_*', 
        'keep *_hltL3TkTracksFromL2IOHit_*_*', 
        'keep *_hltL3TkTracksFromL2OIHit_*_*', 
        'keep *_hltL3TkTracksFromL2OIState_*_*', 
        'keep *_hltL3TkTracksFromL2_*_*', 
        'keep *_hltL3TrackCandidateFromL2IOHit_*_*', 
        'keep *_hltL3TrackCandidateFromL2OIHit_*_*', 
        'keep *_hltL3TrackCandidateFromL2OIState_*_*', 
        'keep *_hltL3TrajSeedIOHit_*_*', 
        'keep *_hltL3TrajSeedOIHit_*_*', 
        'keep *_hltL3TrajSeedOIState_*_*', 
        'keep *_hltL3TrajectorySeed_*_*', 
        'keep *_hltL3crIsoL1sMu16Eta2p1L1f0L2f16QL3f18QL3crIsoRhoFiltered0p15_*_*', 
        'keep *_hltLeadingCentralJets30SingleTopEle27_*_*', 
        'keep *_hltLeadingCentralJets30SingleTopIsoMu24_*_*', 
        'keep *_hltMetForHf_*_*', 
        'keep *_hltMet_*_*', 
        'keep *_hltMu8Ele17CaloIdTCaloIsoVLPixelMatchFilter_*_*', 
        'keep *_hltMuTrackJpsiCtfTrackCands_*_*', 
        'keep *_hltMuTrackJpsiPixelTrackCands_*_*', 
        'keep *_hltMuonCSCDigis_*_*', 
        'keep *_hltMuonCSCDigis_MuonCSCStripDigi_*', 
        'keep *_hltMuonCSCDigis_MuonCSCWireDigi_*', 
        'keep *_hltMuonDTDigis_*_*', 
        'keep *_hltMuonRPCDigis_*_*', 
        'keep *_hltOnlineBeamSpot_*_*', 
        'keep *_hltOnlinePrimaryVertices_*_*', 
        'keep *_hltOverlapFilterEle20LooseIsoPFTau20OldVersion_*_*', 
        'keep *_hltOverlapFilterIsoEle20MediumIsoPFTau20_*_*', 
        'keep *_hltOverlapFilterIsoMu15IsoPFTau20_*_*', 
        'keep *_hltOverlapFilterIsoMu15MediumIsoPFTau20_*_*', 
        'keep *_hltOverlapFilterIsoMu15TightIsoPFTau20_*_*', 
        'keep *_hltOverlapFilterIsoMu18LooseIsoPFTau20_*_*', 
        'keep *_hltOverlapFilterIsoMu18PFTau25TrackPt5Prong4_*_*', 
        'keep *_hltPFTau15TrackLooseIso_*_*', 
        'keep *_hltPFTau15Track_*_*', 
        'keep *_hltPFTau15_*_*', 
        'keep *_hltPFTau20IsoMuVertex_*_*', 
        'keep *_hltPFTau20TrackLooseIso_*_*', 
        'keep *_hltPFTau20Track_*_*', 
        'keep *_hltPFTau20_*_*', 
        'keep *_hltPFTau25TrackPt5MediumIsolationProng4IsoMuVertex_*_*', 
        'keep *_hltPFTau25TrackPt5MediumIsolationProng4_*_*', 
        'keep *_hltPFTau25TrackPt5MediumIsolation_*_*', 
        'keep *_hltPFTau25TrackPt5_*_*', 
        'keep *_hltPFTau25_*_*', 
        'keep *_hltPFTau35TrackPt20LooseIsoProng2_*_*', 
        'keep *_hltPFTau35TrackPt20LooseIso_*_*', 
        'keep *_hltPFTau35TrackPt20_*_*', 
        'keep *_hltPFTau35Track_*_*', 
        'keep *_hltPFTau35_*_*', 
        'keep *_hltPFTauEleVertex20_*_*', 
        'keep *_hltPFTauJetTracksAssociator_*_*', 
        'keep *_hltPFTauMediumIso20TrackMediumIso_*_*', 
        'keep *_hltPFTauMediumIso20Track_*_*', 
        'keep *_hltPFTauMediumIso20_*_*', 
        'keep *_hltPFTauMediumIso35Track_*_*', 
        'keep *_hltPFTauMediumIso35_*_*', 
        'keep *_hltPFTauTagInfo_*_*', 
        'keep *_hltPFTauTightIso20TrackTightIso_*_*', 
        'keep *_hltPFTauTightIso20Track_*_*', 
        'keep *_hltPFTauTightIso20_*_*', 
        'keep *_hltPFlowTrackSelectionHighPurity_*_*', 
        'keep *_hltParticleFlowForTaus_*_*', 
        'keep *_hltParticleFlow_*_*', 
        'keep *_hltPixelMatch3HitElectronsActivity_*_*', 
        'keep *_hltPixelMatch3HitElectronsL1Seeded_*_*', 
        'keep *_hltPixelMatchCleanElectronsL1Seeded_*_*', 
        'keep *_hltPixelMatchElectronsActivity_*_*', 
        'keep *_hltPixelMatchElectronsL1Iso_*_*', 
        'keep *_hltPixelMatchElectronsL1NonIso_*_*', 
        'keep *_hltPixelMatchElectronsL1Seeded_*_*', 
        'keep *_hltPixelTracks_*_*', 
        'keep *_hltPixelVertices3DbbPhi_*_*', 
        'keep *_hltPixelVertices_*_*', 
        'keep *_hltRecoEcalSuperClusterActivityCandidateSC4_*_*', 
        'keep *_hltRecoEcalSuperClusterActivityCandidateSC5_*_*', 
        'keep *_hltRecoEcalSuperClusterActivityCandidate_*_*', 
        'keep *_hltRpcRecHits_*_*', 
        'keep *_hltSelector4CentralJetsL1FastJet_*_*', 
        'keep *_hltSelectorJets20L1FastJet_*_*', 
        'keep *_hltSiPixelCluster_*_*', 
        'keep *_hltSiPixelClusters_*_*', 
        'keep *_hltSiStripClusters_*_*', 
        'keep *_hltSiStripRawToClustersFacility_*_*', 
        'keep *_hltSingleMu15L3Filtered15_*_*', 
        'keep *_hltSingleMuIsoL1s14L3IsoFiltered15eta2p1_*_*', 
        'keep *_hltSingleMuIsoL3IsoFiltered15_*_*', 
        'keep *_hltTowerMakerForAll_*_*', 
        'keep *_hltTowerMakerForMuons_*_*', 
        'keep *_hltTriggerSummaryAOD_*_*', 
        'keep *_hltTriggerSummaryRAW_*_*', 
        'keep *_hltTrimmedPixelVertices_*_*', 
        'keep *_hltVerticesL3_*_*', 
        'keep DcsStatuss_hltScalersRawToDigi_*_*', 
        'keep FEDRawDataCollection_rawDataCollector_*_*', 
        'keep FEDRawDataCollection_rawDataRepacker_*_*', 
        'keep FEDRawDataCollection_source_*_*', 
        'keep FEDRawDataCollection_virginRawDataRepacker_*_*', 
        'keep L1GlobalTriggerReadoutRecord_hltGtDigis_*_*', 
        'keep L1MuGMTCands_hltGtDigis_*_*', 
        'keep L1MuGMTReadoutCollection_hltGtDigis_*_*', 
        'keep L2MuonTrajectorySeeds_hltL2MuonSeeds_*_*', 
        'keep L3MuonTrajectorySeeds_hltHIL3TrajSeedOIHit_*_*', 
        'keep L3MuonTrajectorySeeds_hltHIL3TrajectorySeed_*_*', 
        'keep L3MuonTrajectorySeeds_hltL3TrajSeedOIState_*_*', 
        'keep LumiScalerss_hltScalersRawToDigi_*_*', 
        'keep SiPixelClusteredmNewDetSetVector_hltSiPixelClusters_*_*', 
        'keep TrackCandidates_hltHIL3TrackCandidateFromL2OIHit_*_*', 
        'keep TrackCandidates_hltHIL3TrackCandidateFromL2OIState_*_*', 
        'keep TrackingRecHitsOwned_hltL3Muons_*_*', 
        'keep edmTriggerResults_*_*_*', 
        'keep recoCaloJets_*_*_*', 
        'keep recoCaloMETs_*_*_*', 
        'keep recoCaloMETs_hltMet_*_*', 
        'keep recoCompositeCandidates_*_*_*', 
        'keep recoElectrons_*_*_*', 
        'keep recoIsolatedPixelTrackCandidates_*_*_*', 
        'keep recoMETs_*_*_*', 
        'keep recoPFJets_*_*_*', 
        'keep recoPFTaus_*_*_*', 
        'keep recoRecoChargedCandidates_*_*_*', 
        'keep recoRecoChargedCandidates_hltHIL3MuonCandidates_*_*', 
        'keep recoRecoChargedCandidates_hltL2MuonCandidates_*_*', 
        'keep recoRecoEcalCandidatesToValuefloatAssociation_hltL1IsoHLTClusterShape_*_*', 
        'keep recoRecoEcalCandidatesToValuefloatAssociation_hltL1IsolatedPhotonEcalIsol_*_*', 
        'keep recoRecoEcalCandidatesToValuefloatAssociation_hltL1IsolatedPhotonHcalForHE_*_*', 
        'keep recoRecoEcalCandidatesToValuefloatAssociation_hltL1IsolatedPhotonHcalIsol_*_*', 
        'keep recoRecoEcalCandidatesToValuefloatAssociation_hltL1NonIsoHLTClusterShape_*_*', 
        'keep recoRecoEcalCandidatesToValuefloatAssociation_hltL1NonIsolatedPhotonEcalIsol_*_*', 
        'keep recoRecoEcalCandidatesToValuefloatAssociation_hltL1NonIsolatedPhotonHcalForHE_*_*', 
        'keep recoRecoEcalCandidatesToValuefloatAssociation_hltL1NonIsolatedPhotonHcalIsol_*_*', 
        'keep recoRecoEcalCandidates_*_*_*', 
        'keep recoRecoEcalCandidates_hltL1IsoRecoEcalCandidate_*_*', 
        'keep recoRecoEcalCandidates_hltL1NonIsoRecoEcalCandidate_*_*', 
        'keep recoTrackExtras_hltHIL3MuonsOIHit_*_*', 
        'keep recoTrackExtras_hltHIL3MuonsOIState_*_*', 
        'keep recoTracks_hltHIL3MuonsOIHit_*_*', 
        'keep recoTracks_hltHIL3MuonsOIState_*_*', 
        'keep recoTracks_hltHIL3Muons_*_*', 
        'keep recoTracks_hltHIL3TkTracksFromL2OIHit_*_*', 
        'keep recoTracks_hltHIL3TkTracksFromL2OIState_*_*', 
        'keep recoTracks_hltHIL3TkTracksFromL2_*_*', 
        'keep triggerTriggerEventWithRefs_*_*_*', 
        'keep triggerTriggerEvent_*_*_*', 
        'keep triggerTriggerFilterObjectWithRefs_*_*_*' ) )
)

process.FEVTEventContent = cms.PSet(
    splitLevel = cms.untracked.int32(0),
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
    outputCommands = cms.untracked.vstring( ('drop *', 
        'drop *', 
        'keep  FEDRawDataCollection_rawDataCollector_*_*', 
        'keep  FEDRawDataCollection_source_*_*', 
        'keep  FEDRawDataCollection_rawDataCollector_*_*', 
        'keep  FEDRawDataCollection_source_*_*', 
        'drop *_hlt*_*_*', 
        'keep *_hltL1GtObjectMap_*_*', 
        'keep FEDRawDataCollection_rawDataCollector_*_*', 
        'keep FEDRawDataCollection_source_*_*', 
        'keep edmTriggerResults_*_*_*', 
        'keep triggerTriggerEvent_*_*_*', 
        'keep DetIdedmEDCollection_siStripDigis_*_*', 
        'keep DetIdedmEDCollection_siPixelDigis_*_*', 
        'keep *_siPixelClusters_*_*', 
        'keep *_siStripClusters_*_*', 
        'keep *_clusterSummaryProducer_*_*', 
        'keep *_dt1DRecHits_*_*', 
        'keep *_dt4DSegments_*_*', 
        'keep *_dt1DCosmicRecHits_*_*', 
        'keep *_dt4DCosmicSegments_*_*', 
        'keep *_csc2DRecHits_*_*', 
        'keep *_cscSegments_*_*', 
        'keep *_rpcRecHits_*_*', 
        'keep *_hbhereco_*_*', 
        'keep *_hbheprereco_*_*', 
        'keep *_hfreco_*_*', 
        'keep *_horeco_*_*', 
        'keep HBHERecHitsSorted_hbherecoMB_*_*', 
        'keep HORecHitsSorted_horecoMB_*_*', 
        'keep HFRecHitsSorted_hfrecoMB_*_*', 
        'keep ZDCDataFramesSorted_*Digis_*_*', 
        'keep ZDCRecHitsSorted_*_*_*', 
        'keep *_reducedHcalRecHits_*_*', 
        'keep *_castorreco_*_*', 
        'keep HcalUnpackerReport_*_*_*', 
        'keep *_ecalPreshowerRecHit_*_*', 
        'keep *_ecalRecHit_*_*', 
        'keep *_ecalCompactTrigPrim_*_*', 
        'keep *_ecalTPSkim_*_*', 
        'keep *_selectDigi_*_*', 
        'keep EcalRecHitsSorted_reducedEcalRecHitsEE_*_*', 
        'keep EcalRecHitsSorted_reducedEcalRecHitsEB_*_*', 
        'keep EcalRecHitsSorted_reducedEcalRecHitsES_*_*', 
        'keep *_hybridSuperClusters_*_*', 
        'keep recoSuperClusters_correctedHybridSuperClusters_*_*', 
        'keep *_multi5x5SuperClusters_*_*', 
        'keep recoSuperClusters_multi5x5SuperClusters_*_*', 
        'keep recoSuperClusters_multi5x5SuperClustersWithPreshower_*_*', 
        'keep recoSuperClusters_correctedMulti5x5SuperClustersWithPreshower_*_*', 
        'keep recoPreshowerClusters_multi5x5SuperClustersWithPreshower_*_*', 
        'keep recoPreshowerClusterShapes_multi5x5PreshowerClusterShape_*_*', 
        'keep *_particleFlowSuperClusterECAL_*_*', 
        'drop recoClusterShapes_*_*_*', 
        'drop recoBasicClustersToOnerecoClusterShapesAssociation_*_*_*', 
        'drop recoBasicClusters_multi5x5BasicClusters_multi5x5BarrelBasicClusters_*', 
        'drop recoSuperClusters_multi5x5SuperClusters_multi5x5BarrelSuperClusters_*', 
        'keep *_CkfElectronCandidates_*_*', 
        'keep *_GsfGlobalElectronTest_*_*', 
        'keep *_electronMergedSeeds_*_*', 
        'keep recoGsfTracks_electronGsfTracks_*_*', 
        'keep recoGsfTrackExtras_electronGsfTracks_*_*', 
        'keep recoTrackExtras_electronGsfTracks_*_*', 
        'keep TrackingRecHitsOwned_electronGsfTracks_*_*', 
        'keep recoTracks_generalTracks_*_*', 
        'keep recoTrackExtras_generalTracks_*_*', 
        'keep TrackingRecHitsOwned_extraFromSeeds_*_*', 
        'keep uints_extraFromSeeds_*_*', 
        'keep TrackingRecHitsOwned_generalTracks_*_*', 
        'keep recoTracks_beamhaloTracks_*_*', 
        'keep recoTrackExtras_beamhaloTracks_*_*', 
        'keep TrackingRecHitsOwned_beamhaloTracks_*_*', 
        'keep recoTracks_regionalCosmicTracks_*_*', 
        'keep recoTrackExtras_regionalCosmicTracks_*_*', 
        'keep TrackingRecHitsOwned_regionalCosmicTracks_*_*', 
        'keep recoTracks_rsWithMaterialTracks_*_*', 
        'keep recoTrackExtras_rsWithMaterialTracks_*_*', 
        'keep TrackingRecHitsOwned_rsWithMaterialTracks_*_*', 
        'keep recoTracks_conversionStepTracks_*_*', 
        'keep recoTrackExtras_conversionStepTracks_*_*', 
        'keep TrackingRecHitsOwned_conversionStepTracks_*_*', 
        'keep *_ctfPixelLess_*_*', 
        'keep *_dedxTruncated40_*_*', 
        'keep *_dedxDiscrimASmi_*_*', 
        'keep *_dedxHarmonic2_*_*', 
        'keep *_trackExtrapolator_*_*', 
        'keep floatedmValueMap_generalTracks_*_*', 
        'keep *_kt6CaloJets_*_*', 
        'keep *_ak4CaloJets_*_*', 
        'keep *_ak5CaloJets_*_*', 
        'keep double_kt6PFJets_rho_*', 
        'keep *_ak4PFJets_*_*', 
        'keep *_ak5PFJets_*_*', 
        'keep *_ak8PFJets_*_*', 
        'keep *_ak4PFJetsCHS_*_*', 
        'keep *_ak5PFJetsCHS_*_*', 
        'keep *_ak8PFJetsCHS_*_*', 
        'keep *_ca8PFJetsCHS_*_*', 
        'keep *_ca8PFJetsCHSPruned_*_*', 
        'keep *_cmsTopTagPFJetsCHS_*_*', 
        'keep *_JetPlusTrackZSPCorJetAntiKt4_*_*', 
        'keep *_ak4TrackJets_*_*', 
        'keep recoRecoChargedRefCandidates_trackRefsForJets_*_*', 
        'keep *_caloTowers_*_*', 
        'keep *_towerMaker_*_*', 
        'keep *_CastorTowerReco_*_*', 
        'keep *_kt4JetTracksAssociatorAtVertex_*_*', 
        'keep *_kt4JetTracksAssociatorAtCaloFace_*_*', 
        'keep *_kt4JetExtender_*_*', 
        'keep *_ak4JetTracksAssociatorAtVertex_*_*', 
        'keep *_ak4JetTracksAssociatorAtVertexPF_*_*', 
        'keep *_ak4JetTracksAssociatorAtCaloFace_*_*', 
        'keep *_ak4JetTracksAssociatorExplicit_*_*', 
        'keep *_ak4JetExtender_*_*', 
        'keep *_ak7JetTracksAssociatorAtVertex*_*_*', 
        'keep *_ak7JetTracksAssociatorAtCaloFace*_*_*', 
        'keep *_ak7JetExtender_*_*', 
        'keep *_ak4JetID_*_*', 
        'keep *_ak5JetID_*_*', 
        'keep *_ak7JetID_*_*', 
        'keep *_ic5JetID_*_*', 
        'keep *_kt4JetID_*_*', 
        'keep *_kt6JetID_*_*', 
        'keep *_ak7BasicJets_*_*', 
        'keep *_ak7CastorJetID_*_*', 
        'keep double_kt6CaloJetsCentral_*_*', 
        'keep double_kt6PFJetsCentralChargedPileUp_*_*', 
        'keep double_kt6PFJetsCentralNeutral_*_*', 
        'keep double_kt6PFJetsCentralNeutralTight_*_*', 
        'keep *_fixedGridRho*_*_*', 
        'keep *_ca8PFJetsCHSPrunedLinks_*_*', 
        'keep recoCaloMETs_caloMet_*_*', 
        'keep recoCaloMETs_caloMetBE_*_*', 
        'keep recoCaloMETs_caloMetBEFO_*_*', 
        'keep recoCaloMETs_caloMetM_*_*', 
        'keep recoPFMETs_pfMet_*_*', 
        'keep recoPFMETs_pfChMet_*_*', 
        'keep recoMuonMETCorrectionDataedmValueMap_muonMETValueMapProducer_*_*', 
        'keep recoHcalNoiseRBXs_hcalnoise_*_*', 
        'keep HcalNoiseSummary_hcalnoise_*_*', 
        'keep *HaloData_*_*_*', 
        'keep *BeamHaloSummary_BeamHaloSummary_*_*', 
        'keep *_MuonSeed_*_*', 
        'keep *_ancientMuonSeed_*_*', 
        'keep *_displacedMuonSeeds_*_*', 
        'keep *_mergedStandAloneMuonSeeds_*_*', 
        'keep TrackingRecHitsOwned_globalMuons_*_*', 
        'keep TrackingRecHitsOwned_tevMuons_*_*', 
        'keep recoCaloMuons_calomuons_*_*', 
        'keep *_CosmicMuonSeed_*_*', 
        'keep recoTrackExtras_cosmicMuons_*_*', 
        'keep TrackingRecHitsOwned_cosmicMuons_*_*', 
        'keep recoTrackExtras_globalCosmicMuons_*_*', 
        'keep TrackingRecHitsOwned_globalCosmicMuons_*_*', 
        'keep recoTrackExtras_cosmicMuons1Leg_*_*', 
        'keep TrackingRecHitsOwned_cosmicMuons1Leg_*_*', 
        'keep recoTrackExtras_globalCosmicMuons1Leg_*_*', 
        'keep TrackingRecHitsOwned_globalCosmicMuons1Leg_*_*', 
        'keep recoTracks_cosmicsVetoTracks_*_*', 
        'keep *_SETMuonSeed_*_*', 
        'keep recoTracks_standAloneSETMuons_*_*', 
        'keep recoTrackExtras_standAloneSETMuons_*_*', 
        'keep TrackingRecHitsOwned_standAloneSETMuons_*_*', 
        'keep recoTracks_globalSETMuons_*_*', 
        'keep recoTrackExtras_globalSETMuons_*_*', 
        'keep TrackingRecHitsOwned_globalSETMuons_*_*', 
        'keep recoMuons_muonsWithSET_*_*', 
        'keep *_muons_*_*', 
        'keep *_*_muons_*', 
        'drop *_muons_muons1stStep2muonsMap_*', 
        'drop recoIsoDepositedmValueMap_muons_*_*', 
        'drop doubleedmValueMap_muons_muPFIso*_*', 
        'keep recoTracks_standAloneMuons_*_*', 
        'keep recoTrackExtras_standAloneMuons_*_*', 
        'keep TrackingRecHitsOwned_standAloneMuons_*_*', 
        'keep recoTracks_globalMuons_*_*', 
        'keep recoTrackExtras_globalMuons_*_*', 
        'keep recoTracks_tevMuons_*_*', 
        'keep recoTrackExtras_tevMuons_*_*', 
        'keep recoTracks_generalTracks_*_*', 
        'keep recoTracksToOnerecoTracksAssociation_tevMuons_*_*', 
        'keep recoTracks_cosmicMuons_*_*', 
        'keep recoTracks_globalCosmicMuons_*_*', 
        'keep recoMuons_muonsFromCosmics_*_*', 
        'keep recoTracks_cosmicMuons1Leg_*_*', 
        'keep recoTracks_globalCosmicMuons1Leg_*_*', 
        'keep recoMuons_muonsFromCosmics1Leg_*_*', 
        'keep recoTracks_refittedStandAloneMuons_*_*', 
        'keep recoTrackExtras_refittedStandAloneMuons_*_*', 
        'keep TrackingRecHitsOwned_refittedStandAloneMuons_*_*', 
        'keep recoTracks_displacedStandAloneMuons__*', 
        'keep recoTrackExtras_displacedStandAloneMuons_*_*', 
        'keep TrackingRecHitsOwned_displacedStandAloneMuons_*_*', 
        'keep *_muIsoDepositTk_*_*', 
        'keep *_muIsoDepositCalByAssociatorTowers_*_*', 
        'keep *_muIsoDepositCalByAssociatorHits_*_*', 
        'keep *_muIsoDepositJets_*_*', 
        'keep *_muGlobalIsoDepositCtfTk_*_*', 
        'keep *_muGlobalIsoDepositCalByAssociatorTowers_*_*', 
        'keep *_muGlobalIsoDepositCalByAssociatorHits_*_*', 
        'keep *_muGlobalIsoDepositJets_*_*', 
        'keep *_impactParameterTagInfos_*_*', 
        'keep *_trackCountingHighEffBJetTags_*_*', 
        'keep *_trackCountingHighPurBJetTags_*_*', 
        'keep *_jetProbabilityBJetTags_*_*', 
        'keep *_jetBProbabilityBJetTags_*_*', 
        'keep *_secondaryVertexTagInfos_*_*', 
        'keep *_inclusiveSecondaryVertexFinderTagInfos_*_*', 
        'keep *_ghostTrackVertexTagInfos_*_*', 
        'keep *_simpleSecondaryVertexBJetTags_*_*', 
        'keep *_simpleSecondaryVertexHighEffBJetTags_*_*', 
        'keep *_simpleSecondaryVertexHighPurBJetTags_*_*', 
        'keep *_combinedSecondaryVertexBJetTags_*_*', 
        'keep *_combinedInclusiveSecondaryVertexV2BJetTags_*_*', 
        'keep *_ghostTrackBJetTags_*_*', 
        'keep *_softPFMuonsTagInfos_*_*', 
        'keep *_softPFElectronsTagInfos_*_*', 
        'keep *_softPFElectronBJetTags_*_*', 
        'keep *_softPFMuonBJetTags_*_*', 
        'keep *_softMuonTagInfos_*_*', 
        'keep *_softMuonBJetTags_*_*', 
        'keep *_softMuonByIP3dBJetTags_*_*', 
        'keep *_softMuonByPtBJetTags_*_*', 
        'keep *_combinedMVABJetTags_*_*', 
        'keep *_pfImpactParameterTagInfos_*_*', 
        'keep *_pfSecondaryVertexTagInfos_*_*', 
        'keep *_pfCombinedSecondaryVertexBJetTags_*_*', 
        'keep *_ak4PFJetsRecoTauPiZeros_*_*', 
        'keep *_hpsPFTauProducer_*_*', 
        'keep *_hpsPFTauDiscrimination*_*_*', 
        'keep *_hpsPFTau*PtSum_*_*', 
        'keep *_hpsPFTauTransverseImpactParameters_*_*', 
        'keep  *_offlinePrimaryVertices__*', 
        'keep  *_offlinePrimaryVerticesWithBS_*_*', 
        'keep  *_offlinePrimaryVerticesFromCosmicTracks_*_*', 
        'keep  *_nuclearInteractionMaker_*_*', 
        'keep *_generalV0Candidates_*_*', 
        'keep *_inclusiveSecondaryVertices_*_*', 
        'keep recoGsfElectronCores_gsfElectronCores_*_*', 
        'keep recoGsfElectrons_gsfElectrons_*_*', 
        'keep recoGsfElectronCores_uncleanedOnlyGsfElectronCores_*_*', 
        'keep recoGsfElectrons_uncleanedOnlyGsfElectrons_*_*', 
        'keep floatedmValueMap_eidRobustLoose_*_*', 
        'keep floatedmValueMap_eidRobustTight_*_*', 
        'keep floatedmValueMap_eidRobustHighEnergy_*_*', 
        'keep floatedmValueMap_eidLoose_*_*', 
        'keep floatedmValueMap_eidTight_*_*', 
        'keep *_egmGedGsfElectronPFIsolation_*_*', 
        'keep *_egmGsfElectronIDs_*_*', 
        'keep *_gedPhotonCore_*_*', 
        'keep *_gedPhotons_*_*', 
        'keep *_particleBasedIsolation_*_*', 
        'keep recoPhotons_mustachePhotons_*_*', 
        'keep recoPhotonCores_mustachePhotonCore_*_*', 
        'keep recoPhotons_photons_*_*', 
        'keep recoPhotonCores_photonCore_*_*', 
        'keep recoConversions_conversions_*_*', 
        'keep recoConversions_mustacheConversions_*_*', 
        'drop *_conversions_uncleanedConversions_*', 
        'drop *_gedPhotonsTmp_valMapPFEgammaCandToPhoton_*', 
        'keep recoConversions_allConversions_*_*', 
        'keep recoConversions_allConversionsOldEG_*_*', 
        'keep recoTracks_ckfOutInTracksFrom*Conversions_*_*', 
        'keep recoTracks_ckfInOutTracksFrom*Conversions_*_*', 
        'keep recoTrackExtras_ckfOutInTracksFrom*Conversions_*_*', 
        'keep recoTrackExtras_ckfInOutTracksFrom*Conversions_*_*', 
        'keep TrackingRecHitsOwned_ckfOutInTracksFrom*Conversions_*_*', 
        'keep TrackingRecHitsOwned_ckfInOutTracksFrom*Conversions_*_*', 
        'keep recoConversions_uncleanedOnlyAllConversions_*_*', 
        'keep recoTracks_uncleanedOnlyCkfOutInTracksFromConversions_*_*', 
        'keep recoTracks_uncleanedOnlyCkfInOutTracksFromConversions_*_*', 
        'keep recoTrackExtras_uncleanedOnlyCkfOutInTracksFromConversions_*_*', 
        'keep recoTrackExtras_uncleanedOnlyCkfInOutTracksFromConversions_*_*', 
        'keep TrackingRecHitsOwned_uncleanedOnlyCkfOutInTracksFromConversions_*_*', 
        'keep TrackingRecHitsOwned_uncleanedOnlyCkfInOutTracksFromConversions_*_*', 
        'keep *_PhotonIDProd_*_*', 
        'keep *_PhotonIDProdGED_*_*', 
        'keep *_hfRecoEcalCandidate_*_*', 
        'keep *_hfEMClusters_*_*', 
        'keep *_gedGsfElectronCores_*_*', 
        'keep *_gedGsfElectrons_*_*', 
        'keep *_pixelTracks_*_*', 
        'keep *_pixelVertices_*_*', 
        'drop CaloTowersSorted_towerMakerPF_*_*', 
        'keep recoPFRecHits_particleFlowClusterECAL_Cleaned_*', 
        'keep recoPFRecHits_particleFlowClusterHCAL_Cleaned_*', 
        'keep recoPFRecHits_particleFlowClusterHO_Cleaned_*', 
        'keep recoPFRecHits_particleFlowClusterHFEM_Cleaned_*', 
        'keep recoPFRecHits_particleFlowClusterHFHAD_Cleaned_*', 
        'keep recoPFRecHits_particleFlowClusterPS_Cleaned_*', 
        'keep recoPFRecHits_particleFlowRecHitECAL_Cleaned_*', 
        'keep recoPFRecHits_particleFlowRecHitHCAL_Cleaned_*', 
        'keep recoPFRecHits_particleFlowRecHitHO_Cleaned_*', 
        'keep recoPFRecHits_particleFlowRecHitPS_Cleaned_*', 
        'keep recoPFClusters_particleFlowClusterECAL_*_*', 
        'keep recoPFClusters_particleFlowClusterHCAL_*_*', 
        'keep recoPFClusters_particleFlowClusterHO_*_*', 
        'keep recoPFClusters_particleFlowClusterPS_*_*', 
        'keep recoPFBlocks_particleFlowBlock_*_*', 
        'keep recoPFCandidates_particleFlowEGamma_*_*', 
        'keep recoCaloClusters_particleFlowEGamma_*_*', 
        'keep recoSuperClusters_particleFlowEGamma_*_*', 
        'keep recoConversions_particleFlowEGamma_*_*', 
        'keep recoPFCandidates_particleFlow_*_*', 
        'keep recoPFCandidates_particleFlowTmp_electrons_*', 
        'keep recoPFCandidates_particleFlowTmp_*_*', 
        'drop recoPFCandidates_particleFlowTmp__*', 
        'keep recoPFDisplacedVertexs_particleFlowDisplacedVertex_*_*', 
        'keep *_pfElectronTranslator_*_*', 
        'keep *_pfPhotonTranslator_*_*', 
        'keep *_particleFlow_electrons_*', 
        'keep *_particleFlow_photons_*', 
        'keep *_trackerDrivenElectronSeeds_preid_*', 
        'keep *_particleFlowPtrs_*_*', 
        'keep *_particleFlowTmpPtrs_*_*', 
        'keep *_offlineBeamSpot_*_*', 
        'keep L1GlobalTriggerReadoutRecord_gtDigis_*_*', 
        'keep *_l1GtRecord_*_*', 
        'keep *_l1GtTriggerMenuLite_*_*', 
        'keep *_conditionsInEdm_*_*', 
        'keep *_l1extraParticles_*_*', 
        'keep *_l1L1GtObjectMap_*_*', 
        'keep L1MuGMTReadoutCollection_gtDigis_*_*', 
        'keep L1GctEmCand*_gctDigis_*_*', 
        'keep L1GctJetCand*_gctDigis_*_*', 
        'keep L1GctEtHad*_gctDigis_*_*', 
        'keep L1GctEtMiss*_gctDigis_*_*', 
        'keep L1GctEtTotal*_gctDigis_*_*', 
        'keep L1GctHtMiss*_gctDigis_*_*', 
        'keep L1GctJetCounts*_gctDigis_*_*', 
        'keep L1GctHFRingEtSums*_gctDigis_*_*', 
        'keep L1GctHFBitCounts*_gctDigis_*_*', 
        'keep LumiDetails_lumiProducer_*_*', 
        'keep LumiSummary_lumiProducer_*_*', 
        'drop *_hlt*_*_*', 
        'keep *_hltL1GtObjectMap_*_*', 
        'keep edmTriggerResults_*_*_*', 
        'keep triggerTriggerEvent_*_*_*', 
        'keep L1AcceptBunchCrossings_*_*_*', 
        'keep L1TriggerScalerss_*_*_*', 
        'keep Level1TriggerScalerss_*_*_*', 
        'keep LumiScalerss_*_*_*', 
        'keep BeamSpotOnlines_*_*_*', 
        'keep DcsStatuss_*_*_*', 
        'keep *_logErrorHarvester_*_*', 
        'keep *_pfIsolatedElectronsEI_*_*', 
        'keep *_pfIsolatedMuonsEI_*_*', 
        'keep recoPFJets_pfJetsEI_*_*', 
        'keep *_pfJetTrackAssociatorEI_*_*', 
        'keep *_impactParameterTagInfosEI_*_*', 
        'keep *_secondaryVertexTagInfosEI_*_*', 
        'keep *_combinedSecondaryVertexBJetTagsEI_*_*', 
        'keep recoPFTaus_pfTausEI_*_*', 
        'keep recoPFTauDiscriminator_pfTausDiscrimination*_*_*', 
        'keep *_pfMetEI_*_*' ) )
)

process.FEVTHLTALLEventContent = cms.PSet(
    splitLevel = cms.untracked.int32(0),
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
    outputCommands = cms.untracked.vstring( ('drop *', 
        'drop *', 
        'drop *', 
        'keep  FEDRawDataCollection_rawDataCollector_*_*', 
        'keep  FEDRawDataCollection_source_*_*', 
        'keep  FEDRawDataCollection_rawDataCollector_*_*', 
        'keep  FEDRawDataCollection_source_*_*', 
        'drop *_hlt*_*_*', 
        'keep *_hltL1GtObjectMap_*_*', 
        'keep FEDRawDataCollection_rawDataCollector_*_*', 
        'keep FEDRawDataCollection_source_*_*', 
        'keep edmTriggerResults_*_*_*', 
        'keep triggerTriggerEvent_*_*_*', 
        'keep DetIdedmEDCollection_siStripDigis_*_*', 
        'keep DetIdedmEDCollection_siPixelDigis_*_*', 
        'keep *_siPixelClusters_*_*', 
        'keep *_siStripClusters_*_*', 
        'keep *_clusterSummaryProducer_*_*', 
        'keep *_dt1DRecHits_*_*', 
        'keep *_dt4DSegments_*_*', 
        'keep *_dt1DCosmicRecHits_*_*', 
        'keep *_dt4DCosmicSegments_*_*', 
        'keep *_csc2DRecHits_*_*', 
        'keep *_cscSegments_*_*', 
        'keep *_rpcRecHits_*_*', 
        'keep *_hbhereco_*_*', 
        'keep *_hbheprereco_*_*', 
        'keep *_hfreco_*_*', 
        'keep *_horeco_*_*', 
        'keep HBHERecHitsSorted_hbherecoMB_*_*', 
        'keep HORecHitsSorted_horecoMB_*_*', 
        'keep HFRecHitsSorted_hfrecoMB_*_*', 
        'keep ZDCDataFramesSorted_*Digis_*_*', 
        'keep ZDCRecHitsSorted_*_*_*', 
        'keep *_reducedHcalRecHits_*_*', 
        'keep *_castorreco_*_*', 
        'keep HcalUnpackerReport_*_*_*', 
        'keep *_ecalPreshowerRecHit_*_*', 
        'keep *_ecalRecHit_*_*', 
        'keep *_ecalCompactTrigPrim_*_*', 
        'keep *_ecalTPSkim_*_*', 
        'keep *_selectDigi_*_*', 
        'keep EcalRecHitsSorted_reducedEcalRecHitsEE_*_*', 
        'keep EcalRecHitsSorted_reducedEcalRecHitsEB_*_*', 
        'keep EcalRecHitsSorted_reducedEcalRecHitsES_*_*', 
        'keep *_hybridSuperClusters_*_*', 
        'keep recoSuperClusters_correctedHybridSuperClusters_*_*', 
        'keep *_multi5x5SuperClusters_*_*', 
        'keep recoSuperClusters_multi5x5SuperClusters_*_*', 
        'keep recoSuperClusters_multi5x5SuperClustersWithPreshower_*_*', 
        'keep recoSuperClusters_correctedMulti5x5SuperClustersWithPreshower_*_*', 
        'keep recoPreshowerClusters_multi5x5SuperClustersWithPreshower_*_*', 
        'keep recoPreshowerClusterShapes_multi5x5PreshowerClusterShape_*_*', 
        'keep *_particleFlowSuperClusterECAL_*_*', 
        'drop recoClusterShapes_*_*_*', 
        'drop recoBasicClustersToOnerecoClusterShapesAssociation_*_*_*', 
        'drop recoBasicClusters_multi5x5BasicClusters_multi5x5BarrelBasicClusters_*', 
        'drop recoSuperClusters_multi5x5SuperClusters_multi5x5BarrelSuperClusters_*', 
        'keep *_CkfElectronCandidates_*_*', 
        'keep *_GsfGlobalElectronTest_*_*', 
        'keep *_electronMergedSeeds_*_*', 
        'keep recoGsfTracks_electronGsfTracks_*_*', 
        'keep recoGsfTrackExtras_electronGsfTracks_*_*', 
        'keep recoTrackExtras_electronGsfTracks_*_*', 
        'keep TrackingRecHitsOwned_electronGsfTracks_*_*', 
        'keep recoTracks_generalTracks_*_*', 
        'keep recoTrackExtras_generalTracks_*_*', 
        'keep TrackingRecHitsOwned_extraFromSeeds_*_*', 
        'keep uints_extraFromSeeds_*_*', 
        'keep TrackingRecHitsOwned_generalTracks_*_*', 
        'keep recoTracks_beamhaloTracks_*_*', 
        'keep recoTrackExtras_beamhaloTracks_*_*', 
        'keep TrackingRecHitsOwned_beamhaloTracks_*_*', 
        'keep recoTracks_regionalCosmicTracks_*_*', 
        'keep recoTrackExtras_regionalCosmicTracks_*_*', 
        'keep TrackingRecHitsOwned_regionalCosmicTracks_*_*', 
        'keep recoTracks_rsWithMaterialTracks_*_*', 
        'keep recoTrackExtras_rsWithMaterialTracks_*_*', 
        'keep TrackingRecHitsOwned_rsWithMaterialTracks_*_*', 
        'keep recoTracks_conversionStepTracks_*_*', 
        'keep recoTrackExtras_conversionStepTracks_*_*', 
        'keep TrackingRecHitsOwned_conversionStepTracks_*_*', 
        'keep *_ctfPixelLess_*_*', 
        'keep *_dedxTruncated40_*_*', 
        'keep *_dedxDiscrimASmi_*_*', 
        'keep *_dedxHarmonic2_*_*', 
        'keep *_trackExtrapolator_*_*', 
        'keep floatedmValueMap_generalTracks_*_*', 
        'keep *_kt6CaloJets_*_*', 
        'keep *_ak4CaloJets_*_*', 
        'keep *_ak5CaloJets_*_*', 
        'keep double_kt6PFJets_rho_*', 
        'keep *_ak4PFJets_*_*', 
        'keep *_ak5PFJets_*_*', 
        'keep *_ak8PFJets_*_*', 
        'keep *_ak4PFJetsCHS_*_*', 
        'keep *_ak5PFJetsCHS_*_*', 
        'keep *_ak8PFJetsCHS_*_*', 
        'keep *_ca8PFJetsCHS_*_*', 
        'keep *_ca8PFJetsCHSPruned_*_*', 
        'keep *_cmsTopTagPFJetsCHS_*_*', 
        'keep *_JetPlusTrackZSPCorJetAntiKt4_*_*', 
        'keep *_ak4TrackJets_*_*', 
        'keep recoRecoChargedRefCandidates_trackRefsForJets_*_*', 
        'keep *_caloTowers_*_*', 
        'keep *_towerMaker_*_*', 
        'keep *_CastorTowerReco_*_*', 
        'keep *_kt4JetTracksAssociatorAtVertex_*_*', 
        'keep *_kt4JetTracksAssociatorAtCaloFace_*_*', 
        'keep *_kt4JetExtender_*_*', 
        'keep *_ak4JetTracksAssociatorAtVertex_*_*', 
        'keep *_ak4JetTracksAssociatorAtVertexPF_*_*', 
        'keep *_ak4JetTracksAssociatorAtCaloFace_*_*', 
        'keep *_ak4JetTracksAssociatorExplicit_*_*', 
        'keep *_ak4JetExtender_*_*', 
        'keep *_ak7JetTracksAssociatorAtVertex*_*_*', 
        'keep *_ak7JetTracksAssociatorAtCaloFace*_*_*', 
        'keep *_ak7JetExtender_*_*', 
        'keep *_ak4JetID_*_*', 
        'keep *_ak5JetID_*_*', 
        'keep *_ak7JetID_*_*', 
        'keep *_ic5JetID_*_*', 
        'keep *_kt4JetID_*_*', 
        'keep *_kt6JetID_*_*', 
        'keep *_ak7BasicJets_*_*', 
        'keep *_ak7CastorJetID_*_*', 
        'keep double_kt6CaloJetsCentral_*_*', 
        'keep double_kt6PFJetsCentralChargedPileUp_*_*', 
        'keep double_kt6PFJetsCentralNeutral_*_*', 
        'keep double_kt6PFJetsCentralNeutralTight_*_*', 
        'keep *_fixedGridRho*_*_*', 
        'keep *_ca8PFJetsCHSPrunedLinks_*_*', 
        'keep recoCaloMETs_caloMet_*_*', 
        'keep recoCaloMETs_caloMetBE_*_*', 
        'keep recoCaloMETs_caloMetBEFO_*_*', 
        'keep recoCaloMETs_caloMetM_*_*', 
        'keep recoPFMETs_pfMet_*_*', 
        'keep recoPFMETs_pfChMet_*_*', 
        'keep recoMuonMETCorrectionDataedmValueMap_muonMETValueMapProducer_*_*', 
        'keep recoHcalNoiseRBXs_hcalnoise_*_*', 
        'keep HcalNoiseSummary_hcalnoise_*_*', 
        'keep *HaloData_*_*_*', 
        'keep *BeamHaloSummary_BeamHaloSummary_*_*', 
        'keep *_MuonSeed_*_*', 
        'keep *_ancientMuonSeed_*_*', 
        'keep *_displacedMuonSeeds_*_*', 
        'keep *_mergedStandAloneMuonSeeds_*_*', 
        'keep TrackingRecHitsOwned_globalMuons_*_*', 
        'keep TrackingRecHitsOwned_tevMuons_*_*', 
        'keep recoCaloMuons_calomuons_*_*', 
        'keep *_CosmicMuonSeed_*_*', 
        'keep recoTrackExtras_cosmicMuons_*_*', 
        'keep TrackingRecHitsOwned_cosmicMuons_*_*', 
        'keep recoTrackExtras_globalCosmicMuons_*_*', 
        'keep TrackingRecHitsOwned_globalCosmicMuons_*_*', 
        'keep recoTrackExtras_cosmicMuons1Leg_*_*', 
        'keep TrackingRecHitsOwned_cosmicMuons1Leg_*_*', 
        'keep recoTrackExtras_globalCosmicMuons1Leg_*_*', 
        'keep TrackingRecHitsOwned_globalCosmicMuons1Leg_*_*', 
        'keep recoTracks_cosmicsVetoTracks_*_*', 
        'keep *_SETMuonSeed_*_*', 
        'keep recoTracks_standAloneSETMuons_*_*', 
        'keep recoTrackExtras_standAloneSETMuons_*_*', 
        'keep TrackingRecHitsOwned_standAloneSETMuons_*_*', 
        'keep recoTracks_globalSETMuons_*_*', 
        'keep recoTrackExtras_globalSETMuons_*_*', 
        'keep TrackingRecHitsOwned_globalSETMuons_*_*', 
        'keep recoMuons_muonsWithSET_*_*', 
        'keep *_muons_*_*', 
        'keep *_*_muons_*', 
        'drop *_muons_muons1stStep2muonsMap_*', 
        'drop recoIsoDepositedmValueMap_muons_*_*', 
        'drop doubleedmValueMap_muons_muPFIso*_*', 
        'keep recoTracks_standAloneMuons_*_*', 
        'keep recoTrackExtras_standAloneMuons_*_*', 
        'keep TrackingRecHitsOwned_standAloneMuons_*_*', 
        'keep recoTracks_globalMuons_*_*', 
        'keep recoTrackExtras_globalMuons_*_*', 
        'keep recoTracks_tevMuons_*_*', 
        'keep recoTrackExtras_tevMuons_*_*', 
        'keep recoTracks_generalTracks_*_*', 
        'keep recoTracksToOnerecoTracksAssociation_tevMuons_*_*', 
        'keep recoTracks_cosmicMuons_*_*', 
        'keep recoTracks_globalCosmicMuons_*_*', 
        'keep recoMuons_muonsFromCosmics_*_*', 
        'keep recoTracks_cosmicMuons1Leg_*_*', 
        'keep recoTracks_globalCosmicMuons1Leg_*_*', 
        'keep recoMuons_muonsFromCosmics1Leg_*_*', 
        'keep recoTracks_refittedStandAloneMuons_*_*', 
        'keep recoTrackExtras_refittedStandAloneMuons_*_*', 
        'keep TrackingRecHitsOwned_refittedStandAloneMuons_*_*', 
        'keep recoTracks_displacedStandAloneMuons__*', 
        'keep recoTrackExtras_displacedStandAloneMuons_*_*', 
        'keep TrackingRecHitsOwned_displacedStandAloneMuons_*_*', 
        'keep *_muIsoDepositTk_*_*', 
        'keep *_muIsoDepositCalByAssociatorTowers_*_*', 
        'keep *_muIsoDepositCalByAssociatorHits_*_*', 
        'keep *_muIsoDepositJets_*_*', 
        'keep *_muGlobalIsoDepositCtfTk_*_*', 
        'keep *_muGlobalIsoDepositCalByAssociatorTowers_*_*', 
        'keep *_muGlobalIsoDepositCalByAssociatorHits_*_*', 
        'keep *_muGlobalIsoDepositJets_*_*', 
        'keep *_impactParameterTagInfos_*_*', 
        'keep *_trackCountingHighEffBJetTags_*_*', 
        'keep *_trackCountingHighPurBJetTags_*_*', 
        'keep *_jetProbabilityBJetTags_*_*', 
        'keep *_jetBProbabilityBJetTags_*_*', 
        'keep *_secondaryVertexTagInfos_*_*', 
        'keep *_inclusiveSecondaryVertexFinderTagInfos_*_*', 
        'keep *_ghostTrackVertexTagInfos_*_*', 
        'keep *_simpleSecondaryVertexBJetTags_*_*', 
        'keep *_simpleSecondaryVertexHighEffBJetTags_*_*', 
        'keep *_simpleSecondaryVertexHighPurBJetTags_*_*', 
        'keep *_combinedSecondaryVertexBJetTags_*_*', 
        'keep *_combinedInclusiveSecondaryVertexV2BJetTags_*_*', 
        'keep *_ghostTrackBJetTags_*_*', 
        'keep *_softPFMuonsTagInfos_*_*', 
        'keep *_softPFElectronsTagInfos_*_*', 
        'keep *_softPFElectronBJetTags_*_*', 
        'keep *_softPFMuonBJetTags_*_*', 
        'keep *_softMuonTagInfos_*_*', 
        'keep *_softMuonBJetTags_*_*', 
        'keep *_softMuonByIP3dBJetTags_*_*', 
        'keep *_softMuonByPtBJetTags_*_*', 
        'keep *_combinedMVABJetTags_*_*', 
        'keep *_pfImpactParameterTagInfos_*_*', 
        'keep *_pfSecondaryVertexTagInfos_*_*', 
        'keep *_pfCombinedSecondaryVertexBJetTags_*_*', 
        'keep *_ak4PFJetsRecoTauPiZeros_*_*', 
        'keep *_hpsPFTauProducer_*_*', 
        'keep *_hpsPFTauDiscrimination*_*_*', 
        'keep *_hpsPFTau*PtSum_*_*', 
        'keep *_hpsPFTauTransverseImpactParameters_*_*', 
        'keep  *_offlinePrimaryVertices__*', 
        'keep  *_offlinePrimaryVerticesWithBS_*_*', 
        'keep  *_offlinePrimaryVerticesFromCosmicTracks_*_*', 
        'keep  *_nuclearInteractionMaker_*_*', 
        'keep *_generalV0Candidates_*_*', 
        'keep *_inclusiveSecondaryVertices_*_*', 
        'keep recoGsfElectronCores_gsfElectronCores_*_*', 
        'keep recoGsfElectrons_gsfElectrons_*_*', 
        'keep recoGsfElectronCores_uncleanedOnlyGsfElectronCores_*_*', 
        'keep recoGsfElectrons_uncleanedOnlyGsfElectrons_*_*', 
        'keep floatedmValueMap_eidRobustLoose_*_*', 
        'keep floatedmValueMap_eidRobustTight_*_*', 
        'keep floatedmValueMap_eidRobustHighEnergy_*_*', 
        'keep floatedmValueMap_eidLoose_*_*', 
        'keep floatedmValueMap_eidTight_*_*', 
        'keep *_egmGedGsfElectronPFIsolation_*_*', 
        'keep *_egmGsfElectronIDs_*_*', 
        'keep *_gedPhotonCore_*_*', 
        'keep *_gedPhotons_*_*', 
        'keep *_particleBasedIsolation_*_*', 
        'keep recoPhotons_mustachePhotons_*_*', 
        'keep recoPhotonCores_mustachePhotonCore_*_*', 
        'keep recoPhotons_photons_*_*', 
        'keep recoPhotonCores_photonCore_*_*', 
        'keep recoConversions_conversions_*_*', 
        'keep recoConversions_mustacheConversions_*_*', 
        'drop *_conversions_uncleanedConversions_*', 
        'drop *_gedPhotonsTmp_valMapPFEgammaCandToPhoton_*', 
        'keep recoConversions_allConversions_*_*', 
        'keep recoConversions_allConversionsOldEG_*_*', 
        'keep recoTracks_ckfOutInTracksFrom*Conversions_*_*', 
        'keep recoTracks_ckfInOutTracksFrom*Conversions_*_*', 
        'keep recoTrackExtras_ckfOutInTracksFrom*Conversions_*_*', 
        'keep recoTrackExtras_ckfInOutTracksFrom*Conversions_*_*', 
        'keep TrackingRecHitsOwned_ckfOutInTracksFrom*Conversions_*_*', 
        'keep TrackingRecHitsOwned_ckfInOutTracksFrom*Conversions_*_*', 
        'keep recoConversions_uncleanedOnlyAllConversions_*_*', 
        'keep recoTracks_uncleanedOnlyCkfOutInTracksFromConversions_*_*', 
        'keep recoTracks_uncleanedOnlyCkfInOutTracksFromConversions_*_*', 
        'keep recoTrackExtras_uncleanedOnlyCkfOutInTracksFromConversions_*_*', 
        'keep recoTrackExtras_uncleanedOnlyCkfInOutTracksFromConversions_*_*', 
        'keep TrackingRecHitsOwned_uncleanedOnlyCkfOutInTracksFromConversions_*_*', 
        'keep TrackingRecHitsOwned_uncleanedOnlyCkfInOutTracksFromConversions_*_*', 
        'keep *_PhotonIDProd_*_*', 
        'keep *_PhotonIDProdGED_*_*', 
        'keep *_hfRecoEcalCandidate_*_*', 
        'keep *_hfEMClusters_*_*', 
        'keep *_gedGsfElectronCores_*_*', 
        'keep *_gedGsfElectrons_*_*', 
        'keep *_pixelTracks_*_*', 
        'keep *_pixelVertices_*_*', 
        'drop CaloTowersSorted_towerMakerPF_*_*', 
        'keep recoPFRecHits_particleFlowClusterECAL_Cleaned_*', 
        'keep recoPFRecHits_particleFlowClusterHCAL_Cleaned_*', 
        'keep recoPFRecHits_particleFlowClusterHO_Cleaned_*', 
        'keep recoPFRecHits_particleFlowClusterHFEM_Cleaned_*', 
        'keep recoPFRecHits_particleFlowClusterHFHAD_Cleaned_*', 
        'keep recoPFRecHits_particleFlowClusterPS_Cleaned_*', 
        'keep recoPFRecHits_particleFlowRecHitECAL_Cleaned_*', 
        'keep recoPFRecHits_particleFlowRecHitHCAL_Cleaned_*', 
        'keep recoPFRecHits_particleFlowRecHitHO_Cleaned_*', 
        'keep recoPFRecHits_particleFlowRecHitPS_Cleaned_*', 
        'keep recoPFClusters_particleFlowClusterECAL_*_*', 
        'keep recoPFClusters_particleFlowClusterHCAL_*_*', 
        'keep recoPFClusters_particleFlowClusterHO_*_*', 
        'keep recoPFClusters_particleFlowClusterPS_*_*', 
        'keep recoPFBlocks_particleFlowBlock_*_*', 
        'keep recoPFCandidates_particleFlowEGamma_*_*', 
        'keep recoCaloClusters_particleFlowEGamma_*_*', 
        'keep recoSuperClusters_particleFlowEGamma_*_*', 
        'keep recoConversions_particleFlowEGamma_*_*', 
        'keep recoPFCandidates_particleFlow_*_*', 
        'keep recoPFCandidates_particleFlowTmp_electrons_*', 
        'keep recoPFCandidates_particleFlowTmp_*_*', 
        'drop recoPFCandidates_particleFlowTmp__*', 
        'keep recoPFDisplacedVertexs_particleFlowDisplacedVertex_*_*', 
        'keep *_pfElectronTranslator_*_*', 
        'keep *_pfPhotonTranslator_*_*', 
        'keep *_particleFlow_electrons_*', 
        'keep *_particleFlow_photons_*', 
        'keep *_trackerDrivenElectronSeeds_preid_*', 
        'keep *_particleFlowPtrs_*_*', 
        'keep *_particleFlowTmpPtrs_*_*', 
        'keep *_offlineBeamSpot_*_*', 
        'keep L1GlobalTriggerReadoutRecord_gtDigis_*_*', 
        'keep *_l1GtRecord_*_*', 
        'keep *_l1GtTriggerMenuLite_*_*', 
        'keep *_conditionsInEdm_*_*', 
        'keep *_l1extraParticles_*_*', 
        'keep *_l1L1GtObjectMap_*_*', 
        'keep L1MuGMTReadoutCollection_gtDigis_*_*', 
        'keep L1GctEmCand*_gctDigis_*_*', 
        'keep L1GctJetCand*_gctDigis_*_*', 
        'keep L1GctEtHad*_gctDigis_*_*', 
        'keep L1GctEtMiss*_gctDigis_*_*', 
        'keep L1GctEtTotal*_gctDigis_*_*', 
        'keep L1GctHtMiss*_gctDigis_*_*', 
        'keep L1GctJetCounts*_gctDigis_*_*', 
        'keep L1GctHFRingEtSums*_gctDigis_*_*', 
        'keep L1GctHFBitCounts*_gctDigis_*_*', 
        'keep LumiDetails_lumiProducer_*_*', 
        'keep LumiSummary_lumiProducer_*_*', 
        'drop *_hlt*_*_*', 
        'keep *_hltL1GtObjectMap_*_*', 
        'keep edmTriggerResults_*_*_*', 
        'keep triggerTriggerEvent_*_*_*', 
        'keep L1AcceptBunchCrossings_*_*_*', 
        'keep L1TriggerScalerss_*_*_*', 
        'keep Level1TriggerScalerss_*_*_*', 
        'keep LumiScalerss_*_*_*', 
        'keep BeamSpotOnlines_*_*_*', 
        'keep DcsStatuss_*_*_*', 
        'keep *_logErrorHarvester_*_*', 
        'keep *_pfIsolatedElectronsEI_*_*', 
        'keep *_pfIsolatedMuonsEI_*_*', 
        'keep recoPFJets_pfJetsEI_*_*', 
        'keep *_pfJetTrackAssociatorEI_*_*', 
        'keep *_impactParameterTagInfosEI_*_*', 
        'keep *_secondaryVertexTagInfosEI_*_*', 
        'keep *_combinedSecondaryVertexBJetTagsEI_*_*', 
        'keep recoPFTaus_pfTausEI_*_*', 
        'keep recoPFTauDiscriminator_pfTausDiscrimination*_*_*', 
        'keep *_pfMetEI_*_*', 
        'keep *_*_*_HLT' ) )
)

process.FEVTSIMEventContent = cms.PSet(
    splitLevel = cms.untracked.int32(0),
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
    outputCommands = cms.untracked.vstring( ('drop *', 
        'drop *', 
        'keep  FEDRawDataCollection_rawDataCollector_*_*', 
        'keep  FEDRawDataCollection_source_*_*', 
        'keep  FEDRawDataCollection_rawDataCollector_*_*', 
        'keep  FEDRawDataCollection_source_*_*', 
        'drop *_hlt*_*_*', 
        'keep *_hltL1GtObjectMap_*_*', 
        'keep FEDRawDataCollection_rawDataCollector_*_*', 
        'keep FEDRawDataCollection_source_*_*', 
        'keep edmTriggerResults_*_*_*', 
        'keep triggerTriggerEvent_*_*_*', 
        'keep *_g4SimHits_*_*', 
        'keep edmHepMCProduct_source_*_*', 
        'keep *_allTrackMCMatch_*_*', 
        'keep StripDigiSimLinkedmDetSetVector_simMuonCSCDigis_*_*', 
        'keep CSCDetIdCSCComparatorDigiMuonDigiCollection_simMuonCSCDigis_*_*', 
        'keep DTLayerIdDTDigiSimLinkMuonDigiCollection_simMuonDTDigis_*_*', 
        'keep RPCDigiSimLinkedmDetSetVector_simMuonRPCDigis_*_*', 
        'keep EBSrFlagsSorted_simEcalDigis_*_*', 
        'keep EESrFlagsSorted_simEcalDigis_*_*', 
        'keep CrossingFramePlaybackInfoExtended_*_*_*', 
        'keep PileupSummaryInfos_*_*_*', 
        'keep LHERunInfoProduct_*_*_*', 
        'keep LHEEventProduct_*_*_*', 
        'keep GenRunInfoProduct_generator_*_*', 
        'keep GenLumiInfoProduct_generator_*_*', 
        'keep GenEventInfoProduct_generator_*_*', 
        'keep edmHepMCProduct_generator_*_*', 
        'keep GenFilterInfo_*_*_*', 
        'keep *_genParticles_*_*', 
        'keep recoGenJets_*_*_*', 
        'keep *_genParticle_*_*', 
        'keep recoGenMETs_*_*_*', 
        'keep FEDRawDataCollection_source_*_*', 
        'keep FEDRawDataCollection_rawDataCollector_*_*', 
        'keep *_MEtoEDMConverter_*_*', 
        'keep *_randomEngineStateProducer_*_*', 
        'keep DetIdedmEDCollection_siStripDigis_*_*', 
        'keep DetIdedmEDCollection_siPixelDigis_*_*', 
        'keep *_siPixelClusters_*_*', 
        'keep *_siStripClusters_*_*', 
        'keep *_clusterSummaryProducer_*_*', 
        'keep *_dt1DRecHits_*_*', 
        'keep *_dt4DSegments_*_*', 
        'keep *_dt1DCosmicRecHits_*_*', 
        'keep *_dt4DCosmicSegments_*_*', 
        'keep *_csc2DRecHits_*_*', 
        'keep *_cscSegments_*_*', 
        'keep *_rpcRecHits_*_*', 
        'keep *_hbhereco_*_*', 
        'keep *_hbheprereco_*_*', 
        'keep *_hfreco_*_*', 
        'keep *_horeco_*_*', 
        'keep HBHERecHitsSorted_hbherecoMB_*_*', 
        'keep HORecHitsSorted_horecoMB_*_*', 
        'keep HFRecHitsSorted_hfrecoMB_*_*', 
        'keep ZDCDataFramesSorted_*Digis_*_*', 
        'keep ZDCRecHitsSorted_*_*_*', 
        'keep *_reducedHcalRecHits_*_*', 
        'keep *_castorreco_*_*', 
        'keep HcalUnpackerReport_*_*_*', 
        'keep *_ecalPreshowerRecHit_*_*', 
        'keep *_ecalRecHit_*_*', 
        'keep *_ecalCompactTrigPrim_*_*', 
        'keep *_ecalTPSkim_*_*', 
        'keep *_selectDigi_*_*', 
        'keep EcalRecHitsSorted_reducedEcalRecHitsEE_*_*', 
        'keep EcalRecHitsSorted_reducedEcalRecHitsEB_*_*', 
        'keep EcalRecHitsSorted_reducedEcalRecHitsES_*_*', 
        'keep *_hybridSuperClusters_*_*', 
        'keep recoSuperClusters_correctedHybridSuperClusters_*_*', 
        'keep *_multi5x5SuperClusters_*_*', 
        'keep recoSuperClusters_multi5x5SuperClusters_*_*', 
        'keep recoSuperClusters_multi5x5SuperClustersWithPreshower_*_*', 
        'keep recoSuperClusters_correctedMulti5x5SuperClustersWithPreshower_*_*', 
        'keep recoPreshowerClusters_multi5x5SuperClustersWithPreshower_*_*', 
        'keep recoPreshowerClusterShapes_multi5x5PreshowerClusterShape_*_*', 
        'keep *_particleFlowSuperClusterECAL_*_*', 
        'drop recoClusterShapes_*_*_*', 
        'drop recoBasicClustersToOnerecoClusterShapesAssociation_*_*_*', 
        'drop recoBasicClusters_multi5x5BasicClusters_multi5x5BarrelBasicClusters_*', 
        'drop recoSuperClusters_multi5x5SuperClusters_multi5x5BarrelSuperClusters_*', 
        'keep *_CkfElectronCandidates_*_*', 
        'keep *_GsfGlobalElectronTest_*_*', 
        'keep *_electronMergedSeeds_*_*', 
        'keep recoGsfTracks_electronGsfTracks_*_*', 
        'keep recoGsfTrackExtras_electronGsfTracks_*_*', 
        'keep recoTrackExtras_electronGsfTracks_*_*', 
        'keep TrackingRecHitsOwned_electronGsfTracks_*_*', 
        'keep recoTracks_generalTracks_*_*', 
        'keep recoTrackExtras_generalTracks_*_*', 
        'keep TrackingRecHitsOwned_extraFromSeeds_*_*', 
        'keep uints_extraFromSeeds_*_*', 
        'keep TrackingRecHitsOwned_generalTracks_*_*', 
        'keep recoTracks_beamhaloTracks_*_*', 
        'keep recoTrackExtras_beamhaloTracks_*_*', 
        'keep TrackingRecHitsOwned_beamhaloTracks_*_*', 
        'keep recoTracks_regionalCosmicTracks_*_*', 
        'keep recoTrackExtras_regionalCosmicTracks_*_*', 
        'keep TrackingRecHitsOwned_regionalCosmicTracks_*_*', 
        'keep recoTracks_rsWithMaterialTracks_*_*', 
        'keep recoTrackExtras_rsWithMaterialTracks_*_*', 
        'keep TrackingRecHitsOwned_rsWithMaterialTracks_*_*', 
        'keep recoTracks_conversionStepTracks_*_*', 
        'keep recoTrackExtras_conversionStepTracks_*_*', 
        'keep TrackingRecHitsOwned_conversionStepTracks_*_*', 
        'keep *_ctfPixelLess_*_*', 
        'keep *_dedxTruncated40_*_*', 
        'keep *_dedxDiscrimASmi_*_*', 
        'keep *_dedxHarmonic2_*_*', 
        'keep *_trackExtrapolator_*_*', 
        'keep floatedmValueMap_generalTracks_*_*', 
        'keep *_kt6CaloJets_*_*', 
        'keep *_ak4CaloJets_*_*', 
        'keep *_ak5CaloJets_*_*', 
        'keep double_kt6PFJets_rho_*', 
        'keep *_ak4PFJets_*_*', 
        'keep *_ak5PFJets_*_*', 
        'keep *_ak8PFJets_*_*', 
        'keep *_ak4PFJetsCHS_*_*', 
        'keep *_ak5PFJetsCHS_*_*', 
        'keep *_ak8PFJetsCHS_*_*', 
        'keep *_ca8PFJetsCHS_*_*', 
        'keep *_ca8PFJetsCHSPruned_*_*', 
        'keep *_cmsTopTagPFJetsCHS_*_*', 
        'keep *_JetPlusTrackZSPCorJetAntiKt4_*_*', 
        'keep *_ak4TrackJets_*_*', 
        'keep recoRecoChargedRefCandidates_trackRefsForJets_*_*', 
        'keep *_caloTowers_*_*', 
        'keep *_towerMaker_*_*', 
        'keep *_CastorTowerReco_*_*', 
        'keep *_kt4JetTracksAssociatorAtVertex_*_*', 
        'keep *_kt4JetTracksAssociatorAtCaloFace_*_*', 
        'keep *_kt4JetExtender_*_*', 
        'keep *_ak4JetTracksAssociatorAtVertex_*_*', 
        'keep *_ak4JetTracksAssociatorAtVertexPF_*_*', 
        'keep *_ak4JetTracksAssociatorAtCaloFace_*_*', 
        'keep *_ak4JetTracksAssociatorExplicit_*_*', 
        'keep *_ak4JetExtender_*_*', 
        'keep *_ak7JetTracksAssociatorAtVertex*_*_*', 
        'keep *_ak7JetTracksAssociatorAtCaloFace*_*_*', 
        'keep *_ak7JetExtender_*_*', 
        'keep *_ak4JetID_*_*', 
        'keep *_ak5JetID_*_*', 
        'keep *_ak7JetID_*_*', 
        'keep *_ic5JetID_*_*', 
        'keep *_kt4JetID_*_*', 
        'keep *_kt6JetID_*_*', 
        'keep *_ak7BasicJets_*_*', 
        'keep *_ak7CastorJetID_*_*', 
        'keep double_kt6CaloJetsCentral_*_*', 
        'keep double_kt6PFJetsCentralChargedPileUp_*_*', 
        'keep double_kt6PFJetsCentralNeutral_*_*', 
        'keep double_kt6PFJetsCentralNeutralTight_*_*', 
        'keep *_fixedGridRho*_*_*', 
        'keep *_ca8PFJetsCHSPrunedLinks_*_*', 
        'keep recoCaloMETs_caloMet_*_*', 
        'keep recoCaloMETs_caloMetBE_*_*', 
        'keep recoCaloMETs_caloMetBEFO_*_*', 
        'keep recoCaloMETs_caloMetM_*_*', 
        'keep recoPFMETs_pfMet_*_*', 
        'keep recoPFMETs_pfChMet_*_*', 
        'keep recoMuonMETCorrectionDataedmValueMap_muonMETValueMapProducer_*_*', 
        'keep recoHcalNoiseRBXs_hcalnoise_*_*', 
        'keep HcalNoiseSummary_hcalnoise_*_*', 
        'keep *HaloData_*_*_*', 
        'keep *BeamHaloSummary_BeamHaloSummary_*_*', 
        'keep *_MuonSeed_*_*', 
        'keep *_ancientMuonSeed_*_*', 
        'keep *_displacedMuonSeeds_*_*', 
        'keep *_mergedStandAloneMuonSeeds_*_*', 
        'keep TrackingRecHitsOwned_globalMuons_*_*', 
        'keep TrackingRecHitsOwned_tevMuons_*_*', 
        'keep recoCaloMuons_calomuons_*_*', 
        'keep *_CosmicMuonSeed_*_*', 
        'keep recoTrackExtras_cosmicMuons_*_*', 
        'keep TrackingRecHitsOwned_cosmicMuons_*_*', 
        'keep recoTrackExtras_globalCosmicMuons_*_*', 
        'keep TrackingRecHitsOwned_globalCosmicMuons_*_*', 
        'keep recoTrackExtras_cosmicMuons1Leg_*_*', 
        'keep TrackingRecHitsOwned_cosmicMuons1Leg_*_*', 
        'keep recoTrackExtras_globalCosmicMuons1Leg_*_*', 
        'keep TrackingRecHitsOwned_globalCosmicMuons1Leg_*_*', 
        'keep recoTracks_cosmicsVetoTracks_*_*', 
        'keep *_SETMuonSeed_*_*', 
        'keep recoTracks_standAloneSETMuons_*_*', 
        'keep recoTrackExtras_standAloneSETMuons_*_*', 
        'keep TrackingRecHitsOwned_standAloneSETMuons_*_*', 
        'keep recoTracks_globalSETMuons_*_*', 
        'keep recoTrackExtras_globalSETMuons_*_*', 
        'keep TrackingRecHitsOwned_globalSETMuons_*_*', 
        'keep recoMuons_muonsWithSET_*_*', 
        'keep *_muons_*_*', 
        'keep *_*_muons_*', 
        'drop *_muons_muons1stStep2muonsMap_*', 
        'drop recoIsoDepositedmValueMap_muons_*_*', 
        'drop doubleedmValueMap_muons_muPFIso*_*', 
        'keep recoTracks_standAloneMuons_*_*', 
        'keep recoTrackExtras_standAloneMuons_*_*', 
        'keep TrackingRecHitsOwned_standAloneMuons_*_*', 
        'keep recoTracks_globalMuons_*_*', 
        'keep recoTrackExtras_globalMuons_*_*', 
        'keep recoTracks_tevMuons_*_*', 
        'keep recoTrackExtras_tevMuons_*_*', 
        'keep recoTracks_generalTracks_*_*', 
        'keep recoTracksToOnerecoTracksAssociation_tevMuons_*_*', 
        'keep recoTracks_cosmicMuons_*_*', 
        'keep recoTracks_globalCosmicMuons_*_*', 
        'keep recoMuons_muonsFromCosmics_*_*', 
        'keep recoTracks_cosmicMuons1Leg_*_*', 
        'keep recoTracks_globalCosmicMuons1Leg_*_*', 
        'keep recoMuons_muonsFromCosmics1Leg_*_*', 
        'keep recoTracks_refittedStandAloneMuons_*_*', 
        'keep recoTrackExtras_refittedStandAloneMuons_*_*', 
        'keep TrackingRecHitsOwned_refittedStandAloneMuons_*_*', 
        'keep recoTracks_displacedStandAloneMuons__*', 
        'keep recoTrackExtras_displacedStandAloneMuons_*_*', 
        'keep TrackingRecHitsOwned_displacedStandAloneMuons_*_*', 
        'keep *_muIsoDepositTk_*_*', 
        'keep *_muIsoDepositCalByAssociatorTowers_*_*', 
        'keep *_muIsoDepositCalByAssociatorHits_*_*', 
        'keep *_muIsoDepositJets_*_*', 
        'keep *_muGlobalIsoDepositCtfTk_*_*', 
        'keep *_muGlobalIsoDepositCalByAssociatorTowers_*_*', 
        'keep *_muGlobalIsoDepositCalByAssociatorHits_*_*', 
        'keep *_muGlobalIsoDepositJets_*_*', 
        'keep *_impactParameterTagInfos_*_*', 
        'keep *_trackCountingHighEffBJetTags_*_*', 
        'keep *_trackCountingHighPurBJetTags_*_*', 
        'keep *_jetProbabilityBJetTags_*_*', 
        'keep *_jetBProbabilityBJetTags_*_*', 
        'keep *_secondaryVertexTagInfos_*_*', 
        'keep *_inclusiveSecondaryVertexFinderTagInfos_*_*', 
        'keep *_ghostTrackVertexTagInfos_*_*', 
        'keep *_simpleSecondaryVertexBJetTags_*_*', 
        'keep *_simpleSecondaryVertexHighEffBJetTags_*_*', 
        'keep *_simpleSecondaryVertexHighPurBJetTags_*_*', 
        'keep *_combinedSecondaryVertexBJetTags_*_*', 
        'keep *_combinedInclusiveSecondaryVertexV2BJetTags_*_*', 
        'keep *_ghostTrackBJetTags_*_*', 
        'keep *_softPFMuonsTagInfos_*_*', 
        'keep *_softPFElectronsTagInfos_*_*', 
        'keep *_softPFElectronBJetTags_*_*', 
        'keep *_softPFMuonBJetTags_*_*', 
        'keep *_softMuonTagInfos_*_*', 
        'keep *_softMuonBJetTags_*_*', 
        'keep *_softMuonByIP3dBJetTags_*_*', 
        'keep *_softMuonByPtBJetTags_*_*', 
        'keep *_combinedMVABJetTags_*_*', 
        'keep *_pfImpactParameterTagInfos_*_*', 
        'keep *_pfSecondaryVertexTagInfos_*_*', 
        'keep *_pfCombinedSecondaryVertexBJetTags_*_*', 
        'keep *_ak4PFJetsRecoTauPiZeros_*_*', 
        'keep *_hpsPFTauProducer_*_*', 
        'keep *_hpsPFTauDiscrimination*_*_*', 
        'keep *_hpsPFTau*PtSum_*_*', 
        'keep *_hpsPFTauTransverseImpactParameters_*_*', 
        'keep  *_offlinePrimaryVertices__*', 
        'keep  *_offlinePrimaryVerticesWithBS_*_*', 
        'keep  *_offlinePrimaryVerticesFromCosmicTracks_*_*', 
        'keep  *_nuclearInteractionMaker_*_*', 
        'keep *_generalV0Candidates_*_*', 
        'keep *_inclusiveSecondaryVertices_*_*', 
        'keep recoGsfElectronCores_gsfElectronCores_*_*', 
        'keep recoGsfElectrons_gsfElectrons_*_*', 
        'keep recoGsfElectronCores_uncleanedOnlyGsfElectronCores_*_*', 
        'keep recoGsfElectrons_uncleanedOnlyGsfElectrons_*_*', 
        'keep floatedmValueMap_eidRobustLoose_*_*', 
        'keep floatedmValueMap_eidRobustTight_*_*', 
        'keep floatedmValueMap_eidRobustHighEnergy_*_*', 
        'keep floatedmValueMap_eidLoose_*_*', 
        'keep floatedmValueMap_eidTight_*_*', 
        'keep *_egmGedGsfElectronPFIsolation_*_*', 
        'keep *_egmGsfElectronIDs_*_*', 
        'keep *_gedPhotonCore_*_*', 
        'keep *_gedPhotons_*_*', 
        'keep *_particleBasedIsolation_*_*', 
        'keep recoPhotons_mustachePhotons_*_*', 
        'keep recoPhotonCores_mustachePhotonCore_*_*', 
        'keep recoPhotons_photons_*_*', 
        'keep recoPhotonCores_photonCore_*_*', 
        'keep recoConversions_conversions_*_*', 
        'keep recoConversions_mustacheConversions_*_*', 
        'drop *_conversions_uncleanedConversions_*', 
        'drop *_gedPhotonsTmp_valMapPFEgammaCandToPhoton_*', 
        'keep recoConversions_allConversions_*_*', 
        'keep recoConversions_allConversionsOldEG_*_*', 
        'keep recoTracks_ckfOutInTracksFrom*Conversions_*_*', 
        'keep recoTracks_ckfInOutTracksFrom*Conversions_*_*', 
        'keep recoTrackExtras_ckfOutInTracksFrom*Conversions_*_*', 
        'keep recoTrackExtras_ckfInOutTracksFrom*Conversions_*_*', 
        'keep TrackingRecHitsOwned_ckfOutInTracksFrom*Conversions_*_*', 
        'keep TrackingRecHitsOwned_ckfInOutTracksFrom*Conversions_*_*', 
        'keep recoConversions_uncleanedOnlyAllConversions_*_*', 
        'keep recoTracks_uncleanedOnlyCkfOutInTracksFromConversions_*_*', 
        'keep recoTracks_uncleanedOnlyCkfInOutTracksFromConversions_*_*', 
        'keep recoTrackExtras_uncleanedOnlyCkfOutInTracksFromConversions_*_*', 
        'keep recoTrackExtras_uncleanedOnlyCkfInOutTracksFromConversions_*_*', 
        'keep TrackingRecHitsOwned_uncleanedOnlyCkfOutInTracksFromConversions_*_*', 
        'keep TrackingRecHitsOwned_uncleanedOnlyCkfInOutTracksFromConversions_*_*', 
        'keep *_PhotonIDProd_*_*', 
        'keep *_PhotonIDProdGED_*_*', 
        'keep *_hfRecoEcalCandidate_*_*', 
        'keep *_hfEMClusters_*_*', 
        'keep *_gedGsfElectronCores_*_*', 
        'keep *_gedGsfElectrons_*_*', 
        'keep *_pixelTracks_*_*', 
        'keep *_pixelVertices_*_*', 
        'drop CaloTowersSorted_towerMakerPF_*_*', 
        'keep recoPFRecHits_particleFlowClusterECAL_Cleaned_*', 
        'keep recoPFRecHits_particleFlowClusterHCAL_Cleaned_*', 
        'keep recoPFRecHits_particleFlowClusterHO_Cleaned_*', 
        'keep recoPFRecHits_particleFlowClusterHFEM_Cleaned_*', 
        'keep recoPFRecHits_particleFlowClusterHFHAD_Cleaned_*', 
        'keep recoPFRecHits_particleFlowClusterPS_Cleaned_*', 
        'keep recoPFRecHits_particleFlowRecHitECAL_Cleaned_*', 
        'keep recoPFRecHits_particleFlowRecHitHCAL_Cleaned_*', 
        'keep recoPFRecHits_particleFlowRecHitHO_Cleaned_*', 
        'keep recoPFRecHits_particleFlowRecHitPS_Cleaned_*', 
        'keep recoPFClusters_particleFlowClusterECAL_*_*', 
        'keep recoPFClusters_particleFlowClusterHCAL_*_*', 
        'keep recoPFClusters_particleFlowClusterHO_*_*', 
        'keep recoPFClusters_particleFlowClusterPS_*_*', 
        'keep recoPFBlocks_particleFlowBlock_*_*', 
        'keep recoPFCandidates_particleFlowEGamma_*_*', 
        'keep recoCaloClusters_particleFlowEGamma_*_*', 
        'keep recoSuperClusters_particleFlowEGamma_*_*', 
        'keep recoConversions_particleFlowEGamma_*_*', 
        'keep recoPFCandidates_particleFlow_*_*', 
        'keep recoPFCandidates_particleFlowTmp_electrons_*', 
        'keep recoPFCandidates_particleFlowTmp_*_*', 
        'drop recoPFCandidates_particleFlowTmp__*', 
        'keep recoPFDisplacedVertexs_particleFlowDisplacedVertex_*_*', 
        'keep *_pfElectronTranslator_*_*', 
        'keep *_pfPhotonTranslator_*_*', 
        'keep *_particleFlow_electrons_*', 
        'keep *_particleFlow_photons_*', 
        'keep *_trackerDrivenElectronSeeds_preid_*', 
        'keep *_particleFlowPtrs_*_*', 
        'keep *_particleFlowTmpPtrs_*_*', 
        'keep *_offlineBeamSpot_*_*', 
        'keep L1GlobalTriggerReadoutRecord_gtDigis_*_*', 
        'keep *_l1GtRecord_*_*', 
        'keep *_l1GtTriggerMenuLite_*_*', 
        'keep *_conditionsInEdm_*_*', 
        'keep *_l1extraParticles_*_*', 
        'keep *_l1L1GtObjectMap_*_*', 
        'keep L1MuGMTReadoutCollection_gtDigis_*_*', 
        'keep L1GctEmCand*_gctDigis_*_*', 
        'keep L1GctJetCand*_gctDigis_*_*', 
        'keep L1GctEtHad*_gctDigis_*_*', 
        'keep L1GctEtMiss*_gctDigis_*_*', 
        'keep L1GctEtTotal*_gctDigis_*_*', 
        'keep L1GctHtMiss*_gctDigis_*_*', 
        'keep L1GctJetCounts*_gctDigis_*_*', 
        'keep L1GctHFRingEtSums*_gctDigis_*_*', 
        'keep L1GctHFBitCounts*_gctDigis_*_*', 
        'keep LumiDetails_lumiProducer_*_*', 
        'keep LumiSummary_lumiProducer_*_*', 
        'drop *_hlt*_*_*', 
        'keep *_hltL1GtObjectMap_*_*', 
        'keep edmTriggerResults_*_*_*', 
        'keep triggerTriggerEvent_*_*_*', 
        'keep LHERunInfoProduct_*_*_*', 
        'keep LHEEventProduct_*_*_*', 
        'keep GenRunInfoProduct_generator_*_*', 
        'keep GenLumiInfoProduct_generator_*_*', 
        'keep GenEventInfoProduct_generator_*_*', 
        'keep edmHepMCProduct_generator_*_*', 
        'keep GenFilterInfo_*_*_*', 
        'keep *_genParticles_*_*', 
        'keep recoGenMETs_*_*_*', 
        'keep *_kt4GenJets_*_*', 
        'keep *_kt6GenJets_*_*', 
        'keep *_ak4GenJets_*_*', 
        'keep *_ak5GenJets_*_*', 
        'keep *_ak7GenJets_*_*', 
        'keep *_iterativeCone5GenJets_*_*', 
        'keep *_genParticle_*_*', 
        'keep edmHepMCProduct_source_*_*', 
        'keep SimTracks_g4SimHits_*_*', 
        'keep SimVertexs_g4SimHits_*_*', 
        'keep *_allTrackMCMatch_*_*', 
        'keep StripDigiSimLinkedmDetSetVector_simMuonCSCDigis_*_*', 
        'keep DTLayerIdDTDigiSimLinkMuonDigiCollection_simMuonDTDigis_*_*', 
        'keep RPCDigiSimLinkedmDetSetVector_simMuonRPCDigis_*_*', 
        'keep PileupSummaryInfos_*_*_*', 
        'keep L1AcceptBunchCrossings_*_*_*', 
        'keep L1TriggerScalerss_*_*_*', 
        'keep Level1TriggerScalerss_*_*_*', 
        'keep LumiScalerss_*_*_*', 
        'keep BeamSpotOnlines_*_*_*', 
        'keep DcsStatuss_*_*_*', 
        'keep *_logErrorHarvester_*_*', 
        'keep *_pfIsolatedElectronsEI_*_*', 
        'keep *_pfIsolatedMuonsEI_*_*', 
        'keep recoPFJets_pfJetsEI_*_*', 
        'keep *_pfJetTrackAssociatorEI_*_*', 
        'keep *_impactParameterTagInfosEI_*_*', 
        'keep *_secondaryVertexTagInfosEI_*_*', 
        'keep *_combinedSecondaryVertexBJetTagsEI_*_*', 
        'keep recoPFTaus_pfTausEI_*_*', 
        'keep recoPFTauDiscriminator_pfTausDiscrimination*_*_*', 
        'keep *_pfMetEI_*_*' ) )
)

process.GENRAWEventContent = cms.PSet(
    splitLevel = cms.untracked.int32(0),
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
    outputCommands = cms.untracked.vstring('drop *', 
        'drop *', 
        'keep  FEDRawDataCollection_rawDataCollector_*_*', 
        'keep  FEDRawDataCollection_source_*_*', 
        'keep  FEDRawDataCollection_rawDataCollector_*_*', 
        'keep  FEDRawDataCollection_source_*_*', 
        'drop *_hlt*_*_*', 
        'keep *_hltL1GtObjectMap_*_*', 
        'keep FEDRawDataCollection_rawDataCollector_*_*', 
        'keep FEDRawDataCollection_source_*_*', 
        'keep edmTriggerResults_*_*_*', 
        'keep triggerTriggerEvent_*_*_*', 
        'keep LHERunInfoProduct_*_*_*', 
        'keep LHEEventProduct_*_*_*', 
        'keep GenRunInfoProduct_generator_*_*', 
        'keep GenLumiInfoProduct_generator_*_*', 
        'keep GenEventInfoProduct_generator_*_*', 
        'keep edmHepMCProduct_generator_*_*', 
        'keep GenFilterInfo_*_*_*', 
        'keep *_genParticles_*_*', 
        'keep edmHepMCProduct_source_*_*', 
        'keep SimTracks_g4SimHits_*_*', 
        'keep SimVertexs_g4SimHits_*_*', 
        'keep *_allTrackMCMatch_*_*', 
        'keep StripDigiSimLinkedmDetSetVector_simMuonCSCDigis_*_*', 
        'keep DTLayerIdDTDigiSimLinkMuonDigiCollection_simMuonDTDigis_*_*', 
        'keep RPCDigiSimLinkedmDetSetVector_simMuonRPCDigis_*_*', 
        'keep PileupSummaryInfos_*_*_*', 
        'keep recoGenMETs_*_*_*', 
        'keep recoGenJets_*_*_*', 
        'keep *_genParticle_*_*', 
        'keep *_MEtoEDMConverter_*_*', 
        'keep *_randomEngineStateProducer_*_*', 
        'keep FEDRawDataCollection_source_*_*', 
        'keep FEDRawDataCollection_rawDataCollector_*_*', 
        'keep *_logErrorHarvester_*_*')
)

process.GeneratorInterfaceAOD = cms.PSet(
    outputCommands = cms.untracked.vstring('keep LHERunInfoProduct_*_*_*', 
        'keep LHEEventProduct_*_*_*', 
        'keep GenRunInfoProduct_generator_*_*', 
        'keep GenLumiInfoProduct_generator_*_*', 
        'keep GenEventInfoProduct_generator_*_*', 
        'keep GenFilterInfo_*_*_*', 
        'keep *_genParticles_*_*')
)

process.GeneratorInterfaceLHE = cms.PSet(
    outputCommands = cms.untracked.vstring('keep LHERunInfoProduct_*_*_*', 
        'keep LHEEventProduct_*_*_*', 
        'keep *_externalLHEProducer_LHEScriptOutput_*')
)

process.GeneratorInterfaceRAW = cms.PSet(
    outputCommands = cms.untracked.vstring('keep LHERunInfoProduct_*_*_*', 
        'keep LHEEventProduct_*_*_*', 
        'keep GenRunInfoProduct_generator_*_*', 
        'keep GenLumiInfoProduct_generator_*_*', 
        'keep GenEventInfoProduct_generator_*_*', 
        'keep edmHepMCProduct_generator_*_*', 
        'keep GenFilterInfo_*_*_*', 
        'keep *_genParticles_*_*')
)

process.GeneratorInterfaceRECO = cms.PSet(
    outputCommands = cms.untracked.vstring('keep LHERunInfoProduct_*_*_*', 
        'keep LHEEventProduct_*_*_*', 
        'keep GenRunInfoProduct_generator_*_*', 
        'keep GenLumiInfoProduct_generator_*_*', 
        'keep GenEventInfoProduct_generator_*_*', 
        'keep edmHepMCProduct_generator_*_*', 
        'keep GenFilterInfo_*_*_*', 
        'keep *_genParticles_*_*')
)

process.HLTDEBUGEventContent = cms.PSet(
    splitLevel = cms.untracked.int32(0),
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
    outputCommands = cms.untracked.vstring( ('drop *', 
        'keep *_logErrorHarvester_*_*', 
        'drop *_hlt*_*_*', 
        'keep *_hltAK4CaloJetsCorrectedIDPassed_*_*', 
        'keep *_hltAK4CaloJetsIDPassed_*_*', 
        'keep *_hltAK4CaloJets_*_*', 
        'keep *_hltAK4PFJetsCorrected_*_*', 
        'keep *_hltAK4PFJetsForTaus_*_*', 
        'keep *_hltAK4PFJets_*_*', 
        'keep *_hltAlCaEtaEBUncalibrator_*_*', 
        'keep *_hltAlCaEtaEEUncalibrator_*_*', 
        'keep *_hltAlCaEtaRecHitsFilterEBonly_*_*', 
        'keep *_hltAlCaEtaRecHitsFilterEEonly_*_*', 
        'keep *_hltAlCaEtaRecHitsFilterEEonly_etaEcalRecHitsES_*', 
        'keep *_hltAlCaEtaRecHitsFilter_*_*', 
        'keep *_hltAlCaPhiSymStream_*_*', 
        'keep *_hltAlCaPhiSymUncalibrator_*_*', 
        'keep *_hltAlCaPi0EBUncalibrator_*_*', 
        'keep *_hltAlCaPi0EEUncalibrator_*_*', 
        'keep *_hltAlCaPi0RecHitsFilterEBonly_*_*', 
        'keep *_hltAlCaPi0RecHitsFilterEEonly_*_*', 
        'keep *_hltAlCaPi0RecHitsFilterEEonly_pi0EcalRecHitsES_*', 
        'keep *_hltAlCaPi0RecHitsFilter_*_*', 
        'keep *_hltBLifetimeL25AssociatorbbPhiL1FastJetFastPV_*_*', 
        'keep *_hltBLifetimeL25BJetTagsbbPhiL1FastJetFastPV_*_*', 
        'keep *_hltBLifetimeL25TagInfosbbPhiL1FastJetFastPV_*_*', 
        'keep *_hltBLifetimeL3AssociatorbbPhiL1FastJetFastPV_*_*', 
        'keep *_hltBLifetimeL3BJetTagsbbPhiL1FastJetFastPV_*_*', 
        'keep *_hltBLifetimeL3TagInfosbbPhiL1FastJetFastPV_*_*', 
        'keep *_hltBLifetimeRegionalCtfWithMaterialTracksbbPhiL1FastJetFastPV_*_*', 
        'keep *_hltBSoftMuonDiJet110Mu5L3FilterByDR_*_*', 
        'keep *_hltBSoftMuonDiJet110Mu5SelL3BJetTagsByDR_*_*', 
        'keep *_hltBSoftMuonDiJet20Mu5L3FilterByDR_*_*', 
        'keep *_hltBSoftMuonDiJet20Mu5SelL3BJetTagsByDR_*_*', 
        'keep *_hltBSoftMuonDiJet40Mu5L3FilterByDR_*_*', 
        'keep *_hltBSoftMuonDiJet40Mu5SelL3BJetTagsByDR_*_*', 
        'keep *_hltBSoftMuonDiJet70Mu5L3FilterByDR_*_*', 
        'keep *_hltBSoftMuonDiJet70Mu5SelL3BJetTagsByDR_*_*', 
        'keep *_hltBSoftMuonMu5L3_*_*', 
        'keep *_hltCSVJetTagSingleTopEle27_*_*', 
        'keep *_hltCSVJetTagSingleTopIsoMu24_*_*', 
        'keep *_hltCaloJetCorrectedRegional_*_*', 
        'keep *_hltCaloJetCorrected_*_*', 
        'keep *_hltCaloJetL1FastJetCorrected_*_*', 
        'keep *_hltCleanedCorrectedIslandBarrelSuperClustersHI_*_*', 
        'keep *_hltCleanedHiCorrectedIslandBarrelSuperClustersHI_*_*', 
        'keep *_hltConvPFTausTightIsoTrackFindingIsolation_*_*', 
        'keep *_hltConvPFTausTightIsoTrackFinding_*_*', 
        'keep *_hltConvPFTausTightIsoTrackPt5Isolation_*_*', 
        'keep *_hltConvPFTausTightIsoTrackPt5_*_*', 
        'keep *_hltConvPFTausTightIso_*_*', 
        'keep *_hltConvPFTausTrackFindingLooseIsolation_*_*', 
        'keep *_hltConvPFTausTrackFinding_*_*', 
        'keep *_hltConvPFTaus_*_*', 
        'keep *_hltCorrectedIslandBarrelSuperClustersHI_*_*', 
        'keep *_hltCorrectedIslandEndcapSuperClustersHI_*_*', 
        'keep *_hltCsc2DRecHits_*_*', 
        'keep *_hltCscSegments_*_*', 
        'keep *_hltDoublePFTau25TrackPt5MediumIsolationProng4L1HLTMatched_*_*', 
        'keep *_hltDoublePFTau25TrackPt5MediumIsolationProng4_*_*', 
        'keep *_hltDoublePFTau25TrackPt5MediumIsolation_*_*', 
        'keep *_hltDoublePFTau25TrackPt5_*_*', 
        'keep *_hltDoublePFTau25_*_*', 
        'keep *_hltDoublePFTauTightIso45Track5_*_*', 
        'keep *_hltDoublePFTauTightIso45Track_*_*', 
        'keep *_hltDt4DSegments_*_*', 
        'keep *_hltEcalRecHitAll_*_*', 
        'keep *_hltEle20CaloIdVTCaloIsoTTrkIdTTrkIsoTTrackIsoFilterL1SingleEG18orL1SingleEG20_*_*', 
        'keep *_hltEle20CaloIdVTTrkIdTDphiFilter_*_*', 
        'keep *_hltEle27WP85PixelMatchElectronsL1Seeded_*_*', 
        'keep *_hltFastPVPixelTracksMerger_*_*', 
        'keep *_hltFastPVPixelTracksRecover_*_*', 
        'keep *_hltFastPVPixelTracks_*_*', 
        'keep *_hltFastPVPixelVertices3D_*_*', 
        'keep *_hltFastPVPixelVertices_*_*', 
        'keep *_hltFastPixelBLifetimeL3TagInfos_*_*', 
        'keep *_hltFastPrimaryVertex_*_*', 
        'keep *_hltFilterDoubleIsoPFTau45Trk5LeadTrack5IsolationL1HLTMatched_*_*', 
        'keep *_hltFilterL2EtCutDoublePFIsoTau45Trk5_*_*', 
        'keep *_hltFilterL2EtCutSingleIsoPFTau35Trk20MET70_*_*', 
        'keep *_hltFilterSingleIsoPFTau35Trk20LeadTrackPt20_*_*', 
        'keep *_hltFilterSingleIsoPFTau35Trk20MET60LeadTrack20IsolationL1HLTMatched_*_*', 
        'keep *_hltFilterSingleIsoPFTau35Trk20MET70LeadTrack20IsolationL1HLTMatched_*_*', 
        'keep *_hltGoodOnlinePVs_*_*', 
        'keep *_hltHICaloJetCorrected_*_*', 
        'keep *_hltHICaloJetIDPassed_*_*', 
        'keep *_hltHIGoodLooseTracks_*_*', 
        'keep *_hltHIPixel3PrimTracks_*_*', 
        'keep *_hltHISelectedVertex_*_*', 
        'keep *_hltHISiPixelClusters_*_*', 
        'keep *_hltHITIPTCorrectorHB_*_*', 
        'keep *_hltHITIPTCorrectorHE_*_*', 
        'keep *_hltHiCorrectedIslandBarrelSuperClustersHI_*_*', 
        'keep *_hltHiCorrectedIslandEndcapSuperClustersHI_*_*', 
        'keep *_hltHiIslandSuperClustersHI_*_*', 
        'keep *_hltIsolPixelTrackProdHB_*_*', 
        'keep *_hltIsolPixelTrackProdHE_*_*', 
        'keep *_hltIter0PFlowTrackSelectionHighPurity_*_*', 
        'keep *_hltIter1Merged_*_*', 
        'keep *_hltIter1PFlowTrackSelectionHighPurity_*_*', 
        'keep *_hltIter2Merged_*_*', 
        'keep *_hltIter2PFlowTrackSelectionHighPurity_*_*', 
        'keep *_hltIter3Merged_*_*', 
        'keep *_hltIter4Merged_*_*', 
        'keep *_hltIterativeCone5PileupSubtractionCaloJets_*_*', 
        'keep *_hltL1GtObjectMap_*_*', 
        'keep *_hltL1HLTSingleIsoPFTau35Trk20Met60JetsMatch_*_*', 
        'keep *_hltL1IsoElectronTrackIsol_*_*', 
        'keep *_hltL1NonIsoElectronTrackIsol_*_*', 
        'keep *_hltL1SeededRecoEcalCandidate_*_*', 
        'keep *_hltL1extraParticlesCentral_*_*', 
        'keep *_hltL1extraParticlesNonIsolated_*_*', 
        'keep *_hltL1extraParticlesTau_*_*', 
        'keep *_hltL1extraParticles_*_*', 
        'keep *_hltL1sDoubleTauJet44Eta2p17orDoubleJet64Central_*_*', 
        'keep *_hltL1sDoubleTauJet44erorDoubleJetC64_*_*', 
        'keep *_hltL1sL1EG18er_*_*', 
        'keep *_hltL1sL1ETM36ORETM40_*_*', 
        'keep *_hltL1sL1Jet52ETM30_*_*', 
        'keep *_hltL1sL1SingleEG12_*_*', 
        'keep *_hltL1sL1SingleEG15_*_*', 
        'keep *_hltL1sL1SingleEG18orL1SingleEG20_*_*', 
        'keep *_hltL1sL1SingleMu10_*_*', 
        'keep *_hltL1sL1SingleMu14Eta2p1_*_*', 
        'keep *_hltL1sMu16Eta2p1_*_*', 
        'keep *_hltL2MuonCandidatesNoVtx_*_*', 
        'keep *_hltL2MuonCandidates_*_*', 
        'keep *_hltL2MuonSeeds_*_*', 
        'keep *_hltL2Muons_*_*', 
        'keep *_hltL2TauJets_*_*', 
        'keep *_hltL3CombinedSecondaryVertexBJetTags_*_*', 
        'keep *_hltL3MuonCandidates_*_*', 
        'keep *_hltL3MuonsIOHit_*_*', 
        'keep *_hltL3MuonsLinksCombination_*_*', 
        'keep *_hltL3MuonsOIHit_*_*', 
        'keep *_hltL3MuonsOIState_*_*', 
        'keep *_hltL3Muons_*_*', 
        'keep *_hltL3NoFiltersNoVtxMuonCandidates_*_*', 
        'keep *_hltL3NoFiltersNoVtxMuons_*_*', 
        'keep *_hltL3SecondaryVertexTagInfos_*_*', 
        'keep *_hltL3TkFromL2OICombination_*_*', 
        'keep *_hltL3TkTracksFromL2IOHit_*_*', 
        'keep *_hltL3TkTracksFromL2OIHit_*_*', 
        'keep *_hltL3TkTracksFromL2OIState_*_*', 
        'keep *_hltL3TkTracksFromL2_*_*', 
        'keep *_hltL3TrackCandidateFromL2IOHit_*_*', 
        'keep *_hltL3TrackCandidateFromL2OIHit_*_*', 
        'keep *_hltL3TrackCandidateFromL2OIState_*_*', 
        'keep *_hltL3TrajSeedIOHit_*_*', 
        'keep *_hltL3TrajSeedOIHit_*_*', 
        'keep *_hltL3TrajSeedOIState_*_*', 
        'keep *_hltL3TrajectorySeed_*_*', 
        'keep *_hltL3crIsoL1sMu16Eta2p1L1f0L2f16QL3f18QL3crIsoRhoFiltered0p15_*_*', 
        'keep *_hltLeadingCentralJets30SingleTopEle27_*_*', 
        'keep *_hltLeadingCentralJets30SingleTopIsoMu24_*_*', 
        'keep *_hltMetForHf_*_*', 
        'keep *_hltMet_*_*', 
        'keep *_hltMu8Ele17CaloIdTCaloIsoVLPixelMatchFilter_*_*', 
        'keep *_hltMuTrackJpsiCtfTrackCands_*_*', 
        'keep *_hltMuTrackJpsiPixelTrackCands_*_*', 
        'keep *_hltMuonCSCDigis_*_*', 
        'keep *_hltMuonCSCDigis_MuonCSCStripDigi_*', 
        'keep *_hltMuonCSCDigis_MuonCSCWireDigi_*', 
        'keep *_hltMuonDTDigis_*_*', 
        'keep *_hltMuonRPCDigis_*_*', 
        'keep *_hltOnlineBeamSpot_*_*', 
        'keep *_hltOnlinePrimaryVertices_*_*', 
        'keep *_hltOverlapFilterEle20LooseIsoPFTau20OldVersion_*_*', 
        'keep *_hltOverlapFilterIsoEle20MediumIsoPFTau20_*_*', 
        'keep *_hltOverlapFilterIsoMu15IsoPFTau20_*_*', 
        'keep *_hltOverlapFilterIsoMu15MediumIsoPFTau20_*_*', 
        'keep *_hltOverlapFilterIsoMu15TightIsoPFTau20_*_*', 
        'keep *_hltOverlapFilterIsoMu18LooseIsoPFTau20_*_*', 
        'keep *_hltOverlapFilterIsoMu18PFTau25TrackPt5Prong4_*_*', 
        'keep *_hltPFTau15TrackLooseIso_*_*', 
        'keep *_hltPFTau15Track_*_*', 
        'keep *_hltPFTau15_*_*', 
        'keep *_hltPFTau20IsoMuVertex_*_*', 
        'keep *_hltPFTau20TrackLooseIso_*_*', 
        'keep *_hltPFTau20Track_*_*', 
        'keep *_hltPFTau20_*_*', 
        'keep *_hltPFTau25TrackPt5MediumIsolationProng4IsoMuVertex_*_*', 
        'keep *_hltPFTau25TrackPt5MediumIsolationProng4_*_*', 
        'keep *_hltPFTau25TrackPt5MediumIsolation_*_*', 
        'keep *_hltPFTau25TrackPt5_*_*', 
        'keep *_hltPFTau25_*_*', 
        'keep *_hltPFTau35TrackPt20LooseIsoProng2_*_*', 
        'keep *_hltPFTau35TrackPt20LooseIso_*_*', 
        'keep *_hltPFTau35TrackPt20_*_*', 
        'keep *_hltPFTau35Track_*_*', 
        'keep *_hltPFTau35_*_*', 
        'keep *_hltPFTauEleVertex20_*_*', 
        'keep *_hltPFTauJetTracksAssociator_*_*', 
        'keep *_hltPFTauMediumIso20TrackMediumIso_*_*', 
        'keep *_hltPFTauMediumIso20Track_*_*', 
        'keep *_hltPFTauMediumIso20_*_*', 
        'keep *_hltPFTauMediumIso35Track_*_*', 
        'keep *_hltPFTauMediumIso35_*_*', 
        'keep *_hltPFTauTagInfo_*_*', 
        'keep *_hltPFTauTightIso20TrackTightIso_*_*', 
        'keep *_hltPFTauTightIso20Track_*_*', 
        'keep *_hltPFTauTightIso20_*_*', 
        'keep *_hltPFlowTrackSelectionHighPurity_*_*', 
        'keep *_hltParticleFlowForTaus_*_*', 
        'keep *_hltParticleFlow_*_*', 
        'keep *_hltPixelMatch3HitElectronsActivity_*_*', 
        'keep *_hltPixelMatch3HitElectronsL1Seeded_*_*', 
        'keep *_hltPixelMatchCleanElectronsL1Seeded_*_*', 
        'keep *_hltPixelMatchElectronsActivity_*_*', 
        'keep *_hltPixelMatchElectronsL1Iso_*_*', 
        'keep *_hltPixelMatchElectronsL1NonIso_*_*', 
        'keep *_hltPixelMatchElectronsL1Seeded_*_*', 
        'keep *_hltPixelTracks_*_*', 
        'keep *_hltPixelVertices3DbbPhi_*_*', 
        'keep *_hltPixelVertices_*_*', 
        'keep *_hltRecoEcalSuperClusterActivityCandidateSC4_*_*', 
        'keep *_hltRecoEcalSuperClusterActivityCandidateSC5_*_*', 
        'keep *_hltRecoEcalSuperClusterActivityCandidate_*_*', 
        'keep *_hltRpcRecHits_*_*', 
        'keep *_hltSelector4CentralJetsL1FastJet_*_*', 
        'keep *_hltSelectorJets20L1FastJet_*_*', 
        'keep *_hltSiPixelCluster_*_*', 
        'keep *_hltSiPixelClusters_*_*', 
        'keep *_hltSiStripClusters_*_*', 
        'keep *_hltSiStripRawToClustersFacility_*_*', 
        'keep *_hltSingleMu15L3Filtered15_*_*', 
        'keep *_hltSingleMuIsoL1s14L3IsoFiltered15eta2p1_*_*', 
        'keep *_hltSingleMuIsoL3IsoFiltered15_*_*', 
        'keep *_hltTowerMakerForAll_*_*', 
        'keep *_hltTowerMakerForMuons_*_*', 
        'keep *_hltTriggerSummaryAOD_*_*', 
        'keep *_hltTriggerSummaryRAW_*_*', 
        'keep *_hltTrimmedPixelVertices_*_*', 
        'keep *_hltVerticesL3_*_*', 
        'keep DcsStatuss_hltScalersRawToDigi_*_*', 
        'keep FEDRawDataCollection_rawDataCollector_*_*', 
        'keep FEDRawDataCollection_rawDataRepacker_*_*', 
        'keep FEDRawDataCollection_source_*_*', 
        'keep FEDRawDataCollection_virginRawDataRepacker_*_*', 
        'keep L1GlobalTriggerReadoutRecord_hltGtDigis_*_*', 
        'keep L1MuGMTCands_hltGtDigis_*_*', 
        'keep L1MuGMTReadoutCollection_hltGtDigis_*_*', 
        'keep L2MuonTrajectorySeeds_hltL2MuonSeeds_*_*', 
        'keep L3MuonTrajectorySeeds_hltHIL3TrajSeedOIHit_*_*', 
        'keep L3MuonTrajectorySeeds_hltHIL3TrajectorySeed_*_*', 
        'keep L3MuonTrajectorySeeds_hltL3TrajSeedOIState_*_*', 
        'keep LumiScalerss_hltScalersRawToDigi_*_*', 
        'keep SiPixelClusteredmNewDetSetVector_hltSiPixelClusters_*_*', 
        'keep TrackCandidates_hltHIL3TrackCandidateFromL2OIHit_*_*', 
        'keep TrackCandidates_hltHIL3TrackCandidateFromL2OIState_*_*', 
        'keep TrackingRecHitsOwned_hltL3Muons_*_*', 
        'keep edmTriggerResults_*_*_*', 
        'keep recoCaloJets_*_*_*', 
        'keep recoCaloMETs_*_*_*', 
        'keep recoCaloMETs_hltMet_*_*', 
        'keep recoCompositeCandidates_*_*_*', 
        'keep recoElectrons_*_*_*', 
        'keep recoIsolatedPixelTrackCandidates_*_*_*', 
        'keep recoMETs_*_*_*', 
        'keep recoPFJets_*_*_*', 
        'keep recoPFTaus_*_*_*', 
        'keep recoRecoChargedCandidates_*_*_*', 
        'keep recoRecoChargedCandidates_hltHIL3MuonCandidates_*_*', 
        'keep recoRecoChargedCandidates_hltL2MuonCandidates_*_*', 
        'keep recoRecoEcalCandidatesToValuefloatAssociation_hltL1IsoHLTClusterShape_*_*', 
        'keep recoRecoEcalCandidatesToValuefloatAssociation_hltL1IsolatedPhotonEcalIsol_*_*', 
        'keep recoRecoEcalCandidatesToValuefloatAssociation_hltL1IsolatedPhotonHcalForHE_*_*', 
        'keep recoRecoEcalCandidatesToValuefloatAssociation_hltL1IsolatedPhotonHcalIsol_*_*', 
        'keep recoRecoEcalCandidatesToValuefloatAssociation_hltL1NonIsoHLTClusterShape_*_*', 
        'keep recoRecoEcalCandidatesToValuefloatAssociation_hltL1NonIsolatedPhotonEcalIsol_*_*', 
        'keep recoRecoEcalCandidatesToValuefloatAssociation_hltL1NonIsolatedPhotonHcalForHE_*_*', 
        'keep recoRecoEcalCandidatesToValuefloatAssociation_hltL1NonIsolatedPhotonHcalIsol_*_*', 
        'keep recoRecoEcalCandidates_*_*_*', 
        'keep recoRecoEcalCandidates_hltL1IsoRecoEcalCandidate_*_*', 
        'keep recoRecoEcalCandidates_hltL1NonIsoRecoEcalCandidate_*_*', 
        'keep recoTrackExtras_hltHIL3MuonsOIHit_*_*', 
        'keep recoTrackExtras_hltHIL3MuonsOIState_*_*', 
        'keep recoTracks_hltHIL3MuonsOIHit_*_*', 
        'keep recoTracks_hltHIL3MuonsOIState_*_*', 
        'keep recoTracks_hltHIL3Muons_*_*', 
        'keep recoTracks_hltHIL3TkTracksFromL2OIHit_*_*', 
        'keep recoTracks_hltHIL3TkTracksFromL2OIState_*_*', 
        'keep recoTracks_hltHIL3TkTracksFromL2_*_*', 
        'keep triggerTriggerEventWithRefs_*_*_*', 
        'keep triggerTriggerEvent_*_*_*', 
        'keep triggerTriggerFilterObjectWithRefs_*_*_*' ) )
)

process.HLTDebugFEVT = cms.PSet(
    outputCommands = cms.vstring( ('drop *_hlt*_*_*', 
        'keep *_hltAK4CaloJetsCorrectedIDPassed_*_*', 
        'keep *_hltAK4CaloJetsIDPassed_*_*', 
        'keep *_hltAK4CaloJets_*_*', 
        'keep *_hltAK4PFJetsCorrected_*_*', 
        'keep *_hltAK4PFJetsForTaus_*_*', 
        'keep *_hltAK4PFJets_*_*', 
        'keep *_hltAlCaEtaEBUncalibrator_*_*', 
        'keep *_hltAlCaEtaEEUncalibrator_*_*', 
        'keep *_hltAlCaEtaRecHitsFilterEBonly_*_*', 
        'keep *_hltAlCaEtaRecHitsFilterEEonly_*_*', 
        'keep *_hltAlCaEtaRecHitsFilterEEonly_etaEcalRecHitsES_*', 
        'keep *_hltAlCaEtaRecHitsFilter_*_*', 
        'keep *_hltAlCaPhiSymStream_*_*', 
        'keep *_hltAlCaPhiSymUncalibrator_*_*', 
        'keep *_hltAlCaPi0EBUncalibrator_*_*', 
        'keep *_hltAlCaPi0EEUncalibrator_*_*', 
        'keep *_hltAlCaPi0RecHitsFilterEBonly_*_*', 
        'keep *_hltAlCaPi0RecHitsFilterEEonly_*_*', 
        'keep *_hltAlCaPi0RecHitsFilterEEonly_pi0EcalRecHitsES_*', 
        'keep *_hltAlCaPi0RecHitsFilter_*_*', 
        'keep *_hltBLifetimeL25AssociatorbbPhiL1FastJetFastPV_*_*', 
        'keep *_hltBLifetimeL25BJetTagsbbPhiL1FastJetFastPV_*_*', 
        'keep *_hltBLifetimeL25TagInfosbbPhiL1FastJetFastPV_*_*', 
        'keep *_hltBLifetimeL3AssociatorbbPhiL1FastJetFastPV_*_*', 
        'keep *_hltBLifetimeL3BJetTagsbbPhiL1FastJetFastPV_*_*', 
        'keep *_hltBLifetimeL3TagInfosbbPhiL1FastJetFastPV_*_*', 
        'keep *_hltBLifetimeRegionalCtfWithMaterialTracksbbPhiL1FastJetFastPV_*_*', 
        'keep *_hltBSoftMuonDiJet110Mu5L3FilterByDR_*_*', 
        'keep *_hltBSoftMuonDiJet110Mu5SelL3BJetTagsByDR_*_*', 
        'keep *_hltBSoftMuonDiJet20Mu5L3FilterByDR_*_*', 
        'keep *_hltBSoftMuonDiJet20Mu5SelL3BJetTagsByDR_*_*', 
        'keep *_hltBSoftMuonDiJet40Mu5L3FilterByDR_*_*', 
        'keep *_hltBSoftMuonDiJet40Mu5SelL3BJetTagsByDR_*_*', 
        'keep *_hltBSoftMuonDiJet70Mu5L3FilterByDR_*_*', 
        'keep *_hltBSoftMuonDiJet70Mu5SelL3BJetTagsByDR_*_*', 
        'keep *_hltBSoftMuonMu5L3_*_*', 
        'keep *_hltCSVJetTagSingleTopEle27_*_*', 
        'keep *_hltCSVJetTagSingleTopIsoMu24_*_*', 
        'keep *_hltCaloJetCorrectedRegional_*_*', 
        'keep *_hltCaloJetCorrected_*_*', 
        'keep *_hltCaloJetL1FastJetCorrected_*_*', 
        'keep *_hltCleanedCorrectedIslandBarrelSuperClustersHI_*_*', 
        'keep *_hltCleanedHiCorrectedIslandBarrelSuperClustersHI_*_*', 
        'keep *_hltConvPFTausTightIsoTrackFindingIsolation_*_*', 
        'keep *_hltConvPFTausTightIsoTrackFinding_*_*', 
        'keep *_hltConvPFTausTightIsoTrackPt5Isolation_*_*', 
        'keep *_hltConvPFTausTightIsoTrackPt5_*_*', 
        'keep *_hltConvPFTausTightIso_*_*', 
        'keep *_hltConvPFTausTrackFindingLooseIsolation_*_*', 
        'keep *_hltConvPFTausTrackFinding_*_*', 
        'keep *_hltConvPFTaus_*_*', 
        'keep *_hltCorrectedIslandBarrelSuperClustersHI_*_*', 
        'keep *_hltCorrectedIslandEndcapSuperClustersHI_*_*', 
        'keep *_hltCsc2DRecHits_*_*', 
        'keep *_hltCscSegments_*_*', 
        'keep *_hltDoublePFTau25TrackPt5MediumIsolationProng4L1HLTMatched_*_*', 
        'keep *_hltDoublePFTau25TrackPt5MediumIsolationProng4_*_*', 
        'keep *_hltDoublePFTau25TrackPt5MediumIsolation_*_*', 
        'keep *_hltDoublePFTau25TrackPt5_*_*', 
        'keep *_hltDoublePFTau25_*_*', 
        'keep *_hltDoublePFTauTightIso45Track5_*_*', 
        'keep *_hltDoublePFTauTightIso45Track_*_*', 
        'keep *_hltDt4DSegments_*_*', 
        'keep *_hltEcalRecHitAll_*_*', 
        'keep *_hltEle20CaloIdVTCaloIsoTTrkIdTTrkIsoTTrackIsoFilterL1SingleEG18orL1SingleEG20_*_*', 
        'keep *_hltEle20CaloIdVTTrkIdTDphiFilter_*_*', 
        'keep *_hltEle27WP85PixelMatchElectronsL1Seeded_*_*', 
        'keep *_hltFastPVPixelTracksMerger_*_*', 
        'keep *_hltFastPVPixelTracksRecover_*_*', 
        'keep *_hltFastPVPixelTracks_*_*', 
        'keep *_hltFastPVPixelVertices3D_*_*', 
        'keep *_hltFastPVPixelVertices_*_*', 
        'keep *_hltFastPixelBLifetimeL3TagInfos_*_*', 
        'keep *_hltFastPrimaryVertex_*_*', 
        'keep *_hltFilterDoubleIsoPFTau45Trk5LeadTrack5IsolationL1HLTMatched_*_*', 
        'keep *_hltFilterL2EtCutDoublePFIsoTau45Trk5_*_*', 
        'keep *_hltFilterL2EtCutSingleIsoPFTau35Trk20MET70_*_*', 
        'keep *_hltFilterSingleIsoPFTau35Trk20LeadTrackPt20_*_*', 
        'keep *_hltFilterSingleIsoPFTau35Trk20MET60LeadTrack20IsolationL1HLTMatched_*_*', 
        'keep *_hltFilterSingleIsoPFTau35Trk20MET70LeadTrack20IsolationL1HLTMatched_*_*', 
        'keep *_hltGoodOnlinePVs_*_*', 
        'keep *_hltHICaloJetCorrected_*_*', 
        'keep *_hltHICaloJetIDPassed_*_*', 
        'keep *_hltHIGoodLooseTracks_*_*', 
        'keep *_hltHIPixel3PrimTracks_*_*', 
        'keep *_hltHISelectedVertex_*_*', 
        'keep *_hltHISiPixelClusters_*_*', 
        'keep *_hltHITIPTCorrectorHB_*_*', 
        'keep *_hltHITIPTCorrectorHE_*_*', 
        'keep *_hltHiCorrectedIslandBarrelSuperClustersHI_*_*', 
        'keep *_hltHiCorrectedIslandEndcapSuperClustersHI_*_*', 
        'keep *_hltHiIslandSuperClustersHI_*_*', 
        'keep *_hltIsolPixelTrackProdHB_*_*', 
        'keep *_hltIsolPixelTrackProdHE_*_*', 
        'keep *_hltIter0PFlowTrackSelectionHighPurity_*_*', 
        'keep *_hltIter1Merged_*_*', 
        'keep *_hltIter1PFlowTrackSelectionHighPurity_*_*', 
        'keep *_hltIter2Merged_*_*', 
        'keep *_hltIter2PFlowTrackSelectionHighPurity_*_*', 
        'keep *_hltIter3Merged_*_*', 
        'keep *_hltIter4Merged_*_*', 
        'keep *_hltIterativeCone5PileupSubtractionCaloJets_*_*', 
        'keep *_hltL1GtObjectMap_*_*', 
        'keep *_hltL1HLTSingleIsoPFTau35Trk20Met60JetsMatch_*_*', 
        'keep *_hltL1IsoElectronTrackIsol_*_*', 
        'keep *_hltL1NonIsoElectronTrackIsol_*_*', 
        'keep *_hltL1SeededRecoEcalCandidate_*_*', 
        'keep *_hltL1extraParticlesCentral_*_*', 
        'keep *_hltL1extraParticlesNonIsolated_*_*', 
        'keep *_hltL1extraParticlesTau_*_*', 
        'keep *_hltL1extraParticles_*_*', 
        'keep *_hltL1sDoubleTauJet44Eta2p17orDoubleJet64Central_*_*', 
        'keep *_hltL1sDoubleTauJet44erorDoubleJetC64_*_*', 
        'keep *_hltL1sL1EG18er_*_*', 
        'keep *_hltL1sL1ETM36ORETM40_*_*', 
        'keep *_hltL1sL1Jet52ETM30_*_*', 
        'keep *_hltL1sL1SingleEG12_*_*', 
        'keep *_hltL1sL1SingleEG15_*_*', 
        'keep *_hltL1sL1SingleEG18orL1SingleEG20_*_*', 
        'keep *_hltL1sL1SingleMu10_*_*', 
        'keep *_hltL1sL1SingleMu14Eta2p1_*_*', 
        'keep *_hltL1sMu16Eta2p1_*_*', 
        'keep *_hltL2MuonCandidatesNoVtx_*_*', 
        'keep *_hltL2MuonCandidates_*_*', 
        'keep *_hltL2MuonSeeds_*_*', 
        'keep *_hltL2Muons_*_*', 
        'keep *_hltL2TauJets_*_*', 
        'keep *_hltL3CombinedSecondaryVertexBJetTags_*_*', 
        'keep *_hltL3MuonCandidates_*_*', 
        'keep *_hltL3MuonsIOHit_*_*', 
        'keep *_hltL3MuonsLinksCombination_*_*', 
        'keep *_hltL3MuonsOIHit_*_*', 
        'keep *_hltL3MuonsOIState_*_*', 
        'keep *_hltL3Muons_*_*', 
        'keep *_hltL3NoFiltersNoVtxMuonCandidates_*_*', 
        'keep *_hltL3NoFiltersNoVtxMuons_*_*', 
        'keep *_hltL3SecondaryVertexTagInfos_*_*', 
        'keep *_hltL3TkFromL2OICombination_*_*', 
        'keep *_hltL3TkTracksFromL2IOHit_*_*', 
        'keep *_hltL3TkTracksFromL2OIHit_*_*', 
        'keep *_hltL3TkTracksFromL2OIState_*_*', 
        'keep *_hltL3TkTracksFromL2_*_*', 
        'keep *_hltL3TrackCandidateFromL2IOHit_*_*', 
        'keep *_hltL3TrackCandidateFromL2OIHit_*_*', 
        'keep *_hltL3TrackCandidateFromL2OIState_*_*', 
        'keep *_hltL3TrajSeedIOHit_*_*', 
        'keep *_hltL3TrajSeedOIHit_*_*', 
        'keep *_hltL3TrajSeedOIState_*_*', 
        'keep *_hltL3TrajectorySeed_*_*', 
        'keep *_hltL3crIsoL1sMu16Eta2p1L1f0L2f16QL3f18QL3crIsoRhoFiltered0p15_*_*', 
        'keep *_hltLeadingCentralJets30SingleTopEle27_*_*', 
        'keep *_hltLeadingCentralJets30SingleTopIsoMu24_*_*', 
        'keep *_hltMetForHf_*_*', 
        'keep *_hltMet_*_*', 
        'keep *_hltMu8Ele17CaloIdTCaloIsoVLPixelMatchFilter_*_*', 
        'keep *_hltMuTrackJpsiCtfTrackCands_*_*', 
        'keep *_hltMuTrackJpsiPixelTrackCands_*_*', 
        'keep *_hltMuonCSCDigis_*_*', 
        'keep *_hltMuonCSCDigis_MuonCSCStripDigi_*', 
        'keep *_hltMuonCSCDigis_MuonCSCWireDigi_*', 
        'keep *_hltMuonDTDigis_*_*', 
        'keep *_hltMuonRPCDigis_*_*', 
        'keep *_hltOnlineBeamSpot_*_*', 
        'keep *_hltOnlinePrimaryVertices_*_*', 
        'keep *_hltOverlapFilterEle20LooseIsoPFTau20OldVersion_*_*', 
        'keep *_hltOverlapFilterIsoEle20MediumIsoPFTau20_*_*', 
        'keep *_hltOverlapFilterIsoMu15IsoPFTau20_*_*', 
        'keep *_hltOverlapFilterIsoMu15MediumIsoPFTau20_*_*', 
        'keep *_hltOverlapFilterIsoMu15TightIsoPFTau20_*_*', 
        'keep *_hltOverlapFilterIsoMu18LooseIsoPFTau20_*_*', 
        'keep *_hltOverlapFilterIsoMu18PFTau25TrackPt5Prong4_*_*', 
        'keep *_hltPFTau15TrackLooseIso_*_*', 
        'keep *_hltPFTau15Track_*_*', 
        'keep *_hltPFTau15_*_*', 
        'keep *_hltPFTau20IsoMuVertex_*_*', 
        'keep *_hltPFTau20TrackLooseIso_*_*', 
        'keep *_hltPFTau20Track_*_*', 
        'keep *_hltPFTau20_*_*', 
        'keep *_hltPFTau25TrackPt5MediumIsolationProng4IsoMuVertex_*_*', 
        'keep *_hltPFTau25TrackPt5MediumIsolationProng4_*_*', 
        'keep *_hltPFTau25TrackPt5MediumIsolation_*_*', 
        'keep *_hltPFTau25TrackPt5_*_*', 
        'keep *_hltPFTau25_*_*', 
        'keep *_hltPFTau35TrackPt20LooseIsoProng2_*_*', 
        'keep *_hltPFTau35TrackPt20LooseIso_*_*', 
        'keep *_hltPFTau35TrackPt20_*_*', 
        'keep *_hltPFTau35Track_*_*', 
        'keep *_hltPFTau35_*_*', 
        'keep *_hltPFTauEleVertex20_*_*', 
        'keep *_hltPFTauJetTracksAssociator_*_*', 
        'keep *_hltPFTauMediumIso20TrackMediumIso_*_*', 
        'keep *_hltPFTauMediumIso20Track_*_*', 
        'keep *_hltPFTauMediumIso20_*_*', 
        'keep *_hltPFTauMediumIso35Track_*_*', 
        'keep *_hltPFTauMediumIso35_*_*', 
        'keep *_hltPFTauTagInfo_*_*', 
        'keep *_hltPFTauTightIso20TrackTightIso_*_*', 
        'keep *_hltPFTauTightIso20Track_*_*', 
        'keep *_hltPFTauTightIso20_*_*', 
        'keep *_hltPFlowTrackSelectionHighPurity_*_*', 
        'keep *_hltParticleFlowForTaus_*_*', 
        'keep *_hltParticleFlow_*_*', 
        'keep *_hltPixelMatch3HitElectronsActivity_*_*', 
        'keep *_hltPixelMatch3HitElectronsL1Seeded_*_*', 
        'keep *_hltPixelMatchCleanElectronsL1Seeded_*_*', 
        'keep *_hltPixelMatchElectronsActivity_*_*', 
        'keep *_hltPixelMatchElectronsL1Iso_*_*', 
        'keep *_hltPixelMatchElectronsL1NonIso_*_*', 
        'keep *_hltPixelMatchElectronsL1Seeded_*_*', 
        'keep *_hltPixelTracks_*_*', 
        'keep *_hltPixelVertices3DbbPhi_*_*', 
        'keep *_hltPixelVertices_*_*', 
        'keep *_hltRecoEcalSuperClusterActivityCandidateSC4_*_*', 
        'keep *_hltRecoEcalSuperClusterActivityCandidateSC5_*_*', 
        'keep *_hltRecoEcalSuperClusterActivityCandidate_*_*', 
        'keep *_hltRpcRecHits_*_*', 
        'keep *_hltSelector4CentralJetsL1FastJet_*_*', 
        'keep *_hltSelectorJets20L1FastJet_*_*', 
        'keep *_hltSiPixelCluster_*_*', 
        'keep *_hltSiPixelClusters_*_*', 
        'keep *_hltSiStripClusters_*_*', 
        'keep *_hltSiStripRawToClustersFacility_*_*', 
        'keep *_hltSingleMu15L3Filtered15_*_*', 
        'keep *_hltSingleMuIsoL1s14L3IsoFiltered15eta2p1_*_*', 
        'keep *_hltSingleMuIsoL3IsoFiltered15_*_*', 
        'keep *_hltTowerMakerForAll_*_*', 
        'keep *_hltTowerMakerForMuons_*_*', 
        'keep *_hltTriggerSummaryAOD_*_*', 
        'keep *_hltTriggerSummaryRAW_*_*', 
        'keep *_hltTrimmedPixelVertices_*_*', 
        'keep *_hltVerticesL3_*_*', 
        'keep DcsStatuss_hltScalersRawToDigi_*_*', 
        'keep FEDRawDataCollection_rawDataCollector_*_*', 
        'keep FEDRawDataCollection_rawDataRepacker_*_*', 
        'keep FEDRawDataCollection_source_*_*', 
        'keep FEDRawDataCollection_virginRawDataRepacker_*_*', 
        'keep L1GlobalTriggerReadoutRecord_hltGtDigis_*_*', 
        'keep L1MuGMTCands_hltGtDigis_*_*', 
        'keep L1MuGMTReadoutCollection_hltGtDigis_*_*', 
        'keep L2MuonTrajectorySeeds_hltL2MuonSeeds_*_*', 
        'keep L3MuonTrajectorySeeds_hltHIL3TrajSeedOIHit_*_*', 
        'keep L3MuonTrajectorySeeds_hltHIL3TrajectorySeed_*_*', 
        'keep L3MuonTrajectorySeeds_hltL3TrajSeedOIState_*_*', 
        'keep LumiScalerss_hltScalersRawToDigi_*_*', 
        'keep SiPixelClusteredmNewDetSetVector_hltSiPixelClusters_*_*', 
        'keep TrackCandidates_hltHIL3TrackCandidateFromL2OIHit_*_*', 
        'keep TrackCandidates_hltHIL3TrackCandidateFromL2OIState_*_*', 
        'keep TrackingRecHitsOwned_hltL3Muons_*_*', 
        'keep edmTriggerResults_*_*_*', 
        'keep recoCaloJets_*_*_*', 
        'keep recoCaloMETs_*_*_*', 
        'keep recoCaloMETs_hltMet_*_*', 
        'keep recoCompositeCandidates_*_*_*', 
        'keep recoElectrons_*_*_*', 
        'keep recoIsolatedPixelTrackCandidates_*_*_*', 
        'keep recoMETs_*_*_*', 
        'keep recoPFJets_*_*_*', 
        'keep recoPFTaus_*_*_*', 
        'keep recoRecoChargedCandidates_*_*_*', 
        'keep recoRecoChargedCandidates_hltHIL3MuonCandidates_*_*', 
        'keep recoRecoChargedCandidates_hltL2MuonCandidates_*_*', 
        'keep recoRecoEcalCandidatesToValuefloatAssociation_hltL1IsoHLTClusterShape_*_*', 
        'keep recoRecoEcalCandidatesToValuefloatAssociation_hltL1IsolatedPhotonEcalIsol_*_*', 
        'keep recoRecoEcalCandidatesToValuefloatAssociation_hltL1IsolatedPhotonHcalForHE_*_*', 
        'keep recoRecoEcalCandidatesToValuefloatAssociation_hltL1IsolatedPhotonHcalIsol_*_*', 
        'keep recoRecoEcalCandidatesToValuefloatAssociation_hltL1NonIsoHLTClusterShape_*_*', 
        'keep recoRecoEcalCandidatesToValuefloatAssociation_hltL1NonIsolatedPhotonEcalIsol_*_*', 
        'keep recoRecoEcalCandidatesToValuefloatAssociation_hltL1NonIsolatedPhotonHcalForHE_*_*', 
        'keep recoRecoEcalCandidatesToValuefloatAssociation_hltL1NonIsolatedPhotonHcalIsol_*_*', 
        'keep recoRecoEcalCandidates_*_*_*', 
        'keep recoRecoEcalCandidates_hltL1IsoRecoEcalCandidate_*_*', 
        'keep recoRecoEcalCandidates_hltL1NonIsoRecoEcalCandidate_*_*', 
        'keep recoTrackExtras_hltHIL3MuonsOIHit_*_*', 
        'keep recoTrackExtras_hltHIL3MuonsOIState_*_*', 
        'keep recoTracks_hltHIL3MuonsOIHit_*_*', 
        'keep recoTracks_hltHIL3MuonsOIState_*_*', 
        'keep recoTracks_hltHIL3Muons_*_*', 
        'keep recoTracks_hltHIL3TkTracksFromL2OIHit_*_*', 
        'keep recoTracks_hltHIL3TkTracksFromL2OIState_*_*', 
        'keep recoTracks_hltHIL3TkTracksFromL2_*_*', 
        'keep triggerTriggerEventWithRefs_*_*_*', 
        'keep triggerTriggerEvent_*_*_*', 
        'keep triggerTriggerFilterObjectWithRefs_*_*_*' ) )
)

process.HLTDebugRAW = cms.PSet(
    outputCommands = cms.vstring( ('drop *_hlt*_*_*', 
        'keep *_hltAK4CaloJetsCorrectedIDPassed_*_*', 
        'keep *_hltAK4CaloJetsIDPassed_*_*', 
        'keep *_hltAK4CaloJets_*_*', 
        'keep *_hltAK4PFJetsCorrected_*_*', 
        'keep *_hltAK4PFJetsForTaus_*_*', 
        'keep *_hltAK4PFJets_*_*', 
        'keep *_hltAlCaEtaEBUncalibrator_*_*', 
        'keep *_hltAlCaEtaEEUncalibrator_*_*', 
        'keep *_hltAlCaEtaRecHitsFilterEBonly_*_*', 
        'keep *_hltAlCaEtaRecHitsFilterEEonly_*_*', 
        'keep *_hltAlCaEtaRecHitsFilterEEonly_etaEcalRecHitsES_*', 
        'keep *_hltAlCaEtaRecHitsFilter_*_*', 
        'keep *_hltAlCaPhiSymStream_*_*', 
        'keep *_hltAlCaPhiSymUncalibrator_*_*', 
        'keep *_hltAlCaPi0EBUncalibrator_*_*', 
        'keep *_hltAlCaPi0EEUncalibrator_*_*', 
        'keep *_hltAlCaPi0RecHitsFilterEBonly_*_*', 
        'keep *_hltAlCaPi0RecHitsFilterEEonly_*_*', 
        'keep *_hltAlCaPi0RecHitsFilterEEonly_pi0EcalRecHitsES_*', 
        'keep *_hltAlCaPi0RecHitsFilter_*_*', 
        'keep *_hltBLifetimeL25AssociatorbbPhiL1FastJetFastPV_*_*', 
        'keep *_hltBLifetimeL25BJetTagsbbPhiL1FastJetFastPV_*_*', 
        'keep *_hltBLifetimeL25TagInfosbbPhiL1FastJetFastPV_*_*', 
        'keep *_hltBLifetimeL3AssociatorbbPhiL1FastJetFastPV_*_*', 
        'keep *_hltBLifetimeL3BJetTagsbbPhiL1FastJetFastPV_*_*', 
        'keep *_hltBLifetimeL3TagInfosbbPhiL1FastJetFastPV_*_*', 
        'keep *_hltBLifetimeRegionalCtfWithMaterialTracksbbPhiL1FastJetFastPV_*_*', 
        'keep *_hltBSoftMuonDiJet110Mu5L3FilterByDR_*_*', 
        'keep *_hltBSoftMuonDiJet110Mu5SelL3BJetTagsByDR_*_*', 
        'keep *_hltBSoftMuonDiJet20Mu5L3FilterByDR_*_*', 
        'keep *_hltBSoftMuonDiJet20Mu5SelL3BJetTagsByDR_*_*', 
        'keep *_hltBSoftMuonDiJet40Mu5L3FilterByDR_*_*', 
        'keep *_hltBSoftMuonDiJet40Mu5SelL3BJetTagsByDR_*_*', 
        'keep *_hltBSoftMuonDiJet70Mu5L3FilterByDR_*_*', 
        'keep *_hltBSoftMuonDiJet70Mu5SelL3BJetTagsByDR_*_*', 
        'keep *_hltBSoftMuonMu5L3_*_*', 
        'keep *_hltCSVJetTagSingleTopEle27_*_*', 
        'keep *_hltCSVJetTagSingleTopIsoMu24_*_*', 
        'keep *_hltCaloJetCorrectedRegional_*_*', 
        'keep *_hltCaloJetCorrected_*_*', 
        'keep *_hltCaloJetL1FastJetCorrected_*_*', 
        'keep *_hltCleanedCorrectedIslandBarrelSuperClustersHI_*_*', 
        'keep *_hltCleanedHiCorrectedIslandBarrelSuperClustersHI_*_*', 
        'keep *_hltConvPFTausTightIsoTrackFindingIsolation_*_*', 
        'keep *_hltConvPFTausTightIsoTrackFinding_*_*', 
        'keep *_hltConvPFTausTightIsoTrackPt5Isolation_*_*', 
        'keep *_hltConvPFTausTightIsoTrackPt5_*_*', 
        'keep *_hltConvPFTausTightIso_*_*', 
        'keep *_hltConvPFTausTrackFindingLooseIsolation_*_*', 
        'keep *_hltConvPFTausTrackFinding_*_*', 
        'keep *_hltConvPFTaus_*_*', 
        'keep *_hltCorrectedIslandBarrelSuperClustersHI_*_*', 
        'keep *_hltCorrectedIslandEndcapSuperClustersHI_*_*', 
        'keep *_hltCsc2DRecHits_*_*', 
        'keep *_hltCscSegments_*_*', 
        'keep *_hltDoublePFTau25TrackPt5MediumIsolationProng4L1HLTMatched_*_*', 
        'keep *_hltDoublePFTau25TrackPt5MediumIsolationProng4_*_*', 
        'keep *_hltDoublePFTau25TrackPt5MediumIsolation_*_*', 
        'keep *_hltDoublePFTau25TrackPt5_*_*', 
        'keep *_hltDoublePFTau25_*_*', 
        'keep *_hltDoublePFTauTightIso45Track5_*_*', 
        'keep *_hltDoublePFTauTightIso45Track_*_*', 
        'keep *_hltDt4DSegments_*_*', 
        'keep *_hltEcalRecHitAll_*_*', 
        'keep *_hltEle20CaloIdVTCaloIsoTTrkIdTTrkIsoTTrackIsoFilterL1SingleEG18orL1SingleEG20_*_*', 
        'keep *_hltEle20CaloIdVTTrkIdTDphiFilter_*_*', 
        'keep *_hltEle27WP85PixelMatchElectronsL1Seeded_*_*', 
        'keep *_hltFastPVPixelTracksMerger_*_*', 
        'keep *_hltFastPVPixelTracksRecover_*_*', 
        'keep *_hltFastPVPixelTracks_*_*', 
        'keep *_hltFastPVPixelVertices3D_*_*', 
        'keep *_hltFastPVPixelVertices_*_*', 
        'keep *_hltFastPixelBLifetimeL3TagInfos_*_*', 
        'keep *_hltFastPrimaryVertex_*_*', 
        'keep *_hltFilterDoubleIsoPFTau45Trk5LeadTrack5IsolationL1HLTMatched_*_*', 
        'keep *_hltFilterL2EtCutDoublePFIsoTau45Trk5_*_*', 
        'keep *_hltFilterL2EtCutSingleIsoPFTau35Trk20MET70_*_*', 
        'keep *_hltFilterSingleIsoPFTau35Trk20LeadTrackPt20_*_*', 
        'keep *_hltFilterSingleIsoPFTau35Trk20MET60LeadTrack20IsolationL1HLTMatched_*_*', 
        'keep *_hltFilterSingleIsoPFTau35Trk20MET70LeadTrack20IsolationL1HLTMatched_*_*', 
        'keep *_hltGoodOnlinePVs_*_*', 
        'keep *_hltHICaloJetCorrected_*_*', 
        'keep *_hltHICaloJetIDPassed_*_*', 
        'keep *_hltHIGoodLooseTracks_*_*', 
        'keep *_hltHIPixel3PrimTracks_*_*', 
        'keep *_hltHISelectedVertex_*_*', 
        'keep *_hltHISiPixelClusters_*_*', 
        'keep *_hltHITIPTCorrectorHB_*_*', 
        'keep *_hltHITIPTCorrectorHE_*_*', 
        'keep *_hltHiCorrectedIslandBarrelSuperClustersHI_*_*', 
        'keep *_hltHiCorrectedIslandEndcapSuperClustersHI_*_*', 
        'keep *_hltHiIslandSuperClustersHI_*_*', 
        'keep *_hltIsolPixelTrackProdHB_*_*', 
        'keep *_hltIsolPixelTrackProdHE_*_*', 
        'keep *_hltIter0PFlowTrackSelectionHighPurity_*_*', 
        'keep *_hltIter1Merged_*_*', 
        'keep *_hltIter1PFlowTrackSelectionHighPurity_*_*', 
        'keep *_hltIter2Merged_*_*', 
        'keep *_hltIter2PFlowTrackSelectionHighPurity_*_*', 
        'keep *_hltIter3Merged_*_*', 
        'keep *_hltIter4Merged_*_*', 
        'keep *_hltIterativeCone5PileupSubtractionCaloJets_*_*', 
        'keep *_hltL1GtObjectMap_*_*', 
        'keep *_hltL1HLTSingleIsoPFTau35Trk20Met60JetsMatch_*_*', 
        'keep *_hltL1IsoElectronTrackIsol_*_*', 
        'keep *_hltL1NonIsoElectronTrackIsol_*_*', 
        'keep *_hltL1SeededRecoEcalCandidate_*_*', 
        'keep *_hltL1extraParticlesCentral_*_*', 
        'keep *_hltL1extraParticlesNonIsolated_*_*', 
        'keep *_hltL1extraParticlesTau_*_*', 
        'keep *_hltL1extraParticles_*_*', 
        'keep *_hltL1sDoubleTauJet44Eta2p17orDoubleJet64Central_*_*', 
        'keep *_hltL1sDoubleTauJet44erorDoubleJetC64_*_*', 
        'keep *_hltL1sL1EG18er_*_*', 
        'keep *_hltL1sL1ETM36ORETM40_*_*', 
        'keep *_hltL1sL1Jet52ETM30_*_*', 
        'keep *_hltL1sL1SingleEG12_*_*', 
        'keep *_hltL1sL1SingleEG15_*_*', 
        'keep *_hltL1sL1SingleEG18orL1SingleEG20_*_*', 
        'keep *_hltL1sL1SingleMu10_*_*', 
        'keep *_hltL1sL1SingleMu14Eta2p1_*_*', 
        'keep *_hltL1sMu16Eta2p1_*_*', 
        'keep *_hltL2MuonCandidatesNoVtx_*_*', 
        'keep *_hltL2MuonCandidates_*_*', 
        'keep *_hltL2MuonSeeds_*_*', 
        'keep *_hltL2Muons_*_*', 
        'keep *_hltL2TauJets_*_*', 
        'keep *_hltL3CombinedSecondaryVertexBJetTags_*_*', 
        'keep *_hltL3MuonCandidates_*_*', 
        'keep *_hltL3MuonsIOHit_*_*', 
        'keep *_hltL3MuonsLinksCombination_*_*', 
        'keep *_hltL3MuonsOIHit_*_*', 
        'keep *_hltL3MuonsOIState_*_*', 
        'keep *_hltL3Muons_*_*', 
        'keep *_hltL3NoFiltersNoVtxMuonCandidates_*_*', 
        'keep *_hltL3NoFiltersNoVtxMuons_*_*', 
        'keep *_hltL3SecondaryVertexTagInfos_*_*', 
        'keep *_hltL3TkFromL2OICombination_*_*', 
        'keep *_hltL3TkTracksFromL2IOHit_*_*', 
        'keep *_hltL3TkTracksFromL2OIHit_*_*', 
        'keep *_hltL3TkTracksFromL2OIState_*_*', 
        'keep *_hltL3TkTracksFromL2_*_*', 
        'keep *_hltL3TrackCandidateFromL2IOHit_*_*', 
        'keep *_hltL3TrackCandidateFromL2OIHit_*_*', 
        'keep *_hltL3TrackCandidateFromL2OIState_*_*', 
        'keep *_hltL3TrajSeedIOHit_*_*', 
        'keep *_hltL3TrajSeedOIHit_*_*', 
        'keep *_hltL3TrajSeedOIState_*_*', 
        'keep *_hltL3TrajectorySeed_*_*', 
        'keep *_hltL3crIsoL1sMu16Eta2p1L1f0L2f16QL3f18QL3crIsoRhoFiltered0p15_*_*', 
        'keep *_hltLeadingCentralJets30SingleTopEle27_*_*', 
        'keep *_hltLeadingCentralJets30SingleTopIsoMu24_*_*', 
        'keep *_hltMetForHf_*_*', 
        'keep *_hltMet_*_*', 
        'keep *_hltMu8Ele17CaloIdTCaloIsoVLPixelMatchFilter_*_*', 
        'keep *_hltMuTrackJpsiCtfTrackCands_*_*', 
        'keep *_hltMuTrackJpsiPixelTrackCands_*_*', 
        'keep *_hltMuonCSCDigis_*_*', 
        'keep *_hltMuonCSCDigis_MuonCSCStripDigi_*', 
        'keep *_hltMuonCSCDigis_MuonCSCWireDigi_*', 
        'keep *_hltMuonDTDigis_*_*', 
        'keep *_hltMuonRPCDigis_*_*', 
        'keep *_hltOnlineBeamSpot_*_*', 
        'keep *_hltOnlinePrimaryVertices_*_*', 
        'keep *_hltOverlapFilterEle20LooseIsoPFTau20OldVersion_*_*', 
        'keep *_hltOverlapFilterIsoEle20MediumIsoPFTau20_*_*', 
        'keep *_hltOverlapFilterIsoMu15IsoPFTau20_*_*', 
        'keep *_hltOverlapFilterIsoMu15MediumIsoPFTau20_*_*', 
        'keep *_hltOverlapFilterIsoMu15TightIsoPFTau20_*_*', 
        'keep *_hltOverlapFilterIsoMu18LooseIsoPFTau20_*_*', 
        'keep *_hltOverlapFilterIsoMu18PFTau25TrackPt5Prong4_*_*', 
        'keep *_hltPFTau15TrackLooseIso_*_*', 
        'keep *_hltPFTau15Track_*_*', 
        'keep *_hltPFTau15_*_*', 
        'keep *_hltPFTau20IsoMuVertex_*_*', 
        'keep *_hltPFTau20TrackLooseIso_*_*', 
        'keep *_hltPFTau20Track_*_*', 
        'keep *_hltPFTau20_*_*', 
        'keep *_hltPFTau25TrackPt5MediumIsolationProng4IsoMuVertex_*_*', 
        'keep *_hltPFTau25TrackPt5MediumIsolationProng4_*_*', 
        'keep *_hltPFTau25TrackPt5MediumIsolation_*_*', 
        'keep *_hltPFTau25TrackPt5_*_*', 
        'keep *_hltPFTau25_*_*', 
        'keep *_hltPFTau35TrackPt20LooseIsoProng2_*_*', 
        'keep *_hltPFTau35TrackPt20LooseIso_*_*', 
        'keep *_hltPFTau35TrackPt20_*_*', 
        'keep *_hltPFTau35Track_*_*', 
        'keep *_hltPFTau35_*_*', 
        'keep *_hltPFTauEleVertex20_*_*', 
        'keep *_hltPFTauJetTracksAssociator_*_*', 
        'keep *_hltPFTauMediumIso20TrackMediumIso_*_*', 
        'keep *_hltPFTauMediumIso20Track_*_*', 
        'keep *_hltPFTauMediumIso20_*_*', 
        'keep *_hltPFTauMediumIso35Track_*_*', 
        'keep *_hltPFTauMediumIso35_*_*', 
        'keep *_hltPFTauTagInfo_*_*', 
        'keep *_hltPFTauTightIso20TrackTightIso_*_*', 
        'keep *_hltPFTauTightIso20Track_*_*', 
        'keep *_hltPFTauTightIso20_*_*', 
        'keep *_hltPFlowTrackSelectionHighPurity_*_*', 
        'keep *_hltParticleFlowForTaus_*_*', 
        'keep *_hltParticleFlow_*_*', 
        'keep *_hltPixelMatch3HitElectronsActivity_*_*', 
        'keep *_hltPixelMatch3HitElectronsL1Seeded_*_*', 
        'keep *_hltPixelMatchCleanElectronsL1Seeded_*_*', 
        'keep *_hltPixelMatchElectronsActivity_*_*', 
        'keep *_hltPixelMatchElectronsL1Iso_*_*', 
        'keep *_hltPixelMatchElectronsL1NonIso_*_*', 
        'keep *_hltPixelMatchElectronsL1Seeded_*_*', 
        'keep *_hltPixelTracks_*_*', 
        'keep *_hltPixelVertices3DbbPhi_*_*', 
        'keep *_hltPixelVertices_*_*', 
        'keep *_hltRecoEcalSuperClusterActivityCandidateSC4_*_*', 
        'keep *_hltRecoEcalSuperClusterActivityCandidateSC5_*_*', 
        'keep *_hltRecoEcalSuperClusterActivityCandidate_*_*', 
        'keep *_hltRpcRecHits_*_*', 
        'keep *_hltSelector4CentralJetsL1FastJet_*_*', 
        'keep *_hltSelectorJets20L1FastJet_*_*', 
        'keep *_hltSiPixelCluster_*_*', 
        'keep *_hltSiPixelClusters_*_*', 
        'keep *_hltSiStripClusters_*_*', 
        'keep *_hltSiStripRawToClustersFacility_*_*', 
        'keep *_hltSingleMu15L3Filtered15_*_*', 
        'keep *_hltSingleMuIsoL1s14L3IsoFiltered15eta2p1_*_*', 
        'keep *_hltSingleMuIsoL3IsoFiltered15_*_*', 
        'keep *_hltTowerMakerForAll_*_*', 
        'keep *_hltTowerMakerForMuons_*_*', 
        'keep *_hltTriggerSummaryAOD_*_*', 
        'keep *_hltTriggerSummaryRAW_*_*', 
        'keep *_hltTrimmedPixelVertices_*_*', 
        'keep *_hltVerticesL3_*_*', 
        'keep DcsStatuss_hltScalersRawToDigi_*_*', 
        'keep FEDRawDataCollection_rawDataCollector_*_*', 
        'keep FEDRawDataCollection_rawDataRepacker_*_*', 
        'keep FEDRawDataCollection_source_*_*', 
        'keep FEDRawDataCollection_virginRawDataRepacker_*_*', 
        'keep L1GlobalTriggerReadoutRecord_hltGtDigis_*_*', 
        'keep L1MuGMTCands_hltGtDigis_*_*', 
        'keep L1MuGMTReadoutCollection_hltGtDigis_*_*', 
        'keep L2MuonTrajectorySeeds_hltL2MuonSeeds_*_*', 
        'keep L3MuonTrajectorySeeds_hltHIL3TrajSeedOIHit_*_*', 
        'keep L3MuonTrajectorySeeds_hltHIL3TrajectorySeed_*_*', 
        'keep L3MuonTrajectorySeeds_hltL3TrajSeedOIState_*_*', 
        'keep LumiScalerss_hltScalersRawToDigi_*_*', 
        'keep SiPixelClusteredmNewDetSetVector_hltSiPixelClusters_*_*', 
        'keep TrackCandidates_hltHIL3TrackCandidateFromL2OIHit_*_*', 
        'keep TrackCandidates_hltHIL3TrackCandidateFromL2OIState_*_*', 
        'keep TrackingRecHitsOwned_hltL3Muons_*_*', 
        'keep edmTriggerResults_*_*_*', 
        'keep recoCaloJets_*_*_*', 
        'keep recoCaloMETs_*_*_*', 
        'keep recoCaloMETs_hltMet_*_*', 
        'keep recoCompositeCandidates_*_*_*', 
        'keep recoElectrons_*_*_*', 
        'keep recoIsolatedPixelTrackCandidates_*_*_*', 
        'keep recoMETs_*_*_*', 
        'keep recoPFJets_*_*_*', 
        'keep recoPFTaus_*_*_*', 
        'keep recoRecoChargedCandidates_*_*_*', 
        'keep recoRecoChargedCandidates_hltHIL3MuonCandidates_*_*', 
        'keep recoRecoChargedCandidates_hltL2MuonCandidates_*_*', 
        'keep recoRecoEcalCandidatesToValuefloatAssociation_hltL1IsoHLTClusterShape_*_*', 
        'keep recoRecoEcalCandidatesToValuefloatAssociation_hltL1IsolatedPhotonEcalIsol_*_*', 
        'keep recoRecoEcalCandidatesToValuefloatAssociation_hltL1IsolatedPhotonHcalForHE_*_*', 
        'keep recoRecoEcalCandidatesToValuefloatAssociation_hltL1IsolatedPhotonHcalIsol_*_*', 
        'keep recoRecoEcalCandidatesToValuefloatAssociation_hltL1NonIsoHLTClusterShape_*_*', 
        'keep recoRecoEcalCandidatesToValuefloatAssociation_hltL1NonIsolatedPhotonEcalIsol_*_*', 
        'keep recoRecoEcalCandidatesToValuefloatAssociation_hltL1NonIsolatedPhotonHcalForHE_*_*', 
        'keep recoRecoEcalCandidatesToValuefloatAssociation_hltL1NonIsolatedPhotonHcalIsol_*_*', 
        'keep recoRecoEcalCandidates_*_*_*', 
        'keep recoRecoEcalCandidates_hltL1IsoRecoEcalCandidate_*_*', 
        'keep recoRecoEcalCandidates_hltL1NonIsoRecoEcalCandidate_*_*', 
        'keep recoTrackExtras_hltHIL3MuonsOIHit_*_*', 
        'keep recoTrackExtras_hltHIL3MuonsOIState_*_*', 
        'keep recoTracks_hltHIL3MuonsOIHit_*_*', 
        'keep recoTracks_hltHIL3MuonsOIState_*_*', 
        'keep recoTracks_hltHIL3Muons_*_*', 
        'keep recoTracks_hltHIL3TkTracksFromL2OIHit_*_*', 
        'keep recoTracks_hltHIL3TkTracksFromL2OIState_*_*', 
        'keep recoTracks_hltHIL3TkTracksFromL2_*_*', 
        'keep triggerTriggerEventWithRefs_*_*_*', 
        'keep triggerTriggerEvent_*_*_*', 
        'keep triggerTriggerFilterObjectWithRefs_*_*_*' ) )
)

process.HLTriggerAOD = cms.PSet(
    outputCommands = cms.vstring('drop *_hlt*_*_*', 
        'keep *_hltL1GtObjectMap_*_*', 
        'drop L1GlobalTriggerObjectMapRecord_hltL1GtObjectMap_*_*', 
        'keep edmTriggerResults_*_*_*', 
        'keep triggerTriggerEvent_*_*_*')
)

process.HLTriggerRAW = cms.PSet(
    outputCommands = cms.vstring('drop *_hlt*_*_*', 
        'keep *_hltL1GtObjectMap_*_*', 
        'keep FEDRawDataCollection_rawDataCollector_*_*', 
        'keep FEDRawDataCollection_source_*_*', 
        'keep edmTriggerResults_*_*_*', 
        'keep triggerTriggerEvent_*_*_*')
)

process.HLTriggerRECO = cms.PSet(
    outputCommands = cms.vstring('drop *_hlt*_*_*', 
        'keep *_hltL1GtObjectMap_*_*', 
        'keep edmTriggerResults_*_*_*', 
        'keep triggerTriggerEvent_*_*_*')
)

process.HcalReLabel = cms.PSet(
    RelabelRules = cms.untracked.PSet(
        Eta16 = cms.untracked.vint32(1, 1, 2, 2, 2, 
            2, 2, 2, 2, 3, 
            3, 3, 3, 3, 3, 
            3, 3, 3, 3),
        Eta17 = cms.untracked.vint32(1, 1, 2, 2, 3, 
            3, 3, 4, 4, 4, 
            4, 4, 5, 5, 5, 
            5, 5, 5, 5),
        Eta1 = cms.untracked.vint32(1, 2, 2, 2, 3, 
            3, 3, 3, 3, 3, 
            3, 3, 3, 3, 3, 
            3, 3, 3, 3),
        CorrectPhi = cms.untracked.bool(False)
    ),
    RelabelHits = cms.untracked.bool(False)
)

process.IOMCRAW = cms.PSet(
    outputCommands = cms.untracked.vstring('keep *_randomEngineStateProducer_*_*')
)

process.L1TriggerAOD = cms.PSet(
    outputCommands = cms.untracked.vstring('keep L1GlobalTriggerReadoutRecord_gtDigis_*_*', 
        'keep *_l1GtRecord_*_*', 
        'keep *_l1GtTriggerMenuLite_*_*', 
        'keep *_conditionsInEdm_*_*', 
        'keep *_l1extraParticles_*_*', 
        'keep *_l1L1GtObjectMap_*_*', 
        'keep LumiSummary_lumiProducer_*_*')
)

process.L1TriggerFEVTDEBUG = cms.PSet(
    outputCommands = cms.untracked.vstring('keep *_simCscTriggerPrimitiveDigis_*_*', 
        'keep *_simDtTriggerPrimitiveDigis_*_*', 
        'keep *_simRpcTriggerDigis_*_*', 
        'keep *_simRctDigis_*_*', 
        'keep *_simCsctfDigis_*_*', 
        'keep *_simCsctfTrackDigis_*_*', 
        'keep *_simDttfDigis_*_*', 
        'keep *_simGctDigis_*_*', 
        'keep *_simGmtDigis_*_*', 
        'keep *_simGtDigis_*_*', 
        'keep *_cscTriggerPrimitiveDigis_*_*', 
        'keep *_dtTriggerPrimitiveDigis_*_*', 
        'keep *_rpcTriggerDigis_*_*', 
        'keep *_rctDigis_*_*', 
        'keep *_csctfDigis_*_*', 
        'keep *_csctfTrackDigis_*_*', 
        'keep *_dttfDigis_*_*', 
        'keep *_gctDigis_*_*', 
        'keep *_gmtDigis_*_*', 
        'keep *_gtDigis_*_*', 
        'keep *_gtEvmDigis_*_*', 
        'keep *_l1GtRecord_*_*', 
        'keep *_l1GtTriggerMenuLite_*_*', 
        'keep *_conditionsInEdm_*_*', 
        'keep *_l1extraParticles_*_*', 
        'keep *_l1L1GtObjectMap_*_*', 
        'keep LumiDetails_lumiProducer_*_*', 
        'keep LumiSummary_lumiProducer_*_*')
)

process.L1TriggerRAW = cms.PSet(
    outputCommands = cms.untracked.vstring('keep  FEDRawDataCollection_rawDataCollector_*_*', 
        'keep  FEDRawDataCollection_source_*_*')
)

process.L1TriggerRAWDEBUG = cms.PSet(
    outputCommands = cms.untracked.vstring('keep  FEDRawDataCollection_rawDataCollector_*_*', 
        'keep  FEDRawDataCollection_source_*_*', 
        'keep L1GlobalTriggerReadoutRecord_gtDigis_*_*', 
        'keep *_l1GtRecord_*_*', 
        'keep *_l1GtTriggerMenuLite_*_*', 
        'keep *_conditionsInEdm_*_*', 
        'keep *_l1extraParticles_*_*')
)

process.L1TriggerRECO = cms.PSet(
    outputCommands = cms.untracked.vstring('keep L1GlobalTriggerReadoutRecord_gtDigis_*_*', 
        'keep *_l1GtRecord_*_*', 
        'keep *_l1GtTriggerMenuLite_*_*', 
        'keep *_conditionsInEdm_*_*', 
        'keep *_l1extraParticles_*_*', 
        'keep *_l1L1GtObjectMap_*_*', 
        'keep L1MuGMTReadoutCollection_gtDigis_*_*', 
        'keep L1GctEmCand*_gctDigis_*_*', 
        'keep L1GctJetCand*_gctDigis_*_*', 
        'keep L1GctEtHad*_gctDigis_*_*', 
        'keep L1GctEtMiss*_gctDigis_*_*', 
        'keep L1GctEtTotal*_gctDigis_*_*', 
        'keep L1GctHtMiss*_gctDigis_*_*', 
        'keep L1GctJetCounts*_gctDigis_*_*', 
        'keep L1GctHFRingEtSums*_gctDigis_*_*', 
        'keep L1GctHFBitCounts*_gctDigis_*_*', 
        'keep LumiDetails_lumiProducer_*_*', 
        'keep LumiSummary_lumiProducer_*_*')
)

process.LHEEventContent = cms.PSet(
    splitLevel = cms.untracked.int32(0),
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
    outputCommands = cms.untracked.vstring('drop *', 
        'keep LHERunInfoProduct_*_*_*', 
        'keep LHEEventProduct_*_*_*', 
        'keep *_externalLHEProducer_LHEScriptOutput_*')
)

process.MEtoEDMConverterAOD = cms.PSet(
    outputCommands = cms.untracked.vstring()
)

process.MEtoEDMConverterFEVT = cms.PSet(
    outputCommands = cms.untracked.vstring('keep *_MEtoEDMConverter_*_*')
)

process.MEtoEDMConverterRECO = cms.PSet(
    outputCommands = cms.untracked.vstring()
)

process.MINIAODEventContent = cms.PSet(
    compressionLevel = cms.untracked.int32(4),
    compressionAlgorithm = cms.untracked.string('LZMA'),
    outputCommands = cms.untracked.vstring('drop *', 
        'drop *', 
        'keep *_slimmedPhotons*_*_*', 
        'keep *_slimmedElectrons_*_*', 
        'keep *_slimmedMuons*_*_*', 
        'keep *_slimmedTaus*_*_*', 
        'keep *_slimmedJets*_*_*', 
        'keep *_slimmedMETs*_*_*', 
        'keep *_slimmedSecondaryVertices*_*_*', 
        'keep recoPhotonCores_reducedEgamma_*_*', 
        'keep recoGsfElectronCores_reducedEgamma_*_*', 
        'keep recoConversions_reducedEgamma_*_*', 
        'keep recoSuperClusters_reducedEgamma_*_*', 
        'keep recoCaloClusters_reducedEgamma_*_*', 
        'keep EcalRecHitsSorted_reducedEgamma_*_*', 
        'drop *_*_caloTowers_*', 
        'drop *_*_pfCandidates_*', 
        'drop *_*_genJets_*', 
        'keep *_offlineBeamSpot_*_*', 
        'keep *_offlineSlimmedPrimaryVertices_*_*', 
        'keep patPackedCandidates_packedPFCandidates_*_*', 
        'keep double_fixedGridRho*__*', 
        'keep *_selectedPatTrigger_*_*', 
        'keep patPackedTriggerPrescales_patTrigger__*', 
        'keep *_l1extraParticles_*_*', 
        'keep L1GlobalTriggerReadoutRecord_gtDigis_*_*', 
        'keep *_TriggerResults_*_HLT', 
        'keep *_TriggerResults_*_PAT', 
        'keep patPackedCandidates_lostTracks_*_*', 
        'keep HcalNoiseSummary_hcalnoise__*', 
        'keep *_caTopTagInfos_*_*'),
    eventAutoFlushCompressedSize = cms.untracked.int32(15728640)
)

process.MINIAODSIMEventContent = cms.PSet(
    compressionLevel = cms.untracked.int32(4),
    compressionAlgorithm = cms.untracked.string('LZMA'),
    outputCommands = cms.untracked.vstring('drop *', 
        'drop *', 
        'keep *_slimmedPhotons*_*_*', 
        'keep *_slimmedElectrons_*_*', 
        'keep *_slimmedMuons*_*_*', 
        'keep *_slimmedTaus*_*_*', 
        'keep *_slimmedJets*_*_*', 
        'keep *_slimmedMETs*_*_*', 
        'keep *_slimmedSecondaryVertices*_*_*', 
        'keep recoPhotonCores_reducedEgamma_*_*', 
        'keep recoGsfElectronCores_reducedEgamma_*_*', 
        'keep recoConversions_reducedEgamma_*_*', 
        'keep recoSuperClusters_reducedEgamma_*_*', 
        'keep recoCaloClusters_reducedEgamma_*_*', 
        'keep EcalRecHitsSorted_reducedEgamma_*_*', 
        'drop *_*_caloTowers_*', 
        'drop *_*_pfCandidates_*', 
        'drop *_*_genJets_*', 
        'keep *_offlineBeamSpot_*_*', 
        'keep *_offlineSlimmedPrimaryVertices_*_*', 
        'keep patPackedCandidates_packedPFCandidates_*_*', 
        'keep double_fixedGridRho*__*', 
        'keep *_selectedPatTrigger_*_*', 
        'keep patPackedTriggerPrescales_patTrigger__*', 
        'keep *_l1extraParticles_*_*', 
        'keep L1GlobalTriggerReadoutRecord_gtDigis_*_*', 
        'keep *_TriggerResults_*_HLT', 
        'keep *_TriggerResults_*_PAT', 
        'keep patPackedCandidates_lostTracks_*_*', 
        'keep HcalNoiseSummary_hcalnoise__*', 
        'keep *_caTopTagInfos_*_*', 
        'keep *_slimmedGenJets_*_*', 
        'keep patPackedGenParticles_packedGenParticles_*_*', 
        'keep recoGenParticles_prunedGenParticles_*_*', 
        'keep LHEEventProduct_*_*_*', 
        'keep PileupSummaryInfos_*_*_*', 
        'keep GenFilterInfo_*_*_*', 
        'keep GenEventInfoProduct_generator_*_*', 
        'keep LHERunInfoProduct_*_*_*', 
        'keep GenRunInfoProduct_*_*_*', 
        'keep L1GtTriggerMenuLite_l1GtTriggerMenuLite__*'),
    eventAutoFlushCompressedSize = cms.untracked.int32(15728640)
)

process.MIXINGMODULEEventContent = cms.PSet(
    splitLevel = cms.untracked.int32(0),
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
    outputCommands = cms.untracked.vstring('drop *', 
        'keep *_cfWriter_*_*')
)

process.MicroEventContent = cms.PSet(
    outputCommands = cms.untracked.vstring('drop *', 
        'keep *_slimmedPhotons*_*_*', 
        'keep *_slimmedElectrons_*_*', 
        'keep *_slimmedMuons*_*_*', 
        'keep *_slimmedTaus*_*_*', 
        'keep *_slimmedJets*_*_*', 
        'keep *_slimmedMETs*_*_*', 
        'keep *_slimmedSecondaryVertices*_*_*', 
        'keep recoPhotonCores_reducedEgamma_*_*', 
        'keep recoGsfElectronCores_reducedEgamma_*_*', 
        'keep recoConversions_reducedEgamma_*_*', 
        'keep recoSuperClusters_reducedEgamma_*_*', 
        'keep recoCaloClusters_reducedEgamma_*_*', 
        'keep EcalRecHitsSorted_reducedEgamma_*_*', 
        'drop *_*_caloTowers_*', 
        'drop *_*_pfCandidates_*', 
        'drop *_*_genJets_*', 
        'keep *_offlineBeamSpot_*_*', 
        'keep *_offlineSlimmedPrimaryVertices_*_*', 
        'keep patPackedCandidates_packedPFCandidates_*_*', 
        'keep double_fixedGridRho*__*', 
        'keep *_selectedPatTrigger_*_*', 
        'keep patPackedTriggerPrescales_patTrigger__*', 
        'keep *_l1extraParticles_*_*', 
        'keep L1GlobalTriggerReadoutRecord_gtDigis_*_*', 
        'keep *_TriggerResults_*_HLT', 
        'keep *_TriggerResults_*_PAT', 
        'keep patPackedCandidates_lostTracks_*_*', 
        'keep HcalNoiseSummary_hcalnoise__*', 
        'keep *_caTopTagInfos_*_*')
)

process.MicroEventContentMC = cms.PSet(
    outputCommands = cms.untracked.vstring('drop *', 
        'keep *_slimmedPhotons*_*_*', 
        'keep *_slimmedElectrons_*_*', 
        'keep *_slimmedMuons*_*_*', 
        'keep *_slimmedTaus*_*_*', 
        'keep *_slimmedJets*_*_*', 
        'keep *_slimmedMETs*_*_*', 
        'keep *_slimmedSecondaryVertices*_*_*', 
        'keep recoPhotonCores_reducedEgamma_*_*', 
        'keep recoGsfElectronCores_reducedEgamma_*_*', 
        'keep recoConversions_reducedEgamma_*_*', 
        'keep recoSuperClusters_reducedEgamma_*_*', 
        'keep recoCaloClusters_reducedEgamma_*_*', 
        'keep EcalRecHitsSorted_reducedEgamma_*_*', 
        'drop *_*_caloTowers_*', 
        'drop *_*_pfCandidates_*', 
        'drop *_*_genJets_*', 
        'keep *_offlineBeamSpot_*_*', 
        'keep *_offlineSlimmedPrimaryVertices_*_*', 
        'keep patPackedCandidates_packedPFCandidates_*_*', 
        'keep double_fixedGridRho*__*', 
        'keep *_selectedPatTrigger_*_*', 
        'keep patPackedTriggerPrescales_patTrigger__*', 
        'keep *_l1extraParticles_*_*', 
        'keep L1GlobalTriggerReadoutRecord_gtDigis_*_*', 
        'keep *_TriggerResults_*_HLT', 
        'keep *_TriggerResults_*_PAT', 
        'keep patPackedCandidates_lostTracks_*_*', 
        'keep HcalNoiseSummary_hcalnoise__*', 
        'keep *_caTopTagInfos_*_*', 
        'keep *_slimmedGenJets_*_*', 
        'keep patPackedGenParticles_packedGenParticles_*_*', 
        'keep recoGenParticles_prunedGenParticles_*_*', 
        'keep LHEEventProduct_*_*_*', 
        'keep PileupSummaryInfos_*_*_*', 
        'keep GenFilterInfo_*_*_*', 
        'keep GenEventInfoProduct_generator_*_*', 
        'keep LHERunInfoProduct_*_*_*', 
        'keep GenRunInfoProduct_*_*_*', 
        'keep L1GtTriggerMenuLite_l1GtTriggerMenuLite__*')
)

process.OutALCARECODtCalib = cms.PSet(
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('pathALCARECODtCalib')
    ),
    outputCommands = cms.untracked.vstring('drop *', 
        'keep *_dt4DSegments_*_*', 
        'keep *_dt4DSegmentsNoWire_*_*', 
        'keep *_muonDTDigis_*_*', 
        'keep *_dttfDigis_*_*', 
        'keep *_gtDigis_*_*', 
        'keep *_TriggerResults_*_*', 
        'keep recoMuons_muons_*_*', 
        'keep booledmValueMap_muid*_*_*')
)

process.OutALCARECODtCalibCosmics = cms.PSet(
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('pathALCARECODtCalibCosmics')
    ),
    outputCommands = cms.untracked.vstring('drop *', 
        'keep *_dt4DSegments_*_*', 
        'keep *_dt4DSegmentsNoWire_*_*', 
        'keep *_muonDTDigis_*_*', 
        'keep *_dttfDigis_*_*', 
        'keep *_gtDigis_*_*', 
        'keep *_TriggerResults_*_*', 
        'keep recoTracks_cosmicMuons_*_*', 
        'keep recoMuons_muons_*_*', 
        'keep booledmValueMap_muid*_*_*')
)

process.OutALCARECODtCalibCosmics_noDrop = cms.PSet(
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('pathALCARECODtCalibCosmics')
    ),
    outputCommands = cms.untracked.vstring('keep *_dt4DSegments_*_*', 
        'keep *_dt4DSegmentsNoWire_*_*', 
        'keep *_muonDTDigis_*_*', 
        'keep *_dttfDigis_*_*', 
        'keep *_gtDigis_*_*', 
        'keep *_TriggerResults_*_*', 
        'keep recoTracks_cosmicMuons_*_*', 
        'keep recoMuons_muons_*_*', 
        'keep booledmValueMap_muid*_*_*')
)

process.OutALCARECODtCalibHI = cms.PSet(
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('pathALCARECODtCalibHI')
    ),
    outputCommands = cms.untracked.vstring('drop *', 
        'keep *_dt4DSegments_*_*', 
        'keep *_dt4DSegmentsNoWire_*_*', 
        'keep *_muonDTDigis_*_*', 
        'keep *_dttfDigis_*_*', 
        'keep *_gtDigis_*_*', 
        'keep *_TriggerResults_*_*', 
        'keep recoTracks_cosmicMuons_*_*', 
        'keep recoMuons_muons_*_*', 
        'keep booledmValueMap_muid*_*_*', 
        'keep *_hiSelectedVertex_*_*')
)

process.OutALCARECODtCalibHI_noDrop = cms.PSet(
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('pathALCARECODtCalibHI')
    ),
    outputCommands = cms.untracked.vstring('keep *_dt4DSegments_*_*', 
        'keep *_dt4DSegmentsNoWire_*_*', 
        'keep *_muonDTDigis_*_*', 
        'keep *_dttfDigis_*_*', 
        'keep *_gtDigis_*_*', 
        'keep *_TriggerResults_*_*', 
        'keep recoTracks_cosmicMuons_*_*', 
        'keep recoMuons_muons_*_*', 
        'keep booledmValueMap_muid*_*_*', 
        'keep *_hiSelectedVertex_*_*')
)

process.OutALCARECODtCalib_noDrop = cms.PSet(
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('pathALCARECODtCalib')
    ),
    outputCommands = cms.untracked.vstring('keep *_dt4DSegments_*_*', 
        'keep *_dt4DSegmentsNoWire_*_*', 
        'keep *_muonDTDigis_*_*', 
        'keep *_dttfDigis_*_*', 
        'keep *_gtDigis_*_*', 
        'keep *_TriggerResults_*_*', 
        'keep recoMuons_muons_*_*', 
        'keep booledmValueMap_muid*_*_*')
)

process.OutALCARECOEcalCalElectron = cms.PSet(
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('pathALCARECOEcalCalZElectron', 
            'pathALCARECOEcalCalWElectron')
    ),
    outputCommands = cms.untracked.vstring('drop *', 
        'keep *_pfMet_*_*', 
        'keep *_kt6PFJetsForRhoCorrection_rho_*', 
        'keep *_kt6PFJets_rho_*', 
        'keep *_offlinePrimaryVerticesWithBS_*_*', 
        'keep *BeamSpot_offlineBeamSpot_*_*', 
        'keep *_allConversions_*_*', 
        'keep *_conversions_*_*', 
        'keep *GsfTrack*_*_*_*', 
        'keep *_generator_*_*', 
        'keep *_addPileupInfo_*_*', 
        'keep *_genParticles_*_*', 
        'keep recoGsfElectron*_gsfElectron*_*_*', 
        'keep recoCaloClusters_*_*_*', 
        'keep recoSuperClusters_*_*_*', 
        'keep recoPreshowerCluster*_*_*_*', 
        'drop reco*Clusters_hfEMClusters_*_*', 
        'drop reco*Clusters_pfPhotonTranslator_*_*', 
        'drop *_*Unclean*_*_*', 
        'drop *_*unclean*_*_*', 
        'drop *_*_*Unclean*_*', 
        'drop *_*_*unclean*_*', 
        'keep *_*_*_HLT', 
        'keep recoTracks_generalTracks_*_*', 
        'drop *EcalRecHit*_ecalRecHit_*_*', 
        'drop *EcalrecHit*_*ecalPreshowerRecHit*_*EcalRecHitsES*_*', 
        'drop *EcalRecHit*_reducedEcalRecHitsE*_*_*', 
        'keep *EcalRecHit*_alCaIsolatedElectrons_*_*', 
        'keep *EcalRecHit*_reducedEcalRecHitsES_alCaRecHitsES_*')
)

process.OutALCARECOEcalCalElectron_noDrop = cms.PSet(
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('pathALCARECOEcalCalZElectron', 
            'pathALCARECOEcalCalWElectron')
    ),
    outputCommands = cms.untracked.vstring('keep *_pfMet_*_*', 
        'keep *_kt6PFJetsForRhoCorrection_rho_*', 
        'keep *_kt6PFJets_rho_*', 
        'keep *_offlinePrimaryVerticesWithBS_*_*', 
        'keep *BeamSpot_offlineBeamSpot_*_*', 
        'keep *_allConversions_*_*', 
        'keep *_conversions_*_*', 
        'keep *GsfTrack*_*_*_*', 
        'keep *_generator_*_*', 
        'keep *_addPileupInfo_*_*', 
        'keep *_genParticles_*_*', 
        'keep recoGsfElectron*_gsfElectron*_*_*', 
        'keep recoCaloClusters_*_*_*', 
        'keep recoSuperClusters_*_*_*', 
        'keep recoPreshowerCluster*_*_*_*', 
        'drop reco*Clusters_hfEMClusters_*_*', 
        'drop reco*Clusters_pfPhotonTranslator_*_*', 
        'drop *_*Unclean*_*_*', 
        'drop *_*unclean*_*_*', 
        'drop *_*_*Unclean*_*', 
        'drop *_*_*unclean*_*', 
        'keep *_*_*_HLT', 
        'keep recoTracks_generalTracks_*_*', 
        'drop *EcalRecHit*_ecalRecHit_*_*', 
        'drop *EcalrecHit*_*ecalPreshowerRecHit*_*EcalRecHitsES*_*', 
        'drop *EcalRecHit*_reducedEcalRecHitsE*_*_*', 
        'keep *EcalRecHit*_alCaIsolatedElectrons_*_*', 
        'keep *EcalRecHit*_reducedEcalRecHitsES_alCaRecHitsES_*')
)

process.OutALCARECOEcalCalEtaCalib = cms.PSet(
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('pathALCARECOEcalCalEtaCalib')
    ),
    outputCommands = cms.untracked.vstring('drop *', 
        'keep *_ecalEtaCorrected_etaEcalRecHitsEB_*', 
        'keep *_ecalEtaCorrected_etaEcalRecHitsEE_*', 
        'keep L1GlobalTriggerReadoutRecord_hltGtDigis_*_*', 
        'keep *_hltAlCaEtaRecHitsFilter_etaEcalRecHitsES_*')
)

process.OutALCARECOEcalCalEtaCalib_noDrop = cms.PSet(
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('pathALCARECOEcalCalEtaCalib')
    ),
    outputCommands = cms.untracked.vstring('keep *_ecalEtaCorrected_etaEcalRecHitsEB_*', 
        'keep *_ecalEtaCorrected_etaEcalRecHitsEE_*', 
        'keep L1GlobalTriggerReadoutRecord_hltGtDigis_*_*', 
        'keep *_hltAlCaEtaRecHitsFilter_etaEcalRecHitsES_*')
)

process.OutALCARECOEcalCalPhiSym = cms.PSet(
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('pathALCARECOEcalCalPhiSym')
    ),
    outputCommands = cms.untracked.vstring('drop *', 
        'keep *_ecalPhiSymCorrected_phiSymEcalRecHitsEB_*', 
        'keep *_ecalPhiSymCorrected_phiSymEcalRecHitsEE_*', 
        'keep L1GlobalTriggerReadoutRecord_hltGtDigis_*_*')
)

process.OutALCARECOEcalCalPhiSym_noDrop = cms.PSet(
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('pathALCARECOEcalCalPhiSym')
    ),
    outputCommands = cms.untracked.vstring('keep *_ecalPhiSymCorrected_phiSymEcalRecHitsEB_*', 
        'keep *_ecalPhiSymCorrected_phiSymEcalRecHitsEE_*', 
        'keep L1GlobalTriggerReadoutRecord_hltGtDigis_*_*')
)

process.OutALCARECOEcalCalPi0Calib = cms.PSet(
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('pathALCARECOEcalCalPi0Calib')
    ),
    outputCommands = cms.untracked.vstring('drop *', 
        'keep *_ecalPi0Corrected_pi0EcalRecHitsEB_*', 
        'keep *_ecalPi0Corrected_pi0EcalRecHitsEE_*', 
        'keep L1GlobalTriggerReadoutRecord_hltGtDigis_*_*', 
        'keep *_hltAlCaPi0RecHitsFilter_pi0EcalRecHitsES_*')
)

process.OutALCARECOEcalCalPi0Calib_noDrop = cms.PSet(
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('pathALCARECOEcalCalPi0Calib')
    ),
    outputCommands = cms.untracked.vstring('keep *_ecalPi0Corrected_pi0EcalRecHitsEB_*', 
        'keep *_ecalPi0Corrected_pi0EcalRecHitsEE_*', 
        'keep L1GlobalTriggerReadoutRecord_hltGtDigis_*_*', 
        'keep *_hltAlCaPi0RecHitsFilter_pi0EcalRecHitsES_*')
)

process.OutALCARECOHcalCalDijets = cms.PSet(
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('pathALCARECOHcalCalDijets')
    ),
    outputCommands = cms.untracked.vstring('drop *', 
        'keep *_DiJProd_*_*', 
        'keep triggerTriggerEvent_*_*_*')
)

process.OutALCARECOHcalCalDijets_noDrop = cms.PSet(
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('pathALCARECOHcalCalDijets')
    ),
    outputCommands = cms.untracked.vstring('keep *_DiJProd_*_*', 
        'keep triggerTriggerEvent_*_*_*')
)

process.OutALCARECOHcalCalGammaJet = cms.PSet(
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('pathALCARECOHcalCalGammaJet')
    ),
    outputCommands = cms.untracked.vstring('drop *', 
        'keep *_GammaJetProd_*_*')
)

process.OutALCARECOHcalCalGammaJet_noDrop = cms.PSet(
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('pathALCARECOHcalCalGammaJet')
    ),
    outputCommands = cms.untracked.vstring('keep *_GammaJetProd_*_*')
)

process.OutALCARECOHcalCalHO = cms.PSet(
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('pathALCARECOHcalCalHO')
    ),
    outputCommands = cms.untracked.vstring('drop *', 
        'keep *_hoCalibProducer_*_*')
)

process.OutALCARECOHcalCalHOCosmics = cms.PSet(
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('pathALCARECOHcalCalHOCosmics')
    ),
    outputCommands = cms.untracked.vstring('drop *', 
        'keep HOCalibVariabless_*_*_*')
)

process.OutALCARECOHcalCalHOCosmics_noDrop = cms.PSet(
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('pathALCARECOHcalCalHOCosmics')
    ),
    outputCommands = cms.untracked.vstring('keep HOCalibVariabless_*_*_*')
)

process.OutALCARECOHcalCalHO_noDrop = cms.PSet(
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('pathALCARECOHcalCalHO')
    ),
    outputCommands = cms.untracked.vstring('keep *_hoCalibProducer_*_*')
)

process.OutALCARECOHcalCalIsoTrk = cms.PSet(
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('pathALCARECOHcalCalIsoTrk')
    ),
    outputCommands = cms.untracked.vstring('drop *', 
        'keep *_IsoProd_*_*', 
        'keep *_TkAlIsoProd_*_*', 
        'keep *_offlinePrimaryVertices_*_*', 
        'keep edmTriggerResults_*_*_*', 
        'keep triggerTriggerEvent_*_*_*')
)

process.OutALCARECOHcalCalIsoTrkNoHLT = cms.PSet(
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('pathALCARECOHcalCalIsoTrkNoHLT')
    ),
    outputCommands = cms.untracked.vstring('drop *', 
        'keep *_IsoProd_*_*')
)

process.OutALCARECOHcalCalIsoTrkNoHLT_noDrop = cms.PSet(
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('pathALCARECOHcalCalIsoTrkNoHLT')
    ),
    outputCommands = cms.untracked.vstring('keep *_IsoProd_*_*')
)

process.OutALCARECOHcalCalIsoTrk_noDrop = cms.PSet(
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('pathALCARECOHcalCalIsoTrk')
    ),
    outputCommands = cms.untracked.vstring('keep *_IsoProd_*_*', 
        'keep *_TkAlIsoProd_*_*', 
        'keep *_offlinePrimaryVertices_*_*', 
        'keep edmTriggerResults_*_*_*', 
        'keep triggerTriggerEvent_*_*_*')
)

process.OutALCARECOHcalCalMinBias = cms.PSet(
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('pathALCARECOHcalCalMinBias')
    ),
    outputCommands = cms.untracked.vstring('drop *', 
        'keep *_gtDigisAlCaMB_*_*', 
        'keep HBHERecHitsSorted_hbherecoMB_*_*', 
        'keep HORecHitsSorted_horecoMB_*_*', 
        'keep HFRecHitsSorted_hfrecoMB_*_*', 
        'keep HFRecHitsSorted_hfrecoMBspecial_*_*', 
        'keep HBHERecHitsSorted_hbherecoNoise_*_*', 
        'keep HORecHitsSorted_horecoNoise_*_*', 
        'keep HFRecHitsSorted_hfrecoNoise_*_*')
)

process.OutALCARECOHcalCalMinBiasHI = cms.PSet(
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('pathALCARECOHcalCalMinBias')
    ),
    outputCommands = cms.untracked.vstring('drop *', 
        'keep *_gtDigisAlCaMB_*_*', 
        'keep HBHERecHitsSorted_hbhereco_*_*', 
        'keep HORecHitsSorted_horeco_*_*', 
        'keep HFRecHitsSorted_hfreco_*_*', 
        'keep HFRecHitsSorted_hfrecoMBspecial_*_*', 
        'keep HBHERecHitsSorted_hbherecoNoise_*_*', 
        'keep HORecHitsSorted_horecoNoise_*_*', 
        'keep HFRecHitsSorted_hfrecoNoise_*_*')
)

process.OutALCARECOHcalCalMinBiasHI_noDrop = cms.PSet(
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('pathALCARECOHcalCalMinBias')
    ),
    outputCommands = cms.untracked.vstring('keep *_gtDigisAlCaMB_*_*', 
        'keep HBHERecHitsSorted_hbhereco_*_*', 
        'keep HORecHitsSorted_horeco_*_*', 
        'keep HFRecHitsSorted_hfreco_*_*', 
        'keep HFRecHitsSorted_hfrecoMBspecial_*_*', 
        'keep HBHERecHitsSorted_hbherecoNoise_*_*', 
        'keep HORecHitsSorted_horecoNoise_*_*', 
        'keep HFRecHitsSorted_hfrecoNoise_*_*')
)

process.OutALCARECOHcalCalMinBias_noDrop = cms.PSet(
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('pathALCARECOHcalCalMinBias')
    ),
    outputCommands = cms.untracked.vstring('keep *_gtDigisAlCaMB_*_*', 
        'keep HBHERecHitsSorted_hbherecoMB_*_*', 
        'keep HORecHitsSorted_horecoMB_*_*', 
        'keep HFRecHitsSorted_hfrecoMB_*_*', 
        'keep HFRecHitsSorted_hfrecoMBspecial_*_*', 
        'keep HBHERecHitsSorted_hbherecoNoise_*_*', 
        'keep HORecHitsSorted_horecoNoise_*_*', 
        'keep HFRecHitsSorted_hfrecoNoise_*_*')
)

process.OutALCARECOHcalCalNoise = cms.PSet(
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('pathALCARECOHcalCalNoise')
    ),
    outputCommands = cms.untracked.vstring('drop *', 
        'keep *_HcalNoiseProd_*_*', 
        'keep edmTriggerResults_*_*_HLT')
)

process.OutALCARECOHcalCalNoise_noDrop = cms.PSet(
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('pathALCARECOHcalCalNoise')
    ),
    outputCommands = cms.untracked.vstring('keep *_HcalNoiseProd_*_*', 
        'keep edmTriggerResults_*_*_HLT')
)

process.OutALCARECOLumiPixels = cms.PSet(
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('pathALCARECOLumiPixels')
    ),
    outputCommands = cms.untracked.vstring('drop *', 
        'keep *_siPixelClustersForLumi_*_*', 
        'keep *_TriggerResults_*_HLT')
)

process.OutALCARECOLumiPixels_noDrop = cms.PSet(
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('pathALCARECOLumiPixels')
    ),
    outputCommands = cms.untracked.vstring('keep *_siPixelClustersForLumi_*_*', 
        'keep *_TriggerResults_*_HLT')
)

process.OutALCARECOMuAlBeamHalo = cms.PSet(
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('pathALCARECOMuAlBeamHalo')
    ),
    outputCommands = cms.untracked.vstring('drop *', 
        'keep *_ALCARECOMuAlBeamHalo_*_*', 
        'keep *_muonCSCDigis_*_*', 
        'keep *_csc2DRecHits_*_*', 
        'keep *_cscSegments_*_*', 
        'keep L1AcceptBunchCrossings_*_*_*', 
        'keep L1GlobalTriggerReadoutRecord_gtDigis_*_*', 
        'keep *_TriggerResults_*_*', 
        'keep DcsStatuss_scalersRawToDigi_*_*')
)

process.OutALCARECOMuAlBeamHaloOverlaps = cms.PSet(
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('pathALCARECOMuAlBeamHaloOverlaps')
    ),
    outputCommands = cms.untracked.vstring('drop *', 
        'keep *_ALCARECOMuAlBeamHaloOverlaps_*_*', 
        'keep *_muonCSCDigis_*_*', 
        'keep *_csc2DRecHits_*_*', 
        'keep *_cscSegments_*_*', 
        'keep L1AcceptBunchCrossings_*_*_*', 
        'keep L1GlobalTriggerReadoutRecord_gtDigis_*_*', 
        'keep *_TriggerResults_*_*', 
        'keep DcsStatuss_scalersRawToDigi_*_*')
)

process.OutALCARECOMuAlBeamHaloOverlaps_noDrop = cms.PSet(
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('pathALCARECOMuAlBeamHaloOverlaps')
    ),
    outputCommands = cms.untracked.vstring('keep *_ALCARECOMuAlBeamHaloOverlaps_*_*', 
        'keep *_muonCSCDigis_*_*', 
        'keep *_csc2DRecHits_*_*', 
        'keep *_cscSegments_*_*', 
        'keep L1AcceptBunchCrossings_*_*_*', 
        'keep L1GlobalTriggerReadoutRecord_gtDigis_*_*', 
        'keep *_TriggerResults_*_*', 
        'keep DcsStatuss_scalersRawToDigi_*_*')
)

process.OutALCARECOMuAlBeamHalo_noDrop = cms.PSet(
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('pathALCARECOMuAlBeamHalo')
    ),
    outputCommands = cms.untracked.vstring('keep *_ALCARECOMuAlBeamHalo_*_*', 
        'keep *_muonCSCDigis_*_*', 
        'keep *_csc2DRecHits_*_*', 
        'keep *_cscSegments_*_*', 
        'keep L1AcceptBunchCrossings_*_*_*', 
        'keep L1GlobalTriggerReadoutRecord_gtDigis_*_*', 
        'keep *_TriggerResults_*_*', 
        'keep DcsStatuss_scalersRawToDigi_*_*')
)

process.OutALCARECOMuAlCalIsolatedMu = cms.PSet(
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('pathALCARECOMuAlCalIsolatedMu')
    ),
    outputCommands = cms.untracked.vstring('drop *', 
        'keep *_ALCARECOMuAlCalIsolatedMu_*_*', 
        'keep *_muonCSCDigis_*_*', 
        'keep *_muonDTDigis_*_*', 
        'keep *_muonRPCDigis_*_*', 
        'keep *_dt1DRecHits_*_*', 
        'keep *_dt2DSegments_*_*', 
        'keep *_dt4DSegments_*_*', 
        'keep *_csc2DRecHits_*_*', 
        'keep *_cscSegments_*_*', 
        'keep *_rpcRecHits_*_*', 
        'keep L1AcceptBunchCrossings_*_*_*', 
        'keep L1GlobalTriggerReadoutRecord_gtDigis_*_*', 
        'keep *_TriggerResults_*_*', 
        'keep DcsStatuss_scalersRawToDigi_*_*')
)

process.OutALCARECOMuAlCalIsolatedMu_noDrop = cms.PSet(
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('pathALCARECOMuAlCalIsolatedMu')
    ),
    outputCommands = cms.untracked.vstring('keep *_ALCARECOMuAlCalIsolatedMu_*_*', 
        'keep *_muonCSCDigis_*_*', 
        'keep *_muonDTDigis_*_*', 
        'keep *_muonRPCDigis_*_*', 
        'keep *_dt1DRecHits_*_*', 
        'keep *_dt2DSegments_*_*', 
        'keep *_dt4DSegments_*_*', 
        'keep *_csc2DRecHits_*_*', 
        'keep *_cscSegments_*_*', 
        'keep *_rpcRecHits_*_*', 
        'keep L1AcceptBunchCrossings_*_*_*', 
        'keep L1GlobalTriggerReadoutRecord_gtDigis_*_*', 
        'keep *_TriggerResults_*_*', 
        'keep DcsStatuss_scalersRawToDigi_*_*')
)

process.OutALCARECOMuAlGlobalCosmics = cms.PSet(
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('pathALCARECOMuAlGlobalCosmics')
    ),
    outputCommands = cms.untracked.vstring('drop *', 
        'keep *_ALCARECOMuAlGlobalCosmics_*_*', 
        'keep *_muonCSCDigis_*_*', 
        'keep *_muonDTDigis_*_*', 
        'keep *_muonRPCDigis_*_*', 
        'keep *_dt1DRecHits_*_*', 
        'keep *_dt2DSegments_*_*', 
        'keep *_dt4DSegments_*_*', 
        'keep *_csc2DRecHits_*_*', 
        'keep *_cscSegments_*_*', 
        'keep *_rpcRecHits_*_*', 
        'keep L1AcceptBunchCrossings_*_*_*', 
        'keep L1GlobalTriggerReadoutRecord_gtDigis_*_*', 
        'keep *_TriggerResults_*_*', 
        'keep DcsStatuss_scalersRawToDigi_*_*')
)

process.OutALCARECOMuAlGlobalCosmicsInCollisions = cms.PSet(
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('pathALCARECOMuAlGlobalCosmicsInCollisions')
    ),
    outputCommands = cms.untracked.vstring('drop *', 
        'keep *_ALCARECOMuAlGlobalCosmicsInCollisions_*_*', 
        'keep *_muonCSCDigis_*_*', 
        'keep *_muonDTDigis_*_*', 
        'keep *_muonRPCDigis_*_*', 
        'keep *_dt1DRecHits_*_*', 
        'keep *_dt2DSegments_*_*', 
        'keep *_dt4DSegments_*_*', 
        'keep *_csc2DRecHits_*_*', 
        'keep *_cscSegments_*_*', 
        'keep *_rpcRecHits_*_*', 
        'keep L1AcceptBunchCrossings_*_*_*', 
        'keep L1GlobalTriggerReadoutRecord_gtDigis_*_*', 
        'keep *_TriggerResults_*_*', 
        'keep DcsStatuss_scalersRawToDigi_*_*')
)

process.OutALCARECOMuAlGlobalCosmicsInCollisions_noDrop = cms.PSet(
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('pathALCARECOMuAlGlobalCosmicsInCollisions')
    ),
    outputCommands = cms.untracked.vstring('keep *_ALCARECOMuAlGlobalCosmicsInCollisions_*_*', 
        'keep *_muonCSCDigis_*_*', 
        'keep *_muonDTDigis_*_*', 
        'keep *_muonRPCDigis_*_*', 
        'keep *_dt1DRecHits_*_*', 
        'keep *_dt2DSegments_*_*', 
        'keep *_dt4DSegments_*_*', 
        'keep *_csc2DRecHits_*_*', 
        'keep *_cscSegments_*_*', 
        'keep *_rpcRecHits_*_*', 
        'keep L1AcceptBunchCrossings_*_*_*', 
        'keep L1GlobalTriggerReadoutRecord_gtDigis_*_*', 
        'keep *_TriggerResults_*_*', 
        'keep DcsStatuss_scalersRawToDigi_*_*')
)

process.OutALCARECOMuAlGlobalCosmics_noDrop = cms.PSet(
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('pathALCARECOMuAlGlobalCosmics')
    ),
    outputCommands = cms.untracked.vstring('keep *_ALCARECOMuAlGlobalCosmics_*_*', 
        'keep *_muonCSCDigis_*_*', 
        'keep *_muonDTDigis_*_*', 
        'keep *_muonRPCDigis_*_*', 
        'keep *_dt1DRecHits_*_*', 
        'keep *_dt2DSegments_*_*', 
        'keep *_dt4DSegments_*_*', 
        'keep *_csc2DRecHits_*_*', 
        'keep *_cscSegments_*_*', 
        'keep *_rpcRecHits_*_*', 
        'keep L1AcceptBunchCrossings_*_*_*', 
        'keep L1GlobalTriggerReadoutRecord_gtDigis_*_*', 
        'keep *_TriggerResults_*_*', 
        'keep DcsStatuss_scalersRawToDigi_*_*')
)

process.OutALCARECOMuAlOverlaps = cms.PSet(
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('pathALCARECOMuAlOverlaps')
    ),
    outputCommands = cms.untracked.vstring('drop *', 
        'keep *_ALCARECOMuAlOverlaps_*_*', 
        'keep L1AcceptBunchCrossings_*_*_*', 
        'keep L1GlobalTriggerReadoutRecord_gtDigis_*_*', 
        'keep *_TriggerResults_*_*', 
        'keep DcsStatuss_scalersRawToDigi_*_*')
)

process.OutALCARECOMuAlOverlaps_noDrop = cms.PSet(
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('pathALCARECOMuAlOverlaps')
    ),
    outputCommands = cms.untracked.vstring('keep *_ALCARECOMuAlOverlaps_*_*', 
        'keep L1AcceptBunchCrossings_*_*_*', 
        'keep L1GlobalTriggerReadoutRecord_gtDigis_*_*', 
        'keep *_TriggerResults_*_*', 
        'keep DcsStatuss_scalersRawToDigi_*_*')
)

process.OutALCARECOMuAlStandAloneCosmics = cms.PSet(
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('pathALCARECOMuAlStandAloneCosmics')
    ),
    outputCommands = cms.untracked.vstring('drop *', 
        'keep *_ALCARECOMuAlStandAloneCosmics_*_*', 
        'keep *_muonCSCDigis_*_*', 
        'keep *_muonDTDigis_*_*', 
        'keep *_muonRPCDigis_*_*', 
        'keep *_dt1DRecHits_*_*', 
        'keep *_dt2DSegments_*_*', 
        'keep *_dt4DSegments_*_*', 
        'keep *_csc2DRecHits_*_*', 
        'keep *_cscSegments_*_*', 
        'keep *_rpcRecHits_*_*', 
        'keep L1AcceptBunchCrossings_*_*_*', 
        'keep L1GlobalTriggerReadoutRecord_gtDigis_*_*', 
        'keep *_TriggerResults_*_*', 
        'keep DcsStatuss_scalersRawToDigi_*_*')
)

process.OutALCARECOMuAlStandAloneCosmics_noDrop = cms.PSet(
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('pathALCARECOMuAlStandAloneCosmics')
    ),
    outputCommands = cms.untracked.vstring('keep *_ALCARECOMuAlStandAloneCosmics_*_*', 
        'keep *_muonCSCDigis_*_*', 
        'keep *_muonDTDigis_*_*', 
        'keep *_muonRPCDigis_*_*', 
        'keep *_dt1DRecHits_*_*', 
        'keep *_dt2DSegments_*_*', 
        'keep *_dt4DSegments_*_*', 
        'keep *_csc2DRecHits_*_*', 
        'keep *_cscSegments_*_*', 
        'keep *_rpcRecHits_*_*', 
        'keep L1AcceptBunchCrossings_*_*_*', 
        'keep L1GlobalTriggerReadoutRecord_gtDigis_*_*', 
        'keep *_TriggerResults_*_*', 
        'keep DcsStatuss_scalersRawToDigi_*_*')
)

process.OutALCARECOMuAlZMuMu = cms.PSet(
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('pathALCARECOMuAlZMuMu')
    ),
    outputCommands = cms.untracked.vstring('drop *', 
        'keep *_ALCARECOMuAlZMuMu_*_*', 
        'keep *_muonCSCDigis_*_*', 
        'keep *_muonDTDigis_*_*', 
        'keep *_muonRPCDigis_*_*', 
        'keep *_dt1DRecHits_*_*', 
        'keep *_dt2DSegments_*_*', 
        'keep *_dt4DSegments_*_*', 
        'keep *_csc2DRecHits_*_*', 
        'keep *_cscSegments_*_*', 
        'keep *_rpcRecHits_*_*', 
        'keep L1AcceptBunchCrossings_*_*_*', 
        'keep L1GlobalTriggerReadoutRecord_gtDigis_*_*', 
        'keep *_TriggerResults_*_*', 
        'keep DcsStatuss_scalersRawToDigi_*_*')
)

process.OutALCARECOMuAlZMuMu_noDrop = cms.PSet(
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('pathALCARECOMuAlZMuMu')
    ),
    outputCommands = cms.untracked.vstring('keep *_ALCARECOMuAlZMuMu_*_*', 
        'keep *_muonCSCDigis_*_*', 
        'keep *_muonDTDigis_*_*', 
        'keep *_muonRPCDigis_*_*', 
        'keep *_dt1DRecHits_*_*', 
        'keep *_dt2DSegments_*_*', 
        'keep *_dt4DSegments_*_*', 
        'keep *_csc2DRecHits_*_*', 
        'keep *_cscSegments_*_*', 
        'keep *_rpcRecHits_*_*', 
        'keep L1AcceptBunchCrossings_*_*_*', 
        'keep L1GlobalTriggerReadoutRecord_gtDigis_*_*', 
        'keep *_TriggerResults_*_*', 
        'keep DcsStatuss_scalersRawToDigi_*_*')
)

process.OutALCARECOPromptCalibProd = cms.PSet(
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('pathALCARECOPromptCalibProd')
    ),
    outputCommands = cms.untracked.vstring('drop *', 
        'keep *_alcaBeamSpotProducer_*_*', 
        'keep *_MEtoEDMConvertSiStrip_*_*')
)

process.OutALCARECOPromptCalibProdSiStrip = cms.PSet(
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('pathALCARECOPromptCalibProdSiStrip')
    ),
    outputCommands = cms.untracked.vstring('drop *', 
        'keep *_MEtoEDMConvertSiStrip_*_*')
)

process.OutALCARECOPromptCalibProdSiStripGains = cms.PSet(
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('pathALCARECOPromptCalibProdSiStripGains')
    ),
    outputCommands = cms.untracked.vstring('drop *', 
        'keep *_alcaBeamSpotProducer_*_*', 
        'keep *_MEtoEDMConvertSiStripGains_*_*')
)

process.OutALCARECOPromptCalibProdSiStripGains_noDrop = cms.PSet(
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('pathALCARECOPromptCalibProdSiStripGains')
    ),
    outputCommands = cms.untracked.vstring('keep *_alcaBeamSpotProducer_*_*', 
        'keep *_MEtoEDMConvertSiStripGains_*_*')
)

process.OutALCARECOPromptCalibProdSiStrip_noDrop = cms.PSet(
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('pathALCARECOPromptCalibProdSiStrip')
    ),
    outputCommands = cms.untracked.vstring('keep *_MEtoEDMConvertSiStrip_*_*')
)

process.OutALCARECOPromptCalibProd_noDrop = cms.PSet(
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('pathALCARECOPromptCalibProd')
    ),
    outputCommands = cms.untracked.vstring('keep *_alcaBeamSpotProducer_*_*', 
        'keep *_MEtoEDMConvertSiStrip_*_*')
)

process.OutALCARECORpcCalHLT = cms.PSet(
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('pathALCARECORpcCalHLT')
    ),
    outputCommands = cms.untracked.vstring('drop *', 
        'keep *_muonDTDigis_*_*', 
        'keep CSCDetIdCSCWireDigiMuonDigiCollection_*_*_*', 
        'keep CSCDetIdCSCStripDigiMuonDigiCollection_*_*_*', 
        'keep DTLayerIdDTDigiMuonDigiCollection_*_*_*', 
        'keep *_dt4DSegments_*_*', 
        'keep *_cscSegments_*_*', 
        'keep *_rpcRecHits_*_*', 
        'keep RPCDetIdRPCDigiMuonDigiCollection_*_*_*', 
        'keep recoMuons_muonsNoRPC_*_*', 
        'keep L1MuRegionalCands_*_RPCb_*', 
        'keep L1MuRegionalCands_*_RPCf_*', 
        'keep L1MuGMTCands_*_*_*', 
        'keep L1MuGMTReadoutCollection_*_*_*')
)

process.OutALCARECORpcCalHLT_noDrop = cms.PSet(
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('pathALCARECORpcCalHLT')
    ),
    outputCommands = cms.untracked.vstring('keep *_muonDTDigis_*_*', 
        'keep CSCDetIdCSCWireDigiMuonDigiCollection_*_*_*', 
        'keep CSCDetIdCSCStripDigiMuonDigiCollection_*_*_*', 
        'keep DTLayerIdDTDigiMuonDigiCollection_*_*_*', 
        'keep *_dt4DSegments_*_*', 
        'keep *_cscSegments_*_*', 
        'keep *_rpcRecHits_*_*', 
        'keep RPCDetIdRPCDigiMuonDigiCollection_*_*_*', 
        'keep recoMuons_muonsNoRPC_*_*', 
        'keep L1MuRegionalCands_*_RPCb_*', 
        'keep L1MuRegionalCands_*_RPCf_*', 
        'keep L1MuGMTCands_*_*_*', 
        'keep L1MuGMTReadoutCollection_*_*_*')
)

process.OutALCARECOSiPixelLorentzAngle = cms.PSet(
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('pathALCARECOSiPixelLorentzAngle')
    ),
    outputCommands = cms.untracked.vstring('drop *', 
        'keep *_globalMuons_*_*', 
        'keep *_siStripClusters_*_*', 
        'keep *_siPixelClusters_*_*', 
        'drop *_*_*_HLT')
)

process.OutALCARECOSiPixelLorentzAngle_noDrop = cms.PSet(
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('pathALCARECOSiPixelLorentzAngle')
    ),
    outputCommands = cms.untracked.vstring('keep *_globalMuons_*_*', 
        'keep *_siStripClusters_*_*', 
        'keep *_siPixelClusters_*_*', 
        'drop *_*_*_HLT')
)

process.OutALCARECOSiStripCalMinBias = cms.PSet(
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('pathALCARECOSiStripCalMinBias')
    ),
    outputCommands = cms.untracked.vstring('drop *', 
        'keep *_ALCARECOSiStripCalMinBias_*_*', 
        'keep *_siStripClusters_*_*', 
        'keep *_siPixelClusters_*_*', 
        'keep DetIdedmEDCollection_siStripDigis_*_*', 
        'keep L1AcceptBunchCrossings_*_*_*', 
        'keep L1GlobalTriggerReadoutRecord_gtDigis_*_*', 
        'keep *_TriggerResults_*_*')
)

process.OutALCARECOSiStripCalMinBias_noDrop = cms.PSet(
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('pathALCARECOSiStripCalMinBias')
    ),
    outputCommands = cms.untracked.vstring('keep *_ALCARECOSiStripCalMinBias_*_*', 
        'keep *_siStripClusters_*_*', 
        'keep *_siPixelClusters_*_*', 
        'keep DetIdedmEDCollection_siStripDigis_*_*', 
        'keep L1AcceptBunchCrossings_*_*_*', 
        'keep L1GlobalTriggerReadoutRecord_gtDigis_*_*', 
        'keep *_TriggerResults_*_*')
)

process.OutALCARECOSiStripCalZeroBias = cms.PSet(
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('pathALCARECOSiStripCalZeroBias')
    ),
    outputCommands = cms.untracked.vstring('drop *', 
        'keep *_ALCARECOSiStripCalZeroBias_*_*', 
        'keep *_calZeroBiasClusters_*_*', 
        'keep L1AcceptBunchCrossings_*_*_*', 
        'keep *_TriggerResults_*_*')
)

process.OutALCARECOSiStripCalZeroBias_noDrop = cms.PSet(
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('pathALCARECOSiStripCalZeroBias')
    ),
    outputCommands = cms.untracked.vstring('keep *_ALCARECOSiStripCalZeroBias_*_*', 
        'keep *_calZeroBiasClusters_*_*', 
        'keep L1AcceptBunchCrossings_*_*_*', 
        'keep *_TriggerResults_*_*')
)

process.OutALCARECOSiStripPCLHistos = cms.PSet(
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('pathALCARECOSiStripPCLHistos')
    ),
    outputCommands = cms.untracked.vstring('drop *', 
        'keep *_MEtoEDMConvertSiStrip_*_*')
)

process.OutALCARECOSiStripPCLHistos_noDrop = cms.PSet(
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('pathALCARECOSiStripPCLHistos')
    ),
    outputCommands = cms.untracked.vstring('keep *_MEtoEDMConvertSiStrip_*_*')
)

process.OutALCARECOTkAlBeamHalo = cms.PSet(
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('pathALCARECOTkAlBeamHalo')
    ),
    outputCommands = cms.untracked.vstring('drop *', 
        'keep *_ALCARECOTkAlBeamHalo_*_*', 
        'keep L1AcceptBunchCrossings_*_*_*', 
        'keep L1GlobalTriggerReadoutRecord_gtDigis_*_*', 
        'keep *_TriggerResults_*_*', 
        'keep DcsStatuss_scalersRawToDigi_*_*')
)

process.OutALCARECOTkAlBeamHalo_noDrop = cms.PSet(
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('pathALCARECOTkAlBeamHalo')
    ),
    outputCommands = cms.untracked.vstring('keep *_ALCARECOTkAlBeamHalo_*_*', 
        'keep L1AcceptBunchCrossings_*_*_*', 
        'keep L1GlobalTriggerReadoutRecord_gtDigis_*_*', 
        'keep *_TriggerResults_*_*', 
        'keep DcsStatuss_scalersRawToDigi_*_*')
)

process.OutALCARECOTkAlCosmics = cms.PSet(
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('pathALCARECOTkAlCosmicsCTF', 
            'pathALCARECOTkAlCosmicsCosmicTF', 
            'pathALCARECOTkAlCosmicsRegional')
    ),
    outputCommands = cms.untracked.vstring('drop *', 
        'keep *_ALCARECOTkAlCosmicsCTF_*_*', 
        'keep *_ALCARECOTkAlCosmicsCosmicTF_*_*', 
        'keep *_ALCARECOTkAlCosmicsRegional_*_*', 
        'keep siStripDigis_DetIdCollection_*_*', 
        'keep L1AcceptBunchCrossings_*_*_*', 
        'keep L1GlobalTriggerReadoutRecord_gtDigis_*_*', 
        'keep *_TriggerResults_*_*', 
        'keep DcsStatuss_scalersRawToDigi_*_*', 
        'keep Si*Cluster*_si*Clusters_*_*', 
        'keep recoMuons_muons1Leg_*_*')
)

process.OutALCARECOTkAlCosmics0T = cms.PSet(
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('pathALCARECOTkAlCosmicsCTF0T', 
            'pathALCARECOTkAlCosmicsCosmicTF0T', 
            'pathALCARECOTkAlCosmicsRegional0T')
    ),
    outputCommands = cms.untracked.vstring('drop *', 
        'keep *_ALCARECOTkAlCosmics*0T_*_*', 
        'keep siStripDigis_DetIdCollection_*_*', 
        'keep L1AcceptBunchCrossings_*_*_*', 
        'keep L1GlobalTriggerReadoutRecord_gtDigis_*_*', 
        'keep *_TriggerResults_*_*', 
        'keep DcsStatuss_scalersRawToDigi_*_*', 
        'keep Si*Cluster*_si*Clusters_*_*', 
        'keep recoMuons_muons1Leg_*_*')
)

process.OutALCARECOTkAlCosmics0THLT = cms.PSet(
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('pathALCARECOTkAlCosmicsCTF0THLT', 
            'pathALCARECOTkAlCosmicsCosmicTF0THLT', 
            'pathALCARECOTkAlCosmicsRegional0THLT')
    ),
    outputCommands = cms.untracked.vstring('drop *', 
        'keep *_ALCARECOTkAlCosmics*0T_*_*', 
        'keep siStripDigis_DetIdCollection_*_*', 
        'keep L1AcceptBunchCrossings_*_*_*', 
        'keep L1GlobalTriggerReadoutRecord_gtDigis_*_*', 
        'keep *_TriggerResults_*_*', 
        'keep DcsStatuss_scalersRawToDigi_*_*', 
        'keep Si*Cluster*_si*Clusters_*_*', 
        'keep recoMuons_muons1Leg_*_*')
)

process.OutALCARECOTkAlCosmics0THLT_noDrop = cms.PSet(
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('pathALCARECOTkAlCosmicsCTF0THLT', 
            'pathALCARECOTkAlCosmicsCosmicTF0THLT', 
            'pathALCARECOTkAlCosmicsRegional0THLT')
    ),
    outputCommands = cms.untracked.vstring('keep *_ALCARECOTkAlCosmics*0T_*_*', 
        'keep siStripDigis_DetIdCollection_*_*', 
        'keep L1AcceptBunchCrossings_*_*_*', 
        'keep L1GlobalTriggerReadoutRecord_gtDigis_*_*', 
        'keep *_TriggerResults_*_*', 
        'keep DcsStatuss_scalersRawToDigi_*_*', 
        'keep Si*Cluster*_si*Clusters_*_*', 
        'keep recoMuons_muons1Leg_*_*')
)

process.OutALCARECOTkAlCosmics0T_noDrop = cms.PSet(
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('pathALCARECOTkAlCosmicsCTF0T', 
            'pathALCARECOTkAlCosmicsCosmicTF0T', 
            'pathALCARECOTkAlCosmicsRegional0T')
    ),
    outputCommands = cms.untracked.vstring('keep *_ALCARECOTkAlCosmics*0T_*_*', 
        'keep siStripDigis_DetIdCollection_*_*', 
        'keep L1AcceptBunchCrossings_*_*_*', 
        'keep L1GlobalTriggerReadoutRecord_gtDigis_*_*', 
        'keep *_TriggerResults_*_*', 
        'keep DcsStatuss_scalersRawToDigi_*_*', 
        'keep Si*Cluster*_si*Clusters_*_*', 
        'keep recoMuons_muons1Leg_*_*')
)

process.OutALCARECOTkAlCosmicsHLT = cms.PSet(
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('pathALCARECOTkAlCosmicsCTFHLT', 
            'pathALCARECOTkAlCosmicsCosmicTFHLT', 
            'pathALCARECOTkAlCosmicsRegionalHLT')
    ),
    outputCommands = cms.untracked.vstring('drop *', 
        'keep *_ALCARECOTkAlCosmicsCTF_*_*', 
        'keep *_ALCARECOTkAlCosmicsCosmicTF_*_*', 
        'keep *_ALCARECOTkAlCosmicsRegional_*_*', 
        'keep siStripDigis_DetIdCollection_*_*', 
        'keep L1AcceptBunchCrossings_*_*_*', 
        'keep L1GlobalTriggerReadoutRecord_gtDigis_*_*', 
        'keep *_TriggerResults_*_*', 
        'keep DcsStatuss_scalersRawToDigi_*_*', 
        'keep Si*Cluster*_si*Clusters_*_*', 
        'keep recoMuons_muons1Leg_*_*')
)

process.OutALCARECOTkAlCosmicsHLT_noDrop = cms.PSet(
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('pathALCARECOTkAlCosmicsCTFHLT', 
            'pathALCARECOTkAlCosmicsCosmicTFHLT', 
            'pathALCARECOTkAlCosmicsRegionalHLT')
    ),
    outputCommands = cms.untracked.vstring('keep *_ALCARECOTkAlCosmicsCTF_*_*', 
        'keep *_ALCARECOTkAlCosmicsCosmicTF_*_*', 
        'keep *_ALCARECOTkAlCosmicsRegional_*_*', 
        'keep siStripDigis_DetIdCollection_*_*', 
        'keep L1AcceptBunchCrossings_*_*_*', 
        'keep L1GlobalTriggerReadoutRecord_gtDigis_*_*', 
        'keep *_TriggerResults_*_*', 
        'keep DcsStatuss_scalersRawToDigi_*_*', 
        'keep Si*Cluster*_si*Clusters_*_*', 
        'keep recoMuons_muons1Leg_*_*')
)

process.OutALCARECOTkAlCosmicsInCollisions = cms.PSet(
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('pathALCARECOTkAlCosmicsInCollisions')
    ),
    outputCommands = cms.untracked.vstring('drop *', 
        'keep *_ALCARECOTkAlCosmicsInCollisions_*_*', 
        'keep siStripDigis_DetIdCollection_*_*', 
        'keep L1AcceptBunchCrossings_*_*_*', 
        'keep L1GlobalTriggerReadoutRecord_gtDigis_*_*', 
        'keep *_TriggerResults_*_*', 
        'keep DcsStatuss_scalersRawToDigi_*_*', 
        'keep Si*Cluster*_si*Clusters_*_*', 
        'keep recoMuons_muons1Leg_*_*')
)

process.OutALCARECOTkAlCosmicsInCollisions_noDrop = cms.PSet(
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('pathALCARECOTkAlCosmicsInCollisions')
    ),
    outputCommands = cms.untracked.vstring('keep *_ALCARECOTkAlCosmicsInCollisions_*_*', 
        'keep siStripDigis_DetIdCollection_*_*', 
        'keep L1AcceptBunchCrossings_*_*_*', 
        'keep L1GlobalTriggerReadoutRecord_gtDigis_*_*', 
        'keep *_TriggerResults_*_*', 
        'keep DcsStatuss_scalersRawToDigi_*_*', 
        'keep Si*Cluster*_si*Clusters_*_*', 
        'keep recoMuons_muons1Leg_*_*')
)

process.OutALCARECOTkAlCosmics_noDrop = cms.PSet(
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('pathALCARECOTkAlCosmicsCTF', 
            'pathALCARECOTkAlCosmicsCosmicTF', 
            'pathALCARECOTkAlCosmicsRegional')
    ),
    outputCommands = cms.untracked.vstring('keep *_ALCARECOTkAlCosmicsCTF_*_*', 
        'keep *_ALCARECOTkAlCosmicsCosmicTF_*_*', 
        'keep *_ALCARECOTkAlCosmicsRegional_*_*', 
        'keep siStripDigis_DetIdCollection_*_*', 
        'keep L1AcceptBunchCrossings_*_*_*', 
        'keep L1GlobalTriggerReadoutRecord_gtDigis_*_*', 
        'keep *_TriggerResults_*_*', 
        'keep DcsStatuss_scalersRawToDigi_*_*', 
        'keep Si*Cluster*_si*Clusters_*_*', 
        'keep recoMuons_muons1Leg_*_*')
)

process.OutALCARECOTkAlJpsiMuMu = cms.PSet(
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('pathALCARECOTkAlJpsiMuMu')
    ),
    outputCommands = cms.untracked.vstring('drop *', 
        'keep *_ALCARECOTkAlJpsiMuMu_*_*', 
        'keep L1AcceptBunchCrossings_*_*_*', 
        'keep L1GlobalTriggerReadoutRecord_gtDigis_*_*', 
        'keep *_TriggerResults_*_*', 
        'keep DcsStatuss_scalersRawToDigi_*_*', 
        'keep *_offlinePrimaryVertices_*_*')
)

process.OutALCARECOTkAlJpsiMuMu_noDrop = cms.PSet(
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('pathALCARECOTkAlJpsiMuMu')
    ),
    outputCommands = cms.untracked.vstring('keep *_ALCARECOTkAlJpsiMuMu_*_*', 
        'keep L1AcceptBunchCrossings_*_*_*', 
        'keep L1GlobalTriggerReadoutRecord_gtDigis_*_*', 
        'keep *_TriggerResults_*_*', 
        'keep DcsStatuss_scalersRawToDigi_*_*', 
        'keep *_offlinePrimaryVertices_*_*')
)

process.OutALCARECOTkAlLAS = cms.PSet(
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('pathALCARECOTkAlLAS')
    ),
    outputCommands = cms.untracked.vstring('drop *', 
        'keep *_ALCARECOTkAlLAST0Producer_*_*', 
        'keep DcsStatuss_scalersRawToDigi_*_*')
)

process.OutALCARECOTkAlLAS_noDrop = cms.PSet(
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('pathALCARECOTkAlLAS')
    ),
    outputCommands = cms.untracked.vstring('keep *_ALCARECOTkAlLAST0Producer_*_*', 
        'keep DcsStatuss_scalersRawToDigi_*_*')
)

process.OutALCARECOTkAlMinBias = cms.PSet(
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('pathALCARECOTkAlMinBias')
    ),
    outputCommands = cms.untracked.vstring('drop *', 
        'keep *_ALCARECOTkAlMinBias_*_*', 
        'keep L1AcceptBunchCrossings_*_*_*', 
        'keep L1GlobalTriggerReadoutRecord_gtDigis_*_*', 
        'keep *_TriggerResults_*_*', 
        'keep DcsStatuss_scalersRawToDigi_*_*', 
        'keep *_offlinePrimaryVertices_*_*', 
        'keep *_offlineBeamSpot_*_*')
)

process.OutALCARECOTkAlMinBiasHI = cms.PSet(
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('pathALCARECOTkAlMinBiasHI')
    ),
    outputCommands = cms.untracked.vstring('drop *', 
        'keep *_ALCARECOTkAlMinBiasHI_*_*', 
        'keep L1AcceptBunchCrossings_*_*_*', 
        'keep L1GlobalTriggerReadoutRecord_gtDigis_*_*', 
        'keep *_TriggerResults_*_*', 
        'keep DcsStatuss_scalersRawToDigi_*_*', 
        'keep *_hiSelectedVertex_*_*', 
        'keep *_offlineBeamSpot_*_*')
)

process.OutALCARECOTkAlMinBiasHI_noDrop = cms.PSet(
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('pathALCARECOTkAlMinBiasHI')
    ),
    outputCommands = cms.untracked.vstring('keep *_ALCARECOTkAlMinBiasHI_*_*', 
        'keep L1AcceptBunchCrossings_*_*_*', 
        'keep L1GlobalTriggerReadoutRecord_gtDigis_*_*', 
        'keep *_TriggerResults_*_*', 
        'keep DcsStatuss_scalersRawToDigi_*_*', 
        'keep *_hiSelectedVertex_*_*', 
        'keep *_offlineBeamSpot_*_*')
)

process.OutALCARECOTkAlMinBias_noDrop = cms.PSet(
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('pathALCARECOTkAlMinBias')
    ),
    outputCommands = cms.untracked.vstring('keep *_ALCARECOTkAlMinBias_*_*', 
        'keep L1AcceptBunchCrossings_*_*_*', 
        'keep L1GlobalTriggerReadoutRecord_gtDigis_*_*', 
        'keep *_TriggerResults_*_*', 
        'keep DcsStatuss_scalersRawToDigi_*_*', 
        'keep *_offlinePrimaryVertices_*_*', 
        'keep *_offlineBeamSpot_*_*')
)

process.OutALCARECOTkAlMuonIsolated = cms.PSet(
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('pathALCARECOTkAlMuonIsolated')
    ),
    outputCommands = cms.untracked.vstring('drop *', 
        'keep *_ALCARECOTkAlMuonIsolated_*_*', 
        'keep L1AcceptBunchCrossings_*_*_*', 
        'keep L1GlobalTriggerReadoutRecord_gtDigis_*_*', 
        'keep *_TriggerResults_*_*', 
        'keep DcsStatuss_scalersRawToDigi_*_*', 
        'keep *_offlinePrimaryVertices_*_*')
)

process.OutALCARECOTkAlMuonIsolatedPA = cms.PSet(
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('pathALCARECOTkAlMuonIsolatedPA')
    ),
    outputCommands = cms.untracked.vstring('drop *', 
        'keep *_ALCARECOTkAlMuonIsolatedPA_*_*', 
        'keep L1AcceptBunchCrossings_*_*_*', 
        'keep L1GlobalTriggerReadoutRecord_gtDigis_*_*', 
        'keep *_TriggerResults_*_*', 
        'keep DcsStatuss_scalersRawToDigi_*_*', 
        'keep *_offlinePrimaryVertices_*_*')
)

process.OutALCARECOTkAlMuonIsolatedPA_noDrop = cms.PSet(
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('pathALCARECOTkAlMuonIsolatedPA')
    ),
    outputCommands = cms.untracked.vstring('keep *_ALCARECOTkAlMuonIsolatedPA_*_*', 
        'keep L1AcceptBunchCrossings_*_*_*', 
        'keep L1GlobalTriggerReadoutRecord_gtDigis_*_*', 
        'keep *_TriggerResults_*_*', 
        'keep DcsStatuss_scalersRawToDigi_*_*', 
        'keep *_offlinePrimaryVertices_*_*')
)

process.OutALCARECOTkAlMuonIsolated_noDrop = cms.PSet(
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('pathALCARECOTkAlMuonIsolated')
    ),
    outputCommands = cms.untracked.vstring('keep *_ALCARECOTkAlMuonIsolated_*_*', 
        'keep L1AcceptBunchCrossings_*_*_*', 
        'keep L1GlobalTriggerReadoutRecord_gtDigis_*_*', 
        'keep *_TriggerResults_*_*', 
        'keep DcsStatuss_scalersRawToDigi_*_*', 
        'keep *_offlinePrimaryVertices_*_*')
)

process.OutALCARECOTkAlUpsilonMuMu = cms.PSet(
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('pathALCARECOTkAlUpsilonMuMu')
    ),
    outputCommands = cms.untracked.vstring('drop *', 
        'keep *_ALCARECOTkAlUpsilonMuMu_*_*', 
        'keep L1AcceptBunchCrossings_*_*_*', 
        'keep L1GlobalTriggerReadoutRecord_gtDigis_*_*', 
        'keep *_TriggerResults_*_*', 
        'keep DcsStatuss_scalersRawToDigi_*_*', 
        'keep *_offlinePrimaryVertices_*_*')
)

process.OutALCARECOTkAlUpsilonMuMu_noDrop = cms.PSet(
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('pathALCARECOTkAlUpsilonMuMu')
    ),
    outputCommands = cms.untracked.vstring('keep *_ALCARECOTkAlUpsilonMuMu_*_*', 
        'keep L1AcceptBunchCrossings_*_*_*', 
        'keep L1GlobalTriggerReadoutRecord_gtDigis_*_*', 
        'keep *_TriggerResults_*_*', 
        'keep DcsStatuss_scalersRawToDigi_*_*', 
        'keep *_offlinePrimaryVertices_*_*')
)

process.OutALCARECOTkAlZMuMu = cms.PSet(
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('pathALCARECOTkAlZMuMu')
    ),
    outputCommands = cms.untracked.vstring('drop *', 
        'keep *_ALCARECOTkAlZMuMu_*_*', 
        'keep L1AcceptBunchCrossings_*_*_*', 
        'keep L1GlobalTriggerReadoutRecord_gtDigis_*_*', 
        'keep *_TriggerResults_*_*', 
        'keep DcsStatuss_scalersRawToDigi_*_*', 
        'keep *_offlinePrimaryVertices_*_*')
)

process.OutALCARECOTkAlZMuMu_noDrop = cms.PSet(
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('pathALCARECOTkAlZMuMu')
    ),
    outputCommands = cms.untracked.vstring('keep *_ALCARECOTkAlZMuMu_*_*', 
        'keep L1AcceptBunchCrossings_*_*_*', 
        'keep L1GlobalTriggerReadoutRecord_gtDigis_*_*', 
        'keep *_TriggerResults_*_*', 
        'keep DcsStatuss_scalersRawToDigi_*_*', 
        'keep *_offlinePrimaryVertices_*_*')
)

process.PREMIXEventContent = cms.PSet(
    splitLevel = cms.untracked.int32(0),
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
    outputCommands = cms.untracked.vstring('drop *', 
        'drop *', 
        'drop *', 
        'keep  FEDRawDataCollection_rawDataCollector_*_*', 
        'keep  FEDRawDataCollection_source_*_*', 
        'keep  FEDRawDataCollection_rawDataCollector_*_*', 
        'keep  FEDRawDataCollection_source_*_*', 
        'drop *_hlt*_*_*', 
        'keep *_hltL1GtObjectMap_*_*', 
        'keep FEDRawDataCollection_rawDataCollector_*_*', 
        'keep FEDRawDataCollection_source_*_*', 
        'keep edmTriggerResults_*_*_*', 
        'keep triggerTriggerEvent_*_*_*', 
        'keep LHERunInfoProduct_*_*_*', 
        'keep LHEEventProduct_*_*_*', 
        'keep GenRunInfoProduct_generator_*_*', 
        'keep GenLumiInfoProduct_generator_*_*', 
        'keep GenEventInfoProduct_generator_*_*', 
        'keep edmHepMCProduct_generator_*_*', 
        'keep GenFilterInfo_*_*_*', 
        'keep *_genParticles_*_*', 
        'keep edmHepMCProduct_source_*_*', 
        'keep SimTracks_g4SimHits_*_*', 
        'keep SimVertexs_g4SimHits_*_*', 
        'keep *_allTrackMCMatch_*_*', 
        'keep StripDigiSimLinkedmDetSetVector_simMuonCSCDigis_*_*', 
        'keep DTLayerIdDTDigiSimLinkMuonDigiCollection_simMuonDTDigis_*_*', 
        'keep RPCDigiSimLinkedmDetSetVector_simMuonRPCDigis_*_*', 
        'keep PileupSummaryInfos_*_*_*', 
        'keep recoGenMETs_*_*_*', 
        'keep recoGenJets_*_*_*', 
        'keep *_genParticle_*_*', 
        'keep *_MEtoEDMConverter_*_*', 
        'keep *_randomEngineStateProducer_*_*', 
        'keep FEDRawDataCollection_source_*_*', 
        'keep FEDRawDataCollection_rawDataCollector_*_*', 
        'keep *_logErrorHarvester_*_*', 
        'keep RPCDetIdRPCDigiMuonDigiCollection_simMuonRPCDigis_*_*', 
        'keep CrossingFramePlaybackInfoExtended_*_*_*')
)

process.PREMIXRAWEventContent = cms.PSet(
    splitLevel = cms.untracked.int32(0),
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
    outputCommands = cms.untracked.vstring('drop *', 
        'drop *', 
        'drop *', 
        'keep  FEDRawDataCollection_rawDataCollector_*_*', 
        'keep  FEDRawDataCollection_source_*_*', 
        'keep  FEDRawDataCollection_rawDataCollector_*_*', 
        'keep  FEDRawDataCollection_source_*_*', 
        'drop *_hlt*_*_*', 
        'keep *_hltL1GtObjectMap_*_*', 
        'keep FEDRawDataCollection_rawDataCollector_*_*', 
        'keep FEDRawDataCollection_source_*_*', 
        'keep edmTriggerResults_*_*_*', 
        'keep triggerTriggerEvent_*_*_*', 
        'keep *_g4SimHits_*_*', 
        'keep edmHepMCProduct_source_*_*', 
        'keep *_allTrackMCMatch_*_*', 
        'keep StripDigiSimLinkedmDetSetVector_simMuonCSCDigis_*_*', 
        'keep CSCDetIdCSCComparatorDigiMuonDigiCollection_simMuonCSCDigis_*_*', 
        'keep DTLayerIdDTDigiSimLinkMuonDigiCollection_simMuonDTDigis_*_*', 
        'keep RPCDigiSimLinkedmDetSetVector_simMuonRPCDigis_*_*', 
        'keep EBSrFlagsSorted_simEcalDigis_*_*', 
        'keep EESrFlagsSorted_simEcalDigis_*_*', 
        'keep CrossingFramePlaybackInfoExtended_*_*_*', 
        'keep PileupSummaryInfos_*_*_*', 
        'keep LHERunInfoProduct_*_*_*', 
        'keep LHEEventProduct_*_*_*', 
        'keep GenRunInfoProduct_generator_*_*', 
        'keep GenLumiInfoProduct_generator_*_*', 
        'keep GenEventInfoProduct_generator_*_*', 
        'keep edmHepMCProduct_generator_*_*', 
        'keep GenFilterInfo_*_*_*', 
        'keep *_genParticles_*_*', 
        'keep recoGenJets_*_*_*', 
        'keep *_genParticle_*_*', 
        'keep recoGenMETs_*_*_*', 
        'keep FEDRawDataCollection_source_*_*', 
        'keep FEDRawDataCollection_rawDataCollector_*_*', 
        'keep *_MEtoEDMConverter_*_*', 
        'keep *_randomEngineStateProducer_*_*', 
        'keep *_logErrorHarvester_*_*', 
        'keep CrossingFramePlaybackInfoExtended_*_*_*', 
        'drop CrossingFramePlaybackInfoExtended_mix_*_*', 
        'drop PileupSummaryInfos_addPileupInfo_*_*')
)

process.RAWDEBUGEventContent = cms.PSet(
    splitLevel = cms.untracked.int32(0),
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
    outputCommands = cms.untracked.vstring('drop *', 
        'drop *', 
        'drop *', 
        'keep  FEDRawDataCollection_rawDataCollector_*_*', 
        'keep  FEDRawDataCollection_source_*_*', 
        'keep  FEDRawDataCollection_rawDataCollector_*_*', 
        'keep  FEDRawDataCollection_source_*_*', 
        'drop *_hlt*_*_*', 
        'keep *_hltL1GtObjectMap_*_*', 
        'keep FEDRawDataCollection_rawDataCollector_*_*', 
        'keep FEDRawDataCollection_source_*_*', 
        'keep edmTriggerResults_*_*_*', 
        'keep triggerTriggerEvent_*_*_*', 
        'keep *_g4SimHits_*_*', 
        'keep edmHepMCProduct_source_*_*', 
        'keep *_allTrackMCMatch_*_*', 
        'keep StripDigiSimLinkedmDetSetVector_simMuonCSCDigis_*_*', 
        'keep CSCDetIdCSCComparatorDigiMuonDigiCollection_simMuonCSCDigis_*_*', 
        'keep DTLayerIdDTDigiSimLinkMuonDigiCollection_simMuonDTDigis_*_*', 
        'keep RPCDigiSimLinkedmDetSetVector_simMuonRPCDigis_*_*', 
        'keep EBSrFlagsSorted_simEcalDigis_*_*', 
        'keep EESrFlagsSorted_simEcalDigis_*_*', 
        'keep CrossingFramePlaybackInfoExtended_*_*_*', 
        'keep PileupSummaryInfos_*_*_*', 
        'keep LHERunInfoProduct_*_*_*', 
        'keep LHEEventProduct_*_*_*', 
        'keep GenRunInfoProduct_generator_*_*', 
        'keep GenLumiInfoProduct_generator_*_*', 
        'keep GenEventInfoProduct_generator_*_*', 
        'keep edmHepMCProduct_generator_*_*', 
        'keep GenFilterInfo_*_*_*', 
        'keep *_genParticles_*_*', 
        'keep recoGenJets_*_*_*', 
        'keep *_genParticle_*_*', 
        'keep recoGenMETs_*_*_*', 
        'keep FEDRawDataCollection_source_*_*', 
        'keep FEDRawDataCollection_rawDataCollector_*_*', 
        'keep *_MEtoEDMConverter_*_*', 
        'keep *_randomEngineStateProducer_*_*', 
        'keep *_logErrorHarvester_*_*', 
        'keep PixelDigiSimLinkedmDetSetVector_simSiPixelDigis_*_*', 
        'keep StripDigiSimLinkedmDetSetVector_simSiStripDigis_*_*', 
        'drop *_mix_simSiPixelDigis*_*', 
        'drop *_mix_simSiStripDigis*_*', 
        'keep *_allTrackMCMatch_*_*', 
        'drop *_trackingtruthprod_*_*', 
        'drop *_electrontruth_*_*', 
        'keep *_mix_MergedTrackTruth_*', 
        'keep CrossingFramePlaybackInfoExtended_*_*_*', 
        'keep  FEDRawDataCollection_rawDataCollector_*_*', 
        'keep  FEDRawDataCollection_source_*_*', 
        'keep L1GlobalTriggerReadoutRecord_gtDigis_*_*', 
        'keep *_l1GtRecord_*_*', 
        'keep *_l1GtTriggerMenuLite_*_*', 
        'keep *_conditionsInEdm_*_*', 
        'keep *_l1extraParticles_*_*')
)

process.RAWDEBUGHLTEventContent = cms.PSet(
    splitLevel = cms.untracked.int32(0),
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
    outputCommands = cms.untracked.vstring( ('drop *', 
        'drop *', 
        'drop *', 
        'drop *', 
        'keep  FEDRawDataCollection_rawDataCollector_*_*', 
        'keep  FEDRawDataCollection_source_*_*', 
        'keep  FEDRawDataCollection_rawDataCollector_*_*', 
        'keep  FEDRawDataCollection_source_*_*', 
        'drop *_hlt*_*_*', 
        'keep *_hltL1GtObjectMap_*_*', 
        'keep FEDRawDataCollection_rawDataCollector_*_*', 
        'keep FEDRawDataCollection_source_*_*', 
        'keep edmTriggerResults_*_*_*', 
        'keep triggerTriggerEvent_*_*_*', 
        'keep *_g4SimHits_*_*', 
        'keep edmHepMCProduct_source_*_*', 
        'keep *_allTrackMCMatch_*_*', 
        'keep StripDigiSimLinkedmDetSetVector_simMuonCSCDigis_*_*', 
        'keep CSCDetIdCSCComparatorDigiMuonDigiCollection_simMuonCSCDigis_*_*', 
        'keep DTLayerIdDTDigiSimLinkMuonDigiCollection_simMuonDTDigis_*_*', 
        'keep RPCDigiSimLinkedmDetSetVector_simMuonRPCDigis_*_*', 
        'keep EBSrFlagsSorted_simEcalDigis_*_*', 
        'keep EESrFlagsSorted_simEcalDigis_*_*', 
        'keep CrossingFramePlaybackInfoExtended_*_*_*', 
        'keep PileupSummaryInfos_*_*_*', 
        'keep LHERunInfoProduct_*_*_*', 
        'keep LHEEventProduct_*_*_*', 
        'keep GenRunInfoProduct_generator_*_*', 
        'keep GenLumiInfoProduct_generator_*_*', 
        'keep GenEventInfoProduct_generator_*_*', 
        'keep edmHepMCProduct_generator_*_*', 
        'keep GenFilterInfo_*_*_*', 
        'keep *_genParticles_*_*', 
        'keep recoGenJets_*_*_*', 
        'keep *_genParticle_*_*', 
        'keep recoGenMETs_*_*_*', 
        'keep FEDRawDataCollection_source_*_*', 
        'keep FEDRawDataCollection_rawDataCollector_*_*', 
        'keep *_MEtoEDMConverter_*_*', 
        'keep *_randomEngineStateProducer_*_*', 
        'keep *_logErrorHarvester_*_*', 
        'keep PixelDigiSimLinkedmDetSetVector_simSiPixelDigis_*_*', 
        'keep StripDigiSimLinkedmDetSetVector_simSiStripDigis_*_*', 
        'drop *_mix_simSiPixelDigis*_*', 
        'drop *_mix_simSiStripDigis*_*', 
        'keep *_allTrackMCMatch_*_*', 
        'drop *_trackingtruthprod_*_*', 
        'drop *_electrontruth_*_*', 
        'keep *_mix_MergedTrackTruth_*', 
        'keep CrossingFramePlaybackInfoExtended_*_*_*', 
        'keep  FEDRawDataCollection_rawDataCollector_*_*', 
        'keep  FEDRawDataCollection_source_*_*', 
        'keep L1GlobalTriggerReadoutRecord_gtDigis_*_*', 
        'keep *_l1GtRecord_*_*', 
        'keep *_l1GtTriggerMenuLite_*_*', 
        'keep *_conditionsInEdm_*_*', 
        'keep *_l1extraParticles_*_*', 
        'drop *_hlt*_*_*', 
        'keep *_hltAK4CaloJetsCorrectedIDPassed_*_*', 
        'keep *_hltAK4CaloJetsIDPassed_*_*', 
        'keep *_hltAK4CaloJets_*_*', 
        'keep *_hltAK4PFJetsCorrected_*_*', 
        'keep *_hltAK4PFJetsForTaus_*_*', 
        'keep *_hltAK4PFJets_*_*', 
        'keep *_hltAlCaEtaEBUncalibrator_*_*', 
        'keep *_hltAlCaEtaEEUncalibrator_*_*', 
        'keep *_hltAlCaEtaRecHitsFilterEBonly_*_*', 
        'keep *_hltAlCaEtaRecHitsFilterEEonly_*_*', 
        'keep *_hltAlCaEtaRecHitsFilterEEonly_etaEcalRecHitsES_*', 
        'keep *_hltAlCaEtaRecHitsFilter_*_*', 
        'keep *_hltAlCaPhiSymStream_*_*', 
        'keep *_hltAlCaPhiSymUncalibrator_*_*', 
        'keep *_hltAlCaPi0EBUncalibrator_*_*', 
        'keep *_hltAlCaPi0EEUncalibrator_*_*', 
        'keep *_hltAlCaPi0RecHitsFilterEBonly_*_*', 
        'keep *_hltAlCaPi0RecHitsFilterEEonly_*_*', 
        'keep *_hltAlCaPi0RecHitsFilterEEonly_pi0EcalRecHitsES_*', 
        'keep *_hltAlCaPi0RecHitsFilter_*_*', 
        'keep *_hltBLifetimeL25AssociatorbbPhiL1FastJetFastPV_*_*', 
        'keep *_hltBLifetimeL25BJetTagsbbPhiL1FastJetFastPV_*_*', 
        'keep *_hltBLifetimeL25TagInfosbbPhiL1FastJetFastPV_*_*', 
        'keep *_hltBLifetimeL3AssociatorbbPhiL1FastJetFastPV_*_*', 
        'keep *_hltBLifetimeL3BJetTagsbbPhiL1FastJetFastPV_*_*', 
        'keep *_hltBLifetimeL3TagInfosbbPhiL1FastJetFastPV_*_*', 
        'keep *_hltBLifetimeRegionalCtfWithMaterialTracksbbPhiL1FastJetFastPV_*_*', 
        'keep *_hltBSoftMuonDiJet110Mu5L3FilterByDR_*_*', 
        'keep *_hltBSoftMuonDiJet110Mu5SelL3BJetTagsByDR_*_*', 
        'keep *_hltBSoftMuonDiJet20Mu5L3FilterByDR_*_*', 
        'keep *_hltBSoftMuonDiJet20Mu5SelL3BJetTagsByDR_*_*', 
        'keep *_hltBSoftMuonDiJet40Mu5L3FilterByDR_*_*', 
        'keep *_hltBSoftMuonDiJet40Mu5SelL3BJetTagsByDR_*_*', 
        'keep *_hltBSoftMuonDiJet70Mu5L3FilterByDR_*_*', 
        'keep *_hltBSoftMuonDiJet70Mu5SelL3BJetTagsByDR_*_*', 
        'keep *_hltBSoftMuonMu5L3_*_*', 
        'keep *_hltCSVJetTagSingleTopEle27_*_*', 
        'keep *_hltCSVJetTagSingleTopIsoMu24_*_*', 
        'keep *_hltCaloJetCorrectedRegional_*_*', 
        'keep *_hltCaloJetCorrected_*_*', 
        'keep *_hltCaloJetL1FastJetCorrected_*_*', 
        'keep *_hltCleanedCorrectedIslandBarrelSuperClustersHI_*_*', 
        'keep *_hltCleanedHiCorrectedIslandBarrelSuperClustersHI_*_*', 
        'keep *_hltConvPFTausTightIsoTrackFindingIsolation_*_*', 
        'keep *_hltConvPFTausTightIsoTrackFinding_*_*', 
        'keep *_hltConvPFTausTightIsoTrackPt5Isolation_*_*', 
        'keep *_hltConvPFTausTightIsoTrackPt5_*_*', 
        'keep *_hltConvPFTausTightIso_*_*', 
        'keep *_hltConvPFTausTrackFindingLooseIsolation_*_*', 
        'keep *_hltConvPFTausTrackFinding_*_*', 
        'keep *_hltConvPFTaus_*_*', 
        'keep *_hltCorrectedIslandBarrelSuperClustersHI_*_*', 
        'keep *_hltCorrectedIslandEndcapSuperClustersHI_*_*', 
        'keep *_hltCsc2DRecHits_*_*', 
        'keep *_hltCscSegments_*_*', 
        'keep *_hltDoublePFTau25TrackPt5MediumIsolationProng4L1HLTMatched_*_*', 
        'keep *_hltDoublePFTau25TrackPt5MediumIsolationProng4_*_*', 
        'keep *_hltDoublePFTau25TrackPt5MediumIsolation_*_*', 
        'keep *_hltDoublePFTau25TrackPt5_*_*', 
        'keep *_hltDoublePFTau25_*_*', 
        'keep *_hltDoublePFTauTightIso45Track5_*_*', 
        'keep *_hltDoublePFTauTightIso45Track_*_*', 
        'keep *_hltDt4DSegments_*_*', 
        'keep *_hltEcalRecHitAll_*_*', 
        'keep *_hltEle20CaloIdVTCaloIsoTTrkIdTTrkIsoTTrackIsoFilterL1SingleEG18orL1SingleEG20_*_*', 
        'keep *_hltEle20CaloIdVTTrkIdTDphiFilter_*_*', 
        'keep *_hltEle27WP85PixelMatchElectronsL1Seeded_*_*', 
        'keep *_hltFastPVPixelTracksMerger_*_*', 
        'keep *_hltFastPVPixelTracksRecover_*_*', 
        'keep *_hltFastPVPixelTracks_*_*', 
        'keep *_hltFastPVPixelVertices3D_*_*', 
        'keep *_hltFastPVPixelVertices_*_*', 
        'keep *_hltFastPixelBLifetimeL3TagInfos_*_*', 
        'keep *_hltFastPrimaryVertex_*_*', 
        'keep *_hltFilterDoubleIsoPFTau45Trk5LeadTrack5IsolationL1HLTMatched_*_*', 
        'keep *_hltFilterL2EtCutDoublePFIsoTau45Trk5_*_*', 
        'keep *_hltFilterL2EtCutSingleIsoPFTau35Trk20MET70_*_*', 
        'keep *_hltFilterSingleIsoPFTau35Trk20LeadTrackPt20_*_*', 
        'keep *_hltFilterSingleIsoPFTau35Trk20MET60LeadTrack20IsolationL1HLTMatched_*_*', 
        'keep *_hltFilterSingleIsoPFTau35Trk20MET70LeadTrack20IsolationL1HLTMatched_*_*', 
        'keep *_hltGoodOnlinePVs_*_*', 
        'keep *_hltHICaloJetCorrected_*_*', 
        'keep *_hltHICaloJetIDPassed_*_*', 
        'keep *_hltHIGoodLooseTracks_*_*', 
        'keep *_hltHIPixel3PrimTracks_*_*', 
        'keep *_hltHISelectedVertex_*_*', 
        'keep *_hltHISiPixelClusters_*_*', 
        'keep *_hltHITIPTCorrectorHB_*_*', 
        'keep *_hltHITIPTCorrectorHE_*_*', 
        'keep *_hltHiCorrectedIslandBarrelSuperClustersHI_*_*', 
        'keep *_hltHiCorrectedIslandEndcapSuperClustersHI_*_*', 
        'keep *_hltHiIslandSuperClustersHI_*_*', 
        'keep *_hltIsolPixelTrackProdHB_*_*', 
        'keep *_hltIsolPixelTrackProdHE_*_*', 
        'keep *_hltIter0PFlowTrackSelectionHighPurity_*_*', 
        'keep *_hltIter1Merged_*_*', 
        'keep *_hltIter1PFlowTrackSelectionHighPurity_*_*', 
        'keep *_hltIter2Merged_*_*', 
        'keep *_hltIter2PFlowTrackSelectionHighPurity_*_*', 
        'keep *_hltIter3Merged_*_*', 
        'keep *_hltIter4Merged_*_*', 
        'keep *_hltIterativeCone5PileupSubtractionCaloJets_*_*', 
        'keep *_hltL1GtObjectMap_*_*', 
        'keep *_hltL1HLTSingleIsoPFTau35Trk20Met60JetsMatch_*_*', 
        'keep *_hltL1IsoElectronTrackIsol_*_*', 
        'keep *_hltL1NonIsoElectronTrackIsol_*_*', 
        'keep *_hltL1SeededRecoEcalCandidate_*_*', 
        'keep *_hltL1extraParticlesCentral_*_*', 
        'keep *_hltL1extraParticlesNonIsolated_*_*', 
        'keep *_hltL1extraParticlesTau_*_*', 
        'keep *_hltL1extraParticles_*_*', 
        'keep *_hltL1sDoubleTauJet44Eta2p17orDoubleJet64Central_*_*', 
        'keep *_hltL1sDoubleTauJet44erorDoubleJetC64_*_*', 
        'keep *_hltL1sL1EG18er_*_*', 
        'keep *_hltL1sL1ETM36ORETM40_*_*', 
        'keep *_hltL1sL1Jet52ETM30_*_*', 
        'keep *_hltL1sL1SingleEG12_*_*', 
        'keep *_hltL1sL1SingleEG15_*_*', 
        'keep *_hltL1sL1SingleEG18orL1SingleEG20_*_*', 
        'keep *_hltL1sL1SingleMu10_*_*', 
        'keep *_hltL1sL1SingleMu14Eta2p1_*_*', 
        'keep *_hltL1sMu16Eta2p1_*_*', 
        'keep *_hltL2MuonCandidatesNoVtx_*_*', 
        'keep *_hltL2MuonCandidates_*_*', 
        'keep *_hltL2MuonSeeds_*_*', 
        'keep *_hltL2Muons_*_*', 
        'keep *_hltL2TauJets_*_*', 
        'keep *_hltL3CombinedSecondaryVertexBJetTags_*_*', 
        'keep *_hltL3MuonCandidates_*_*', 
        'keep *_hltL3MuonsIOHit_*_*', 
        'keep *_hltL3MuonsLinksCombination_*_*', 
        'keep *_hltL3MuonsOIHit_*_*', 
        'keep *_hltL3MuonsOIState_*_*', 
        'keep *_hltL3Muons_*_*', 
        'keep *_hltL3NoFiltersNoVtxMuonCandidates_*_*', 
        'keep *_hltL3NoFiltersNoVtxMuons_*_*', 
        'keep *_hltL3SecondaryVertexTagInfos_*_*', 
        'keep *_hltL3TkFromL2OICombination_*_*', 
        'keep *_hltL3TkTracksFromL2IOHit_*_*', 
        'keep *_hltL3TkTracksFromL2OIHit_*_*', 
        'keep *_hltL3TkTracksFromL2OIState_*_*', 
        'keep *_hltL3TkTracksFromL2_*_*', 
        'keep *_hltL3TrackCandidateFromL2IOHit_*_*', 
        'keep *_hltL3TrackCandidateFromL2OIHit_*_*', 
        'keep *_hltL3TrackCandidateFromL2OIState_*_*', 
        'keep *_hltL3TrajSeedIOHit_*_*', 
        'keep *_hltL3TrajSeedOIHit_*_*', 
        'keep *_hltL3TrajSeedOIState_*_*', 
        'keep *_hltL3TrajectorySeed_*_*', 
        'keep *_hltL3crIsoL1sMu16Eta2p1L1f0L2f16QL3f18QL3crIsoRhoFiltered0p15_*_*', 
        'keep *_hltLeadingCentralJets30SingleTopEle27_*_*', 
        'keep *_hltLeadingCentralJets30SingleTopIsoMu24_*_*', 
        'keep *_hltMetForHf_*_*', 
        'keep *_hltMet_*_*', 
        'keep *_hltMu8Ele17CaloIdTCaloIsoVLPixelMatchFilter_*_*', 
        'keep *_hltMuTrackJpsiCtfTrackCands_*_*', 
        'keep *_hltMuTrackJpsiPixelTrackCands_*_*', 
        'keep *_hltMuonCSCDigis_*_*', 
        'keep *_hltMuonCSCDigis_MuonCSCStripDigi_*', 
        'keep *_hltMuonCSCDigis_MuonCSCWireDigi_*', 
        'keep *_hltMuonDTDigis_*_*', 
        'keep *_hltMuonRPCDigis_*_*', 
        'keep *_hltOnlineBeamSpot_*_*', 
        'keep *_hltOnlinePrimaryVertices_*_*', 
        'keep *_hltOverlapFilterEle20LooseIsoPFTau20OldVersion_*_*', 
        'keep *_hltOverlapFilterIsoEle20MediumIsoPFTau20_*_*', 
        'keep *_hltOverlapFilterIsoMu15IsoPFTau20_*_*', 
        'keep *_hltOverlapFilterIsoMu15MediumIsoPFTau20_*_*', 
        'keep *_hltOverlapFilterIsoMu15TightIsoPFTau20_*_*', 
        'keep *_hltOverlapFilterIsoMu18LooseIsoPFTau20_*_*', 
        'keep *_hltOverlapFilterIsoMu18PFTau25TrackPt5Prong4_*_*', 
        'keep *_hltPFTau15TrackLooseIso_*_*', 
        'keep *_hltPFTau15Track_*_*', 
        'keep *_hltPFTau15_*_*', 
        'keep *_hltPFTau20IsoMuVertex_*_*', 
        'keep *_hltPFTau20TrackLooseIso_*_*', 
        'keep *_hltPFTau20Track_*_*', 
        'keep *_hltPFTau20_*_*', 
        'keep *_hltPFTau25TrackPt5MediumIsolationProng4IsoMuVertex_*_*', 
        'keep *_hltPFTau25TrackPt5MediumIsolationProng4_*_*', 
        'keep *_hltPFTau25TrackPt5MediumIsolation_*_*', 
        'keep *_hltPFTau25TrackPt5_*_*', 
        'keep *_hltPFTau25_*_*', 
        'keep *_hltPFTau35TrackPt20LooseIsoProng2_*_*', 
        'keep *_hltPFTau35TrackPt20LooseIso_*_*', 
        'keep *_hltPFTau35TrackPt20_*_*', 
        'keep *_hltPFTau35Track_*_*', 
        'keep *_hltPFTau35_*_*', 
        'keep *_hltPFTauEleVertex20_*_*', 
        'keep *_hltPFTauJetTracksAssociator_*_*', 
        'keep *_hltPFTauMediumIso20TrackMediumIso_*_*', 
        'keep *_hltPFTauMediumIso20Track_*_*', 
        'keep *_hltPFTauMediumIso20_*_*', 
        'keep *_hltPFTauMediumIso35Track_*_*', 
        'keep *_hltPFTauMediumIso35_*_*', 
        'keep *_hltPFTauTagInfo_*_*', 
        'keep *_hltPFTauTightIso20TrackTightIso_*_*', 
        'keep *_hltPFTauTightIso20Track_*_*', 
        'keep *_hltPFTauTightIso20_*_*', 
        'keep *_hltPFlowTrackSelectionHighPurity_*_*', 
        'keep *_hltParticleFlowForTaus_*_*', 
        'keep *_hltParticleFlow_*_*', 
        'keep *_hltPixelMatch3HitElectronsActivity_*_*', 
        'keep *_hltPixelMatch3HitElectronsL1Seeded_*_*', 
        'keep *_hltPixelMatchCleanElectronsL1Seeded_*_*', 
        'keep *_hltPixelMatchElectronsActivity_*_*', 
        'keep *_hltPixelMatchElectronsL1Iso_*_*', 
        'keep *_hltPixelMatchElectronsL1NonIso_*_*', 
        'keep *_hltPixelMatchElectronsL1Seeded_*_*', 
        'keep *_hltPixelTracks_*_*', 
        'keep *_hltPixelVertices3DbbPhi_*_*', 
        'keep *_hltPixelVertices_*_*', 
        'keep *_hltRecoEcalSuperClusterActivityCandidateSC4_*_*', 
        'keep *_hltRecoEcalSuperClusterActivityCandidateSC5_*_*', 
        'keep *_hltRecoEcalSuperClusterActivityCandidate_*_*', 
        'keep *_hltRpcRecHits_*_*', 
        'keep *_hltSelector4CentralJetsL1FastJet_*_*', 
        'keep *_hltSelectorJets20L1FastJet_*_*', 
        'keep *_hltSiPixelCluster_*_*', 
        'keep *_hltSiPixelClusters_*_*', 
        'keep *_hltSiStripClusters_*_*', 
        'keep *_hltSiStripRawToClustersFacility_*_*', 
        'keep *_hltSingleMu15L3Filtered15_*_*', 
        'keep *_hltSingleMuIsoL1s14L3IsoFiltered15eta2p1_*_*', 
        'keep *_hltSingleMuIsoL3IsoFiltered15_*_*', 
        'keep *_hltTowerMakerForAll_*_*', 
        'keep *_hltTowerMakerForMuons_*_*', 
        'keep *_hltTriggerSummaryAOD_*_*', 
        'keep *_hltTriggerSummaryRAW_*_*', 
        'keep *_hltTrimmedPixelVertices_*_*', 
        'keep *_hltVerticesL3_*_*', 
        'keep DcsStatuss_hltScalersRawToDigi_*_*', 
        'keep FEDRawDataCollection_rawDataCollector_*_*', 
        'keep FEDRawDataCollection_rawDataRepacker_*_*', 
        'keep FEDRawDataCollection_source_*_*', 
        'keep FEDRawDataCollection_virginRawDataRepacker_*_*', 
        'keep L1GlobalTriggerReadoutRecord_hltGtDigis_*_*', 
        'keep L1MuGMTCands_hltGtDigis_*_*', 
        'keep L1MuGMTReadoutCollection_hltGtDigis_*_*', 
        'keep L2MuonTrajectorySeeds_hltL2MuonSeeds_*_*', 
        'keep L3MuonTrajectorySeeds_hltHIL3TrajSeedOIHit_*_*', 
        'keep L3MuonTrajectorySeeds_hltHIL3TrajectorySeed_*_*', 
        'keep L3MuonTrajectorySeeds_hltL3TrajSeedOIState_*_*', 
        'keep LumiScalerss_hltScalersRawToDigi_*_*', 
        'keep SiPixelClusteredmNewDetSetVector_hltSiPixelClusters_*_*', 
        'keep TrackCandidates_hltHIL3TrackCandidateFromL2OIHit_*_*', 
        'keep TrackCandidates_hltHIL3TrackCandidateFromL2OIState_*_*', 
        'keep TrackingRecHitsOwned_hltL3Muons_*_*', 
        'keep edmTriggerResults_*_*_*', 
        'keep recoCaloJets_*_*_*', 
        'keep recoCaloMETs_*_*_*', 
        'keep recoCaloMETs_hltMet_*_*', 
        'keep recoCompositeCandidates_*_*_*', 
        'keep recoElectrons_*_*_*', 
        'keep recoIsolatedPixelTrackCandidates_*_*_*', 
        'keep recoMETs_*_*_*', 
        'keep recoPFJets_*_*_*', 
        'keep recoPFTaus_*_*_*', 
        'keep recoRecoChargedCandidates_*_*_*', 
        'keep recoRecoChargedCandidates_hltHIL3MuonCandidates_*_*', 
        'keep recoRecoChargedCandidates_hltL2MuonCandidates_*_*', 
        'keep recoRecoEcalCandidatesToValuefloatAssociation_hltL1IsoHLTClusterShape_*_*', 
        'keep recoRecoEcalCandidatesToValuefloatAssociation_hltL1IsolatedPhotonEcalIsol_*_*', 
        'keep recoRecoEcalCandidatesToValuefloatAssociation_hltL1IsolatedPhotonHcalForHE_*_*', 
        'keep recoRecoEcalCandidatesToValuefloatAssociation_hltL1IsolatedPhotonHcalIsol_*_*', 
        'keep recoRecoEcalCandidatesToValuefloatAssociation_hltL1NonIsoHLTClusterShape_*_*', 
        'keep recoRecoEcalCandidatesToValuefloatAssociation_hltL1NonIsolatedPhotonEcalIsol_*_*', 
        'keep recoRecoEcalCandidatesToValuefloatAssociation_hltL1NonIsolatedPhotonHcalForHE_*_*', 
        'keep recoRecoEcalCandidatesToValuefloatAssociation_hltL1NonIsolatedPhotonHcalIsol_*_*', 
        'keep recoRecoEcalCandidates_*_*_*', 
        'keep recoRecoEcalCandidates_hltL1IsoRecoEcalCandidate_*_*', 
        'keep recoRecoEcalCandidates_hltL1NonIsoRecoEcalCandidate_*_*', 
        'keep recoTrackExtras_hltHIL3MuonsOIHit_*_*', 
        'keep recoTrackExtras_hltHIL3MuonsOIState_*_*', 
        'keep recoTracks_hltHIL3MuonsOIHit_*_*', 
        'keep recoTracks_hltHIL3MuonsOIState_*_*', 
        'keep recoTracks_hltHIL3Muons_*_*', 
        'keep recoTracks_hltHIL3TkTracksFromL2OIHit_*_*', 
        'keep recoTracks_hltHIL3TkTracksFromL2OIState_*_*', 
        'keep recoTracks_hltHIL3TkTracksFromL2_*_*', 
        'keep triggerTriggerEventWithRefs_*_*_*', 
        'keep triggerTriggerEvent_*_*_*', 
        'keep triggerTriggerFilterObjectWithRefs_*_*_*' ) )
)

process.RAWEventContent = cms.PSet(
    splitLevel = cms.untracked.int32(0),
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
    outputCommands = cms.untracked.vstring('drop *', 
        'keep  FEDRawDataCollection_rawDataCollector_*_*', 
        'keep  FEDRawDataCollection_source_*_*', 
        'keep  FEDRawDataCollection_rawDataCollector_*_*', 
        'keep  FEDRawDataCollection_source_*_*', 
        'drop *_hlt*_*_*', 
        'keep *_hltL1GtObjectMap_*_*', 
        'keep FEDRawDataCollection_rawDataCollector_*_*', 
        'keep FEDRawDataCollection_source_*_*', 
        'keep edmTriggerResults_*_*_*', 
        'keep triggerTriggerEvent_*_*_*')
)

process.RAWRECODEBUGHLTEventContent = cms.PSet(
    splitLevel = cms.untracked.int32(0),
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
    outputCommands = cms.untracked.vstring( ('drop *', 
        'drop *', 
        'drop *', 
        'drop *', 
        'keep DetIdedmEDCollection_siStripDigis_*_*', 
        'keep DetIdedmEDCollection_siPixelDigis_*_*', 
        'keep *_siPixelClusters_*_*', 
        'keep *_siStripClusters_*_*', 
        'keep *_clusterSummaryProducer_*_*', 
        'keep *_dt1DRecHits_*_*', 
        'keep *_dt4DSegments_*_*', 
        'keep *_dt1DCosmicRecHits_*_*', 
        'keep *_dt4DCosmicSegments_*_*', 
        'keep *_csc2DRecHits_*_*', 
        'keep *_cscSegments_*_*', 
        'keep *_rpcRecHits_*_*', 
        'keep *_hbhereco_*_*', 
        'keep *_hbheprereco_*_*', 
        'keep *_hfreco_*_*', 
        'keep *_horeco_*_*', 
        'keep HBHERecHitsSorted_hbherecoMB_*_*', 
        'keep HORecHitsSorted_horecoMB_*_*', 
        'keep HFRecHitsSorted_hfrecoMB_*_*', 
        'keep ZDCDataFramesSorted_*Digis_*_*', 
        'keep ZDCRecHitsSorted_*_*_*', 
        'keep *_reducedHcalRecHits_*_*', 
        'keep *_castorreco_*_*', 
        'keep HcalUnpackerReport_*_*_*', 
        'keep *_ecalPreshowerRecHit_*_*', 
        'keep *_ecalRecHit_*_*', 
        'keep *_ecalCompactTrigPrim_*_*', 
        'keep *_ecalTPSkim_*_*', 
        'keep *_selectDigi_*_*', 
        'keep EcalRecHitsSorted_reducedEcalRecHitsEE_*_*', 
        'keep EcalRecHitsSorted_reducedEcalRecHitsEB_*_*', 
        'keep EcalRecHitsSorted_reducedEcalRecHitsES_*_*', 
        'keep *_hybridSuperClusters_*_*', 
        'keep recoSuperClusters_correctedHybridSuperClusters_*_*', 
        'keep *_multi5x5SuperClusters_*_*', 
        'keep recoSuperClusters_multi5x5SuperClusters_*_*', 
        'keep recoSuperClusters_multi5x5SuperClustersWithPreshower_*_*', 
        'keep recoSuperClusters_correctedMulti5x5SuperClustersWithPreshower_*_*', 
        'keep recoPreshowerClusters_multi5x5SuperClustersWithPreshower_*_*', 
        'keep recoPreshowerClusterShapes_multi5x5PreshowerClusterShape_*_*', 
        'keep *_particleFlowSuperClusterECAL_*_*', 
        'drop recoClusterShapes_*_*_*', 
        'drop recoBasicClustersToOnerecoClusterShapesAssociation_*_*_*', 
        'drop recoBasicClusters_multi5x5BasicClusters_multi5x5BarrelBasicClusters_*', 
        'drop recoSuperClusters_multi5x5SuperClusters_multi5x5BarrelSuperClusters_*', 
        'keep *_CkfElectronCandidates_*_*', 
        'keep *_GsfGlobalElectronTest_*_*', 
        'keep *_electronMergedSeeds_*_*', 
        'keep recoGsfTracks_electronGsfTracks_*_*', 
        'keep recoGsfTrackExtras_electronGsfTracks_*_*', 
        'keep recoTrackExtras_electronGsfTracks_*_*', 
        'keep TrackingRecHitsOwned_electronGsfTracks_*_*', 
        'keep recoTracks_generalTracks_*_*', 
        'keep recoTrackExtras_generalTracks_*_*', 
        'keep TrackingRecHitsOwned_extraFromSeeds_*_*', 
        'keep uints_extraFromSeeds_*_*', 
        'keep TrackingRecHitsOwned_generalTracks_*_*', 
        'keep recoTracks_beamhaloTracks_*_*', 
        'keep recoTrackExtras_beamhaloTracks_*_*', 
        'keep TrackingRecHitsOwned_beamhaloTracks_*_*', 
        'keep recoTracks_regionalCosmicTracks_*_*', 
        'keep recoTrackExtras_regionalCosmicTracks_*_*', 
        'keep TrackingRecHitsOwned_regionalCosmicTracks_*_*', 
        'keep recoTracks_rsWithMaterialTracks_*_*', 
        'keep recoTrackExtras_rsWithMaterialTracks_*_*', 
        'keep TrackingRecHitsOwned_rsWithMaterialTracks_*_*', 
        'keep recoTracks_conversionStepTracks_*_*', 
        'keep recoTrackExtras_conversionStepTracks_*_*', 
        'keep TrackingRecHitsOwned_conversionStepTracks_*_*', 
        'keep *_ctfPixelLess_*_*', 
        'keep *_dedxTruncated40_*_*', 
        'keep *_dedxDiscrimASmi_*_*', 
        'keep *_dedxHarmonic2_*_*', 
        'keep *_trackExtrapolator_*_*', 
        'keep floatedmValueMap_generalTracks_*_*', 
        'keep *_kt6CaloJets_*_*', 
        'keep *_ak4CaloJets_*_*', 
        'keep *_ak5CaloJets_*_*', 
        'keep double_kt6PFJets_rho_*', 
        'keep *_ak4PFJets_*_*', 
        'keep *_ak5PFJets_*_*', 
        'keep *_ak8PFJets_*_*', 
        'keep *_ak4PFJetsCHS_*_*', 
        'keep *_ak5PFJetsCHS_*_*', 
        'keep *_ak8PFJetsCHS_*_*', 
        'keep *_ca8PFJetsCHS_*_*', 
        'keep *_ca8PFJetsCHSPruned_*_*', 
        'keep *_cmsTopTagPFJetsCHS_*_*', 
        'keep *_JetPlusTrackZSPCorJetAntiKt4_*_*', 
        'keep *_ak4TrackJets_*_*', 
        'keep recoRecoChargedRefCandidates_trackRefsForJets_*_*', 
        'keep *_caloTowers_*_*', 
        'keep *_towerMaker_*_*', 
        'keep *_CastorTowerReco_*_*', 
        'keep *_kt4JetTracksAssociatorAtVertex_*_*', 
        'keep *_kt4JetTracksAssociatorAtCaloFace_*_*', 
        'keep *_kt4JetExtender_*_*', 
        'keep *_ak4JetTracksAssociatorAtVertex_*_*', 
        'keep *_ak4JetTracksAssociatorAtVertexPF_*_*', 
        'keep *_ak4JetTracksAssociatorAtCaloFace_*_*', 
        'keep *_ak4JetTracksAssociatorExplicit_*_*', 
        'keep *_ak4JetExtender_*_*', 
        'keep *_ak7JetTracksAssociatorAtVertex*_*_*', 
        'keep *_ak7JetTracksAssociatorAtCaloFace*_*_*', 
        'keep *_ak7JetExtender_*_*', 
        'keep *_ak4JetID_*_*', 
        'keep *_ak5JetID_*_*', 
        'keep *_ak7JetID_*_*', 
        'keep *_ic5JetID_*_*', 
        'keep *_kt4JetID_*_*', 
        'keep *_kt6JetID_*_*', 
        'keep *_ak7BasicJets_*_*', 
        'keep *_ak7CastorJetID_*_*', 
        'keep double_kt6CaloJetsCentral_*_*', 
        'keep double_kt6PFJetsCentralChargedPileUp_*_*', 
        'keep double_kt6PFJetsCentralNeutral_*_*', 
        'keep double_kt6PFJetsCentralNeutralTight_*_*', 
        'keep *_fixedGridRho*_*_*', 
        'keep *_ca8PFJetsCHSPrunedLinks_*_*', 
        'keep recoCaloMETs_caloMet_*_*', 
        'keep recoCaloMETs_caloMetBE_*_*', 
        'keep recoCaloMETs_caloMetBEFO_*_*', 
        'keep recoCaloMETs_caloMetM_*_*', 
        'keep recoPFMETs_pfMet_*_*', 
        'keep recoPFMETs_pfChMet_*_*', 
        'keep recoMuonMETCorrectionDataedmValueMap_muonMETValueMapProducer_*_*', 
        'keep recoHcalNoiseRBXs_hcalnoise_*_*', 
        'keep HcalNoiseSummary_hcalnoise_*_*', 
        'keep *HaloData_*_*_*', 
        'keep *BeamHaloSummary_BeamHaloSummary_*_*', 
        'keep *_MuonSeed_*_*', 
        'keep *_ancientMuonSeed_*_*', 
        'keep *_displacedMuonSeeds_*_*', 
        'keep *_mergedStandAloneMuonSeeds_*_*', 
        'keep TrackingRecHitsOwned_globalMuons_*_*', 
        'keep TrackingRecHitsOwned_tevMuons_*_*', 
        'keep recoCaloMuons_calomuons_*_*', 
        'keep *_CosmicMuonSeed_*_*', 
        'keep recoTrackExtras_cosmicMuons_*_*', 
        'keep TrackingRecHitsOwned_cosmicMuons_*_*', 
        'keep recoTrackExtras_globalCosmicMuons_*_*', 
        'keep TrackingRecHitsOwned_globalCosmicMuons_*_*', 
        'keep recoTrackExtras_cosmicMuons1Leg_*_*', 
        'keep TrackingRecHitsOwned_cosmicMuons1Leg_*_*', 
        'keep recoTrackExtras_globalCosmicMuons1Leg_*_*', 
        'keep TrackingRecHitsOwned_globalCosmicMuons1Leg_*_*', 
        'keep recoTracks_cosmicsVetoTracks_*_*', 
        'keep *_SETMuonSeed_*_*', 
        'keep recoTracks_standAloneSETMuons_*_*', 
        'keep recoTrackExtras_standAloneSETMuons_*_*', 
        'keep TrackingRecHitsOwned_standAloneSETMuons_*_*', 
        'keep recoTracks_globalSETMuons_*_*', 
        'keep recoTrackExtras_globalSETMuons_*_*', 
        'keep TrackingRecHitsOwned_globalSETMuons_*_*', 
        'keep recoMuons_muonsWithSET_*_*', 
        'keep *_muons_*_*', 
        'keep *_*_muons_*', 
        'drop *_muons_muons1stStep2muonsMap_*', 
        'drop recoIsoDepositedmValueMap_muons_*_*', 
        'drop doubleedmValueMap_muons_muPFIso*_*', 
        'keep recoTracks_standAloneMuons_*_*', 
        'keep recoTrackExtras_standAloneMuons_*_*', 
        'keep TrackingRecHitsOwned_standAloneMuons_*_*', 
        'keep recoTracks_globalMuons_*_*', 
        'keep recoTrackExtras_globalMuons_*_*', 
        'keep recoTracks_tevMuons_*_*', 
        'keep recoTrackExtras_tevMuons_*_*', 
        'keep recoTracks_generalTracks_*_*', 
        'keep recoTracksToOnerecoTracksAssociation_tevMuons_*_*', 
        'keep recoTracks_cosmicMuons_*_*', 
        'keep recoTracks_globalCosmicMuons_*_*', 
        'keep recoMuons_muonsFromCosmics_*_*', 
        'keep recoTracks_cosmicMuons1Leg_*_*', 
        'keep recoTracks_globalCosmicMuons1Leg_*_*', 
        'keep recoMuons_muonsFromCosmics1Leg_*_*', 
        'keep recoTracks_refittedStandAloneMuons_*_*', 
        'keep recoTrackExtras_refittedStandAloneMuons_*_*', 
        'keep TrackingRecHitsOwned_refittedStandAloneMuons_*_*', 
        'keep recoTracks_displacedStandAloneMuons__*', 
        'keep recoTrackExtras_displacedStandAloneMuons_*_*', 
        'keep TrackingRecHitsOwned_displacedStandAloneMuons_*_*', 
        'keep *_muIsoDepositTk_*_*', 
        'keep *_muIsoDepositCalByAssociatorTowers_*_*', 
        'keep *_muIsoDepositCalByAssociatorHits_*_*', 
        'keep *_muIsoDepositJets_*_*', 
        'keep *_muGlobalIsoDepositCtfTk_*_*', 
        'keep *_muGlobalIsoDepositCalByAssociatorTowers_*_*', 
        'keep *_muGlobalIsoDepositCalByAssociatorHits_*_*', 
        'keep *_muGlobalIsoDepositJets_*_*', 
        'keep *_impactParameterTagInfos_*_*', 
        'keep *_trackCountingHighEffBJetTags_*_*', 
        'keep *_trackCountingHighPurBJetTags_*_*', 
        'keep *_jetProbabilityBJetTags_*_*', 
        'keep *_jetBProbabilityBJetTags_*_*', 
        'keep *_secondaryVertexTagInfos_*_*', 
        'keep *_inclusiveSecondaryVertexFinderTagInfos_*_*', 
        'keep *_ghostTrackVertexTagInfos_*_*', 
        'keep *_simpleSecondaryVertexBJetTags_*_*', 
        'keep *_simpleSecondaryVertexHighEffBJetTags_*_*', 
        'keep *_simpleSecondaryVertexHighPurBJetTags_*_*', 
        'keep *_combinedSecondaryVertexBJetTags_*_*', 
        'keep *_combinedInclusiveSecondaryVertexV2BJetTags_*_*', 
        'keep *_ghostTrackBJetTags_*_*', 
        'keep *_softPFMuonsTagInfos_*_*', 
        'keep *_softPFElectronsTagInfos_*_*', 
        'keep *_softPFElectronBJetTags_*_*', 
        'keep *_softPFMuonBJetTags_*_*', 
        'keep *_softMuonTagInfos_*_*', 
        'keep *_softMuonBJetTags_*_*', 
        'keep *_softMuonByIP3dBJetTags_*_*', 
        'keep *_softMuonByPtBJetTags_*_*', 
        'keep *_combinedMVABJetTags_*_*', 
        'keep *_pfImpactParameterTagInfos_*_*', 
        'keep *_pfSecondaryVertexTagInfos_*_*', 
        'keep *_pfCombinedSecondaryVertexBJetTags_*_*', 
        'keep *_ak4PFJetsRecoTauPiZeros_*_*', 
        'keep *_hpsPFTauProducer_*_*', 
        'keep *_hpsPFTauDiscrimination*_*_*', 
        'keep *_hpsPFTau*PtSum_*_*', 
        'keep *_hpsPFTauTransverseImpactParameters_*_*', 
        'keep  *_offlinePrimaryVertices__*', 
        'keep  *_offlinePrimaryVerticesWithBS_*_*', 
        'keep  *_offlinePrimaryVerticesFromCosmicTracks_*_*', 
        'keep  *_nuclearInteractionMaker_*_*', 
        'keep *_generalV0Candidates_*_*', 
        'keep *_inclusiveSecondaryVertices_*_*', 
        'keep recoGsfElectronCores_gsfElectronCores_*_*', 
        'keep recoGsfElectrons_gsfElectrons_*_*', 
        'keep recoGsfElectronCores_uncleanedOnlyGsfElectronCores_*_*', 
        'keep recoGsfElectrons_uncleanedOnlyGsfElectrons_*_*', 
        'keep floatedmValueMap_eidRobustLoose_*_*', 
        'keep floatedmValueMap_eidRobustTight_*_*', 
        'keep floatedmValueMap_eidRobustHighEnergy_*_*', 
        'keep floatedmValueMap_eidLoose_*_*', 
        'keep floatedmValueMap_eidTight_*_*', 
        'keep *_egmGedGsfElectronPFIsolation_*_*', 
        'keep *_egmGsfElectronIDs_*_*', 
        'keep *_gedPhotonCore_*_*', 
        'keep *_gedPhotons_*_*', 
        'keep *_particleBasedIsolation_*_*', 
        'keep recoPhotons_mustachePhotons_*_*', 
        'keep recoPhotonCores_mustachePhotonCore_*_*', 
        'keep recoPhotons_photons_*_*', 
        'keep recoPhotonCores_photonCore_*_*', 
        'keep recoConversions_conversions_*_*', 
        'keep recoConversions_mustacheConversions_*_*', 
        'drop *_conversions_uncleanedConversions_*', 
        'drop *_gedPhotonsTmp_valMapPFEgammaCandToPhoton_*', 
        'keep recoConversions_allConversions_*_*', 
        'keep recoConversions_allConversionsOldEG_*_*', 
        'keep recoTracks_ckfOutInTracksFrom*Conversions_*_*', 
        'keep recoTracks_ckfInOutTracksFrom*Conversions_*_*', 
        'keep recoTrackExtras_ckfOutInTracksFrom*Conversions_*_*', 
        'keep recoTrackExtras_ckfInOutTracksFrom*Conversions_*_*', 
        'keep TrackingRecHitsOwned_ckfOutInTracksFrom*Conversions_*_*', 
        'keep TrackingRecHitsOwned_ckfInOutTracksFrom*Conversions_*_*', 
        'keep recoConversions_uncleanedOnlyAllConversions_*_*', 
        'keep recoTracks_uncleanedOnlyCkfOutInTracksFromConversions_*_*', 
        'keep recoTracks_uncleanedOnlyCkfInOutTracksFromConversions_*_*', 
        'keep recoTrackExtras_uncleanedOnlyCkfOutInTracksFromConversions_*_*', 
        'keep recoTrackExtras_uncleanedOnlyCkfInOutTracksFromConversions_*_*', 
        'keep TrackingRecHitsOwned_uncleanedOnlyCkfOutInTracksFromConversions_*_*', 
        'keep TrackingRecHitsOwned_uncleanedOnlyCkfInOutTracksFromConversions_*_*', 
        'keep *_PhotonIDProd_*_*', 
        'keep *_PhotonIDProdGED_*_*', 
        'keep *_hfRecoEcalCandidate_*_*', 
        'keep *_hfEMClusters_*_*', 
        'keep *_gedGsfElectronCores_*_*', 
        'keep *_gedGsfElectrons_*_*', 
        'keep *_pixelTracks_*_*', 
        'keep *_pixelVertices_*_*', 
        'drop CaloTowersSorted_towerMakerPF_*_*', 
        'keep recoPFRecHits_particleFlowClusterECAL_Cleaned_*', 
        'keep recoPFRecHits_particleFlowClusterHCAL_Cleaned_*', 
        'keep recoPFRecHits_particleFlowClusterHO_Cleaned_*', 
        'keep recoPFRecHits_particleFlowClusterHFEM_Cleaned_*', 
        'keep recoPFRecHits_particleFlowClusterHFHAD_Cleaned_*', 
        'keep recoPFRecHits_particleFlowClusterPS_Cleaned_*', 
        'keep recoPFRecHits_particleFlowRecHitECAL_Cleaned_*', 
        'keep recoPFRecHits_particleFlowRecHitHCAL_Cleaned_*', 
        'keep recoPFRecHits_particleFlowRecHitHO_Cleaned_*', 
        'keep recoPFRecHits_particleFlowRecHitPS_Cleaned_*', 
        'keep recoPFClusters_particleFlowClusterECAL_*_*', 
        'keep recoPFClusters_particleFlowClusterHCAL_*_*', 
        'keep recoPFClusters_particleFlowClusterHO_*_*', 
        'keep recoPFClusters_particleFlowClusterPS_*_*', 
        'keep recoPFBlocks_particleFlowBlock_*_*', 
        'keep recoPFCandidates_particleFlowEGamma_*_*', 
        'keep recoCaloClusters_particleFlowEGamma_*_*', 
        'keep recoSuperClusters_particleFlowEGamma_*_*', 
        'keep recoConversions_particleFlowEGamma_*_*', 
        'keep recoPFCandidates_particleFlow_*_*', 
        'keep recoPFCandidates_particleFlowTmp_electrons_*', 
        'keep recoPFCandidates_particleFlowTmp_*_*', 
        'drop recoPFCandidates_particleFlowTmp__*', 
        'keep recoPFDisplacedVertexs_particleFlowDisplacedVertex_*_*', 
        'keep *_pfElectronTranslator_*_*', 
        'keep *_pfPhotonTranslator_*_*', 
        'keep *_particleFlow_electrons_*', 
        'keep *_particleFlow_photons_*', 
        'keep *_trackerDrivenElectronSeeds_preid_*', 
        'keep *_particleFlowPtrs_*_*', 
        'keep *_particleFlowTmpPtrs_*_*', 
        'keep *_offlineBeamSpot_*_*', 
        'keep L1GlobalTriggerReadoutRecord_gtDigis_*_*', 
        'keep *_l1GtRecord_*_*', 
        'keep *_l1GtTriggerMenuLite_*_*', 
        'keep *_conditionsInEdm_*_*', 
        'keep *_l1extraParticles_*_*', 
        'keep *_l1L1GtObjectMap_*_*', 
        'keep L1MuGMTReadoutCollection_gtDigis_*_*', 
        'keep L1GctEmCand*_gctDigis_*_*', 
        'keep L1GctJetCand*_gctDigis_*_*', 
        'keep L1GctEtHad*_gctDigis_*_*', 
        'keep L1GctEtMiss*_gctDigis_*_*', 
        'keep L1GctEtTotal*_gctDigis_*_*', 
        'keep L1GctHtMiss*_gctDigis_*_*', 
        'keep L1GctJetCounts*_gctDigis_*_*', 
        'keep L1GctHFRingEtSums*_gctDigis_*_*', 
        'keep L1GctHFBitCounts*_gctDigis_*_*', 
        'keep LumiDetails_lumiProducer_*_*', 
        'keep LumiSummary_lumiProducer_*_*', 
        'drop *_hlt*_*_*', 
        'keep *_hltL1GtObjectMap_*_*', 
        'keep edmTriggerResults_*_*_*', 
        'keep triggerTriggerEvent_*_*_*', 
        'keep L1AcceptBunchCrossings_*_*_*', 
        'keep L1TriggerScalerss_*_*_*', 
        'keep Level1TriggerScalerss_*_*_*', 
        'keep LumiScalerss_*_*_*', 
        'keep BeamSpotOnlines_*_*_*', 
        'keep DcsStatuss_*_*_*', 
        'keep *_logErrorHarvester_*_*', 
        'keep *_pfIsolatedElectronsEI_*_*', 
        'keep *_pfIsolatedMuonsEI_*_*', 
        'keep recoPFJets_pfJetsEI_*_*', 
        'keep *_pfJetTrackAssociatorEI_*_*', 
        'keep *_impactParameterTagInfosEI_*_*', 
        'keep *_secondaryVertexTagInfosEI_*_*', 
        'keep *_combinedSecondaryVertexBJetTagsEI_*_*', 
        'keep recoPFTaus_pfTausEI_*_*', 
        'keep recoPFTauDiscriminator_pfTausDiscrimination*_*_*', 
        'keep *_pfMetEI_*_*', 
        'keep FEDRawDataCollection_rawDataCollector_*_*', 
        'keep FEDRawDataCollection_source_*_*', 
        'keep LHERunInfoProduct_*_*_*', 
        'keep LHEEventProduct_*_*_*', 
        'keep GenRunInfoProduct_generator_*_*', 
        'keep GenLumiInfoProduct_generator_*_*', 
        'keep GenEventInfoProduct_generator_*_*', 
        'keep edmHepMCProduct_generator_*_*', 
        'keep GenFilterInfo_*_*_*', 
        'keep *_genParticles_*_*', 
        'keep recoGenMETs_*_*_*', 
        'keep *_kt4GenJets_*_*', 
        'keep *_kt6GenJets_*_*', 
        'keep *_ak4GenJets_*_*', 
        'keep *_ak5GenJets_*_*', 
        'keep *_ak7GenJets_*_*', 
        'keep *_iterativeCone5GenJets_*_*', 
        'keep *_genParticle_*_*', 
        'keep edmHepMCProduct_source_*_*', 
        'keep SimTracks_g4SimHits_*_*', 
        'keep SimVertexs_g4SimHits_*_*', 
        'keep *_allTrackMCMatch_*_*', 
        'keep StripDigiSimLinkedmDetSetVector_simMuonCSCDigis_*_*', 
        'keep DTLayerIdDTDigiSimLinkMuonDigiCollection_simMuonDTDigis_*_*', 
        'keep RPCDigiSimLinkedmDetSetVector_simMuonRPCDigis_*_*', 
        'keep PileupSummaryInfos_*_*_*', 
        'drop *_hlt*_*_*', 
        'keep *_hltAK4CaloJetsCorrectedIDPassed_*_*', 
        'keep *_hltAK4CaloJetsIDPassed_*_*', 
        'keep *_hltAK4CaloJets_*_*', 
        'keep *_hltAK4PFJetsCorrected_*_*', 
        'keep *_hltAK4PFJetsForTaus_*_*', 
        'keep *_hltAK4PFJets_*_*', 
        'keep *_hltAlCaEtaEBUncalibrator_*_*', 
        'keep *_hltAlCaEtaEEUncalibrator_*_*', 
        'keep *_hltAlCaEtaRecHitsFilterEBonly_*_*', 
        'keep *_hltAlCaEtaRecHitsFilterEEonly_*_*', 
        'keep *_hltAlCaEtaRecHitsFilterEEonly_etaEcalRecHitsES_*', 
        'keep *_hltAlCaEtaRecHitsFilter_*_*', 
        'keep *_hltAlCaPhiSymStream_*_*', 
        'keep *_hltAlCaPhiSymUncalibrator_*_*', 
        'keep *_hltAlCaPi0EBUncalibrator_*_*', 
        'keep *_hltAlCaPi0EEUncalibrator_*_*', 
        'keep *_hltAlCaPi0RecHitsFilterEBonly_*_*', 
        'keep *_hltAlCaPi0RecHitsFilterEEonly_*_*', 
        'keep *_hltAlCaPi0RecHitsFilterEEonly_pi0EcalRecHitsES_*', 
        'keep *_hltAlCaPi0RecHitsFilter_*_*', 
        'keep *_hltBLifetimeL25AssociatorbbPhiL1FastJetFastPV_*_*', 
        'keep *_hltBLifetimeL25BJetTagsbbPhiL1FastJetFastPV_*_*', 
        'keep *_hltBLifetimeL25TagInfosbbPhiL1FastJetFastPV_*_*', 
        'keep *_hltBLifetimeL3AssociatorbbPhiL1FastJetFastPV_*_*', 
        'keep *_hltBLifetimeL3BJetTagsbbPhiL1FastJetFastPV_*_*', 
        'keep *_hltBLifetimeL3TagInfosbbPhiL1FastJetFastPV_*_*', 
        'keep *_hltBLifetimeRegionalCtfWithMaterialTracksbbPhiL1FastJetFastPV_*_*', 
        'keep *_hltBSoftMuonDiJet110Mu5L3FilterByDR_*_*', 
        'keep *_hltBSoftMuonDiJet110Mu5SelL3BJetTagsByDR_*_*', 
        'keep *_hltBSoftMuonDiJet20Mu5L3FilterByDR_*_*', 
        'keep *_hltBSoftMuonDiJet20Mu5SelL3BJetTagsByDR_*_*', 
        'keep *_hltBSoftMuonDiJet40Mu5L3FilterByDR_*_*', 
        'keep *_hltBSoftMuonDiJet40Mu5SelL3BJetTagsByDR_*_*', 
        'keep *_hltBSoftMuonDiJet70Mu5L3FilterByDR_*_*', 
        'keep *_hltBSoftMuonDiJet70Mu5SelL3BJetTagsByDR_*_*', 
        'keep *_hltBSoftMuonMu5L3_*_*', 
        'keep *_hltCSVJetTagSingleTopEle27_*_*', 
        'keep *_hltCSVJetTagSingleTopIsoMu24_*_*', 
        'keep *_hltCaloJetCorrectedRegional_*_*', 
        'keep *_hltCaloJetCorrected_*_*', 
        'keep *_hltCaloJetL1FastJetCorrected_*_*', 
        'keep *_hltCleanedCorrectedIslandBarrelSuperClustersHI_*_*', 
        'keep *_hltCleanedHiCorrectedIslandBarrelSuperClustersHI_*_*', 
        'keep *_hltConvPFTausTightIsoTrackFindingIsolation_*_*', 
        'keep *_hltConvPFTausTightIsoTrackFinding_*_*', 
        'keep *_hltConvPFTausTightIsoTrackPt5Isolation_*_*', 
        'keep *_hltConvPFTausTightIsoTrackPt5_*_*', 
        'keep *_hltConvPFTausTightIso_*_*', 
        'keep *_hltConvPFTausTrackFindingLooseIsolation_*_*', 
        'keep *_hltConvPFTausTrackFinding_*_*', 
        'keep *_hltConvPFTaus_*_*', 
        'keep *_hltCorrectedIslandBarrelSuperClustersHI_*_*', 
        'keep *_hltCorrectedIslandEndcapSuperClustersHI_*_*', 
        'keep *_hltCsc2DRecHits_*_*', 
        'keep *_hltCscSegments_*_*', 
        'keep *_hltDoublePFTau25TrackPt5MediumIsolationProng4L1HLTMatched_*_*', 
        'keep *_hltDoublePFTau25TrackPt5MediumIsolationProng4_*_*', 
        'keep *_hltDoublePFTau25TrackPt5MediumIsolation_*_*', 
        'keep *_hltDoublePFTau25TrackPt5_*_*', 
        'keep *_hltDoublePFTau25_*_*', 
        'keep *_hltDoublePFTauTightIso45Track5_*_*', 
        'keep *_hltDoublePFTauTightIso45Track_*_*', 
        'keep *_hltDt4DSegments_*_*', 
        'keep *_hltEcalRecHitAll_*_*', 
        'keep *_hltEle20CaloIdVTCaloIsoTTrkIdTTrkIsoTTrackIsoFilterL1SingleEG18orL1SingleEG20_*_*', 
        'keep *_hltEle20CaloIdVTTrkIdTDphiFilter_*_*', 
        'keep *_hltEle27WP85PixelMatchElectronsL1Seeded_*_*', 
        'keep *_hltFastPVPixelTracksMerger_*_*', 
        'keep *_hltFastPVPixelTracksRecover_*_*', 
        'keep *_hltFastPVPixelTracks_*_*', 
        'keep *_hltFastPVPixelVertices3D_*_*', 
        'keep *_hltFastPVPixelVertices_*_*', 
        'keep *_hltFastPixelBLifetimeL3TagInfos_*_*', 
        'keep *_hltFastPrimaryVertex_*_*', 
        'keep *_hltFilterDoubleIsoPFTau45Trk5LeadTrack5IsolationL1HLTMatched_*_*', 
        'keep *_hltFilterL2EtCutDoublePFIsoTau45Trk5_*_*', 
        'keep *_hltFilterL2EtCutSingleIsoPFTau35Trk20MET70_*_*', 
        'keep *_hltFilterSingleIsoPFTau35Trk20LeadTrackPt20_*_*', 
        'keep *_hltFilterSingleIsoPFTau35Trk20MET60LeadTrack20IsolationL1HLTMatched_*_*', 
        'keep *_hltFilterSingleIsoPFTau35Trk20MET70LeadTrack20IsolationL1HLTMatched_*_*', 
        'keep *_hltGoodOnlinePVs_*_*', 
        'keep *_hltHICaloJetCorrected_*_*', 
        'keep *_hltHICaloJetIDPassed_*_*', 
        'keep *_hltHIGoodLooseTracks_*_*', 
        'keep *_hltHIPixel3PrimTracks_*_*', 
        'keep *_hltHISelectedVertex_*_*', 
        'keep *_hltHISiPixelClusters_*_*', 
        'keep *_hltHITIPTCorrectorHB_*_*', 
        'keep *_hltHITIPTCorrectorHE_*_*', 
        'keep *_hltHiCorrectedIslandBarrelSuperClustersHI_*_*', 
        'keep *_hltHiCorrectedIslandEndcapSuperClustersHI_*_*', 
        'keep *_hltHiIslandSuperClustersHI_*_*', 
        'keep *_hltIsolPixelTrackProdHB_*_*', 
        'keep *_hltIsolPixelTrackProdHE_*_*', 
        'keep *_hltIter0PFlowTrackSelectionHighPurity_*_*', 
        'keep *_hltIter1Merged_*_*', 
        'keep *_hltIter1PFlowTrackSelectionHighPurity_*_*', 
        'keep *_hltIter2Merged_*_*', 
        'keep *_hltIter2PFlowTrackSelectionHighPurity_*_*', 
        'keep *_hltIter3Merged_*_*', 
        'keep *_hltIter4Merged_*_*', 
        'keep *_hltIterativeCone5PileupSubtractionCaloJets_*_*', 
        'keep *_hltL1GtObjectMap_*_*', 
        'keep *_hltL1HLTSingleIsoPFTau35Trk20Met60JetsMatch_*_*', 
        'keep *_hltL1IsoElectronTrackIsol_*_*', 
        'keep *_hltL1NonIsoElectronTrackIsol_*_*', 
        'keep *_hltL1SeededRecoEcalCandidate_*_*', 
        'keep *_hltL1extraParticlesCentral_*_*', 
        'keep *_hltL1extraParticlesNonIsolated_*_*', 
        'keep *_hltL1extraParticlesTau_*_*', 
        'keep *_hltL1extraParticles_*_*', 
        'keep *_hltL1sDoubleTauJet44Eta2p17orDoubleJet64Central_*_*', 
        'keep *_hltL1sDoubleTauJet44erorDoubleJetC64_*_*', 
        'keep *_hltL1sL1EG18er_*_*', 
        'keep *_hltL1sL1ETM36ORETM40_*_*', 
        'keep *_hltL1sL1Jet52ETM30_*_*', 
        'keep *_hltL1sL1SingleEG12_*_*', 
        'keep *_hltL1sL1SingleEG15_*_*', 
        'keep *_hltL1sL1SingleEG18orL1SingleEG20_*_*', 
        'keep *_hltL1sL1SingleMu10_*_*', 
        'keep *_hltL1sL1SingleMu14Eta2p1_*_*', 
        'keep *_hltL1sMu16Eta2p1_*_*', 
        'keep *_hltL2MuonCandidatesNoVtx_*_*', 
        'keep *_hltL2MuonCandidates_*_*', 
        'keep *_hltL2MuonSeeds_*_*', 
        'keep *_hltL2Muons_*_*', 
        'keep *_hltL2TauJets_*_*', 
        'keep *_hltL3CombinedSecondaryVertexBJetTags_*_*', 
        'keep *_hltL3MuonCandidates_*_*', 
        'keep *_hltL3MuonsIOHit_*_*', 
        'keep *_hltL3MuonsLinksCombination_*_*', 
        'keep *_hltL3MuonsOIHit_*_*', 
        'keep *_hltL3MuonsOIState_*_*', 
        'keep *_hltL3Muons_*_*', 
        'keep *_hltL3NoFiltersNoVtxMuonCandidates_*_*', 
        'keep *_hltL3NoFiltersNoVtxMuons_*_*', 
        'keep *_hltL3SecondaryVertexTagInfos_*_*', 
        'keep *_hltL3TkFromL2OICombination_*_*', 
        'keep *_hltL3TkTracksFromL2IOHit_*_*', 
        'keep *_hltL3TkTracksFromL2OIHit_*_*', 
        'keep *_hltL3TkTracksFromL2OIState_*_*', 
        'keep *_hltL3TkTracksFromL2_*_*', 
        'keep *_hltL3TrackCandidateFromL2IOHit_*_*', 
        'keep *_hltL3TrackCandidateFromL2OIHit_*_*', 
        'keep *_hltL3TrackCandidateFromL2OIState_*_*', 
        'keep *_hltL3TrajSeedIOHit_*_*', 
        'keep *_hltL3TrajSeedOIHit_*_*', 
        'keep *_hltL3TrajSeedOIState_*_*', 
        'keep *_hltL3TrajectorySeed_*_*', 
        'keep *_hltL3crIsoL1sMu16Eta2p1L1f0L2f16QL3f18QL3crIsoRhoFiltered0p15_*_*', 
        'keep *_hltLeadingCentralJets30SingleTopEle27_*_*', 
        'keep *_hltLeadingCentralJets30SingleTopIsoMu24_*_*', 
        'keep *_hltMetForHf_*_*', 
        'keep *_hltMet_*_*', 
        'keep *_hltMu8Ele17CaloIdTCaloIsoVLPixelMatchFilter_*_*', 
        'keep *_hltMuTrackJpsiCtfTrackCands_*_*', 
        'keep *_hltMuTrackJpsiPixelTrackCands_*_*', 
        'keep *_hltMuonCSCDigis_*_*', 
        'keep *_hltMuonCSCDigis_MuonCSCStripDigi_*', 
        'keep *_hltMuonCSCDigis_MuonCSCWireDigi_*', 
        'keep *_hltMuonDTDigis_*_*', 
        'keep *_hltMuonRPCDigis_*_*', 
        'keep *_hltOnlineBeamSpot_*_*', 
        'keep *_hltOnlinePrimaryVertices_*_*', 
        'keep *_hltOverlapFilterEle20LooseIsoPFTau20OldVersion_*_*', 
        'keep *_hltOverlapFilterIsoEle20MediumIsoPFTau20_*_*', 
        'keep *_hltOverlapFilterIsoMu15IsoPFTau20_*_*', 
        'keep *_hltOverlapFilterIsoMu15MediumIsoPFTau20_*_*', 
        'keep *_hltOverlapFilterIsoMu15TightIsoPFTau20_*_*', 
        'keep *_hltOverlapFilterIsoMu18LooseIsoPFTau20_*_*', 
        'keep *_hltOverlapFilterIsoMu18PFTau25TrackPt5Prong4_*_*', 
        'keep *_hltPFTau15TrackLooseIso_*_*', 
        'keep *_hltPFTau15Track_*_*', 
        'keep *_hltPFTau15_*_*', 
        'keep *_hltPFTau20IsoMuVertex_*_*', 
        'keep *_hltPFTau20TrackLooseIso_*_*', 
        'keep *_hltPFTau20Track_*_*', 
        'keep *_hltPFTau20_*_*', 
        'keep *_hltPFTau25TrackPt5MediumIsolationProng4IsoMuVertex_*_*', 
        'keep *_hltPFTau25TrackPt5MediumIsolationProng4_*_*', 
        'keep *_hltPFTau25TrackPt5MediumIsolation_*_*', 
        'keep *_hltPFTau25TrackPt5_*_*', 
        'keep *_hltPFTau25_*_*', 
        'keep *_hltPFTau35TrackPt20LooseIsoProng2_*_*', 
        'keep *_hltPFTau35TrackPt20LooseIso_*_*', 
        'keep *_hltPFTau35TrackPt20_*_*', 
        'keep *_hltPFTau35Track_*_*', 
        'keep *_hltPFTau35_*_*', 
        'keep *_hltPFTauEleVertex20_*_*', 
        'keep *_hltPFTauJetTracksAssociator_*_*', 
        'keep *_hltPFTauMediumIso20TrackMediumIso_*_*', 
        'keep *_hltPFTauMediumIso20Track_*_*', 
        'keep *_hltPFTauMediumIso20_*_*', 
        'keep *_hltPFTauMediumIso35Track_*_*', 
        'keep *_hltPFTauMediumIso35_*_*', 
        'keep *_hltPFTauTagInfo_*_*', 
        'keep *_hltPFTauTightIso20TrackTightIso_*_*', 
        'keep *_hltPFTauTightIso20Track_*_*', 
        'keep *_hltPFTauTightIso20_*_*', 
        'keep *_hltPFlowTrackSelectionHighPurity_*_*', 
        'keep *_hltParticleFlowForTaus_*_*', 
        'keep *_hltParticleFlow_*_*', 
        'keep *_hltPixelMatch3HitElectronsActivity_*_*', 
        'keep *_hltPixelMatch3HitElectronsL1Seeded_*_*', 
        'keep *_hltPixelMatchCleanElectronsL1Seeded_*_*', 
        'keep *_hltPixelMatchElectronsActivity_*_*', 
        'keep *_hltPixelMatchElectronsL1Iso_*_*', 
        'keep *_hltPixelMatchElectronsL1NonIso_*_*', 
        'keep *_hltPixelMatchElectronsL1Seeded_*_*', 
        'keep *_hltPixelTracks_*_*', 
        'keep *_hltPixelVertices3DbbPhi_*_*', 
        'keep *_hltPixelVertices_*_*', 
        'keep *_hltRecoEcalSuperClusterActivityCandidateSC4_*_*', 
        'keep *_hltRecoEcalSuperClusterActivityCandidateSC5_*_*', 
        'keep *_hltRecoEcalSuperClusterActivityCandidate_*_*', 
        'keep *_hltRpcRecHits_*_*', 
        'keep *_hltSelector4CentralJetsL1FastJet_*_*', 
        'keep *_hltSelectorJets20L1FastJet_*_*', 
        'keep *_hltSiPixelCluster_*_*', 
        'keep *_hltSiPixelClusters_*_*', 
        'keep *_hltSiStripClusters_*_*', 
        'keep *_hltSiStripRawToClustersFacility_*_*', 
        'keep *_hltSingleMu15L3Filtered15_*_*', 
        'keep *_hltSingleMuIsoL1s14L3IsoFiltered15eta2p1_*_*', 
        'keep *_hltSingleMuIsoL3IsoFiltered15_*_*', 
        'keep *_hltTowerMakerForAll_*_*', 
        'keep *_hltTowerMakerForMuons_*_*', 
        'keep *_hltTriggerSummaryAOD_*_*', 
        'keep *_hltTriggerSummaryRAW_*_*', 
        'keep *_hltTrimmedPixelVertices_*_*', 
        'keep *_hltVerticesL3_*_*', 
        'keep DcsStatuss_hltScalersRawToDigi_*_*', 
        'keep FEDRawDataCollection_rawDataCollector_*_*', 
        'keep FEDRawDataCollection_rawDataRepacker_*_*', 
        'keep FEDRawDataCollection_source_*_*', 
        'keep FEDRawDataCollection_virginRawDataRepacker_*_*', 
        'keep L1GlobalTriggerReadoutRecord_hltGtDigis_*_*', 
        'keep L1MuGMTCands_hltGtDigis_*_*', 
        'keep L1MuGMTReadoutCollection_hltGtDigis_*_*', 
        'keep L2MuonTrajectorySeeds_hltL2MuonSeeds_*_*', 
        'keep L3MuonTrajectorySeeds_hltHIL3TrajSeedOIHit_*_*', 
        'keep L3MuonTrajectorySeeds_hltHIL3TrajectorySeed_*_*', 
        'keep L3MuonTrajectorySeeds_hltL3TrajSeedOIState_*_*', 
        'keep LumiScalerss_hltScalersRawToDigi_*_*', 
        'keep SiPixelClusteredmNewDetSetVector_hltSiPixelClusters_*_*', 
        'keep TrackCandidates_hltHIL3TrackCandidateFromL2OIHit_*_*', 
        'keep TrackCandidates_hltHIL3TrackCandidateFromL2OIState_*_*', 
        'keep TrackingRecHitsOwned_hltL3Muons_*_*', 
        'keep edmTriggerResults_*_*_*', 
        'keep recoCaloJets_*_*_*', 
        'keep recoCaloMETs_*_*_*', 
        'keep recoCaloMETs_hltMet_*_*', 
        'keep recoCompositeCandidates_*_*_*', 
        'keep recoElectrons_*_*_*', 
        'keep recoIsolatedPixelTrackCandidates_*_*_*', 
        'keep recoMETs_*_*_*', 
        'keep recoPFJets_*_*_*', 
        'keep recoPFTaus_*_*_*', 
        'keep recoRecoChargedCandidates_*_*_*', 
        'keep recoRecoChargedCandidates_hltHIL3MuonCandidates_*_*', 
        'keep recoRecoChargedCandidates_hltL2MuonCandidates_*_*', 
        'keep recoRecoEcalCandidatesToValuefloatAssociation_hltL1IsoHLTClusterShape_*_*', 
        'keep recoRecoEcalCandidatesToValuefloatAssociation_hltL1IsolatedPhotonEcalIsol_*_*', 
        'keep recoRecoEcalCandidatesToValuefloatAssociation_hltL1IsolatedPhotonHcalForHE_*_*', 
        'keep recoRecoEcalCandidatesToValuefloatAssociation_hltL1IsolatedPhotonHcalIsol_*_*', 
        'keep recoRecoEcalCandidatesToValuefloatAssociation_hltL1NonIsoHLTClusterShape_*_*', 
        'keep recoRecoEcalCandidatesToValuefloatAssociation_hltL1NonIsolatedPhotonEcalIsol_*_*', 
        'keep recoRecoEcalCandidatesToValuefloatAssociation_hltL1NonIsolatedPhotonHcalForHE_*_*', 
        'keep recoRecoEcalCandidatesToValuefloatAssociation_hltL1NonIsolatedPhotonHcalIsol_*_*', 
        'keep recoRecoEcalCandidates_*_*_*', 
        'keep recoRecoEcalCandidates_hltL1IsoRecoEcalCandidate_*_*', 
        'keep recoRecoEcalCandidates_hltL1NonIsoRecoEcalCandidate_*_*', 
        'keep recoTrackExtras_hltHIL3MuonsOIHit_*_*', 
        'keep recoTrackExtras_hltHIL3MuonsOIState_*_*', 
        'keep recoTracks_hltHIL3MuonsOIHit_*_*', 
        'keep recoTracks_hltHIL3MuonsOIState_*_*', 
        'keep recoTracks_hltHIL3Muons_*_*', 
        'keep recoTracks_hltHIL3TkTracksFromL2OIHit_*_*', 
        'keep recoTracks_hltHIL3TkTracksFromL2OIState_*_*', 
        'keep recoTracks_hltHIL3TkTracksFromL2_*_*', 
        'keep triggerTriggerEventWithRefs_*_*_*', 
        'keep triggerTriggerEvent_*_*_*', 
        'keep triggerTriggerFilterObjectWithRefs_*_*_*', 
        'drop *_trackingtruthprod_*_*', 
        'drop *_electrontruth_*_*', 
        'keep *_mix_MergedTrackTruth_*', 
        'keep CrossingFramePlaybackInfoExtended_*_*_*', 
        'keep PixelDigiSimLinkedmDetSetVector_simSiPixelDigis_*_*', 
        'keep StripDigiSimLinkedmDetSetVector_simSiStripDigis_*_*', 
        'drop *_mix_simSiPixelDigis*_*', 
        'drop *_mix_simSiStripDigis*_*', 
        'keep *_allTrackMCMatch_*_*' ) )
)

process.RAWRECOEventContent = cms.PSet(
    splitLevel = cms.untracked.int32(0),
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
    outputCommands = cms.untracked.vstring( ('drop *', 
        'drop *', 
        'keep DetIdedmEDCollection_siStripDigis_*_*', 
        'keep DetIdedmEDCollection_siPixelDigis_*_*', 
        'keep *_siPixelClusters_*_*', 
        'keep *_siStripClusters_*_*', 
        'keep *_clusterSummaryProducer_*_*', 
        'keep *_dt1DRecHits_*_*', 
        'keep *_dt4DSegments_*_*', 
        'keep *_dt1DCosmicRecHits_*_*', 
        'keep *_dt4DCosmicSegments_*_*', 
        'keep *_csc2DRecHits_*_*', 
        'keep *_cscSegments_*_*', 
        'keep *_rpcRecHits_*_*', 
        'keep *_hbhereco_*_*', 
        'keep *_hbheprereco_*_*', 
        'keep *_hfreco_*_*', 
        'keep *_horeco_*_*', 
        'keep HBHERecHitsSorted_hbherecoMB_*_*', 
        'keep HORecHitsSorted_horecoMB_*_*', 
        'keep HFRecHitsSorted_hfrecoMB_*_*', 
        'keep ZDCDataFramesSorted_*Digis_*_*', 
        'keep ZDCRecHitsSorted_*_*_*', 
        'keep *_reducedHcalRecHits_*_*', 
        'keep *_castorreco_*_*', 
        'keep HcalUnpackerReport_*_*_*', 
        'keep *_ecalPreshowerRecHit_*_*', 
        'keep *_ecalRecHit_*_*', 
        'keep *_ecalCompactTrigPrim_*_*', 
        'keep *_ecalTPSkim_*_*', 
        'keep *_selectDigi_*_*', 
        'keep EcalRecHitsSorted_reducedEcalRecHitsEE_*_*', 
        'keep EcalRecHitsSorted_reducedEcalRecHitsEB_*_*', 
        'keep EcalRecHitsSorted_reducedEcalRecHitsES_*_*', 
        'keep *_hybridSuperClusters_*_*', 
        'keep recoSuperClusters_correctedHybridSuperClusters_*_*', 
        'keep *_multi5x5SuperClusters_*_*', 
        'keep recoSuperClusters_multi5x5SuperClusters_*_*', 
        'keep recoSuperClusters_multi5x5SuperClustersWithPreshower_*_*', 
        'keep recoSuperClusters_correctedMulti5x5SuperClustersWithPreshower_*_*', 
        'keep recoPreshowerClusters_multi5x5SuperClustersWithPreshower_*_*', 
        'keep recoPreshowerClusterShapes_multi5x5PreshowerClusterShape_*_*', 
        'keep *_particleFlowSuperClusterECAL_*_*', 
        'drop recoClusterShapes_*_*_*', 
        'drop recoBasicClustersToOnerecoClusterShapesAssociation_*_*_*', 
        'drop recoBasicClusters_multi5x5BasicClusters_multi5x5BarrelBasicClusters_*', 
        'drop recoSuperClusters_multi5x5SuperClusters_multi5x5BarrelSuperClusters_*', 
        'keep *_CkfElectronCandidates_*_*', 
        'keep *_GsfGlobalElectronTest_*_*', 
        'keep *_electronMergedSeeds_*_*', 
        'keep recoGsfTracks_electronGsfTracks_*_*', 
        'keep recoGsfTrackExtras_electronGsfTracks_*_*', 
        'keep recoTrackExtras_electronGsfTracks_*_*', 
        'keep TrackingRecHitsOwned_electronGsfTracks_*_*', 
        'keep recoTracks_generalTracks_*_*', 
        'keep recoTrackExtras_generalTracks_*_*', 
        'keep TrackingRecHitsOwned_extraFromSeeds_*_*', 
        'keep uints_extraFromSeeds_*_*', 
        'keep TrackingRecHitsOwned_generalTracks_*_*', 
        'keep recoTracks_beamhaloTracks_*_*', 
        'keep recoTrackExtras_beamhaloTracks_*_*', 
        'keep TrackingRecHitsOwned_beamhaloTracks_*_*', 
        'keep recoTracks_regionalCosmicTracks_*_*', 
        'keep recoTrackExtras_regionalCosmicTracks_*_*', 
        'keep TrackingRecHitsOwned_regionalCosmicTracks_*_*', 
        'keep recoTracks_rsWithMaterialTracks_*_*', 
        'keep recoTrackExtras_rsWithMaterialTracks_*_*', 
        'keep TrackingRecHitsOwned_rsWithMaterialTracks_*_*', 
        'keep recoTracks_conversionStepTracks_*_*', 
        'keep recoTrackExtras_conversionStepTracks_*_*', 
        'keep TrackingRecHitsOwned_conversionStepTracks_*_*', 
        'keep *_ctfPixelLess_*_*', 
        'keep *_dedxTruncated40_*_*', 
        'keep *_dedxDiscrimASmi_*_*', 
        'keep *_dedxHarmonic2_*_*', 
        'keep *_trackExtrapolator_*_*', 
        'keep floatedmValueMap_generalTracks_*_*', 
        'keep *_kt6CaloJets_*_*', 
        'keep *_ak4CaloJets_*_*', 
        'keep *_ak5CaloJets_*_*', 
        'keep double_kt6PFJets_rho_*', 
        'keep *_ak4PFJets_*_*', 
        'keep *_ak5PFJets_*_*', 
        'keep *_ak8PFJets_*_*', 
        'keep *_ak4PFJetsCHS_*_*', 
        'keep *_ak5PFJetsCHS_*_*', 
        'keep *_ak8PFJetsCHS_*_*', 
        'keep *_ca8PFJetsCHS_*_*', 
        'keep *_ca8PFJetsCHSPruned_*_*', 
        'keep *_cmsTopTagPFJetsCHS_*_*', 
        'keep *_JetPlusTrackZSPCorJetAntiKt4_*_*', 
        'keep *_ak4TrackJets_*_*', 
        'keep recoRecoChargedRefCandidates_trackRefsForJets_*_*', 
        'keep *_caloTowers_*_*', 
        'keep *_towerMaker_*_*', 
        'keep *_CastorTowerReco_*_*', 
        'keep *_kt4JetTracksAssociatorAtVertex_*_*', 
        'keep *_kt4JetTracksAssociatorAtCaloFace_*_*', 
        'keep *_kt4JetExtender_*_*', 
        'keep *_ak4JetTracksAssociatorAtVertex_*_*', 
        'keep *_ak4JetTracksAssociatorAtVertexPF_*_*', 
        'keep *_ak4JetTracksAssociatorAtCaloFace_*_*', 
        'keep *_ak4JetTracksAssociatorExplicit_*_*', 
        'keep *_ak4JetExtender_*_*', 
        'keep *_ak7JetTracksAssociatorAtVertex*_*_*', 
        'keep *_ak7JetTracksAssociatorAtCaloFace*_*_*', 
        'keep *_ak7JetExtender_*_*', 
        'keep *_ak4JetID_*_*', 
        'keep *_ak5JetID_*_*', 
        'keep *_ak7JetID_*_*', 
        'keep *_ic5JetID_*_*', 
        'keep *_kt4JetID_*_*', 
        'keep *_kt6JetID_*_*', 
        'keep *_ak7BasicJets_*_*', 
        'keep *_ak7CastorJetID_*_*', 
        'keep double_kt6CaloJetsCentral_*_*', 
        'keep double_kt6PFJetsCentralChargedPileUp_*_*', 
        'keep double_kt6PFJetsCentralNeutral_*_*', 
        'keep double_kt6PFJetsCentralNeutralTight_*_*', 
        'keep *_fixedGridRho*_*_*', 
        'keep *_ca8PFJetsCHSPrunedLinks_*_*', 
        'keep recoCaloMETs_caloMet_*_*', 
        'keep recoCaloMETs_caloMetBE_*_*', 
        'keep recoCaloMETs_caloMetBEFO_*_*', 
        'keep recoCaloMETs_caloMetM_*_*', 
        'keep recoPFMETs_pfMet_*_*', 
        'keep recoPFMETs_pfChMet_*_*', 
        'keep recoMuonMETCorrectionDataedmValueMap_muonMETValueMapProducer_*_*', 
        'keep recoHcalNoiseRBXs_hcalnoise_*_*', 
        'keep HcalNoiseSummary_hcalnoise_*_*', 
        'keep *HaloData_*_*_*', 
        'keep *BeamHaloSummary_BeamHaloSummary_*_*', 
        'keep *_MuonSeed_*_*', 
        'keep *_ancientMuonSeed_*_*', 
        'keep *_displacedMuonSeeds_*_*', 
        'keep *_mergedStandAloneMuonSeeds_*_*', 
        'keep TrackingRecHitsOwned_globalMuons_*_*', 
        'keep TrackingRecHitsOwned_tevMuons_*_*', 
        'keep recoCaloMuons_calomuons_*_*', 
        'keep *_CosmicMuonSeed_*_*', 
        'keep recoTrackExtras_cosmicMuons_*_*', 
        'keep TrackingRecHitsOwned_cosmicMuons_*_*', 
        'keep recoTrackExtras_globalCosmicMuons_*_*', 
        'keep TrackingRecHitsOwned_globalCosmicMuons_*_*', 
        'keep recoTrackExtras_cosmicMuons1Leg_*_*', 
        'keep TrackingRecHitsOwned_cosmicMuons1Leg_*_*', 
        'keep recoTrackExtras_globalCosmicMuons1Leg_*_*', 
        'keep TrackingRecHitsOwned_globalCosmicMuons1Leg_*_*', 
        'keep recoTracks_cosmicsVetoTracks_*_*', 
        'keep *_SETMuonSeed_*_*', 
        'keep recoTracks_standAloneSETMuons_*_*', 
        'keep recoTrackExtras_standAloneSETMuons_*_*', 
        'keep TrackingRecHitsOwned_standAloneSETMuons_*_*', 
        'keep recoTracks_globalSETMuons_*_*', 
        'keep recoTrackExtras_globalSETMuons_*_*', 
        'keep TrackingRecHitsOwned_globalSETMuons_*_*', 
        'keep recoMuons_muonsWithSET_*_*', 
        'keep *_muons_*_*', 
        'keep *_*_muons_*', 
        'drop *_muons_muons1stStep2muonsMap_*', 
        'drop recoIsoDepositedmValueMap_muons_*_*', 
        'drop doubleedmValueMap_muons_muPFIso*_*', 
        'keep recoTracks_standAloneMuons_*_*', 
        'keep recoTrackExtras_standAloneMuons_*_*', 
        'keep TrackingRecHitsOwned_standAloneMuons_*_*', 
        'keep recoTracks_globalMuons_*_*', 
        'keep recoTrackExtras_globalMuons_*_*', 
        'keep recoTracks_tevMuons_*_*', 
        'keep recoTrackExtras_tevMuons_*_*', 
        'keep recoTracks_generalTracks_*_*', 
        'keep recoTracksToOnerecoTracksAssociation_tevMuons_*_*', 
        'keep recoTracks_cosmicMuons_*_*', 
        'keep recoTracks_globalCosmicMuons_*_*', 
        'keep recoMuons_muonsFromCosmics_*_*', 
        'keep recoTracks_cosmicMuons1Leg_*_*', 
        'keep recoTracks_globalCosmicMuons1Leg_*_*', 
        'keep recoMuons_muonsFromCosmics1Leg_*_*', 
        'keep recoTracks_refittedStandAloneMuons_*_*', 
        'keep recoTrackExtras_refittedStandAloneMuons_*_*', 
        'keep TrackingRecHitsOwned_refittedStandAloneMuons_*_*', 
        'keep recoTracks_displacedStandAloneMuons__*', 
        'keep recoTrackExtras_displacedStandAloneMuons_*_*', 
        'keep TrackingRecHitsOwned_displacedStandAloneMuons_*_*', 
        'keep *_muIsoDepositTk_*_*', 
        'keep *_muIsoDepositCalByAssociatorTowers_*_*', 
        'keep *_muIsoDepositCalByAssociatorHits_*_*', 
        'keep *_muIsoDepositJets_*_*', 
        'keep *_muGlobalIsoDepositCtfTk_*_*', 
        'keep *_muGlobalIsoDepositCalByAssociatorTowers_*_*', 
        'keep *_muGlobalIsoDepositCalByAssociatorHits_*_*', 
        'keep *_muGlobalIsoDepositJets_*_*', 
        'keep *_impactParameterTagInfos_*_*', 
        'keep *_trackCountingHighEffBJetTags_*_*', 
        'keep *_trackCountingHighPurBJetTags_*_*', 
        'keep *_jetProbabilityBJetTags_*_*', 
        'keep *_jetBProbabilityBJetTags_*_*', 
        'keep *_secondaryVertexTagInfos_*_*', 
        'keep *_inclusiveSecondaryVertexFinderTagInfos_*_*', 
        'keep *_ghostTrackVertexTagInfos_*_*', 
        'keep *_simpleSecondaryVertexBJetTags_*_*', 
        'keep *_simpleSecondaryVertexHighEffBJetTags_*_*', 
        'keep *_simpleSecondaryVertexHighPurBJetTags_*_*', 
        'keep *_combinedSecondaryVertexBJetTags_*_*', 
        'keep *_combinedInclusiveSecondaryVertexV2BJetTags_*_*', 
        'keep *_ghostTrackBJetTags_*_*', 
        'keep *_softPFMuonsTagInfos_*_*', 
        'keep *_softPFElectronsTagInfos_*_*', 
        'keep *_softPFElectronBJetTags_*_*', 
        'keep *_softPFMuonBJetTags_*_*', 
        'keep *_softMuonTagInfos_*_*', 
        'keep *_softMuonBJetTags_*_*', 
        'keep *_softMuonByIP3dBJetTags_*_*', 
        'keep *_softMuonByPtBJetTags_*_*', 
        'keep *_combinedMVABJetTags_*_*', 
        'keep *_pfImpactParameterTagInfos_*_*', 
        'keep *_pfSecondaryVertexTagInfos_*_*', 
        'keep *_pfCombinedSecondaryVertexBJetTags_*_*', 
        'keep *_ak4PFJetsRecoTauPiZeros_*_*', 
        'keep *_hpsPFTauProducer_*_*', 
        'keep *_hpsPFTauDiscrimination*_*_*', 
        'keep *_hpsPFTau*PtSum_*_*', 
        'keep *_hpsPFTauTransverseImpactParameters_*_*', 
        'keep  *_offlinePrimaryVertices__*', 
        'keep  *_offlinePrimaryVerticesWithBS_*_*', 
        'keep  *_offlinePrimaryVerticesFromCosmicTracks_*_*', 
        'keep  *_nuclearInteractionMaker_*_*', 
        'keep *_generalV0Candidates_*_*', 
        'keep *_inclusiveSecondaryVertices_*_*', 
        'keep recoGsfElectronCores_gsfElectronCores_*_*', 
        'keep recoGsfElectrons_gsfElectrons_*_*', 
        'keep recoGsfElectronCores_uncleanedOnlyGsfElectronCores_*_*', 
        'keep recoGsfElectrons_uncleanedOnlyGsfElectrons_*_*', 
        'keep floatedmValueMap_eidRobustLoose_*_*', 
        'keep floatedmValueMap_eidRobustTight_*_*', 
        'keep floatedmValueMap_eidRobustHighEnergy_*_*', 
        'keep floatedmValueMap_eidLoose_*_*', 
        'keep floatedmValueMap_eidTight_*_*', 
        'keep *_egmGedGsfElectronPFIsolation_*_*', 
        'keep *_egmGsfElectronIDs_*_*', 
        'keep *_gedPhotonCore_*_*', 
        'keep *_gedPhotons_*_*', 
        'keep *_particleBasedIsolation_*_*', 
        'keep recoPhotons_mustachePhotons_*_*', 
        'keep recoPhotonCores_mustachePhotonCore_*_*', 
        'keep recoPhotons_photons_*_*', 
        'keep recoPhotonCores_photonCore_*_*', 
        'keep recoConversions_conversions_*_*', 
        'keep recoConversions_mustacheConversions_*_*', 
        'drop *_conversions_uncleanedConversions_*', 
        'drop *_gedPhotonsTmp_valMapPFEgammaCandToPhoton_*', 
        'keep recoConversions_allConversions_*_*', 
        'keep recoConversions_allConversionsOldEG_*_*', 
        'keep recoTracks_ckfOutInTracksFrom*Conversions_*_*', 
        'keep recoTracks_ckfInOutTracksFrom*Conversions_*_*', 
        'keep recoTrackExtras_ckfOutInTracksFrom*Conversions_*_*', 
        'keep recoTrackExtras_ckfInOutTracksFrom*Conversions_*_*', 
        'keep TrackingRecHitsOwned_ckfOutInTracksFrom*Conversions_*_*', 
        'keep TrackingRecHitsOwned_ckfInOutTracksFrom*Conversions_*_*', 
        'keep recoConversions_uncleanedOnlyAllConversions_*_*', 
        'keep recoTracks_uncleanedOnlyCkfOutInTracksFromConversions_*_*', 
        'keep recoTracks_uncleanedOnlyCkfInOutTracksFromConversions_*_*', 
        'keep recoTrackExtras_uncleanedOnlyCkfOutInTracksFromConversions_*_*', 
        'keep recoTrackExtras_uncleanedOnlyCkfInOutTracksFromConversions_*_*', 
        'keep TrackingRecHitsOwned_uncleanedOnlyCkfOutInTracksFromConversions_*_*', 
        'keep TrackingRecHitsOwned_uncleanedOnlyCkfInOutTracksFromConversions_*_*', 
        'keep *_PhotonIDProd_*_*', 
        'keep *_PhotonIDProdGED_*_*', 
        'keep *_hfRecoEcalCandidate_*_*', 
        'keep *_hfEMClusters_*_*', 
        'keep *_gedGsfElectronCores_*_*', 
        'keep *_gedGsfElectrons_*_*', 
        'keep *_pixelTracks_*_*', 
        'keep *_pixelVertices_*_*', 
        'drop CaloTowersSorted_towerMakerPF_*_*', 
        'keep recoPFRecHits_particleFlowClusterECAL_Cleaned_*', 
        'keep recoPFRecHits_particleFlowClusterHCAL_Cleaned_*', 
        'keep recoPFRecHits_particleFlowClusterHO_Cleaned_*', 
        'keep recoPFRecHits_particleFlowClusterHFEM_Cleaned_*', 
        'keep recoPFRecHits_particleFlowClusterHFHAD_Cleaned_*', 
        'keep recoPFRecHits_particleFlowClusterPS_Cleaned_*', 
        'keep recoPFRecHits_particleFlowRecHitECAL_Cleaned_*', 
        'keep recoPFRecHits_particleFlowRecHitHCAL_Cleaned_*', 
        'keep recoPFRecHits_particleFlowRecHitHO_Cleaned_*', 
        'keep recoPFRecHits_particleFlowRecHitPS_Cleaned_*', 
        'keep recoPFClusters_particleFlowClusterECAL_*_*', 
        'keep recoPFClusters_particleFlowClusterHCAL_*_*', 
        'keep recoPFClusters_particleFlowClusterHO_*_*', 
        'keep recoPFClusters_particleFlowClusterPS_*_*', 
        'keep recoPFBlocks_particleFlowBlock_*_*', 
        'keep recoPFCandidates_particleFlowEGamma_*_*', 
        'keep recoCaloClusters_particleFlowEGamma_*_*', 
        'keep recoSuperClusters_particleFlowEGamma_*_*', 
        'keep recoConversions_particleFlowEGamma_*_*', 
        'keep recoPFCandidates_particleFlow_*_*', 
        'keep recoPFCandidates_particleFlowTmp_electrons_*', 
        'keep recoPFCandidates_particleFlowTmp_*_*', 
        'drop recoPFCandidates_particleFlowTmp__*', 
        'keep recoPFDisplacedVertexs_particleFlowDisplacedVertex_*_*', 
        'keep *_pfElectronTranslator_*_*', 
        'keep *_pfPhotonTranslator_*_*', 
        'keep *_particleFlow_electrons_*', 
        'keep *_particleFlow_photons_*', 
        'keep *_trackerDrivenElectronSeeds_preid_*', 
        'keep *_particleFlowPtrs_*_*', 
        'keep *_particleFlowTmpPtrs_*_*', 
        'keep *_offlineBeamSpot_*_*', 
        'keep L1GlobalTriggerReadoutRecord_gtDigis_*_*', 
        'keep *_l1GtRecord_*_*', 
        'keep *_l1GtTriggerMenuLite_*_*', 
        'keep *_conditionsInEdm_*_*', 
        'keep *_l1extraParticles_*_*', 
        'keep *_l1L1GtObjectMap_*_*', 
        'keep L1MuGMTReadoutCollection_gtDigis_*_*', 
        'keep L1GctEmCand*_gctDigis_*_*', 
        'keep L1GctJetCand*_gctDigis_*_*', 
        'keep L1GctEtHad*_gctDigis_*_*', 
        'keep L1GctEtMiss*_gctDigis_*_*', 
        'keep L1GctEtTotal*_gctDigis_*_*', 
        'keep L1GctHtMiss*_gctDigis_*_*', 
        'keep L1GctJetCounts*_gctDigis_*_*', 
        'keep L1GctHFRingEtSums*_gctDigis_*_*', 
        'keep L1GctHFBitCounts*_gctDigis_*_*', 
        'keep LumiDetails_lumiProducer_*_*', 
        'keep LumiSummary_lumiProducer_*_*', 
        'drop *_hlt*_*_*', 
        'keep *_hltL1GtObjectMap_*_*', 
        'keep edmTriggerResults_*_*_*', 
        'keep triggerTriggerEvent_*_*_*', 
        'keep L1AcceptBunchCrossings_*_*_*', 
        'keep L1TriggerScalerss_*_*_*', 
        'keep Level1TriggerScalerss_*_*_*', 
        'keep LumiScalerss_*_*_*', 
        'keep BeamSpotOnlines_*_*_*', 
        'keep DcsStatuss_*_*_*', 
        'keep *_logErrorHarvester_*_*', 
        'keep *_pfIsolatedElectronsEI_*_*', 
        'keep *_pfIsolatedMuonsEI_*_*', 
        'keep recoPFJets_pfJetsEI_*_*', 
        'keep *_pfJetTrackAssociatorEI_*_*', 
        'keep *_impactParameterTagInfosEI_*_*', 
        'keep *_secondaryVertexTagInfosEI_*_*', 
        'keep *_combinedSecondaryVertexBJetTagsEI_*_*', 
        'keep recoPFTaus_pfTausEI_*_*', 
        'keep recoPFTauDiscriminator_pfTausDiscrimination*_*_*', 
        'keep *_pfMetEI_*_*', 
        'keep FEDRawDataCollection_rawDataCollector_*_*', 
        'keep FEDRawDataCollection_source_*_*' ) )
)

process.RAWRECOSIMHLTEventContent = cms.PSet(
    splitLevel = cms.untracked.int32(0),
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
    outputCommands = cms.untracked.vstring( ('drop *', 
        'drop *', 
        'drop *', 
        'keep DetIdedmEDCollection_siStripDigis_*_*', 
        'keep DetIdedmEDCollection_siPixelDigis_*_*', 
        'keep *_siPixelClusters_*_*', 
        'keep *_siStripClusters_*_*', 
        'keep *_clusterSummaryProducer_*_*', 
        'keep *_dt1DRecHits_*_*', 
        'keep *_dt4DSegments_*_*', 
        'keep *_dt1DCosmicRecHits_*_*', 
        'keep *_dt4DCosmicSegments_*_*', 
        'keep *_csc2DRecHits_*_*', 
        'keep *_cscSegments_*_*', 
        'keep *_rpcRecHits_*_*', 
        'keep *_hbhereco_*_*', 
        'keep *_hbheprereco_*_*', 
        'keep *_hfreco_*_*', 
        'keep *_horeco_*_*', 
        'keep HBHERecHitsSorted_hbherecoMB_*_*', 
        'keep HORecHitsSorted_horecoMB_*_*', 
        'keep HFRecHitsSorted_hfrecoMB_*_*', 
        'keep ZDCDataFramesSorted_*Digis_*_*', 
        'keep ZDCRecHitsSorted_*_*_*', 
        'keep *_reducedHcalRecHits_*_*', 
        'keep *_castorreco_*_*', 
        'keep HcalUnpackerReport_*_*_*', 
        'keep *_ecalPreshowerRecHit_*_*', 
        'keep *_ecalRecHit_*_*', 
        'keep *_ecalCompactTrigPrim_*_*', 
        'keep *_ecalTPSkim_*_*', 
        'keep *_selectDigi_*_*', 
        'keep EcalRecHitsSorted_reducedEcalRecHitsEE_*_*', 
        'keep EcalRecHitsSorted_reducedEcalRecHitsEB_*_*', 
        'keep EcalRecHitsSorted_reducedEcalRecHitsES_*_*', 
        'keep *_hybridSuperClusters_*_*', 
        'keep recoSuperClusters_correctedHybridSuperClusters_*_*', 
        'keep *_multi5x5SuperClusters_*_*', 
        'keep recoSuperClusters_multi5x5SuperClusters_*_*', 
        'keep recoSuperClusters_multi5x5SuperClustersWithPreshower_*_*', 
        'keep recoSuperClusters_correctedMulti5x5SuperClustersWithPreshower_*_*', 
        'keep recoPreshowerClusters_multi5x5SuperClustersWithPreshower_*_*', 
        'keep recoPreshowerClusterShapes_multi5x5PreshowerClusterShape_*_*', 
        'keep *_particleFlowSuperClusterECAL_*_*', 
        'drop recoClusterShapes_*_*_*', 
        'drop recoBasicClustersToOnerecoClusterShapesAssociation_*_*_*', 
        'drop recoBasicClusters_multi5x5BasicClusters_multi5x5BarrelBasicClusters_*', 
        'drop recoSuperClusters_multi5x5SuperClusters_multi5x5BarrelSuperClusters_*', 
        'keep *_CkfElectronCandidates_*_*', 
        'keep *_GsfGlobalElectronTest_*_*', 
        'keep *_electronMergedSeeds_*_*', 
        'keep recoGsfTracks_electronGsfTracks_*_*', 
        'keep recoGsfTrackExtras_electronGsfTracks_*_*', 
        'keep recoTrackExtras_electronGsfTracks_*_*', 
        'keep TrackingRecHitsOwned_electronGsfTracks_*_*', 
        'keep recoTracks_generalTracks_*_*', 
        'keep recoTrackExtras_generalTracks_*_*', 
        'keep TrackingRecHitsOwned_extraFromSeeds_*_*', 
        'keep uints_extraFromSeeds_*_*', 
        'keep TrackingRecHitsOwned_generalTracks_*_*', 
        'keep recoTracks_beamhaloTracks_*_*', 
        'keep recoTrackExtras_beamhaloTracks_*_*', 
        'keep TrackingRecHitsOwned_beamhaloTracks_*_*', 
        'keep recoTracks_regionalCosmicTracks_*_*', 
        'keep recoTrackExtras_regionalCosmicTracks_*_*', 
        'keep TrackingRecHitsOwned_regionalCosmicTracks_*_*', 
        'keep recoTracks_rsWithMaterialTracks_*_*', 
        'keep recoTrackExtras_rsWithMaterialTracks_*_*', 
        'keep TrackingRecHitsOwned_rsWithMaterialTracks_*_*', 
        'keep recoTracks_conversionStepTracks_*_*', 
        'keep recoTrackExtras_conversionStepTracks_*_*', 
        'keep TrackingRecHitsOwned_conversionStepTracks_*_*', 
        'keep *_ctfPixelLess_*_*', 
        'keep *_dedxTruncated40_*_*', 
        'keep *_dedxDiscrimASmi_*_*', 
        'keep *_dedxHarmonic2_*_*', 
        'keep *_trackExtrapolator_*_*', 
        'keep floatedmValueMap_generalTracks_*_*', 
        'keep *_kt6CaloJets_*_*', 
        'keep *_ak4CaloJets_*_*', 
        'keep *_ak5CaloJets_*_*', 
        'keep double_kt6PFJets_rho_*', 
        'keep *_ak4PFJets_*_*', 
        'keep *_ak5PFJets_*_*', 
        'keep *_ak8PFJets_*_*', 
        'keep *_ak4PFJetsCHS_*_*', 
        'keep *_ak5PFJetsCHS_*_*', 
        'keep *_ak8PFJetsCHS_*_*', 
        'keep *_ca8PFJetsCHS_*_*', 
        'keep *_ca8PFJetsCHSPruned_*_*', 
        'keep *_cmsTopTagPFJetsCHS_*_*', 
        'keep *_JetPlusTrackZSPCorJetAntiKt4_*_*', 
        'keep *_ak4TrackJets_*_*', 
        'keep recoRecoChargedRefCandidates_trackRefsForJets_*_*', 
        'keep *_caloTowers_*_*', 
        'keep *_towerMaker_*_*', 
        'keep *_CastorTowerReco_*_*', 
        'keep *_kt4JetTracksAssociatorAtVertex_*_*', 
        'keep *_kt4JetTracksAssociatorAtCaloFace_*_*', 
        'keep *_kt4JetExtender_*_*', 
        'keep *_ak4JetTracksAssociatorAtVertex_*_*', 
        'keep *_ak4JetTracksAssociatorAtVertexPF_*_*', 
        'keep *_ak4JetTracksAssociatorAtCaloFace_*_*', 
        'keep *_ak4JetTracksAssociatorExplicit_*_*', 
        'keep *_ak4JetExtender_*_*', 
        'keep *_ak7JetTracksAssociatorAtVertex*_*_*', 
        'keep *_ak7JetTracksAssociatorAtCaloFace*_*_*', 
        'keep *_ak7JetExtender_*_*', 
        'keep *_ak4JetID_*_*', 
        'keep *_ak5JetID_*_*', 
        'keep *_ak7JetID_*_*', 
        'keep *_ic5JetID_*_*', 
        'keep *_kt4JetID_*_*', 
        'keep *_kt6JetID_*_*', 
        'keep *_ak7BasicJets_*_*', 
        'keep *_ak7CastorJetID_*_*', 
        'keep double_kt6CaloJetsCentral_*_*', 
        'keep double_kt6PFJetsCentralChargedPileUp_*_*', 
        'keep double_kt6PFJetsCentralNeutral_*_*', 
        'keep double_kt6PFJetsCentralNeutralTight_*_*', 
        'keep *_fixedGridRho*_*_*', 
        'keep *_ca8PFJetsCHSPrunedLinks_*_*', 
        'keep recoCaloMETs_caloMet_*_*', 
        'keep recoCaloMETs_caloMetBE_*_*', 
        'keep recoCaloMETs_caloMetBEFO_*_*', 
        'keep recoCaloMETs_caloMetM_*_*', 
        'keep recoPFMETs_pfMet_*_*', 
        'keep recoPFMETs_pfChMet_*_*', 
        'keep recoMuonMETCorrectionDataedmValueMap_muonMETValueMapProducer_*_*', 
        'keep recoHcalNoiseRBXs_hcalnoise_*_*', 
        'keep HcalNoiseSummary_hcalnoise_*_*', 
        'keep *HaloData_*_*_*', 
        'keep *BeamHaloSummary_BeamHaloSummary_*_*', 
        'keep *_MuonSeed_*_*', 
        'keep *_ancientMuonSeed_*_*', 
        'keep *_displacedMuonSeeds_*_*', 
        'keep *_mergedStandAloneMuonSeeds_*_*', 
        'keep TrackingRecHitsOwned_globalMuons_*_*', 
        'keep TrackingRecHitsOwned_tevMuons_*_*', 
        'keep recoCaloMuons_calomuons_*_*', 
        'keep *_CosmicMuonSeed_*_*', 
        'keep recoTrackExtras_cosmicMuons_*_*', 
        'keep TrackingRecHitsOwned_cosmicMuons_*_*', 
        'keep recoTrackExtras_globalCosmicMuons_*_*', 
        'keep TrackingRecHitsOwned_globalCosmicMuons_*_*', 
        'keep recoTrackExtras_cosmicMuons1Leg_*_*', 
        'keep TrackingRecHitsOwned_cosmicMuons1Leg_*_*', 
        'keep recoTrackExtras_globalCosmicMuons1Leg_*_*', 
        'keep TrackingRecHitsOwned_globalCosmicMuons1Leg_*_*', 
        'keep recoTracks_cosmicsVetoTracks_*_*', 
        'keep *_SETMuonSeed_*_*', 
        'keep recoTracks_standAloneSETMuons_*_*', 
        'keep recoTrackExtras_standAloneSETMuons_*_*', 
        'keep TrackingRecHitsOwned_standAloneSETMuons_*_*', 
        'keep recoTracks_globalSETMuons_*_*', 
        'keep recoTrackExtras_globalSETMuons_*_*', 
        'keep TrackingRecHitsOwned_globalSETMuons_*_*', 
        'keep recoMuons_muonsWithSET_*_*', 
        'keep *_muons_*_*', 
        'keep *_*_muons_*', 
        'drop *_muons_muons1stStep2muonsMap_*', 
        'drop recoIsoDepositedmValueMap_muons_*_*', 
        'drop doubleedmValueMap_muons_muPFIso*_*', 
        'keep recoTracks_standAloneMuons_*_*', 
        'keep recoTrackExtras_standAloneMuons_*_*', 
        'keep TrackingRecHitsOwned_standAloneMuons_*_*', 
        'keep recoTracks_globalMuons_*_*', 
        'keep recoTrackExtras_globalMuons_*_*', 
        'keep recoTracks_tevMuons_*_*', 
        'keep recoTrackExtras_tevMuons_*_*', 
        'keep recoTracks_generalTracks_*_*', 
        'keep recoTracksToOnerecoTracksAssociation_tevMuons_*_*', 
        'keep recoTracks_cosmicMuons_*_*', 
        'keep recoTracks_globalCosmicMuons_*_*', 
        'keep recoMuons_muonsFromCosmics_*_*', 
        'keep recoTracks_cosmicMuons1Leg_*_*', 
        'keep recoTracks_globalCosmicMuons1Leg_*_*', 
        'keep recoMuons_muonsFromCosmics1Leg_*_*', 
        'keep recoTracks_refittedStandAloneMuons_*_*', 
        'keep recoTrackExtras_refittedStandAloneMuons_*_*', 
        'keep TrackingRecHitsOwned_refittedStandAloneMuons_*_*', 
        'keep recoTracks_displacedStandAloneMuons__*', 
        'keep recoTrackExtras_displacedStandAloneMuons_*_*', 
        'keep TrackingRecHitsOwned_displacedStandAloneMuons_*_*', 
        'keep *_muIsoDepositTk_*_*', 
        'keep *_muIsoDepositCalByAssociatorTowers_*_*', 
        'keep *_muIsoDepositCalByAssociatorHits_*_*', 
        'keep *_muIsoDepositJets_*_*', 
        'keep *_muGlobalIsoDepositCtfTk_*_*', 
        'keep *_muGlobalIsoDepositCalByAssociatorTowers_*_*', 
        'keep *_muGlobalIsoDepositCalByAssociatorHits_*_*', 
        'keep *_muGlobalIsoDepositJets_*_*', 
        'keep *_impactParameterTagInfos_*_*', 
        'keep *_trackCountingHighEffBJetTags_*_*', 
        'keep *_trackCountingHighPurBJetTags_*_*', 
        'keep *_jetProbabilityBJetTags_*_*', 
        'keep *_jetBProbabilityBJetTags_*_*', 
        'keep *_secondaryVertexTagInfos_*_*', 
        'keep *_inclusiveSecondaryVertexFinderTagInfos_*_*', 
        'keep *_ghostTrackVertexTagInfos_*_*', 
        'keep *_simpleSecondaryVertexBJetTags_*_*', 
        'keep *_simpleSecondaryVertexHighEffBJetTags_*_*', 
        'keep *_simpleSecondaryVertexHighPurBJetTags_*_*', 
        'keep *_combinedSecondaryVertexBJetTags_*_*', 
        'keep *_combinedInclusiveSecondaryVertexV2BJetTags_*_*', 
        'keep *_ghostTrackBJetTags_*_*', 
        'keep *_softPFMuonsTagInfos_*_*', 
        'keep *_softPFElectronsTagInfos_*_*', 
        'keep *_softPFElectronBJetTags_*_*', 
        'keep *_softPFMuonBJetTags_*_*', 
        'keep *_softMuonTagInfos_*_*', 
        'keep *_softMuonBJetTags_*_*', 
        'keep *_softMuonByIP3dBJetTags_*_*', 
        'keep *_softMuonByPtBJetTags_*_*', 
        'keep *_combinedMVABJetTags_*_*', 
        'keep *_pfImpactParameterTagInfos_*_*', 
        'keep *_pfSecondaryVertexTagInfos_*_*', 
        'keep *_pfCombinedSecondaryVertexBJetTags_*_*', 
        'keep *_ak4PFJetsRecoTauPiZeros_*_*', 
        'keep *_hpsPFTauProducer_*_*', 
        'keep *_hpsPFTauDiscrimination*_*_*', 
        'keep *_hpsPFTau*PtSum_*_*', 
        'keep *_hpsPFTauTransverseImpactParameters_*_*', 
        'keep  *_offlinePrimaryVertices__*', 
        'keep  *_offlinePrimaryVerticesWithBS_*_*', 
        'keep  *_offlinePrimaryVerticesFromCosmicTracks_*_*', 
        'keep  *_nuclearInteractionMaker_*_*', 
        'keep *_generalV0Candidates_*_*', 
        'keep *_inclusiveSecondaryVertices_*_*', 
        'keep recoGsfElectronCores_gsfElectronCores_*_*', 
        'keep recoGsfElectrons_gsfElectrons_*_*', 
        'keep recoGsfElectronCores_uncleanedOnlyGsfElectronCores_*_*', 
        'keep recoGsfElectrons_uncleanedOnlyGsfElectrons_*_*', 
        'keep floatedmValueMap_eidRobustLoose_*_*', 
        'keep floatedmValueMap_eidRobustTight_*_*', 
        'keep floatedmValueMap_eidRobustHighEnergy_*_*', 
        'keep floatedmValueMap_eidLoose_*_*', 
        'keep floatedmValueMap_eidTight_*_*', 
        'keep *_egmGedGsfElectronPFIsolation_*_*', 
        'keep *_egmGsfElectronIDs_*_*', 
        'keep *_gedPhotonCore_*_*', 
        'keep *_gedPhotons_*_*', 
        'keep *_particleBasedIsolation_*_*', 
        'keep recoPhotons_mustachePhotons_*_*', 
        'keep recoPhotonCores_mustachePhotonCore_*_*', 
        'keep recoPhotons_photons_*_*', 
        'keep recoPhotonCores_photonCore_*_*', 
        'keep recoConversions_conversions_*_*', 
        'keep recoConversions_mustacheConversions_*_*', 
        'drop *_conversions_uncleanedConversions_*', 
        'drop *_gedPhotonsTmp_valMapPFEgammaCandToPhoton_*', 
        'keep recoConversions_allConversions_*_*', 
        'keep recoConversions_allConversionsOldEG_*_*', 
        'keep recoTracks_ckfOutInTracksFrom*Conversions_*_*', 
        'keep recoTracks_ckfInOutTracksFrom*Conversions_*_*', 
        'keep recoTrackExtras_ckfOutInTracksFrom*Conversions_*_*', 
        'keep recoTrackExtras_ckfInOutTracksFrom*Conversions_*_*', 
        'keep TrackingRecHitsOwned_ckfOutInTracksFrom*Conversions_*_*', 
        'keep TrackingRecHitsOwned_ckfInOutTracksFrom*Conversions_*_*', 
        'keep recoConversions_uncleanedOnlyAllConversions_*_*', 
        'keep recoTracks_uncleanedOnlyCkfOutInTracksFromConversions_*_*', 
        'keep recoTracks_uncleanedOnlyCkfInOutTracksFromConversions_*_*', 
        'keep recoTrackExtras_uncleanedOnlyCkfOutInTracksFromConversions_*_*', 
        'keep recoTrackExtras_uncleanedOnlyCkfInOutTracksFromConversions_*_*', 
        'keep TrackingRecHitsOwned_uncleanedOnlyCkfOutInTracksFromConversions_*_*', 
        'keep TrackingRecHitsOwned_uncleanedOnlyCkfInOutTracksFromConversions_*_*', 
        'keep *_PhotonIDProd_*_*', 
        'keep *_PhotonIDProdGED_*_*', 
        'keep *_hfRecoEcalCandidate_*_*', 
        'keep *_hfEMClusters_*_*', 
        'keep *_gedGsfElectronCores_*_*', 
        'keep *_gedGsfElectrons_*_*', 
        'keep *_pixelTracks_*_*', 
        'keep *_pixelVertices_*_*', 
        'drop CaloTowersSorted_towerMakerPF_*_*', 
        'keep recoPFRecHits_particleFlowClusterECAL_Cleaned_*', 
        'keep recoPFRecHits_particleFlowClusterHCAL_Cleaned_*', 
        'keep recoPFRecHits_particleFlowClusterHO_Cleaned_*', 
        'keep recoPFRecHits_particleFlowClusterHFEM_Cleaned_*', 
        'keep recoPFRecHits_particleFlowClusterHFHAD_Cleaned_*', 
        'keep recoPFRecHits_particleFlowClusterPS_Cleaned_*', 
        'keep recoPFRecHits_particleFlowRecHitECAL_Cleaned_*', 
        'keep recoPFRecHits_particleFlowRecHitHCAL_Cleaned_*', 
        'keep recoPFRecHits_particleFlowRecHitHO_Cleaned_*', 
        'keep recoPFRecHits_particleFlowRecHitPS_Cleaned_*', 
        'keep recoPFClusters_particleFlowClusterECAL_*_*', 
        'keep recoPFClusters_particleFlowClusterHCAL_*_*', 
        'keep recoPFClusters_particleFlowClusterHO_*_*', 
        'keep recoPFClusters_particleFlowClusterPS_*_*', 
        'keep recoPFBlocks_particleFlowBlock_*_*', 
        'keep recoPFCandidates_particleFlowEGamma_*_*', 
        'keep recoCaloClusters_particleFlowEGamma_*_*', 
        'keep recoSuperClusters_particleFlowEGamma_*_*', 
        'keep recoConversions_particleFlowEGamma_*_*', 
        'keep recoPFCandidates_particleFlow_*_*', 
        'keep recoPFCandidates_particleFlowTmp_electrons_*', 
        'keep recoPFCandidates_particleFlowTmp_*_*', 
        'drop recoPFCandidates_particleFlowTmp__*', 
        'keep recoPFDisplacedVertexs_particleFlowDisplacedVertex_*_*', 
        'keep *_pfElectronTranslator_*_*', 
        'keep *_pfPhotonTranslator_*_*', 
        'keep *_particleFlow_electrons_*', 
        'keep *_particleFlow_photons_*', 
        'keep *_trackerDrivenElectronSeeds_preid_*', 
        'keep *_particleFlowPtrs_*_*', 
        'keep *_particleFlowTmpPtrs_*_*', 
        'keep *_offlineBeamSpot_*_*', 
        'keep L1GlobalTriggerReadoutRecord_gtDigis_*_*', 
        'keep *_l1GtRecord_*_*', 
        'keep *_l1GtTriggerMenuLite_*_*', 
        'keep *_conditionsInEdm_*_*', 
        'keep *_l1extraParticles_*_*', 
        'keep *_l1L1GtObjectMap_*_*', 
        'keep L1MuGMTReadoutCollection_gtDigis_*_*', 
        'keep L1GctEmCand*_gctDigis_*_*', 
        'keep L1GctJetCand*_gctDigis_*_*', 
        'keep L1GctEtHad*_gctDigis_*_*', 
        'keep L1GctEtMiss*_gctDigis_*_*', 
        'keep L1GctEtTotal*_gctDigis_*_*', 
        'keep L1GctHtMiss*_gctDigis_*_*', 
        'keep L1GctJetCounts*_gctDigis_*_*', 
        'keep L1GctHFRingEtSums*_gctDigis_*_*', 
        'keep L1GctHFBitCounts*_gctDigis_*_*', 
        'keep LumiDetails_lumiProducer_*_*', 
        'keep LumiSummary_lumiProducer_*_*', 
        'drop *_hlt*_*_*', 
        'keep *_hltL1GtObjectMap_*_*', 
        'keep edmTriggerResults_*_*_*', 
        'keep triggerTriggerEvent_*_*_*', 
        'keep L1AcceptBunchCrossings_*_*_*', 
        'keep L1TriggerScalerss_*_*_*', 
        'keep Level1TriggerScalerss_*_*_*', 
        'keep LumiScalerss_*_*_*', 
        'keep BeamSpotOnlines_*_*_*', 
        'keep DcsStatuss_*_*_*', 
        'keep *_logErrorHarvester_*_*', 
        'keep *_pfIsolatedElectronsEI_*_*', 
        'keep *_pfIsolatedMuonsEI_*_*', 
        'keep recoPFJets_pfJetsEI_*_*', 
        'keep *_pfJetTrackAssociatorEI_*_*', 
        'keep *_impactParameterTagInfosEI_*_*', 
        'keep *_secondaryVertexTagInfosEI_*_*', 
        'keep *_combinedSecondaryVertexBJetTagsEI_*_*', 
        'keep recoPFTaus_pfTausEI_*_*', 
        'keep recoPFTauDiscriminator_pfTausDiscrimination*_*_*', 
        'keep *_pfMetEI_*_*', 
        'keep FEDRawDataCollection_rawDataCollector_*_*', 
        'keep FEDRawDataCollection_source_*_*', 
        'keep LHERunInfoProduct_*_*_*', 
        'keep LHEEventProduct_*_*_*', 
        'keep GenRunInfoProduct_generator_*_*', 
        'keep GenLumiInfoProduct_generator_*_*', 
        'keep GenEventInfoProduct_generator_*_*', 
        'keep edmHepMCProduct_generator_*_*', 
        'keep GenFilterInfo_*_*_*', 
        'keep *_genParticles_*_*', 
        'keep recoGenMETs_*_*_*', 
        'keep *_kt4GenJets_*_*', 
        'keep *_kt6GenJets_*_*', 
        'keep *_ak4GenJets_*_*', 
        'keep *_ak5GenJets_*_*', 
        'keep *_ak7GenJets_*_*', 
        'keep *_iterativeCone5GenJets_*_*', 
        'keep *_genParticle_*_*', 
        'keep edmHepMCProduct_source_*_*', 
        'keep SimTracks_g4SimHits_*_*', 
        'keep SimVertexs_g4SimHits_*_*', 
        'keep *_allTrackMCMatch_*_*', 
        'keep StripDigiSimLinkedmDetSetVector_simMuonCSCDigis_*_*', 
        'keep DTLayerIdDTDigiSimLinkMuonDigiCollection_simMuonDTDigis_*_*', 
        'keep RPCDigiSimLinkedmDetSetVector_simMuonRPCDigis_*_*', 
        'keep PileupSummaryInfos_*_*_*', 
        'drop *_hlt*_*_*', 
        'keep *_hltAK4CaloJetsCorrectedIDPassed_*_*', 
        'keep *_hltAK4CaloJetsIDPassed_*_*', 
        'keep *_hltAK4CaloJets_*_*', 
        'keep *_hltAK4PFJetsCorrected_*_*', 
        'keep *_hltAK4PFJetsForTaus_*_*', 
        'keep *_hltAK4PFJets_*_*', 
        'keep *_hltAlCaEtaEBUncalibrator_*_*', 
        'keep *_hltAlCaEtaEEUncalibrator_*_*', 
        'keep *_hltAlCaEtaRecHitsFilterEBonly_*_*', 
        'keep *_hltAlCaEtaRecHitsFilterEEonly_*_*', 
        'keep *_hltAlCaEtaRecHitsFilterEEonly_etaEcalRecHitsES_*', 
        'keep *_hltAlCaEtaRecHitsFilter_*_*', 
        'keep *_hltAlCaPhiSymStream_*_*', 
        'keep *_hltAlCaPhiSymUncalibrator_*_*', 
        'keep *_hltAlCaPi0EBUncalibrator_*_*', 
        'keep *_hltAlCaPi0EEUncalibrator_*_*', 
        'keep *_hltAlCaPi0RecHitsFilterEBonly_*_*', 
        'keep *_hltAlCaPi0RecHitsFilterEEonly_*_*', 
        'keep *_hltAlCaPi0RecHitsFilterEEonly_pi0EcalRecHitsES_*', 
        'keep *_hltAlCaPi0RecHitsFilter_*_*', 
        'keep *_hltBLifetimeL25AssociatorbbPhiL1FastJetFastPV_*_*', 
        'keep *_hltBLifetimeL25BJetTagsbbPhiL1FastJetFastPV_*_*', 
        'keep *_hltBLifetimeL25TagInfosbbPhiL1FastJetFastPV_*_*', 
        'keep *_hltBLifetimeL3AssociatorbbPhiL1FastJetFastPV_*_*', 
        'keep *_hltBLifetimeL3BJetTagsbbPhiL1FastJetFastPV_*_*', 
        'keep *_hltBLifetimeL3TagInfosbbPhiL1FastJetFastPV_*_*', 
        'keep *_hltBLifetimeRegionalCtfWithMaterialTracksbbPhiL1FastJetFastPV_*_*', 
        'keep *_hltBSoftMuonDiJet110Mu5L3FilterByDR_*_*', 
        'keep *_hltBSoftMuonDiJet110Mu5SelL3BJetTagsByDR_*_*', 
        'keep *_hltBSoftMuonDiJet20Mu5L3FilterByDR_*_*', 
        'keep *_hltBSoftMuonDiJet20Mu5SelL3BJetTagsByDR_*_*', 
        'keep *_hltBSoftMuonDiJet40Mu5L3FilterByDR_*_*', 
        'keep *_hltBSoftMuonDiJet40Mu5SelL3BJetTagsByDR_*_*', 
        'keep *_hltBSoftMuonDiJet70Mu5L3FilterByDR_*_*', 
        'keep *_hltBSoftMuonDiJet70Mu5SelL3BJetTagsByDR_*_*', 
        'keep *_hltBSoftMuonMu5L3_*_*', 
        'keep *_hltCSVJetTagSingleTopEle27_*_*', 
        'keep *_hltCSVJetTagSingleTopIsoMu24_*_*', 
        'keep *_hltCaloJetCorrectedRegional_*_*', 
        'keep *_hltCaloJetCorrected_*_*', 
        'keep *_hltCaloJetL1FastJetCorrected_*_*', 
        'keep *_hltCleanedCorrectedIslandBarrelSuperClustersHI_*_*', 
        'keep *_hltCleanedHiCorrectedIslandBarrelSuperClustersHI_*_*', 
        'keep *_hltConvPFTausTightIsoTrackFindingIsolation_*_*', 
        'keep *_hltConvPFTausTightIsoTrackFinding_*_*', 
        'keep *_hltConvPFTausTightIsoTrackPt5Isolation_*_*', 
        'keep *_hltConvPFTausTightIsoTrackPt5_*_*', 
        'keep *_hltConvPFTausTightIso_*_*', 
        'keep *_hltConvPFTausTrackFindingLooseIsolation_*_*', 
        'keep *_hltConvPFTausTrackFinding_*_*', 
        'keep *_hltConvPFTaus_*_*', 
        'keep *_hltCorrectedIslandBarrelSuperClustersHI_*_*', 
        'keep *_hltCorrectedIslandEndcapSuperClustersHI_*_*', 
        'keep *_hltCsc2DRecHits_*_*', 
        'keep *_hltCscSegments_*_*', 
        'keep *_hltDoublePFTau25TrackPt5MediumIsolationProng4L1HLTMatched_*_*', 
        'keep *_hltDoublePFTau25TrackPt5MediumIsolationProng4_*_*', 
        'keep *_hltDoublePFTau25TrackPt5MediumIsolation_*_*', 
        'keep *_hltDoublePFTau25TrackPt5_*_*', 
        'keep *_hltDoublePFTau25_*_*', 
        'keep *_hltDoublePFTauTightIso45Track5_*_*', 
        'keep *_hltDoublePFTauTightIso45Track_*_*', 
        'keep *_hltDt4DSegments_*_*', 
        'keep *_hltEcalRecHitAll_*_*', 
        'keep *_hltEle20CaloIdVTCaloIsoTTrkIdTTrkIsoTTrackIsoFilterL1SingleEG18orL1SingleEG20_*_*', 
        'keep *_hltEle20CaloIdVTTrkIdTDphiFilter_*_*', 
        'keep *_hltEle27WP85PixelMatchElectronsL1Seeded_*_*', 
        'keep *_hltFastPVPixelTracksMerger_*_*', 
        'keep *_hltFastPVPixelTracksRecover_*_*', 
        'keep *_hltFastPVPixelTracks_*_*', 
        'keep *_hltFastPVPixelVertices3D_*_*', 
        'keep *_hltFastPVPixelVertices_*_*', 
        'keep *_hltFastPixelBLifetimeL3TagInfos_*_*', 
        'keep *_hltFastPrimaryVertex_*_*', 
        'keep *_hltFilterDoubleIsoPFTau45Trk5LeadTrack5IsolationL1HLTMatched_*_*', 
        'keep *_hltFilterL2EtCutDoublePFIsoTau45Trk5_*_*', 
        'keep *_hltFilterL2EtCutSingleIsoPFTau35Trk20MET70_*_*', 
        'keep *_hltFilterSingleIsoPFTau35Trk20LeadTrackPt20_*_*', 
        'keep *_hltFilterSingleIsoPFTau35Trk20MET60LeadTrack20IsolationL1HLTMatched_*_*', 
        'keep *_hltFilterSingleIsoPFTau35Trk20MET70LeadTrack20IsolationL1HLTMatched_*_*', 
        'keep *_hltGoodOnlinePVs_*_*', 
        'keep *_hltHICaloJetCorrected_*_*', 
        'keep *_hltHICaloJetIDPassed_*_*', 
        'keep *_hltHIGoodLooseTracks_*_*', 
        'keep *_hltHIPixel3PrimTracks_*_*', 
        'keep *_hltHISelectedVertex_*_*', 
        'keep *_hltHISiPixelClusters_*_*', 
        'keep *_hltHITIPTCorrectorHB_*_*', 
        'keep *_hltHITIPTCorrectorHE_*_*', 
        'keep *_hltHiCorrectedIslandBarrelSuperClustersHI_*_*', 
        'keep *_hltHiCorrectedIslandEndcapSuperClustersHI_*_*', 
        'keep *_hltHiIslandSuperClustersHI_*_*', 
        'keep *_hltIsolPixelTrackProdHB_*_*', 
        'keep *_hltIsolPixelTrackProdHE_*_*', 
        'keep *_hltIter0PFlowTrackSelectionHighPurity_*_*', 
        'keep *_hltIter1Merged_*_*', 
        'keep *_hltIter1PFlowTrackSelectionHighPurity_*_*', 
        'keep *_hltIter2Merged_*_*', 
        'keep *_hltIter2PFlowTrackSelectionHighPurity_*_*', 
        'keep *_hltIter3Merged_*_*', 
        'keep *_hltIter4Merged_*_*', 
        'keep *_hltIterativeCone5PileupSubtractionCaloJets_*_*', 
        'keep *_hltL1GtObjectMap_*_*', 
        'keep *_hltL1HLTSingleIsoPFTau35Trk20Met60JetsMatch_*_*', 
        'keep *_hltL1IsoElectronTrackIsol_*_*', 
        'keep *_hltL1NonIsoElectronTrackIsol_*_*', 
        'keep *_hltL1SeededRecoEcalCandidate_*_*', 
        'keep *_hltL1extraParticlesCentral_*_*', 
        'keep *_hltL1extraParticlesNonIsolated_*_*', 
        'keep *_hltL1extraParticlesTau_*_*', 
        'keep *_hltL1extraParticles_*_*', 
        'keep *_hltL1sDoubleTauJet44Eta2p17orDoubleJet64Central_*_*', 
        'keep *_hltL1sDoubleTauJet44erorDoubleJetC64_*_*', 
        'keep *_hltL1sL1EG18er_*_*', 
        'keep *_hltL1sL1ETM36ORETM40_*_*', 
        'keep *_hltL1sL1Jet52ETM30_*_*', 
        'keep *_hltL1sL1SingleEG12_*_*', 
        'keep *_hltL1sL1SingleEG15_*_*', 
        'keep *_hltL1sL1SingleEG18orL1SingleEG20_*_*', 
        'keep *_hltL1sL1SingleMu10_*_*', 
        'keep *_hltL1sL1SingleMu14Eta2p1_*_*', 
        'keep *_hltL1sMu16Eta2p1_*_*', 
        'keep *_hltL2MuonCandidatesNoVtx_*_*', 
        'keep *_hltL2MuonCandidates_*_*', 
        'keep *_hltL2MuonSeeds_*_*', 
        'keep *_hltL2Muons_*_*', 
        'keep *_hltL2TauJets_*_*', 
        'keep *_hltL3CombinedSecondaryVertexBJetTags_*_*', 
        'keep *_hltL3MuonCandidates_*_*', 
        'keep *_hltL3MuonsIOHit_*_*', 
        'keep *_hltL3MuonsLinksCombination_*_*', 
        'keep *_hltL3MuonsOIHit_*_*', 
        'keep *_hltL3MuonsOIState_*_*', 
        'keep *_hltL3Muons_*_*', 
        'keep *_hltL3NoFiltersNoVtxMuonCandidates_*_*', 
        'keep *_hltL3NoFiltersNoVtxMuons_*_*', 
        'keep *_hltL3SecondaryVertexTagInfos_*_*', 
        'keep *_hltL3TkFromL2OICombination_*_*', 
        'keep *_hltL3TkTracksFromL2IOHit_*_*', 
        'keep *_hltL3TkTracksFromL2OIHit_*_*', 
        'keep *_hltL3TkTracksFromL2OIState_*_*', 
        'keep *_hltL3TkTracksFromL2_*_*', 
        'keep *_hltL3TrackCandidateFromL2IOHit_*_*', 
        'keep *_hltL3TrackCandidateFromL2OIHit_*_*', 
        'keep *_hltL3TrackCandidateFromL2OIState_*_*', 
        'keep *_hltL3TrajSeedIOHit_*_*', 
        'keep *_hltL3TrajSeedOIHit_*_*', 
        'keep *_hltL3TrajSeedOIState_*_*', 
        'keep *_hltL3TrajectorySeed_*_*', 
        'keep *_hltL3crIsoL1sMu16Eta2p1L1f0L2f16QL3f18QL3crIsoRhoFiltered0p15_*_*', 
        'keep *_hltLeadingCentralJets30SingleTopEle27_*_*', 
        'keep *_hltLeadingCentralJets30SingleTopIsoMu24_*_*', 
        'keep *_hltMetForHf_*_*', 
        'keep *_hltMet_*_*', 
        'keep *_hltMu8Ele17CaloIdTCaloIsoVLPixelMatchFilter_*_*', 
        'keep *_hltMuTrackJpsiCtfTrackCands_*_*', 
        'keep *_hltMuTrackJpsiPixelTrackCands_*_*', 
        'keep *_hltMuonCSCDigis_*_*', 
        'keep *_hltMuonCSCDigis_MuonCSCStripDigi_*', 
        'keep *_hltMuonCSCDigis_MuonCSCWireDigi_*', 
        'keep *_hltMuonDTDigis_*_*', 
        'keep *_hltMuonRPCDigis_*_*', 
        'keep *_hltOnlineBeamSpot_*_*', 
        'keep *_hltOnlinePrimaryVertices_*_*', 
        'keep *_hltOverlapFilterEle20LooseIsoPFTau20OldVersion_*_*', 
        'keep *_hltOverlapFilterIsoEle20MediumIsoPFTau20_*_*', 
        'keep *_hltOverlapFilterIsoMu15IsoPFTau20_*_*', 
        'keep *_hltOverlapFilterIsoMu15MediumIsoPFTau20_*_*', 
        'keep *_hltOverlapFilterIsoMu15TightIsoPFTau20_*_*', 
        'keep *_hltOverlapFilterIsoMu18LooseIsoPFTau20_*_*', 
        'keep *_hltOverlapFilterIsoMu18PFTau25TrackPt5Prong4_*_*', 
        'keep *_hltPFTau15TrackLooseIso_*_*', 
        'keep *_hltPFTau15Track_*_*', 
        'keep *_hltPFTau15_*_*', 
        'keep *_hltPFTau20IsoMuVertex_*_*', 
        'keep *_hltPFTau20TrackLooseIso_*_*', 
        'keep *_hltPFTau20Track_*_*', 
        'keep *_hltPFTau20_*_*', 
        'keep *_hltPFTau25TrackPt5MediumIsolationProng4IsoMuVertex_*_*', 
        'keep *_hltPFTau25TrackPt5MediumIsolationProng4_*_*', 
        'keep *_hltPFTau25TrackPt5MediumIsolation_*_*', 
        'keep *_hltPFTau25TrackPt5_*_*', 
        'keep *_hltPFTau25_*_*', 
        'keep *_hltPFTau35TrackPt20LooseIsoProng2_*_*', 
        'keep *_hltPFTau35TrackPt20LooseIso_*_*', 
        'keep *_hltPFTau35TrackPt20_*_*', 
        'keep *_hltPFTau35Track_*_*', 
        'keep *_hltPFTau35_*_*', 
        'keep *_hltPFTauEleVertex20_*_*', 
        'keep *_hltPFTauJetTracksAssociator_*_*', 
        'keep *_hltPFTauMediumIso20TrackMediumIso_*_*', 
        'keep *_hltPFTauMediumIso20Track_*_*', 
        'keep *_hltPFTauMediumIso20_*_*', 
        'keep *_hltPFTauMediumIso35Track_*_*', 
        'keep *_hltPFTauMediumIso35_*_*', 
        'keep *_hltPFTauTagInfo_*_*', 
        'keep *_hltPFTauTightIso20TrackTightIso_*_*', 
        'keep *_hltPFTauTightIso20Track_*_*', 
        'keep *_hltPFTauTightIso20_*_*', 
        'keep *_hltPFlowTrackSelectionHighPurity_*_*', 
        'keep *_hltParticleFlowForTaus_*_*', 
        'keep *_hltParticleFlow_*_*', 
        'keep *_hltPixelMatch3HitElectronsActivity_*_*', 
        'keep *_hltPixelMatch3HitElectronsL1Seeded_*_*', 
        'keep *_hltPixelMatchCleanElectronsL1Seeded_*_*', 
        'keep *_hltPixelMatchElectronsActivity_*_*', 
        'keep *_hltPixelMatchElectronsL1Iso_*_*', 
        'keep *_hltPixelMatchElectronsL1NonIso_*_*', 
        'keep *_hltPixelMatchElectronsL1Seeded_*_*', 
        'keep *_hltPixelTracks_*_*', 
        'keep *_hltPixelVertices3DbbPhi_*_*', 
        'keep *_hltPixelVertices_*_*', 
        'keep *_hltRecoEcalSuperClusterActivityCandidateSC4_*_*', 
        'keep *_hltRecoEcalSuperClusterActivityCandidateSC5_*_*', 
        'keep *_hltRecoEcalSuperClusterActivityCandidate_*_*', 
        'keep *_hltRpcRecHits_*_*', 
        'keep *_hltSelector4CentralJetsL1FastJet_*_*', 
        'keep *_hltSelectorJets20L1FastJet_*_*', 
        'keep *_hltSiPixelCluster_*_*', 
        'keep *_hltSiPixelClusters_*_*', 
        'keep *_hltSiStripClusters_*_*', 
        'keep *_hltSiStripRawToClustersFacility_*_*', 
        'keep *_hltSingleMu15L3Filtered15_*_*', 
        'keep *_hltSingleMuIsoL1s14L3IsoFiltered15eta2p1_*_*', 
        'keep *_hltSingleMuIsoL3IsoFiltered15_*_*', 
        'keep *_hltTowerMakerForAll_*_*', 
        'keep *_hltTowerMakerForMuons_*_*', 
        'keep *_hltTriggerSummaryAOD_*_*', 
        'keep *_hltTriggerSummaryRAW_*_*', 
        'keep *_hltTrimmedPixelVertices_*_*', 
        'keep *_hltVerticesL3_*_*', 
        'keep DcsStatuss_hltScalersRawToDigi_*_*', 
        'keep FEDRawDataCollection_rawDataCollector_*_*', 
        'keep FEDRawDataCollection_rawDataRepacker_*_*', 
        'keep FEDRawDataCollection_source_*_*', 
        'keep FEDRawDataCollection_virginRawDataRepacker_*_*', 
        'keep L1GlobalTriggerReadoutRecord_hltGtDigis_*_*', 
        'keep L1MuGMTCands_hltGtDigis_*_*', 
        'keep L1MuGMTReadoutCollection_hltGtDigis_*_*', 
        'keep L2MuonTrajectorySeeds_hltL2MuonSeeds_*_*', 
        'keep L3MuonTrajectorySeeds_hltHIL3TrajSeedOIHit_*_*', 
        'keep L3MuonTrajectorySeeds_hltHIL3TrajectorySeed_*_*', 
        'keep L3MuonTrajectorySeeds_hltL3TrajSeedOIState_*_*', 
        'keep LumiScalerss_hltScalersRawToDigi_*_*', 
        'keep SiPixelClusteredmNewDetSetVector_hltSiPixelClusters_*_*', 
        'keep TrackCandidates_hltHIL3TrackCandidateFromL2OIHit_*_*', 
        'keep TrackCandidates_hltHIL3TrackCandidateFromL2OIState_*_*', 
        'keep TrackingRecHitsOwned_hltL3Muons_*_*', 
        'keep edmTriggerResults_*_*_*', 
        'keep recoCaloJets_*_*_*', 
        'keep recoCaloMETs_*_*_*', 
        'keep recoCaloMETs_hltMet_*_*', 
        'keep recoCompositeCandidates_*_*_*', 
        'keep recoElectrons_*_*_*', 
        'keep recoIsolatedPixelTrackCandidates_*_*_*', 
        'keep recoMETs_*_*_*', 
        'keep recoPFJets_*_*_*', 
        'keep recoPFTaus_*_*_*', 
        'keep recoRecoChargedCandidates_*_*_*', 
        'keep recoRecoChargedCandidates_hltHIL3MuonCandidates_*_*', 
        'keep recoRecoChargedCandidates_hltL2MuonCandidates_*_*', 
        'keep recoRecoEcalCandidatesToValuefloatAssociation_hltL1IsoHLTClusterShape_*_*', 
        'keep recoRecoEcalCandidatesToValuefloatAssociation_hltL1IsolatedPhotonEcalIsol_*_*', 
        'keep recoRecoEcalCandidatesToValuefloatAssociation_hltL1IsolatedPhotonHcalForHE_*_*', 
        'keep recoRecoEcalCandidatesToValuefloatAssociation_hltL1IsolatedPhotonHcalIsol_*_*', 
        'keep recoRecoEcalCandidatesToValuefloatAssociation_hltL1NonIsoHLTClusterShape_*_*', 
        'keep recoRecoEcalCandidatesToValuefloatAssociation_hltL1NonIsolatedPhotonEcalIsol_*_*', 
        'keep recoRecoEcalCandidatesToValuefloatAssociation_hltL1NonIsolatedPhotonHcalForHE_*_*', 
        'keep recoRecoEcalCandidatesToValuefloatAssociation_hltL1NonIsolatedPhotonHcalIsol_*_*', 
        'keep recoRecoEcalCandidates_*_*_*', 
        'keep recoRecoEcalCandidates_hltL1IsoRecoEcalCandidate_*_*', 
        'keep recoRecoEcalCandidates_hltL1NonIsoRecoEcalCandidate_*_*', 
        'keep recoTrackExtras_hltHIL3MuonsOIHit_*_*', 
        'keep recoTrackExtras_hltHIL3MuonsOIState_*_*', 
        'keep recoTracks_hltHIL3MuonsOIHit_*_*', 
        'keep recoTracks_hltHIL3MuonsOIState_*_*', 
        'keep recoTracks_hltHIL3Muons_*_*', 
        'keep recoTracks_hltHIL3TkTracksFromL2OIHit_*_*', 
        'keep recoTracks_hltHIL3TkTracksFromL2OIState_*_*', 
        'keep recoTracks_hltHIL3TkTracksFromL2_*_*', 
        'keep triggerTriggerEventWithRefs_*_*_*', 
        'keep triggerTriggerEvent_*_*_*', 
        'keep triggerTriggerFilterObjectWithRefs_*_*_*' ) )
)

process.RAWSIMEventContent = cms.PSet(
    splitLevel = cms.untracked.int32(0),
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
    outputCommands = cms.untracked.vstring('drop *', 
        'drop *', 
        'keep  FEDRawDataCollection_rawDataCollector_*_*', 
        'keep  FEDRawDataCollection_source_*_*', 
        'keep  FEDRawDataCollection_rawDataCollector_*_*', 
        'keep  FEDRawDataCollection_source_*_*', 
        'drop *_hlt*_*_*', 
        'keep *_hltL1GtObjectMap_*_*', 
        'keep FEDRawDataCollection_rawDataCollector_*_*', 
        'keep FEDRawDataCollection_source_*_*', 
        'keep edmTriggerResults_*_*_*', 
        'keep triggerTriggerEvent_*_*_*', 
        'keep *_g4SimHits_*_*', 
        'keep edmHepMCProduct_source_*_*', 
        'keep *_allTrackMCMatch_*_*', 
        'keep StripDigiSimLinkedmDetSetVector_simMuonCSCDigis_*_*', 
        'keep CSCDetIdCSCComparatorDigiMuonDigiCollection_simMuonCSCDigis_*_*', 
        'keep DTLayerIdDTDigiSimLinkMuonDigiCollection_simMuonDTDigis_*_*', 
        'keep RPCDigiSimLinkedmDetSetVector_simMuonRPCDigis_*_*', 
        'keep EBSrFlagsSorted_simEcalDigis_*_*', 
        'keep EESrFlagsSorted_simEcalDigis_*_*', 
        'keep CrossingFramePlaybackInfoExtended_*_*_*', 
        'keep PileupSummaryInfos_*_*_*', 
        'keep LHERunInfoProduct_*_*_*', 
        'keep LHEEventProduct_*_*_*', 
        'keep GenRunInfoProduct_generator_*_*', 
        'keep GenLumiInfoProduct_generator_*_*', 
        'keep GenEventInfoProduct_generator_*_*', 
        'keep edmHepMCProduct_generator_*_*', 
        'keep GenFilterInfo_*_*_*', 
        'keep *_genParticles_*_*', 
        'keep recoGenJets_*_*_*', 
        'keep *_genParticle_*_*', 
        'keep recoGenMETs_*_*_*', 
        'keep FEDRawDataCollection_source_*_*', 
        'keep FEDRawDataCollection_rawDataCollector_*_*', 
        'keep *_MEtoEDMConverter_*_*', 
        'keep *_randomEngineStateProducer_*_*', 
        'keep *_logErrorHarvester_*_*')
)

process.RAWSIMHLTEventContent = cms.PSet(
    splitLevel = cms.untracked.int32(0),
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
    outputCommands = cms.untracked.vstring( ('drop *', 
        'drop *', 
        'drop *', 
        'keep  FEDRawDataCollection_rawDataCollector_*_*', 
        'keep  FEDRawDataCollection_source_*_*', 
        'keep  FEDRawDataCollection_rawDataCollector_*_*', 
        'keep  FEDRawDataCollection_source_*_*', 
        'drop *_hlt*_*_*', 
        'keep *_hltL1GtObjectMap_*_*', 
        'keep FEDRawDataCollection_rawDataCollector_*_*', 
        'keep FEDRawDataCollection_source_*_*', 
        'keep edmTriggerResults_*_*_*', 
        'keep triggerTriggerEvent_*_*_*', 
        'keep *_g4SimHits_*_*', 
        'keep edmHepMCProduct_source_*_*', 
        'keep *_allTrackMCMatch_*_*', 
        'keep StripDigiSimLinkedmDetSetVector_simMuonCSCDigis_*_*', 
        'keep CSCDetIdCSCComparatorDigiMuonDigiCollection_simMuonCSCDigis_*_*', 
        'keep DTLayerIdDTDigiSimLinkMuonDigiCollection_simMuonDTDigis_*_*', 
        'keep RPCDigiSimLinkedmDetSetVector_simMuonRPCDigis_*_*', 
        'keep EBSrFlagsSorted_simEcalDigis_*_*', 
        'keep EESrFlagsSorted_simEcalDigis_*_*', 
        'keep CrossingFramePlaybackInfoExtended_*_*_*', 
        'keep PileupSummaryInfos_*_*_*', 
        'keep LHERunInfoProduct_*_*_*', 
        'keep LHEEventProduct_*_*_*', 
        'keep GenRunInfoProduct_generator_*_*', 
        'keep GenLumiInfoProduct_generator_*_*', 
        'keep GenEventInfoProduct_generator_*_*', 
        'keep edmHepMCProduct_generator_*_*', 
        'keep GenFilterInfo_*_*_*', 
        'keep *_genParticles_*_*', 
        'keep recoGenJets_*_*_*', 
        'keep *_genParticle_*_*', 
        'keep recoGenMETs_*_*_*', 
        'keep FEDRawDataCollection_source_*_*', 
        'keep FEDRawDataCollection_rawDataCollector_*_*', 
        'keep *_MEtoEDMConverter_*_*', 
        'keep *_randomEngineStateProducer_*_*', 
        'keep *_logErrorHarvester_*_*', 
        'drop *_hlt*_*_*', 
        'keep *_hltAK4CaloJetsCorrectedIDPassed_*_*', 
        'keep *_hltAK4CaloJetsIDPassed_*_*', 
        'keep *_hltAK4CaloJets_*_*', 
        'keep *_hltAK4PFJetsCorrected_*_*', 
        'keep *_hltAK4PFJetsForTaus_*_*', 
        'keep *_hltAK4PFJets_*_*', 
        'keep *_hltAlCaEtaEBUncalibrator_*_*', 
        'keep *_hltAlCaEtaEEUncalibrator_*_*', 
        'keep *_hltAlCaEtaRecHitsFilterEBonly_*_*', 
        'keep *_hltAlCaEtaRecHitsFilterEEonly_*_*', 
        'keep *_hltAlCaEtaRecHitsFilterEEonly_etaEcalRecHitsES_*', 
        'keep *_hltAlCaEtaRecHitsFilter_*_*', 
        'keep *_hltAlCaPhiSymStream_*_*', 
        'keep *_hltAlCaPhiSymUncalibrator_*_*', 
        'keep *_hltAlCaPi0EBUncalibrator_*_*', 
        'keep *_hltAlCaPi0EEUncalibrator_*_*', 
        'keep *_hltAlCaPi0RecHitsFilterEBonly_*_*', 
        'keep *_hltAlCaPi0RecHitsFilterEEonly_*_*', 
        'keep *_hltAlCaPi0RecHitsFilterEEonly_pi0EcalRecHitsES_*', 
        'keep *_hltAlCaPi0RecHitsFilter_*_*', 
        'keep *_hltBLifetimeL25AssociatorbbPhiL1FastJetFastPV_*_*', 
        'keep *_hltBLifetimeL25BJetTagsbbPhiL1FastJetFastPV_*_*', 
        'keep *_hltBLifetimeL25TagInfosbbPhiL1FastJetFastPV_*_*', 
        'keep *_hltBLifetimeL3AssociatorbbPhiL1FastJetFastPV_*_*', 
        'keep *_hltBLifetimeL3BJetTagsbbPhiL1FastJetFastPV_*_*', 
        'keep *_hltBLifetimeL3TagInfosbbPhiL1FastJetFastPV_*_*', 
        'keep *_hltBLifetimeRegionalCtfWithMaterialTracksbbPhiL1FastJetFastPV_*_*', 
        'keep *_hltBSoftMuonDiJet110Mu5L3FilterByDR_*_*', 
        'keep *_hltBSoftMuonDiJet110Mu5SelL3BJetTagsByDR_*_*', 
        'keep *_hltBSoftMuonDiJet20Mu5L3FilterByDR_*_*', 
        'keep *_hltBSoftMuonDiJet20Mu5SelL3BJetTagsByDR_*_*', 
        'keep *_hltBSoftMuonDiJet40Mu5L3FilterByDR_*_*', 
        'keep *_hltBSoftMuonDiJet40Mu5SelL3BJetTagsByDR_*_*', 
        'keep *_hltBSoftMuonDiJet70Mu5L3FilterByDR_*_*', 
        'keep *_hltBSoftMuonDiJet70Mu5SelL3BJetTagsByDR_*_*', 
        'keep *_hltBSoftMuonMu5L3_*_*', 
        'keep *_hltCSVJetTagSingleTopEle27_*_*', 
        'keep *_hltCSVJetTagSingleTopIsoMu24_*_*', 
        'keep *_hltCaloJetCorrectedRegional_*_*', 
        'keep *_hltCaloJetCorrected_*_*', 
        'keep *_hltCaloJetL1FastJetCorrected_*_*', 
        'keep *_hltCleanedCorrectedIslandBarrelSuperClustersHI_*_*', 
        'keep *_hltCleanedHiCorrectedIslandBarrelSuperClustersHI_*_*', 
        'keep *_hltConvPFTausTightIsoTrackFindingIsolation_*_*', 
        'keep *_hltConvPFTausTightIsoTrackFinding_*_*', 
        'keep *_hltConvPFTausTightIsoTrackPt5Isolation_*_*', 
        'keep *_hltConvPFTausTightIsoTrackPt5_*_*', 
        'keep *_hltConvPFTausTightIso_*_*', 
        'keep *_hltConvPFTausTrackFindingLooseIsolation_*_*', 
        'keep *_hltConvPFTausTrackFinding_*_*', 
        'keep *_hltConvPFTaus_*_*', 
        'keep *_hltCorrectedIslandBarrelSuperClustersHI_*_*', 
        'keep *_hltCorrectedIslandEndcapSuperClustersHI_*_*', 
        'keep *_hltCsc2DRecHits_*_*', 
        'keep *_hltCscSegments_*_*', 
        'keep *_hltDoublePFTau25TrackPt5MediumIsolationProng4L1HLTMatched_*_*', 
        'keep *_hltDoublePFTau25TrackPt5MediumIsolationProng4_*_*', 
        'keep *_hltDoublePFTau25TrackPt5MediumIsolation_*_*', 
        'keep *_hltDoublePFTau25TrackPt5_*_*', 
        'keep *_hltDoublePFTau25_*_*', 
        'keep *_hltDoublePFTauTightIso45Track5_*_*', 
        'keep *_hltDoublePFTauTightIso45Track_*_*', 
        'keep *_hltDt4DSegments_*_*', 
        'keep *_hltEcalRecHitAll_*_*', 
        'keep *_hltEle20CaloIdVTCaloIsoTTrkIdTTrkIsoTTrackIsoFilterL1SingleEG18orL1SingleEG20_*_*', 
        'keep *_hltEle20CaloIdVTTrkIdTDphiFilter_*_*', 
        'keep *_hltEle27WP85PixelMatchElectronsL1Seeded_*_*', 
        'keep *_hltFastPVPixelTracksMerger_*_*', 
        'keep *_hltFastPVPixelTracksRecover_*_*', 
        'keep *_hltFastPVPixelTracks_*_*', 
        'keep *_hltFastPVPixelVertices3D_*_*', 
        'keep *_hltFastPVPixelVertices_*_*', 
        'keep *_hltFastPixelBLifetimeL3TagInfos_*_*', 
        'keep *_hltFastPrimaryVertex_*_*', 
        'keep *_hltFilterDoubleIsoPFTau45Trk5LeadTrack5IsolationL1HLTMatched_*_*', 
        'keep *_hltFilterL2EtCutDoublePFIsoTau45Trk5_*_*', 
        'keep *_hltFilterL2EtCutSingleIsoPFTau35Trk20MET70_*_*', 
        'keep *_hltFilterSingleIsoPFTau35Trk20LeadTrackPt20_*_*', 
        'keep *_hltFilterSingleIsoPFTau35Trk20MET60LeadTrack20IsolationL1HLTMatched_*_*', 
        'keep *_hltFilterSingleIsoPFTau35Trk20MET70LeadTrack20IsolationL1HLTMatched_*_*', 
        'keep *_hltGoodOnlinePVs_*_*', 
        'keep *_hltHICaloJetCorrected_*_*', 
        'keep *_hltHICaloJetIDPassed_*_*', 
        'keep *_hltHIGoodLooseTracks_*_*', 
        'keep *_hltHIPixel3PrimTracks_*_*', 
        'keep *_hltHISelectedVertex_*_*', 
        'keep *_hltHISiPixelClusters_*_*', 
        'keep *_hltHITIPTCorrectorHB_*_*', 
        'keep *_hltHITIPTCorrectorHE_*_*', 
        'keep *_hltHiCorrectedIslandBarrelSuperClustersHI_*_*', 
        'keep *_hltHiCorrectedIslandEndcapSuperClustersHI_*_*', 
        'keep *_hltHiIslandSuperClustersHI_*_*', 
        'keep *_hltIsolPixelTrackProdHB_*_*', 
        'keep *_hltIsolPixelTrackProdHE_*_*', 
        'keep *_hltIter0PFlowTrackSelectionHighPurity_*_*', 
        'keep *_hltIter1Merged_*_*', 
        'keep *_hltIter1PFlowTrackSelectionHighPurity_*_*', 
        'keep *_hltIter2Merged_*_*', 
        'keep *_hltIter2PFlowTrackSelectionHighPurity_*_*', 
        'keep *_hltIter3Merged_*_*', 
        'keep *_hltIter4Merged_*_*', 
        'keep *_hltIterativeCone5PileupSubtractionCaloJets_*_*', 
        'keep *_hltL1GtObjectMap_*_*', 
        'keep *_hltL1HLTSingleIsoPFTau35Trk20Met60JetsMatch_*_*', 
        'keep *_hltL1IsoElectronTrackIsol_*_*', 
        'keep *_hltL1NonIsoElectronTrackIsol_*_*', 
        'keep *_hltL1SeededRecoEcalCandidate_*_*', 
        'keep *_hltL1extraParticlesCentral_*_*', 
        'keep *_hltL1extraParticlesNonIsolated_*_*', 
        'keep *_hltL1extraParticlesTau_*_*', 
        'keep *_hltL1extraParticles_*_*', 
        'keep *_hltL1sDoubleTauJet44Eta2p17orDoubleJet64Central_*_*', 
        'keep *_hltL1sDoubleTauJet44erorDoubleJetC64_*_*', 
        'keep *_hltL1sL1EG18er_*_*', 
        'keep *_hltL1sL1ETM36ORETM40_*_*', 
        'keep *_hltL1sL1Jet52ETM30_*_*', 
        'keep *_hltL1sL1SingleEG12_*_*', 
        'keep *_hltL1sL1SingleEG15_*_*', 
        'keep *_hltL1sL1SingleEG18orL1SingleEG20_*_*', 
        'keep *_hltL1sL1SingleMu10_*_*', 
        'keep *_hltL1sL1SingleMu14Eta2p1_*_*', 
        'keep *_hltL1sMu16Eta2p1_*_*', 
        'keep *_hltL2MuonCandidatesNoVtx_*_*', 
        'keep *_hltL2MuonCandidates_*_*', 
        'keep *_hltL2MuonSeeds_*_*', 
        'keep *_hltL2Muons_*_*', 
        'keep *_hltL2TauJets_*_*', 
        'keep *_hltL3CombinedSecondaryVertexBJetTags_*_*', 
        'keep *_hltL3MuonCandidates_*_*', 
        'keep *_hltL3MuonsIOHit_*_*', 
        'keep *_hltL3MuonsLinksCombination_*_*', 
        'keep *_hltL3MuonsOIHit_*_*', 
        'keep *_hltL3MuonsOIState_*_*', 
        'keep *_hltL3Muons_*_*', 
        'keep *_hltL3NoFiltersNoVtxMuonCandidates_*_*', 
        'keep *_hltL3NoFiltersNoVtxMuons_*_*', 
        'keep *_hltL3SecondaryVertexTagInfos_*_*', 
        'keep *_hltL3TkFromL2OICombination_*_*', 
        'keep *_hltL3TkTracksFromL2IOHit_*_*', 
        'keep *_hltL3TkTracksFromL2OIHit_*_*', 
        'keep *_hltL3TkTracksFromL2OIState_*_*', 
        'keep *_hltL3TkTracksFromL2_*_*', 
        'keep *_hltL3TrackCandidateFromL2IOHit_*_*', 
        'keep *_hltL3TrackCandidateFromL2OIHit_*_*', 
        'keep *_hltL3TrackCandidateFromL2OIState_*_*', 
        'keep *_hltL3TrajSeedIOHit_*_*', 
        'keep *_hltL3TrajSeedOIHit_*_*', 
        'keep *_hltL3TrajSeedOIState_*_*', 
        'keep *_hltL3TrajectorySeed_*_*', 
        'keep *_hltL3crIsoL1sMu16Eta2p1L1f0L2f16QL3f18QL3crIsoRhoFiltered0p15_*_*', 
        'keep *_hltLeadingCentralJets30SingleTopEle27_*_*', 
        'keep *_hltLeadingCentralJets30SingleTopIsoMu24_*_*', 
        'keep *_hltMetForHf_*_*', 
        'keep *_hltMet_*_*', 
        'keep *_hltMu8Ele17CaloIdTCaloIsoVLPixelMatchFilter_*_*', 
        'keep *_hltMuTrackJpsiCtfTrackCands_*_*', 
        'keep *_hltMuTrackJpsiPixelTrackCands_*_*', 
        'keep *_hltMuonCSCDigis_*_*', 
        'keep *_hltMuonCSCDigis_MuonCSCStripDigi_*', 
        'keep *_hltMuonCSCDigis_MuonCSCWireDigi_*', 
        'keep *_hltMuonDTDigis_*_*', 
        'keep *_hltMuonRPCDigis_*_*', 
        'keep *_hltOnlineBeamSpot_*_*', 
        'keep *_hltOnlinePrimaryVertices_*_*', 
        'keep *_hltOverlapFilterEle20LooseIsoPFTau20OldVersion_*_*', 
        'keep *_hltOverlapFilterIsoEle20MediumIsoPFTau20_*_*', 
        'keep *_hltOverlapFilterIsoMu15IsoPFTau20_*_*', 
        'keep *_hltOverlapFilterIsoMu15MediumIsoPFTau20_*_*', 
        'keep *_hltOverlapFilterIsoMu15TightIsoPFTau20_*_*', 
        'keep *_hltOverlapFilterIsoMu18LooseIsoPFTau20_*_*', 
        'keep *_hltOverlapFilterIsoMu18PFTau25TrackPt5Prong4_*_*', 
        'keep *_hltPFTau15TrackLooseIso_*_*', 
        'keep *_hltPFTau15Track_*_*', 
        'keep *_hltPFTau15_*_*', 
        'keep *_hltPFTau20IsoMuVertex_*_*', 
        'keep *_hltPFTau20TrackLooseIso_*_*', 
        'keep *_hltPFTau20Track_*_*', 
        'keep *_hltPFTau20_*_*', 
        'keep *_hltPFTau25TrackPt5MediumIsolationProng4IsoMuVertex_*_*', 
        'keep *_hltPFTau25TrackPt5MediumIsolationProng4_*_*', 
        'keep *_hltPFTau25TrackPt5MediumIsolation_*_*', 
        'keep *_hltPFTau25TrackPt5_*_*', 
        'keep *_hltPFTau25_*_*', 
        'keep *_hltPFTau35TrackPt20LooseIsoProng2_*_*', 
        'keep *_hltPFTau35TrackPt20LooseIso_*_*', 
        'keep *_hltPFTau35TrackPt20_*_*', 
        'keep *_hltPFTau35Track_*_*', 
        'keep *_hltPFTau35_*_*', 
        'keep *_hltPFTauEleVertex20_*_*', 
        'keep *_hltPFTauJetTracksAssociator_*_*', 
        'keep *_hltPFTauMediumIso20TrackMediumIso_*_*', 
        'keep *_hltPFTauMediumIso20Track_*_*', 
        'keep *_hltPFTauMediumIso20_*_*', 
        'keep *_hltPFTauMediumIso35Track_*_*', 
        'keep *_hltPFTauMediumIso35_*_*', 
        'keep *_hltPFTauTagInfo_*_*', 
        'keep *_hltPFTauTightIso20TrackTightIso_*_*', 
        'keep *_hltPFTauTightIso20Track_*_*', 
        'keep *_hltPFTauTightIso20_*_*', 
        'keep *_hltPFlowTrackSelectionHighPurity_*_*', 
        'keep *_hltParticleFlowForTaus_*_*', 
        'keep *_hltParticleFlow_*_*', 
        'keep *_hltPixelMatch3HitElectronsActivity_*_*', 
        'keep *_hltPixelMatch3HitElectronsL1Seeded_*_*', 
        'keep *_hltPixelMatchCleanElectronsL1Seeded_*_*', 
        'keep *_hltPixelMatchElectronsActivity_*_*', 
        'keep *_hltPixelMatchElectronsL1Iso_*_*', 
        'keep *_hltPixelMatchElectronsL1NonIso_*_*', 
        'keep *_hltPixelMatchElectronsL1Seeded_*_*', 
        'keep *_hltPixelTracks_*_*', 
        'keep *_hltPixelVertices3DbbPhi_*_*', 
        'keep *_hltPixelVertices_*_*', 
        'keep *_hltRecoEcalSuperClusterActivityCandidateSC4_*_*', 
        'keep *_hltRecoEcalSuperClusterActivityCandidateSC5_*_*', 
        'keep *_hltRecoEcalSuperClusterActivityCandidate_*_*', 
        'keep *_hltRpcRecHits_*_*', 
        'keep *_hltSelector4CentralJetsL1FastJet_*_*', 
        'keep *_hltSelectorJets20L1FastJet_*_*', 
        'keep *_hltSiPixelCluster_*_*', 
        'keep *_hltSiPixelClusters_*_*', 
        'keep *_hltSiStripClusters_*_*', 
        'keep *_hltSiStripRawToClustersFacility_*_*', 
        'keep *_hltSingleMu15L3Filtered15_*_*', 
        'keep *_hltSingleMuIsoL1s14L3IsoFiltered15eta2p1_*_*', 
        'keep *_hltSingleMuIsoL3IsoFiltered15_*_*', 
        'keep *_hltTowerMakerForAll_*_*', 
        'keep *_hltTowerMakerForMuons_*_*', 
        'keep *_hltTriggerSummaryAOD_*_*', 
        'keep *_hltTriggerSummaryRAW_*_*', 
        'keep *_hltTrimmedPixelVertices_*_*', 
        'keep *_hltVerticesL3_*_*', 
        'keep DcsStatuss_hltScalersRawToDigi_*_*', 
        'keep FEDRawDataCollection_rawDataCollector_*_*', 
        'keep FEDRawDataCollection_rawDataRepacker_*_*', 
        'keep FEDRawDataCollection_source_*_*', 
        'keep FEDRawDataCollection_virginRawDataRepacker_*_*', 
        'keep L1GlobalTriggerReadoutRecord_hltGtDigis_*_*', 
        'keep L1MuGMTCands_hltGtDigis_*_*', 
        'keep L1MuGMTReadoutCollection_hltGtDigis_*_*', 
        'keep L2MuonTrajectorySeeds_hltL2MuonSeeds_*_*', 
        'keep L3MuonTrajectorySeeds_hltHIL3TrajSeedOIHit_*_*', 
        'keep L3MuonTrajectorySeeds_hltHIL3TrajectorySeed_*_*', 
        'keep L3MuonTrajectorySeeds_hltL3TrajSeedOIState_*_*', 
        'keep LumiScalerss_hltScalersRawToDigi_*_*', 
        'keep SiPixelClusteredmNewDetSetVector_hltSiPixelClusters_*_*', 
        'keep TrackCandidates_hltHIL3TrackCandidateFromL2OIHit_*_*', 
        'keep TrackCandidates_hltHIL3TrackCandidateFromL2OIState_*_*', 
        'keep TrackingRecHitsOwned_hltL3Muons_*_*', 
        'keep edmTriggerResults_*_*_*', 
        'keep recoCaloJets_*_*_*', 
        'keep recoCaloMETs_*_*_*', 
        'keep recoCaloMETs_hltMet_*_*', 
        'keep recoCompositeCandidates_*_*_*', 
        'keep recoElectrons_*_*_*', 
        'keep recoIsolatedPixelTrackCandidates_*_*_*', 
        'keep recoMETs_*_*_*', 
        'keep recoPFJets_*_*_*', 
        'keep recoPFTaus_*_*_*', 
        'keep recoRecoChargedCandidates_*_*_*', 
        'keep recoRecoChargedCandidates_hltHIL3MuonCandidates_*_*', 
        'keep recoRecoChargedCandidates_hltL2MuonCandidates_*_*', 
        'keep recoRecoEcalCandidatesToValuefloatAssociation_hltL1IsoHLTClusterShape_*_*', 
        'keep recoRecoEcalCandidatesToValuefloatAssociation_hltL1IsolatedPhotonEcalIsol_*_*', 
        'keep recoRecoEcalCandidatesToValuefloatAssociation_hltL1IsolatedPhotonHcalForHE_*_*', 
        'keep recoRecoEcalCandidatesToValuefloatAssociation_hltL1IsolatedPhotonHcalIsol_*_*', 
        'keep recoRecoEcalCandidatesToValuefloatAssociation_hltL1NonIsoHLTClusterShape_*_*', 
        'keep recoRecoEcalCandidatesToValuefloatAssociation_hltL1NonIsolatedPhotonEcalIsol_*_*', 
        'keep recoRecoEcalCandidatesToValuefloatAssociation_hltL1NonIsolatedPhotonHcalForHE_*_*', 
        'keep recoRecoEcalCandidatesToValuefloatAssociation_hltL1NonIsolatedPhotonHcalIsol_*_*', 
        'keep recoRecoEcalCandidates_*_*_*', 
        'keep recoRecoEcalCandidates_hltL1IsoRecoEcalCandidate_*_*', 
        'keep recoRecoEcalCandidates_hltL1NonIsoRecoEcalCandidate_*_*', 
        'keep recoTrackExtras_hltHIL3MuonsOIHit_*_*', 
        'keep recoTrackExtras_hltHIL3MuonsOIState_*_*', 
        'keep recoTracks_hltHIL3MuonsOIHit_*_*', 
        'keep recoTracks_hltHIL3MuonsOIState_*_*', 
        'keep recoTracks_hltHIL3Muons_*_*', 
        'keep recoTracks_hltHIL3TkTracksFromL2OIHit_*_*', 
        'keep recoTracks_hltHIL3TkTracksFromL2OIState_*_*', 
        'keep recoTracks_hltHIL3TkTracksFromL2_*_*', 
        'keep triggerTriggerEventWithRefs_*_*_*', 
        'keep triggerTriggerEvent_*_*_*', 
        'keep triggerTriggerFilterObjectWithRefs_*_*_*' ) )
)

process.RECODEBUGEventContent = cms.PSet(
    splitLevel = cms.untracked.int32(0),
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
    outputCommands = cms.untracked.vstring( ('drop *', 
        'drop *', 
        'drop *', 
        'keep DetIdedmEDCollection_siStripDigis_*_*', 
        'keep DetIdedmEDCollection_siPixelDigis_*_*', 
        'keep *_siPixelClusters_*_*', 
        'keep *_siStripClusters_*_*', 
        'keep *_clusterSummaryProducer_*_*', 
        'keep *_dt1DRecHits_*_*', 
        'keep *_dt4DSegments_*_*', 
        'keep *_dt1DCosmicRecHits_*_*', 
        'keep *_dt4DCosmicSegments_*_*', 
        'keep *_csc2DRecHits_*_*', 
        'keep *_cscSegments_*_*', 
        'keep *_rpcRecHits_*_*', 
        'keep *_hbhereco_*_*', 
        'keep *_hbheprereco_*_*', 
        'keep *_hfreco_*_*', 
        'keep *_horeco_*_*', 
        'keep HBHERecHitsSorted_hbherecoMB_*_*', 
        'keep HORecHitsSorted_horecoMB_*_*', 
        'keep HFRecHitsSorted_hfrecoMB_*_*', 
        'keep ZDCDataFramesSorted_*Digis_*_*', 
        'keep ZDCRecHitsSorted_*_*_*', 
        'keep *_reducedHcalRecHits_*_*', 
        'keep *_castorreco_*_*', 
        'keep HcalUnpackerReport_*_*_*', 
        'keep *_ecalPreshowerRecHit_*_*', 
        'keep *_ecalRecHit_*_*', 
        'keep *_ecalCompactTrigPrim_*_*', 
        'keep *_ecalTPSkim_*_*', 
        'keep *_selectDigi_*_*', 
        'keep EcalRecHitsSorted_reducedEcalRecHitsEE_*_*', 
        'keep EcalRecHitsSorted_reducedEcalRecHitsEB_*_*', 
        'keep EcalRecHitsSorted_reducedEcalRecHitsES_*_*', 
        'keep *_hybridSuperClusters_*_*', 
        'keep recoSuperClusters_correctedHybridSuperClusters_*_*', 
        'keep *_multi5x5SuperClusters_*_*', 
        'keep recoSuperClusters_multi5x5SuperClusters_*_*', 
        'keep recoSuperClusters_multi5x5SuperClustersWithPreshower_*_*', 
        'keep recoSuperClusters_correctedMulti5x5SuperClustersWithPreshower_*_*', 
        'keep recoPreshowerClusters_multi5x5SuperClustersWithPreshower_*_*', 
        'keep recoPreshowerClusterShapes_multi5x5PreshowerClusterShape_*_*', 
        'keep *_particleFlowSuperClusterECAL_*_*', 
        'drop recoClusterShapes_*_*_*', 
        'drop recoBasicClustersToOnerecoClusterShapesAssociation_*_*_*', 
        'drop recoBasicClusters_multi5x5BasicClusters_multi5x5BarrelBasicClusters_*', 
        'drop recoSuperClusters_multi5x5SuperClusters_multi5x5BarrelSuperClusters_*', 
        'keep *_CkfElectronCandidates_*_*', 
        'keep *_GsfGlobalElectronTest_*_*', 
        'keep *_electronMergedSeeds_*_*', 
        'keep recoGsfTracks_electronGsfTracks_*_*', 
        'keep recoGsfTrackExtras_electronGsfTracks_*_*', 
        'keep recoTrackExtras_electronGsfTracks_*_*', 
        'keep TrackingRecHitsOwned_electronGsfTracks_*_*', 
        'keep recoTracks_generalTracks_*_*', 
        'keep recoTrackExtras_generalTracks_*_*', 
        'keep TrackingRecHitsOwned_extraFromSeeds_*_*', 
        'keep uints_extraFromSeeds_*_*', 
        'keep TrackingRecHitsOwned_generalTracks_*_*', 
        'keep recoTracks_beamhaloTracks_*_*', 
        'keep recoTrackExtras_beamhaloTracks_*_*', 
        'keep TrackingRecHitsOwned_beamhaloTracks_*_*', 
        'keep recoTracks_regionalCosmicTracks_*_*', 
        'keep recoTrackExtras_regionalCosmicTracks_*_*', 
        'keep TrackingRecHitsOwned_regionalCosmicTracks_*_*', 
        'keep recoTracks_rsWithMaterialTracks_*_*', 
        'keep recoTrackExtras_rsWithMaterialTracks_*_*', 
        'keep TrackingRecHitsOwned_rsWithMaterialTracks_*_*', 
        'keep recoTracks_conversionStepTracks_*_*', 
        'keep recoTrackExtras_conversionStepTracks_*_*', 
        'keep TrackingRecHitsOwned_conversionStepTracks_*_*', 
        'keep *_ctfPixelLess_*_*', 
        'keep *_dedxTruncated40_*_*', 
        'keep *_dedxDiscrimASmi_*_*', 
        'keep *_dedxHarmonic2_*_*', 
        'keep *_trackExtrapolator_*_*', 
        'keep floatedmValueMap_generalTracks_*_*', 
        'keep *_kt6CaloJets_*_*', 
        'keep *_ak4CaloJets_*_*', 
        'keep *_ak5CaloJets_*_*', 
        'keep double_kt6PFJets_rho_*', 
        'keep *_ak4PFJets_*_*', 
        'keep *_ak5PFJets_*_*', 
        'keep *_ak8PFJets_*_*', 
        'keep *_ak4PFJetsCHS_*_*', 
        'keep *_ak5PFJetsCHS_*_*', 
        'keep *_ak8PFJetsCHS_*_*', 
        'keep *_ca8PFJetsCHS_*_*', 
        'keep *_ca8PFJetsCHSPruned_*_*', 
        'keep *_cmsTopTagPFJetsCHS_*_*', 
        'keep *_JetPlusTrackZSPCorJetAntiKt4_*_*', 
        'keep *_ak4TrackJets_*_*', 
        'keep recoRecoChargedRefCandidates_trackRefsForJets_*_*', 
        'keep *_caloTowers_*_*', 
        'keep *_towerMaker_*_*', 
        'keep *_CastorTowerReco_*_*', 
        'keep *_kt4JetTracksAssociatorAtVertex_*_*', 
        'keep *_kt4JetTracksAssociatorAtCaloFace_*_*', 
        'keep *_kt4JetExtender_*_*', 
        'keep *_ak4JetTracksAssociatorAtVertex_*_*', 
        'keep *_ak4JetTracksAssociatorAtVertexPF_*_*', 
        'keep *_ak4JetTracksAssociatorAtCaloFace_*_*', 
        'keep *_ak4JetTracksAssociatorExplicit_*_*', 
        'keep *_ak4JetExtender_*_*', 
        'keep *_ak7JetTracksAssociatorAtVertex*_*_*', 
        'keep *_ak7JetTracksAssociatorAtCaloFace*_*_*', 
        'keep *_ak7JetExtender_*_*', 
        'keep *_ak4JetID_*_*', 
        'keep *_ak5JetID_*_*', 
        'keep *_ak7JetID_*_*', 
        'keep *_ic5JetID_*_*', 
        'keep *_kt4JetID_*_*', 
        'keep *_kt6JetID_*_*', 
        'keep *_ak7BasicJets_*_*', 
        'keep *_ak7CastorJetID_*_*', 
        'keep double_kt6CaloJetsCentral_*_*', 
        'keep double_kt6PFJetsCentralChargedPileUp_*_*', 
        'keep double_kt6PFJetsCentralNeutral_*_*', 
        'keep double_kt6PFJetsCentralNeutralTight_*_*', 
        'keep *_fixedGridRho*_*_*', 
        'keep *_ca8PFJetsCHSPrunedLinks_*_*', 
        'keep recoCaloMETs_caloMet_*_*', 
        'keep recoCaloMETs_caloMetBE_*_*', 
        'keep recoCaloMETs_caloMetBEFO_*_*', 
        'keep recoCaloMETs_caloMetM_*_*', 
        'keep recoPFMETs_pfMet_*_*', 
        'keep recoPFMETs_pfChMet_*_*', 
        'keep recoMuonMETCorrectionDataedmValueMap_muonMETValueMapProducer_*_*', 
        'keep recoHcalNoiseRBXs_hcalnoise_*_*', 
        'keep HcalNoiseSummary_hcalnoise_*_*', 
        'keep *HaloData_*_*_*', 
        'keep *BeamHaloSummary_BeamHaloSummary_*_*', 
        'keep *_MuonSeed_*_*', 
        'keep *_ancientMuonSeed_*_*', 
        'keep *_displacedMuonSeeds_*_*', 
        'keep *_mergedStandAloneMuonSeeds_*_*', 
        'keep TrackingRecHitsOwned_globalMuons_*_*', 
        'keep TrackingRecHitsOwned_tevMuons_*_*', 
        'keep recoCaloMuons_calomuons_*_*', 
        'keep *_CosmicMuonSeed_*_*', 
        'keep recoTrackExtras_cosmicMuons_*_*', 
        'keep TrackingRecHitsOwned_cosmicMuons_*_*', 
        'keep recoTrackExtras_globalCosmicMuons_*_*', 
        'keep TrackingRecHitsOwned_globalCosmicMuons_*_*', 
        'keep recoTrackExtras_cosmicMuons1Leg_*_*', 
        'keep TrackingRecHitsOwned_cosmicMuons1Leg_*_*', 
        'keep recoTrackExtras_globalCosmicMuons1Leg_*_*', 
        'keep TrackingRecHitsOwned_globalCosmicMuons1Leg_*_*', 
        'keep recoTracks_cosmicsVetoTracks_*_*', 
        'keep *_SETMuonSeed_*_*', 
        'keep recoTracks_standAloneSETMuons_*_*', 
        'keep recoTrackExtras_standAloneSETMuons_*_*', 
        'keep TrackingRecHitsOwned_standAloneSETMuons_*_*', 
        'keep recoTracks_globalSETMuons_*_*', 
        'keep recoTrackExtras_globalSETMuons_*_*', 
        'keep TrackingRecHitsOwned_globalSETMuons_*_*', 
        'keep recoMuons_muonsWithSET_*_*', 
        'keep *_muons_*_*', 
        'keep *_*_muons_*', 
        'drop *_muons_muons1stStep2muonsMap_*', 
        'drop recoIsoDepositedmValueMap_muons_*_*', 
        'drop doubleedmValueMap_muons_muPFIso*_*', 
        'keep recoTracks_standAloneMuons_*_*', 
        'keep recoTrackExtras_standAloneMuons_*_*', 
        'keep TrackingRecHitsOwned_standAloneMuons_*_*', 
        'keep recoTracks_globalMuons_*_*', 
        'keep recoTrackExtras_globalMuons_*_*', 
        'keep recoTracks_tevMuons_*_*', 
        'keep recoTrackExtras_tevMuons_*_*', 
        'keep recoTracks_generalTracks_*_*', 
        'keep recoTracksToOnerecoTracksAssociation_tevMuons_*_*', 
        'keep recoTracks_cosmicMuons_*_*', 
        'keep recoTracks_globalCosmicMuons_*_*', 
        'keep recoMuons_muonsFromCosmics_*_*', 
        'keep recoTracks_cosmicMuons1Leg_*_*', 
        'keep recoTracks_globalCosmicMuons1Leg_*_*', 
        'keep recoMuons_muonsFromCosmics1Leg_*_*', 
        'keep recoTracks_refittedStandAloneMuons_*_*', 
        'keep recoTrackExtras_refittedStandAloneMuons_*_*', 
        'keep TrackingRecHitsOwned_refittedStandAloneMuons_*_*', 
        'keep recoTracks_displacedStandAloneMuons__*', 
        'keep recoTrackExtras_displacedStandAloneMuons_*_*', 
        'keep TrackingRecHitsOwned_displacedStandAloneMuons_*_*', 
        'keep *_muIsoDepositTk_*_*', 
        'keep *_muIsoDepositCalByAssociatorTowers_*_*', 
        'keep *_muIsoDepositCalByAssociatorHits_*_*', 
        'keep *_muIsoDepositJets_*_*', 
        'keep *_muGlobalIsoDepositCtfTk_*_*', 
        'keep *_muGlobalIsoDepositCalByAssociatorTowers_*_*', 
        'keep *_muGlobalIsoDepositCalByAssociatorHits_*_*', 
        'keep *_muGlobalIsoDepositJets_*_*', 
        'keep *_impactParameterTagInfos_*_*', 
        'keep *_trackCountingHighEffBJetTags_*_*', 
        'keep *_trackCountingHighPurBJetTags_*_*', 
        'keep *_jetProbabilityBJetTags_*_*', 
        'keep *_jetBProbabilityBJetTags_*_*', 
        'keep *_secondaryVertexTagInfos_*_*', 
        'keep *_inclusiveSecondaryVertexFinderTagInfos_*_*', 
        'keep *_ghostTrackVertexTagInfos_*_*', 
        'keep *_simpleSecondaryVertexBJetTags_*_*', 
        'keep *_simpleSecondaryVertexHighEffBJetTags_*_*', 
        'keep *_simpleSecondaryVertexHighPurBJetTags_*_*', 
        'keep *_combinedSecondaryVertexBJetTags_*_*', 
        'keep *_combinedInclusiveSecondaryVertexV2BJetTags_*_*', 
        'keep *_ghostTrackBJetTags_*_*', 
        'keep *_softPFMuonsTagInfos_*_*', 
        'keep *_softPFElectronsTagInfos_*_*', 
        'keep *_softPFElectronBJetTags_*_*', 
        'keep *_softPFMuonBJetTags_*_*', 
        'keep *_softMuonTagInfos_*_*', 
        'keep *_softMuonBJetTags_*_*', 
        'keep *_softMuonByIP3dBJetTags_*_*', 
        'keep *_softMuonByPtBJetTags_*_*', 
        'keep *_combinedMVABJetTags_*_*', 
        'keep *_pfImpactParameterTagInfos_*_*', 
        'keep *_pfSecondaryVertexTagInfos_*_*', 
        'keep *_pfCombinedSecondaryVertexBJetTags_*_*', 
        'keep *_ak4PFJetsRecoTauPiZeros_*_*', 
        'keep *_hpsPFTauProducer_*_*', 
        'keep *_hpsPFTauDiscrimination*_*_*', 
        'keep *_hpsPFTau*PtSum_*_*', 
        'keep *_hpsPFTauTransverseImpactParameters_*_*', 
        'keep  *_offlinePrimaryVertices__*', 
        'keep  *_offlinePrimaryVerticesWithBS_*_*', 
        'keep  *_offlinePrimaryVerticesFromCosmicTracks_*_*', 
        'keep  *_nuclearInteractionMaker_*_*', 
        'keep *_generalV0Candidates_*_*', 
        'keep *_inclusiveSecondaryVertices_*_*', 
        'keep recoGsfElectronCores_gsfElectronCores_*_*', 
        'keep recoGsfElectrons_gsfElectrons_*_*', 
        'keep recoGsfElectronCores_uncleanedOnlyGsfElectronCores_*_*', 
        'keep recoGsfElectrons_uncleanedOnlyGsfElectrons_*_*', 
        'keep floatedmValueMap_eidRobustLoose_*_*', 
        'keep floatedmValueMap_eidRobustTight_*_*', 
        'keep floatedmValueMap_eidRobustHighEnergy_*_*', 
        'keep floatedmValueMap_eidLoose_*_*', 
        'keep floatedmValueMap_eidTight_*_*', 
        'keep *_egmGedGsfElectronPFIsolation_*_*', 
        'keep *_egmGsfElectronIDs_*_*', 
        'keep *_gedPhotonCore_*_*', 
        'keep *_gedPhotons_*_*', 
        'keep *_particleBasedIsolation_*_*', 
        'keep recoPhotons_mustachePhotons_*_*', 
        'keep recoPhotonCores_mustachePhotonCore_*_*', 
        'keep recoPhotons_photons_*_*', 
        'keep recoPhotonCores_photonCore_*_*', 
        'keep recoConversions_conversions_*_*', 
        'keep recoConversions_mustacheConversions_*_*', 
        'drop *_conversions_uncleanedConversions_*', 
        'drop *_gedPhotonsTmp_valMapPFEgammaCandToPhoton_*', 
        'keep recoConversions_allConversions_*_*', 
        'keep recoConversions_allConversionsOldEG_*_*', 
        'keep recoTracks_ckfOutInTracksFrom*Conversions_*_*', 
        'keep recoTracks_ckfInOutTracksFrom*Conversions_*_*', 
        'keep recoTrackExtras_ckfOutInTracksFrom*Conversions_*_*', 
        'keep recoTrackExtras_ckfInOutTracksFrom*Conversions_*_*', 
        'keep TrackingRecHitsOwned_ckfOutInTracksFrom*Conversions_*_*', 
        'keep TrackingRecHitsOwned_ckfInOutTracksFrom*Conversions_*_*', 
        'keep recoConversions_uncleanedOnlyAllConversions_*_*', 
        'keep recoTracks_uncleanedOnlyCkfOutInTracksFromConversions_*_*', 
        'keep recoTracks_uncleanedOnlyCkfInOutTracksFromConversions_*_*', 
        'keep recoTrackExtras_uncleanedOnlyCkfOutInTracksFromConversions_*_*', 
        'keep recoTrackExtras_uncleanedOnlyCkfInOutTracksFromConversions_*_*', 
        'keep TrackingRecHitsOwned_uncleanedOnlyCkfOutInTracksFromConversions_*_*', 
        'keep TrackingRecHitsOwned_uncleanedOnlyCkfInOutTracksFromConversions_*_*', 
        'keep *_PhotonIDProd_*_*', 
        'keep *_PhotonIDProdGED_*_*', 
        'keep *_hfRecoEcalCandidate_*_*', 
        'keep *_hfEMClusters_*_*', 
        'keep *_gedGsfElectronCores_*_*', 
        'keep *_gedGsfElectrons_*_*', 
        'keep *_pixelTracks_*_*', 
        'keep *_pixelVertices_*_*', 
        'drop CaloTowersSorted_towerMakerPF_*_*', 
        'keep recoPFRecHits_particleFlowClusterECAL_Cleaned_*', 
        'keep recoPFRecHits_particleFlowClusterHCAL_Cleaned_*', 
        'keep recoPFRecHits_particleFlowClusterHO_Cleaned_*', 
        'keep recoPFRecHits_particleFlowClusterHFEM_Cleaned_*', 
        'keep recoPFRecHits_particleFlowClusterHFHAD_Cleaned_*', 
        'keep recoPFRecHits_particleFlowClusterPS_Cleaned_*', 
        'keep recoPFRecHits_particleFlowRecHitECAL_Cleaned_*', 
        'keep recoPFRecHits_particleFlowRecHitHCAL_Cleaned_*', 
        'keep recoPFRecHits_particleFlowRecHitHO_Cleaned_*', 
        'keep recoPFRecHits_particleFlowRecHitPS_Cleaned_*', 
        'keep recoPFClusters_particleFlowClusterECAL_*_*', 
        'keep recoPFClusters_particleFlowClusterHCAL_*_*', 
        'keep recoPFClusters_particleFlowClusterHO_*_*', 
        'keep recoPFClusters_particleFlowClusterPS_*_*', 
        'keep recoPFBlocks_particleFlowBlock_*_*', 
        'keep recoPFCandidates_particleFlowEGamma_*_*', 
        'keep recoCaloClusters_particleFlowEGamma_*_*', 
        'keep recoSuperClusters_particleFlowEGamma_*_*', 
        'keep recoConversions_particleFlowEGamma_*_*', 
        'keep recoPFCandidates_particleFlow_*_*', 
        'keep recoPFCandidates_particleFlowTmp_electrons_*', 
        'keep recoPFCandidates_particleFlowTmp_*_*', 
        'drop recoPFCandidates_particleFlowTmp__*', 
        'keep recoPFDisplacedVertexs_particleFlowDisplacedVertex_*_*', 
        'keep *_pfElectronTranslator_*_*', 
        'keep *_pfPhotonTranslator_*_*', 
        'keep *_particleFlow_electrons_*', 
        'keep *_particleFlow_photons_*', 
        'keep *_trackerDrivenElectronSeeds_preid_*', 
        'keep *_particleFlowPtrs_*_*', 
        'keep *_particleFlowTmpPtrs_*_*', 
        'keep *_offlineBeamSpot_*_*', 
        'keep L1GlobalTriggerReadoutRecord_gtDigis_*_*', 
        'keep *_l1GtRecord_*_*', 
        'keep *_l1GtTriggerMenuLite_*_*', 
        'keep *_conditionsInEdm_*_*', 
        'keep *_l1extraParticles_*_*', 
        'keep *_l1L1GtObjectMap_*_*', 
        'keep L1MuGMTReadoutCollection_gtDigis_*_*', 
        'keep L1GctEmCand*_gctDigis_*_*', 
        'keep L1GctJetCand*_gctDigis_*_*', 
        'keep L1GctEtHad*_gctDigis_*_*', 
        'keep L1GctEtMiss*_gctDigis_*_*', 
        'keep L1GctEtTotal*_gctDigis_*_*', 
        'keep L1GctHtMiss*_gctDigis_*_*', 
        'keep L1GctJetCounts*_gctDigis_*_*', 
        'keep L1GctHFRingEtSums*_gctDigis_*_*', 
        'keep L1GctHFBitCounts*_gctDigis_*_*', 
        'keep LumiDetails_lumiProducer_*_*', 
        'keep LumiSummary_lumiProducer_*_*', 
        'drop *_hlt*_*_*', 
        'keep *_hltL1GtObjectMap_*_*', 
        'keep edmTriggerResults_*_*_*', 
        'keep triggerTriggerEvent_*_*_*', 
        'keep L1AcceptBunchCrossings_*_*_*', 
        'keep L1TriggerScalerss_*_*_*', 
        'keep Level1TriggerScalerss_*_*_*', 
        'keep LumiScalerss_*_*_*', 
        'keep BeamSpotOnlines_*_*_*', 
        'keep DcsStatuss_*_*_*', 
        'keep *_logErrorHarvester_*_*', 
        'keep *_pfIsolatedElectronsEI_*_*', 
        'keep *_pfIsolatedMuonsEI_*_*', 
        'keep recoPFJets_pfJetsEI_*_*', 
        'keep *_pfJetTrackAssociatorEI_*_*', 
        'keep *_impactParameterTagInfosEI_*_*', 
        'keep *_secondaryVertexTagInfosEI_*_*', 
        'keep *_combinedSecondaryVertexBJetTagsEI_*_*', 
        'keep recoPFTaus_pfTausEI_*_*', 
        'keep recoPFTauDiscriminator_pfTausDiscrimination*_*_*', 
        'keep *_pfMetEI_*_*', 
        'keep LHERunInfoProduct_*_*_*', 
        'keep LHEEventProduct_*_*_*', 
        'keep GenRunInfoProduct_generator_*_*', 
        'keep GenLumiInfoProduct_generator_*_*', 
        'keep GenEventInfoProduct_generator_*_*', 
        'keep edmHepMCProduct_generator_*_*', 
        'keep GenFilterInfo_*_*_*', 
        'keep *_genParticles_*_*', 
        'keep recoGenMETs_*_*_*', 
        'keep *_kt4GenJets_*_*', 
        'keep *_kt6GenJets_*_*', 
        'keep *_ak4GenJets_*_*', 
        'keep *_ak5GenJets_*_*', 
        'keep *_ak7GenJets_*_*', 
        'keep *_iterativeCone5GenJets_*_*', 
        'keep *_genParticle_*_*', 
        'keep edmHepMCProduct_source_*_*', 
        'keep SimTracks_g4SimHits_*_*', 
        'keep SimVertexs_g4SimHits_*_*', 
        'keep *_allTrackMCMatch_*_*', 
        'keep StripDigiSimLinkedmDetSetVector_simMuonCSCDigis_*_*', 
        'keep DTLayerIdDTDigiSimLinkMuonDigiCollection_simMuonDTDigis_*_*', 
        'keep RPCDigiSimLinkedmDetSetVector_simMuonRPCDigis_*_*', 
        'keep PileupSummaryInfos_*_*_*', 
        'drop *_trackingtruthprod_*_*', 
        'drop *_electrontruth_*_*', 
        'keep *_mix_MergedTrackTruth_*', 
        'keep CrossingFramePlaybackInfoExtended_*_*_*', 
        'keep PixelDigiSimLinkedmDetSetVector_simSiPixelDigis_*_*', 
        'keep StripDigiSimLinkedmDetSetVector_simSiStripDigis_*_*', 
        'drop *_mix_simSiPixelDigis*_*', 
        'drop *_mix_simSiStripDigis*_*', 
        'keep *_allTrackMCMatch_*_*' ) )
)

process.RECOEventContent = cms.PSet(
    splitLevel = cms.untracked.int32(0),
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
    outputCommands = cms.untracked.vstring( ('drop *', 
        'keep DetIdedmEDCollection_siStripDigis_*_*', 
        'keep DetIdedmEDCollection_siPixelDigis_*_*', 
        'keep *_siPixelClusters_*_*', 
        'keep *_siStripClusters_*_*', 
        'keep *_clusterSummaryProducer_*_*', 
        'keep *_dt1DRecHits_*_*', 
        'keep *_dt4DSegments_*_*', 
        'keep *_dt1DCosmicRecHits_*_*', 
        'keep *_dt4DCosmicSegments_*_*', 
        'keep *_csc2DRecHits_*_*', 
        'keep *_cscSegments_*_*', 
        'keep *_rpcRecHits_*_*', 
        'keep *_hbhereco_*_*', 
        'keep *_hbheprereco_*_*', 
        'keep *_hfreco_*_*', 
        'keep *_horeco_*_*', 
        'keep HBHERecHitsSorted_hbherecoMB_*_*', 
        'keep HORecHitsSorted_horecoMB_*_*', 
        'keep HFRecHitsSorted_hfrecoMB_*_*', 
        'keep ZDCDataFramesSorted_*Digis_*_*', 
        'keep ZDCRecHitsSorted_*_*_*', 
        'keep *_reducedHcalRecHits_*_*', 
        'keep *_castorreco_*_*', 
        'keep HcalUnpackerReport_*_*_*', 
        'keep *_ecalPreshowerRecHit_*_*', 
        'keep *_ecalRecHit_*_*', 
        'keep *_ecalCompactTrigPrim_*_*', 
        'keep *_ecalTPSkim_*_*', 
        'keep *_selectDigi_*_*', 
        'keep EcalRecHitsSorted_reducedEcalRecHitsEE_*_*', 
        'keep EcalRecHitsSorted_reducedEcalRecHitsEB_*_*', 
        'keep EcalRecHitsSorted_reducedEcalRecHitsES_*_*', 
        'keep *_hybridSuperClusters_*_*', 
        'keep recoSuperClusters_correctedHybridSuperClusters_*_*', 
        'keep *_multi5x5SuperClusters_*_*', 
        'keep recoSuperClusters_multi5x5SuperClusters_*_*', 
        'keep recoSuperClusters_multi5x5SuperClustersWithPreshower_*_*', 
        'keep recoSuperClusters_correctedMulti5x5SuperClustersWithPreshower_*_*', 
        'keep recoPreshowerClusters_multi5x5SuperClustersWithPreshower_*_*', 
        'keep recoPreshowerClusterShapes_multi5x5PreshowerClusterShape_*_*', 
        'keep *_particleFlowSuperClusterECAL_*_*', 
        'drop recoClusterShapes_*_*_*', 
        'drop recoBasicClustersToOnerecoClusterShapesAssociation_*_*_*', 
        'drop recoBasicClusters_multi5x5BasicClusters_multi5x5BarrelBasicClusters_*', 
        'drop recoSuperClusters_multi5x5SuperClusters_multi5x5BarrelSuperClusters_*', 
        'keep *_CkfElectronCandidates_*_*', 
        'keep *_GsfGlobalElectronTest_*_*', 
        'keep *_electronMergedSeeds_*_*', 
        'keep recoGsfTracks_electronGsfTracks_*_*', 
        'keep recoGsfTrackExtras_electronGsfTracks_*_*', 
        'keep recoTrackExtras_electronGsfTracks_*_*', 
        'keep TrackingRecHitsOwned_electronGsfTracks_*_*', 
        'keep recoTracks_generalTracks_*_*', 
        'keep recoTrackExtras_generalTracks_*_*', 
        'keep TrackingRecHitsOwned_extraFromSeeds_*_*', 
        'keep uints_extraFromSeeds_*_*', 
        'keep TrackingRecHitsOwned_generalTracks_*_*', 
        'keep recoTracks_beamhaloTracks_*_*', 
        'keep recoTrackExtras_beamhaloTracks_*_*', 
        'keep TrackingRecHitsOwned_beamhaloTracks_*_*', 
        'keep recoTracks_regionalCosmicTracks_*_*', 
        'keep recoTrackExtras_regionalCosmicTracks_*_*', 
        'keep TrackingRecHitsOwned_regionalCosmicTracks_*_*', 
        'keep recoTracks_rsWithMaterialTracks_*_*', 
        'keep recoTrackExtras_rsWithMaterialTracks_*_*', 
        'keep TrackingRecHitsOwned_rsWithMaterialTracks_*_*', 
        'keep recoTracks_conversionStepTracks_*_*', 
        'keep recoTrackExtras_conversionStepTracks_*_*', 
        'keep TrackingRecHitsOwned_conversionStepTracks_*_*', 
        'keep *_ctfPixelLess_*_*', 
        'keep *_dedxTruncated40_*_*', 
        'keep *_dedxDiscrimASmi_*_*', 
        'keep *_dedxHarmonic2_*_*', 
        'keep *_trackExtrapolator_*_*', 
        'keep floatedmValueMap_generalTracks_*_*', 
        'keep *_kt6CaloJets_*_*', 
        'keep *_ak4CaloJets_*_*', 
        'keep *_ak5CaloJets_*_*', 
        'keep double_kt6PFJets_rho_*', 
        'keep *_ak4PFJets_*_*', 
        'keep *_ak5PFJets_*_*', 
        'keep *_ak8PFJets_*_*', 
        'keep *_ak4PFJetsCHS_*_*', 
        'keep *_ak5PFJetsCHS_*_*', 
        'keep *_ak8PFJetsCHS_*_*', 
        'keep *_ca8PFJetsCHS_*_*', 
        'keep *_ca8PFJetsCHSPruned_*_*', 
        'keep *_cmsTopTagPFJetsCHS_*_*', 
        'keep *_JetPlusTrackZSPCorJetAntiKt4_*_*', 
        'keep *_ak4TrackJets_*_*', 
        'keep recoRecoChargedRefCandidates_trackRefsForJets_*_*', 
        'keep *_caloTowers_*_*', 
        'keep *_towerMaker_*_*', 
        'keep *_CastorTowerReco_*_*', 
        'keep *_kt4JetTracksAssociatorAtVertex_*_*', 
        'keep *_kt4JetTracksAssociatorAtCaloFace_*_*', 
        'keep *_kt4JetExtender_*_*', 
        'keep *_ak4JetTracksAssociatorAtVertex_*_*', 
        'keep *_ak4JetTracksAssociatorAtVertexPF_*_*', 
        'keep *_ak4JetTracksAssociatorAtCaloFace_*_*', 
        'keep *_ak4JetTracksAssociatorExplicit_*_*', 
        'keep *_ak4JetExtender_*_*', 
        'keep *_ak7JetTracksAssociatorAtVertex*_*_*', 
        'keep *_ak7JetTracksAssociatorAtCaloFace*_*_*', 
        'keep *_ak7JetExtender_*_*', 
        'keep *_ak4JetID_*_*', 
        'keep *_ak5JetID_*_*', 
        'keep *_ak7JetID_*_*', 
        'keep *_ic5JetID_*_*', 
        'keep *_kt4JetID_*_*', 
        'keep *_kt6JetID_*_*', 
        'keep *_ak7BasicJets_*_*', 
        'keep *_ak7CastorJetID_*_*', 
        'keep double_kt6CaloJetsCentral_*_*', 
        'keep double_kt6PFJetsCentralChargedPileUp_*_*', 
        'keep double_kt6PFJetsCentralNeutral_*_*', 
        'keep double_kt6PFJetsCentralNeutralTight_*_*', 
        'keep *_fixedGridRho*_*_*', 
        'keep *_ca8PFJetsCHSPrunedLinks_*_*', 
        'keep recoCaloMETs_caloMet_*_*', 
        'keep recoCaloMETs_caloMetBE_*_*', 
        'keep recoCaloMETs_caloMetBEFO_*_*', 
        'keep recoCaloMETs_caloMetM_*_*', 
        'keep recoPFMETs_pfMet_*_*', 
        'keep recoPFMETs_pfChMet_*_*', 
        'keep recoMuonMETCorrectionDataedmValueMap_muonMETValueMapProducer_*_*', 
        'keep recoHcalNoiseRBXs_hcalnoise_*_*', 
        'keep HcalNoiseSummary_hcalnoise_*_*', 
        'keep *HaloData_*_*_*', 
        'keep *BeamHaloSummary_BeamHaloSummary_*_*', 
        'keep *_MuonSeed_*_*', 
        'keep *_ancientMuonSeed_*_*', 
        'keep *_displacedMuonSeeds_*_*', 
        'keep *_mergedStandAloneMuonSeeds_*_*', 
        'keep TrackingRecHitsOwned_globalMuons_*_*', 
        'keep TrackingRecHitsOwned_tevMuons_*_*', 
        'keep recoCaloMuons_calomuons_*_*', 
        'keep *_CosmicMuonSeed_*_*', 
        'keep recoTrackExtras_cosmicMuons_*_*', 
        'keep TrackingRecHitsOwned_cosmicMuons_*_*', 
        'keep recoTrackExtras_globalCosmicMuons_*_*', 
        'keep TrackingRecHitsOwned_globalCosmicMuons_*_*', 
        'keep recoTrackExtras_cosmicMuons1Leg_*_*', 
        'keep TrackingRecHitsOwned_cosmicMuons1Leg_*_*', 
        'keep recoTrackExtras_globalCosmicMuons1Leg_*_*', 
        'keep TrackingRecHitsOwned_globalCosmicMuons1Leg_*_*', 
        'keep recoTracks_cosmicsVetoTracks_*_*', 
        'keep *_SETMuonSeed_*_*', 
        'keep recoTracks_standAloneSETMuons_*_*', 
        'keep recoTrackExtras_standAloneSETMuons_*_*', 
        'keep TrackingRecHitsOwned_standAloneSETMuons_*_*', 
        'keep recoTracks_globalSETMuons_*_*', 
        'keep recoTrackExtras_globalSETMuons_*_*', 
        'keep TrackingRecHitsOwned_globalSETMuons_*_*', 
        'keep recoMuons_muonsWithSET_*_*', 
        'keep *_muons_*_*', 
        'keep *_*_muons_*', 
        'drop *_muons_muons1stStep2muonsMap_*', 
        'drop recoIsoDepositedmValueMap_muons_*_*', 
        'drop doubleedmValueMap_muons_muPFIso*_*', 
        'keep recoTracks_standAloneMuons_*_*', 
        'keep recoTrackExtras_standAloneMuons_*_*', 
        'keep TrackingRecHitsOwned_standAloneMuons_*_*', 
        'keep recoTracks_globalMuons_*_*', 
        'keep recoTrackExtras_globalMuons_*_*', 
        'keep recoTracks_tevMuons_*_*', 
        'keep recoTrackExtras_tevMuons_*_*', 
        'keep recoTracks_generalTracks_*_*', 
        'keep recoTracksToOnerecoTracksAssociation_tevMuons_*_*', 
        'keep recoTracks_cosmicMuons_*_*', 
        'keep recoTracks_globalCosmicMuons_*_*', 
        'keep recoMuons_muonsFromCosmics_*_*', 
        'keep recoTracks_cosmicMuons1Leg_*_*', 
        'keep recoTracks_globalCosmicMuons1Leg_*_*', 
        'keep recoMuons_muonsFromCosmics1Leg_*_*', 
        'keep recoTracks_refittedStandAloneMuons_*_*', 
        'keep recoTrackExtras_refittedStandAloneMuons_*_*', 
        'keep TrackingRecHitsOwned_refittedStandAloneMuons_*_*', 
        'keep recoTracks_displacedStandAloneMuons__*', 
        'keep recoTrackExtras_displacedStandAloneMuons_*_*', 
        'keep TrackingRecHitsOwned_displacedStandAloneMuons_*_*', 
        'keep *_muIsoDepositTk_*_*', 
        'keep *_muIsoDepositCalByAssociatorTowers_*_*', 
        'keep *_muIsoDepositCalByAssociatorHits_*_*', 
        'keep *_muIsoDepositJets_*_*', 
        'keep *_muGlobalIsoDepositCtfTk_*_*', 
        'keep *_muGlobalIsoDepositCalByAssociatorTowers_*_*', 
        'keep *_muGlobalIsoDepositCalByAssociatorHits_*_*', 
        'keep *_muGlobalIsoDepositJets_*_*', 
        'keep *_impactParameterTagInfos_*_*', 
        'keep *_trackCountingHighEffBJetTags_*_*', 
        'keep *_trackCountingHighPurBJetTags_*_*', 
        'keep *_jetProbabilityBJetTags_*_*', 
        'keep *_jetBProbabilityBJetTags_*_*', 
        'keep *_secondaryVertexTagInfos_*_*', 
        'keep *_inclusiveSecondaryVertexFinderTagInfos_*_*', 
        'keep *_ghostTrackVertexTagInfos_*_*', 
        'keep *_simpleSecondaryVertexBJetTags_*_*', 
        'keep *_simpleSecondaryVertexHighEffBJetTags_*_*', 
        'keep *_simpleSecondaryVertexHighPurBJetTags_*_*', 
        'keep *_combinedSecondaryVertexBJetTags_*_*', 
        'keep *_combinedInclusiveSecondaryVertexV2BJetTags_*_*', 
        'keep *_ghostTrackBJetTags_*_*', 
        'keep *_softPFMuonsTagInfos_*_*', 
        'keep *_softPFElectronsTagInfos_*_*', 
        'keep *_softPFElectronBJetTags_*_*', 
        'keep *_softPFMuonBJetTags_*_*', 
        'keep *_softMuonTagInfos_*_*', 
        'keep *_softMuonBJetTags_*_*', 
        'keep *_softMuonByIP3dBJetTags_*_*', 
        'keep *_softMuonByPtBJetTags_*_*', 
        'keep *_combinedMVABJetTags_*_*', 
        'keep *_pfImpactParameterTagInfos_*_*', 
        'keep *_pfSecondaryVertexTagInfos_*_*', 
        'keep *_pfCombinedSecondaryVertexBJetTags_*_*', 
        'keep *_ak4PFJetsRecoTauPiZeros_*_*', 
        'keep *_hpsPFTauProducer_*_*', 
        'keep *_hpsPFTauDiscrimination*_*_*', 
        'keep *_hpsPFTau*PtSum_*_*', 
        'keep *_hpsPFTauTransverseImpactParameters_*_*', 
        'keep  *_offlinePrimaryVertices__*', 
        'keep  *_offlinePrimaryVerticesWithBS_*_*', 
        'keep  *_offlinePrimaryVerticesFromCosmicTracks_*_*', 
        'keep  *_nuclearInteractionMaker_*_*', 
        'keep *_generalV0Candidates_*_*', 
        'keep *_inclusiveSecondaryVertices_*_*', 
        'keep recoGsfElectronCores_gsfElectronCores_*_*', 
        'keep recoGsfElectrons_gsfElectrons_*_*', 
        'keep recoGsfElectronCores_uncleanedOnlyGsfElectronCores_*_*', 
        'keep recoGsfElectrons_uncleanedOnlyGsfElectrons_*_*', 
        'keep floatedmValueMap_eidRobustLoose_*_*', 
        'keep floatedmValueMap_eidRobustTight_*_*', 
        'keep floatedmValueMap_eidRobustHighEnergy_*_*', 
        'keep floatedmValueMap_eidLoose_*_*', 
        'keep floatedmValueMap_eidTight_*_*', 
        'keep *_egmGedGsfElectronPFIsolation_*_*', 
        'keep *_egmGsfElectronIDs_*_*', 
        'keep *_gedPhotonCore_*_*', 
        'keep *_gedPhotons_*_*', 
        'keep *_particleBasedIsolation_*_*', 
        'keep recoPhotons_mustachePhotons_*_*', 
        'keep recoPhotonCores_mustachePhotonCore_*_*', 
        'keep recoPhotons_photons_*_*', 
        'keep recoPhotonCores_photonCore_*_*', 
        'keep recoConversions_conversions_*_*', 
        'keep recoConversions_mustacheConversions_*_*', 
        'drop *_conversions_uncleanedConversions_*', 
        'drop *_gedPhotonsTmp_valMapPFEgammaCandToPhoton_*', 
        'keep recoConversions_allConversions_*_*', 
        'keep recoConversions_allConversionsOldEG_*_*', 
        'keep recoTracks_ckfOutInTracksFrom*Conversions_*_*', 
        'keep recoTracks_ckfInOutTracksFrom*Conversions_*_*', 
        'keep recoTrackExtras_ckfOutInTracksFrom*Conversions_*_*', 
        'keep recoTrackExtras_ckfInOutTracksFrom*Conversions_*_*', 
        'keep TrackingRecHitsOwned_ckfOutInTracksFrom*Conversions_*_*', 
        'keep TrackingRecHitsOwned_ckfInOutTracksFrom*Conversions_*_*', 
        'keep recoConversions_uncleanedOnlyAllConversions_*_*', 
        'keep recoTracks_uncleanedOnlyCkfOutInTracksFromConversions_*_*', 
        'keep recoTracks_uncleanedOnlyCkfInOutTracksFromConversions_*_*', 
        'keep recoTrackExtras_uncleanedOnlyCkfOutInTracksFromConversions_*_*', 
        'keep recoTrackExtras_uncleanedOnlyCkfInOutTracksFromConversions_*_*', 
        'keep TrackingRecHitsOwned_uncleanedOnlyCkfOutInTracksFromConversions_*_*', 
        'keep TrackingRecHitsOwned_uncleanedOnlyCkfInOutTracksFromConversions_*_*', 
        'keep *_PhotonIDProd_*_*', 
        'keep *_PhotonIDProdGED_*_*', 
        'keep *_hfRecoEcalCandidate_*_*', 
        'keep *_hfEMClusters_*_*', 
        'keep *_gedGsfElectronCores_*_*', 
        'keep *_gedGsfElectrons_*_*', 
        'keep *_pixelTracks_*_*', 
        'keep *_pixelVertices_*_*', 
        'drop CaloTowersSorted_towerMakerPF_*_*', 
        'keep recoPFRecHits_particleFlowClusterECAL_Cleaned_*', 
        'keep recoPFRecHits_particleFlowClusterHCAL_Cleaned_*', 
        'keep recoPFRecHits_particleFlowClusterHO_Cleaned_*', 
        'keep recoPFRecHits_particleFlowClusterHFEM_Cleaned_*', 
        'keep recoPFRecHits_particleFlowClusterHFHAD_Cleaned_*', 
        'keep recoPFRecHits_particleFlowClusterPS_Cleaned_*', 
        'keep recoPFRecHits_particleFlowRecHitECAL_Cleaned_*', 
        'keep recoPFRecHits_particleFlowRecHitHCAL_Cleaned_*', 
        'keep recoPFRecHits_particleFlowRecHitHO_Cleaned_*', 
        'keep recoPFRecHits_particleFlowRecHitPS_Cleaned_*', 
        'keep recoPFClusters_particleFlowClusterECAL_*_*', 
        'keep recoPFClusters_particleFlowClusterHCAL_*_*', 
        'keep recoPFClusters_particleFlowClusterHO_*_*', 
        'keep recoPFClusters_particleFlowClusterPS_*_*', 
        'keep recoPFBlocks_particleFlowBlock_*_*', 
        'keep recoPFCandidates_particleFlowEGamma_*_*', 
        'keep recoCaloClusters_particleFlowEGamma_*_*', 
        'keep recoSuperClusters_particleFlowEGamma_*_*', 
        'keep recoConversions_particleFlowEGamma_*_*', 
        'keep recoPFCandidates_particleFlow_*_*', 
        'keep recoPFCandidates_particleFlowTmp_electrons_*', 
        'keep recoPFCandidates_particleFlowTmp_*_*', 
        'drop recoPFCandidates_particleFlowTmp__*', 
        'keep recoPFDisplacedVertexs_particleFlowDisplacedVertex_*_*', 
        'keep *_pfElectronTranslator_*_*', 
        'keep *_pfPhotonTranslator_*_*', 
        'keep *_particleFlow_electrons_*', 
        'keep *_particleFlow_photons_*', 
        'keep *_trackerDrivenElectronSeeds_preid_*', 
        'keep *_particleFlowPtrs_*_*', 
        'keep *_particleFlowTmpPtrs_*_*', 
        'keep *_offlineBeamSpot_*_*', 
        'keep L1GlobalTriggerReadoutRecord_gtDigis_*_*', 
        'keep *_l1GtRecord_*_*', 
        'keep *_l1GtTriggerMenuLite_*_*', 
        'keep *_conditionsInEdm_*_*', 
        'keep *_l1extraParticles_*_*', 
        'keep *_l1L1GtObjectMap_*_*', 
        'keep L1MuGMTReadoutCollection_gtDigis_*_*', 
        'keep L1GctEmCand*_gctDigis_*_*', 
        'keep L1GctJetCand*_gctDigis_*_*', 
        'keep L1GctEtHad*_gctDigis_*_*', 
        'keep L1GctEtMiss*_gctDigis_*_*', 
        'keep L1GctEtTotal*_gctDigis_*_*', 
        'keep L1GctHtMiss*_gctDigis_*_*', 
        'keep L1GctJetCounts*_gctDigis_*_*', 
        'keep L1GctHFRingEtSums*_gctDigis_*_*', 
        'keep L1GctHFBitCounts*_gctDigis_*_*', 
        'keep LumiDetails_lumiProducer_*_*', 
        'keep LumiSummary_lumiProducer_*_*', 
        'drop *_hlt*_*_*', 
        'keep *_hltL1GtObjectMap_*_*', 
        'keep edmTriggerResults_*_*_*', 
        'keep triggerTriggerEvent_*_*_*', 
        'keep L1AcceptBunchCrossings_*_*_*', 
        'keep L1TriggerScalerss_*_*_*', 
        'keep Level1TriggerScalerss_*_*_*', 
        'keep LumiScalerss_*_*_*', 
        'keep BeamSpotOnlines_*_*_*', 
        'keep DcsStatuss_*_*_*', 
        'keep *_logErrorHarvester_*_*', 
        'keep *_pfIsolatedElectronsEI_*_*', 
        'keep *_pfIsolatedMuonsEI_*_*', 
        'keep recoPFJets_pfJetsEI_*_*', 
        'keep *_pfJetTrackAssociatorEI_*_*', 
        'keep *_impactParameterTagInfosEI_*_*', 
        'keep *_secondaryVertexTagInfosEI_*_*', 
        'keep *_combinedSecondaryVertexBJetTagsEI_*_*', 
        'keep recoPFTaus_pfTausEI_*_*', 
        'keep recoPFTauDiscriminator_pfTausDiscrimination*_*_*', 
        'keep *_pfMetEI_*_*' ) )
)

process.RECOSIMEventContent = cms.PSet(
    splitLevel = cms.untracked.int32(0),
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
    outputCommands = cms.untracked.vstring( ('drop *', 
        'drop *', 
        'keep DetIdedmEDCollection_siStripDigis_*_*', 
        'keep DetIdedmEDCollection_siPixelDigis_*_*', 
        'keep *_siPixelClusters_*_*', 
        'keep *_siStripClusters_*_*', 
        'keep *_clusterSummaryProducer_*_*', 
        'keep *_dt1DRecHits_*_*', 
        'keep *_dt4DSegments_*_*', 
        'keep *_dt1DCosmicRecHits_*_*', 
        'keep *_dt4DCosmicSegments_*_*', 
        'keep *_csc2DRecHits_*_*', 
        'keep *_cscSegments_*_*', 
        'keep *_rpcRecHits_*_*', 
        'keep *_hbhereco_*_*', 
        'keep *_hbheprereco_*_*', 
        'keep *_hfreco_*_*', 
        'keep *_horeco_*_*', 
        'keep HBHERecHitsSorted_hbherecoMB_*_*', 
        'keep HORecHitsSorted_horecoMB_*_*', 
        'keep HFRecHitsSorted_hfrecoMB_*_*', 
        'keep ZDCDataFramesSorted_*Digis_*_*', 
        'keep ZDCRecHitsSorted_*_*_*', 
        'keep *_reducedHcalRecHits_*_*', 
        'keep *_castorreco_*_*', 
        'keep HcalUnpackerReport_*_*_*', 
        'keep *_ecalPreshowerRecHit_*_*', 
        'keep *_ecalRecHit_*_*', 
        'keep *_ecalCompactTrigPrim_*_*', 
        'keep *_ecalTPSkim_*_*', 
        'keep *_selectDigi_*_*', 
        'keep EcalRecHitsSorted_reducedEcalRecHitsEE_*_*', 
        'keep EcalRecHitsSorted_reducedEcalRecHitsEB_*_*', 
        'keep EcalRecHitsSorted_reducedEcalRecHitsES_*_*', 
        'keep *_hybridSuperClusters_*_*', 
        'keep recoSuperClusters_correctedHybridSuperClusters_*_*', 
        'keep *_multi5x5SuperClusters_*_*', 
        'keep recoSuperClusters_multi5x5SuperClusters_*_*', 
        'keep recoSuperClusters_multi5x5SuperClustersWithPreshower_*_*', 
        'keep recoSuperClusters_correctedMulti5x5SuperClustersWithPreshower_*_*', 
        'keep recoPreshowerClusters_multi5x5SuperClustersWithPreshower_*_*', 
        'keep recoPreshowerClusterShapes_multi5x5PreshowerClusterShape_*_*', 
        'keep *_particleFlowSuperClusterECAL_*_*', 
        'drop recoClusterShapes_*_*_*', 
        'drop recoBasicClustersToOnerecoClusterShapesAssociation_*_*_*', 
        'drop recoBasicClusters_multi5x5BasicClusters_multi5x5BarrelBasicClusters_*', 
        'drop recoSuperClusters_multi5x5SuperClusters_multi5x5BarrelSuperClusters_*', 
        'keep *_CkfElectronCandidates_*_*', 
        'keep *_GsfGlobalElectronTest_*_*', 
        'keep *_electronMergedSeeds_*_*', 
        'keep recoGsfTracks_electronGsfTracks_*_*', 
        'keep recoGsfTrackExtras_electronGsfTracks_*_*', 
        'keep recoTrackExtras_electronGsfTracks_*_*', 
        'keep TrackingRecHitsOwned_electronGsfTracks_*_*', 
        'keep recoTracks_generalTracks_*_*', 
        'keep recoTrackExtras_generalTracks_*_*', 
        'keep TrackingRecHitsOwned_extraFromSeeds_*_*', 
        'keep uints_extraFromSeeds_*_*', 
        'keep TrackingRecHitsOwned_generalTracks_*_*', 
        'keep recoTracks_beamhaloTracks_*_*', 
        'keep recoTrackExtras_beamhaloTracks_*_*', 
        'keep TrackingRecHitsOwned_beamhaloTracks_*_*', 
        'keep recoTracks_regionalCosmicTracks_*_*', 
        'keep recoTrackExtras_regionalCosmicTracks_*_*', 
        'keep TrackingRecHitsOwned_regionalCosmicTracks_*_*', 
        'keep recoTracks_rsWithMaterialTracks_*_*', 
        'keep recoTrackExtras_rsWithMaterialTracks_*_*', 
        'keep TrackingRecHitsOwned_rsWithMaterialTracks_*_*', 
        'keep recoTracks_conversionStepTracks_*_*', 
        'keep recoTrackExtras_conversionStepTracks_*_*', 
        'keep TrackingRecHitsOwned_conversionStepTracks_*_*', 
        'keep *_ctfPixelLess_*_*', 
        'keep *_dedxTruncated40_*_*', 
        'keep *_dedxDiscrimASmi_*_*', 
        'keep *_dedxHarmonic2_*_*', 
        'keep *_trackExtrapolator_*_*', 
        'keep floatedmValueMap_generalTracks_*_*', 
        'keep *_kt6CaloJets_*_*', 
        'keep *_ak4CaloJets_*_*', 
        'keep *_ak5CaloJets_*_*', 
        'keep double_kt6PFJets_rho_*', 
        'keep *_ak4PFJets_*_*', 
        'keep *_ak5PFJets_*_*', 
        'keep *_ak8PFJets_*_*', 
        'keep *_ak4PFJetsCHS_*_*', 
        'keep *_ak5PFJetsCHS_*_*', 
        'keep *_ak8PFJetsCHS_*_*', 
        'keep *_ca8PFJetsCHS_*_*', 
        'keep *_ca8PFJetsCHSPruned_*_*', 
        'keep *_cmsTopTagPFJetsCHS_*_*', 
        'keep *_JetPlusTrackZSPCorJetAntiKt4_*_*', 
        'keep *_ak4TrackJets_*_*', 
        'keep recoRecoChargedRefCandidates_trackRefsForJets_*_*', 
        'keep *_caloTowers_*_*', 
        'keep *_towerMaker_*_*', 
        'keep *_CastorTowerReco_*_*', 
        'keep *_kt4JetTracksAssociatorAtVertex_*_*', 
        'keep *_kt4JetTracksAssociatorAtCaloFace_*_*', 
        'keep *_kt4JetExtender_*_*', 
        'keep *_ak4JetTracksAssociatorAtVertex_*_*', 
        'keep *_ak4JetTracksAssociatorAtVertexPF_*_*', 
        'keep *_ak4JetTracksAssociatorAtCaloFace_*_*', 
        'keep *_ak4JetTracksAssociatorExplicit_*_*', 
        'keep *_ak4JetExtender_*_*', 
        'keep *_ak7JetTracksAssociatorAtVertex*_*_*', 
        'keep *_ak7JetTracksAssociatorAtCaloFace*_*_*', 
        'keep *_ak7JetExtender_*_*', 
        'keep *_ak4JetID_*_*', 
        'keep *_ak5JetID_*_*', 
        'keep *_ak7JetID_*_*', 
        'keep *_ic5JetID_*_*', 
        'keep *_kt4JetID_*_*', 
        'keep *_kt6JetID_*_*', 
        'keep *_ak7BasicJets_*_*', 
        'keep *_ak7CastorJetID_*_*', 
        'keep double_kt6CaloJetsCentral_*_*', 
        'keep double_kt6PFJetsCentralChargedPileUp_*_*', 
        'keep double_kt6PFJetsCentralNeutral_*_*', 
        'keep double_kt6PFJetsCentralNeutralTight_*_*', 
        'keep *_fixedGridRho*_*_*', 
        'keep *_ca8PFJetsCHSPrunedLinks_*_*', 
        'keep recoCaloMETs_caloMet_*_*', 
        'keep recoCaloMETs_caloMetBE_*_*', 
        'keep recoCaloMETs_caloMetBEFO_*_*', 
        'keep recoCaloMETs_caloMetM_*_*', 
        'keep recoPFMETs_pfMet_*_*', 
        'keep recoPFMETs_pfChMet_*_*', 
        'keep recoMuonMETCorrectionDataedmValueMap_muonMETValueMapProducer_*_*', 
        'keep recoHcalNoiseRBXs_hcalnoise_*_*', 
        'keep HcalNoiseSummary_hcalnoise_*_*', 
        'keep *HaloData_*_*_*', 
        'keep *BeamHaloSummary_BeamHaloSummary_*_*', 
        'keep *_MuonSeed_*_*', 
        'keep *_ancientMuonSeed_*_*', 
        'keep *_displacedMuonSeeds_*_*', 
        'keep *_mergedStandAloneMuonSeeds_*_*', 
        'keep TrackingRecHitsOwned_globalMuons_*_*', 
        'keep TrackingRecHitsOwned_tevMuons_*_*', 
        'keep recoCaloMuons_calomuons_*_*', 
        'keep *_CosmicMuonSeed_*_*', 
        'keep recoTrackExtras_cosmicMuons_*_*', 
        'keep TrackingRecHitsOwned_cosmicMuons_*_*', 
        'keep recoTrackExtras_globalCosmicMuons_*_*', 
        'keep TrackingRecHitsOwned_globalCosmicMuons_*_*', 
        'keep recoTrackExtras_cosmicMuons1Leg_*_*', 
        'keep TrackingRecHitsOwned_cosmicMuons1Leg_*_*', 
        'keep recoTrackExtras_globalCosmicMuons1Leg_*_*', 
        'keep TrackingRecHitsOwned_globalCosmicMuons1Leg_*_*', 
        'keep recoTracks_cosmicsVetoTracks_*_*', 
        'keep *_SETMuonSeed_*_*', 
        'keep recoTracks_standAloneSETMuons_*_*', 
        'keep recoTrackExtras_standAloneSETMuons_*_*', 
        'keep TrackingRecHitsOwned_standAloneSETMuons_*_*', 
        'keep recoTracks_globalSETMuons_*_*', 
        'keep recoTrackExtras_globalSETMuons_*_*', 
        'keep TrackingRecHitsOwned_globalSETMuons_*_*', 
        'keep recoMuons_muonsWithSET_*_*', 
        'keep *_muons_*_*', 
        'keep *_*_muons_*', 
        'drop *_muons_muons1stStep2muonsMap_*', 
        'drop recoIsoDepositedmValueMap_muons_*_*', 
        'drop doubleedmValueMap_muons_muPFIso*_*', 
        'keep recoTracks_standAloneMuons_*_*', 
        'keep recoTrackExtras_standAloneMuons_*_*', 
        'keep TrackingRecHitsOwned_standAloneMuons_*_*', 
        'keep recoTracks_globalMuons_*_*', 
        'keep recoTrackExtras_globalMuons_*_*', 
        'keep recoTracks_tevMuons_*_*', 
        'keep recoTrackExtras_tevMuons_*_*', 
        'keep recoTracks_generalTracks_*_*', 
        'keep recoTracksToOnerecoTracksAssociation_tevMuons_*_*', 
        'keep recoTracks_cosmicMuons_*_*', 
        'keep recoTracks_globalCosmicMuons_*_*', 
        'keep recoMuons_muonsFromCosmics_*_*', 
        'keep recoTracks_cosmicMuons1Leg_*_*', 
        'keep recoTracks_globalCosmicMuons1Leg_*_*', 
        'keep recoMuons_muonsFromCosmics1Leg_*_*', 
        'keep recoTracks_refittedStandAloneMuons_*_*', 
        'keep recoTrackExtras_refittedStandAloneMuons_*_*', 
        'keep TrackingRecHitsOwned_refittedStandAloneMuons_*_*', 
        'keep recoTracks_displacedStandAloneMuons__*', 
        'keep recoTrackExtras_displacedStandAloneMuons_*_*', 
        'keep TrackingRecHitsOwned_displacedStandAloneMuons_*_*', 
        'keep *_muIsoDepositTk_*_*', 
        'keep *_muIsoDepositCalByAssociatorTowers_*_*', 
        'keep *_muIsoDepositCalByAssociatorHits_*_*', 
        'keep *_muIsoDepositJets_*_*', 
        'keep *_muGlobalIsoDepositCtfTk_*_*', 
        'keep *_muGlobalIsoDepositCalByAssociatorTowers_*_*', 
        'keep *_muGlobalIsoDepositCalByAssociatorHits_*_*', 
        'keep *_muGlobalIsoDepositJets_*_*', 
        'keep *_impactParameterTagInfos_*_*', 
        'keep *_trackCountingHighEffBJetTags_*_*', 
        'keep *_trackCountingHighPurBJetTags_*_*', 
        'keep *_jetProbabilityBJetTags_*_*', 
        'keep *_jetBProbabilityBJetTags_*_*', 
        'keep *_secondaryVertexTagInfos_*_*', 
        'keep *_inclusiveSecondaryVertexFinderTagInfos_*_*', 
        'keep *_ghostTrackVertexTagInfos_*_*', 
        'keep *_simpleSecondaryVertexBJetTags_*_*', 
        'keep *_simpleSecondaryVertexHighEffBJetTags_*_*', 
        'keep *_simpleSecondaryVertexHighPurBJetTags_*_*', 
        'keep *_combinedSecondaryVertexBJetTags_*_*', 
        'keep *_combinedInclusiveSecondaryVertexV2BJetTags_*_*', 
        'keep *_ghostTrackBJetTags_*_*', 
        'keep *_softPFMuonsTagInfos_*_*', 
        'keep *_softPFElectronsTagInfos_*_*', 
        'keep *_softPFElectronBJetTags_*_*', 
        'keep *_softPFMuonBJetTags_*_*', 
        'keep *_softMuonTagInfos_*_*', 
        'keep *_softMuonBJetTags_*_*', 
        'keep *_softMuonByIP3dBJetTags_*_*', 
        'keep *_softMuonByPtBJetTags_*_*', 
        'keep *_combinedMVABJetTags_*_*', 
        'keep *_pfImpactParameterTagInfos_*_*', 
        'keep *_pfSecondaryVertexTagInfos_*_*', 
        'keep *_pfCombinedSecondaryVertexBJetTags_*_*', 
        'keep *_ak4PFJetsRecoTauPiZeros_*_*', 
        'keep *_hpsPFTauProducer_*_*', 
        'keep *_hpsPFTauDiscrimination*_*_*', 
        'keep *_hpsPFTau*PtSum_*_*', 
        'keep *_hpsPFTauTransverseImpactParameters_*_*', 
        'keep  *_offlinePrimaryVertices__*', 
        'keep  *_offlinePrimaryVerticesWithBS_*_*', 
        'keep  *_offlinePrimaryVerticesFromCosmicTracks_*_*', 
        'keep  *_nuclearInteractionMaker_*_*', 
        'keep *_generalV0Candidates_*_*', 
        'keep *_inclusiveSecondaryVertices_*_*', 
        'keep recoGsfElectronCores_gsfElectronCores_*_*', 
        'keep recoGsfElectrons_gsfElectrons_*_*', 
        'keep recoGsfElectronCores_uncleanedOnlyGsfElectronCores_*_*', 
        'keep recoGsfElectrons_uncleanedOnlyGsfElectrons_*_*', 
        'keep floatedmValueMap_eidRobustLoose_*_*', 
        'keep floatedmValueMap_eidRobustTight_*_*', 
        'keep floatedmValueMap_eidRobustHighEnergy_*_*', 
        'keep floatedmValueMap_eidLoose_*_*', 
        'keep floatedmValueMap_eidTight_*_*', 
        'keep *_egmGedGsfElectronPFIsolation_*_*', 
        'keep *_egmGsfElectronIDs_*_*', 
        'keep *_gedPhotonCore_*_*', 
        'keep *_gedPhotons_*_*', 
        'keep *_particleBasedIsolation_*_*', 
        'keep recoPhotons_mustachePhotons_*_*', 
        'keep recoPhotonCores_mustachePhotonCore_*_*', 
        'keep recoPhotons_photons_*_*', 
        'keep recoPhotonCores_photonCore_*_*', 
        'keep recoConversions_conversions_*_*', 
        'keep recoConversions_mustacheConversions_*_*', 
        'drop *_conversions_uncleanedConversions_*', 
        'drop *_gedPhotonsTmp_valMapPFEgammaCandToPhoton_*', 
        'keep recoConversions_allConversions_*_*', 
        'keep recoConversions_allConversionsOldEG_*_*', 
        'keep recoTracks_ckfOutInTracksFrom*Conversions_*_*', 
        'keep recoTracks_ckfInOutTracksFrom*Conversions_*_*', 
        'keep recoTrackExtras_ckfOutInTracksFrom*Conversions_*_*', 
        'keep recoTrackExtras_ckfInOutTracksFrom*Conversions_*_*', 
        'keep TrackingRecHitsOwned_ckfOutInTracksFrom*Conversions_*_*', 
        'keep TrackingRecHitsOwned_ckfInOutTracksFrom*Conversions_*_*', 
        'keep recoConversions_uncleanedOnlyAllConversions_*_*', 
        'keep recoTracks_uncleanedOnlyCkfOutInTracksFromConversions_*_*', 
        'keep recoTracks_uncleanedOnlyCkfInOutTracksFromConversions_*_*', 
        'keep recoTrackExtras_uncleanedOnlyCkfOutInTracksFromConversions_*_*', 
        'keep recoTrackExtras_uncleanedOnlyCkfInOutTracksFromConversions_*_*', 
        'keep TrackingRecHitsOwned_uncleanedOnlyCkfOutInTracksFromConversions_*_*', 
        'keep TrackingRecHitsOwned_uncleanedOnlyCkfInOutTracksFromConversions_*_*', 
        'keep *_PhotonIDProd_*_*', 
        'keep *_PhotonIDProdGED_*_*', 
        'keep *_hfRecoEcalCandidate_*_*', 
        'keep *_hfEMClusters_*_*', 
        'keep *_gedGsfElectronCores_*_*', 
        'keep *_gedGsfElectrons_*_*', 
        'keep *_pixelTracks_*_*', 
        'keep *_pixelVertices_*_*', 
        'drop CaloTowersSorted_towerMakerPF_*_*', 
        'keep recoPFRecHits_particleFlowClusterECAL_Cleaned_*', 
        'keep recoPFRecHits_particleFlowClusterHCAL_Cleaned_*', 
        'keep recoPFRecHits_particleFlowClusterHO_Cleaned_*', 
        'keep recoPFRecHits_particleFlowClusterHFEM_Cleaned_*', 
        'keep recoPFRecHits_particleFlowClusterHFHAD_Cleaned_*', 
        'keep recoPFRecHits_particleFlowClusterPS_Cleaned_*', 
        'keep recoPFRecHits_particleFlowRecHitECAL_Cleaned_*', 
        'keep recoPFRecHits_particleFlowRecHitHCAL_Cleaned_*', 
        'keep recoPFRecHits_particleFlowRecHitHO_Cleaned_*', 
        'keep recoPFRecHits_particleFlowRecHitPS_Cleaned_*', 
        'keep recoPFClusters_particleFlowClusterECAL_*_*', 
        'keep recoPFClusters_particleFlowClusterHCAL_*_*', 
        'keep recoPFClusters_particleFlowClusterHO_*_*', 
        'keep recoPFClusters_particleFlowClusterPS_*_*', 
        'keep recoPFBlocks_particleFlowBlock_*_*', 
        'keep recoPFCandidates_particleFlowEGamma_*_*', 
        'keep recoCaloClusters_particleFlowEGamma_*_*', 
        'keep recoSuperClusters_particleFlowEGamma_*_*', 
        'keep recoConversions_particleFlowEGamma_*_*', 
        'keep recoPFCandidates_particleFlow_*_*', 
        'keep recoPFCandidates_particleFlowTmp_electrons_*', 
        'keep recoPFCandidates_particleFlowTmp_*_*', 
        'drop recoPFCandidates_particleFlowTmp__*', 
        'keep recoPFDisplacedVertexs_particleFlowDisplacedVertex_*_*', 
        'keep *_pfElectronTranslator_*_*', 
        'keep *_pfPhotonTranslator_*_*', 
        'keep *_particleFlow_electrons_*', 
        'keep *_particleFlow_photons_*', 
        'keep *_trackerDrivenElectronSeeds_preid_*', 
        'keep *_particleFlowPtrs_*_*', 
        'keep *_particleFlowTmpPtrs_*_*', 
        'keep *_offlineBeamSpot_*_*', 
        'keep L1GlobalTriggerReadoutRecord_gtDigis_*_*', 
        'keep *_l1GtRecord_*_*', 
        'keep *_l1GtTriggerMenuLite_*_*', 
        'keep *_conditionsInEdm_*_*', 
        'keep *_l1extraParticles_*_*', 
        'keep *_l1L1GtObjectMap_*_*', 
        'keep L1MuGMTReadoutCollection_gtDigis_*_*', 
        'keep L1GctEmCand*_gctDigis_*_*', 
        'keep L1GctJetCand*_gctDigis_*_*', 
        'keep L1GctEtHad*_gctDigis_*_*', 
        'keep L1GctEtMiss*_gctDigis_*_*', 
        'keep L1GctEtTotal*_gctDigis_*_*', 
        'keep L1GctHtMiss*_gctDigis_*_*', 
        'keep L1GctJetCounts*_gctDigis_*_*', 
        'keep L1GctHFRingEtSums*_gctDigis_*_*', 
        'keep L1GctHFBitCounts*_gctDigis_*_*', 
        'keep LumiDetails_lumiProducer_*_*', 
        'keep LumiSummary_lumiProducer_*_*', 
        'drop *_hlt*_*_*', 
        'keep *_hltL1GtObjectMap_*_*', 
        'keep edmTriggerResults_*_*_*', 
        'keep triggerTriggerEvent_*_*_*', 
        'keep L1AcceptBunchCrossings_*_*_*', 
        'keep L1TriggerScalerss_*_*_*', 
        'keep Level1TriggerScalerss_*_*_*', 
        'keep LumiScalerss_*_*_*', 
        'keep BeamSpotOnlines_*_*_*', 
        'keep DcsStatuss_*_*_*', 
        'keep *_logErrorHarvester_*_*', 
        'keep *_pfIsolatedElectronsEI_*_*', 
        'keep *_pfIsolatedMuonsEI_*_*', 
        'keep recoPFJets_pfJetsEI_*_*', 
        'keep *_pfJetTrackAssociatorEI_*_*', 
        'keep *_impactParameterTagInfosEI_*_*', 
        'keep *_secondaryVertexTagInfosEI_*_*', 
        'keep *_combinedSecondaryVertexBJetTagsEI_*_*', 
        'keep recoPFTaus_pfTausEI_*_*', 
        'keep recoPFTauDiscriminator_pfTausDiscrimination*_*_*', 
        'keep *_pfMetEI_*_*', 
        'keep LHERunInfoProduct_*_*_*', 
        'keep LHEEventProduct_*_*_*', 
        'keep GenRunInfoProduct_generator_*_*', 
        'keep GenLumiInfoProduct_generator_*_*', 
        'keep GenEventInfoProduct_generator_*_*', 
        'keep edmHepMCProduct_generator_*_*', 
        'keep GenFilterInfo_*_*_*', 
        'keep *_genParticles_*_*', 
        'keep recoGenMETs_*_*_*', 
        'keep *_kt4GenJets_*_*', 
        'keep *_kt6GenJets_*_*', 
        'keep *_ak4GenJets_*_*', 
        'keep *_ak5GenJets_*_*', 
        'keep *_ak7GenJets_*_*', 
        'keep *_iterativeCone5GenJets_*_*', 
        'keep *_genParticle_*_*', 
        'keep edmHepMCProduct_source_*_*', 
        'keep SimTracks_g4SimHits_*_*', 
        'keep SimVertexs_g4SimHits_*_*', 
        'keep *_allTrackMCMatch_*_*', 
        'keep StripDigiSimLinkedmDetSetVector_simMuonCSCDigis_*_*', 
        'keep DTLayerIdDTDigiSimLinkMuonDigiCollection_simMuonDTDigis_*_*', 
        'keep RPCDigiSimLinkedmDetSetVector_simMuonRPCDigis_*_*', 
        'keep PileupSummaryInfos_*_*_*' ) )
)

process.REDIGIEventContent = cms.PSet(
    inputCommands = cms.untracked.vstring('drop *', 
        'keep *_g4SimHits_*_*', 
        'keep edmHepMCProduct_source_*_*', 
        'keep *_randomEngineStateProducer_*_*', 
        'keep LHERunInfoProduct_*_*_*', 
        'keep LHEEventProduct_*_*_*', 
        'keep GenRunInfoProduct_generator_*_*', 
        'keep GenLumiInfoProduct_generator_*_*', 
        'keep GenEventInfoProduct_generator_*_*', 
        'keep edmHepMCProduct_generator_*_*', 
        'keep GenFilterInfo_*_*_*', 
        'keep *_genParticles_*_*', 
        'drop *_randomEngineStateProducer_*_*')
)

process.REGENEventContent = cms.PSet(
    inputCommands = cms.untracked.vstring('keep *', 
        'drop *_genParticles_*_*', 
        'drop *_genParticlesForJets_*_*', 
        'drop *_kt4GenJets_*_*', 
        'drop *_kt6GenJets_*_*', 
        'drop *_iterativeCone5GenJets_*_*', 
        'drop *_ak4GenJets_*_*', 
        'drop *_ak5GenJets_*_*', 
        'drop *_ak7GenJets_*_*', 
        'drop *_genCandidatesForMET_*_*', 
        'drop *_genParticlesForMETAllVisible_*_*', 
        'drop *_genMetCalo_*_*', 
        'drop *_genMetCaloAndNonPrompt_*_*', 
        'drop *_genMetTrue_*_*', 
        'drop *_genMetIC5GenJs_*_*')
)

process.REPACKRAWEventContent = cms.PSet(
    splitLevel = cms.untracked.int32(0),
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
    outputCommands = cms.untracked.vstring('drop *', 
        'drop FEDRawDataCollection_*_*_*', 
        'keep FEDRawDataCollection_rawDataRepacker_*_*', 
        'keep FEDRawDataCollection_virginRawDataRepacker_*_*', 
        'keep  FEDRawDataCollection_rawDataCollector_*_*', 
        'keep  FEDRawDataCollection_source_*_*', 
        'drop *_hlt*_*_*', 
        'keep *_hltL1GtObjectMap_*_*', 
        'keep FEDRawDataCollection_rawDataCollector_*_*', 
        'keep FEDRawDataCollection_source_*_*', 
        'keep edmTriggerResults_*_*_*', 
        'keep triggerTriggerEvent_*_*_*', 
        'drop FEDRawDataCollection_source_*_*', 
        'drop FEDRawDataCollection_rawDataCollector_*_*')
)

process.REPACKRAWSIMEventContent = cms.PSet(
    splitLevel = cms.untracked.int32(0),
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
    outputCommands = cms.untracked.vstring('drop *', 
        'drop FEDRawDataCollection_*_*_*', 
        'keep FEDRawDataCollection_rawDataRepacker_*_*', 
        'keep FEDRawDataCollection_virginRawDataRepacker_*_*', 
        'keep  FEDRawDataCollection_rawDataCollector_*_*', 
        'keep  FEDRawDataCollection_source_*_*', 
        'drop *_hlt*_*_*', 
        'keep *_hltL1GtObjectMap_*_*', 
        'keep FEDRawDataCollection_rawDataCollector_*_*', 
        'keep FEDRawDataCollection_source_*_*', 
        'keep edmTriggerResults_*_*_*', 
        'keep triggerTriggerEvent_*_*_*', 
        'keep *_g4SimHits_*_*', 
        'keep edmHepMCProduct_source_*_*', 
        'keep *_allTrackMCMatch_*_*', 
        'keep StripDigiSimLinkedmDetSetVector_simMuonCSCDigis_*_*', 
        'keep CSCDetIdCSCComparatorDigiMuonDigiCollection_simMuonCSCDigis_*_*', 
        'keep DTLayerIdDTDigiSimLinkMuonDigiCollection_simMuonDTDigis_*_*', 
        'keep RPCDigiSimLinkedmDetSetVector_simMuonRPCDigis_*_*', 
        'keep EBSrFlagsSorted_simEcalDigis_*_*', 
        'keep EESrFlagsSorted_simEcalDigis_*_*', 
        'keep CrossingFramePlaybackInfoExtended_*_*_*', 
        'keep PileupSummaryInfos_*_*_*', 
        'keep LHERunInfoProduct_*_*_*', 
        'keep LHEEventProduct_*_*_*', 
        'keep GenRunInfoProduct_generator_*_*', 
        'keep GenLumiInfoProduct_generator_*_*', 
        'keep GenEventInfoProduct_generator_*_*', 
        'keep edmHepMCProduct_generator_*_*', 
        'keep GenFilterInfo_*_*_*', 
        'keep *_genParticles_*_*', 
        'keep recoGenJets_*_*_*', 
        'keep *_genParticle_*_*', 
        'keep recoGenMETs_*_*_*', 
        'keep FEDRawDataCollection_source_*_*', 
        'keep FEDRawDataCollection_rawDataCollector_*_*', 
        'keep *_MEtoEDMConverter_*_*', 
        'keep *_randomEngineStateProducer_*_*', 
        'keep *_logErrorHarvester_*_*', 
        'drop FEDRawDataCollection_source_*_*', 
        'drop FEDRawDataCollection_rawDataCollector_*_*')
)

process.RESIMEventContent = cms.PSet(
    inputCommands = cms.untracked.vstring('drop *', 
        'keep *_randomEngineStateProducer_*_*', 
        'keep LHERunInfoProduct_*_*_*', 
        'keep LHEEventProduct_*_*_*', 
        'keep GenRunInfoProduct_generator_*_*', 
        'keep GenLumiInfoProduct_generator_*_*', 
        'keep GenEventInfoProduct_generator_*_*', 
        'keep edmHepMCProduct_generator_*_*', 
        'keep GenFilterInfo_*_*_*', 
        'keep *_genParticles_*_*')
)

process.RecoBTagAOD = cms.PSet(
    outputCommands = cms.untracked.vstring('keep *_trackCountingHighEffBJetTags_*_*', 
        'keep *_trackCountingHighPurBJetTags_*_*', 
        'keep *_jetProbabilityBJetTags_*_*', 
        'keep *_jetBProbabilityBJetTags_*_*', 
        'keep *_simpleSecondaryVertexBJetTags_*_*', 
        'keep *_simpleSecondaryVertexHighEffBJetTags_*_*', 
        'keep *_simpleSecondaryVertexHighPurBJetTags_*_*', 
        'keep *_combinedSecondaryVertexBJetTags_*_*', 
        'keep *_combinedInclusiveSecondaryVertexV2BJetTags_*_*', 
        'keep *_ghostTrackBJetTags_*_*', 
        'keep *_softPFElectronBJetTags_*_*', 
        'keep *_softPFMuonBJetTags_*_*', 
        'keep *_softMuonBJetTags_*_*', 
        'keep *_softMuonByIP3dBJetTags_*_*', 
        'keep *_softMuonByPtBJetTags_*_*', 
        'keep *_combinedMVABJetTags_*_*', 
        'keep *_pfCombinedSecondaryVertexBJetTags_*_*')
)

process.RecoBTagFEVT = cms.PSet(
    outputCommands = cms.untracked.vstring('keep *_impactParameterTagInfos_*_*', 
        'keep *_trackCountingHighEffBJetTags_*_*', 
        'keep *_trackCountingHighPurBJetTags_*_*', 
        'keep *_jetProbabilityBJetTags_*_*', 
        'keep *_jetBProbabilityBJetTags_*_*', 
        'keep *_secondaryVertexTagInfos_*_*', 
        'keep *_inclusiveSecondaryVertexFinderTagInfos_*_*', 
        'keep *_ghostTrackVertexTagInfos_*_*', 
        'keep *_simpleSecondaryVertexBJetTags_*_*', 
        'keep *_simpleSecondaryVertexHighEffBJetTags_*_*', 
        'keep *_simpleSecondaryVertexHighPurBJetTags_*_*', 
        'keep *_combinedSecondaryVertexBJetTags_*_*', 
        'keep *_combinedInclusiveSecondaryVertexV2BJetTags_*_*', 
        'keep *_ghostTrackBJetTags_*_*', 
        'keep *_softPFMuonsTagInfos_*_*', 
        'keep *_softPFElectronsTagInfos_*_*', 
        'keep *_softPFElectronBJetTags_*_*', 
        'keep *_softPFMuonBJetTags_*_*', 
        'keep *_softMuonTagInfos_*_*', 
        'keep *_softMuonBJetTags_*_*', 
        'keep *_softMuonByIP3dBJetTags_*_*', 
        'keep *_softMuonByPtBJetTags_*_*', 
        'keep *_combinedMVABJetTags_*_*', 
        'keep *_pfImpactParameterTagInfos_*_*', 
        'keep *_pfSecondaryVertexTagInfos_*_*', 
        'keep *_pfCombinedSecondaryVertexBJetTags_*_*')
)

process.RecoBTagRECO = cms.PSet(
    outputCommands = cms.untracked.vstring('keep *_impactParameterTagInfos_*_*', 
        'keep *_trackCountingHighEffBJetTags_*_*', 
        'keep *_trackCountingHighPurBJetTags_*_*', 
        'keep *_jetProbabilityBJetTags_*_*', 
        'keep *_jetBProbabilityBJetTags_*_*', 
        'keep *_secondaryVertexTagInfos_*_*', 
        'keep *_inclusiveSecondaryVertexFinderTagInfos_*_*', 
        'keep *_ghostTrackVertexTagInfos_*_*', 
        'keep *_simpleSecondaryVertexBJetTags_*_*', 
        'keep *_simpleSecondaryVertexHighEffBJetTags_*_*', 
        'keep *_simpleSecondaryVertexHighPurBJetTags_*_*', 
        'keep *_combinedSecondaryVertexBJetTags_*_*', 
        'keep *_combinedInclusiveSecondaryVertexV2BJetTags_*_*', 
        'keep *_ghostTrackBJetTags_*_*', 
        'keep *_softPFMuonsTagInfos_*_*', 
        'keep *_softPFElectronsTagInfos_*_*', 
        'keep *_softPFElectronBJetTags_*_*', 
        'keep *_softPFMuonBJetTags_*_*', 
        'keep *_softMuonTagInfos_*_*', 
        'keep *_softMuonBJetTags_*_*', 
        'keep *_softMuonByIP3dBJetTags_*_*', 
        'keep *_softMuonByPtBJetTags_*_*', 
        'keep *_combinedMVABJetTags_*_*', 
        'keep *_pfImpactParameterTagInfos_*_*', 
        'keep *_pfSecondaryVertexTagInfos_*_*', 
        'keep *_pfCombinedSecondaryVertexBJetTags_*_*')
)

process.RecoBTauAOD = cms.PSet(
    outputCommands = cms.untracked.vstring()
)

process.RecoBTauFEVT = cms.PSet(
    outputCommands = cms.untracked.vstring()
)

process.RecoBTauRECO = cms.PSet(
    outputCommands = cms.untracked.vstring()
)

process.RecoEcalAOD = cms.PSet(
    outputCommands = cms.untracked.vstring('keep *_selectDigi_*_*', 
        'keep EcalRecHitsSorted_reducedEcalRecHitsEB_*_*', 
        'keep EcalRecHitsSorted_reducedEcalRecHitsEE_*_*', 
        'keep EcalRecHitsSorted_reducedEcalRecHitsES_*_*', 
        'keep recoSuperClusters_correctedHybridSuperClusters_*_*', 
        'keep recoCaloClusters_hybridSuperClusters_*_*', 
        'keep recoSuperClusters_hybridSuperClusters_uncleanOnlyHybridSuperClusters_*', 
        'keep recoCaloClusters_multi5x5SuperClusters_multi5x5EndcapBasicClusters_*', 
        'keep recoSuperClusters_correctedMulti5x5SuperClustersWithPreshower_*_*', 
        'keep recoPreshowerClusters_multi5x5SuperClustersWithPreshower_*_*', 
        'keep recoPreshowerClusterShapes_multi5x5PreshowerClusterShape_*_*', 
        'keep recoSuperClusters_particleFlowSuperClusterECAL_*_*', 
        'keep recoCaloClusters_particleFlowSuperClusterECAL_*_*')
)

process.RecoEcalFEVT = cms.PSet(
    outputCommands = cms.untracked.vstring('keep *_selectDigi_*_*', 
        'keep *_reducedEcalRecHitsEB_*_*', 
        'keep *_reducedEcalRecHitsEE_*_*', 
        'keep *_reducedEcalRecHitsES_*_*', 
        'keep *_interestingEcalDetId*_*_*', 
        'keep *_ecalWeightUncalibRecHit_*_*', 
        'keep *_ecalPreshowerRecHit_*_*', 
        'keep *_hybridSuperClusters_*_*', 
        'keep *_correctedHybridSuperClusters_*_*', 
        'keep *_multi5x5*_*_*', 
        'keep *_correctedMulti5x5*_*_*', 
        'keep recoPreshowerClusters_multi5x5SuperClustersWithPreshower_*_*', 
        'keep recoPreshowerClusterShapes_multi5x5PreshowerClusterShape_*_*', 
        'keep *_particleFlowSuperClusterECAL_*_*', 
        'drop recoBasicClusters_multi5x5BasicClusters_multi5x5BarrelBasicClusters_*', 
        'drop recoSuperClusters_multi5x5SuperClusters_multi5x5BarrelSuperClusters_*')
)

process.RecoEcalRECO = cms.PSet(
    outputCommands = cms.untracked.vstring('keep *_selectDigi_*_*', 
        'keep EcalRecHitsSorted_reducedEcalRecHitsEE_*_*', 
        'keep EcalRecHitsSorted_reducedEcalRecHitsEB_*_*', 
        'keep EcalRecHitsSorted_reducedEcalRecHitsES_*_*', 
        'keep *_hybridSuperClusters_*_*', 
        'keep recoSuperClusters_correctedHybridSuperClusters_*_*', 
        'keep *_multi5x5SuperClusters_*_*', 
        'keep recoSuperClusters_multi5x5SuperClusters_*_*', 
        'keep recoSuperClusters_multi5x5SuperClustersWithPreshower_*_*', 
        'keep recoSuperClusters_correctedMulti5x5SuperClustersWithPreshower_*_*', 
        'keep recoPreshowerClusters_multi5x5SuperClustersWithPreshower_*_*', 
        'keep recoPreshowerClusterShapes_multi5x5PreshowerClusterShape_*_*', 
        'keep *_particleFlowSuperClusterECAL_*_*', 
        'drop recoClusterShapes_*_*_*', 
        'drop recoBasicClustersToOnerecoClusterShapesAssociation_*_*_*', 
        'drop recoBasicClusters_multi5x5BasicClusters_multi5x5BarrelBasicClusters_*', 
        'drop recoSuperClusters_multi5x5SuperClusters_multi5x5BarrelSuperClusters_*')
)

process.RecoEgammaAOD = cms.PSet(
    outputCommands = cms.untracked.vstring('keep recoGsfElectronCores_gsfElectronCores_*_*', 
        'keep recoGsfElectrons_gsfElectrons_*_*', 
        'keep recoGsfElectronCores_uncleanedOnlyGsfElectronCores_*_*', 
        'keep recoGsfElectrons_uncleanedOnlyGsfElectrons_*_*', 
        'keep floatedmValueMap_eidRobustLoose_*_*', 
        'keep floatedmValueMap_eidRobustTight_*_*', 
        'keep floatedmValueMap_eidRobustHighEnergy_*_*', 
        'keep floatedmValueMap_eidLoose_*_*', 
        'keep floatedmValueMap_eidTight_*_*', 
        'keep *_egmGedGsfElectronPFIsolation_*_*', 
        'keep *_egmGsfElectronIDs_*_*', 
        'keep recoPhotonCores_gedPhotonCore_*_*', 
        'keep recoPhotons_gedPhotons_*_*', 
        'keep *_particleBasedIsolation_*_*', 
        'drop *_gedPhotons_valMapPFEgammaCandToPhoton_*', 
        'keep recoPhotonCores_photonCore_*_*', 
        'keep recoPhotons_photons_*_*', 
        'keep recoConversions_conversions_*_*', 
        'keep recoConversions_mustacheConversions_*_*', 
        'drop *_conversions_uncleanedConversions_*', 
        'keep recoConversions_allConversions_*_*', 
        'keep recoConversions_allConversionsOldEG_*_*', 
        'keep recoTracks_ckfOutInTracksFrom*Conversions_*_*', 
        'keep recoTracks_ckfInOutTracksFrom*Conversions_*_*', 
        'keep recoConversions_uncleanedOnlyAllConversions_*_*', 
        'keep recoTracks_uncleanedOnlyCkfOutInTracksFromConversions_*_*', 
        'keep recoTracks_uncleanedOnlyCkfInOutTracksFromConversions_*_*', 
        'keep *_PhotonIDProd_*_*', 
        'keep *_PhotonIDProdGED_*_*', 
        'keep *_hfRecoEcalCandidate_*_*', 
        'keep *_hfEMClusters_*_*', 
        'keep *_gedGsfElectronCores_*_*', 
        'keep *_gedGsfElectrons_*_*')
)

process.RecoEgammaFEVT = cms.PSet(
    outputCommands = cms.untracked.vstring('keep *_gsfElectronCores_*_*', 
        'keep *_gsfElectrons_*_*', 
        'keep *_uncleanedOnlyGsfElectronCores_*_*', 
        'keep *_uncleanedOnlyGsfElectrons_*_*', 
        'keep *_eidRobustLoose_*_*', 
        'keep *_eidRobustTight_*_*', 
        'keep *_eidRobustHighEnergy_*_*', 
        'keep *_eidLoose_*_*', 
        'keep *_eidTight_*_*', 
        'keep *_egmGedGsfElectronPF*Isolation_*_*', 
        'keep *_egmGsfElectronIDs_*_*', 
        'keep *_conversions_*_*', 
        'keep *_mustacheConversions_*_*', 
        'drop *_conversions_uncleanedConversions_*', 
        'keep *_gedPhotonCore_*_*', 
        'keep *_gedPhotonsTmp_*_*', 
        'keep *_gedPhotons_*_*', 
        'keep *_particleBasedIsolation_*_*', 
        'keep *_photonCore_*_*', 
        'keep *_photons_*_*', 
        'keep *_mustachePhotonCore_*_*', 
        'keep *_mustachePhotons_*_*', 
        'keep *_allConversions_*_*', 
        'keep *_allConversionsOldEG_*_*', 
        'keep *_ckfOutInTracksFrom*Conversions_*_*', 
        'keep *_ckfInOutTracksFrom*Conversions_*_*', 
        'keep *_uncleanedOnlyAllConversions_*_*', 
        'keep *_uncleanedOnlyCkfOutInTracksFromConversions_*_*', 
        'keep *_uncleanedOnlyCkfInOutTracksFromConversions_*_*', 
        'keep *_PhotonIDProd_*_*', 
        'keep *_PhotonIDProdGED_*_*', 
        'keep *_hfRecoEcalCandidate_*_*', 
        'keep *_hfEMClusters_*_*', 
        'keep *_gedGsfElectronCores_*_*', 
        'keep *_gedGsfElectrons_*_*')
)

process.RecoEgammaRECO = cms.PSet(
    outputCommands = cms.untracked.vstring('keep recoGsfElectronCores_gsfElectronCores_*_*', 
        'keep recoGsfElectrons_gsfElectrons_*_*', 
        'keep recoGsfElectronCores_uncleanedOnlyGsfElectronCores_*_*', 
        'keep recoGsfElectrons_uncleanedOnlyGsfElectrons_*_*', 
        'keep floatedmValueMap_eidRobustLoose_*_*', 
        'keep floatedmValueMap_eidRobustTight_*_*', 
        'keep floatedmValueMap_eidRobustHighEnergy_*_*', 
        'keep floatedmValueMap_eidLoose_*_*', 
        'keep floatedmValueMap_eidTight_*_*', 
        'keep *_egmGedGsfElectronPFIsolation_*_*', 
        'keep *_egmGsfElectronIDs_*_*', 
        'keep *_gedPhotonCore_*_*', 
        'keep *_gedPhotons_*_*', 
        'keep *_particleBasedIsolation_*_*', 
        'keep recoPhotons_mustachePhotons_*_*', 
        'keep recoPhotonCores_mustachePhotonCore_*_*', 
        'keep recoPhotons_photons_*_*', 
        'keep recoPhotonCores_photonCore_*_*', 
        'keep recoConversions_conversions_*_*', 
        'keep recoConversions_mustacheConversions_*_*', 
        'drop *_conversions_uncleanedConversions_*', 
        'drop *_gedPhotonsTmp_valMapPFEgammaCandToPhoton_*', 
        'keep recoConversions_allConversions_*_*', 
        'keep recoConversions_allConversionsOldEG_*_*', 
        'keep recoTracks_ckfOutInTracksFrom*Conversions_*_*', 
        'keep recoTracks_ckfInOutTracksFrom*Conversions_*_*', 
        'keep recoTrackExtras_ckfOutInTracksFrom*Conversions_*_*', 
        'keep recoTrackExtras_ckfInOutTracksFrom*Conversions_*_*', 
        'keep TrackingRecHitsOwned_ckfOutInTracksFrom*Conversions_*_*', 
        'keep TrackingRecHitsOwned_ckfInOutTracksFrom*Conversions_*_*', 
        'keep recoConversions_uncleanedOnlyAllConversions_*_*', 
        'keep recoTracks_uncleanedOnlyCkfOutInTracksFromConversions_*_*', 
        'keep recoTracks_uncleanedOnlyCkfInOutTracksFromConversions_*_*', 
        'keep recoTrackExtras_uncleanedOnlyCkfOutInTracksFromConversions_*_*', 
        'keep recoTrackExtras_uncleanedOnlyCkfInOutTracksFromConversions_*_*', 
        'keep TrackingRecHitsOwned_uncleanedOnlyCkfOutInTracksFromConversions_*_*', 
        'keep TrackingRecHitsOwned_uncleanedOnlyCkfInOutTracksFromConversions_*_*', 
        'keep *_PhotonIDProd_*_*', 
        'keep *_PhotonIDProdGED_*_*', 
        'keep *_hfRecoEcalCandidate_*_*', 
        'keep *_hfEMClusters_*_*', 
        'keep *_gedGsfElectronCores_*_*', 
        'keep *_gedGsfElectrons_*_*')
)

process.RecoGenJetsAOD = cms.PSet(
    outputCommands = cms.untracked.vstring('keep *_kt4GenJets_*_*', 
        'keep *_kt6GenJets_*_*', 
        'keep *_ak4GenJets_*_*', 
        'keep *_ak5GenJets_*_*', 
        'keep *_ak8GenJets_*_*', 
        'keep *_genParticle_*_*')
)

process.RecoGenJetsFEVT = cms.PSet(
    outputCommands = cms.untracked.vstring('keep recoGenJets_*_*_*', 
        'keep *_genParticle_*_*')
)

process.RecoGenJetsRECO = cms.PSet(
    outputCommands = cms.untracked.vstring('keep *_kt4GenJets_*_*', 
        'keep *_kt6GenJets_*_*', 
        'keep *_ak4GenJets_*_*', 
        'keep *_ak5GenJets_*_*', 
        'keep *_ak7GenJets_*_*', 
        'keep *_iterativeCone5GenJets_*_*', 
        'keep *_genParticle_*_*')
)

process.RecoGenMETAOD = cms.PSet(
    outputCommands = cms.untracked.vstring('keep recoGenMETs_*_*_*')
)

process.RecoGenMETFEVT = cms.PSet(
    outputCommands = cms.untracked.vstring('keep recoGenMETs_*_*_*')
)

process.RecoGenMETRECO = cms.PSet(
    outputCommands = cms.untracked.vstring('keep recoGenMETs_*_*_*')
)

process.RecoHcalNoiseAOD = cms.PSet(
    outputCommands = cms.untracked.vstring('drop recoHcalNoiseRBXs_hcalnoise_*_*', 
        'keep HcalNoiseSummary_hcalnoise_*_*')
)

process.RecoHcalNoiseFEVT = cms.PSet(
    outputCommands = cms.untracked.vstring('keep recoHcalNoiseRBXs_hcalnoise_*_*', 
        'keep HcalNoiseSummary_hcalnoise_*_*')
)

process.RecoHcalNoiseRECO = cms.PSet(
    outputCommands = cms.untracked.vstring('keep recoHcalNoiseRBXs_hcalnoise_*_*', 
        'keep HcalNoiseSummary_hcalnoise_*_*')
)

process.RecoJetsAOD = cms.PSet(
    outputCommands = cms.untracked.vstring('keep *_ak4CaloJets_*_*', 
        'keep *_ak5CaloJets_*_*', 
        'keep *_ak4PFJetsCHS_*_*', 
        'keep *_ak5PFJetsCHS_*_*', 
        'keep *_ak8PFJetsCHS_*_*', 
        'keep *_ca8PFJetsCHS_*_*', 
        'keep *_ca8PFJetsCHSPruned_*_*', 
        'keep *_cmsTopTagPFJetsCHS_*_*', 
        'keep double_kt6PFJets_rho_*', 
        'keep *_ak4PFJets_*_*', 
        'keep *_ak5PFJets_*_*', 
        'keep *_ak8PFJets_*_*', 
        'keep *_JetPlusTrackZSPCorJetAntiKt4_*_*', 
        'keep *_ak4TrackJets_*_*', 
        'keep recoRecoChargedRefCandidates_trackRefsForJets_*_*', 
        'keep *_caloTowers_*_*', 
        'keep *_CastorTowerReco_*_*', 
        'keep *_ak4JetTracksAssociatorAtVertex_*_*', 
        'keep *_ak4JetTracksAssociatorAtVertexPF_*_*', 
        'keep *_ak4JetTracksAssociatorExplicit_*_*', 
        'keep *_ak4JetExtender_*_*', 
        'keep *_ak4JetID_*_*', 
        'keep double_kt6CaloJetsCentral_*_*', 
        'keep double_kt6PFJetsCentralChargedPileUp_*_*', 
        'keep double_kt6PFJetsCentralNeutral_*_*', 
        'keep double_kt6PFJetsCentralNeutralTight_*_*', 
        'keep *_fixedGridRho*_*_*', 
        'drop doubles_*Jets_rhos_*', 
        'drop doubles_*Jets_sigmas_*', 
        'keep *_ca8PFJetsCHSPrunedLinks_*_*')
)

process.RecoJetsFEVT = cms.PSet(
    outputCommands = cms.untracked.vstring('keep recoCaloJets_*_*_*', 
        'keep recoPFJets_*_*_*', 
        'keep recoTrackJets_*_*_*', 
        'keep recoJPTJets_*_*_*', 
        'keep recoBasicJets_*_*_*', 
        'keep *_caloTowers_*_*', 
        'keep *_towerMaker_*_*', 
        'keep *_CastorTowerReco_*_*', 
        'keep recoRecoChargedRefCandidates_trackRefsForJets_*_*', 
        'keep *_kt4JetTracksAssociatorAtVertex_*_*', 
        'keep *_kt4JetTracksAssociatorAtCaloFace_*_*', 
        'keep *_kt4JetExtender_*_*', 
        'keep *_ak4JetTracksAssociatorAtVertex*_*_*', 
        'keep *_ak4JetTracksAssociatorAtCaloFace*_*_*', 
        'keep *_ak4JetExtender_*_*', 
        'keep *_ak4JetTracksAssociatorExplicit_*_*', 
        'keep *_ak7JetTracksAssociatorAtVertex*_*_*', 
        'keep *_ak7JetTracksAssociatorAtCaloFace*_*_*', 
        'keep *_ak7JetExtender_*_*', 
        'keep *_*JetID_*_*', 
        'keep *_kt4CaloJets_*_*', 
        'keep *_kt6CaloJets_*_*', 
        'keep *_ak4CaloJets_*_*', 
        'keep *_ak5CaloJets_*_*', 
        'keep *_ak7CaloJets_*_*', 
        'keep *_kt4PFJets_*_*', 
        'keep *_kt6PFJets_*_*', 
        'keep *_ak4PFJets_*_*', 
        'keep *_ak5PFJets_*_*', 
        'keep *_ak7PFJets_*_*', 
        'keep *_JetPlusTrackZSPCorJetAntiKt4_*_*', 
        'keep *_ak4TrackJets_*_*', 
        'keep *_kt4TrackJets_*_*', 
        'keep *_ak7BasicJets_*_*', 
        'keep *_ak7CastorJetID_*_*', 
        'keep *_fixedGridRho*_*_*', 
        'keep *_ca*Links_*_*', 
        'keep *_ak*Links_*_*')
)

process.RecoJetsRECO = cms.PSet(
    outputCommands = cms.untracked.vstring('keep *_kt6CaloJets_*_*', 
        'keep *_ak4CaloJets_*_*', 
        'keep *_ak5CaloJets_*_*', 
        'keep double_kt6PFJets_rho_*', 
        'keep *_ak4PFJets_*_*', 
        'keep *_ak5PFJets_*_*', 
        'keep *_ak8PFJets_*_*', 
        'keep *_ak4PFJetsCHS_*_*', 
        'keep *_ak5PFJetsCHS_*_*', 
        'keep *_ak8PFJetsCHS_*_*', 
        'keep *_ca8PFJetsCHS_*_*', 
        'keep *_ca8PFJetsCHSPruned_*_*', 
        'keep *_cmsTopTagPFJetsCHS_*_*', 
        'keep *_JetPlusTrackZSPCorJetAntiKt4_*_*', 
        'keep *_ak4TrackJets_*_*', 
        'keep recoRecoChargedRefCandidates_trackRefsForJets_*_*', 
        'keep *_caloTowers_*_*', 
        'keep *_towerMaker_*_*', 
        'keep *_CastorTowerReco_*_*', 
        'keep *_kt4JetTracksAssociatorAtVertex_*_*', 
        'keep *_kt4JetTracksAssociatorAtCaloFace_*_*', 
        'keep *_kt4JetExtender_*_*', 
        'keep *_ak4JetTracksAssociatorAtVertex_*_*', 
        'keep *_ak4JetTracksAssociatorAtVertexPF_*_*', 
        'keep *_ak4JetTracksAssociatorAtCaloFace_*_*', 
        'keep *_ak4JetTracksAssociatorExplicit_*_*', 
        'keep *_ak4JetExtender_*_*', 
        'keep *_ak7JetTracksAssociatorAtVertex*_*_*', 
        'keep *_ak7JetTracksAssociatorAtCaloFace*_*_*', 
        'keep *_ak7JetExtender_*_*', 
        'keep *_ak4JetID_*_*', 
        'keep *_ak5JetID_*_*', 
        'keep *_ak7JetID_*_*', 
        'keep *_ic5JetID_*_*', 
        'keep *_kt4JetID_*_*', 
        'keep *_kt6JetID_*_*', 
        'keep *_ak7BasicJets_*_*', 
        'keep *_ak7CastorJetID_*_*', 
        'keep double_kt6CaloJetsCentral_*_*', 
        'keep double_kt6PFJetsCentralChargedPileUp_*_*', 
        'keep double_kt6PFJetsCentralNeutral_*_*', 
        'keep double_kt6PFJetsCentralNeutralTight_*_*', 
        'keep *_fixedGridRho*_*_*', 
        'keep *_ca8PFJetsCHSPrunedLinks_*_*')
)

process.RecoLocalCaloAOD = cms.PSet(
    outputCommands = cms.untracked.vstring('keep *_castorreco_*_*', 
        'keep *_reducedHcalRecHits_*_*', 
        'keep HcalUnpackerReport_*_*_*')
)

process.RecoLocalCaloFEVT = cms.PSet(
    outputCommands = cms.untracked.vstring('keep *_hbhereco_*_*', 
        'keep *_hbheprereco_*_*', 
        'keep *_hfreco_*_*', 
        'keep *_horeco_*_*', 
        'keep HBHERecHitsSorted_hbherecoMB_*_*', 
        'keep HBHERecHitsSorted_hbheprerecoMB_*_*', 
        'keep HORecHitsSorted_horecoMB_*_*', 
        'keep HFRecHitsSorted_hfrecoMB_*_*', 
        'keep ZDCDataFramesSorted_*Digis_*_*', 
        'keep ZDCRecHitsSorted_*_*_*', 
        'keep *_reducedHcalRecHits_*_*', 
        'keep *_castorreco_*_*', 
        'keep HcalUnpackerReport_*_*_*', 
        'keep *_ecalMultiFitUncalibRecHit_*_*', 
        'keep *_ecalPreshowerRecHit_*_*', 
        'keep *_ecalRecHit_*_*')
)

process.RecoLocalCaloRECO = cms.PSet(
    outputCommands = cms.untracked.vstring('keep *_hbhereco_*_*', 
        'keep *_hbheprereco_*_*', 
        'keep *_hfreco_*_*', 
        'keep *_horeco_*_*', 
        'keep HBHERecHitsSorted_hbherecoMB_*_*', 
        'keep HORecHitsSorted_horecoMB_*_*', 
        'keep HFRecHitsSorted_hfrecoMB_*_*', 
        'keep ZDCDataFramesSorted_*Digis_*_*', 
        'keep ZDCRecHitsSorted_*_*_*', 
        'keep *_reducedHcalRecHits_*_*', 
        'keep *_castorreco_*_*', 
        'keep HcalUnpackerReport_*_*_*', 
        'keep *_ecalPreshowerRecHit_*_*', 
        'keep *_ecalRecHit_*_*', 
        'keep *_ecalCompactTrigPrim_*_*', 
        'keep *_ecalTPSkim_*_*')
)

process.RecoLocalMuonAOD = cms.PSet(
    outputCommands = cms.untracked.vstring()
)

process.RecoLocalMuonFEVT = cms.PSet(
    outputCommands = cms.untracked.vstring('keep *_dt1DRecHits_*_*', 
        'keep *_dt4DSegments_*_*', 
        'keep *_csc2DRecHits_*_*', 
        'keep *_cscSegments_*_*', 
        'keep *_rpcRecHits_*_*')
)

process.RecoLocalMuonRECO = cms.PSet(
    outputCommands = cms.untracked.vstring('keep *_dt1DRecHits_*_*', 
        'keep *_dt4DSegments_*_*', 
        'keep *_dt1DCosmicRecHits_*_*', 
        'keep *_dt4DCosmicSegments_*_*', 
        'keep *_csc2DRecHits_*_*', 
        'keep *_cscSegments_*_*', 
        'keep *_rpcRecHits_*_*')
)

process.RecoLocalTrackerAOD = cms.PSet(
    outputCommands = cms.untracked.vstring('keep *_clusterSummaryProducer_*_*')
)

process.RecoLocalTrackerFEVT = cms.PSet(
    outputCommands = cms.untracked.vstring('keep DetIdedmEDCollection_siStripDigis_*_*', 
        'keep DetIdedmEDCollection_siPixelDigis_*_*', 
        'keep *_siPixelClusters_*_*', 
        'keep *_siStripClusters_*_*', 
        'keep *_clusterSummaryProducer_*_*')
)

process.RecoLocalTrackerRECO = cms.PSet(
    outputCommands = cms.untracked.vstring('keep DetIdedmEDCollection_siStripDigis_*_*', 
        'keep DetIdedmEDCollection_siPixelDigis_*_*', 
        'keep *_siPixelClusters_*_*', 
        'keep *_siStripClusters_*_*', 
        'keep *_clusterSummaryProducer_*_*')
)

process.RecoMETAOD = cms.PSet(
    outputCommands = cms.untracked.vstring('keep recoCaloMETs_caloMet_*_*', 
        'keep recoCaloMETs_caloMetBE_*_*', 
        'keep recoCaloMETs_caloMetBEFO_*_*', 
        'keep recoCaloMETs_caloMetM_*_*', 
        'keep recoPFMETs_pfMet_*_*', 
        'keep recoPFMETs_pfChMet_*_*', 
        'keep recoMuonMETCorrectionDataedmValueMap_muonMETValueMapProducer_*_*', 
        'drop recoHcalNoiseRBXs_*_*_*', 
        'keep HcalNoiseSummary_hcalnoise_*_*', 
        'keep *GlobalHaloData_*_*_*', 
        'keep *BeamHaloSummary_BeamHaloSummary_*_*')
)

process.RecoMETFEVT = cms.PSet(
    outputCommands = cms.untracked.vstring('keep recoCaloMETs_caloMet_*_*', 
        'keep recoCaloMETs_caloMetBE_*_*', 
        'keep recoCaloMETs_caloMetBEFO_*_*', 
        'keep recoCaloMETs_caloMetM_*_*', 
        'keep recoPFMETs_pfMet_*_*', 
        'keep recoPFMETs_pfChMet_*_*', 
        'keep recoMuonMETCorrectionDataedmValueMap_muonMETValueMapProducer_*_*', 
        'keep recoHcalNoiseRBXs_hcalnoise_*_*', 
        'keep HcalNoiseSummary_hcalnoise_*_*', 
        'keep *HaloData_*_*_*', 
        'keep *BeamHaloSummary_BeamHaloSummary_*_*')
)

process.RecoMETRECO = cms.PSet(
    outputCommands = cms.untracked.vstring('keep recoCaloMETs_caloMet_*_*', 
        'keep recoCaloMETs_caloMetBE_*_*', 
        'keep recoCaloMETs_caloMetBEFO_*_*', 
        'keep recoCaloMETs_caloMetM_*_*', 
        'keep recoPFMETs_pfMet_*_*', 
        'keep recoPFMETs_pfChMet_*_*', 
        'keep recoMuonMETCorrectionDataedmValueMap_muonMETValueMapProducer_*_*', 
        'keep recoHcalNoiseRBXs_hcalnoise_*_*', 
        'keep HcalNoiseSummary_hcalnoise_*_*', 
        'keep *HaloData_*_*_*', 
        'keep *BeamHaloSummary_BeamHaloSummary_*_*')
)

process.RecoMuonAOD = cms.PSet(
    outputCommands = cms.untracked.vstring('keep *_muons_*_*', 
        'keep *_*_muons_*', 
        'drop *_muons_muons1stStep2muonsMap_*', 
        'drop recoIsoDepositedmValueMap_muons_*_*', 
        'drop doubleedmValueMap_muons_muPFIso*_*', 
        'keep recoTracks_standAloneMuons_*_*', 
        'keep recoTrackExtras_standAloneMuons_*_*', 
        'keep TrackingRecHitsOwned_standAloneMuons_*_*', 
        'keep recoTracks_globalMuons_*_*', 
        'keep recoTrackExtras_globalMuons_*_*', 
        'keep recoTracks_tevMuons_*_*', 
        'keep recoTrackExtras_tevMuons_*_*', 
        'keep recoTracks_generalTracks_*_*', 
        'keep recoTracksToOnerecoTracksAssociation_tevMuons_*_*', 
        'keep recoTracks_cosmicMuons_*_*', 
        'keep recoTracks_globalCosmicMuons_*_*', 
        'keep recoMuons_muonsFromCosmics_*_*', 
        'keep recoTracks_cosmicMuons1Leg_*_*', 
        'keep recoTracks_globalCosmicMuons1Leg_*_*', 
        'keep recoMuons_muonsFromCosmics1Leg_*_*', 
        'keep recoTracks_refittedStandAloneMuons_*_*', 
        'keep recoTrackExtras_refittedStandAloneMuons_*_*', 
        'keep TrackingRecHitsOwned_refittedStandAloneMuons_*_*', 
        'keep recoTracks_displacedStandAloneMuons__*', 
        'keep recoTrackExtras_displacedStandAloneMuons_*_*', 
        'keep TrackingRecHitsOwned_displacedStandAloneMuons_*_*')
)

process.RecoMuonFEVT = cms.PSet(
    outputCommands = cms.untracked.vstring('keep *_MuonSeed_*_*', 
        'keep *_ancientMuonSeed_*_*', 
        'keep *_displacedMuonSeeds_*_*', 
        'keep *_mergedStandAloneMuonSeeds_*_*', 
        'keep TrackingRecHitsOwned_globalMuons_*_*', 
        'keep TrackingRecHitsOwned_tevMuons_*_*', 
        'keep recoCaloMuons_calomuons_*_*', 
        'keep *_CosmicMuonSeed_*_*', 
        'keep recoTrackExtras_cosmicMuons_*_*', 
        'keep TrackingRecHitsOwned_cosmicMuons_*_*', 
        'keep recoTrackExtras_globalCosmicMuons_*_*', 
        'keep TrackingRecHitsOwned_globalCosmicMuons_*_*', 
        'keep recoTrackExtras_cosmicMuons1Leg_*_*', 
        'keep TrackingRecHitsOwned_cosmicMuons1Leg_*_*', 
        'keep recoTrackExtras_globalCosmicMuons1Leg_*_*', 
        'keep TrackingRecHitsOwned_globalCosmicMuons1Leg_*_*', 
        'keep recoTracks_cosmicsVetoTracks_*_*', 
        'keep *_SETMuonSeed_*_*', 
        'keep recoTracks_standAloneSETMuons_*_*', 
        'keep recoTrackExtras_standAloneSETMuons_*_*', 
        'keep TrackingRecHitsOwned_standAloneSETMuons_*_*', 
        'keep recoTracks_globalSETMuons_*_*', 
        'keep recoTrackExtras_globalSETMuons_*_*', 
        'keep TrackingRecHitsOwned_globalSETMuons_*_*', 
        'keep recoMuons_muonsWithSET_*_*', 
        'keep *_muons_*_*', 
        'keep *_*_muons_*', 
        'drop *_muons_muons1stStep2muonsMap_*', 
        'drop recoIsoDepositedmValueMap_muons_*_*', 
        'drop doubleedmValueMap_muons_muPFIso*_*', 
        'keep recoTracks_standAloneMuons_*_*', 
        'keep recoTrackExtras_standAloneMuons_*_*', 
        'keep TrackingRecHitsOwned_standAloneMuons_*_*', 
        'keep recoTracks_globalMuons_*_*', 
        'keep recoTrackExtras_globalMuons_*_*', 
        'keep recoTracks_tevMuons_*_*', 
        'keep recoTrackExtras_tevMuons_*_*', 
        'keep recoTracks_generalTracks_*_*', 
        'keep recoTracksToOnerecoTracksAssociation_tevMuons_*_*', 
        'keep recoTracks_cosmicMuons_*_*', 
        'keep recoTracks_globalCosmicMuons_*_*', 
        'keep recoMuons_muonsFromCosmics_*_*', 
        'keep recoTracks_cosmicMuons1Leg_*_*', 
        'keep recoTracks_globalCosmicMuons1Leg_*_*', 
        'keep recoMuons_muonsFromCosmics1Leg_*_*', 
        'keep recoTracks_refittedStandAloneMuons_*_*', 
        'keep recoTrackExtras_refittedStandAloneMuons_*_*', 
        'keep TrackingRecHitsOwned_refittedStandAloneMuons_*_*', 
        'keep recoTracks_displacedStandAloneMuons__*', 
        'keep recoTrackExtras_displacedStandAloneMuons_*_*', 
        'keep TrackingRecHitsOwned_displacedStandAloneMuons_*_*', 
        'keep *_muIsoDepositTk_*_*', 
        'keep *_muIsoDepositCalByAssociatorTowers_*_*', 
        'keep *_muIsoDepositCalByAssociatorHits_*_*', 
        'keep *_muIsoDepositJets_*_*', 
        'keep *_muGlobalIsoDepositCtfTk_*_*', 
        'keep *_muGlobalIsoDepositCalByAssociatorTowers_*_*', 
        'keep *_muGlobalIsoDepositCalByAssociatorHits_*_*', 
        'keep *_muGlobalIsoDepositJets_*_*')
)

process.RecoMuonIsolationAOD = cms.PSet(
    outputCommands = cms.untracked.vstring()
)

process.RecoMuonIsolationFEVT = cms.PSet(
    outputCommands = cms.untracked.vstring('keep *_muIsoDepositTk_*_*', 
        'keep *_muIsoDepositCalByAssociatorTowers_*_*', 
        'keep *_muIsoDepositCalByAssociatorHits_*_*', 
        'keep *_muIsoDepositJets_*_*', 
        'keep *_muGlobalIsoDepositCtfTk_*_*', 
        'keep *_muGlobalIsoDepositCalByAssociatorTowers_*_*', 
        'keep *_muGlobalIsoDepositCalByAssociatorHits_*_*', 
        'keep *_muGlobalIsoDepositJets_*_*')
)

process.RecoMuonIsolationParamGlobal = cms.PSet(
    outputCommands = cms.untracked.vstring('keep *_muParamGlobalIsoDepositGsTk_*_*', 
        'keep *_muParamGlobalIsoDepositCalEcal_*_*', 
        'keep *_muParamGlobalIsoDepositCalHcal_*_*', 
        'keep *_muParamGlobalIsoDepositCtfTk_*_*', 
        'keep *_muParamGlobalIsoDepositCalByAssociatorTowers_*_*', 
        'keep *_muParamGlobalIsoDepositCalByAssociatorHits_*_*', 
        'keep *_muParamGlobalIsoDepositJets_*_*')
)

process.RecoMuonIsolationRECO = cms.PSet(
    outputCommands = cms.untracked.vstring('keep *_muIsoDepositTk_*_*', 
        'keep *_muIsoDepositCalByAssociatorTowers_*_*', 
        'keep *_muIsoDepositCalByAssociatorHits_*_*', 
        'keep *_muIsoDepositJets_*_*', 
        'keep *_muGlobalIsoDepositCtfTk_*_*', 
        'keep *_muGlobalIsoDepositCalByAssociatorTowers_*_*', 
        'keep *_muGlobalIsoDepositCalByAssociatorHits_*_*', 
        'keep *_muGlobalIsoDepositJets_*_*')
)

process.RecoMuonRECO = cms.PSet(
    outputCommands = cms.untracked.vstring('keep *_MuonSeed_*_*', 
        'keep *_ancientMuonSeed_*_*', 
        'keep *_displacedMuonSeeds_*_*', 
        'keep *_mergedStandAloneMuonSeeds_*_*', 
        'keep TrackingRecHitsOwned_globalMuons_*_*', 
        'keep TrackingRecHitsOwned_tevMuons_*_*', 
        'keep recoCaloMuons_calomuons_*_*', 
        'keep *_CosmicMuonSeed_*_*', 
        'keep recoTrackExtras_cosmicMuons_*_*', 
        'keep TrackingRecHitsOwned_cosmicMuons_*_*', 
        'keep recoTrackExtras_globalCosmicMuons_*_*', 
        'keep TrackingRecHitsOwned_globalCosmicMuons_*_*', 
        'keep recoTrackExtras_cosmicMuons1Leg_*_*', 
        'keep TrackingRecHitsOwned_cosmicMuons1Leg_*_*', 
        'keep recoTrackExtras_globalCosmicMuons1Leg_*_*', 
        'keep TrackingRecHitsOwned_globalCosmicMuons1Leg_*_*', 
        'keep recoTracks_cosmicsVetoTracks_*_*', 
        'keep *_SETMuonSeed_*_*', 
        'keep recoTracks_standAloneSETMuons_*_*', 
        'keep recoTrackExtras_standAloneSETMuons_*_*', 
        'keep TrackingRecHitsOwned_standAloneSETMuons_*_*', 
        'keep recoTracks_globalSETMuons_*_*', 
        'keep recoTrackExtras_globalSETMuons_*_*', 
        'keep TrackingRecHitsOwned_globalSETMuons_*_*', 
        'keep recoMuons_muonsWithSET_*_*', 
        'keep *_muons_*_*', 
        'keep *_*_muons_*', 
        'drop *_muons_muons1stStep2muonsMap_*', 
        'drop recoIsoDepositedmValueMap_muons_*_*', 
        'drop doubleedmValueMap_muons_muPFIso*_*', 
        'keep recoTracks_standAloneMuons_*_*', 
        'keep recoTrackExtras_standAloneMuons_*_*', 
        'keep TrackingRecHitsOwned_standAloneMuons_*_*', 
        'keep recoTracks_globalMuons_*_*', 
        'keep recoTrackExtras_globalMuons_*_*', 
        'keep recoTracks_tevMuons_*_*', 
        'keep recoTrackExtras_tevMuons_*_*', 
        'keep recoTracks_generalTracks_*_*', 
        'keep recoTracksToOnerecoTracksAssociation_tevMuons_*_*', 
        'keep recoTracks_cosmicMuons_*_*', 
        'keep recoTracks_globalCosmicMuons_*_*', 
        'keep recoMuons_muonsFromCosmics_*_*', 
        'keep recoTracks_cosmicMuons1Leg_*_*', 
        'keep recoTracks_globalCosmicMuons1Leg_*_*', 
        'keep recoMuons_muonsFromCosmics1Leg_*_*', 
        'keep recoTracks_refittedStandAloneMuons_*_*', 
        'keep recoTrackExtras_refittedStandAloneMuons_*_*', 
        'keep TrackingRecHitsOwned_refittedStandAloneMuons_*_*', 
        'keep recoTracks_displacedStandAloneMuons__*', 
        'keep recoTrackExtras_displacedStandAloneMuons_*_*', 
        'keep TrackingRecHitsOwned_displacedStandAloneMuons_*_*', 
        'keep *_muIsoDepositTk_*_*', 
        'keep *_muIsoDepositCalByAssociatorTowers_*_*', 
        'keep *_muIsoDepositCalByAssociatorHits_*_*', 
        'keep *_muIsoDepositJets_*_*', 
        'keep *_muGlobalIsoDepositCtfTk_*_*', 
        'keep *_muGlobalIsoDepositCalByAssociatorTowers_*_*', 
        'keep *_muGlobalIsoDepositCalByAssociatorHits_*_*', 
        'keep *_muGlobalIsoDepositJets_*_*')
)

process.RecoParticleFlowAOD = cms.PSet(
    outputCommands = cms.untracked.vstring('drop CaloTowersSorted_towerMakerPF_*_*', 
        'drop *_pfElectronTranslator_*_*', 
        'keep recoPFRecHits_particleFlowClusterECAL_Cleaned_*', 
        'keep recoPFRecHits_particleFlowClusterHCAL_Cleaned_*', 
        'keep recoPFRecHits_particleFlowClusterHO_Cleaned_*', 
        'keep recoPFRecHits_particleFlowClusterHFEM_Cleaned_*', 
        'keep recoPFRecHits_particleFlowClusterHFHAD_Cleaned_*', 
        'keep recoPFRecHits_particleFlowClusterPS_Cleaned_*', 
        'keep recoPFRecHits_particleFlowRecHitECAL_Cleaned_*', 
        'keep recoPFRecHits_particleFlowRecHitHCAL_Cleaned_*', 
        'keep recoPFRecHits_particleFlowRecHitHO_Cleaned_*', 
        'keep recoPFRecHits_particleFlowRecHitPS_Cleaned_*', 
        'keep recoCaloClusters_particleFlowEGamma_*_*', 
        'keep recoSuperClusters_particleFlowEGamma_*_*', 
        'keep recoConversions_particleFlowEGamma_*_*', 
        'keep recoPFCandidates_particleFlow_*_*', 
        'keep recoPFCandidates_particleFlowTmp_*_*', 
        'drop recoPFCandidates_particleFlowTmp__*', 
        'keep *_particleFlow_electrons_*', 
        'keep *_particleFlow_photons_*', 
        'keep recoCaloClusters_pfElectronTranslator_*_*', 
        'keep recoPreshowerClusters_pfElectronTranslator_*_*', 
        'keep recoSuperClusters_pfElectronTranslator_*_*', 
        'keep recoCaloClusters_pfPhotonTranslator_*_*', 
        'keep recoPreshowerClusters_pfPhotonTranslator_*_*', 
        'keep recoSuperClusters_pfPhotonTranslator_*_*', 
        'keep recoPhotons_pfPhotonTranslator_*_*', 
        'keep recoPhotonCores_pfPhotonTranslator_*_*', 
        'keep recoConversions_pfPhotonTranslator_*_*', 
        'keep *_particleFlowPtrs_*_*', 
        'keep *_particleFlowTmpPtrs_*_*')
)

process.RecoParticleFlowFEVT = cms.PSet(
    outputCommands = cms.untracked.vstring('drop CaloTowersSorted_towerMakerPF_*_*', 
        'keep recoPFRecHits_particleFlowClusterECAL_Cleaned_*', 
        'keep recoPFRecHits_particleFlowClusterHCAL_Cleaned_*', 
        'keep recoPFRecHits_particleFlowClusterHO_Cleaned_*', 
        'keep recoPFRecHits_particleFlowClusterHFEM_Cleaned_*', 
        'keep recoPFRecHits_particleFlowClusterHFHAD_Cleaned_*', 
        'keep recoPFRecHits_particleFlowClusterPS_Cleaned_*', 
        'keep recoPFRecHits_particleFlowRecHitECAL_Cleaned_*', 
        'keep recoPFRecHits_particleFlowRecHitHO_Cleaned_*', 
        'keep recoPFRecHits_particleFlowRecHitHCAL_Cleaned_*', 
        'keep recoPFRecHits_particleFlowRecHitPS_Cleaned_*', 
        'keep recoPFClusters_particleFlowClusterECAL_*_*', 
        'keep recoPFClusters_particleFlowClusterHCAL_*_*', 
        'keep recoPFClusters_particleFlowClusterHO_*_*', 
        'keep recoPFClusters_particleFlowClusterHFEM_*_*', 
        'keep recoPFClusters_particleFlowClusterHFHAD_*_*', 
        'keep recoPFClusters_particleFlowClusterPS_*_*', 
        'keep recoPFBlocks_particleFlowBlock_*_*', 
        'keep recoPFCandidates_particleFlowEGamma_*_*', 
        'keep recoCaloClusters_particleFlowEGamma_*_*', 
        'keep recoSuperClusters_particleFlowEGamma_*_*', 
        'keep recoConversions_particleFlowEGamma_*_*', 
        'keep recoPFCandidates_particleFlow_*_*', 
        'keep recoPFCandidates_particleFlowTmp_*_*', 
        'drop recoPFCandidates_particleFlowTmp__*', 
        'keep recoPFDisplacedVertexs_particleFlowDisplacedVertex_*_*', 
        'keep *_pfElectronTranslator_*_*', 
        'keep *_pfPhotonTranslator_*_*', 
        'keep *_particleFlow_electrons_*', 
        'keep *_particleFlow_photons_*', 
        'keep *_trackerDrivenElectronSeeds_preid_*', 
        'keep *_particleFlowPtrs_*_*', 
        'keep *_particleFlowTmpPtrs_*_*')
)

process.RecoParticleFlowRECO = cms.PSet(
    outputCommands = cms.untracked.vstring('drop CaloTowersSorted_towerMakerPF_*_*', 
        'keep recoPFRecHits_particleFlowClusterECAL_Cleaned_*', 
        'keep recoPFRecHits_particleFlowClusterHCAL_Cleaned_*', 
        'keep recoPFRecHits_particleFlowClusterHO_Cleaned_*', 
        'keep recoPFRecHits_particleFlowClusterHFEM_Cleaned_*', 
        'keep recoPFRecHits_particleFlowClusterHFHAD_Cleaned_*', 
        'keep recoPFRecHits_particleFlowClusterPS_Cleaned_*', 
        'keep recoPFRecHits_particleFlowRecHitECAL_Cleaned_*', 
        'keep recoPFRecHits_particleFlowRecHitHCAL_Cleaned_*', 
        'keep recoPFRecHits_particleFlowRecHitHO_Cleaned_*', 
        'keep recoPFRecHits_particleFlowRecHitPS_Cleaned_*', 
        'keep recoPFClusters_particleFlowClusterECAL_*_*', 
        'keep recoPFClusters_particleFlowClusterHCAL_*_*', 
        'keep recoPFClusters_particleFlowClusterHO_*_*', 
        'keep recoPFClusters_particleFlowClusterPS_*_*', 
        'keep recoPFBlocks_particleFlowBlock_*_*', 
        'keep recoPFCandidates_particleFlowEGamma_*_*', 
        'keep recoCaloClusters_particleFlowEGamma_*_*', 
        'keep recoSuperClusters_particleFlowEGamma_*_*', 
        'keep recoConversions_particleFlowEGamma_*_*', 
        'keep recoPFCandidates_particleFlow_*_*', 
        'keep recoPFCandidates_particleFlowTmp_electrons_*', 
        'keep recoPFCandidates_particleFlowTmp_*_*', 
        'drop recoPFCandidates_particleFlowTmp__*', 
        'keep recoPFDisplacedVertexs_particleFlowDisplacedVertex_*_*', 
        'keep *_pfElectronTranslator_*_*', 
        'keep *_pfPhotonTranslator_*_*', 
        'keep *_particleFlow_electrons_*', 
        'keep *_particleFlow_photons_*', 
        'keep *_trackerDrivenElectronSeeds_preid_*', 
        'keep *_particleFlowPtrs_*_*', 
        'keep *_particleFlowTmpPtrs_*_*')
)

process.RecoPixelVertexingFEVT = cms.PSet(
    outputCommands = cms.untracked.vstring('keep *_pixelTracks_*_*', 
        'keep *_pixelVertices_*_*')
)

process.RecoPixelVertexingRECO = cms.PSet(
    outputCommands = cms.untracked.vstring('keep *_pixelTracks_*_*', 
        'keep *_pixelVertices_*_*')
)

process.RecoTauTagAOD = cms.PSet(
    outputCommands = cms.untracked.vstring('keep *_ak4PFJetsRecoTauPiZeros_*_*', 
        'keep *_hpsPFTauProducer_*_*', 
        'keep *_hpsPFTauDiscrimination*_*_*', 
        'keep *_hpsPFTau*PtSum_*_*', 
        'keep *_hpsPFTauTransverseImpactParameters_*_*')
)

process.RecoTauTagFEVT = cms.PSet(
    outputCommands = cms.untracked.vstring('keep *_ak4PFJetsRecoTauPiZeros_*_*', 
        'keep *_hpsPFTauProducer_*_*', 
        'keep *_hpsPFTauDiscrimination*_*_*', 
        'keep *_hpsPFTau*PtSum_*_*', 
        'keep *_hpsPFTauTransverseImpactParameters_*_*')
)

process.RecoTauTagRECO = cms.PSet(
    outputCommands = cms.untracked.vstring('keep *_ak4PFJetsRecoTauPiZeros_*_*', 
        'keep *_hpsPFTauProducer_*_*', 
        'keep *_hpsPFTauDiscrimination*_*_*', 
        'keep *_hpsPFTau*PtSum_*_*', 
        'keep *_hpsPFTauTransverseImpactParameters_*_*')
)

process.RecoTrackerAOD = cms.PSet(
    outputCommands = cms.untracked.vstring('keep recoTracks_generalTracks_*_*', 
        'keep recoTracks_rsWithMaterialTracks_*_*', 
        'keep recoTracks_conversionStepTracks_*_*', 
        'keep recoTracks_beamhaloTracks_*_*', 
        'keep recoTracks_regionalCosmicTracks_*_*', 
        'keep recoTracks_ctfPixelLess_*_*', 
        'keep *_dedxHarmonic2_*_*', 
        'keep *_dedxDiscrimASmi_*_*', 
        'keep *_trackExtrapolator_*_*', 
        'keep floatedmValueMap_generalTracks_*_*')
)

process.RecoTrackerFEVT = cms.PSet(
    outputCommands = cms.untracked.vstring('keep recoTracks_generalTracks_*_*', 
        'keep recoTrackExtras_generalTracks_*_*', 
        'keep TrackingRecHitsOwned_extraFromSeeds_*_*', 
        'keep uints_extraFromSeeds_*_*', 
        'keep TrackingRecHitsOwned_generalTracks_*_*', 
        'keep recoTracks_beamhaloTracks_*_*', 
        'keep recoTrackExtras_beamhaloTracks_*_*', 
        'keep TrackingRecHitsOwned_beamhaloTracks_*_*', 
        'keep recoTracks_regionalCosmicTracks_*_*', 
        'keep recoTrackExtras_regionalCosmicTracks_*_*', 
        'keep TrackingRecHitsOwned_regionalCosmicTracks_*_*', 
        'keep recoTracks_rsWithMaterialTracks_*_*', 
        'keep recoTrackExtras_rsWithMaterialTracks_*_*', 
        'keep TrackingRecHitsOwned_rsWithMaterialTracks_*_*', 
        'keep recoTracks_conversionStepTracks_*_*', 
        'keep recoTrackExtras_conversionStepTracks_*_*', 
        'keep TrackingRecHitsOwned_conversionStepTracks_*_*', 
        'keep *_ctfPixelLess_*_*', 
        'keep *_dedxTruncated40_*_*', 
        'keep *_dedxDiscrimASmi_*_*', 
        'keep *_dedxHarmonic2_*_*', 
        'keep *_trackExtrapolator_*_*', 
        'keep floatedmValueMap_generalTracks_*_*')
)

process.RecoTrackerRECO = cms.PSet(
    outputCommands = cms.untracked.vstring('keep recoTracks_generalTracks_*_*', 
        'keep recoTrackExtras_generalTracks_*_*', 
        'keep TrackingRecHitsOwned_extraFromSeeds_*_*', 
        'keep uints_extraFromSeeds_*_*', 
        'keep TrackingRecHitsOwned_generalTracks_*_*', 
        'keep recoTracks_beamhaloTracks_*_*', 
        'keep recoTrackExtras_beamhaloTracks_*_*', 
        'keep TrackingRecHitsOwned_beamhaloTracks_*_*', 
        'keep recoTracks_regionalCosmicTracks_*_*', 
        'keep recoTrackExtras_regionalCosmicTracks_*_*', 
        'keep TrackingRecHitsOwned_regionalCosmicTracks_*_*', 
        'keep recoTracks_rsWithMaterialTracks_*_*', 
        'keep recoTrackExtras_rsWithMaterialTracks_*_*', 
        'keep TrackingRecHitsOwned_rsWithMaterialTracks_*_*', 
        'keep recoTracks_conversionStepTracks_*_*', 
        'keep recoTrackExtras_conversionStepTracks_*_*', 
        'keep TrackingRecHitsOwned_conversionStepTracks_*_*', 
        'keep *_ctfPixelLess_*_*', 
        'keep *_dedxTruncated40_*_*', 
        'keep *_dedxDiscrimASmi_*_*', 
        'keep *_dedxHarmonic2_*_*', 
        'keep *_trackExtrapolator_*_*', 
        'keep floatedmValueMap_generalTracks_*_*')
)

process.RecoVertexAOD = cms.PSet(
    outputCommands = cms.untracked.vstring('keep  *_offlinePrimaryVertices__*', 
        'keep  *_offlinePrimaryVerticesWithBS_*_*', 
        'keep  *_offlinePrimaryVerticesFromCosmicTracks_*_*', 
        'keep  *_nuclearInteractionMaker_*_*', 
        'keep *_generalV0Candidates_*_*', 
        'keep *_inclusiveSecondaryVertices_*_*')
)

process.RecoVertexFEVT = cms.PSet(
    outputCommands = cms.untracked.vstring('keep  *_offlinePrimaryVertices__*', 
        'keep  *_offlinePrimaryVerticesWithBS_*_*', 
        'keep  *_offlinePrimaryVerticesFromCosmicTracks_*_*', 
        'keep  *_nuclearInteractionMaker_*_*', 
        'keep *_generalV0Candidates_*_*', 
        'keep *_inclusiveSecondaryVertices_*_*')
)

process.RecoVertexRECO = cms.PSet(
    outputCommands = cms.untracked.vstring('keep  *_offlinePrimaryVertices__*', 
        'keep  *_offlinePrimaryVerticesWithBS_*_*', 
        'keep  *_offlinePrimaryVerticesFromCosmicTracks_*_*', 
        'keep  *_nuclearInteractionMaker_*_*', 
        'keep *_generalV0Candidates_*_*', 
        'keep *_inclusiveSecondaryVertices_*_*')
)

process.SimCalorimetryAOD = cms.PSet(
    outputCommands = cms.untracked.vstring()
)

process.SimCalorimetryFEVTDEBUG = cms.PSet(
    outputCommands = cms.untracked.vstring('keep *_simEcalDigis_*_*', 
        'keep *_simEcalPreshowerDigis_*_*', 
        'keep *_simEcalTriggerPrimitiveDigis_*_*', 
        'keep *_simHcalDigis_*_*', 
        'keep ZDCDataFramesSorted_simHcalUnsuppressedDigis_*_*', 
        'drop ZDCDataFramesSorted_mix_simHcalUnsuppressedDigis*_*', 
        'keep *_simHcalTriggerPrimitiveDigis_*_*')
)

process.SimCalorimetryRAW = cms.PSet(
    outputCommands = cms.untracked.vstring('keep EBSrFlagsSorted_simEcalDigis_*_*', 
        'keep EESrFlagsSorted_simEcalDigis_*_*')
)

process.SimCalorimetryRECO = cms.PSet(
    outputCommands = cms.untracked.vstring()
)

process.SimG4CoreAOD = cms.PSet(
    outputCommands = cms.untracked.vstring()
)

process.SimG4CoreRAW = cms.PSet(
    outputCommands = cms.untracked.vstring('keep *_g4SimHits_*_*', 
        'keep edmHepMCProduct_source_*_*')
)

process.SimG4CoreRECO = cms.PSet(
    outputCommands = cms.untracked.vstring('keep edmHepMCProduct_source_*_*', 
        'keep SimTracks_g4SimHits_*_*', 
        'keep SimVertexs_g4SimHits_*_*')
)

process.SimGeneralAOD = cms.PSet(
    outputCommands = cms.untracked.vstring('keep PileupSummaryInfos_*_*_*')
)

process.SimGeneralFEVTDEBUG = cms.PSet(
    outputCommands = cms.untracked.vstring('drop *_trackingtruthprod_*_*', 
        'drop *_electrontruth_*_*', 
        'keep *_mix_MergedTrackTruth_*', 
        'keep CrossingFramePlaybackInfoExtended_*_*_*')
)

process.SimGeneralRAW = cms.PSet(
    outputCommands = cms.untracked.vstring('keep CrossingFramePlaybackInfoExtended_*_*_*', 
        'keep PileupSummaryInfos_*_*_*')
)

process.SimGeneralRECO = cms.PSet(
    outputCommands = cms.untracked.vstring('keep PileupSummaryInfos_*_*_*')
)

process.SimMuonAOD = cms.PSet(
    outputCommands = cms.untracked.vstring()
)

process.SimMuonFEVTDEBUG = cms.PSet(
    outputCommands = cms.untracked.vstring('keep *_simMuonCSCDigis_*_*', 
        'keep *_simMuonDTDigis_*_*', 
        'keep *_simMuonRPCDigis_*_*')
)

process.SimMuonRAW = cms.PSet(
    outputCommands = cms.untracked.vstring('keep StripDigiSimLinkedmDetSetVector_simMuonCSCDigis_*_*', 
        'keep CSCDetIdCSCComparatorDigiMuonDigiCollection_simMuonCSCDigis_*_*', 
        'keep DTLayerIdDTDigiSimLinkMuonDigiCollection_simMuonDTDigis_*_*', 
        'keep RPCDigiSimLinkedmDetSetVector_simMuonRPCDigis_*_*')
)

process.SimMuonRECO = cms.PSet(
    outputCommands = cms.untracked.vstring('keep StripDigiSimLinkedmDetSetVector_simMuonCSCDigis_*_*', 
        'keep DTLayerIdDTDigiSimLinkMuonDigiCollection_simMuonDTDigis_*_*', 
        'keep RPCDigiSimLinkedmDetSetVector_simMuonRPCDigis_*_*')
)

process.SimTrackerAOD = cms.PSet(
    outputCommands = cms.untracked.vstring('keep *_allTrackMCMatch_*_*')
)

process.SimTrackerDEBUG = cms.PSet(
    outputCommands = cms.untracked.vstring('keep PixelDigiSimLinkedmDetSetVector_simSiPixelDigis_*_*', 
        'keep StripDigiSimLinkedmDetSetVector_simSiStripDigis_*_*', 
        'drop *_mix_simSiPixelDigis*_*', 
        'drop *_mix_simSiStripDigis*_*', 
        'keep *_allTrackMCMatch_*_*')
)

process.SimTrackerFEVTDEBUG = cms.PSet(
    outputCommands = cms.untracked.vstring('keep *_simSiPixelDigis_*_*', 
        'keep *_simSiStripDigis_*_*', 
        'drop *_mix_simSiPixelDigis*_*', 
        'drop *_mix_simSiStripDigis*_*', 
        'keep *_allTrackMCMatch_*_*', 
        'keep *_trackingParticleRecoTrackAsssociation_*_*', 
        'keep *_assoc2secStepTk_*_*', 
        'keep *_assoc2thStepTk_*_*', 
        'keep *_assoc2GsfTracks_*_*', 
        'keep *_assocOutInConversionTracks_*_*', 
        'keep *_assocInOutConversionTracks_*_*')
)

process.SimTrackerRAW = cms.PSet(
    outputCommands = cms.untracked.vstring('keep *_allTrackMCMatch_*_*')
)

process.SimTrackerRECO = cms.PSet(
    outputCommands = cms.untracked.vstring('keep *_allTrackMCMatch_*_*')
)

process.TrackingToolsAOD = cms.PSet(
    outputCommands = cms.untracked.vstring('keep recoTracks_GsfGlobalElectronTest_*_*', 
        'keep recoGsfTracks_electronGsfTracks_*_*')
)

process.TrackingToolsFEVT = cms.PSet(
    outputCommands = cms.untracked.vstring('keep *_CkfElectronCandidates_*_*', 
        'keep *_GsfGlobalElectronTest_*_*', 
        'keep *_electronMergedSeeds_*_*', 
        'keep *_electronGsfTracks_*_*')
)

process.TrackingToolsRECO = cms.PSet(
    outputCommands = cms.untracked.vstring('keep *_CkfElectronCandidates_*_*', 
        'keep *_GsfGlobalElectronTest_*_*', 
        'keep *_electronMergedSeeds_*_*', 
        'keep recoGsfTracks_electronGsfTracks_*_*', 
        'keep recoGsfTrackExtras_electronGsfTracks_*_*', 
        'keep recoTrackExtras_electronGsfTracks_*_*', 
        'keep TrackingRecHitsOwned_electronGsfTracks_*_*')
)

process.combinedSecondaryVertexCommon = cms.PSet(
    trackPseudoSelection = cms.PSet(
        b_pT = cms.double(0.3684),
        a_dR = cms.double(-0.001053),
        min_pT = cms.double(120),
        max_pT = cms.double(500),
        min_pT_dRcut = cms.double(0.5),
        max_pT_trackPTcut = cms.double(3),
        max_pT_dRcut = cms.double(0.1),
        b_dR = cms.double(0.6263),
        a_pT = cms.double(0.005263),
        totalHitsMin = cms.uint32(0),
        jetDeltaRMax = cms.double(0.3),
        qualityClass = cms.string('any'),
        pixelHitsMin = cms.uint32(0),
        sip3dSigMin = cms.double(-99999.9),
        sip3dSigMax = cms.double(99999.9),
        maxDistToAxis = cms.double(0.07),
        useVariableJTA = cms.bool(False),
        sip2dValMax = cms.double(99999.9),
        maxDecayLen = cms.double(5),
        ptMin = cms.double(0.0),
        sip2dSigMax = cms.double(99999.9),
        sip2dSigMin = cms.double(2.0),
        sip3dValMax = cms.double(99999.9),
        sip3dValMin = cms.double(-99999.9),
        sip2dValMin = cms.double(-99999.9),
        normChi2Max = cms.double(99999.9)
    ),
    trackSelection = cms.PSet(
        b_pT = cms.double(0.3684),
        a_dR = cms.double(-0.001053),
        min_pT = cms.double(120),
        max_pT = cms.double(500),
        min_pT_dRcut = cms.double(0.5),
        max_pT_trackPTcut = cms.double(3),
        max_pT_dRcut = cms.double(0.1),
        b_dR = cms.double(0.6263),
        a_pT = cms.double(0.005263),
        totalHitsMin = cms.uint32(0),
        jetDeltaRMax = cms.double(0.3),
        qualityClass = cms.string('any'),
        pixelHitsMin = cms.uint32(0),
        sip3dSigMin = cms.double(-99999.9),
        sip3dSigMax = cms.double(99999.9),
        maxDistToAxis = cms.double(0.07),
        useVariableJTA = cms.bool(False),
        sip2dValMax = cms.double(99999.9),
        maxDecayLen = cms.double(5),
        ptMin = cms.double(0.0),
        sip2dSigMax = cms.double(99999.9),
        sip2dSigMin = cms.double(-99999.9),
        sip3dValMax = cms.double(99999.9),
        sip3dValMin = cms.double(-99999.9),
        sip2dValMin = cms.double(-99999.9),
        normChi2Max = cms.double(99999.9)
    ),
    useTrackWeights = cms.bool(True),
    pseudoMultiplicityMin = cms.uint32(2),
    correctVertexMass = cms.bool(True),
    trackPairV0Filter = cms.PSet(
        k0sMassWindow = cms.double(0.03)
    ),
    charmCut = cms.double(1.5),
    vertexFlip = cms.bool(False),
    minimumTrackWeight = cms.double(0.5),
    pseudoVertexV0Filter = cms.PSet(
        k0sMassWindow = cms.double(0.05)
    ),
    trackMultiplicityMin = cms.uint32(2),
    trackSort = cms.string('sip2dSig'),
    trackFlip = cms.bool(False)
)

process.ecalLocalRecoAOD = cms.PSet(
    outputCommands = cms.untracked.vstring()
)

process.ecalLocalRecoFEVT = cms.PSet(
    outputCommands = cms.untracked.vstring('keep *_ecalMultiFitUncalibRecHit_*_*', 
        'keep *_ecalPreshowerRecHit_*_*', 
        'keep *_ecalRecHit_*_*')
)

process.ecalLocalRecoRECO = cms.PSet(
    outputCommands = cms.untracked.vstring('keep *_ecalPreshowerRecHit_*_*', 
        'keep *_ecalRecHit_*_*', 
        'keep *_ecalCompactTrigPrim_*_*', 
        'keep *_ecalTPSkim_*_*')
)

process.fieldScaling = cms.PSet(
    scalingVolumes = cms.vint32(14100, 14200, 17600, 17800, 17900, 
        18100, 18300, 18400, 18600, 23100, 
        23300, 23400, 23600, 23800, 23900, 
        24100, 28600, 28800, 28900, 29100, 
        29300, 29400, 29600, 28609, 28809, 
        28909, 29109, 29309, 29409, 29609, 
        28610, 28810, 28910, 29110, 29310, 
        29410, 29610, 28611, 28811, 28911, 
        29111, 29311, 29411, 29611),
    scalingFactors = cms.vdouble(1, 1, 0.994, 1.004, 1.004, 
        1.005, 1.004, 1.004, 0.994, 0.965, 
        0.958, 0.958, 0.953, 0.958, 0.958, 
        0.965, 0.918, 0.924, 0.924, 0.906, 
        0.924, 0.924, 0.918, 0.991, 0.998, 
        0.998, 0.978, 0.998, 0.998, 0.991, 
        0.991, 0.998, 0.998, 0.978, 0.998, 
        0.998, 0.991, 0.991, 0.998, 0.998, 
        0.978, 0.998, 0.998, 0.991)
)

process.ghostTrackCommon = cms.PSet(
    trackSelection = cms.PSet(
        b_pT = cms.double(0.3684),
        a_dR = cms.double(-0.001053),
        min_pT = cms.double(120),
        max_pT = cms.double(500),
        min_pT_dRcut = cms.double(0.5),
        max_pT_trackPTcut = cms.double(3),
        max_pT_dRcut = cms.double(0.1),
        b_dR = cms.double(0.6263),
        a_pT = cms.double(0.005263),
        totalHitsMin = cms.uint32(0),
        jetDeltaRMax = cms.double(0.3),
        qualityClass = cms.string('any'),
        pixelHitsMin = cms.uint32(0),
        sip3dSigMin = cms.double(-99999.9),
        sip3dSigMax = cms.double(99999.9),
        maxDistToAxis = cms.double(0.07),
        useVariableJTA = cms.bool(False),
        sip2dValMax = cms.double(99999.9),
        maxDecayLen = cms.double(5),
        ptMin = cms.double(0.0),
        sip2dSigMax = cms.double(99999.9),
        sip2dSigMin = cms.double(-99999.9),
        sip3dValMax = cms.double(99999.9),
        sip3dValMin = cms.double(-99999.9),
        sip2dValMin = cms.double(-99999.9),
        normChi2Max = cms.double(99999.9)
    ),
    trackPairV0Filter = cms.PSet(
        k0sMassWindow = cms.double(0.03)
    ),
    charmCut = cms.double(1.5),
    trackSort = cms.string('sip2dSig'),
    minimumTrackWeight = cms.double(0.5)
)

process.ghostTrackVertexRecoBlock = cms.PSet(
    vertexReco = cms.PSet(
        primcut = cms.double(2.0),
        seccut = cms.double(4.0),
        maxFitChi2 = cms.double(10.0),
        fitType = cms.string('RefitGhostTrackWithVertices'),
        mergeThreshold = cms.double(3.0),
        finder = cms.string('gtvr')
    )
)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True),
    allowUnscheduled = cms.untracked.bool(True)
)

process.trackPseudoSelectionBlock = cms.PSet(
    trackPseudoSelection = cms.PSet(
        b_pT = cms.double(0.3684),
        a_dR = cms.double(-0.001053),
        min_pT = cms.double(120),
        max_pT = cms.double(500),
        min_pT_dRcut = cms.double(0.5),
        max_pT_trackPTcut = cms.double(3),
        max_pT_dRcut = cms.double(0.1),
        b_dR = cms.double(0.6263),
        a_pT = cms.double(0.005263),
        totalHitsMin = cms.uint32(0),
        jetDeltaRMax = cms.double(0.3),
        qualityClass = cms.string('any'),
        pixelHitsMin = cms.uint32(0),
        sip3dSigMin = cms.double(-99999.9),
        sip3dSigMax = cms.double(99999.9),
        maxDistToAxis = cms.double(0.07),
        useVariableJTA = cms.bool(False),
        sip2dValMax = cms.double(99999.9),
        maxDecayLen = cms.double(5),
        ptMin = cms.double(0.0),
        sip2dSigMax = cms.double(99999.9),
        sip2dSigMin = cms.double(2.0),
        sip3dValMax = cms.double(99999.9),
        sip3dValMin = cms.double(-99999.9),
        sip2dValMin = cms.double(-99999.9),
        normChi2Max = cms.double(99999.9)
    )
)

process.trackSelectionBlock = cms.PSet(
    trackSelection = cms.PSet(
        b_pT = cms.double(0.3684),
        a_dR = cms.double(-0.001053),
        min_pT = cms.double(120),
        max_pT = cms.double(500),
        min_pT_dRcut = cms.double(0.5),
        max_pT_trackPTcut = cms.double(3),
        max_pT_dRcut = cms.double(0.1),
        b_dR = cms.double(0.6263),
        a_pT = cms.double(0.005263),
        totalHitsMin = cms.uint32(0),
        jetDeltaRMax = cms.double(0.3),
        qualityClass = cms.string('any'),
        pixelHitsMin = cms.uint32(0),
        sip3dSigMin = cms.double(-99999.9),
        sip3dSigMax = cms.double(99999.9),
        maxDistToAxis = cms.double(0.07),
        useVariableJTA = cms.bool(False),
        sip2dValMax = cms.double(99999.9),
        maxDecayLen = cms.double(5),
        ptMin = cms.double(0.0),
        sip2dSigMax = cms.double(99999.9),
        sip2dSigMin = cms.double(-99999.9),
        sip3dValMax = cms.double(99999.9),
        sip3dValMin = cms.double(-99999.9),
        sip2dValMin = cms.double(-99999.9),
        normChi2Max = cms.double(99999.9)
    )
)

process.variableJTAPars = cms.PSet(
    b_pT = cms.double(0.3684),
    a_dR = cms.double(-0.001053),
    min_pT = cms.double(120),
    max_pT = cms.double(500),
    min_pT_dRcut = cms.double(0.5),
    max_pT_trackPTcut = cms.double(3),
    max_pT_dRcut = cms.double(0.1),
    b_dR = cms.double(0.6263),
    a_pT = cms.double(0.005263)
)

process.vertexCutsBlock = cms.PSet(
    vertexCuts = cms.PSet(
        distSig3dMax = cms.double(99999.9),
        fracPV = cms.double(0.65),
        distVal2dMax = cms.double(2.5),
        useTrackWeights = cms.bool(True),
        maxDeltaRToJetAxis = cms.double(0.5),
        v0Filter = cms.PSet(
            k0sMassWindow = cms.double(0.05)
        ),
        distSig2dMin = cms.double(3.0),
        multiplicityMin = cms.uint32(2),
        massMax = cms.double(6.5),
        distSig2dMax = cms.double(99999.9),
        distVal3dMax = cms.double(99999.9),
        minimumTrackWeight = cms.double(0.5),
        distVal3dMin = cms.double(-99999.9),
        distVal2dMin = cms.double(0.01),
        distSig3dMin = cms.double(-99999.9)
    )
)

process.vertexRecoBlock = cms.PSet(
    vertexReco = cms.PSet(
        seccut = cms.double(6.0),
        primcut = cms.double(1.8),
        smoothing = cms.bool(False),
        weightthreshold = cms.double(0.001),
        minweight = cms.double(0.5),
        finder = cms.string('avr')
    )
)

process.vertexSelectionBlock = cms.PSet(
    vertexSelection = cms.PSet(
        sortCriterium = cms.string('dist3dError')
    )
)

process.vertexTrackSelectionBlock = cms.PSet(
    trackSelection = cms.PSet(
        b_pT = cms.double(0.3684),
        a_dR = cms.double(-0.001053),
        min_pT = cms.double(120),
        max_pT = cms.double(500),
        min_pT_dRcut = cms.double(0.5),
        max_pT_trackPTcut = cms.double(3),
        max_pT_dRcut = cms.double(0.1),
        b_dR = cms.double(0.6263),
        a_pT = cms.double(0.005263),
        totalHitsMin = cms.uint32(8),
        jetDeltaRMax = cms.double(0.3),
        qualityClass = cms.string('any'),
        pixelHitsMin = cms.uint32(2),
        sip3dSigMin = cms.double(-99999.9),
        sip3dSigMax = cms.double(99999.9),
        maxDistToAxis = cms.double(0.2),
        useVariableJTA = cms.bool(False),
        sip2dValMax = cms.double(99999.9),
        maxDecayLen = cms.double(99999.9),
        ptMin = cms.double(1.0),
        sip2dSigMax = cms.double(99999.9),
        sip2dSigMin = cms.double(-99999.9),
        sip3dValMax = cms.double(99999.9),
        sip3dValMin = cms.double(-99999.9),
        sip2dValMin = cms.double(-99999.9),
        normChi2Max = cms.double(99999.9)
    )
)

