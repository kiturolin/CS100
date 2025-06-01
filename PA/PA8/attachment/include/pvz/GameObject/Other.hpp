#ifndef OTHER_HPP__
#define OTHER_HPP__

#include "pvz/GameObject/GameObject.hpp"
#include "pvz/GameObject/Plant.hpp"

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

public:
  void ExecutePlanting (int x, int y) override;
};

template <typename T_PlantStrategy>
class PlantAction : public Actions
{

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
#endif
