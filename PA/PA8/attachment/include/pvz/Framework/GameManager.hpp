#ifndef GAMEMANAGER_H__
#define GAMEMANAGER_H__

#include "pvz/Framework/ObjectBase.hpp"
#include "pvz/Framework/WorldBase.hpp"

#include <map>
#include <memory>
#include <vector>

class GameManager
{
public:
  // Meyers' singleton pattern
  // 单例设计模式, 整个类有且只能有一个实例
  virtual ~GameManager () {}

  // 禁止使用拷贝与赋值构造
  GameManager (const GameManager &other)	    = delete;
  GameManager &operator= (const GameManager &other) = delete;

  // 获取单例的静态函数
  static GameManager &Instance ()
  {
    static GameManager instance;
    return instance;
  }

  void Play (int argc, char **argv, std::shared_ptr<WorldBase> world);

  bool GetKey (KeyCode key) const;
  bool GetKeyDown (KeyCode key);

  void Update ();
  void Display ();

  void KeyDownEvent (unsigned char key, int x, int y);
  void KeyUpEvent (unsigned char key, int x, int y);
  void SpecialKeyDownEvent (int key, int x, int y);
  void SpecialKeyUpEvent (int key, int x, int y);
  void MouseDownEvent (int x, int y);

  std::size_t DrawOneObject (ImageID imageID, AnimID animID, double x, double y, std::size_t frame) const;

private:
  enum class GameState { TITLE, ANIMATING, PROMPTING, GAMEOVER };
  // 构造函数私有化, 使得不能从外部直接构造对象, 符合单例设计模式
  GameManager ();
  // 将给定的像素坐标[0, pixel]转换为归一化坐标[-1.0, 1.0]
  inline double NormalizeCoord (double pixels, double totalPixels) const;
  // 将给定的归一化坐标转换为像素坐标
  inline int DenormalizeCoord (double normalizedCoord, double totalPixels) const;
  // 将给定的归一化坐标转换为像素坐标
  inline void Rotate (double x, double y, double degrees, double &xout, double &yout) const;
  void Prompt (const char *title, const char *subtitle) const;
  void ShowZombiesWon () const;

  // 检查输入, 并获取按下了什么按键
  inline KeyCode ToKeyCode (unsigned char key) const;
  inline KeyCode SpecialToKeyCode (int key) const;

  GameState m_gameState;
  std::shared_ptr<WorldBase> m_world;

  std::map<KeyCode, bool> m_pressedKeys;

  bool m_pause;
};
#endif	  // !GAMEMANAGER_H__
