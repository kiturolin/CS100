#include "pvz/Framework/TextBase.hpp"
#include "pvz/GameObject/GameObject.hpp"
#include "pvz/GameWorld/GameWorld.hpp"
#include "pvz/utils.hpp"

#include <memory>

int GameWorld::num_sun			     = 0;
uint32_t GameWorld::sun_time_interval	     = 0;
std::shared_ptr<GameWorld> GameObject::world = nullptr;
std::shared_ptr<Actions> GameWorld::spotClickEvent
    = std::make_shared<NoAction> ();
std::list<std::shared_ptr<GameObject>> GameWorld::m_GameObjects;
std::shared_ptr<TextBase> GameWorld::sunText
    = std::make_shared<TextBase> (60, WINDOW_HEIGHT - 78);

void
GameWorld::AddNewObject (const std::shared_ptr<GameObject> &sPtr)
{
  m_GameObjects.push_back (sPtr);
}

void
GameWorld::GenerateSun ()
{
  if (!((sun_time_interval++) % 150)) {
    auto sun
	= std::make_shared<Sun> (randInt (FIRST_COL_CENTER, WINDOW_WIDTH - 10),
				 WINDOW_HEIGHT - 10,
				 Sun::SunType::Sky,
				 shared_from_this ());
    AddNewObject (sun);
  }
}

void
GameWorld::GenerateSun (int x, int y)
{
  auto sun = std::make_shared<Sun> (
      x, y, Sun::SunType::Sunflower, shared_from_this ());
  AddNewObject (sun);
}

void
GameWorld::GetSun ()
{
  GameWorld::num_sun += 25;
}

bool
GameWorld::ZombiesOnRight (int)
{
  // TODO: Logic
  return true;
}

void
GameWorld::InitializeSeedPacks ()
{
  auto sunflower_seed = std::make_shared<Seed> (
      ImageID::SEED_SUNFLOWER, PlantType::SUNFLOWER, 50, 0, 240);
  auto peashooter_seed = std::make_shared<Seed> (
      ImageID::SEED_PEASHOOTER, PlantType::PEASHOOTER, 100, 1, 240);
  auto repeater_seed = std::make_shared<Seed> (
      ImageID::SEED_REPEATER, PlantType::REPEATER, 200, 3, 240);
  auto walnut_seed = std::make_shared<Seed> (
      ImageID::SEED_WALLNUT, PlantType::WALLNUT, 50, 2, 240);
  auto cherryBomb_seed = std::make_shared<Seed> (
      ImageID::SEED_CHERRY_BOMB, PlantType::CHERRYBOMB, 150, 4, 240);
  GameWorld::AddNewObject (sunflower_seed);
  GameWorld::AddNewObject (peashooter_seed);
  GameWorld::AddNewObject (repeater_seed);
  GameWorld::AddNewObject (walnut_seed);
  GameWorld::AddNewObject (cherryBomb_seed);
}

void
GameWorld::InitializePlatingSpots ()
{
  for (int row = 0; row < 5; row++) {
    for (int col = 0; col < 9; col++) {
      GameWorld::AddNewObject (std::make_shared<PlantingSpot> (row, col));
    }
  }
}

void
GameWorld::SetShovel ()
{
  auto shovel = std::make_shared<Shovel> ();
  GameWorld::AddNewObject (shovel);
}

void
GameWorld::Init ()
{
  GameObject::world	    = shared_from_this ();
  GameWorld::spotClickEvent = std::make_shared<NoAction> ();
  GameWorld::AddNewObject (std::make_shared<GameBackground> ());
  InitializePlatingSpots ();
  InitializeSeedPacks ();
  SetShovel ();
}

LevelStatus
GameWorld::Update ()
{
  GenerateSun ();
  sunText->SetText (std::to_string (GameWorld::num_sun));
  for (auto &gameObject_it : m_GameObjects) {
    gameObject_it->Update ();
  }
  for (auto it = m_GameObjects.begin (); it != m_GameObjects.end ();) {
    if ((*it)->IsDead ()) {
      if ((*it)->IsZombie ()) {
	auto zombie = std::dynamic_pointer_cast<Zombie> (*it);
	Zombie::RemoveZombie (zombie->row, zombie);
      }
      it = m_GameObjects.erase (it);
    } else it++;
  }
  return LevelStatus::ONGOING;
}

void
GameWorld::CleanUp ()
{
  // YOUR CODE HERE
}
