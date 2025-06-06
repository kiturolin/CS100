#include "dynarray.hpp"

#include <type_traits>
#include <utility>
#include <iostream>

#if __cplusplus < 201703L && !defined(_MSC_VER)
#error At least C++17 is required.
#endif

static_assert(std::is_default_constructible_v<Dynarray>,
              "`Dynarray` should be default constructible.");

static_assert(!std::is_trivially_default_constructible_v<Dynarray>,
              "`Dynarray` should have a non-trivial default constructor.");

static_assert(std::is_constructible_v<Dynarray, std::size_t>,
              "The constructor `Dynarray(n)` should be supported.");

static_assert(!std::is_convertible_v<std::size_t, Dynarray>,
              "The constructor `Dynarray(n)` should be explicit.");

static_assert(std::is_constructible_v<Dynarray, std::size_t, int>,
              "The constructor `Dynarray(n, x)` should be supported.");

static_assert(std::is_constructible_v<Dynarray, const int *, const int *>,
              "The constructor `Dynarray(begin, end)` should be supported.");

static_assert(std::is_copy_constructible_v<Dynarray>,
              "`Dynarray` should be copy constructible.");

static_assert(!std::is_trivially_copy_constructible_v<Dynarray>,
              "`Dynarray` should have a non-trivial copy constructor.");

static_assert(std::is_copy_assignable_v<Dynarray>,
              "`Dynarray` should be copy assignable.");

static_assert(!std::is_trivially_copy_assignable_v<Dynarray>,
              "`Dynarray` should have a non-trivial copy assignment operator.");

static_assert(std::is_nothrow_move_constructible_v<Dynarray>,
              "`Dynarray` should have a noexcept move constructor.");

static_assert(!std::is_trivially_move_constructible_v<Dynarray>,
              "`Dynarray` should have a non-trivial move constructor.");

static_assert(std::is_nothrow_move_assignable_v<Dynarray>,
              "`Dynarray` should have a noexcept move assignment operator.");

static_assert(!std::is_trivially_move_assignable_v<Dynarray>,
              "`Dynarray` should have a non-trivial move assignment operator.");

static_assert(!std::is_trivially_destructible_v<Dynarray>,
              "`Dynarray` should have a non-trivial destructor.");

namespace detail {

  void assign_ret_requirements(Dynarray a, const Dynarray b) {
    static_assert(
        std::is_same_v<Dynarray &, decltype(a = b)>,
        "Why doesn't your copy assignment operator return reference to "
        "lhs object?`");
    static_assert(
        std::is_same_v<Dynarray &, decltype(a = Dynarray{})>,
        "Why doesn't your move assignment operator return reference to "
        "lhs object?");
  };

  template <typename T, typename U>
  using enable_if_same = std::enable_if_t<std::is_same_v<T, U>>;

#define DEFINE_DETECTER(LABEL, OBJECT, MEMFCN, RET, ...)                       \
  template <typename T = OBJECT,                                               \
            typename U = decltype(std::declval<T>().MEMFCN(__VA_ARGS__)),      \
            typename = enable_if_same<U, RET>>                                 \
  std::true_type detect_##LABEL(int);                                          \
  std::false_type detect_##LABEL(...);                                         \
  inline constexpr auto LABEL##_correct = decltype(detect_##LABEL(0))::value;

  DEFINE_DETECTER(size, const Dynarray, size, std::size_t, )
  DEFINE_DETECTER(empty, const Dynarray, empty, bool, )
  DEFINE_DETECTER(at, Dynarray, at, int &, std::size_t{})
  DEFINE_DETECTER(const_at, const Dynarray, at, const int &, std::size_t{})
  DEFINE_DETECTER(find, const Dynarray, find, std::size_t, int{})
  DEFINE_DETECTER(find_pos, const Dynarray, find, std::size_t, int{},
                  std::size_t{})

#undef DEFINE_DETECTER

} // namespace detail

static_assert(detail::size_correct,
              "`Dynarray::size` not defined or incorrect.");

static_assert(detail::empty_correct,
              "`Dynarray::empty` not defined or incorrect.");

static_assert(detail::at_correct && detail::const_at_correct,
              "`Dynarray::at` not defined or incorrect.");

// Dynarray::size, Dynarray::empty
namespace detail {

#define SINGLE_DEF_DETECTER(MEMFUN)                                            \
  template <typename T = Dynarray, typename = decltype(&T::MEMFUN)>            \
  std::true_type test_##MEMFUN##_def(int);                                     \
  std::false_type test_##MEMFUN##_def(...);                                    \
  inline constexpr auto MEMFUN##_def_correct =                                 \
      decltype(test_##MEMFUN##_def(0))::value;

  SINGLE_DEF_DETECTER(size)
  SINGLE_DEF_DETECTER(empty)

#undef SINGLE_DEF_DETECTER

} // namespace detail

static_assert(
    detail::size_def_correct,
    "`Dynarray::size` should not have `const vs non-const` overloading.");

static_assert(
    detail::empty_def_correct,
    "`Dynarray::empty` should not have `const vs non-const` overloading.");

// Dynarray::at
namespace detail {

  template <typename T, typename U>
  U get_param_type_nonconst(T (Dynarray::*)(U));

  template <typename T, typename U>
  U get_param_type_const(T (Dynarray::*)(U) const);

  inline constexpr auto at_param_type_correct =
      std::is_same_v<decltype(get_param_type_nonconst(&Dynarray::at)),
                     std::size_t>;

  inline constexpr auto const_at_param_type_correct =
      std::is_same_v<decltype(get_param_type_const(&Dynarray::at)),
                     std::size_t>;

} // namespace detail

static_assert(
    detail::at_param_type_correct && detail::const_at_param_type_correct,
    "What might be a better choice for the parameter type of `Dynarray::at?`");

int main() {
  std::cout << "Compile-time checks passed." << std::endl;
  return 0;
}
