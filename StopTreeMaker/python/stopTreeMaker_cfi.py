import FWCore.ParameterSet.Config as cms

stopTreeMaker = cms.EDProducer(
'stopTreeMaker',
# Name of the output tree
TreeName          = cms.string('AUX'),
## might help if something isn working wilil produce couts
debug = cms.bool(False),
# List of InputTags for Float_t variables (Float_t) stored in
# optional names to store in the tree can be defined  not only will have an effect if the number of input tags for variable is exactly the same as number of optional names!!
varsDouble        = cms.VInputTag(),
varsDoubleNamesInTree = cms.vstring(),
# List of InputTags for Int variables (Int) stored in
# optional names to store in the tree can be defined  not only will have an effect if the number of input tags for variable is exactly the same as number of optional names!!
varsInt        = cms.VInputTag(),
varsIntNamesInTree = cms.vstring(),
# List of InputTags for bool variables (bool) stored in
# optional names to store in the tree can be defined  not only will have an effect if the number of input tags for variable is exactly the same as number of optional names!!
varsBool        = cms.VInputTag(),
varsBoolNamesInTree = cms.vstring(),

varsString      = cms.VInputTag(),
varsStringNamesInTree = cms.vstring(),

# List of InputTags for TLorentz variables (TLorentz) stored in
# optional names to store in the tree can be defined  not only will have an effect if the number of input tags for variable is exactly the same as number of optional names!!
varsTLorentzVector        = cms.VInputTag(),
varsTLorentzVectorNamesInTree = cms.vstring(),

vectorDouble              = cms.VInputTag(),
vectorDoubleNamesInTree   = cms.vstring(),

vectorInt                 = cms.VInputTag(),
vectorIntNamesInTree      = cms.vstring(),

vectorBool                = cms.VInputTag(),
vectorBoolNamesInTree     = cms.vstring(),

vectorString              = cms.VInputTag(),
vectorStringNamesInTree   = cms.vstring(),

vectorTLorentzVector        = cms.VInputTag(),
vectorTLorentzVectorNamesInTree = cms.vstring(),
)
