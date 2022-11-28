#!/usr/bin/python3

from ResultsReader import ResultsReader

if __name__ == "__main__":
    verification_files_dir = "/media/Documents/MaxDiff-Experiments/verification-files"
    results_dir = ("/home/jose/Documents/GithubProjects"
            "/AXDInterpolator/tests/benchmark/results"
            "/fuel-1000000_St-360_Sv-4500000-samples-500-April-30-2021")

    memsafety_ = ResultsReader(
            f"{results_dir}/benchmark_memsafety_results-axdinterpolator.txt", 
            f"{verification_files_dir}/MemSafety-Arrays")
    reachsafety_ = ResultsReader(
            f"{results_dir}/benchmark_reachsafety_results-axdinterpolator.txt", 
            f"{verification_files_dir}/ReachSafety-Arrays")
    print(memsafety_.axdInterpolatorTable("Memsafety-track results - Our implementation", "label1"))
    print("")
    print(reachsafety_.axdInterpolatorTable("Reachsafety-track results - Our implementation", "label1"))
