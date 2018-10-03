import os
import shutil
import filecmp
import glob

def mkfile(filename, body=None):
    with open(filename, 'w') as f:
        f.write(body or filename)
    return

def make_test_dir(top):
    if not os.path.exists(top):
        os.mkdir(top)
    curdir = os.getcwd()
    os.chdir(top)
    mkfile('a.txt','1111111')
    mkfile('b.txt','2222222')
    mkfile('b11.txt','333333')
    mkfile('b222.txt','444444')
    comp = filecmp.cmp('a.txt','b.txt', False)   
    print ("Is a=b?", comp)
    shutil.copyfile('a.txt','c.txt')
    os.chdir(curdir)
    allfiles = os.listdir(curdir)
    print ("After creating test directory:", allfiles)
 
    return

if __name__ == '__main__':
    curdir = os.getcwd()
    allfiles = os.listdir(curdir)
    print ("Before creating test directory:", allfiles)
    make_test_dir('test')
    print ("STAT:",os.lstat('test'))
    
    for name in glob.glob('test/?.txt'):
        print name 

    shutil.rmtree('test')
    allfiles = os.listdir(curdir)
    print ("After deleting test directory:", allfiles)
 
    
