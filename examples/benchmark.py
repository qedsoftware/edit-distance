import random
import string
import timeit


def random_string(length):
    return "".join(random.choices(string.ascii_lowercase, k=length))


def mutate_string(s, num_changes):
    s = list(s)
    for _ in range(num_changes):
        idx = random.randrange(len(s))
        s[idx] = random.choice(string.ascii_lowercase)
    return "".join(s)


# Generate pairs with various similarity
pairs = [
    ("kitten", "sitting"),  # moderate distance
    ("abcdef", "abcdef"),  # identical
    ("abcdef", "ghijkl"),  # completely different
    ("hello", "helo"),  # small edit
    ("", ""),  # empty strings
    ("a" * 20, "a" * 20),  # long identical
    ("a" * 20, "b" * 20),  # long different
]

# Add random pairs with controlled mutation
base = random_string(16)
pairs.append((base, mutate_string(base, 1)))  # 1 change
pairs.append((base, mutate_string(base, 4)))  # 4 changes
pairs.append((base, mutate_string(base, 8)))  # 8 changes


def bench_editdistance_osa(text1, text2, number=100000):
    return timeit.timeit(
        stmt="editdistance.osa.compute_distance(text1, text2)",
        setup=f"import editdistance.osa; text1='{text1}'; text2='{text2}'",
        number=number,
    )


def bench_fast_damerau(text1, text2, number=100000):
    return timeit.timeit(
        stmt="fastDamerauLevenshtein.damerauLevenshtein(text1, text2)",
        setup=f"import fastDamerauLevenshtein; text1='{text1}'; text2='{text2}'",
        number=number,
    )


if __name__ == "__main__":
    number = 10000  # Reduce for reasonable runtime
    print(f"{'Pair':<30} {'OSA (s)':>10} {'FastDL (s)':>12} {'Ratio':>10}")
    print("-" * 67)
    for text1, text2 in pairs:
        osa_time = bench_editdistance_osa(text1, text2, number)
        fastdl_time = bench_fast_damerau(text1, text2, number)
        ratio = osa_time / fastdl_time if fastdl_time != 0 else float("inf")
        pair_desc = f"{text1[:6]}.../{text2[:6]}..."
        print(f"{pair_desc:<30} {osa_time:10.4f} {fastdl_time:12.4f} {ratio:10.2f}")
