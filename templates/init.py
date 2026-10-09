import sys

from runner import main, run_tests

from solution import Solution


def run_tests_hook(stream):
    # Fill in the method name plus the argument / return type specifiers,
    # e.g. run_tests(stream, Solution(), "isValid", arg_types=["string"], return_type="bool")
    return run_tests(stream, Solution(), "method", arg_types=[], return_type="void")


if __name__ == "__main__":
    sys.exit(main(run_tests_hook))
