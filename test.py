import json
from subprocess import CompletedProcess, run, PIPE

with open('tests.json') as f:
    data = json.load(f)


def run_test(app_name: str, args: str, stdin: str):
    p = run(app_name + args, input=stdin, encoding='ascii', stdout=PIPE, stderr=PIPE)
    print(p.stdout)
    print(p.stderr)


if __name__ == '__main__':
    baseTests = data['tests']['baseTests']
    stdin = baseTests['stdin']
    tests = baseTests['tests']
    for test_i in tests:
        test = tests[test_i]
        args = test['args']
        s = " ".join(args)
        run_test('./t9search', s, stdin[0])