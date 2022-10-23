import ctypes
import json
from subprocess import CompletedProcess, run, PIPE
from time import sleep
from typing import Dict, List, Tuple

APP_NAME = "t9search"

with open('_tests.json') as f:
    data = json.load(f)

class bcolors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKCYAN = '\033[96m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'

class Test:
    def __init__(self, app_name: str, name: str, args: List[str], test_input: str, expected_output: str, timeout: int = 10, expected_return_code: int = 0):
        self.name = name
        self.command = f'./{app_name}'
        self.args = args
        self.input = test_input
        self.expected = expected_output
        self.expected_code = expected_return_code
        self.timeout = timeout

    def run(self) -> CompletedProcess:
        result = run([self.command, *self.args], stdout=PIPE, stderr=PIPE, timeout=self.timeout, input=self.input)
        return result

    def check_result(self) -> bool:
        result = self.run()
        if result.returncode != 0:
            print(f'{bcolors.FAIL}Test {self.name} failed with return code {result.returncode}{bcolors.ENDC}')
            print(f'Stderr:\n {bcolors.WARNING}{result.stderr.decode()}{bcolors.ENDC}')
            return False
        elif result.returncode == self.expected_code:
            if result.stdout.decode() == self.expected:
                print(f'{bcolors.OKGREEN}Test {self.name} passed{bcolors.ENDC}')
                return True
            else:
                print(f'{bcolors.FAIL}Test {self.name} failed, expected output {self.expected}, got {result.stdout.decode()}{bcolors.ENDC}')
                return False
        else:
            if result.stdout.decode() == self.expected:
                print(f'{bcolors.OKGREEN}Test {self.name} passed{bcolors.ENDC}')
                return True
            else:
                print(f'{bcolors.FAIL}Test {self.name} failed{bcolors.ENDC}')
                print(f'Expected:\n {bcolors.WARNING}{self.expected}{bcolors.ENDC}')
                print(f'Got:\n {bcolors.WARNING}{result.stdout.decode()}{bcolors.ENDC}')
                return False


tests = [Test(APP_NAME, **test) for test in data]


if __name__ == '__main__':
    for test in tests:
        test.check_result()


