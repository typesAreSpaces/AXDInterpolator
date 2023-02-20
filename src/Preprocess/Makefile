include ../../common.mk

CPP_FILES=$(wildcard *.cpp)
TODO=$(CPP_FILES:%.cpp=$(ODIR)/%.o)

all: ${TODO}

$(ODIR)/%.o: %.cpp $(LDIR)/libz3.$(SO_EXT)
	mkdir -p $(ODIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $(FLAGS) \
		-I$(Z3_IDIR) -I../AXDSignature \
		$<
