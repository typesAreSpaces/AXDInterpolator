include ../../common.mk

DEPENDENCIES=AXDSignature Preprocess StandardInput

INCLUDES=-I$(Z3_IDIR) $(DEPENDENCIES:%=-I../%)
CPP_FILES=$(wildcard *.cpp)
TODO=$(CPP_FILES:%.cpp=$(ODIR)/%.o)
LOCAL_TAGS=compile_commands.json

$(SDIR)/AXDInterpolant/done: ${TODO}
	touch $(SDIR)/AXDInterpolant/done

AXDInterpolant.h: _AXDInterpolant.h
	perl -pe "s|replace_once|$(CURRENT_DIR)|g" $< > $@

$(ODIR)/%.o: %.cpp AXDInterpolant.h $(LDIR)/libz3.$(SO_EXT)
	@mkdir -p $(ODIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $(FLAGS) \
		$(INCLUDES) $<

$(LOCAL_TAGS):
	compiledb -n make

.PHONY: clean

clean:
	rm -rf AXDInterpolant.h
	rm -rf ${LOCAL_TAGS}
	rm -rf done
