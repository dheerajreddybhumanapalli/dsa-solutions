import sys

from runner import main, run_tests

from solution import Solution


def run_tests_hook(stream):
    return run_tests(stream, Solution(), "isValid", arg_types=["string"], return_type="bool")


if __name__ == "__main__":
    sys.exit(main(run_tests_hook))
