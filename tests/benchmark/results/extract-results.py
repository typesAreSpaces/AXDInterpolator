#!/usr/bin/python

import os
import subprocess

class BuildFileDB:

    def __init__(self, file_name):
        self.files = {}
        self.file_name = file_name
        self.add(file_name)

    def executeCommand(self, execstr):
        proc = subprocess.Popen([execstr], stdout=subprocess.PIPE, shell=True)
        (out, err) = proc.communicate()
        p_status = proc.wait()
        if(p_status != 0):
            print(f"Unable to execute {execstr}")
            return ""
        return str(out)
    
    def show(self):
        for subtrack in self.files:
            print(f"Subtrack: {subtrack} Size: {len(self.files[subtrack])}")
            for _file in self.files[subtrack]:
                print(_file)

    def add(self, file_name):
        directories = self.executeCommand(f"ls {file_name}")[2:-1]
        for directory in directories.split("\\n"):
            if(directory == ''):
                continue
            curr_dir = file_name + "/" + directory;
            if(os.path.isdir(curr_dir)):
                files = self.executeCommand(f"ls {curr_dir}")[2:-1]
                for _file in files.split("\\n"):
                    if(_file == ''):
                        continue
                    if(not (directory in self.files)):
                        self.files[directory] = set()
                    self.files[directory].add(_file)

    def whichSubtrack(self, _file):
        for subtrack in self.files:
            if(_file in self.files[subtrack]):
                return subtrack
        return None

