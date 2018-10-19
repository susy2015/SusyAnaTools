## Variables set by configure 
OPENCV_DIRECTORY  = /uscms_data/d3/pastika/zinv/dev/CMSSW_7_4_8/src/opencv/
XGBOOST_DIRECTORY = 
TENSORFLOW_DIR    = /cvmfs/cms.cern.ch/slc6_amd64_gcc630/external/tensorflow-c/1.1.0-mmelna
TAGGERDIR         = /uscms_data/d3/huiwang/CMSSW_9_4_4/src/SusyAnaTools/Tools/../../TopTagger

CXX           = g++
LD            = g++
CXXFLAGS      = -g -O2 --std=c++11
LDFLAGS       = 
PREFIX        = /usr/local
LDCONFIG      = 
LIBSUFFIX     = so
PYTHONCFG     = python-config
SCRAMNUMPYCFG = /cvmfs/cms.cern.ch/slc6_amd64_gcc630/external/py2-numpy/1.12.1-fmblme
TMVALIB       = -lTMVA

#manual configuration
IDIR       = .
ODIR       = obj
SDIR       = $(CMSSW_BASE)/src/SusyAnaTools/Tools/

CXX        = g++

CXXFLAGS += -I. -std=c++0x -I$(CMSSW_BASE)/src/ -isystem$(LHAPDF_DATA_PATH)/../../include -I$(CMSSW_RELEASE_BASE)/src 
#CXXFLAGS += -I$(shell scram tool info fastjet | grep '^INCLUDE' | sed 's/INCLUDE=//')
#CXXFLAGS += -I$(shell scram tool info boost | grep '^INCLUDE' | sed 's/INCLUDE=//')
## Optimization flag
CXXFLAGS += -g -fPIC
## Enable the maximun warning
#CXXFLAGS += -Wall -Wextra -Weffc++ -g

## Include ROOT
CXXFLAGS  += $(shell root-config --cflags)

CXXDEPFLAGS = -MMD -MP

LD         = g++
LDFLAGS    =

#Necessary includes and libraries, the order matters here!

# top tagger includes
INCLUDESDIRS  += -I. -I$(TAGGERDIR)/..
# top tagger libraries
LIBS += -L$(TAGGERDIR)/TopTagger/test -lTopTagger

## Include ROOT
INCLUDESDIRS += $(shell root-config --cflags)
# ROOT libraries
LIBS         += $(shell root-config --glibs)
LIBS_base  = $(shell root-config --glibs)

# TMVA library
ifdef TMVALIB
	CXXFLAGS += -DSHOTTOPTAGGER_DO_TMVA
	LIBS     += $(TMVALIB)
endif

#python includes and libraries
ifdef PYTHONCFG
	INCLUDESDIRS  += $(shell $(PYTHONCFG) --includes)

	CXXFLAGS += -DDOTENSORFLOWPYBIND
	LIBS     += -L$(shell $(PYTHONCFG) --prefix)/lib $(shell $(PYTHONCFG) --libs)
endif

#scram specific numpy c-api
ifdef SCRAMNUMPYCFG
	INCLUDESDIRS += -I$(SCRAMNUMPYCFG)/c-api/core/include
endif

# openCV includes and libraries
ifdef OPENCV_DIRECTORY
	INCLUDESDIRS  += -I$(OPENCV_DIRECTORY)/include/ -I$(OPENCV_DIRECTORY)/modules/core/include 
	INCLUDESDIRS  += -I$(OPENCV_DIRECTORY)/modules/video/include -I$(OPENCV_DIRECTORY)/modules/objdetect/include 
	INCLUDESDIRS  += -I$(OPENCV_DIRECTORY)/modules/ml/include/ -I$(OPENCV_DIRECTORY)/modules/photo/include/ 
	INCLUDESDIRS  += -I$(OPENCV_DIRECTORY)/modules/imgproc/include/
	INCLUDESDIRS  += -I$(OPENCV_DIRECTORY)

	CXXFLAGS += -DSHOTTOPTAGGER_DO_OPENCV
	LIBS     += -L$(OPENCV_DIRECTORY)/lib -lopencv_ml -lopencv_core
endif

# xgboost includes and libraries
ifdef XGBOOST_DIRECTORY
	INCLUDESDIRS  += -I$(XGBOOST_DIRECTORY) -I$(XGBOOST_DIRECTORY)/rabit/include

	CXXFLAGS += -DDOXGBOOST
	LIBS     += -L$(XGBOOST_DIRECTORY)/lib -lxgboost
endif

# tensorflow includes and libraries
ifdef TENSORFLOW_DIR
	INCLUDESDIRS  += -I$(TENSORFLOW_DIR)/include

	CXXFLAGS += -DDOTENSORFLOW
	LIBS     += -L$(TENSORFLOW_DIR)/lib -ltensorflow
endif

LHAPDFLIB  = -L$(LHAPDF_DATA_PATH)/../../lib -lLHAPDF

