import sys as System
from subprocess import call

num_threads = [1,2,4,8]
methods = ['sign', 'block', 'interleaved']

histogram_binary = System.argv[1]
test_file = System.argv[2]

auto_increment = 0

for threads in num_threads:
    for method in methods:
        print('number of threads: ' + str(threads)+' method: '+method)
        call([histogram_binary, str(threads), method, test_file, "out"+str(auto_increment)])
        auto_increment += 1