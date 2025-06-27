import unittest

from editdistance.osa import (
    apply_editops,
    compute_distance,
    get_all_paths,
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

EDITOPS_TRANSFORM_TEST_CASES = [
    ("abc", "acb"),
    ("kitten", "sitting"),
    ("flaw", "lawn"),
    ("", "abc"),
    ("abc", ""),
    ("abcdef", "azced"),
    ("a", "a"),
    ("a", ""),
    ("", ""),
    ("banana", "ban"),
    ("intention", "execution"),
    ("gumbo", "gambol"),
    ("sunday", "saturday"),
    ("ca", "abc"),
    ("abcdef", "fedcba"),
    ("racecar", "racecar"),
    ("spelling", "spilling"),
    ("distance", "instance"),
    ("book", "back"),
]


class TestOsaDistance(unittest.TestCase):
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
                distance = compute_distance(source, target)
                self.assertEqual(distance, expected_distance)

    def test_get_all_paths(self):
        for (
            description,
            source,
            target,
            expected_num_paths,
        ) in COMPUTE_ALL_PATHS_TEST_CASES:
            with self.subTest(
                description,
            ):
                paths = get_all_paths(source, target)
                self.assertEqual(len(paths), expected_num_paths)

    def test_editops_transform(self):
        for src, dst in EDITOPS_TRANSFORM_TEST_CASES:
            with self.subTest(src=src, dst=dst):
                paths = get_all_paths(src, dst)
                self.assertTrue(paths, f"No paths found for {src} -> {dst}")
                for path in paths:
                    result = apply_editops(src, dst, path)
                    self.assertEqual(result, dst, f"Failed for {src} -> {dst} with path {path}")
