include common.mk

LINKS=-I$(Z3_IDIR) -Isrc \
			-Isrc/AXDInterpolant -Isrc/StandardInput \
			-Isrc/Preprocess -Isrc/AXDSignature \
			-Isrc/util

#all: $(AXD_INTERPOLATOR)
all: tests/one $(TAGS)
#all: tests/all
#all: tests/print_all

# ---------------------------------------------------------
#  Rules to build the project
$(Z3_DIR)/README.md:
	git submodule update --init --remote $(Z3_DIR) 
	cd $(Z3_DIR); git checkout master

$(LDIR)/libz3.$(SO_EXT): $(Z3_DIR)/README.md
	mkdir -p $(LDIR)
	cd $(Z3_DIR);\
		$(PYTHON_CMD) scripts/mk_make.py \
		--prefix=$(CURRENT_DIR);\
		cd build; make install -j$(NUM_PROCS)

.PHONY: AXDSignature Preprocess \
	StandardInput AXDInterpolant \
	util

AXDSignature: $(LDIR)/libz3.$(SO_EXT) 
	make -C $(SDIR)/$@

Preprocess: $(LDIR)/libz3.$(SO_EXT) 
	make -C $(SDIR)/$@

StandardInput: $(LDIR)/libz3.$(SO_EXT) 
	make -C $(SDIR)/$@

AXDInterpolant: $(LDIR)/libz3.$(SO_EXT) 
	make -C $(SDIR)/$@ 

util: $(LDIR)/libz3.$(SO_EXT) 
	make -C $(SDIR)/$@

$(ODIR)/%.o: $(SDIR)/%.cpp $(LDIR)/libz3.$(SO_EXT) 
	mkdir -p $(ODIR) 
	$(CXX) $(CXXFLAGS) -c -o $@ $(FLAGS) \
		$(LINKS) $<

debug: CXXFLAGS += -DDEBUG -g
debug: CCFLAGS += -DDEBUG -g
debug: $(AXD_INTERPOLATOR)

$(AXD_INTERPOLATOR): AXDSignature Preprocess \
	StandardInput AXDInterpolant util \
	$(ODIR)/main.o
	mkdir -p $(BDIR)
	$(CXX) $(CXXFLAGS) -o $@ \
		$(wildcard $(ODIR)/*.o) $(LDIR)/libz3.$(SO_EXT) \
		$(FLAGS) -lpthread
# ---------------------------------------------------------

# ---------------------------------------------------------
# Generate TAGS
$(TAGS):
	compiledb -n make
# ---------------------------------------------------------

include test.mk

# ---------------------------------------------------------
#  Cleaning
.PHONY: clean z3_clean deep_clean

clean:
	rm -rf $(ODIR) output/*.smt2
	rm -rf $(TEST_DIR)/*.txt
	rm -rf $(AXD_INTERPOLATOR)
	rm -rf $(TAGS)
	cd output; make clean

z3_clean:
	if [ -d "$(Z3_DIR)/build" ]; then \
		cd $(Z3_DIR)/build; make uninstall; \
		fi;
	rm -rf $(LDIR)

deep_clean: clean z3_clean
# ---------------------------------------------------------
