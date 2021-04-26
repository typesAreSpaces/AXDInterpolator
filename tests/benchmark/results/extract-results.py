#!/usr/bin/python

import os
import subprocess

class ResultReader:

    def add(self, file_name):
        self.file_name = file_name
        proc = subprocess.Popen([f"ls {file_name}"], stdout=subprocess.PIPE, shell=True)
        directories = str(proc.communicate()[0])[2:-1]
        for directory in directories.split("\\n"):
            if(directory == ''):
                continue
            print(f"Directory: {directory}")
            proc = subprocess.Popen([f"[ -d {directory} ]"], stdout=subprocess.PIPE, shell=True)
            proc.communicate()
            if(proc.returncode == 0):
                proc = subprocess.Popen([f"ls {directory}"], stdout=subprocess.PIPE, shell=True)
                files = str(proc.communicate()[0])[2:-1]
                for _file in files.split("\\n"):
                    if(_file == ''):
                        continue
                    print(f"file: {_file}")
        print(self.file_name)

if __name__ == "__main__":

    r = ResultReader()
    r.add("/media/Documents/MaxDiff-Experiments/verification-files/MemSafety-Arrays")

    # proc = subprocess.Popen(["ls"], stdout=subprocess.PIPE, shell=True)
    # directories = str(proc.communicate()[0])[2:-1]
    # for directory in directories.split("\\n"):
        # if(directory == ''):
            # continue
        # print(f"directory: {directory}")
        # proc = subprocess.Popen([f"[ -d {directory} ]"], stdout=subprocess.PIPE, shell=True)
        # proc.communicate()
        # if(proc.returncode == 0):
            # proc = subprocess.Popen([f"ls {directory}"], stdout=subprocess.PIPE, shell=True)
            # files = str(proc.communicate()[0])[2:-1]
            # for _file in files.split("\\n"):
                # if(_file == ''):
                    # continue
                # print(f"file: {_file}")


