import sys

from runner import main, run_tests

from solution import Solution


def run_tests_hook(stream):
    return run_tests(
        stream,
        Solution(),
        "checkOverlap",
        arg_types=["int", "int", "int", "int", "int", "int", "int"],
        return_type="bool",
    )


if __name__ == "__main__":
    sys.exit(main(run_tests_hook))
