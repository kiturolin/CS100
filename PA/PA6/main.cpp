#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

/**
 * @brief Given a vector of strings @c strings, reduce this vector so that each
 * string appears only once. After the function call, @c strings contains the
 * same set of elements (possibly sorted) as the original vector, but each
 * element appears only once.
 *
 * @param strings The given vector of strings.
 */
void
dropDuplicates (std::vector<std::string> &strings)
{
  std::sort (strings.begin (), strings.end ());
  auto pos = std::unique (strings.begin (), strings.end ());
  strings.erase (pos, strings.end ());
}

/**
 * @brief Given a vector of strings @c strings and a nonnegative integer @c k,
 * partition the vector into two parts (with the elements rearranged) so that
 * the first part contains all the strings with length no greater than @c k and
 * the second part contains the rest of the strings. Return an iterator just
 * past the last element of the first part.
 *
 * @param strings The given vector of strings.
 * @return An iterator just past the last element of the first part. In
 * particular, if the second part is not empty, the returned iterator will refer
 * to the first element of the second part.
 */
auto
partitionByLength (std::vector<std::string> &strings, std::size_t k)
{
  // FIXME: The type of the parameter `strings` may be incorrect.
  return std::partition (strings.begin (), strings.end (), [k] (const std::string &str) { return str.length () <= k; });
}

/**
 * @brief Generate all the permutations of {1, 2, ..., n} in lexicographical
 * order, and print them to @c os.
 *
 */
void
generatePermutations (int n, std::ostream &os)
{
  // FIXME: The type of the parameter `os` may be incorrect.

  std::vector<int> numbers (n);

  std::generate (numbers.begin (), numbers.end (), [] () {
    static int n = 1;
    return n++;
  });
  do {
    std::for_each (numbers.begin (), numbers.end (), [&os] (int i) { os << i << " "; });
    os << '\n';
  } while (std::next_permutation (numbers.begin (), numbers.end ()));
}
