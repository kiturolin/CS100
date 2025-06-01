#ifndef GAMEWORLD_HPP__
#define GAMEWORLD_HPP__

#include "pvz/Framework/TextBase.hpp"
#include "pvz/Framework/WorldBase.hpp"
#include "pvz/GameObject/GameObject.hpp"
#include "pvz/utils.hpp"

#include <list>
#include <memory>

class GameWorld : public WorldBase,
		  public std::enable_shared_from_this<GameWorld>
{
public:
  // Consider:
  // Use shared_from_this() instead of "this" to create a pointer to oneself?
  // Use unique_ptr<> / shared_ptr<> to manage GameObjects?
  static int num_sun;
  static int waves;
  static std::shared_ptr<TextBase> sunText;
  static std::shared_ptr<TextBase> waveText;
  static uint32_t sun_time_interval;
  static std::shared_ptr<Actions> spotClickEvent;
  static std::list<std::shared_ptr<GameObject>> m_GameObjects;

  GameWorld ()	= default;
  ~GameWorld () = default;

  void Init () override;

  static void AddNewObject (const std::shared_ptr<GameObject> &sPtr);
  static void InitializeSeedPacks ();
  static void InitializePlatingSpots ();
  static void SetShovel ();
  static void SetWaves();
  static bool ZombiesOnRight (int);
  void GenerateSun ();
  void GenerateSun (int x, int y);
  void GetSun ();

  LevelStatus Update () override;

  void CleanUp () override;

private:
};

#endif	  // !GAMEWORLD_HPP__
