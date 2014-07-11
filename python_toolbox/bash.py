"""Provides a handy interface for getting bash stuff done from Python scripts"""


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


import subprocess


class BashCommand:
    """Represents the results of running a bash command"""

    def __init__(self, return_code=-1, stderr='', stdout='', stdin=''):
        self.__return_code = return_code
        self.__stderr = stderr
        self.__stdout = stdout
        self.__stdin = stdin

    def return_code(self):
        """Retrieve the process' return code"""
        return self.__return_code

    def return_code_success(self):
        """Returns true if the command exited successfully, false otherwise"""
        return self.__return_code == 0

    def stdout(self, strip_whitespace=True):
        """Retrieve the process' output to stdout"""
        if strip_whitespace:
            return self.__stdout.strip()
        else:
            self.__stdout

    def stderr(self, strip_whitespace=True):
        """Retrieve the process' output to stderr"""
        return self.__stderr
        if strip_whitespace:
            return self.__stderr.strip()
        else:
            self.__stderr

    def stdin(self, strip_whitespace=True):
        """Retrieve the process' input to stdin"""
        return self.__stdin
        if strip_whitespace:
            return self.__stdin.strip()
        else:
            self.__stdin


def run(command, print_to_console=True, stdin=None):
    """Run command at the bash prompt, providing input to stdin

    If print_to_console == True, command will be run like a regular bash
    command, meaning all output to stderr and stdout will be
    printed to the console. A BashCommand object will be returned
    containing a copy of the output for programmatic processing

    If print_to_console == false, a BashCommand object will be returned
    containing the return code from the process, as well as
    stderr and stdout.  No output is printed to the console
    """

    if stdin == None:
        proc = subprocess.Popen(
                command,
                stdout=subprocess.PIPE,
                stderr=subprocess.PIPE,
                shell=True
                )
        output = proc.communicate()

    else:
        proc = subprocess.Popen(
                command,
                stdin=subprocess.PIPE,
                stdout=subprocess.PIPE,
                stderr=subprocess.PIPE,
                shell=True
                )
        output = proc.communicate(stdin)

    if print_to_console:
        if len(output[0]) > 0:
            print('%s' % output[0])
        if len(output[1]) > 0:
            print('%s' % output[1])

    return BashCommand(
            return_code=proc.returncode,
            stdout=output[0],
            stderr=output[1],
            stdin=stdin
            )

