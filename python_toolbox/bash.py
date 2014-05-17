"""Provides a handy interface for getting bash stuff done from Python scripts"""


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

    def stdout(self):
        """Retrieve the process' output to stdout"""
        return self.__stdout

    def stderr(self):
        """Retrieve the process' output to stderr"""
        return self.__stderr

    def stdin(self):
        """Retrieve the process' input to stdin"""
        return self.__stdin


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
        print('%s\n%s' % output)
    return BashCommand(
            return_code=proc.returncode,
            stdout=output[0],
            stderr=output[1],
            stdin=stdin
            )

