"""Script to split tests.yml runners for faster builds."""
from __future__ import annotations

import json
from typing import Literal

import attrs
from attr import Factory, define


@define
class ConanProfile:
    runner_os: Literal["ubuntu-latest"] | Literal["windows-latest"]
    profile: str


CONAN_PROFILES = (
    ConanProfile(runner_os="ubuntu-latest", profile="profiles/Linux"),
    ConanProfile(runner_os="windows-latest", profile="profiles/Windows_mingw"),
)


@define
class Matrix:
    profile: list[ConanProfile]
    qt_version: list[str] = Factory(lambda: ["6.5.0"])
    python_version: list[str] = Factory(lambda: ["3.9", "3.10", "3.11"])


TESTCASES_PER_RUNNER = 9


def main() -> None:
    matrix = attrs.asdict(
        Matrix(
            profile=CONAN_PROFILES,
        ),
    )

    print(json.dumps(matrix))  # noqa


if __name__ == "__main__":
    main()
