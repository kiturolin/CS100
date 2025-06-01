#include "pvz/Framework/TextBase.hpp"
#include "pvz/GameObject/GameObject.hpp"
#include "pvz/GameObject/Other.hpp"
#include "pvz/GameObject/Zombie.hpp"
#include "pvz/GameWorld/GameWorld.hpp"
#include "pvz/utils.hpp"

#include <algorithm>
#include <cstdio>
#include <memory>
#include <ostream>
#include <string>

int GameWorld::num_sun			     = 0;
int GameWorld::waves			     = 0;
uint32_t GameWorld::sun_time_interval	     = 0;
std::shared_ptr<GameWorld> GameObject::world = nullptr;
std::shared_ptr<Actions> GameWorld::spotClickEvent
    = std::make_shared<NoAction> ();
std::list<std::shared_ptr<GameObject>> GameWorld::m_GameObjects;
std::shared_ptr<TextBase> GameWorld::sunText
    = std::make_shared<TextBase> (60, WINDOW_HEIGHT - 78);
std::shared_ptr<TextBase> GameWorld::waveText
    = std::make_shared<TextBase> (WINDOW_HEIGHT - 160, 8);

void
GameWorld::AddNewObject (const std::shared_ptr<GameObject> &sPtr)
{
  m_GameObjects.push_back (sPtr);
}

void
GameWorld::SetLose ()
{
  m_isLosing = true;
}

void
GameWorld::GenerateSun ()
{
  static int first_sun_interval = 180;
  first_sun_interval--;
  if (first_sun_interval < 0 && !((sun_time_interval++) % 300)) {
    auto sun = std::make_shared<Sun> (randInt (75, WINDOW_WIDTH - 75),
				      WINDOW_HEIGHT - 1,
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

void
GameWorld::GenerateZombies ()
{
  static int wave_interval = 30;
  if (!(wave_interval--)) {
    int num_zombies = 50;    //(15 + waves) / 10;
    int next_wave   = std::max (150, 600 - 20 * waves);
    int p1	    = 20;
    int p2	    = 2 * std::max (waves - 8, 10);
    int p3	    = 3 * std::max (waves - 15, 10);
    int num_regular = 0;    // num_zombies * (1.0F * p1 / (p1 + p2 + p3));
    int num_pole    = 0;    // num_zombies * (1.0F * p2 / (p1 + p2 + p3));
    int num_bucket  = 1;
    num_zombies * (1.0F * p3 / (p1 + p2 + p3));

    std::cout << "num zombies: " << num_zombies << std::endl;
    std::cout << "num regular: " << num_regular << std::endl;
    for (int i = 0; i < num_regular; i++) {
      auto row	   = 1;
      auto regular = std::make_shared<RegularZombie> (
	  row, WINDOW_WIDTH - 50, FIRST_ROW_CENTER + LAWN_GRID_HEIGHT * row);
      GameWorld::AddNewObject (regular);
    }
    for (int i = 0; i < num_pole; i++) {
      auto row	= randInt (0, 4);
      auto pole = std::make_shared<PoleZombie> (
	  row,
	  randInt (WINDOW_WIDTH - 40, WINDOW_WIDTH - 1),
	  FIRST_ROW_CENTER + LAWN_GRID_HEIGHT * row);
      GameWorld::AddNewObject (pole);
    }
    for (int i = 0; i < num_bucket; i++) {
      auto row	  = randInt (0, 4);
      auto bucket = std::make_shared<BucketZombie> (
	  row,
	  randInt (WINDOW_WIDTH - 40, WINDOW_WIDTH - 1),
	  FIRST_ROW_CENTER + LAWN_GRID_HEIGHT * row);
      GameWorld::AddNewObject (bucket);
    }
    waves++;
    wave_interval = next_wave;
  }
}

bool
GameWorld::ZombiesOnRight (int row)
{
  return !Zombie::zombies[row].empty ();
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
  num_sun += 1000;
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
  GenerateZombies ();
  sunText->SetText (std::to_string (GameWorld::num_sun));
  waveText->SetText ("Waves now: " + std::to_string (GameWorld::waves));
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
  if (m_isLosing) return LevelStatus::LOSING;
  return LevelStatus::ONGOING;
}

void
GameWorld::CleanUp ()
{
  m_isLosing = false;
  m_GameObjects.clear ();
  num_sun		    = 0;
  waves			    = 0;
  GameWorld::spotClickEvent = std::make_shared<NoAction> ();
}
