#!/usr/bin/env python3
"""
Example usage of the OSA distance implementation.
First compile with: python setup.py build_ext --inplace
"""

try:
    from editdistance.osa import (
        PyEditopName,
        compute_distance,
        compute_with_all_paths,
        print_all_paths,
    )

    def main():
        # Define cost map
        cost_map = {
            PyEditopName.DELETE: 1.0,
            PyEditopName.INSERT: 1.0,
            PyEditopName.REPLACE: 1.0,
            PyEditopName.TRANSPOSE: 1.0,
        }

        # Test case from original Python code
        print("Testing OSA distance with all paths:")
        print_all_paths("aaaaaaaaaa", "abaabababa", cost_map)

        # Additional test case
        print("\nAdditional test case:")
        paths = compute_with_all_paths("CA", "AX", cost_map)
        distance = compute_distance("CA", "AX", cost_map)

        print(f"OSA Distance from 'CA' to 'AX': {distance}")
        print(f"Number of optimal edit sequences: {len(paths)}")
        print()

        for i, path in enumerate(paths, 1):
            print(f"Path {i}:")
            for op in path:
                print(f"  {op}")
            print()

    if __name__ == "__main__":
        main()

except ImportError as e:
    print("Error importing the compiled module:")
    print(e)
    print("\nTo compile the module, run:")
    print("python setup.py build_ext --inplace")
    print("\nMake sure you have Cython installed:")
    print("pip install cython")
