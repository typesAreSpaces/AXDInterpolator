TEST_DIR=$(CURRENT_DIR)/tests/smt2-files/caxd_examples

#  Rules to test a single or many smt2 files

#-- Interpolation engines
METHOD=0 # Z3
#METHOD=1 # MATHSAT
#METHOD=2 # SMTINTERPOL

#-- Supported Theories
#THEORY=QF_TO
#THEORY=QF_IDL
#THEORY=QF_UTVPI
THEORY=QF_LIA

#-- Sample files
FILE_TEST=$(TEST_DIR)/6_2.smt2

# ---------------------------------------------------------
tests/one: $(CAXD_INTERPOLATOR)
	$(CAXD_INTERPOLATOR) \
		$(THEORY) $(FILE_TEST) $(METHOD)
	rm -rf tests/*.o $@

tests/all: $(CAXD_INTERPOLATOR)
	for smt_file in $(TEST_DIR)/*.smt2; do \
		$(CAXD_INTERPOLATOR) \
		$(THEORY) $${smt_file} $(METHOD); \
		done
	rm -rf tests/*.o $@

tests/print_all: $(CAXD_INTERPOLATOR)
	for smt_file in $(TEST_DIR)/*.smt2; do \
		if [ "${METHOD}" = "0" ]; \
		then METHOD_NAME="Z3"; \
		else \
		if [ "${METHOD}" = "1" ]; \
		then METHOD_NAME="MATHSAT"; \
		else METHOD_NAME="SMTINTERPOL"; \
		fi \
		fi; \
		$(CAXD_INTERPOLATOR) \
		$(THEORY) $${smt_file} $(METHOD) \
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
