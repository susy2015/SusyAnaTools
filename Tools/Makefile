IDIR       = .
ODIR       = obj
SDIR       = .

CXX        = g++

CXXFLAGS  += -I. -std=c++0x -I$(CMSSW_BASE)/src/
## Optimization flag
CXXFLAGS += -g #-O3
## Enable the maximun warning
#CXXFLAGS += -Wall -Wextra -Weffc++ -g

## Include ROOT
CXXFLAGS  += $(shell root-config --cflags)

CXXDEPFLAGS = -MMD -MP

LD         = g++
LDFLAGS    =

LIBS       = $(shell root-config --glibs)
MT2LIB     = -L$(CMSSW_RELEASE_BASE)/../../../external/tbb/42_20131118oss/lib/libtbb.so.2 -L$(CMSSW_BASE)/lib/${SCRAM_ARCH}/ -lrecipeAUXOxbridgeMT2

#OBJS       = $(patsubst %, $(ODIR)/%, $(OBJ))
PROGRAMS = harvestHists tupleTest makePlots #baselineDef

all: $(PROGRAMS)

$(ODIR)/%.o : $(SDIR)/%.C
	$(CXX) $(CXXFLAGS) $(CXXDEPFLAGS) -o $@ -c $<

$(ODIR)/%.o : $(SDIR)/%.cc
	$(CXX) $(CXXFLAGS) $(CXXDEPFLAGS) -o $@ -c $<

harvestHists: $(ODIR)/harvestHists.o
	$(LD) $^ $(LIBS) -o $@

tupleTest: $(ODIR)/NTupleReader.o $(ODIR)/tupleReadTest.o 
	$(LD) $^ $(LIBS) -o $@

makePlots: $(ODIR)/NTupleReader.o $(ODIR)/samples.o $(ODIR)/Plotter.o $(ODIR)/MakePlots.o
	$(LD) $^ $(LIBS) $(MT2LIB) -o $@

baselineDef: $(ODIR)/NTupleReader.o $(ODIR)/baselineDef.o
	$(LD) $^ $(LIBS) $(MT2LIB) -o $@

clean:
	rm -f $(ODIR)/*.o $(ODIR)/*.d $(PROGRAMS) core 

-include $(ODIR)/*.d
