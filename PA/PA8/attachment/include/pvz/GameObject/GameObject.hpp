#ifndef GAMEOBJECT_HPP__
#define GAMEOBJECT_HPP__

#include "pvz/Framework/ObjectBase.hpp"

// Declares the class name GameWorld so that its pointers can be used.
class GameWorld;

class GameObject : public ObjectBase
{
  // Use shared_from_this() instead of "this" to create a pointer to oneself.
public:
  GameObject (ImageID imageID, int x, int y, LayerID layer, int width, int height, AnimID animID) :
      ObjectBase (imageID, x, y, layer, width, height, animID)
  {
  }

private:
};

class GameBackground : public GameObject
{
public:
  static GameBackground &Instance ()
  {
    static GameBackground Instance;
    return Instance;
  }

  // 当update和click时, 应该do nothing
  void Update () override {};
  void OnClick () override {};

private:
  GameBackground ();
};

#endif	  // !GAMEOBJECT_HPP__
