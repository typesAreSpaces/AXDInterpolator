include common.mk

DEPENDENCIES=util AXDSignature Preprocess SeparatedPair \
						 AXDInterpolant InputFormulaParser TODO
INCLUDES=-I$(Z3_IDIR) $(DEPENDENCIES:%=-Isrc/%)
BUILD_DEPENDENCIES=$(DEPENDENCIES:%=$(SDIR)/%/done)

#all: $(AXD_INTERPOLATOR)
all: tests/one $(TAGS)
#all: tests/all
#all: tests/print_all

# -------------------------------------------------
#  Rules to build the project
$(Z3_DIR)/README.md:
	git submodule update --init --remote $(Z3_DIR)
	cd $(Z3_DIR); git checkout master

$(LDIR)/libz3.$(SO_EXT): $(Z3_DIR)/README.md
	mkdir -p $(LDIR)
	cd $(Z3_DIR);\
		$(PYTHON_CMD) scripts/mk_make.py \
		--prefix=$(CURRENT_DIR)
	$(MAKE) -C $(Z3_DIR)/build install

$(SDIR)/util/done: $(LDIR)/libz3.$(SO_EXT) \
	$(SDIR)/util/*.cpp
	$(MAKE) -C $(SDIR)/util

$(SDIR)/AXDSignature/done: $(LDIR)/libz3.$(SO_EXT) \
	$(SDIR)/AXDSignature/*.cpp
	$(MAKE) -C $(SDIR)/AXDSignature

$(SDIR)/Preprocess/done: $(LDIR)/libz3.$(SO_EXT) \
	$(SDIR)/Preprocess/*.cpp
	$(MAKE) -C $(SDIR)/Preprocess

$(SDIR)/SeparatedPair/done: $(LDIR)/libz3.$(SO_EXT) \
	$(SDIR)/SeparatedPair/*.cpp
	$(MAKE) -C $(SDIR)/SeparatedPair

$(SDIR)/AXDInterpolant/done: $(LDIR)/libz3.$(SO_EXT) \
	$(SDIR)/AXDInterpolant/*.cpp
	$(MAKE) -C $(SDIR)/AXDInterpolant

$(SDIR)/InputFormulaParser/done: $(LDIR)/libz3.$(SO_EXT) \
	$(SDIR)/InputFormulaParser/*.cpp
	$(MAKE) -C $(SDIR)/InputFormulaParser

$(SDIR)/TODO/done: $(LDIR)/libz3.$(SO_EXT) \
	$(SDIR)/TODO/*.cpp
	$(MAKE) -C $(SDIR)/TODO

$(ODIR)/%.o: $(SDIR)/%.cpp \
	$(LDIR)/libz3.$(SO_EXT) 
	mkdir -p $(ODIR) 
	$(CXX) $(CXXFLAGS) -c -o $@ $(FLAGS) \
		$(INCLUDES) $<

debug: CXXFLAGS += -DDEBUG -g
debug: CCFLAGS += -DDEBUG -g
debug: $(AXD_INTERPOLATOR)

$(AXD_INTERPOLATOR): $(BUILD_DEPENDENCIES) $(ODIR)/main.o
	mkdir -p $(BDIR)
	$(CXX) $(CXXFLAGS) -o $@ \
		$(wildcard $(ODIR)/*.o) \
		$(LDIR)/libz3.$(SO_EXT) \
		$(FLAGS) -lpthread
# -------------------------------------------------

# ---------------------------------------
# Generate TAGS
$(TAGS): $(AXD_INTERPOLATOR)
	compiledb -n make
	$(MAKE) -C $(SDIR)/AXDSignature \
		compile_commands.json
	$(MAKE) -C $(SDIR)/Preprocess \
		compile_commands.json
	$(MAKE) -C $(SDIR)/SeparatedPair \
		compile_commands.json
	$(MAKE) -C $(SDIR)/AXDInterpolant \
		compile_commands.json
	$(MAKE) -C $(SDIR)/InputFormulaParser \
		compile_commands.json
	$(MAKE) -C $(SDIR)/TODO \
		compile_commands.json
# ---------------------------------------

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
