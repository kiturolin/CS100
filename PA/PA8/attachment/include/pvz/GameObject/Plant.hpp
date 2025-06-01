#ifndef PLANT_HPP__
#define PLANT_HPP__
#include "pvz/GameObject/GameObject.hpp"


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

#endif
