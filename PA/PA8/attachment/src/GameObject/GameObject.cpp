#include "pvz/Framework/GameManager.hpp"
#include "pvz/GameObject/GameObject.hpp"
#include "pvz/GameWorld/GameWorld.hpp"
#include "pvz/utils.hpp"

#include <cmath>
#include <cstdint>
#include <memory>

std::vector<std::vector<std::shared_ptr<Zombie>>> Zombie::zombies (5);

GameObject::GameObject (ImageID imageID,
			int x,
			int y,
			LayerID layer,
			int width,
			int height,
			AnimID animID) :
    ObjectBase (imageID, x, y, layer, width, height, animID),
    m_is_dead (false)
{
}

void
GameObject::SetDead ()
{
  m_is_dead = true;
}

bool
GameObject::IsDead () const
{
  return m_is_dead;
}

void
Actions::ExecuteCallback ()
{
  GameWorld::spotClickEvent = std::make_shared<NoAction> ();
}

GameBackground::GameBackground () :
    GameObject (ImageID::BACKGROUND,
		WINDOW_WIDTH / 2,
		WINDOW_HEIGHT / 2,
		LayerID::BACKGROUND,
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		AnimID::NO_ANIMATION) {};

CooldownMask::CooldownMask (int x, int y, int _cooldown_time) :
    GameObject (ImageID::COOLDOWN_MASK,
		x,
		y,
		LayerID::COOLDOWN_MASK,
		50,
		70,
		AnimID::NO_ANIMATION),
    cooldown_time (_cooldown_time)
{
}

void
CooldownMask::Update ()
{
  if (cooldown_time-- > 0) return;
  SetDead ();
}

Shovel::Shovel () :
    GameObject (ImageID::SHOVEL,
		600,
		WINDOW_HEIGHT - 40,
		LayerID::UI,
		50,
		50,
		AnimID::NO_ANIMATION)
{
}

void
Shovel::OnClick ()
{
  if (GameWorld::spotClickEvent->IsShovel ()) {
    // 执行形式回调: 放下铲子
    GameWorld::spotClickEvent->Execute (0, 0);
    return;
  }
  // 手上有种子: 不做任何事
  if (!GameWorld::spotClickEvent->IsNoAction ()) { return; }

  GameWorld::spotClickEvent = std::make_shared<ShovelAction> ();
}

Pea::Pea (int x, int y) :
    GameObject (
	ImageID::PEA, x, y, LayerID::PROJECTILES, 28, 28, AnimID::NO_ANIMATION)
{
  std::cout << "Spawn at :" << x << " " << y << "\n";
}

void
Pea::Update ()
{
  if (IsDead ()) return;
  MoveTo (GetX () + 8, GetY ());
  if (GetX () + 8 >= WINDOW_WIDTH) SetDead ();
}

Explosion::Explosion (int x, int y) :
    GameObject (ImageID::EXPLOSION,
		x,
		y,
		LayerID::PROJECTILES,
		3 * LAWN_GRID_WIDTH,
		3 * LAWN_GRID_HEIGHT,
		AnimID::NO_ANIMATION),
    present_time (3)
{
}

void
Explosion::Update ()
{
  if (present_time-- > 0) return;
  SetDead ();
}

Sun::Sun (int x, int y, SunType type, const std::shared_ptr<GameWorld> &world) :
    GameObject (ImageID::SUN, x, y, LayerID::SUN, 80, 80, AnimID::IDLE),
    m_type (type),
    m_falling_time (type == Sun::SunType::Sky ? randInt (63, 263) : 12),
    m_dimish_time (300),
    m_dy (type == Sun::SunType::Sky ? -2 : 4),
    m_world (world)
{
}

void
Sun::Update ()
{
  if (m_falling_time-- > 0) {
    switch (m_type) {
    case SunType::Sky:
      MoveTo (GetX (), GetY () + m_dy);
      break;
    case SunType::Sunflower:
      MoveTo (GetX () + 1, GetY () + m_dy);
      m_dy -= 1;
      break;
    }
  } else {
    if (m_dimish_time-- > 0) return;
    SetDead ();
  }
}

void
Sun::OnClick ()
{
  this->m_world->GetSun ();
  this->SetDead ();
}

PlantingSpot::PlantingSpot (int row, int col) :
    GameObject (ImageID::NONE,
		col * LAWN_GRID_WIDTH + FIRST_ROW_CENTER,
		row * LAWN_GRID_HEIGHT + FIRST_COL_CENTER,
		LayerID::UI,
		60,
		80,
		AnimID::NO_ANIMATION),
    m_occupied (false),
    m_x (col * LAWN_GRID_WIDTH + FIRST_ROW_CENTER),
    m_y (row * LAWN_GRID_HEIGHT + FIRST_COL_CENTER)
{
}

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

void
PlantingSpot::OnClick ()
{
  GameWorld::spotClickEvent->Execute (m_x, m_y);
}

