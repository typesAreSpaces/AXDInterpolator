#!/usr/bin/python3

import sys
import os.path
from ResultsReader import ResultsReader

if __name__ == "__main__":

    verification_files_dir = str(sys.argv[1])
    results_dir = str(sys.argv[2])

    results_path_memsafety = f"{results_dir}/benchmark_memsafety_results.txt"
    results_path_reachsafety = f"{results_dir}/benchmark_reachsafety_results.txt"
    db_path_memsafety = f"{verification_files_dir}/MemSafety-Arrays"
    db_path_reachsafety = f"{verification_files_dir}/ReachSafety-Arrays"

    if(not(os.path.isfile(results_path_memsafety))):
        sys.exit("benchmark_memsafety_results.txt not available")
    if(not(os.path.isdir(db_path_memsafety))):
        sys.exit("Memsafety database not available")

    if(not(os.path.isfile(results_path_reachsafety))):
        sys.exit("benchmark_reachsafety_results.txt not available")
    if(not(os.path.isfile(db_path_reachsafety))):
        sys.exit("Reachsafety database not available")

    memsafety_ = ResultsReader(results_path_memsafety, db_path_memsafety)
    reachsafety_ = ResultsReader(results_path_reachsafety, db_path_reachsafety)

    print(memsafety_.caxdInterpolatorTable("Memsafety-track results - CAXDInterpolator", "label1"))
    print("")
    print(memsafety_.axdInterpolatorTable("Memsafety-track results - AXDInterpolator", "label2"))
    print("")
    print(memsafety_.otherSolversTable("Memsafety-track results - Other Solvers", "label3"))
    print("")
    print(reachsafety_.caxdInterpolatorTable("Reachsafety-track results - CAXDInterpolator", "label4"))
    print("")
    print(reachsafety_.axdInterpolatorTable("Reachsafety-track results - AXDInterpolator", "label5"))
    print("")
    print(reachsafety_.otherSolversTable("Reachsafety-track results - Other Solvers", "label6"))
