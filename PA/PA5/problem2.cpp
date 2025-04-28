#include <cstdint>
#include <string>
#include <utility>
#include <vector>

class Picture
{
private:
  std::string path_;
  int width_, height_;

public:
  static uint64_t now_id;
  uint64_t pic_id;

  Picture () = delete;
  Picture (std::string _path, int _width, int _height)
      : path_ (std::move (_path)), width_ (_width), height_ (_height), pic_id (++now_id) {};
};

class BlockedGroup
{
private:
  std::vector<uint64_t> user_id_;

public:
  std::string group_name;

  BlockedGroup () = delete;
  explicit BlockedGroup (std::string name) : group_name (std::move (name)) {};

  // Add blocked user to the group
  void add_user ();
  // Delete blocked user from the group
  void delete_user ();

  // Rename the group
  void rename ();

  // Print members in the group
  void print_group () const;
};

class Moment
{
private:
  std::vector<BlockedGroup> blocked_group_;

public:
  std::string location;
  std::string date_and_time;
  std::string text;
  std::vector<Picture> pics;

  Moment () = delete;

  Moment (std::string _date_and_time, std::string _text)
      : date_and_time (std::move (_date_and_time)), text (std::move (_text))
  {
  }

  Moment (std::string _date_and_time, std::string _text, std::string _location)
      : date_and_time (std::move (_date_and_time)), text (std::move (_text)), location (std::move (_location))
  {
  }

  // Edit location;
  void set_location (std::string new_location);

  // Add and remove picture;
  void add_pic (const Picture &pic);
  void remove_pic (const Picture &pic);

  // Add and remove blocked group;
  void add_blocked (const BlockedGroup &blocked_group);
  void remove_blocked (const BlockedGroup &blocked_group);

  // Show Moments
  [[nodiscard]] std::string show_moment () const;
};

int
main ()
{
  return 0;
}
