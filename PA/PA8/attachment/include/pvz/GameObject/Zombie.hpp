#ifndef ZOMBIE_HPP__
#define ZOMBIE_HPP__
#include "pvz/GameObject/Plant.hpp"

class Zombie : public GameObject,
	       public std::enable_shared_from_this<Zombie>
{
  using std::enable_shared_from_this<Zombie>::shared_from_this;

public:
  enum class ZombieType { REGULAR, BUCKET, POLE };
  std::shared_ptr<GameObject> collided_object;
  Zombie (ImageID imageID,
	  int row,
	  int x,
	  int y,
	  int hp,
	  ZombieType type,
	  AnimID animeID);

  std::shared_ptr<GameObject> CheckCollision ();
  static void RemoveZombie (int row, std::shared_ptr<Zombie> &zombie);
  void OnClick () override {};
  void HandleCollision (std::shared_ptr<GameObject> &);
  void CheckLose ();

  virtual bool IsPole () { return false; }

  virtual bool IsRunning () { return false; }

  ZombieType type;
  int row;
  int hp;

  static std::vector<std::vector<std::shared_ptr<Zombie>>> zombies;
};

class RegularZombie : public Zombie
{
public:
  RegularZombie (int row, int x, int y);
  void Update () override;
};

class BucketZombie : public Zombie
{
public:
  BucketZombie (int row, int x, int y);
  void Update () override;
};

class PoleZombie : public Zombie
{
public:
  PoleZombie (int row, int x, int y);
  void Update () override;
  void HandleJump ();

  bool jumping;
  bool running;

  bool IsPole () override { return true; };

  bool IsRunning () override { return jumping; }

  int jump_anime_countdown;
};
#endif
