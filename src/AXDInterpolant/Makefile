include ../../common.mk

DEPENDENCIES=util AXDSignature Preprocess SeparatedPair

INCLUDES=-I$(Z3_IDIR) $(DEPENDENCIES:%=-I../%)
CPP_FILES=$(wildcard *.cpp)
TODO=$(CPP_FILES:%.cpp=$(ODIR)/%.o)

all: AXDInterpolant.h ${TODO} 

AXDInterpolant.h: _AXDInterpolant.h
	perl -pe "s|replace_once|$(CURRENT_DIR)|g" $< > $@

$(ODIR)/%.o: %.cpp $(LDIR)/libz3.$(SO_EXT)
	@mkdir -p $(ODIR)
	@$(CXX) $(CXXFLAGS) -c -o $@ $(FLAGS) \
		$(INCLUDES) $<
