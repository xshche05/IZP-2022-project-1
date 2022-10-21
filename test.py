import json
from subprocess import CompletedProcess, run, PIPE
from typing import Dict, List, Tuple

with open('tests.json') as f:
    data = json.load(f)


def run_test(app_name: str, args: List[str], stdin: str, stdout: str):
    p = run([app_name] + args, input=stdin, encoding='ascii', stdout=PIPE, stderr=PIPE)
    if p.stdout.replace("\n", "").lower() == stdout.replace("\n", "").lower():
        print('PASSED')
    else:
        print("FAILED")


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
            s = " ".join(args)
            run_test('./t9search', args, stdin, stdout)

