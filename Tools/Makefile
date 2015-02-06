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

#OBJS       = $(patsubst %, $(ODIR)/%, $(OBJ))
PROGRAMS = tupleTest makePlots #baselineDef

all: $(PROGRAMS)

$(ODIR)/%.o : $(SDIR)/%.C
	$(CXX) $(CXXFLAGS) $(CXXDEPFLAGS) -o $@ -c $<

$(ODIR)/%.o : $(SDIR)/%.cc
	$(CXX) $(CXXFLAGS) $(CXXDEPFLAGS) -o $@ -c $<

tupleTest: $(ODIR)/NTupleReader.o $(ODIR)/tupleReadTest.o
	$(LD) $^ $(LIBS) -o $@

makePlots: $(ODIR)/NTupleReader.o $(ODIR)/Plotter.o $(ODIR)/MakePlots.o
	$(LD) $^ $(LIBS) -L$(CMSSW_BASE)/lib/${SCRAM_ARCH}/ -lrecipeAUXOxbridgeMT2 -o $@

baselineDef: $(ODIR)/NTupleReader.o $(ODIR)/baselineDef.o
	$(LD) $^ $(LIBS) -L$(CMSSW_BASE)/lib/${SCRAM_ARCH}/ -lrecipeAUXOxbridgeMT2 -o $@

clean:
	rm -f $(ODIR)/*.o $(ODIR)/*.d $(PROGRAMS) core 

-include $(ODIR)/*.d
