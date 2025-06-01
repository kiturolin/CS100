#ifndef GAMEOBJECT_HPP__
#define GAMEOBJECT_HPP__

#include "pvz/Framework/ObjectBase.hpp"
#include "pvz/utils.hpp"

#include <cstdint>
#include <memory>
#include <type_traits>
#include <vector>

// Declares the class name GameWorld so that its pointers can be used.
class GameWorld;

class GameObject : public ObjectBase,
		   public std::enable_shared_from_this<GameObject>
{
  // Use shared_from_this() instead of "this" to create a pointer to oneself.
public:
  GameObject (ImageID imageID,
	      int x,
	      int y,
	      LayerID layer,
	      int width,
	      int height,
	      AnimID animID);
  static std::shared_ptr<GameWorld> world;
  void SetDead ();
  bool IsDead () const;

  virtual bool IsPea () { return false; };

  virtual bool IsZombie () { return false; };

  virtual bool IsExplosion () { return false; };

  virtual bool IsPlant () { return false; };


private:
  bool m_is_dead;
};

class GameBackground : public GameObject
{
public:
  GameBackground ();
  // 当update和click时, 应该do nothing
  void Update () override {};
  void OnClick () override {};

private:
};

class Sun : public GameObject
{
public:
  enum class SunType { Sky, Sunflower };
  Sun (int x, int y, SunType type, const std::shared_ptr<GameWorld> &world);
  void OnClick () override;
  void Update () override;

private:
  SunType m_type;
  std::shared_ptr<GameWorld> m_world;
  int m_falling_time;
  int m_dy;
  int m_dimish_time;
};

class Shovel : public GameObject
{
public:
  Shovel ();
  void Update () override {};
  void OnClick () override;
};

class Pea : public GameObject
{
public:
  Pea (int x, int y);

  bool IsPea () override { return true; };

  void Update () override;
  void OnClick () override {};
};

class Explosion : public GameObject
{
public:
  Explosion (int x, int y);
  int present_time;

  bool IsExplosion () override { return true; }

  void Update () override;

  void OnClick () override {}
};

class PlantingSpot : public GameObject

{
private:
  int m_x, m_y;
  bool m_occupied;

public:
  PlantingSpot (int row, int col);

  void Update () override {}

  void OnClick () override;
};

class CooldownMask : public GameObject
{
public:
  int cooldown_time;
  CooldownMask (int x, int y, int _cooldown_time);
  void Update () override;

  void OnClick () override {}
};

class Plant : public GameObject
{
protected:
  uint32_t m_hp;

public:
  Plant (ImageID imageID, int x, int y, AnimID animID, int hp);
  std::string plantType;

  bool IsPlant () override { return true; }

  void AdjustHp (int delta);
  void OnClick () override;
  bool CheckHpDead ();
};

class Sunflower : public Plant
{
public:
  int first_sun_generation;
  uint32_t sun_interval;
  Sunflower (int x, int y);

  void GenrateSun ();

  // TODO:Write Update
  void Update () override;
};

class Peashooter : public Plant
{
public:
  Peashooter (int x, int y);

  int shoot_cooldown_time;
  void Shoot ();

  void Update () override;
};

class Wallnut : public Plant
{
public:
  Wallnut (ImageID imageID, int x, int y, int hp);
  Wallnut (int x, int y);

  void Update () override;
};

class CherryBomb : public Plant
{
public:
  CherryBomb (int x, int y);
  int explosion_coundown;

  void Update () override;
};

class Repeater : public Plant
{
public:
  Repeater (int x, int y);

  int shoot_cooldown_time;
  void Shoot ();

  void Update () override;
};

class Actions
{
public:
  virtual ~Actions ()		      = default;
  virtual void Execute (int x, int y) = 0;
  static void ExecuteCallback ();

  virtual bool IsNoAction () { return false; }

  virtual bool IsShovel () { return false; }
};

class NoAction : public Actions
{
public:
  NoAction () = default;

  void Execute (int, int) override {}

  bool IsNoAction () override { return true; }
};

class ShovelAction : public Actions
{
public:
  ShovelAction () = default;

  void Execute (int, int) override { ExecuteCallback (); }

  bool IsShovel () override { return true; }
};

class PlantStrategy
{
public:
  virtual ~PlantStrategy ()		      = default;
  virtual void ExecutePlanting (int x, int y) = 0;
};

template <typename T_PlantType>
class GenericPlantExecution : public PlantStrategy
{
  static_assert (std::is_base_of_v<Plant, T_PlantType>,
		 "T must be a type derived from Plant");

public:
  void ExecutePlanting (int x, int y) override;
};

template <typename T_PlantStrategy>
class PlantAction : public Actions
{
  static_assert (std::is_base_of_v<PlantStrategy, T_PlantStrategy>,
		 "T must be a type derived from PlantStrategy");

private:
  T_PlantStrategy m_plantStrategy;

public:
  PlantAction () = default;

  void Execute (int x, int y) override
  {
    m_plantStrategy.ExecutePlanting (x, y);
    Actions::ExecuteCallback ();
  }
};

class Seed : public GameObject
{
public:
  PlantType type;
  int cost;
  int num_pos;
  int cooldown_time;

public:
  Seed (ImageID imageID,
	PlantType _type,
	int _cost,
	int _num_pos,
	int _cooldown_time);
  void Update () override {};
  void OnClick () override;
  void CoolDown ();
};

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
  void CheckLose();
  virtual bool IsPole() {return false;}
  virtual bool IsRunning() {return false;}
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
  bool IsPole() override {return true;};
  bool IsRunning() override {return jumping;}
  
  int jump_anime_countdown;
};
#endif	  // !GAMEOBJECT_HPP__
