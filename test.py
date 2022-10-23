import ctypes
import json
from subprocess import CompletedProcess, run, PIPE
from time import sleep
from typing import Dict, List, Tuple

with open('tests.json') as f:
    data = json.load(f)


def run_test(test_name: str, app_name: str, args: List[str], stdin: str, stdout: str):
    p = run([app_name] + args, input=stdin, encoding='ascii', stdout=PIPE, stderr=PIPE)
    print(f'Running test {test_name}')
    if p.stdout.lower()[:-1] == stdout.lower():
        print(f'[OK] {test_name}')
    else:
        print(f'[FAIL] {test_name}')
        print(f'Stderr:\n {p.stderr}')


if __name__ == '__main__':
    # str = "a"*50
    # print(str)
    # print(len("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"))
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


