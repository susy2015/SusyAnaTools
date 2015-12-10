import FWCore.ParameterSet.Config as cms

EventListFilter = cms.EDFilter('EventListFilter',
        inputFileList = cms.string(''),
        TagMode = cms.bool(True)
)
