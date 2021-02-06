"""
A quick and dirty pytest plugin to test command line programs given YAML
specifications.
This file comes with absolutely no warranties and support will only be
best-effort. You're encouraged to hack on it, though.
Drop it (still named conftest.py, this is important) at the root of your
functional tests directory, and run with "pytest" from the root of your
project.
Starting from the directory containing this file, and recursively through all
sub-directories, all files ending with ".yaml" will be collected, no matter
their name.
Here is an example yaml:
    name: cat should have identical stdin and stdout
    command: cat
    stdin: |
        foobar
        barfoo
    stdout: |
        foobar
        barfoo
    ---
    name: failing for all three reasons
    command: echo "foo"
    stderr: |
        machin
    returncode: 1
Two noteworthy things, if you're not familiar with YAML:
    * Multiple yaml items can be defined in a single file, as long as they are
    separated by "---".
    * Multiline values are done by adding a pipe.
    See https://yaml-multiline.info/ to learn more about the syntax.
Note: this code relies on third-party packages which must be installed first.
Those dependencies (pytest and pyyaml) should be installed in a virtualenv.
TL;DR:
    * Create a virtualenv with "python -m venv venv"
    * Activate the virtualenv with "source venv/bin/activate"
    * Install the dependencies with "pip install pytest pyyaml"
    * The virtualenv directory should not be commited, add it to your gitignore
See https://docs.python.org/3/tutorial/venv.html for more info.
"""

from dataclasses import dataclass
import pytest
import subprocess
import typing
import difflib
import shlex
import yaml


@dataclass
class TestCase:
    name: str
    command: str
    stdin: typing.Optional[str] = ""
    stdout: typing.Optional[str] = ""
    stderr: typing.Optional[str] = ""
    returncode: typing.Optional[int] = 0

def pytest_collect_file(parent, path):
    if path.ext == ".yaml":
        return YamlFile.from_parent(parent, fspath=path)



class YamlFile(pytest.File):
    def collect(self):
        testcases = yaml.safe_load_all(self.fspath.open())
        for raw_test in testcases:
            test = TestCase(**raw_test)
            yield TestItem.from_parent(self, name=test.name, spec=test)


class TestItem(pytest.Item):
    def __init__(self, name, parent, spec):
        super().__init__(name, parent)
        self.spec = spec

    def runtest(self):
        errors = []
        try:
            proc = subprocess.run(
                shlex.split(self.spec.command),
                input=self.spec.stdin,
                text=True,
                capture_output=True
            )
        except Exception as error:
            raise TestCaseError(self.name, [error])

        # Return code verification
        if proc.returncode != self.spec.returncode:
            errors.append(ReturnCodeError(expected=self.spec.returncode,
                                          actual=proc.returncode))

        # stdout verification
        out_delta = list(difflib.unified_diff(
                self.spec.stdout.splitlines(keepends=True),
                proc.stdout.splitlines(keepends=True),
                fromfile="expected",
                tofile="actual"
            ))
        if out_delta:
            errors.append(DiffError("stdout", out_delta))

        # stderr verification
        err_delta = list(difflib.unified_diff(
                self.spec.stderr.splitlines(keepends=True),
                proc.stderr.splitlines(keepends=True),
                fromfile="expected",
                tofile="actual"
            ))
        if err_delta:
            errors.append(DiffError("stderr", err_delta))

        if errors:
            raise TestCaseError(self.name, errors)

    def repr_failure(self, excinfo):
        """ called when self.runtest() raises an exception. """

        if isinstance(excinfo.value, TestCaseError):
            return '\n\n'.join(str(err) for err in excinfo.value.errors)


    def reportinfo(self):
        return self.fspath, 0, self.name




class TestCaseError(Exception):
    """
    A container for multiple errors, since a testcase can fail for multiple
    reasons, e.g both return code and stdout differ from the spec.
    """
    def __init__(self, name, errors):
        self.name = name
        self.errors = errors


class ReturnCodeError(Exception):
    """
    Thrown when the return code differs from the specifications.
    """
    def __init__(self, expected, actual):
        self.expected = expected
        self.actual = actual

    def __str__(self):
        return f"return code: expected {self.expected}, got {self.actual}"

class DiffError(Exception):
    """
    Thrown when either stdout or stderr differs from the specifications.
    """
    def __init__(self, stream_name, delta):
        self.stream_name = stream_name
        self.delta = delta

    def __str__(self):
        return ''.join([f"{self.stream_name} differs:\n"] + self.delta)
