#!/usr/bin/python

import os
import subprocess

class ResultReader:

    def __init__(self, file_name):
        self.file_name = file_name

    def wat(self):
        print(self.file_name)

if __name__ == "__main__":

    # r = ResultReader("haha")
    # r.wat()

    # os.system("ls /media/Documents/MaxDiff-Experiments/verification-files")
    proc = subprocess.Popen(["ls"], stdout=subprocess.PIPE, shell=True)
    (out, err) = proc.communicate()
    hmm = str(out)[2:-1]
    print(hmm)
    print(f"{hmm}")
    for x in hmm.split("\\n"):
        proc = subprocess.Popen([f"[ -d {x} ]"], stdout=subprocess.PIPE, shell=True)
        (out, err) = proc.communicate()
        if(proc.returncode == 0):
            proc = subprocess.Popen([f"ls {x}"], stdout=subprocess.PIPE, shell=True)
            (out, err) = proc.communicate()


