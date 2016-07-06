IDIR       = .
ODIR       = obj
SDIR       = .

CXX        = g++

CXXFLAGS  += -I. -std=c++0x 
CXXFLAGS  += -I$(LHAPDF_INCLUDE)
## Optimization flag
CXXFLAGS += -g
## Enable the maximun warning
#CXXFLAGS += -Wall -Wextra -Weffc++ -g

## Include ROOT
CXXFLAGS  += $(shell root-config --cflags)
CXXFLAGS  += -I../../

CXXDEPFLAGS = -MMD -MP

LD         = g++
LDFLAGS    =

LIBS       = $(shell root-config --glibs)
MT2LIB     = -L$(CMSSW_BASE)/lib/${SCRAM_ARCH}/ -lrecipeAUXOxbridgeMT2

ifneq ($(shell echo $$CMSSW_BASE), )
  CXXFLAGS  += -I$(CMSSW_BASE)/src/
  MT2LIB     = -L$(CMSSW_BASE)/lib/${SCRAM_ARCH}/ -lrecipeAUXOxbridgeMT2
else
  CXXFLAGS  += -I../..
  MT2LIB     = -L../../recipeAUX/OxbridgeMT2/.libs -loxbridgekinetics-1.0 
endif

ifneq ($(shell echo $$LHAPDF_INCLUDE), )
  CXXFLAGS  += -isystem $(LHAPDF_INCLUDE)
  LIBS += -L$(LHAPDF_LIBDIR) -lLHAPDF
else
  CXXFLAGS  += -isystem $(LHAPDF_DATA_PATH)/../../include
  MT2LIB     = -L $(LHAPDF_DATA_PATH)/../../lib -lLHAPDF
endif


LHAPDFLIB  = -L$(LHAPDF_LIBDIR) -lLHAPDF
#OBJS       = $(patsubst %, $(ODIR)/%, $(OBJ))
PROGRAMS = tupleTest nEvts makeCombPlots
#PROGRAMS = tupleTest nEvts basicCheck makeCombPlots makeSignalHistograms signalScan makeSignalCards batchSignalPlots

FILES = $(wildcard *.cc)
OBJS  := $(FILES:%.cc=$(ODIR)/%.o)

all: mkobj $(OBJS) sampPyWrap $(PROGRAMS) 

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

tupleTest: $(ODIR)/NTupleReader.o $(ODIR)/tupleReadTest.o 
	$(LD) $^ $(LIBS) -o $@
	
#tupleTest: $(ODIR)/NTupleReader.o $(ODIR)/PDFUncertainty.o $(ODIR)/tupleReadTest.o 
	#$(LD) $^ $(LIBS) $(MT2LIB) $(LHAPDFLIB) -o $@

nEvts: $(ODIR)/samples.o $(ODIR)/NTupleReader.o $(ODIR)/nEvts.o
	$(LD) $^ $(LIBS) -o $@

#basicCheck: $(ODIR)/NTupleReader.o $(ODIR)/samples.o $(ODIR)/basicCheck.o $(ODIR)/customize.o $(ODIR)/baselineDef.o $(ODIR)/searchBins.o
	#$(LD) $^ $(LIBS) $(MT2LIB) -o $@

makeCombPlots: $(ODIR)/samples.o $(ODIR)/makeCombPlots.o
	$(LD) $^ $(LIBS) $(MT2LIB) -o $@

signalScan: $(ODIR)/samples.o $(ODIR)/NTupleReader.o $(ODIR)/signalScan.o $(ODIR)/PDFUncertainty.o $(ODIR)/BTagCorrector.o $(ODIR)/BTagCalibrationStandalone.o
	$(LD) $^ $(LIBS) $(MT2LIB) $(LHAPDFLIB) -o $@

makeSignalCards: $(ODIR)/samples.o $(ODIR)/makeSignalCards.o
	$(LD) $^ $(LIBS) $(MT2LIB) -o $@

batchSignalPlots: $(ODIR)/samples.o $(ODIR)/batchSignalPlots.o
	$(LD) $^ $(LIBS) $(MT2LIB) -o $@

makeSignalHistograms: $(ODIR)/samples.o $(ODIR)/NTupleReader.o $(ODIR)/makeSignalHistograms.o
	$(LD) $^ $(LIBS) $(MT2LIB) -o $@

stopNTuple_skim: $(ODIR)/stopNTuple_skim.o
	$(LD) $^ $(LIBS) $(MT2LIB) -o $@

clean:
	rm -f $(ODIR)/*.o $(ODIR)/*.d $(PROGRAMS) core 

-include $(ODIR)/*.d
