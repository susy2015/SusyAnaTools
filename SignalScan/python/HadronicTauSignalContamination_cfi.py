import FWCore.ParameterSet.Config as cms

HadronicTauSignalContamination = cms.EDProducer('HadronicTauSignalContamination',

   muonSource              = cms.InputTag("myPatMuonsPFIDIso"),
   muonForJetOverlapSource = cms.InputTag("patMuonsPFIDIso"),
   eleSource               = cms.InputTag("patElectronsIDIso"),
   jetSource               = cms.InputTag("myPatJetsPFchsnoOverlapLeptons"),
   oriJetSource            = cms.InputTag("patJetsPF"),
   bigPtDiffSource         = cms.InputTag("myPatJetsPFchsnoOverlapLeptons:bigPtDiff"),

   forVetoIsoTrkSrc    = cms.InputTag("trackIsolation"),

   jetMHTcuts              = cms.vdouble(30, 5),
   jetHTcuts               = cms.vdouble(50, 2.5),

   cutJetMETDPhivals       = cms.vdouble(0.5, 0.5, 0.3),

   templateRootName    = cms.string("tauTemplate.root"),

   mhtInputTag         = cms.InputTag("mymhtPFchs"),
   htInputTag          = cms.InputTag("myhtPFchs"),
   metInputTag         = cms.InputTag("patMETsPF"),

   VertexSource        = cms.InputTag("goodVertices"),
   nVtxPUcut           = cms.uint32(4),

   genJetSrc           = cms.InputTag("ak5GenJets"),
   genParticleSrc      = cms.InputTag("genParticles"),

   cutLoHTvals         = cms.vdouble(   350,   500,   800,   800,   500,  1200,  1000,   500,   500,   500,   500,   800,   800,   800,   800,  1000,  1000,  1000, 1200,   1200,  1400 ),
   cutHiHTvals         = cms.vdouble(    -1,    -1,    -1,    -1,    -1,    -1,    -1,   800,   800,   800,   800,  1000,  1000,  1000,  1000,  1200,  1200,  1200, 1400,   1400,    -1 ),

   cutLoMHTvals        = cms.vdouble(   200,   200,   200,   500,   350,   400,   600,   200,   350,   500,   600,   200,   350,   500,   600,   200,   350,   500,  200,    350,   200 ),
   cutHiMHTvals        = cms.vdouble(    -1,    -1,    -1,    -1,    -1,    -1,    -1,   350,   500,   600,    -1,   350,   500,   600,    -1,   350,   500,    -1,  350,     -1,    -1 ),

   effWeightsInput     = cms.InputTag("muonEffProducerMC:effWeights"),
   effWeightErr2sInput = cms.InputTag("muonEffProducerMC:effWeightErr2s"),

   Ntry                = cms.int32(100),

   applyMethod         = cms.uint32(0),

   debug               = cms.bool(False),

   evtWeightInput      = cms.InputTag("weightProducer:weight"),

# for MT cut:  two values each vector ==> 0 : lower cut  1 : higher cut
   cutMTvals           = cms.vdouble(0, 100),

# for sys.
   scaleVIS            = cms.untracked.double(0.0),

   bTagKeyString         = cms.string("combinedSecondaryVertexBJetTags"),
   pfJetCutForJetStoring = cms.string("pt>30 && abs(eta)<5.0"),

#                                    minAbsEta, maxAbsEta, minPt, maxPt
   reqPt30Vec          = cms.vdouble(    -1,       -1,       30,   -1  ),
   reqPt30Eta24Vec     = cms.vdouble(    -1,      2.4,       30,   -1  ),
   reqPt50Eta24Vec     = cms.vdouble(    -1,      2.4,       50,   -1  ),
   reqPt70Eta24Vec     = cms.vdouble(    -1,      2.4,       70,   -1  ),
   reqdPhiVec          = cms.vdouble(    -1,      4.7,       30,   -1  ),
   reqCSVSVec          = cms.vdouble(    -1,      2.4,       30,   -1  ),

# search cuts
   cutLoMETvals        = cms.vdouble(      200,      350,      200,      350,              175,              200,             350,             175,               200,              350),
   cutHiMETvals        = cms.vdouble(       -1,       -1,       -1,       -1,              200,               -1,              -1,             200,                -1,               -1),
# Pick the predScales, because the HLT efficiency is applied directly as an event weight.
# And one can choose not to apply it in the muonEffProducerMC module and no need of correct this in the predScales.
# For >=1 b (Only update the first two, since they are the relevant ones)
   scaleToPred         = cms.vdouble(    0.723,    0.627,    0.668,    0.603,            0.776,            0.726,            0.657,           0.546,             0.701,            0.636),
# For >=2 b (Only update the first two, since they are the relevant ones)
#   scaleToPred         = cms.vdouble(    0.697,    0.598,    0.668,    0.603,            0.776,            0.726,            0.657,           0.546,             0.701,            0.636),
# No space between the words
   searchRegionKeywords = cms.vstring("OriKin", "OriKin", "NewKin", "NewKin", "OriKinflipDphi", "OriKinflipDphi", "OriKinflipDphi", "NewKinflipDphi",  "NewKinflipDphi","NewKinflipDphi"),

   xAxisDivs           = cms.vdouble(0, 5000),
   xAxisBins           =  cms.vint32(  100  ), 

   yAxisDivs           = cms.vdouble(0, 1500),
   yAxisBins           =  cms.vint32(  150  ), 

   susyScanType        = cms.string("SMS"),
   susyScanTopology    = cms.string("T2tt"),

   doFillTree          = cms.bool(True),
   muonKincuts         = cms.vdouble(20, 2.1),
   doZfilter           = cms.bool(False),
)
