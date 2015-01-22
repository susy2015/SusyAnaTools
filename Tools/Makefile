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


OBJ        = NTupleReader.o tupleReadTest.o
PROGRAM    = runStop

OBJS       = $(patsubst %, $(ODIR)/%, $(OBJ))

all:          $(PROGRAM)


$(ODIR)/%.o : $(SDIR)/%.C $(HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $@ -c $<

$(ODIR)/%.o : $(SDIR)/%.cc $(HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $@ -c $<


$(PROGRAM):  $(OBJS)
	@echo "Linking $(PROGRAM) ..."
	@$(LD) $(OBJS) $(LIBS) -o $(PROGRAM)
	@echo "done"

clean:;         @rm -f $(OBJS) $(PROGRAM) core 

