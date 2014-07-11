#!/usr/bin/python

"""Runs qmake -r in parallel to benefit from multi core CPUs"""

# Copyright (c) 2014, Benjamin Porter
# All rights reserved.
#
# (BSD style license)
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# 1. Redistributions of source code must retain the above copyright notice,
# this list of conditions and the following disclaimer.
#
# 2. Redistributions in binary form must reproduce the above copyright notice,
# this list of conditions and the following disclaimer in the documentation
# and/or other materials provided with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.


import sys
import multiprocessing

import bash

def run(qmake, profile_list):
    for i in profile_list:
        print('%s %s' % (qmake, i,))
        bash.run('%s %s' % (qmake, i,))

if __name__ == '__main__':
    if len(sys.argv) > 1:
        qmake = sys.argv[1]
    else:
        # Check if qmake is in path
        results = bash.run('which qmake')

        if results.return_code_success():
            qmake = results.stdout()
        else:
            print('no qmake in path. qmake must appear in path or full path passed as arg 1')
            sys.exit(1)

    # make number of processors qmake processes
    numProcs = int(bash.run('cat /proc/cpuinfo | grep processor | wc -l', False).stdout())

    # Divide up into lists for processing
    allProFiles = bash.run('find $(pwd) -name "*.pro" -type f').stdout().split('\n')

    lists = []
    for i in xrange(numProcs):
        lists.append([])

    filenum = 0
    for i in allProFiles:
        listNum = filenum % numProcs
        lists[listNum].append(i)
        filenum += 1

    procs = []
    for i in lists:
        p = multiprocessing.Process(target=run, args=(qmake, i,))
        procs.append(p)
        p.start()
        
    for p in procs:
        p.join()

    sys.exit(0)

