#!/usr/bin/python3

import sys
from ResultsReader import ResultsReader

if __name__ == "__main__":
    
    verification_files_dir = str(sys.argv[1])
    results_dir = str(sys.argv[2])

    memsafety_ = ResultsReader(
            f"{results_dir}/benchmark_memsafety_results.txt", 
            f"{verification_files_dir}/MemSafety-Arrays")
    reachsafety_ = ResultsReader(
            f"{results_dir}/benchmark_reachsafety_results.txt", 
            f"{verification_files_dir}/ReachSafety-Arrays")
    print("")
    print("")
    print("")
    print(memsafety_.axdInterpolatorTable("Memsafety-track results - Our implementation", "label1"))
    print("")
    print(memsafety_.otherSolversTable("Memsafety-track results - Other Solvers", "label2"))
    print("")
    print(reachsafety_.axdInterpolatorTable("Reachsafety-track results - Our implementation", "label3"))
    print("")
    print(reachsafety_.otherSolversTable("Reachsafety-track results - Other Solvers", "label4"))
