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


def run_command(command, stdin=None, save_output=False):
    """Run command at the bash prompt, providing input to stdin

    If save_output == False, command will be run like a regular bash
    command, meaning all output to stderr and stdout will be
    printed to the console. A BashCommand object will be returned

    If save_output == True, a BashCommand object will be returned
    containing the return code from the process, as well as
    stderr and stdout.  No output is printed to the console
    """

    if save_output:
        if input == None:
            proc = subprocess.Popen(
                    command,
                    stdout=subprocess.PIPE,
                    stderr=subprocess.PIPE,
                    shell=True
                    )
            output = proc.communicate()
            return BashCommand(
                    return_code=proc.returncode,
                    stdout=output[0],
                    stderr=output[1]
                    )

        else:
            proc = subprocess.Popen(
                    command,
                    stdin=subprocess.PIPE,
                    stdout=subprocess.PIPE,
                    stderr=subprocess.PIPE,
                    shell=True
                    )
            output = proc.communicate(stdin)
            return BashCommand(
                    return_code=proc.returncode,
                    stdout=output[0],
                    stderr=output[1],
                    stdin=stdin
                    )

    elif input != None:
        return BashCommand(
                return_code=subprocess.call(command, stdin=stdin, shell=True),
                stdin=stdin
                )
    else:
        return BashCommand(
                return_code=subprocess.call(command, shell=True)
                )