class ResultsReader:

    def __init__(self, file_name, db_dir):
        self.db = BuildFileDB(db_dir)
        self.file_name = file_name
        # self.table is a map from subtrack to a 
        # map of solver -> exit -> num_samples
        # subtrack -> solver -> exit code -> num_samples
        self.exit_codes = set()
        self.table = {}
        self.fillTable()
    
    def fillTable(self):
        temp_file = open(self.file_name, 'r')
        while True:
            line = temp_file.readline()
            if not line:
                break
            curr_line = line.split(" ")
            curr_file = curr_line[1]
            second_underscore = curr_file.find("_", curr_file.find("_") + 1)

            curr_file = curr_file[second_underscore + 1 : ]
            curr_solver_code = curr_line[4]
            curr_exit_code = curr_line[7][0:-1]
            curr_subtrack = self.db.whichSubtrack(curr_file)

            self.exit_codes.add(curr_exit_code)

            if(not (curr_subtrack in self.table)):
                self.table[curr_subtrack] = {}
            if(not (curr_solver_code in self.table[curr_subtrack])):
                self.table[curr_subtrack][curr_solver_code] = {}
            if(not (curr_exit_code in self.table[curr_subtrack][curr_solver_code])):
                self.table[curr_subtrack][curr_solver_code][curr_exit_code] = 0
            self.table[curr_subtrack][curr_solver_code][curr_exit_code] = self.table[curr_subtrack][curr_solver_code][curr_exit_code] + 1
        temp_file.close()

    def show(self):
        for subtrack in self.table:
            print(f"Subtrack: {subtrack}")
            for solver_code in self.table[subtrack]:
                print(f">>>Solver code: {solver_code}")
                for exit_code in self.exit_codes:
                    if(not (exit_code in self.table[subtrack][solver_code])):
                        print(f"Exit Code: {exit_code} Num Samples: 0")
                    else:
                        print(f"Exit Code: {exit_code} Num Samples: {self.table[subtrack][solver_code][exit_code]}")
            print("")

    def makeTableEntry(self, entry):
        num_success = 0
        if('0' in entry):
            num_success += entry['0']
        num_failed = 0
        for exit_code in entry:
            if(exit_code != '0' and exit_code != '152'):
                num_failed += entry[exit_code]
        num_timeout = 0
        if('152' in entry):
            num_timeout += entry['152']
        return (num_success, num_failed, num_timeout)

    def axdInterpolatorTable(self, caption, label):
        print(self.exit_codes)
        table = "\\begin{table}[htp]\n"
        table += "\t\\small\n"
        table += "\t\\begin{centering}\n"
        table += "\t\t\\begin{tabular}{l|ccc|ccc|ccc}\n"
        table += "\t\t\t & \\multicolumn{9}{c}{AXD Interpolator} \\\\ \n"
        table += "\t\t\t Subtracks & \\multicolumn{3}{c}{Using iZ3} & \\multicolumn{3}{c}{Using MathSat} & \\multicolumn{3}{c}{Using SMTInterpol} \\\\ \n"
        table += "\t\t\t \\hline \n"
        table += "\t\t\t  & Success & Failed & Timeout & Success & Failed & Timeout & Success & Failed & Timeout \\\\ \n"
        table += "\t\t\t \\hline \n"
        for subtrack in self.table:
            table += f"\t\t\t{subtrack} "

            axd_z3_entry = self.table[subtrack]['0']
            axd_mathsat_entry = self.table[subtrack]['1']
            axd_smtinterpol_entry = self.table[subtrack]['2']

            (axd_z3_num_success, axd_z3_num_failed, axd_z3_num_timeout) = self.makeTableEntry(axd_z3_entry)
            (axd_mathsat_num_success, axd_mathsat_num_failed, axd_mathsat_num_timeout) = self.makeTableEntry(axd_mathsat_entry)
            (axd_smtinterpol_num_success, axd_smtinterpol_num_failed, axd_smtinterpol_num_timeout) = self.makeTableEntry(axd_smtinterpol_entry)
            
            entry_for_axd_z3 = f"& {axd_z3_num_success} & {axd_z3_num_failed} & {axd_z3_num_timeout} "
            entry_for_axd_mathsat = f"& {axd_mathsat_num_success} & {axd_mathsat_num_failed} & {axd_mathsat_num_timeout} "
            entry_for_axd_smtinterpol = f"& {axd_smtinterpol_num_success} & {axd_smtinterpol_num_failed} & {axd_smtinterpol_num_timeout} "

            table += entry_for_axd_z3 + entry_for_axd_mathsat + entry_for_axd_smtinterpol
            table += "\\\\ \n" 
        table += "\t\t\\end{tabular}\n"
        table += "\t\t\\caption{" + caption + "}\n"
        table += "\t\t\\label{" + label + "}\n"
        table += "\t\\end{centering}\n"
        table += "\\end{table}"
        return table

    def otherSolversTable(self, caption, label):
        table = "\\begin{table}[htp]\n"
        table += "\t\\small\n"
        table += "\t\\begin{centering}\n"
        table += "\t\t\\begin{tabular}{l|ccc|ccc|ccc}\n"
        table += "\t\t\t Subtracks & \\multicolumn{3}{c}{Z3} & \\multicolumn{3}{c}{MathSat} & \\multicolumn{3}{c}{SMTInterpol} \\\\ \n"
        table += "\t\t\t \\hline \n"
        table += "\t\t\t  & Success & Failed & Timeout & Success & Failed & Timeout & Success & Failed & Timeout \\\\ \n"
        table += "\t\t\t \\hline \n"
        for subtrack in self.table:
            table += f"\t\t\t{subtrack} "

            z3_entry = self.table[subtrack]['3']
            mathsat_entry = self.table[subtrack]['4']
            smtinterpol_entry = self.table[subtrack]['5']

            (z3_num_success, z3_num_failed, z3_num_timeout) = self.makeTableEntry(z3_entry)
            (mathsat_num_success, mathsat_num_failed, mathsat_num_timeout) = self.makeTableEntry(mathsat_entry)
            (smtinterpol_num_success, smtinterpol_num_failed, smtinterpol_num_timeout) = self.makeTableEntry(smtinterpol_entry)
            
            entry_for_z3 = f"& {z3_num_success} & {z3_num_failed} & {z3_num_timeout} "
            entry_for_mathsat = f"& {mathsat_num_success} & {mathsat_num_failed} & {mathsat_num_timeout} "
            entry_for_smtinterpol = f"& {smtinterpol_num_success} & {smtinterpol_num_failed} & {smtinterpol_num_timeout} "

            table += entry_for_z3 + entry_for_mathsat + entry_for_smtinterpol
            table += "\\\\ \n" 
        table += "\t\t\\end{tabular}\n"
        table += "\t\t\\caption{" + caption + "}\n"
        table += "\t\t\\label{" + label + "}\n"
        table += "\t\\end{centering}\n"
        table += "\\end{table}"
        return table

    def oneEntryTable(self, caption, label, entry, solver_name):
        table = "\\begin{table}[htp]\n"
        table += "\t\\small\n"
        table += "\t\\begin{centering}\n"
        table += "\t\t\\begin{tabular}{l|ccc}\n"
        table += "\t\t\t Subtracks & \\multicolumn{3}{c}{solver_name} \\\\ \n"
        table += "\t\t\t \\hline \n"
        table += "\t\t\t  & Success & Failed & Timeout \\\\ \n"
        table += "\t\t\t \\hline \n"
        for subtrack in self.table:
            print(subtrack)
            table += f"\t\t\t{subtrack} "

            _entry = self.table[subtrack][entry]

            (_num_success, _num_failed, _num_timeout) = self.makeTableEntry(_entry)
            
            entry_for_ = f"& {_num_success} & {_num_failed} & {_num_timeout} "

            table += entry_for_
            table += "\\\\ \n" 
        table += "\t\t\\end{tabular}\n"
        table += "\t\t\\caption{" + caption + "}\n"
        table += "\t\t\\label{" + label + "}\n"
        table += "\t\\end{centering}\n"
        table += "\\end{table}"
        return table

if __name__ == "__main__":
    verification_files_dir = "/media/Documents/MaxDiff-Experiments/verification-files"
    results_dir = "/home/jose/Documents/GithubProjects/AXDInterpolator/tests/benchmark/results/fuel-1000000_St-360_Sv-4500000-samples-500-April-30-2021"

    memsafety_ = ResultsReader(
            f"{results_dir}/benchmark_memsafety_results-axdinterpolator.txt", 
            f"{verification_files_dir}/MemSafety-Arrays")
    reachsafety_ = ResultsReader(
            f"{results_dir}/benchmark_reachsafety_results-axdinterpolator.txt", 
            f"{verification_files_dir}/ReachSafety-Arrays")
    print(memsafety_.axdInterpolatorTable("Memsafety-track results - Our implementation", "label1"))
    print("")
    print(reachsafety_.axdInterpolatorTable("Reachsafety-track results - Our implementation", "label1"))
