IDIR       = .
ODIR       = obj
SDIR       = $(CMSSW_BASE)/src/SusyAnaTools/Tools/

CXX        = g++

CXXFLAGS += -I. -std=c++0x -I$(CMSSW_BASE)/src/ -I$(LHAPDF_DATA_PATH)/../../include -I$(CMSSW_RELEASE_BASE)/src 
CXXFLAGS += -I$(shell scram tool info fastjet | grep '^INCLUDE' | sed 's/INCLUDE=//')
CXXFLAGS += -I$(shell scram tool info boost | grep '^INCLUDE' | sed 's/INCLUDE=//')
## Optimization flag
CXXFLAGS += -g -fPIC
## Enable the maximun warning
#CXXFLAGS += -Wall -Wextra -Weffc++ -g

## Include ROOT
CXXFLAGS  += $(shell root-config --cflags)

CXXDEPFLAGS = -MMD -MP

LD         = g++
LDFLAGS    =

OPENCV_DIRECTORY = $(CMSSW_BASE)/src/opencv
LIBS       = $(shell root-config --glibs)
LIBS      += -L$(CMSSW_BASE)/lib/${SCRAM_ARCH}/ -lTopTaggerTopTagger -lTopTaggerCfgParser
LIBS      += -L$(OPENCV_DIRECTORY)/lib/ -lopencv_ml -lopencv_core

MT2LIB     = -L$(CMSSW_BASE)/lib/${SCRAM_ARCH}/ -lrecipeAUXOxbridgeMT2 -L$(CMSSW_RELEASE_BASE)/lib/${SCRAM_ARCH}/ -lPhysicsToolsHeppy
LHAPDFLIB  = -L$(LHAPDF_DATA_PATH)/../../lib -lLHAPDF

FILES = $(wildcard $(SDIR)/*.cc)
OBJS := $(FILES:$(SDIR)/%.cc=$(ODIR)/%.o)

PROGRAMS = tupleTest nEvts basicCheck makeCombPlots makeSignalHistograms signalScan makeSignalCards makeUnblindPlots batchSignalPlots DataMC

all: mkobj SusyAnaTools sampPyWrap $(PROGRAMS)

mkobj:
	@mkdir -p obj

SusyAnaTools : $(OBJS)
	ar rcs $(ODIR)/lib$@.a $^
	gcc -shared -o $(ODIR)/lib$@.so $^

#code to compile shared library to link samples to python
sampPyWrap: $(ODIR)/samplesModule.so

$(ODIR)/samplesModule.so: $(ODIR)/samplesPyWrap.o $(ODIR)/samplesModulePyWrap.o
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

tupleTest: $(ODIR)/baselineDef.o $(ODIR)/NTupleReader.o $(ODIR)/SATException.o $(ODIR)/PDFUncertainty.o $(ODIR)/searchBins.o $(ODIR)/tupleReadTest.o $(ODIR)/customize.o
	$(LD) $^ $(LIBS) $(MT2LIB) $(LHAPDFLIB) -o $@

nEvts: $(ODIR)/nEvts.o $(ODIR)/samples.o $(ODIR)/NTupleReader.o $(ODIR)/SATException.o
	$(LD) $^ $(LIBS) $(MT2LIB) $(LHAPDFLIB) -o $@

basicCheck: $(ODIR)/baselineDef.o $(ODIR)/NTupleReader.o $(ODIR)/SATException.o $(ODIR)/samples.o $(ODIR)/basicCheck.o $(ODIR)/PDFUncertainty.o $(ODIR)/searchBins.o $(ODIR)/customize.o
	$(LD) $^ $(LIBS) $(MT2LIB) $(LHAPDFLIB) -o $@

makeCombPlots: $(ODIR)/samples.o $(ODIR)/makeCombPlots.o $(ODIR)/searchBins.o
	$(LD) $^ $(LIBS) $(MT2LIB) -o $@

signalScan: $(ODIR)/baselineDef.o $(ODIR)/samples.o $(ODIR)/NTupleReader.o $(ODIR)/SATException.o $(ODIR)/signalScan.o $(ODIR)/PDFUncertainty.o $(ODIR)/BTagCorrector.o $(ODIR)/BTagCalibrationStandalone.o $(ODIR)/searchBins.o $(ODIR)/ISRCorrector.o $(ODIR)/customize.o
	$(LD) $^ $(LIBS) $(MT2LIB) $(LHAPDFLIB) -o $@

makeSignalCards: $(ODIR)/samples.o $(ODIR)/makeSignalCards.o $(ODIR)/searchBins.o
	$(LD) $^ $(LIBS) $(MT2LIB) -o $@

batchSignalPlots: $(ODIR)/samples.o $(ODIR)/batchSignalPlots.o
	$(LD) $^ $(LIBS) $(MT2LIB) -o $@

makeSignalHistograms: $(ODIR)/baselineDef.o $(ODIR)/samples.o $(ODIR)/NTupleReader.o $(ODIR)/SATException.o $(ODIR)/makeSignalHistograms.o $(ODIR)/searchBins.o $(ODIR)/customize.o
	$(LD) $^ $(LIBS) $(MT2LIB) -o $@

makeUnblindPlots: $(ODIR)/makeUnblindPlots.o $(ODIR)/searchBins.o
	$(LD) $^ $(LIBS) $(MT2LIB) -o $@

DataMC: $(ODIR)/baselineDef.o $(ODIR)/NTupleReader.o $(ODIR)/SATException.o $(ODIR)/samples.o $(ODIR)/customize.o $(ODIR)/customize.o $(ODIR)/searchBins.o $(ODIR)/DataMC.o
	$(LD) $^ $(LIBS) $(MT2LIB) -o $@

clean:
	rm -f $(ODIR)/*.a $(ODIR)/*.so $(ODIR)/*.o $(ODIR)/*.d $(PROGRAMS) core 

-include $(ODIR)/*.d
