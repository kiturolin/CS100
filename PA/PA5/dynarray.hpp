#include <cstddef>
#include <cstdint>
#include <stdexcept>

class Dynarray
{
private:
  int *storage_;
  std::size_t length_;

public:
  // Plain constructor.
  Dynarray () : storage_ (nullptr), length_ (0) {};

  explicit Dynarray (uint32_t n) : storage_ (new int[n]{ 0 }), length_ (n) {}

  Dynarray (uint32_t n, int x) : storage_ (new int[n]), length_ (n)
  {
    for (uint32_t i = 0; i < n; ++i)
      storage_[i] = x;
  }

  Dynarray (const int *begin, const int *end) : storage_ (new int[end - begin]), length_ (end - begin)
  {
    uint32_t len = end - begin;
    for (uint32_t i = 0; i < len; ++i)
      storage_[i] = begin[i];
  }

  // Copy Constructor
  Dynarray (const Dynarray &other) : storage_ (new int[other.size ()]), length_ (other.size ())
  {
    for (uint32_t i = 0; i < other.size (); ++i)
      storage_[i] = other.storage_[i];
  }

  // Copy Assignment Operator
  Dynarray &operator= (const Dynarray &other)
  {
    if (this == &other) return *this;
    int *new_data = new int[other.size ()];
    for (uint32_t i = 0; i < other.size (); ++i)
      new_data[i] = other.at (i);
    delete[] storage_;
    storage_ = new_data;
    length_  = other.size ();

    return *this;
  }

  // Move Constructor
  Dynarray (Dynarray &&other) noexcept : storage_ (other.storage_), length_ (other.size ())
  {
    other.storage_ = nullptr;
    other.length_  = 0;
  }

  // Move Assignment Operator
  Dynarray &operator= (Dynarray &&other) noexcept
  {
    if (this == &other) return *this;
    delete[] storage_;
    storage_ = other.storage_;
    length_  = other.length_;
    other.storage_ = nullptr;
    other.length_  = 0;
    return *this;
  }

  // Destructor
  ~Dynarray () { delete[] storage_; }

  std::size_t size () const { return length_; }

  int &at (std::size_t i)
  {
    if (i >= length_) throw std::out_of_range{ "Dynarray index out of range!" };

    return storage_[i];
  }

  const int &at (std::size_t i) const
  {
    if (i >= length_) throw std::out_of_range{ "Dynarray index out of range!" };

    return storage_[i];
  }

  bool empty () const { return length_ == 0; }
};
