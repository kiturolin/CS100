#ifndef OBJECTBASE_H__
#define OBJECTBASE_H__

#include <iostream>
#include <set>

#include "pvz/utils.hpp"

// "游戏对象"基类
class ObjectBase {
public:
  // 初始化对象的外形图片与坐标, 图层数, 宽度与高度
  ObjectBase(ImageID imageID, int x, int y, LayerID layer, int width, int height, AnimID animID);

  // 禁止拷贝与移动
  ObjectBase(const ObjectBase& other) = delete;
  ObjectBase(ObjectBase&& other) = delete;
  ObjectBase& operator=(const ObjectBase& other) = delete;
  ObjectBase& operator=(ObjectBase&& other) = delete;
  // 虚析构函数
  virtual ~ObjectBase();

  virtual bool operator==(const ObjectBase& other);

  virtual void Update() = 0;
  virtual void OnClick() = 0;

  int GetX() const;
  int GetY() const;
  int GetWidth() const;
  int GetHeight() const;

  void MoveTo(int x, int y);
  
  AnimID GetCurrentAnimation() const;
  void ChangeImage(ImageID imageID);
  void PlayAnimation(AnimID animID);

  friend class GameManager;

private:
  ImageID m_imageID;
  int m_x;
  int m_y;
  LayerID m_layer;
  int m_width;
  int m_height;
  AnimID m_animID;
  std::size_t m_currentFrame;

private:
  template<typename Func>
  static void DisplayAllObjects(Func displayAndAnimateFunc) {
    for (int layer = MAX_LAYERS - 1; layer >= 0; layer--) {
      for (auto& obj : GetObjects(static_cast<LayerID>(layer))) {
        obj->m_currentFrame = displayAndAnimateFunc(obj->m_imageID, obj->m_animID, obj->m_x, obj->m_y, obj->m_currentFrame);
      }
    }
  }

  static void ClickAt(int x, int y) {
    for (int layer = 0; layer < MAX_LAYERS; layer++) {
      for (auto& obj : GetObjects(static_cast<LayerID>(layer))) {
        if (std::abs(x - obj->m_x) <= obj->m_width / 2 && std::abs(y - obj->m_y) <= obj->m_height / 2) {
          obj->OnClick();
          return;
        }
      }
    }
  }

  static std::set<ObjectBase*>& GetObjects(LayerID layer);

};


#endif // !OBJECTBASE_H__
