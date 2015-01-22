IDIR       = .
ODIR       = obj
SDIR       = .

#CXX       = ~/.vim/bundle/ClangComplete/bin/cc_args.py g++
CXX        = g++

CXXFLAGS  += -Iinc -std=c++0x
## Optimization flag
CXXFLAGS += -O3
## Enable the maximun warning
#CXXFLAGS += -Wall -Wextra -Weffc++ -g

## Include ROOT
CXXFLAGS  += $(shell root-config --cflags)

LD         = g++
LDFLAGS    =

LIBS       = $(shell root-config --glibs)


#============================================================================#
#-----------------------------   Include Boost   ----------------------------#
#============================================================================#
ifeq ($(shell echo $(BOOSTROOT)), '')
  CPPFLAGS  += -I$(shell echo $(BOOSTROOT))
endif

OBJS       = $(patsubst %, $(ODIR)/%, $(OBJ))
PROGRAMS = stopTupleTest makePlots

all: $(PROGRAMS)

$(ODIR)/%.o : $(SDIR)/%.C $(HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $@ -c $<

$(ODIR)/%.o : $(SDIR)/%.cc $(HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $@ -c $<

stopTupleTest: $(ODIR)/NTupleReader.o $(ODIR)/tupleReadTest.o
	$(LD) $^ $(LIBS) -o $@

makePlots: $(ODIR)/NTupleReader.o $(ODIR)/Plotter.o $(ODIR)/MakePlots.o
	$(LD) $^ $(LIBS) -o $@

clean:
	rm -f $(ODIR)/*.o $(PROGRAMS) core 


