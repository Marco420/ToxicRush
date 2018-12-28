#ifndef _GAMECONSTANTS_H
#define _GAMECONSTANTS_H

// Windows & SDL 
#include <stdlib.h>
#include <Windows.h>
#include <iostream>
#include <string>
#include <cstdlib>

#include <SDL.h>
// Image Texture loading library
#include <SDL_image.h>
// Font loading library
#include <SDL_ttf.h>
// Sound loading library
#include <SDL_mixer.h>


// Maths functions
#include <math.h>
constexpr auto PI = 3.14159265;

// STL Container & Algorithms
#include <vector>
#include <map>
#include <algorithm>
#include <chrono>
#include<string.h>
using namespace std::chrono;

using namespace std;

//Define the string to appear in the top left corner of the window
#define WINDOW_TITLE "ToxicRush"
#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768
#define GROUND 485


// This header file contains all the constants & enumarated types for the game
typedef struct
{
	float  X;
	float  Y;
}FPoint;

typedef struct
{
	int  C;
	int  R;
}MapRC;

typedef struct
{
	float x, y;
	SDL_Texture *sprite;
} Player;




inline SDL_Point operator+(SDL_Point const &a, SDL_Point const &b)
{
	return SDL_Point{ a.x + b.x , a.y + b.y };
}

inline bool operator==(SDL_Rect const &a, SDL_Rect const &b)
{
	if (a.y == b.y && a.x == b.x)
	{
		return true;
	}
	else
	{
		return false;
	}
}

inline bool operator!=(MapRC const &a, MapRC const &b)
{
	if (a.C != b.C && a.R != b.R)
	{
		return true;
	}
	else
	{
		return false;
	}
}

enum class textType { solid, blended, shaded };
enum class soundType { sfx, music};
enum class gameState { menu, settings, playing, end, quit, highscore };
enum class btnTypes { exit, load, play, save, settings, hscore };
enum class gameObjectType { enemy, obstacle, pickUp };
enum class controlsType{controller,keyboard};

#endif

