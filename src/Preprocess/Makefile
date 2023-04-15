include ../../common.mk

DEPENDENCIES=util AXDSignature

INCLUDES=-I$(Z3_IDIR) $(DEPENDENCIES:%=-I../%)
CPP_FILES=$(wildcard *.cpp)
TODO=$(CPP_FILES:%.cpp=$(ODIR)/%.o)
LOCAL_TAGS=compile_commands.json

$(SDIR)/Preprocess/done: ${TODO}
	touch $(SDIR)/Preprocess/done

$(ODIR)/%.o: %.cpp $(LDIR)/libz3.$(SO_EXT)
	@mkdir -p $(ODIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $(FLAGS) \
		$(INCLUDES) $<

$(LOCAL_TAGS):
	compiledb -n make

.PHONY: clean

clean:
	rm -rf ${LOCAL_TAGS}
	rm -rf done
