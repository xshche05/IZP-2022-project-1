import json
from subprocess import CompletedProcess, run, PIPE
from typing import Dict, List, Tuple

with open('tests.json') as f:
    data = json.load(f)


def run_test(app_name: str, args: List[str], stdin: str, stdout: str):
    p = run([app_name] + args, input=stdin, encoding='ascii', stdout=PIPE, stderr=PIPE)
    print(p.stdout)
    print(stdout)
    if p.stdout == stdout:
        print('PASSED')


if __name__ == '__main__':
    baseTests = data['tests']['baseTests']
    stdin = baseTests['stdin']
    tests = baseTests['tests']
    for test_i in tests:
        test = tests[test_i]
        args = test['args']
        stdout = test['stdout']
        s = " ".join(args)
        run_test('./t9search', args, stdin, stdout)
