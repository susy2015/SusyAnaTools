import FWCore.ParameterSet.Config as cms

#Three options to define/calculate an event weight:
weightProducer = cms.EDProducer('WeightProducer',
   #Option 1 (default):
   weightName  = cms.InputTag('weight'), # name of an existing weight-variable in the event

   #Option 2:
   weight = cms.double( -1.0 ),          # 

   #Option 3:
   Method = cms.string(""),              # "PtHat" to calculate a weight from pthat variable for 'flat' samples
   Exponent = cms.double(-4.5),          # used (only) for 'PtHat'-weighting
   XS = cms.double(4.9E10),              # used (only) for 'PtHat'-weighting
   NumberEvts = cms.double(-1.),         # used (only) for 'PtHat'-weighting
   Lumi = cms.double(100.),              # used (only) for 'PtHat'-weighting                     
   
   generatorSource = cms.InputTag('generator'),

   #The final calculated weight is scaled by the following factor.
   #This can be used e.g. to scale the luminosity by +-1sigma.
   LumiScale = cms.double( 1.0 ),

   weightWARNingLowThreshold = cms.double( 0.0 ),
   weightWARNingUpThreshold  = cms.double( 1e32 ),

   ratioWARNingThreshold     = cms.double( 1e-10 ),
)
