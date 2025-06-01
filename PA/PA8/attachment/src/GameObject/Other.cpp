#include "pvz/GameObject/Other.hpp"
#include "pvz/GameWorld/GameWorld.hpp"

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
Actions::ExecuteCallback ()
{
  GameWorld::spotClickEvent = std::make_shared<NoAction> ();
}

template <typename T_PlantType>
void
GenericPlantExecution<T_PlantType>::ExecutePlanting (int x, int y)
{
  auto plant = std::make_shared<T_PlantType> (x, y);
  GameWorld::AddNewObject (plant);
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
