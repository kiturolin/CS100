#include <algorithm>
#include <cstddef>
#include <iostream>
#include <stdexcept>

class Dynarray
{
public:
  friend std::ostream &operator<< (std::ostream &os, const Dynarray &arr);

  using size_type	= std::size_t;
  using value_type	= int;
  using pointer		= int *;
  using reference	= int &;
  using const_pointer	= const int *;
  using const_reference = const int &;

private:
  value_type *m_storage;
  size_type m_length;

public:
  // Plain constructor.
  Dynarray () : m_storage (nullptr), m_length (0) {};

  explicit Dynarray (size_type n) : m_storage (new value_type[n]{ 0 }), m_length (n) {}

  Dynarray (size_type n, value_type x) : m_storage (new value_type[n]), m_length (n)
  {
    for (size_type i = 0; i < n; ++i)
      m_storage[i] = x;
  }

  Dynarray (const_pointer begin, const_pointer end) : m_storage (new value_type[end - begin]), m_length (end - begin)
  {
    size_type len = end - begin;
    for (size_type i = 0; i < len; ++i)
      m_storage[i] = begin[i];
  }

  // Copy Constructor
  Dynarray (const Dynarray &other) : m_storage (new value_type[other.size ()]), m_length (other.size ())
  {
    for (size_type i = 0; i < other.size (); ++i)
      m_storage[i] = other.m_storage[i];
  }

  // Copy Assignment Operator
  Dynarray &operator= (const Dynarray &other)
  {
    if (this == &other) return *this;
    auto *new_data = new value_type[other.size ()];
    for (size_type i = 0; i < other.size (); ++i)
      new_data[i] = other.at (i);
    delete[] m_storage;
    m_storage = new_data;
    m_length  = other.size ();

    return *this;
  }

  // Move Constructor
  Dynarray (Dynarray &&other) noexcept : m_storage (other.m_storage), m_length (other.size ())
  {
    other.m_storage = nullptr;
    other.m_length  = 0;
  }

  // Move Assignment Operator
  Dynarray &operator= (Dynarray &&other) noexcept
  {
    if (this == &other) return *this;
    delete[] m_storage;
    m_storage	    = other.m_storage;
    m_length	    = other.m_length;
    other.m_storage = nullptr;
    other.m_length  = 0;
    return *this;
  }

  // Destructor
  ~Dynarray () { delete[] m_storage; }

  size_type size () const { return m_length; }

  value_type &at (size_type i)
  {
    if (i >= m_length) throw std::out_of_range{ "Dynarray index out of range!" };

    return m_storage[i];
  }

  const value_type &operator[] (size_type i) const { return m_storage[i]; }

  value_type &operator[] (size_type i) { return m_storage[i]; }

  bool operator< (const Dynarray &other) const
  {
    return std::lexicographical_compare (m_storage, m_storage + m_length, other.m_storage, other.m_storage + other.m_length);
  }

  bool operator== (const Dynarray &other) const
  {
    return std::equal(m_storage, m_storage + m_length, other.m_storage, other.m_storage + other.m_length);
  }

  bool operator> (const Dynarray &other) const { return other < *this; }

  bool operator!= (const Dynarray &other) const { return !(*this == other); }

  bool operator<= (const Dynarray &other) const { return !(*this > other); }

  bool operator>= (const Dynarray &other) const { return !(*this < other); }

  const value_type &at (size_type i) const
  {
    if (i >= m_length) throw std::out_of_range{ "Dynarray index out of range!" };

    return m_storage[i];
  }

  bool empty () const { return m_length == 0; }
};

std::ostream &
operator<< (std::ostream &os, const Dynarray &arr)
{
  os << "[";
  for (std::size_t i = 0; i < arr.m_length; i++) {
    os << arr[i];
    if (i + 1 < arr.m_length) os << ", ";
  }
  os << "]";

  return os;
}
