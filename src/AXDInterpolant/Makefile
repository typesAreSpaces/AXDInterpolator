include ../../common.mk

CPP_FILES=$(wildcard *.cpp)
TODO=$(CPP_FILES:%.cpp=$(ODIR)/%.o)

all: AXDInterpolant.h ${TODO} 

AXDInterpolant.h: _AXDInterpolant.h
	perl -pe "s|replace_once|$(CURRENT_DIR)|g" $< > $@

$(ODIR)/%.o: %.cpp $(LDIR)/libz3.$(SO_EXT)
	$(CXX) $(CXXFLAGS) -c -o $@ $(FLAGS) \
		-I$(Z3_IDIR) -I../AXDSignature \
		-I../Preprocess -I../SeparatedPair \
		$<
