include common.mk

DEPENDENCIES=util AXDSignature Preprocess SeparatedPair \
						 AXDInterpolant InputFormulaParser TODO
INCLUDES=-I$(Z3_IDIR) $(DEPENDENCIES:%=-Isrc/%)

#all: $(AXD_INTERPOLATOR)
all: tests/one $(TAGS)
#all: util \
	#AXDSignature \
	#Preprocess \
	#SeparatedPair \
	#AXDInterpolant \
	#InputFormulaParser \
	#TODO
#all: tests/all
#all: tests/print_all

# ----------------------------------------------
#  Rules to build the project
$(Z3_DIR)/README.md:
	git submodule update --init --remote $(Z3_DIR)
	cd $(Z3_DIR); git checkout master

$(LDIR)/libz3.$(SO_EXT): $(Z3_DIR)/README.md
	mkdir -p $(LDIR)
	cd $(Z3_DIR);\
		$(PYTHON_CMD) scripts/mk_make.py \
		--prefix=$(CURRENT_DIR);\
		cd build; $(MAKE) install

.PHONY: $(DEPENDENCIES)

util: $(LDIR)/libz3.$(SO_EXT) 
	$(MAKE) -C $(SDIR)/$@

AXDSignature: $(LDIR)/libz3.$(SO_EXT) 
	$(MAKE) -C $(SDIR)/$@

Preprocess: $(LDIR)/libz3.$(SO_EXT) 
	$(MAKE) -C $(SDIR)/$@

SeparatedPair: $(LDIR)/libz3.$(SO_EXT) 
	$(MAKE) -C $(SDIR)/$@

AXDInterpolant: $(LDIR)/libz3.$(SO_EXT) 
	$(MAKE) -C $(SDIR)/$@ 

InputFormulaParser: $(LDIR)/libz3.$(SO_EXT) 
	$(MAKE) -C $(SDIR)/$@ 

TODO: $(LDIR)/libz3.$(SO_EXT) 
	$(MAKE) -C $(SDIR)/$@ 

$(ODIR)/%.o: $(SDIR)/%.cpp \
	$(LDIR)/libz3.$(SO_EXT) 
	mkdir -p $(ODIR) 
	$(CXX) $(CXXFLAGS) -c -o $@ $(FLAGS) \
		$(INCLUDES) $<

debug: CXXFLAGS += -DDEBUG -g
debug: CCFLAGS += -DDEBUG -g
debug: $(AXD_INTERPOLATOR)

$(AXD_INTERPOLATOR): $(DEPENDENCIES) $(ODIR)/main.o
	mkdir -p $(BDIR)
	$(CXX) $(CXXFLAGS) -o $@ \
		$(wildcard $(ODIR)/*.o) \
		$(LDIR)/libz3.$(SO_EXT) \
		$(FLAGS) -lpthread
# ----------------------------------------------

# --------------------------
# Generate TAGS
$(TAGS): $(AXD_INTERPOLATOR)
	compiledb -n make
# --------------------------

include test.mk

# -----------------------------------
#  Cleaning
.PHONY: clean z3_clean deep_clean

clean:
	rm -rf $(ODIR) output/*.smt2
	rm -rf $(TEST_DIR)/*.txt
	rm -rf $(AXD_INTERPOLATOR)
	rm -rf $(TAGS)
	cd output; $(MAKE) clean
	$(MAKE) clean -C $(SDIR)/util
	$(MAKE) clean -C $(SDIR)/AXDSignature
	$(MAKE) clean -C $(SDIR)/Preprocess
	$(MAKE) clean -C $(SDIR)/SeparatedPair
	$(MAKE) clean -C $(SDIR)/AXDInterpolant
	$(MAKE) clean -C $(SDIR)/InputFormulaParser
	$(MAKE) clean -C $(SDIR)/TODO

z3_clean:
	if [ -d "$(Z3_DIR)/build" ]; then \
		cd $(Z3_DIR)/build; \
		$(MAKE) uninstall; \
		fi;
	rm -rf $(LDIR)
	rm -rf $(Z3_DIR)

deep_clean: clean z3_clean
# -----------------------------------
