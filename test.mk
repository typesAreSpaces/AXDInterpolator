#  Rules to test a single or many smt2 files

#-- Interpolation engines
METHOD=0 # Z3
#METHOD=1 # MATHSAT
#METHOD=2 # SMTINTERPOL

ALLOWED_ATTEMPS=1000000

#-- Supported Theories
#THEORY=QF_TO
#THEORY=QF_IDL
#THEORY=QF_UTVPI
THEORY=QF_LIA

#-- Sample files
#FILE_TEST=$(TEST_DIR)/relax-1.c_valid-memsafety.prp.smt2
#FILE_TEST=$(TEST_DIR)/array_tiling_poly6.c_unreach-call.prp.smt2
#FILE_TEST=$(TEST_DIR)/simple.smt2
#FILE_TEST=$(TEST_DIR)/simple2.smt2
#FILE_TEST=$(TEST_DIR)/simple3.smt2
#FILE_TEST=$(TEST_DIR)/simple4.smt2
#FILE_TEST=$(TEST_DIR)/ijcar_2018_paper_example4_n_4.smt2
#FILE_TEST=$(TEST_DIR)/length_example.smt2
#FILE_TEST=$(TEST_DIR)/maxdiff_paper_example_compact.smt2
#FILE_TEST=$(TEST_DIR)/maxdiff_paper_example_another_another.smt2
#FILE_TEST=$(TEST_DIR)/maxdiff_paper_example.smt2
#FILE_TEST=$(TEST_DIR)/jhala.smt2
FILE_TEST=$(TEST_DIR)/strcpy_example_variant_1.smt2
#FILE_TEST=$(TEST_DIR)/strcpy_example_variant_2.smt2
#FILE_TEST=$(TEST_DIR)/strcpy_example_variant_3.smt2

# ---------------------------------------------------------
tests/one: $(AXD_INTERPOLATOR)
	$(AXD_INTERPOLATOR) \
		$(THEORY) $(FILE_TEST) $(METHOD) $(ALLOWED_ATTEMPS)
	rm -rf tests/*.o $@

tests/all: $(AXD_INTERPOLATOR)
	for smt_file in $(TEST_DIR)/*.smt2; do \
		$(AXD_INTERPOLATOR) \
		$(THEORY) $${smt_file} $(METHOD) $(ALLOWED_ATTEMPS) ; \
		done
	rm -rf tests/*.o $@

tests/print_all: $(AXD_INTERPOLATOR)
	for smt_file in $(TEST_DIR)/*.smt2; do \
		if [ "${METHOD}" = "0" ]; \
		then METHOD_NAME="Z3"; \
		else \
		if [ "${METHOD}" = "1" ]; \
		then METHOD_NAME="MATHSAT"; \
		else METHOD_NAME="SMTINTERPOL"; \
		fi \
		fi; \
		$(AXD_INTERPOLATOR) \
		$(THEORY) $${smt_file} $(METHOD) $(ALLOWED_ATTEMPS) \
		> $${smt_file}_${THEORY}_$${METHOD_NAME}_output.txt ; \
		done
	rm -rf tests/*.o $@
# ---------------------------------------------------------

# -----------------------------
#  Check output
check: 
	make -C ./output

mathsat_check: 
	SMT_SOLVER=MATHSAT make check

z3_check: 
	SMT_SOLVER=Z3 make check
# -----------------------------
