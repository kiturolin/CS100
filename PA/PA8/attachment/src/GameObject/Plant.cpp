#include "pvz/GameObject/Plant.hpp"
#include "pvz/GameObject/Other.hpp"
#include "pvz/GameWorld/GameWorld.hpp"

Plant::Plant (ImageID imageID, int x, int y, AnimID animID, int hp) :
    GameObject (imageID, x, y, LayerID::PLANTS, 60, 80, animID),
    m_hp (hp)
{
}

bool
Plant::CheckHpDead ()
{
  if (m_hp > 0) return false;
  SetDead ();
  return true;
}

Sunflower::Sunflower (int x, int y) :
    Plant (ImageID::SUNFLOWER, x, y, AnimID::IDLE, 300),
    first_sun_generation (randInt (30, 600)),
    sun_interval (0)
{
}

void
Sunflower::GenrateSun ()
{
  world->GameWorld::GenerateSun (GetX (), GetY ());
}

void
Sunflower::Update ()
{
  if (CheckHpDead ()) return;
  static bool flag = false;
  if ((first_sun_generation--) > 0) return;
  // 只生成一次sun
  if (!flag) {
    GenrateSun ();
    flag = true;
  }
  if (sun_interval++ % 600 == 0) GenrateSun ();
}

Peashooter::Peashooter (int x, int y) :
    Plant (ImageID::PEASHOOTER, x, y, AnimID::IDLE, 300),
    shoot_cooldown_time (30)
{
}

void
Peashooter::Shoot ()
{
  auto pea = std::make_shared<Pea> (GetX () + 30, GetY () + 20);
  GameWorld::AddNewObject (pea);
}

void
Peashooter::Update ()
{
  if (CheckHpDead ()) return;
  if (shoot_cooldown_time-- > 0) return;
  if (GameWorld::ZombiesOnRight (GetY ())) {
    Shoot ();
    shoot_cooldown_time = 30;
  }
}

Wallnut::Wallnut (int x, int y) :
    Plant (ImageID::WALLNUT, x, y, AnimID::IDLE, 4000)
{
}

Wallnut::Wallnut (ImageID imageID, int x, int y, int hp) :
    Plant (imageID, x, y, AnimID::IDLE, hp)
{
}

void
Wallnut::Update ()
{
  if (CheckHpDead ()) return;
  if (m_hp < 4000 / 3) {
    // 使用原地生成一个残血wallnut来实现贴图替换
    auto cracker_wallnut = std::make_shared<Wallnut> (
	ImageID::WALLNUT_CRACKED, GetX (), GetY (), m_hp);
    GameWorld::AddNewObject (cracker_wallnut);
    SetDead ();
  }
}

CherryBomb::CherryBomb (int x, int y) :
    Plant (ImageID::CHERRY_BOMB, x, y, AnimID::IDLE, 4000),
    explosion_coundown (15)
{
}

void
CherryBomb::Update ()
{
  if (CheckHpDead ()) return;
  if (explosion_coundown-- > 0) return;
  auto explosion = std::make_shared<Explosion> (GetX (), GetY ());
  GameWorld::AddNewObject (explosion);
  SetDead ();
}

Repeater::Repeater (int x, int y) :
    Plant (ImageID::REPEATER, x, y, AnimID::IDLE, 300),
    shoot_cooldown_time (30)
{
}

void
Repeater::Shoot ()
{
  auto pea = std::make_shared<Pea> (GetX () + 30, GetY () + 20);
  GameWorld::AddNewObject (pea);
}

void
Repeater::Update ()
{
  if (CheckHpDead ()) return;
  static uint32_t turn = 0;
  if (shoot_cooldown_time-- > 0) return;
  if (GameWorld::ZombiesOnRight (GetY ())) {
    Shoot ();
    shoot_cooldown_time = turn % 2 == 0 ? 5 : 25;
    turn++;
  }
}

void
Plant::OnClick ()
{
  if (GameWorld::spotClickEvent->IsShovel ()) {
    SetDead ();
    // 执行回调
    GameWorld::spotClickEvent->Execute (0, 0);
  }
}

void
Plant::AdjustHp (int delta)
{
  m_hp += delta;
  CheckHpDead ();
}
