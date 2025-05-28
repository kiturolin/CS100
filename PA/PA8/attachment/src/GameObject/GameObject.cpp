#include "pvz/GameObject/GameObject.hpp"
#include "pvz/utils.hpp"

GameBackground::GameBackground () :
    GameObject (ImageID::BACKGROUND,
		WINDOW_WIDTH / 2,
		WINDOW_HEIGHT / 2,
		LayerID::BACKGROUND,
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		AnimID::NO_ANIMATION) {};



// Your everything begins from here.
