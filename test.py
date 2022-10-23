import numpy as np
import json
import sys
from subprocess import CompletedProcess, run, PIPE
import signal
from typing import List

is_windows = sys.platform.startswith('win')

APP_NAME = f"t9search{'.exe' if is_windows else ''}"

with open('tests.json') as f:
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
        result = run([self.command, *self.args], stdout=PIPE, stderr=PIPE, timeout=self.timeout, input=self.input, encoding='utf-8')
        return result

    def check_result(self) -> bool:
        result = self.run()
        return_code = np.uint32(result.returncode).view("int32")
        if return_code != 0 and return_code in [sig.as_integer_ratio()[0] for sig in signal.valid_signals()]:
            print(f'{bcolors.OKCYAN}Test {self.name}:\n{bcolors.FAIL}[FAIL] Failed with return code {return_code}{bcolors.ENDC}')
            print(f'{bcolors.WARNING}Stderr:\n{result.stderr}{bcolors.ENDC}')
            return False
        elif self.expected_code != 0:
            if result.stdout == self.expected:
                print(f'{bcolors.OKCYAN}Test {self.name}:\n{bcolors.OKGREEN}[OK] Error handling passed, code ({return_code}){bcolors.ENDC}\n')
                return True
            else:
                print(f'{bcolors.OKCYAN}Test {self.name}:\n{bcolors.FAIL}[FAIL] Failed\n{bcolors.WARNING}Expected output:\n{self.expected}\nGot:\n{result.stdout}{bcolors.ENDC}')
                return False
        else:
            if result.stdout == self.expected:
                print(f'{bcolors.OKCYAN}Test {self.name}:\n{bcolors.OKGREEN}[OK] Passed{bcolors.ENDC}')
                return True
            else:
                print(f'{bcolors.OKCYAN}Test {self.name}:\n{bcolors.FAIL}[FAIL] Failed\n{bcolors.WARNING}Expected output:\n{self.expected}\nGot:\n{result.stdout}{bcolors.ENDC}')
                return False


tests = [Test(APP_NAME, **test) for test in data]


if __name__ == '__main__':
    for test in tests:
        test.check_result()


