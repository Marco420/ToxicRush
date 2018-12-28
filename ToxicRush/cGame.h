#ifndef _GAME_H
#define _GAME_H

/*
==================================================================================
cGame.h
==================================================================================
*/

#include <SDL.h>

// Game specific includes
#include "ToxicRush.h"
#include<random>
#include <string>

using namespace std;

class cGame
{
public:
	cGame();

	void initialise(SDL_Window* theSDLWND, SDL_Renderer* theRenderer);
	void run(SDL_Window* theSDLWND, SDL_Renderer* theRenderer);
	void cleanUp(SDL_Window* theSDLWND);
	void render(SDL_Window* theSDLWND, SDL_Renderer* theRenderer);
	void render(SDL_Window* theSDLWND, SDL_Renderer* theRenderer, double rotAngle, SDL_Point* spriteCentre);
	void update();
	void update(double deltaTime);
	bool getInput(bool theLoop);
	double getElapsedSeconds();

	static cGame* getInstance();

private:




	static cGame* pInstance;
	// for framerates
	time_point< high_resolution_clock > m_lastTime;
	time_point< high_resolution_clock > m_CurrentTime;
	duration< double > deltaTime;
	bool loop;

	/* Let the computer pick a random number */
	random_device rd;    // non-deterministic engine 
	mt19937 gen{ rd() }; // deterministic engine. For most common uses, std::mersenne_twister_engine, fast and high-quality.
	uniform_int_distribution<> spriteRandom{ 0, 9 };

	
	cSprite spriteBkgd;
	
	
	vector<LPCSTR> tableNums;
	fstream highScoreFile;
	fstream highScoreNameFile;

	
	//Texture related variables
	vector<LPCSTR> textureName;
	vector<LPCSTR> textName;
	vector<LPCSTR> texturesToUse;
	vector<LPCSTR> objectTextures;
	// Fonts to use
	vector<LPCSTR> fontList;
	vector<LPCSTR> fontsToUse;
	// Text for Game
	vector<LPCSTR> gameTextNames;
	vector<LPCSTR> gameTextList;
	vector<LPCSTR> highScoreTextures;
	// Game Sounds
	vector<LPCSTR> soundList;
	vector<soundType> soundTypes;
	vector<LPCSTR> soundsToUse;
	// Create vector array of button textures
	vector<LPCSTR> btnNameList;
	vector<LPCSTR> btnTexturesToUse;
	vector<SDL_Point> btnPos;
	vector <cButton> theButtons;
	vector<string> alphabet;	// Game objects
	// Define the elements and there position in/on the array/map

	//int variables
	int renderWidth, renderHeight;
	int objectSpeed;
	int score;
	bool updateScoreTable = true;
	vector<int> yScorePos = {250,300,350,400,450};
	gameState theGameState;
	btnTypes theBtnType;
	SDL_Rect pos;
	FPoint scale;
	SDL_Rect aRect;
	SDL_Color aColour;
	cTexture* tempTextTexture;
	SDL_Point theAreaClicked;
	//Variables for HighScore Table
	string strScore;
	string playerName;
	bool enemySwitch = false;
	bool gameOver;
	bool newHighScore = false;
	vector<int> letters = { 0,0,0 };
	
	SDL_Joystick *joyStick;

	//Variables for gameObjects
	cPlayer player;
	cSprite highScoreBar;
	vector <cEnemy*> enemies;
	vector<cCloud*> theClouds;
	vector<cSprite*> theLifes;
	vector<cBin*> bins;
	vector<cGameObject*> walls;
	cPickUp pickUp;
	vector<cPlayerBullet*> playerBullets;
	string theHighScoreTable;
	int theHSTableSize;
	cHighScoreTable theHSTable;
	int numTableItems;
	



	//game variables
	int lifes = 3;
	int i = 0;
	bool gunPickUp = false;
	bool controllerActive = false;
	int bullets = 2;
	bool updateScore = false;
	vector<int> enemySpawnPositions = {2650,4450,9690,7990,8550};
	vector<int> wallSpawnPositions = { 11000,1524,10300,5750,6450};
	vector<int> binSpawnPositions = { 2205,3600,5150,9150,7200};
	int pickUpPos = 13500;
	controlsType controlType = controlsType::keyboard;
};

#endif
