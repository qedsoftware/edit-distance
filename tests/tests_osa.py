import unittest

from editdistance.osa import (
    PyEditopName,
    compute_distance,
    compute_with_all_paths,
)

COMPUTE_DISTANCE_TEST_CASES = [
    ("single character", "a", "b", 1.0),
    ("identical strings", "abc", "abc", 0.0),
    ("single deletion", "abc", "ab", 1.0),
    ("single insertion", "ab", "abc", 1.0),
    ("entirely different", "abc", "def", 3.0),
]

COMPUTE_ALL_PATHS_TEST_CASES = [
    ("single character", "a", "b", 1),
    ("identical strings", "abc", "abc", 1),
    ("single deletion", "abc", "ab", 1),
    ("single insertion", "ab", "abc", 1),
    ("entirely different", "abc", "def", 1),
    ("entirely different", "cab", "axb", 2),
]


class TestOsaDistance(unittest.TestCase):
    def setUp(self):
        self.cost_map = {
            PyEditopName.DELETE: 1.0,
            PyEditopName.INSERT: 1.0,
            PyEditopName.REPLACE: 1.0,
            PyEditopName.TRANSPOSE: 1.0,
        }

    def test_compute_distance(self):
        for (
            description,
            source,
            target,
            expected_distance,
        ) in COMPUTE_DISTANCE_TEST_CASES:
            with self.subTest(
                description,
            ):
                distance = compute_distance(source, target, self.cost_map)
                self.assertEqual(distance, expected_distance)

    def test_compute_with_all_paths(self):
        for (
            description,
            source,
            target,
            expected_num_paths,
        ) in COMPUTE_ALL_PATHS_TEST_CASES:
            with self.subTest(
                description,
            ):
                paths = compute_with_all_paths(source, target, self.cost_map)
                self.assertEqual(len(paths), expected_num_paths)
