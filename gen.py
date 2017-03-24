
import os, sys, subprocess, shlex

def find_gpf_path():
    """find GenerateProjectFiles.bat path
    return: str"""
    s = '../../UnrealEngine/Engine/Build/BatchFiles/GenerateProjectFiles.bat'
    for i in range(3):
        if os.path.isfile(s):
            return os.path.abspath(s)
        else:
            s = '../'+s
    return ''
        
def list_dir(dir):
    for d in os.listdir(dir):
        if d.find('.uproject')>0 and os.path.isfile(d):
            #os.environ.data['uproject'] = d
            #print d
            if find_gpf_path() == '':
                print 'unrealengine path is invalide.'
            cmd = find_gpf_path() + ' '+os.path.abspath(d)+' -Game -Engine'
            #args = shlex.split(cmd)
            args = cmd.split(' ')
            #print args
            p = subprocess.Popen(args, shell=True)
            p.communicate()
        
list_dir(".")