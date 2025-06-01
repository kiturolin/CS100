#include "pvz/GameObject/Zombie.hpp"
#include "pvz/GameWorld/GameWorld.hpp"

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
  zombies[row].push_back (nullptr);
}

void
Zombie::CheckLose ()
{
  if (GetX () < 0) world->SetLose ();
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

std::shared_ptr<GameObject>
Zombie::CheckCollision ()
{
  for (auto &object : GameWorld::m_GameObjects) {
    // Explosion的监测需要优先
    if (abs (object->GetY () - GetY ()) <= LAWN_GRID_HEIGHT
	&& abs (object->GetX () - GetX ()) <= LAWN_GRID_WIDTH
	&& object->IsExplosion ()) {
      return object;
    }
    // 常规的Pea与Plant的监测
    if (abs (object->GetY () - GetY ()) <= 25
	&& abs (GetX () - object->GetX ()) < 20 && !object->IsZombie ()
	&& !object->IsDead () && (object->IsPea () || object->IsPlant ())) {
      return object;
    }
  }
  // 运行到这里说明没有检测到任何碰撞
  return nullptr;
}

void
Zombie::HandleCollision (std::shared_ptr<GameObject> &object)
{
  if (object != nullptr) {
    if (object->IsExplosion ()) {
      SetDead ();
      return;
    }
    if (object->IsPea ()) {
      object->SetDead ();
      hp -= 20;
    }
    if (object->IsPlant ()) {
      auto plant = std::dynamic_pointer_cast<Plant> (object);
      plant->AdjustHp (-3);
      if (plant->IsDead ()) {
	if (IsPole () && IsRunning ()) {
	  PlayAnimation (AnimID::RUN);
	} else PlayAnimation (AnimID::WALK);
      } else PlayAnimation (AnimID::EAT);
    }
  }
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
    jumping (false),
    running (true),
    jump_anime_countdown (42)
{
}

void
RegularZombie::Update ()
{
  if (hp <= 0) {
    SetDead ();
    return;
  }
  CheckLose ();
  auto object = CheckCollision ();
  HandleCollision (object);
  if (GetCurrentAnimation () == AnimID::WALK) { MoveTo (GetX () - 1, GetY ()); }
}

void
BucketZombie::Update ()
{
  if (hp <= 0) {
    SetDead ();
    return;
  }
  CheckLose ();
  auto object = CheckCollision ();
  HandleCollision (object);
  if (GetCurrentAnimation () == AnimID::WALK) { MoveTo (GetX () - 1, GetY ()); }
  if (hp <= 300) ChangeImage (ImageID::REGULAR_ZOMBIE);
}

void
PoleZombie::HandleJump ()
{
  std::shared_ptr<GameObject> object = nullptr;
  if (!jumping) {
    MoveTo (GetX () - 40, GetY ());
    auto object = CheckCollision ();
    if (object != nullptr) printf ("find some object\n");
    MoveTo (GetX () + 40, GetY ());
  }
  if (object != nullptr && object->IsPlant ()) {
    if (object != nullptr) printf ("ready to jump\n");
    jumping = true;
    PlayAnimation (AnimID::JUMP);
  }
  if (jumping) jump_anime_countdown--;
  if (jump_anime_countdown <= 0) {
    running = false;
    jumping = false;
    PlayAnimation (AnimID::WALK);
    MoveTo (GetX () - 150, GetY ());
  }
}

void
PoleZombie::Update ()
{
  if (hp <= 0) {
    SetDead ();
    return;
  }
  CheckLose ();
  HandleJump ();
  if (GetCurrentAnimation () == AnimID::RUN) MoveTo (GetX () - 2, GetY ());
  if (!jumping) {
    auto object = CheckCollision ();
    HandleCollision (object);
  }
}

