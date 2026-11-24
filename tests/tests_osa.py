import math
import unittest

from editdistance.osa import (
    apply_editops,
    compute_distance,
    compute_log_probability,
    get_all_paths,
)

M, R, I, D, S = 0.9, 0.01, 0.02, 0.03, 0.04

# (description, a, b, (match, replace, insert, delete, swap), expected probability)
COMPUTE_LOG_PROBABILITY_TEST_CASES = [
    ("empty strings", "", "", (M, R, I, D, S), 1.0),
    ("identical strings", "abc", "abc", (M, 0, 0, 0, 0), M**3),
    ("single replace", "abc", "axc", (M, R, 0, 0, 0), M**2 * R),
    ("single deletion", "abc", "ab", (M, 0, 0, D, 0), M**2 * D),
    ("single insertion", "ab", "abc", (M, 0, I, 0, 0), M**2 * I),
    ("insertions only", "", "ab", (M, R, I, D, S), I**2),
    ("either doubled letter deleted", "ss", "s", (M, 0, 0, D, 0), 2 * M * D),
    ("swap", "ab", "ba", (M, 0, 0, 0, S), S),
    ("swap or two replaces", "ab", "ba", (M, R, 0, 0, S), S + R**2),
    ("equal characters are not swapped", "aa", "aa", (M, 0, 0, 0, S), M**2),
    ("match or delete and insert", "a", "a", (M, R, I, D, S), M + 2 * I * D),
]

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

    def test_compute_log_probability(self):
        for description, a, b, probs, expected in COMPUTE_LOG_PROBABILITY_TEST_CASES:
            with self.subTest(description):
                log_prob = compute_log_probability(a, b, *probs)
                self.assertAlmostEqual(log_prob, math.log(expected), places=9)

    def test_compute_log_probability_impossible(self):
        log_prob = compute_log_probability("a", "b", M, 0, 0, 0, 0)
        self.assertEqual(log_prob, -math.inf)

    def test_editops_transform(self):
        for src, dst in EDITOPS_TRANSFORM_TEST_CASES:
            with self.subTest(src=src, dst=dst):
                paths = get_all_paths(src, dst, return_matches=True)
                self.assertTrue(paths, f"No paths found for {src} -> {dst}")
                for path in paths:
                    result = apply_editops(src, dst, path)
                    self.assertEqual(result, dst, f"Failed for {src} -> {dst} with path {path}")
