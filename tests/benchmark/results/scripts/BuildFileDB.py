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

