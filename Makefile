include common.mk

DEPENDENCIES=AXDSignature \
						 Preprocess StandardInput \
						 AXDInterpolant
INCLUDES=-I$(Z3_IDIR) $(DEPENDENCIES:%=-Isrc/%)
BUILD_DEPENDENCIES=$(DEPENDENCIES:%=$(SDIR)/%/done)

.PHONY: all execute release debug \
	clean z3_clean deep_clean \
	tags

all: execute tags

tags: $(TAGS)

execute: $(AXD_INTERPOLATOR)
	./run.sh
	# ./run.sh > result.txt

release: CXXFLAGS += -O2
release: CCFLAGS += -O2
release: $(AXD_INTERPOLATOR)

debug: CXXFLAGS += -DDEBUG -g
debug: CCFLAGS += -DDEBUG -g
debug: $(AXD_INTERPOLATOR)
	./debug.sh

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
	$(MAKE) -C $(SDIR)/util \
		CCFLAGS="$(CCFLAGS)" CXXFLAGS="$(CXXFLAGS)"

$(SDIR)/AXDSignature/done: $(LDIR)/libz3.$(SO_EXT) \
	$(SDIR)/AXDSignature/*.cpp
	$(MAKE) -C $(SDIR)/AXDSignature \
		CCFLAGS="$(CCFLAGS)" CXXFLAGS="$(CXXFLAGS)"

$(SDIR)/Preprocess/done: $(LDIR)/libz3.$(SO_EXT) \
	$(SDIR)/Preprocess/*.cpp
	$(MAKE) -C $(SDIR)/Preprocess \
		CCFLAGS="$(CCFLAGS)" CXXFLAGS="$(CXXFLAGS)"

$(SDIR)/StandardInput/done: $(LDIR)/libz3.$(SO_EXT) \
	$(SDIR)/StandardInput/*.cpp
	$(MAKE) -C $(SDIR)/StandardInput \
		CCFLAGS="$(CCFLAGS)" CXXFLAGS="$(CXXFLAGS)"

$(SDIR)/AXDInterpolant/done: $(LDIR)/libz3.$(SO_EXT) \
	$(SDIR)/AXDInterpolant/*.cpp
	$(MAKE) -C $(SDIR)/AXDInterpolant \
		CCFLAGS="$(CCFLAGS)" CXXFLAGS="$(CXXFLAGS)"

$(ODIR)/%.o:  $(SDIR)/%.cpp \
	$(LDIR)/libz3.$(SO_EXT)
	mkdir -p $(ODIR)
	# TODO: Fix this
	# Currently this slows down the compilation of main.cpp
	# so AXDInterpolant.h is created
	sleep 1
	$(CXX) $(CXXFLAGS) -c -o $@ $(FLAGS) \
		$(INCLUDES) $<

$(AXD_INTERPOLATOR): $(BUILD_DEPENDENCIES) \
	$(ODIR)/main.o 
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
	$(MAKE) -C $(SDIR)/StandardInput \
		compile_commands.json
	$(MAKE) -C $(SDIR)/AXDInterpolant \
		compile_commands.json
	# ---------------------------------------

include test.mk

# -----------------------------------
#  Cleaning
clean:
	rm -rf $(ODIR) output/*.smt2
	rm -rf $(TEST_DIR)/*.txt
	rm -rf $(AXD_INTERPOLATOR)
	rm -rf $(TAGS)
	$(MAKE) -C output clean
	$(MAKE) -C $(SDIR)/AXDSignature clean
	$(MAKE) -C $(SDIR)/Preprocess clean 
	$(MAKE) -C $(SDIR)/StandardInput clean 
	$(MAKE) -C $(SDIR)/AXDInterpolant clean 

z3_clean:
	if [ -d "$(Z3_DIR)/build" ]; then \
		cd $(Z3_DIR)/build; \
		$(MAKE) uninstall; \
		fi;
	rm -rf $(LDIR)
	rm -rf $(Z3_DIR)

deep_clean: clean z3_clean
	# -----------------------------------
