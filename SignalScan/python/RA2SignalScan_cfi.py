import FWCore.ParameterSet.Config as cms

RA2SignalScan = cms.EDAnalyzer('RA2SignalScan',

   vtxSrc              = cms.InputTag("goodVertices"),
   evtWeightInput      = cms.InputTag("weightProducer:weight"),
   nVtxPUcut           = cms.uint32(4),

   metSrc              = cms.InputTag("patMETsPF"),
   mhtSrc              = cms.InputTag("mhtPF"),
   mht_forSgnfSrc      = cms.InputTag("mhtPFforSgnf"),
   htSrc               = cms.InputTag("htPF"),

   forVetoMuonSrc      = cms.InputTag("patMuonsPFIDIso"),
   forVetoElectronSrc  = cms.InputTag("patElectronsPFIDIso"),

   nVetoLeptons        = cms.vint32(0, 0),

   debug               = cms.bool(False),

   fillTotalOnly       = cms.bool(False),

   cutLoHTvals         = cms.vdouble(350, 800, 800, 500),
   cutHiHTvals         = cms.vdouble( -1,  -1,  -1,  -1),

   cutLoMHTvals        = cms.vdouble(200, 200, 500, 350),
   cutHiMHTvals        = cms.vdouble( -1,  -1,  -1,  -1),

   xAxisDivs           = cms.vdouble(0, 5000),
   xAxisBins           =  cms.vint32(  100  ), 

   yAxisDivs           = cms.vdouble(0, 1500),
   yAxisBins           =  cms.vint32(  150  ), 

   susyScanType        = cms.string("mSUGRA"),
   susyScanTopology    = cms.string(""),

   nCompPDFvec         = cms.vint32(45, 41, 101),
   pdfwgts             = cms.VInputTag(cms.InputTag("pdfWeight:cteq66"), cms.InputTag("pdfWeight:MSTW2008nlo68cl"), cms.InputTag("pdfWeight:NNPDF20")),

   ISRweightSrc        = cms.InputTag("myProducerLabel"),

   doSignalSC          = cms.int32(0),
   hadTauSCweightInputTag  = cms.InputTag("HadronicTauSignalContamination:predictions"),
   lostLepSCweightInputTag = cms.InputTag("LostLeptonSignalContamination:predictions"),

)