FILES = $(wildcard $(SDIR)/*.cc)
OBJS := $(FILES:$(SDIR)/%.cc=$(ODIR)/%.o)

PROGRAMS = tupleReadTest nEvts deepTrim AliasTest bTagEfficiencyCalc ISRJetsProducer # basicCheck makeCombPlots makeSignalHistograms signalScan makeSignalCards makeUnblindPlots batchSignalPlots

all: mkobj SusyAnaTools sampPyWrap $(PROGRAMS)

mkobj:
	@mkdir -p obj

SusyAnaTools : $(OBJS)
	ar rcs $(ODIR)/lib$@.a $^
	gcc -shared -o $(ODIR)/lib$@.$(LIBSUFFIX) $^

#code to compile shared library to link samples to python
sampPyWrap: $(ODIR)/samplesModule.$(LIBSUFFIX)

$(ODIR)/samplesModule.$(LIBSUFFIX): $(ODIR)/samplesPyWrap.o $(ODIR)/samplesModulePyWrap.o
	$(CXX) -shared -o $@ $^

$(ODIR)/samplesPyWrap.o: $(SDIR)/samples.cc $(SDIR)/samples.h 
	$(CXX) --std=c++11 -c -fPIC -o $@ $<

$(ODIR)/samplesModulePyWrap.o: $(SDIR)/samplesModule.cc
	$(CXX) --std=c++11 -c -fPIC -o $@ $<

$(ODIR)/%.o : $(SDIR)/%.C
	$(CXX) $(CXXFLAGS) $(CXXDEPFLAGS) -o $@ -c $<

$(ODIR)/%.o : $(SDIR)/%.cc
	$(CXX) $(CXXFLAGS) $(CXXDEPFLAGS) -o $@ -c $<

$(ODIR)/%.o : $(SDIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(CXXDEPFLAGS) -o $@ -c $<

tupleReadTest: $(ODIR)/baselineDef.o $(ODIR)/NTupleReader.o $(ODIR)/SATException.o $(ODIR)/PDFUncertainty.o $(ODIR)/searchBins.o $(ODIR)/tupleReadTest.o $(ODIR)/customize.o $(ODIR)/BTagCorrector.o $(ODIR)/BTagCalibrationStandalone.o $(ODIR)/ISRCorrector.o
	$(LD) $^ $(LIBS) $(LHAPDFLIB) -o $@

nEvts: $(ODIR)/nEvts.o $(ODIR)/samples.o $(ODIR)/NTupleReader.o $(ODIR)/SATException.o
	$(LD) $^ $(LIBS) $(LHAPDFLIB) -o $@

deepTrim: $(ODIR)/DeepTrim.o $(ODIR)/samples.o $(ODIR)/NTupleReader.o $(ODIR)/customize.o $(ODIR)/baselineDef.o $(ODIR)/SATException.o
	$(LD) $^ $(LIBS) $(LHAPDFLIB) -o $@

basicCheck: $(ODIR)/baselineDef.o $(ODIR)/NTupleReader.o $(ODIR)/SATException.o $(ODIR)/samples.o $(ODIR)/basicCheck.o $(ODIR)/PDFUncertainty.o $(ODIR)/searchBins.o $(ODIR)/customize.o
	$(LD) $^ $(LIBS) $(LHAPDFLIB) -o $@

makeCombPlots: $(ODIR)/samples.o $(ODIR)/SATException.o $(ODIR)/makeCombPlots.o $(ODIR)/searchBins.o
	$(LD) $^ $(LIBS) -o $@

signalScan: $(ODIR)/baselineDef.o $(ODIR)/samples.o $(ODIR)/NTupleReader.o $(ODIR)/SATException.o $(ODIR)/signalScan.o $(ODIR)/PDFUncertainty.o $(ODIR)/BTagCorrector.o $(ODIR)/BTagCalibrationStandalone.o $(ODIR)/searchBins.o $(ODIR)/ISRCorrector.o $(ODIR)/customize.o
	$(LD) $^ $(LIBS) $(LHAPDFLIB) -o $@

makeSignalCards: $(ODIR)/samples.o $(ODIR)/SATException.o $(ODIR)/makeSignalCards.o $(ODIR)/searchBins.o
	$(LD) $^ $(LIBS) -o $@

batchSignalPlots: $(ODIR)/samples.o $(ODIR)/batchSignalPlots.o
	$(LD) $^ $(LIBS) -o $@

makeSignalHistograms: $(ODIR)/baselineDef.o $(ODIR)/samples.o $(ODIR)/NTupleReader.o $(ODIR)/SATException.o $(ODIR)/makeSignalHistograms.o $(ODIR)/searchBins.o $(ODIR)/customize.o
	$(LD) $^ $(LIBS) -o $@

makeUnblindPlots: $(ODIR)/SATException.o $(ODIR)/makeUnblindPlots.o $(ODIR)/searchBins.o
	$(LD) $^ $(LIBS) -o $@

bTagEfficiencyCalc:  $(ODIR)/samples.o $(ODIR)/NTupleReader.o $(ODIR)/bTagEfficiencyCalc.o $(ODIR)/SATException.o $(ODIR)/baselineDef.o $(ODIR)/customize.o
	$(LD) $^ $(LIBS) -o $@

ISRJetsProducer:  $(ODIR)/samples.o $(ODIR)/NTupleReader.o $(ODIR)/ISRJetsProducer.o $(ODIR)/SATException.o $(ODIR)/baselineDef.o $(ODIR)/customize.o
	$(LD) $^ $(LIBS) -o $@

#DataMC: $(ODIR)/baselineDef.o $(ODIR)/NTupleReader.o $(ODIR)/SATException.o $(ODIR)/samples.o $(ODIR)/customize.o $(ODIR)/customize.o $(ODIR)/searchBins.o $(ODIR)/DataMC.o
#	$(LD) $^ $(LIBS) -o $@

AliasTest: $(ODIR)/NTupleReader.o $(ODIR)/SATException.o $(ODIR)/StopleAlias.o  $(ODIR)/AliasTest.o  
	$(LD) $^ $(LIBS) -o $@

stopNTuple_skim: $(ODIR)/stopNTuple_skim.o
	$(LD) $^ $(LIBS_base) -o $@

make_signalcards: $(ODIR)/make_signalcards.o
	$(LD) $^ $(LIBS_base) -o $@

clean:
	rm -f $(ODIR)/*.a $(ODIR)/*.so $(ODIR)/*.o $(ODIR)/*.d $(PROGRAMS) core 

-include $(ODIR)/*.d
