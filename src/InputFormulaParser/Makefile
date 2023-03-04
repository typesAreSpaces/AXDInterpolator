include ../../common.mk

DEPENDENCIES=util AXDSignature Preprocess SeparatedPair AXDInterpolant

INCLUDES=-I$(Z3_IDIR) $(DEPENDENCIES:%=-I../%)
CPP_FILES=$(wildcard *.cpp)
TODO=$(CPP_FILES:%.cpp=$(ODIR)/%.o)
LOCAL_TAGS=compile_commands.json

$(SDIR)/InputFormulaParser/done: ${TODO}
	touch $(SDIR)/InputFormulaParser/done

$(ODIR)/%.o: %.cpp $(LDIR)/libz3.$(SO_EXT)
	@mkdir -p $(ODIR)
	@$(CXX) $(CXXFLAGS) -c -o $@ $(FLAGS) \
		$(INCLUDES) $<

$(LOCAL_TAGS):
	compiledb -n make

.PHONY: clean

clean:
	rm -rf ${LOCAL_TAGS}
