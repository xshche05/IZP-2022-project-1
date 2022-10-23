import ctypes
import json
from subprocess import CompletedProcess, run, PIPE
from time import sleep
from typing import Dict, List, Tuple

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

def run_test(test_name: str, app_name: str, args: List[str], stdin: str, stdout: str):
    p = run([app_name] + args, input=stdin, encoding='ascii', stdout=PIPE, stderr=PIPE)
    print(f'{bcolors.HEADER}Running test {test_name}:')
    if p.stdout.lower()[:-1] == stdout.lower():
        print(f'{bcolors.OKGREEN}[OK]')
    else:
        print(f'{bcolors.FAIL}[FAIL] Erors:')
        print(f'Stderr:\n {p.stderr}')
        print('----------------------------------------')


if __name__ == '__main__':
    tests = data['tests']
    for test_cat in tests:
        cat_tests = tests[test_cat]
        stdin = cat_tests['stdin']
        cat_tests_tests = cat_tests['tests']
        for test_i in cat_tests_tests:
            test = cat_tests_tests[test_i]
            args = test['args']
            stdout = test['stdout']
            code = test['code']
            s = " ".join(args)
            run_test(f'{test_i}', './t9search', args, stdin, stdout)


