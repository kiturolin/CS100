#ifndef TEXTBASE_H__
#define TEXTBASE_H__

#include "pvz/utils.hpp"

#include <iostream>
#include <set>

// 文字基类
class TextBase
{
public:
  TextBase (int x,
	    int y,
	    const std::string &text = "",
	    double colorR	    = 0,
	    double colorG	    = 0,
	    double colorB	    = 0,
	    bool m_centering	    = true);

  // 不允许拷贝或移动, 每个TextBase都应该是distinct的
  TextBase (const TextBase &other)	      = delete;
  TextBase (TextBase &&other)		      = delete;
  TextBase &operator= (const TextBase &other) = delete;
  TextBase &operator= (TextBase &&other)      = delete;
  virtual ~TextBase ();

  int GetX () const;
  int GetY () const;

  void MoveTo (int x, int y);
  void SetText (std::string text);

  // Sets color R, G, B in range [0, 1].
  void SetColor (double r, double g, double b);

  void SetCentering (bool centering);

  friend class GameManager;

private:
  int m_x;
  int m_y;

  std::string m_text;

  double m_colorR;
  double m_colorG;
  double m_colorB;

  bool m_centering;

private:
  // 注意! 这是static函数!
  template <typename Func>
  static void DisplayAllTexts (Func displayFunc)
  {
    for (const auto &obj : TextBase::s_allTexts) {
      displayFunc (obj->m_x, obj->m_y, obj->m_text, obj->m_colorR, obj->m_colorG, obj->m_colorB, obj->m_centering);
    }
  }

  // 静态的"全体文本对象"
  static std::set<TextBase *> s_allTexts;
};

#endif	  // !TEXTBASE_H__
