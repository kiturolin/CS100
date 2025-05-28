#include "pvz/GameWorld/GameWorld.hpp"

void
GameWorld::Init ()
{
  m_GameObjects.push_back (&GameBackground::Instance ());
}

LevelStatus
GameWorld::Update ()
{
  for (auto &gameObject_it : m_GameObjects) {
    gameObject_it->Update ();
  }
  return LevelStatus::ONGOING;
}

void
GameWorld::CleanUp ()
{
  // YOUR CODE HERE
}
