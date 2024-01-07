### Fork
To contribute create a fork and clone your fork locally.

### Install dependencies
We use [Poetry](https://python-poetry.org/) for managing dependencies.
```bash
poetry install
```

### Install [pre-commit](https://pre-commit.com/) hooks
```bash
pre-commit install
```

### Add a `RELEASE.md` file that describes your PR  in the project root.
```md
Release type: <patch/minor/major>

<description>
```

### Build
conan would generate cmake presets for IDE's usage as well.
add the option `test` to build the tests for the core library.
 ```bash
poetry run conan build . -o test=True
```
```
### Testing
Run tests with

TODO: add test command


### Documenting
Your changes require to update the docs?
We use mkdocs-material

run `poetry run mkdocs serve` for a local server.
