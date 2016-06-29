IDIR       = .
ODIR       = obj
SDIR       = .

CXX        = g++

#CXXFLAGS  += -I. -std=c++0x -I$(CMSSW_BASE)/src/ -I$(LHAPDF_DATA_PATH)/../../include -I$(CMSSW_RELEASE_BASE)/../../../external/fastjet/3.1.0-odfocd/include -I$(CMSSW_RELEASE_BASE)/src
CXXFLAGS  += -I. -std=c++0x -I$(CMSSW_BASE)/src/ -I$(LHAPDF_DATA_PATH)/../../include -I$(CMSSW_RELEASE_BASE)/src -I$(shell scram tool info fastjet | grep '^INCLUDE' | sed 's/INCLUDE=//')
## Optimization flag
CXXFLAGS += -g
## Enable the maximun warning
#CXXFLAGS += -Wall -Wextra -Weffc++ -g

## Include ROOT
CXXFLAGS  += $(shell root-config --cflags)

CXXDEPFLAGS = -MMD -MP

LD         = g++
LDFLAGS    =

LIBS       = $(shell root-config --glibs)
MT2LIB     = -L$(CMSSW_BASE)/lib/${SCRAM_ARCH}/ -lrecipeAUXOxbridgeMT2 -L$(CMSSW_RELEASE_BASE)/lib/${SCRAM_ARCH}/ -lPhysicsToolsHeppy
LHAPDFLIB  = -L$(LHAPDF_DATA_PATH)/../../lib -lLHAPDF
#OBJS       = $(patsubst %, $(ODIR)/%, $(OBJ))
#PROGRAMS = tupleTest nEvts basicCheck makeCombPlots makeSignalHistograms signalScan makeSignalCards batchSignalPlots
PROGRAMS = tupleTest nEvts makeCombPlots makeSignalHistograms signalScan basicCheck makeSignalCards makeUnblindPlots

all: mkobj sampPyWrap $(PROGRAMS)


mkobj:
	@mkdir -p obj

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

tupleTest: $(ODIR)/NTupleReader.o $(ODIR)/PDFUncertainty.o $(ODIR)/searchBins.o $(ODIR)/tupleReadTest.o 
	$(LD) $^ $(LIBS) $(MT2LIB) $(LHAPDFLIB) -o $@

nEvts: $(ODIR)/samples.o $(ODIR)/NTupleReader.o $(ODIR)/nEvts.o
	$(LD) $^ $(LIBS) -o $@

basicCheck: $(ODIR)/NTupleReader.o $(ODIR)/samples.o $(ODIR)/basicCheck.o $(ODIR)/PDFUncertainty.o $(ODIR)/searchBins.o
	$(LD) $^ $(LIBS) $(MT2LIB) $(LHAPDFLIB) -o $@

makeCombPlots: $(ODIR)/samples.o $(ODIR)/makeCombPlots.o $(ODIR)/searchBins.o
	$(LD) $^ $(LIBS) $(MT2LIB) -o $@

signalScan: $(ODIR)/samples.o $(ODIR)/NTupleReader.o $(ODIR)/signalScan.o $(ODIR)/PDFUncertainty.o $(ODIR)/BTagCorrector.o $(ODIR)/BTagCalibrationStandalone.o $(ODIR)/searchBins.o
	$(LD) $^ $(LIBS) $(MT2LIB) $(LHAPDFLIB) -o $@

makeSignalCards: $(ODIR)/samples.o $(ODIR)/makeSignalCards.o $(ODIR)/searchBins.o
	$(LD) $^ $(LIBS) $(MT2LIB) -o $@

batchSignalPlots: $(ODIR)/samples.o $(ODIR)/batchSignalPlots.o
	$(LD) $^ $(LIBS) $(MT2LIB) -o $@

makeSignalHistograms: $(ODIR)/samples.o $(ODIR)/NTupleReader.o $(ODIR)/makeSignalHistograms.o $(ODIR)/searchBins.o
	$(LD) $^ $(LIBS) $(MT2LIB) -o $@

makeUnblindPlots: $(ODIR)/makeUnblindPlots.o $(ODIR)/searchBins.o
	$(LD) $^ $(LIBS) $(MT2LIB) -o $@

clean:
	rm -f $(ODIR)/*.o $(ODIR)/*.d $(PROGRAMS) core 

-include $(ODIR)/*.d
