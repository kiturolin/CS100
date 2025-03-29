import random
import math
import sys


def data_gen(length, is_palindrome):
    half_len = math.ceil(length / 2)
    half_str = ""

    if (is_palindrome):
        half_len = length

    for i in range(half_len):
        random_ascii = random.randint(33, 126)
        half_str += chr(random_ascii)

    if (is_palindrome):
        return half_str

    half_rev_str = half_str[::-1]
    if (length % 2 != 0):
        half_rev_str = half_rev_str[1:]

    return half_str + half_rev_str


if __name__ == "__main__":
    n = int(sys.argv[1])
    max_length = int(sys.argv[2])

    print(n)
    for i in range(n):
        length = random.randint(1, max_length)
        is_palindrome = True if random.random() < 0.5 else False
        print(length)
        print(data_gen(length, is_palindrome))
