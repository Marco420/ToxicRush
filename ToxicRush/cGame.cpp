/*
==================================================================================
cGame.cpp
==================================================================================
*/
#include "cGame.h"

cGame* cGame::pInstance = NULL;
static cTextureMgr* theTextureMgr = cTextureMgr::getInstance();
static cFontMgr* theFontMgr = cFontMgr::getInstance();
static cSoundMgr* theSoundMgr = cSoundMgr::getInstance();
static cButtonMgr* theButtonMgr = cButtonMgr::getInstance();

/*
=================================================================================
Constructor
=================================================================================
*/
cGame::cGame()
{

}
/*
=================================================================================
Singleton Design Pattern
=================================================================================
*/
cGame* cGame::getInstance()
{
	if (pInstance == NULL)
	{
		pInstance = new cGame();
	}
	return cGame::pInstance;
}


void cGame::initialise(SDL_Window* theSDLWND, SDL_Renderer* theRenderer)
{
	// Initialize the joystick subsystem
	SDL_InitSubSystem(SDL_INIT_JOYSTICK);

	// Check for joystick
	if (SDL_NumJoysticks() > 0) {
		// Open joystick
		joyStick = SDL_JoystickOpen(0);

		if (joyStick) {
			printf("Opened Joystick 0\n");
			printf("Name: %s\n", SDL_JoystickNameForIndex(0));
			printf("Number of Axes: %d\n", SDL_JoystickNumAxes(joyStick));
			printf("Number of Buttons: %d\n", SDL_JoystickNumButtons(joyStick));
			printf("Number of Balls: %d\n", SDL_JoystickNumBalls(joyStick));
		}
		else {
			printf("Couldn't open Joystick 0\n");
		}
	}
	
	// Get width and height of render context
	SDL_GetRendererOutputSize(theRenderer, &renderWidth, &renderHeight);
	this->m_lastTime = high_resolution_clock::now();
	// Clear the buffer with a black background
	SDL_SetRenderDrawColor(theRenderer, 0, 0, 0, 255);
	SDL_RenderPresent(theRenderer);

	random_device rd;    // non-deterministic engine 
	mt19937 gen{ rd() }; // deterministic engine. For most common uses, std::mersenne_twister_engine, fast and high-quality.
	uniform_int_distribution<> cloudTextDis{ 9, 10 };
	uniform_int_distribution<> cloudDistX{ 1000,1400};
	uniform_int_distribution<> cloudDistY{ -50,25};
	uniform_int_distribution<> objectText{6,9};
	uniform_int_distribution<> spawnPos{ 0,4 };
	
	theTextureMgr->setRenderer(theRenderer);
	theFontMgr->initFontLib();
	theSoundMgr->initMixer();

	theAreaClicked = { 0, 0 };
	//Store inGameObjects textures
	textureName = { "menu","game","end","player","playerGun","doctor","zombie","bin","wall","cloud","cloud2","radSymbol","life","life1","pickUp","playerBullet","playerDeath","highScoreBar" };
	texturesToUse = { "Images/Bkg/OpeningScreen.png","Images/Bkg/GameScreen4.png","Images/Bkg/EndingScreen.png","Images/Player/PlayerNoGun.png","Images/Player/PlayerGun.png","Images/Enemies/Zombie2.png","Images/Enemies/Zombie.png","Images/GameTextures/bin.png","Images/GameTextures/SquareWall.png","Images/GameTextures/MainCloud.png","Images/GameTextures/MainCLoud1.png","Images/Bkg/RadSymbol.png","Images/GameTextures/Life.png","Images/GameTExtures/Life1.png","Images/PickUps/GunPickUp.png","Images/GameTextures/PlayerBullet.png","Images/Player/playerDeath.png","Images/Bkg/HighScoreBar.png" };
	for (unsigned int tCount = 0; tCount < textureName.size(); tCount++)
	{
		theTextureMgr->addTexture(textureName[tCount], texturesToUse[tCount]);	
	}
	tempTextTexture = theTextureMgr->getTexture("menu");
	aRect = { 0, 0, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
	aColour = { 228, 213, 238, 255 };

	// Store the button textures
	btnNameList = { "play_btn","settings_btn","quit_btn","highScore_btn","menu_btn","cntrl_btn","kbrd_btn"};
	btnTexturesToUse = { "Images/GameTextures/button.png","Images/GameTextures/button.png","Images/GameTextures/button.png","Images/GameTextures/button.png","Images/GameTextures/button.png","Images/GameTextures/button.png","Images/GameTextures/button.png" };
	btnPos = { { 400, 375 },{ 400, 375 } ,{ 400, 375 } ,{ 400, 375 },{50,10} ,{100,450},{100,650} };
	for (unsigned int bCount = 0; bCount < btnNameList.size(); bCount++)
	{
		theTextureMgr->addTexture(btnNameList[bCount], btnTexturesToUse[bCount]);
	}
	for (unsigned int bCount = 0; bCount < btnNameList.size(); bCount++)
	{
		cButton * newBtn = new cButton();
		newBtn->setTexture(theTextureMgr->getTexture(btnNameList[bCount]));
		newBtn->setSpritePos(btnPos[bCount]);
		newBtn->setSpriteDimensions(theTextureMgr->getTexture(btnNameList[bCount])->getTWidth(), theTextureMgr->getTexture(btnNameList[bCount])->getTHeight());
		theButtonMgr->add(btnNameList[bCount], newBtn);
	}
	theGameState = gameState::menu;
	theBtnType = btnTypes::exit;

	// Create textures for Game Dialogue (text)
	fontList = { "gameTxt","buttonTxt" };
	fontsToUse = { "Fonts/GameText.ttf","Fonts/ButtonText.ttf"};
	for (unsigned int fonts = 0; fonts < fontList.size(); fonts++)
	{
		if (fonts == 0)
		{
			theFontMgr->addFont(fontList[fonts], fontsToUse[fonts], 135);
		}
		else 
		{
			theFontMgr->addFont(fontList[fonts], fontsToUse[fonts], 30);
		}
	}

	// Create text Textures
	alphabet = { "A","B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "X", "Y", "Z","0","1","2","3","4","5","6","7","8","9" };
	tableNums = { "1.","2.","3.","4.","5." };
	gameTextNames = { "TitleTxt","startTxt","quitTxt","settingsTxt","highScoreTxt","gameOverTxt","youLoseTxt","menuTxt","replayTxt","scoreTxt","keyboard1Txt","keyboard2Txt","controller1Txt","controller2Txt","cntrlTxt","kbrdTxt","HsTableTxt","HScore","controllerSelectedTxt","keyboardSelectedTxt","chooseTxt","endScoreTxt","newHighScoreTxt","playerNameTxt","arrowCntrlsTxt" };
	gameTextList = { "Toxic Rush","START","QUIT","SETTINGS","HIGHSCORE","GAME OVER","YOU LOSE","MENU","REPLAY" , " 0 ","Press UP ARROW to jump"," SPACE BAR to shoot when equipped with gun","press A button to jump,"," B button to shoot when equipped with gun","CONTROLLER","KEYBOARD"," HIGH SCORE TABLE","HIGH SCORE TABLE","CONTROLS: CONTROLLER","CONTROLS: KEYBOARD","Choose controls to use","PLAYER SCORE:","NEW HIGHSCORE!","ENTER PLATER NAME:","USE ARROWS TO SET INITIALS AND PRESS ENTER TO SAVE HIGH SCORE" };
	for (unsigned int text = 0; text < gameTextNames.size(); text++)
	{
		if (text == 0 || text == 5 )
		{
			theTextureMgr->addTexture(gameTextNames[text], theFontMgr->getFont("gameTxt")->createTextTexture(theRenderer, gameTextList[text], textType::solid, { 8, 93, 8, 255 }, { 0, 0, 0, 0 }));
		}
		else if (text == gameTextList.capacity()-1)
		{
			theTextureMgr->addTexture(gameTextNames[text], theFontMgr->getFont("buttonTxt")->createTextTexture(theRenderer, gameTextList[text], textType::solid, { 255, 255, 255, 255 }, { 0, 0, 0, 0 }));
		}
		else
		{
			theTextureMgr->addTexture(gameTextNames[text], theFontMgr->getFont("buttonTxt")->createTextTexture(theRenderer, gameTextList[text], textType::solid, { 8, 93, 8, 255 }, { 0, 0, 0, 0 }));
		}
	}

	for (unsigned int text = 0; text < alphabet.size(); text++)
	{
		theTextureMgr->addTexture(alphabet[text].c_str(), theFontMgr->getFont("buttonTxt")->createTextTexture(theRenderer, alphabet[text].c_str(), textType::solid, { 8, 93, 8, 255 }, { 0, 0, 0, 0 }));
	}

	for (unsigned int text = 0; text < tableNums.size(); text++)
	{
		theTextureMgr->addTexture(tableNums[text], theFontMgr->getFont("buttonTxt")->createTextTexture(theRenderer, tableNums[text], textType::solid, { 8, 93, 8, 255 }, { 0, 0, 0, 0 }));
	}

	numTableItems = 0;
	
	theHSTable.loadFromFile("Data/HighScore.dat");
	theHSTableSize = theHSTable.getTableSize();
	highScoreTextures = { "score1","score2","score3","score4","score5","score6","score7","score8","score9","score10" };

	for (int item = 0; item < theHSTableSize; item++)
	{
		string entry = "";
		entry += theHSTable.getItem(item).Name + " " + to_string(theHSTable.getItem(item).score);
		theTextureMgr->addTexture(highScoreTextures[item], theFontMgr->getFont("buttonTxt")->createTextTexture(theRenderer, entry.c_str(), textType::solid, { 9, 93, 8, 255 }, { 0, 0, 0, 0 }));
	}

	// Load game sounds
	soundList = { "menu","game","shot","jump","lifeLost","lockAndLoad","enemyDeath"};
	soundTypes = { soundType::music,soundType::music,soundType::sfx,soundType::sfx,soundType::sfx,soundType::sfx,soundType::sfx};
	soundsToUse = { "Audio/MainMenu.wav","Audio/Game.wav","Audio/Shot.wav","Audio/Jump.wav","Audio/LifeLost.wav","Audio/LockAndLoad.wav","Audio/EnemyDeath.wav" };
	for (unsigned int sounds = 0; sounds < soundList.size(); sounds++)
	{
		theSoundMgr->add(soundList[sounds], soundsToUse[sounds], soundTypes[sounds]);
	}

	//play menu music
	theSoundMgr->getSnd("menu")->play(-1);
	
	//Set initial sprite background
	spriteBkgd.setSpritePos({ 0, 0 });
	spriteBkgd.setTexture(theTextureMgr->getTexture("menu"));
	spriteBkgd.setSpriteDimensions(theTextureMgr->getTexture("menu")->getTWidth(), theTextureMgr->getTexture("menu")->getTHeight());

	//Set initial highScoreBar position
	highScoreBar.setSpritePos({-10,115 });
	highScoreBar.setTexture(theTextureMgr->getTexture("highScoreBar"));
	highScoreBar.setSpriteDimensions(theTextureMgr->getTexture("highScoreBar")->getTWidth(), theTextureMgr->getTexture("highScoreBar")->getTHeight());
	
	//Set initial player position
	player.setSpritePos({150,GROUND});
	//Set initial pick up position
	pickUp.setSpritePos({pickUpPos,475});

	//Create cloud objects for background
	for (int cloud = 0; cloud < 4; cloud++)
	{
		if (cloud == 0)
		{
			theClouds.push_back(new cCloud);
			theClouds[cloud]->setSpritePos({ 1050 , 50  });
			int randCloud = cloudTextDis(gen);
			theClouds[cloud]->setTexture(theTextureMgr->getTexture(textureName[randCloud]));
			theClouds[cloud]->setSpriteDimensions(theTextureMgr->getTexture(textureName[randCloud])->getTWidth(), theTextureMgr->getTexture(textureName[randCloud])->getTHeight());
			theClouds[cloud]->setActive(true);
		}
		else
		{
			theClouds.push_back(new cCloud);
			theClouds[cloud]->setSpritePos({ theClouds[cloud - 1 ]->getSpritePos().x + cloudDistX(gen) , cloudDistY(gen)  });
			int randCloud = cloudTextDis(gen);
			theClouds[cloud]->setTexture(theTextureMgr->getTexture(theClouds[cloud]->textureName));
			theClouds[cloud]->setSpriteDimensions(theTextureMgr->getTexture(theClouds[cloud]->textureName)->getTWidth(), theTextureMgr->getTexture(theClouds[cloud]->textureName)->getTHeight());
			theClouds[cloud]->setActive(true);
		}
	}

	//Create life textures
	for (int life = 0; life < 3; life++)
	{
		if (life == 0)
		{
			theLifes.push_back(new cSprite);
			theLifes[life]->setSpritePos({ 800 , 20 });
			theLifes[life]->setTexture(theTextureMgr->getTexture("life"));
			theLifes[life]->setSpriteDimensions(theTextureMgr->getTexture("life")->getTWidth(), theTextureMgr->getTexture("life")->getTHeight());
			theLifes[life]->setActive(true);
		}
		else
		{
			theLifes.push_back(new cSprite);
			theLifes[life]->setSpritePos({ theLifes[life - 1]->getSpritePos().x + 65,20 });
			theLifes[life]->setTexture(theTextureMgr->getTexture("life"));
			theLifes[life]->setSpriteDimensions(theTextureMgr->getTexture("life")->getTWidth(), theTextureMgr->getTexture("life")->getTHeight());
			theLifes[life]->setActive(true);
		}
	}

	//Create zombie enemies
	for (unsigned int doctor = 0; doctor < enemySpawnPositions.capacity(); doctor++)
	{
		enemies.push_back(new cEnemy);
		enemies[doctor]->setSpritePos({ enemySpawnPositions[doctor] ,515 });
		enemies[doctor]->setTexture(theTextureMgr->getTexture("zombie"));
		enemies[doctor]->setSpriteDimensions(theTextureMgr->getTexture("zombie")->getTWidth(), theTextureMgr->getTexture("zombie")->getTHeight());
		enemies[doctor]->setActive(true);
	}
	//Create wall objects
	for (unsigned int wall = 0; wall < wallSpawnPositions.capacity(); wall++)
	{
		walls.push_back(new cGameObject);
		walls[wall]->setSpritePos({  wallSpawnPositions[wall] ,535 });
		walls[wall]->setTexture(theTextureMgr->getTexture("wall"));
		walls[wall]->setSpriteDimensions(theTextureMgr->getTexture("wall")->getTWidth(), theTextureMgr->getTexture("wall")->getTHeight());
		walls[wall]->setActive(true);
	}
	//create bin objects
	for (unsigned int bin = 0; bin < binSpawnPositions.capacity(); bin++)
	{
		bins.push_back(new cBin);
		bins[bin]->setSpritePos({ binSpawnPositions[bin] ,537 });
		bins[bin]->setTexture(theTextureMgr->getTexture("bin"));
		bins[bin]->setSpriteDimensions(theTextureMgr->getTexture("bin")->getTWidth(), theTextureMgr->getTexture("bin")->getTHeight());
		bins[bin]->setActive(true);
	}
}

void cGame::run(SDL_Window* theSDLWND, SDL_Renderer* theRenderer)
{
	loop = true;

	while (loop)
	{
		//We get the time that passed since the last frame
		double elapsedTime = this->getElapsedSeconds();

		loop = this->getInput(loop);
		this->update(elapsedTime);
		this->render(theSDLWND, theRenderer);
	}
}

void cGame::render(SDL_Window* theSDLWND, SDL_Renderer* theRenderer)
{
	SDL_RenderClear(theRenderer);
	switch (theGameState)
	{
	case gameState::menu:
	{
		//Render menu background
		spriteBkgd.setTexture(theTextureMgr->getTexture("menu"));
		spriteBkgd.setSpriteDimensions(theTextureMgr->getTexture("menu")->getTWidth(), theTextureMgr->getTexture("menu")->getTHeight());
		spriteBkgd.render(theRenderer, NULL, NULL, spriteBkgd.getSpriteScale());

		// Render the Title
		tempTextTexture = theTextureMgr->getTexture("TitleTxt");
		pos = { 150, 150, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		scale = { 1, 1};
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);

		if (controlType == controlsType::controller)
		{
			//Render controller activated text
			tempTextTexture = theTextureMgr->getTexture("controllerSelectedTxt");
			pos = { 340, 560, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
			scale = { 1, 1 };
			tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		}
		else
		{
			//Render keyboard activated text
			tempTextTexture = theTextureMgr->getTexture("keyboardSelectedTxt");
			pos = { 340, 560, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
			scale = { 1, 1 };
			tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		}

		/* Render Buttons
		-------------------*/
		//Start Button
		theButtonMgr->getBtn("play_btn")->setSpritePos({ 280, 375 });
		theButtonMgr->getBtn("play_btn")->render(theRenderer, &theButtonMgr->getBtn("play_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("play_btn")->getSpritePos(), theButtonMgr->getBtn("play_btn")->getSpriteScale());
		tempTextTexture = theTextureMgr->getTexture("startTxt");
		pos = { 330, 390, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		scale = { 1, 1 };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		//Settings Button
		theButtonMgr->getBtn("settings_btn")->setSpritePos({ 510, 375 });
		theButtonMgr->getBtn("settings_btn")->render(theRenderer, &theButtonMgr->getBtn("settings_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("settings_btn")->getSpritePos(), theButtonMgr->getBtn("settings_btn")->getSpriteScale());
		tempTextTexture = theTextureMgr->getTexture("settingsTxt");
		pos = { 533, 390, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		scale = { 1, 1 };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		//Quit Button
		theButtonMgr->getBtn("quit_btn")->setSpritePos({ 510, 465 });
		theButtonMgr->getBtn("quit_btn")->render(theRenderer, &theButtonMgr->getBtn("quit_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("quit_btn")->getSpritePos(), theButtonMgr->getBtn("quit_btn")->getSpriteScale());
		tempTextTexture = theTextureMgr->getTexture("quitTxt");
		pos = { 570, 480, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		scale = { 1, 1 };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		//High Score Table Button
		theButtonMgr->getBtn("highScore_btn")->setSpritePos({ 280, 465 });
		theButtonMgr->getBtn("highScore_btn")->render(theRenderer, &theButtonMgr->getBtn("highScore_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("highScore_btn")->getSpritePos(), theButtonMgr->getBtn("highScore_btn")->getSpriteScale());
		tempTextTexture = theTextureMgr->getTexture("highScoreTxt");
		pos = { 295, 480, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		scale = { 1, 1 };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		

		
	}
	break;
	case gameState::playing:
	{
		//Render background
		spriteBkgd.setTexture(theTextureMgr->getTexture("game"));
		spriteBkgd.setSpriteDimensions(theTextureMgr->getTexture("game")->getTWidth(), theTextureMgr->getTexture("game")->getTHeight());
		spriteBkgd.render(theRenderer, NULL, NULL, spriteBkgd.getSpriteScale());

		/*================================
			Render objects in game
		================================*/

		//Set player stats and Render
		player.setSpritePos({ player.getSpritePos().x,player.getSpritePos().y });
		player.setTexture(theTextureMgr->getTexture(player.getPlayerName()));
		player.setSpriteDimensions(theTextureMgr->getTexture(player.getPlayerName())->getTWidth(), theTextureMgr->getTexture(player.getPlayerName())->getTHeight());
		player.render(theRenderer, &player.getSpriteDimensions(), &player.getSpritePos(), player.getPlayerRotation(), &player.getSpriteCentre(), player.getSpriteScale());

		//Render each cloud in the vector array
		for (int draw = 0; draw < (int)theClouds.size(); draw++)
		{
			theClouds[draw]->render(theRenderer, &theClouds[draw]->getSpriteDimensions(), &theClouds[draw]->getSpritePos(), theClouds[draw]->getSpriteRotAngle(), &theClouds[draw]->getSpriteCentre(), theClouds[draw]->getSpriteScale());
		}
		// Render each bullet in the vector array
		for (int draw = 0; draw < (int)playerBullets.size(); draw++)
		{
			playerBullets[draw]->render(theRenderer, &playerBullets[draw]->getSpriteDimensions(), &playerBullets[draw]->getSpritePos(), playerBullets[draw]->getSpriteRotAngle(), &playerBullets[draw]->getSpriteCentre(), playerBullets[draw]->getSpriteScale());
		}
		//Render each life texture in the vector array
		for (int draw = 0; draw < (int)theLifes.size(); draw++)
		{
			theLifes[draw]->render(theRenderer, &theLifes[draw]->getSpriteDimensions(), &theLifes[draw]->getSpritePos(), theLifes[draw]->getSpriteRotAngle(), &theLifes[draw]->getSpriteCentre(), theLifes[draw]->getSpriteScale());
		}
		//Render each bin gameObject
		for (int draw = 0; draw < (int)bins.size(); draw++)
		{
			bins[draw]->render(theRenderer, &bins[draw]->getSpriteDimensions(), &bins[draw]->getSpritePos(), bins[draw]->getSpriteRotAngle(), &bins[draw]->getSpriteCentre(), bins[draw]->getSpriteScale());
		}
		//Render each enemy
		for (int draw = 0; draw < (int)enemies.size(); draw++)
		{
			enemies[draw]->render(theRenderer, &enemies[draw]->getSpriteDimensions(), &enemies[draw]->getSpritePos(), enemies[draw]->getSpriteRotAngle(), &enemies[draw]->getSpriteCentre(), enemies[draw]->getSpriteScale());
		}
		//Render each wall gameObject
		for (int draw = 0; draw < (int)walls.size(); draw++)
		{
			walls[draw]->render(theRenderer, &walls[draw]->getSpriteDimensions(), &walls[draw]->getSpritePos(), walls[draw]->getSpriteRotAngle(), &walls[draw]->getSpriteCentre(), walls[draw]->getSpriteScale());
		}
		//Set pickUp stats and render
		pickUp.setSpritePos({ pickUp.getSpritePos().x,pickUp.getSpritePos().y });
		pickUp.setTexture(theTextureMgr->getTexture("pickUp"));
		pickUp.setSpriteDimensions(theTextureMgr->getTexture("pickUp")->getTWidth(), theTextureMgr->getTexture("pickUp")->getTHeight());
		pickUp.render(theRenderer, &pickUp.getSpriteDimensions(), &pickUp.getSpritePos(), pickUp.getSpriteRotAngle(), &pickUp.getSpriteCentre(), pickUp.getSpriteScale());

		//Display game controls based on chosen controls 
		if (controlType == controlsType::controller && score < 200)
		{
			tempTextTexture = theTextureMgr->getTexture("controller1Txt");
			pos = { 10 ,260,tempTextTexture->getTextureRect().w,tempTextTexture->getTextureRect().h };
			scale = { 1,1 };
			tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
			tempTextTexture = theTextureMgr->getTexture("controller2Txt");
			pos = { 10 ,300,tempTextTexture->getTextureRect().w,tempTextTexture->getTextureRect().h };
			scale = { 1,1 };
			tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);

		}
		else if(controlType == controlsType::keyboard && score < 200)
		{
			tempTextTexture = theTextureMgr->getTexture("keyboard1Txt");
			pos = { 10 ,260,tempTextTexture->getTextureRect().w,tempTextTexture->getTextureRect().h };
			scale = { 1,1 };
			tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
			tempTextTexture = theTextureMgr->getTexture("keyboard2Txt");
			pos = { 10 ,300,tempTextTexture->getTextureRect().w,tempTextTexture->getTextureRect().h };
			scale = { 1,1 };
			tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		}
			
		/* Render Buttons
		-------------------*/

		//exit button 
		theButtonMgr->getBtn("menu_btn")->setSpritePos({ 10,10 });
		theButtonMgr->getBtn("menu_btn")->render(theRenderer, &theButtonMgr->getBtn("menu_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("menu_btn")->getSpritePos(), theButtonMgr->getBtn("menu_btn")->getSpriteScale());
		tempTextTexture = theTextureMgr->getTexture("menuTxt");
		pos = { 65,25,tempTextTexture->getTextureRect().w,tempTextTexture->getTextureRect().h };
		scale = { 1,1 };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);

		//Update score texture
		if (updateScore)
		{
			theTextureMgr->deleteTexture(gameTextNames[9]);
			theTextureMgr->addTexture(gameTextNames[9],theFontMgr->getFont("buttonTxt")->createTextTexture(theRenderer,strScore.c_str(), textType::solid, { 44, 203, 112, 255 }, { 0, 0, 0, 0 }));
			updateScore = false;
		}
		tempTextTexture = theTextureMgr->getTexture("scoreTxt");
		pos = { 450,25,tempTextTexture->getTextureRect().w,tempTextTexture->getTextureRect().h };
		scale = { 1,1 };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);	
	}
	break;
	case gameState::end:
	{
		//Render Background
		spriteBkgd.setTexture(theTextureMgr->getTexture("end"));
		spriteBkgd.setSpriteDimensions(theTextureMgr->getTexture("end")->getTWidth(), theTextureMgr->getTexture("menu")->getTHeight());
		spriteBkgd.render(theRenderer, NULL, NULL, spriteBkgd.getSpriteScale());

		// Render the Title
		tempTextTexture = theTextureMgr->getTexture("gameOverTxt");
		pos = { 25, 25, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		scale = { 1, 1 };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
	
		/* Render Buttons
		-------------------*/

		//Menu button
		theButtonMgr->getBtn("menu_btn")->setSpritePos({ 510, 375 });
		theButtonMgr->getBtn("menu_btn")->render(theRenderer, &theButtonMgr->getBtn("menu_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("menu_btn")->getSpritePos(), theButtonMgr->getBtn("menu_btn")->getSpriteScale());
		tempTextTexture = theTextureMgr->getTexture("menuTxt");
		pos = { 560, 390, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		scale = { 1, 1 };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		//Play button
		theButtonMgr->getBtn("play_btn")->setSpritePos({ 280, 375 });
		theButtonMgr->getBtn("play_btn")->render(theRenderer, &theButtonMgr->getBtn("play_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("play_btn")->getSpritePos(), theButtonMgr->getBtn("play_btn")->getSpriteScale());
		tempTextTexture = theTextureMgr->getTexture("replayTxt");
		pos = { 330, 390, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		scale = { 1, 1 };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		//Quit Button
		theButtonMgr->getBtn("quit_btn")->setSpritePos({ 510, 465 });
		theButtonMgr->getBtn("quit_btn")->render(theRenderer, &theButtonMgr->getBtn("quit_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("quit_btn")->getSpritePos(), theButtonMgr->getBtn("quit_btn")->getSpriteScale());
		tempTextTexture = theTextureMgr->getTexture("quitTxt");
		pos = { 570, 480, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		scale = { 1, 1 };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		//High Score Table Button
		theButtonMgr->getBtn("highScore_btn")->setSpritePos({ 280, 465 });
		theButtonMgr->getBtn("highScore_btn")->render(theRenderer, &theButtonMgr->getBtn("highScore_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("highScore_btn")->getSpritePos(), theButtonMgr->getBtn("highScore_btn")->getSpriteScale());
		tempTextTexture = theTextureMgr->getTexture("highScoreTxt");
		pos = { 295, 480, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		scale = { 1, 1 };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);

		if (newHighScore)
		{
			highScoreBar.render(theRenderer, &highScoreBar.getSpriteDimensions(), &highScoreBar.getSpritePos(), highScoreBar.getSpriteRotAngle(), &highScoreBar.getSpriteCentre(), highScoreBar.getSpriteScale());
			tempTextTexture = theTextureMgr->getTexture("newHighScoreTxt");
			pos = { 350, 325, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
			scale = { 1, 1 };
			tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);

			tempTextTexture = theTextureMgr->getTexture("endScoreTxt");
			pos = { 250,395,tempTextTexture->getTextureRect().w,tempTextTexture->getTextureRect().h };
			scale = { 1,1 };
			tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
			tempTextTexture = theTextureMgr->getTexture("playerNameTxt");
			pos = { 250,450,tempTextTexture->getTextureRect().w,tempTextTexture->getTextureRect().h };
			scale = { 1,1 };
			tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);

			tempTextTexture = theTextureMgr->getTexture("scoreTxt");
			pos = { 510,395,tempTextTexture->getTextureRect().w,tempTextTexture->getTextureRect().h };
			scale = { 1,1 };
			tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);

			tempTextTexture = theTextureMgr->getTexture("arrowCntrlsTxt");
			pos = { 5,695,tempTextTexture->getTextureRect().w,tempTextTexture->getTextureRect().h };
			scale = { 1,1 };
			tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);

			tempTextTexture = theTextureMgr->getTexture(alphabet[letters[0]].c_str());
			pos = { 600,450,tempTextTexture->getTextureRect().w,tempTextTexture->getTextureRect().h };
			scale = { 1,1 };
			tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);

			tempTextTexture = theTextureMgr->getTexture(alphabet[letters[1]].c_str());
			pos = { 625,450,tempTextTexture->getTextureRect().w,tempTextTexture->getTextureRect().h };
			scale = { 1,1 };
			tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);

			tempTextTexture = theTextureMgr->getTexture(alphabet[letters[2]].c_str());
			pos = { 650,450,tempTextTexture->getTextureRect().w,tempTextTexture->getTextureRect().h };
			scale = { 1,1 };
			tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		}
	}
	break;
	case gameState::highscore:
	{
		//Render Background
		spriteBkgd.setTexture(theTextureMgr->getTexture("menu"));
		spriteBkgd.setSpriteDimensions(theTextureMgr->getTexture("menu")->getTWidth(), theTextureMgr->getTexture("menu")->getTHeight());
		spriteBkgd.render(theRenderer, NULL, NULL, spriteBkgd.getSpriteScale());
		//Render Game Title
		tempTextTexture = theTextureMgr->getTexture("TitleTxt");
		pos = { 150, 10, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		scale = { 1, 1 };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		// Render subTitle
		tempTextTexture = theTextureMgr->getTexture("HsTableTxt");
		pos = { 345, 120, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		scale = { 1, 1 };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		if (updateScoreTable)
		{
			for (unsigned int score = 0; score < 5; score++)
			{
				theTextureMgr->deleteTexture(highScoreTextures[score]);
				theTextureMgr->addTexture(highScoreTextures[score], theFontMgr->getFont("buttonTxt")->createTextTexture(theRenderer, (theHSTable.getItem(score).Name + " " + to_string(theHSTable.getItem(score).score)).c_str(), textType::solid, { 8, 93, 8, 255 }, { 0, 0, 0, 0 }));

				
			}
			updateScoreTable = false;
		}
				
		for (unsigned int x = 0; x < 5; x++)
		{
			tempTextTexture = theTextureMgr->getTexture(highScoreTextures[x]);
			pos = { 480, yScorePos[x], tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
			scale = { 1, 1 };
			tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		}

		for (unsigned int x = 0; x < 5; x++)
		{
			tempTextTexture = theTextureMgr->getTexture(tableNums[x]);
			pos = { 435, yScorePos[x], tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
			scale = { 1, 1 };
			tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		}

		/* Render Buttons
		-------------------*/

		//Render menu button
		theButtonMgr->getBtn("menu_btn")->setSpritePos({ 800, 700 });
		theButtonMgr->getBtn("menu_btn")->render(theRenderer, &theButtonMgr->getBtn("menu_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("menu_btn")->getSpritePos(), theButtonMgr->getBtn("menu_btn")->getSpriteScale());
		tempTextTexture = theTextureMgr->getTexture("menuTxt");
		pos = { 850,715,tempTextTexture->getTextureRect().w,tempTextTexture->getTextureRect().h };
		scale = { 1,1 };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
	}
	break;
	case gameState::settings:
	{
		//Render background
		spriteBkgd.setTexture(theTextureMgr->getTexture("menu"));
		spriteBkgd.setSpriteDimensions(theTextureMgr->getTexture("menu")->getTWidth(), theTextureMgr->getTexture("menu")->getTHeight());
		spriteBkgd.render(theRenderer, NULL, NULL, spriteBkgd.getSpriteScale());
		//Render Game Title
		tempTextTexture = theTextureMgr->getTexture("TitleTxt");
		pos = { 150, 10, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		scale = { 1, 1 };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		// Render subTitle
		tempTextTexture = theTextureMgr->getTexture("settingsTxt");
		pos = { 420, 120, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		scale = { 1, 1 };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		// Render choose title
		tempTextTexture = theTextureMgr->getTexture("chooseTxt");
		pos = { 305, 260, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		scale = { 1, 1 };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);

		if (controlType == controlsType::controller)
		{
			//Render controller activated text
			tempTextTexture = theTextureMgr->getTexture("controllerSelectedTxt");
			pos = { 325, 485, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
			scale = { 1, 1 };
			tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		}
		else
		{
			//Render keyboard activated text
			tempTextTexture = theTextureMgr->getTexture("keyboardSelectedTxt");
			pos = { 325, 485, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
			scale = { 1, 1 };
			tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		}


		//Render menu button
		theButtonMgr->getBtn("menu_btn")->setSpritePos({ 800, 700 });
		theButtonMgr->getBtn("menu_btn")->render(theRenderer, &theButtonMgr->getBtn("menu_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("menu_btn")->getSpritePos(), theButtonMgr->getBtn("menu_btn")->getSpriteScale());
		tempTextTexture = theTextureMgr->getTexture("menuTxt");
		pos = { 850,715,tempTextTexture->getTextureRect().w,tempTextTexture->getTextureRect().h };
		scale = { 1,1 };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		//Render controller button
		theButtonMgr->getBtn("cntrl_btn")->setSpritePos({ 280,345 });
		theButtonMgr->getBtn("cntrl_btn")->render(theRenderer, &theButtonMgr->getBtn("cntrl_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("cntrl_btn")->getSpritePos(), theButtonMgr->getBtn("cntrl_btn")->getSpriteScale());
		tempTextTexture = theTextureMgr->getTexture("cntrlTxt");
		pos = { 292 ,360,tempTextTexture->getTextureRect().w,tempTextTexture->getTextureRect().h };
		scale = { 1,1 };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		//Render keyboard button
		theButtonMgr->getBtn("kbrd_btn")->setSpritePos({ 500,345 });
		theButtonMgr->getBtn("kbrd_btn")->render(theRenderer, &theButtonMgr->getBtn("kbrd_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("kbrd_btn")->getSpritePos(), theButtonMgr->getBtn("kbrd_btn")->getSpriteScale());
		tempTextTexture = theTextureMgr->getTexture("kbrdTxt");
		pos = { 515 ,360,tempTextTexture->getTextureRect().w,tempTextTexture->getTextureRect().h };
		scale = { 1,1 };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
	}
	break;
	case gameState::quit:
	{
		loop = false;
	}
	break;

	
	default:
		break;
	}
	SDL_RenderPresent(theRenderer);
}

void cGame::render(SDL_Window* theSDLWND, SDL_Renderer* theRenderer, double rotAngle, SDL_Point* spriteCentre)
{
	SDL_RenderPresent(theRenderer);
}

void cGame::update()
{
}

void cGame::update(double deltaTime)
{	
	if (theGameState == gameState::menu || theGameState == gameState::end )
	{
		if (!newHighScore)
		{
			//Button actions
			theGameState = theButtonMgr->getBtn("quit_btn")->update(theGameState, gameState::quit, theAreaClicked);
			theGameState = theButtonMgr->getBtn("menu_btn")->update(theGameState, gameState::menu, theAreaClicked);
			theGameState = theButtonMgr->getBtn("highScore_btn")->update(theGameState, gameState::highscore, theAreaClicked);
			theGameState = theButtonMgr->getBtn("play_btn")->update(theGameState, gameState::playing, theAreaClicked);
		}
	}

	if (theGameState == gameState::settings)
	{
		theGameState = theButtonMgr->getBtn("menu_btn")->update(theGameState, gameState::menu, theAreaClicked);
		controlType = theButtonMgr->getBtn("kbrd_btn")->updateControls(controlType, controlsType::keyboard, theAreaClicked);
		controlType = theButtonMgr->getBtn("cntrl_btn")->updateControls(controlType, controlsType::controller, theAreaClicked);
	}
	
	if (theGameState == gameState::playing  )
	{
		
		//Button to change game state
		theGameState = theButtonMgr->getBtn("menu_btn")->update(theGameState, gameState::menu, theAreaClicked);

		//Re generate game Object pattern
		if (enemies.empty()&&walls.empty() &&bins.empty())
		{
			for (unsigned int gameObjects = 0; gameObjects < enemySpawnPositions.capacity(); gameObjects++)
			{
				enemies.push_back(new cEnemy);
				enemies[gameObjects]->setSpritePos({ enemySpawnPositions[gameObjects] ,515 });
				enemies[gameObjects]->setTexture(theTextureMgr->getTexture("doctor"));
				enemies[gameObjects]->setSpriteDimensions(theTextureMgr->getTexture("doctor")->getTWidth(), theTextureMgr->getTexture("doctor")->getTHeight());
				enemies[gameObjects]->setActive(true);

				walls.push_back(new cGameObject);
				walls[gameObjects]->setSpritePos({ binSpawnPositions[gameObjects] ,535 });
				walls[gameObjects]->setTexture(theTextureMgr->getTexture("wall"));
				walls[gameObjects]->setSpriteDimensions(theTextureMgr->getTexture("wall")->getTWidth(), theTextureMgr->getTexture("wall")->getTHeight());
				walls[gameObjects]->setActive(true);

				bins.push_back(new cBin);
				bins[gameObjects]->setSpritePos({ wallSpawnPositions[gameObjects] ,537 });
				bins[gameObjects]->setTexture(theTextureMgr->getTexture("bin"));
				bins[gameObjects]->setSpriteDimensions(theTextureMgr->getTexture("bin")->getTWidth(), theTextureMgr->getTexture("bin")->getTHeight());
				bins[gameObjects]->setActive(true);
			}
		}
		/*
		===================================================================
							Check for collisions
		===================================================================
		*/

		//Check if player collides with a bin object and take actions
		for (vector<cBin*>::iterator binIterator = bins.begin(); binIterator != bins.end(); ++binIterator)
		{
				if ((&player)->collidedWith(&player.getBoundingRect(), &(*binIterator)->getBoundingRect()) && lifes > 0)
				{
					(*binIterator)->setActive(false);
					int index = lifes - 1;
					theLifes[index]->setTexture(theTextureMgr->getTexture("life1"));
					theLifes[index]->setSpriteDimensions(theTextureMgr->getTexture("life1")->getTWidth(), theTextureMgr->getTexture("life1")->getTHeight());
					--lifes;
				}
		}

		//Check if player collides with a wall object and take actions
		for (vector<cGameObject*>::iterator wallIterator = walls.begin(); wallIterator != walls.end(); ++wallIterator)
		{
			if ((&player)->collidedWith(&player.getBoundingRect(), &(*wallIterator)->getBoundingRect()))
			{
				(*wallIterator)->setActive(false);
				int index = lifes - 1;
				theLifes[index]->setTexture(theTextureMgr->getTexture("life1"));
				theLifes[index]->setSpriteDimensions(theTextureMgr->getTexture("life1")->getTWidth(), theTextureMgr->getTexture("life1")->getTHeight());
				--lifes;
				
			}
		}
		//Check if player collides with an enemy and take actions
		for (vector<cEnemy*>::iterator enemyIterator = enemies.begin(); enemyIterator != enemies.end(); ++enemyIterator)
		{
			if ((&player)->collidedWith(&player.getBoundingRect(), &(*enemyIterator)->getBoundingRect()))
			{
				int index = lifes - 1;
				theLifes[index]->setTexture(theTextureMgr->getTexture("life1"));
				theLifes[index]->setSpriteDimensions(theTextureMgr->getTexture("life1")->getTWidth(), theTextureMgr->getTexture("life1")->getTHeight());
				(*enemyIterator)->setActive(false);
				--lifes;
			}
		}

		//Check if player collides with a pick Up and take actions
		if ((&player)->collidedWith(&player.getBoundingRect(), &pickUp.getBoundingRect()))
		{
			player.playerGun = true;
			pickUp.setSpritePos({ pickUpPos,450 });
		}

		//Check if player bullets collide with enemies.
		for (vector<cEnemy*>::iterator enemyIterator = enemies.begin(); enemyIterator != enemies.end(); ++enemyIterator)
		{
			for (vector<cPlayerBullet*>::iterator playerBulletIterator = playerBullets.begin(); playerBulletIterator != playerBullets.end(); playerBulletIterator++)
			{
				if ((*playerBulletIterator)->collidedWith(&(*playerBulletIterator)->getBoundingRect(), &(*enemyIterator)->getBoundingRect()))
				{
					
					(*enemyIterator)->setActive(false);
					(*playerBulletIterator)->setActive(false);
					score += 250;
					theSoundMgr->getSnd("enemyDeath")->play(0);

				}
			}
		}

		/*
		===================================================================
						 Update game objects in vectors
		===================================================================
		*/

		//Update or erase player bullet
		vector<cPlayerBullet*>::iterator playerBulletIterartor = playerBullets.begin();
		while (playerBulletIterartor != playerBullets.end())
		{
			if ((*playerBulletIterartor)->getSpritePos().x > WINDOW_WIDTH)
				(*playerBulletIterartor)->setActive(false);

			if ((*playerBulletIterartor)->isActive() == false)
			{
				playerBulletIterartor = playerBullets.erase(playerBulletIterartor);
			}
			else
			{
				(*playerBulletIterartor)->update(deltaTime);
				++playerBulletIterartor;
			}
		}
		//Update or erase enemies
		vector<cEnemy*>::iterator enemyIterator = enemies.begin();
		while (enemyIterator != enemies.end())
		{
			//If enemies position gets below screen width update score
			if ((*enemyIterator)->getSpritePos().x < 0)
			{
				(*enemyIterator)->setActive(false);
				score += 100;
				updateScore = true;
			}
			if ((*enemyIterator)->isActive() == false)
			{
				enemyIterator = enemies.erase(enemyIterator);
			}
			else
			{
				(*enemyIterator)->update(deltaTime);
				++enemyIterator;
			}
		}
		//Update or erase wall gameObjects
		vector<cGameObject*>::iterator wallIterator = walls.begin();
		while (wallIterator != walls.end())
		{
			//If walls position gets below screen width update score
			if ((*wallIterator)->getSpritePos().x < 0)
			{
				(*wallIterator)->setActive(false);
				score += 100;
				strScore += gameTextList[9];
				strScore = to_string(score).c_str();
				updateScore = true;
			}
			if ((*wallIterator)->isActive() == false)
			{
				wallIterator = walls.erase(wallIterator);
			}
			else
			{
				(*wallIterator)->update(deltaTime);
				++wallIterator;
			}
		}
		//Update clouds in vector 
		vector<cCloud*>::iterator cloudIterator = theClouds.begin();
		while (cloudIterator != theClouds.end())
		{
			if ((*cloudIterator)->isActive() == false)
			{
				cloudIterator = theClouds.erase(cloudIterator);
			}
			else
			{
				(*cloudIterator)->Update(deltaTime);
				++cloudIterator;
			}
		}
		//Update or erase bin gameObjects
		vector<cBin*>::iterator binIterator = bins.begin();
		while (binIterator != bins.end())
		{
			//If bins position gets below screen width update score
			if ((*binIterator)->getSpritePos().x < 0)
			{
				(*binIterator)->setActive(false);
				score += 100;
				strScore += gameTextList[9];
				strScore = to_string(score).c_str();
				updateScore = true;
			}
			if ((*binIterator)->isActive() == false)
			{
				binIterator = bins.erase(binIterator);
			}
			else
			{
				(*binIterator)->update(deltaTime);
				++binIterator;
			}
		}
		//Update life sprites in vector
		vector<cSprite*>::iterator lifeIterator = theLifes.begin();
		while (lifeIterator != theLifes.end())
		{
			if ((*lifeIterator)->isActive() == false)
			{
				lifeIterator = theLifes.erase(lifeIterator);
			}
			else
			{
				(*lifeIterator)->Update(deltaTime);
				++lifeIterator;
			}
		}

		//Take actios for when player ammo is empty
		if (bullets == 0)
		{
			player.playerGun = false;
			bullets = 3;
		}

		

		for (unsigned int x = 0; x < 5; x++)
		{
			if (score > theHSTable.getItem(x).score)
			{
				newHighScore = true;
			}
		}

		//Update player and pickup
		player.Update(deltaTime);
		pickUp.update(deltaTime);
		highScoreBar.Update(deltaTime);
	}

	
	

	if (theGameState == gameState::highscore)
	{
		//Button actions
		theGameState = theButtonMgr->getBtn("menu_btn")->update(theGameState, gameState::menu, theAreaClicked);
	}

	if (theGameState == gameState::end)
	{
		
			
				
				/*vector<cBin*>::iterator binIterator = bins.begin();
				while (binIterator != bins.end())
				{
					binIterator = bins.erase(binIterator);
				}

				vector<cEnemy*>::iterator enemyIterator = enemies.begin();
				while (enemyIterator != enemies.end())
				{
					enemyIterator = enemies.erase(enemyIterator);
				}

				vector<cGameObject*>::iterator wallIterator = walls.begin();
				while (wallIterator != walls.end())
				{
					wallIterator = walls.erase(wallIterator);
				}

				for (int index = 0; index < theLifes.capacity(); index++)
				{
					theLifes[index]->setTexture(theTextureMgr->getTexture("life"));
					theLifes[index]->setSpriteDimensions(theTextureMgr->getTexture("life")->getTWidth(), theTextureMgr->getTexture("life")->getTHeight());
				}
				score = 0;
				updateScore = true;*/
		
	}
	if (theGameState == gameState::end)
	{
		
		
	}
	if (lifes == 0)
	{
		theGameState = gameState::end;
	}
	/*if (lifes == 0)
	{
		theGameState = gameState::end;
		
	}*/
}

bool cGame::getInput(bool theLoop)
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
		{
			theLoop = false;
		}

		switch (event.type)
		{
			if (controlType == controlsType::controller)
			{
		case SDL_JOYBUTTONDOWN:
		// Handle Joystick Button Presses 
			switch (event.jbutton.button)
			{
				case SDL_CONTROLLER_BUTTON_B:
				if (theGameState == gameState::playing)
				{
					if (player.ground && controllerActive)
					{
						theSoundMgr->getSnd("jump")->play(0);
						player.Jump();
					}
				}
					break;
			case SDL_CONTROLLER_BUTTON_A:
				if (theGameState == gameState::playing)
				{
					if (player.ground && player.playerGun && controllerActive && bullets != 0)
					{
						theSoundMgr->getSnd("shot")->play(0);
						playerBullets.push_back(new cPlayerBullet);
						int numBullets = playerBullets.size() - 1;
						playerBullets[numBullets]->setSpritePos({ player.getBoundingRect().x + player.getSpriteCentre().x + 25, player.getBoundingRect().y + player.getSpriteCentre().y + 18 });
						playerBullets[numBullets]->setTexture(theTextureMgr->getTexture("playerBullet"));
						playerBullets[numBullets]->setSpriteDimensions(theTextureMgr->getTexture("playerBullet")->getTWidth(), theTextureMgr->getTexture("playerBullet")->getTHeight());
						playerBullets[numBullets]->setActive(true);
						--bullets;
					}
				}
				break;
			}
			}
		case SDL_MOUSEBUTTONDOWN:
			switch (event.button.button)
			{
			case SDL_BUTTON_LEFT:
			{
				theAreaClicked = { event.motion.x, event.motion.y };
				
			}
			break;
			case SDL_BUTTON_RIGHT:
				break;
			default:
				break;
			}
			break;
		case SDL_MOUSEBUTTONUP:
			switch (event.button.button)
			{
			case SDL_BUTTON_LEFT:
			{
			}
			break;
			case SDL_BUTTON_RIGHT:
				break;
			default:
				break;
			}
			break;
		case SDL_MOUSEMOTION:
		{
		}
		break;
		case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
				case SDLK_RETURN:
					newHighScore = false;
					playerName = alphabet[letters[0]] + alphabet[letters[1]] + alphabet[letters[2]];
					theHSTable.modifyItem(playerName,score);
					theHSTable.saveToFile("Data/HighScore.dat");		
					break;
				case SDLK_ESCAPE:
					theLoop = false;
					break;
				case SDLK_DOWN:
				{
					if (newHighScore)
					{
						if(letters[i]>0)
						letters[i] -= 1;
					}
				}
				break;
				
				case SDLK_UP:
				{
					if (theGameState == gameState::playing)
					{
						if (controlType == controlsType::keyboard)
						{
							if (player.ground)
							{
								theSoundMgr->getSnd("jump")->play(0);
								player.Jump();
							}
						}
					}

					if (newHighScore)
					{
						if(letters[i] < alphabet.capacity()-1)
						letters[i] += 1;
					}
				}
				break;
				case SDLK_SPACE:
				{
					if (theGameState == gameState::playing)
					{
						if (controlType == controlsType::keyboard)
						{
							if (player.ground && player.playerGun && !controllerActive && bullets != 0)
							{
								theSoundMgr->getSnd("shot")->play(0);
								playerBullets.push_back(new cPlayerBullet);
								int numBullets = playerBullets.size() - 1;
								playerBullets[numBullets]->setSpritePos({ player.getBoundingRect().x + player.getSpriteCentre().x + 25, player.getBoundingRect().y + player.getSpriteCentre().y + 18 });
								playerBullets[numBullets]->setTexture(theTextureMgr->getTexture("playerBullet"));
								playerBullets[numBullets]->setSpriteDimensions(theTextureMgr->getTexture("playerBullet")->getTWidth(), theTextureMgr->getTexture("playerBullet")->getTHeight());
								playerBullets[numBullets]->setActive(true);
								--bullets;
							}
						}

					}
				}
				case SDLK_RIGHT:
				{
					if(i<2)
					i += 1;
				}
				break;

				case SDLK_LEFT:
				{
					if(i>0)
					i -= 1;
				}
				break;

				break;
				default:
					break;
				}
		}

	}
	return theLoop;
}

double cGame::getElapsedSeconds()
{
	this->m_CurrentTime = high_resolution_clock::now();
	this->deltaTime = (this->m_CurrentTime - this->m_lastTime);
	this->m_lastTime = this->m_CurrentTime;
	return deltaTime.count();
}

void cGame::cleanUp(SDL_Window* theSDLWND)
{
	// Delete our OpengL context
	SDL_GL_DeleteContext(theSDLWND);

	// Destroy the window
	SDL_DestroyWindow(theSDLWND);

	//Quit FONT system
	TTF_Quit();

	// Quit IMG system
	IMG_Quit();

	//Close Joystick
	SDL_JoystickClose(joyStick);

	// Shutdown SDL 2
	SDL_Quit();
}