Seed::Seed (ImageID imageID,
	    PlantType _type,
	    int _cost,
	    int _num_pos,
	    int _cooldown_time) :
    GameObject (imageID, 0, 0, LayerID::UI, 50, 70, AnimID::NO_ANIMATION),
    type (_type),
    cost (_cost),
    num_pos (_num_pos),
    cooldown_time (_cooldown_time)
{
  MoveTo (130 + _num_pos * 60, WINDOW_HEIGHT - 44);
}

void
Seed::CoolDown ()
{
  auto mask = std::make_shared<CooldownMask> (GetX (), GetY (), cooldown_time);
  GameWorld::AddNewObject (mask);
}

void
Seed::OnClick ()
{
  // 如果手中拿着铲子或植物种子
  if (!GameWorld::spotClickEvent->IsNoAction ()) return;
  // 如果阳光不够
  if (GameWorld::num_sun < cost) return;
  // 阳光足够, 则准备下一次种植
  GameWorld::num_sun -= cost;
  switch (type) {
  case PlantType::PEASHOOTER:
    GameWorld::spotClickEvent
	= std::make_shared<PlantAction<GenericPlantExecution<Peashooter>>> ();
    break;
  case PlantType::WALLNUT:
    GameWorld::spotClickEvent
	= std::make_shared<PlantAction<GenericPlantExecution<Wallnut>>> ();
    break;
  case PlantType::CHERRYBOMB:
    GameWorld::spotClickEvent
	= std::make_shared<PlantAction<GenericPlantExecution<CherryBomb>>> ();
    break;
  case PlantType::REPEATER:
    GameWorld::spotClickEvent
	= std::make_shared<PlantAction<GenericPlantExecution<Repeater>>> ();
    break;
  case PlantType::SUNFLOWER:
    GameWorld::spotClickEvent
	= std::make_shared<PlantAction<GenericPlantExecution<Sunflower>>> ();
    break;
  }
  // 给出冷却时间
  CoolDown ();
}

template <typename T_PlantType>
void
GenericPlantExecution<T_PlantType>::ExecutePlanting (int x, int y)
{
  auto plant = std::make_shared<T_PlantType> (x, y);
  GameWorld::AddNewObject (plant);
}

Zombie::Zombie (ImageID imageID,
		int row,
		int x,
		int y,
		int hp,
		ZombieType type,
		AnimID animeID) :
    GameObject (imageID, x, y, LayerID::ZOMBIES, 20, 80, animeID),
    type (type),
    row (row),
    hp (hp)
{
  zombies[row].push_back (shared_from_this ());
}

void
Zombie::RemoveZombie (int row, std::shared_ptr<Zombie> &zombie)
{
  for (auto it = Zombie::zombies[row].begin ();
       it != Zombie::zombies[row].end ();
       it++) {
    if (*it == zombie) {
      Zombie::zombies[row].erase (it);
      break;
    }
  }
}

void
Zombie::CheckCollision ()
{
  for (auto &object : GameWorld::m_GameObjects) {
    if (object->GetY () == GetY () && GetX () - object->GetX () < 10) {
      if (object->IsPea ()) {
	object->SetDead ();
	hp -= 20;
	return;
      }

      if (object->IsPlant ()) {
	auto plant = std::dynamic_pointer_cast<Plant> (object);
	plant->AdjustHp (-3);
	PlayAnimation (AnimID::EAT);
	return;
      }
    }
    if (abs (object->GetY () - GetY ()) <= LAWN_GRID_HEIGHT
	&& abs (object->GetX () - GetX ()) <= LAWN_GRID_WIDTH)
      if (object->IsExplosion ()) {
	SetDead ();
	return;
      }
  }
  // 运行到这里说明没有检测到任何碰撞
  PlayAnimation (AnimID::RUN);
}

RegularZombie::RegularZombie (int row, int x, int y) :
    Zombie (ImageID::REGULAR_ZOMBIE,
	    row,
	    x,
	    y,
	    200,
	    Zombie::ZombieType::REGULAR,
	    AnimID::WALK)
{
}

BucketZombie::BucketZombie (int row, int x, int y) :
    Zombie (ImageID::BUCKET_HEAD_ZOMBIE,
	    row,
	    x,
	    y,
	    1300,
	    Zombie::ZombieType::BUCKET,
	    AnimID::WALK)
{
}

PoleZombie::PoleZombie (int row, int x, int y) :
    Zombie (ImageID::POLE_VAULTING_ZOMBIE,
	    row,
	    x,
	    y,
	    340,
	    Zombie::ZombieType::POLE,
	    AnimID::RUN),
    running (true)
{
}

void
RegularZombie::Update ()
{
  if (hp <= 0) {
    SetDead ();
    return;
  }
  if (GetCurrentAnimation () == AnimID::WALK) { MoveTo (GetX () - 1, GetY ()); }
}

void
BucketZombie::Update ()
{
  if (hp <= 0) {
    SetDead ();
    return;
  }
  if (GetCurrentAnimation () == AnimID::WALK) { MoveTo (GetX () - 1, GetY ()); }
  if (hp <= 300) ChangeImage (ImageID::REGULAR_ZOMBIE);
}

void
PoleZombie::Update ()
{
}

// Your everything begins from here.
//

// 植物函数
