# edit-distance

edit-distance is a Python package that provides an implementation of the Optimal String Alignment (OSA) algorithm for calculating edit distances. The package leverages C++ extensions via Cython for improved performance.

## Features

- Calculate edit distances using the OSA algorithm.
- Enables custom weights for each edit operation.
- Find all edit paths resulting in the minimal OSA distance between strings.
- High-performance implementation using C++ and Cython.
- Easy integration into Python projects.

## Installation

Ensure you have a C++ compiler installed. Then, clone the repository and install the package using:

```sh
pip install .
```

Alternatively, call `setup.py` directly:

```sh
python setup.py build_ext --inplace
python setup.py install
```

For more details on the setup, see [setup.py](setup.py).

## Usage

After installation, you can import and use the module in your Python code:

```python
import editdistance.osa

# Example usage:
str1 = "kitten"
str2 = "sitting"
distance = editdistance.osa.calculate_distance(str1, str2)
print(f"The edit distance between '{{}}' and '{{}}' is {{}}".format(str1, str2, distance))
```

See examples located in [examples](examples/osa_example.py) directory.

## Running Tests

The test suite is located in the [tests](tests/tests_osa.py) directory. To run the tests, execute:

```sh
python -m unittest discover -v
```

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.
```