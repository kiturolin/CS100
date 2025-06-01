#include "pvz/GameObject/GameObject.hpp"
#include "pvz/GameWorld/GameWorld.hpp"
#include "pvz/GameObject/Zombie.hpp"
#include "pvz/utils.hpp"

#include <cmath>
#include <cstdint>
#include <cstdio>
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







// Your everything begins from here.
//

// 植物函数
