#ifndef GAMEOBJECT_HPP__
#define GAMEOBJECT_HPP__

#include "pvz/Framework/ObjectBase.hpp"
#include "pvz/utils.hpp"

#include <cstdint>
#include <memory>
#include <type_traits>
#include <vector>

// Declares the class name GameWorld so that its pointers can be used.
class GameWorld;

class GameObject : public ObjectBase,
		   public std::enable_shared_from_this<GameObject>
{
  // Use shared_from_this() instead of "this" to create a pointer to oneself.
public:
  GameObject (ImageID imageID,
	      int x,
	      int y,
	      LayerID layer,
	      int width,
	      int height,
	      AnimID animID);
  static std::shared_ptr<GameWorld> world;
  void SetDead ();
  bool IsDead () const;

  virtual bool IsPea () { return false; };

  virtual bool IsZombie () { return false; };

  virtual bool IsExplosion () { return false; };

  virtual bool IsPlant () { return false; };


private:
  bool m_is_dead;
};


;
#endif	  // !GAMEOBJECT_HPP__
