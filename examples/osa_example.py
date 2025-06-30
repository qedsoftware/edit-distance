#!/usr/bin/env python3
"""
Example usage of the OSA distance implementation.
First compile with: python setup.py build_ext --inplace
"""

try:
    from editdistance.osa import compute_distance, get_all_paths

    def main():
        # Test case from original Python code
        print("Testing OSA distance with all paths:")

        test_cases = (
            ("aaaaaaaaaa", "abaabababa"),
            ("CA", "AX"),
        )

        for source, target in test_cases:
            print(f"\nComputing OSA distance from '{source}' to '{target}':")
            distance = compute_distance(source, target)
            print(f"Distance: {distance}")

            paths = get_all_paths(source, target)
            paths_with_matches = get_all_paths(source, target, return_matches=True)
            print(f"Number of optimal edit sequences: {len(paths)}")

            print("Paths without match editops:")
            for i, path in enumerate(paths, 1):
                print(f"    Path {i}:")
                for op in path:
                    print(f"        {op}")
            print()
            print("Paths with match editops:")
            for i, path in enumerate(paths_with_matches, 1):
                print(f"    Path {i}:")
                for op in path:
                    print(f"        {op}")
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
