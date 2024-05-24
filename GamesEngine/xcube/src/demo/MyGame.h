#ifndef __TEST_GAME_H__
#define __TEST_GAME_H__

#include "../engine/AbstractGame.h"
#include "../engine/MyEngineSystem.h" 
#include <tuple>
#include <iostream>
#include <fstream>

#include <list>

#include <cstdio> // added during testing
#include "SDL_image.h"



using namespace std;



struct GameKey {
	Point2 pos;
	bool isAlive;
};

class MyGame : public AbstractGame {
	private:
		Rect box;

		Vector2i velocity;

		std::vector<std::shared_ptr<GameKey>> gameKeys;
		std::string test;

		/* GAMEPLAY */
		int score, numKeys, lives;
		bool gameWon;
		float timeSinceLastInteract1; // these three handle the cooldown for interactions, saving how many ms have occured since the button was last pressed
		float timeSinceLastInteract2;
		float timeSinceLastInteract3;
		float timeSinceLastUpdate; // saves what the programs current gameticks WAS last update
		int textScrollSpeed;
		

		void handleKeyEvents();
		void update();
		void render();
		void renderUI();
		void createConversationStarter(int arrayPosition, string cutsceneRef, int Xpos, int yPos, string cutsceneStarterSprite, string cutsceneName);
		void cutsceneStart();
	
	
		 

	public:
        MyGame();
		~MyGame();

		class cutsceneNPC // class for cutscene NPCS, have values for their current animation, their intended x and y position, whether they should be animated, and if they are animated, has an array with the animation sprites
		{
		public: 
			string currentSprite;
			string characterName;
			int xpos;
			int ypos;
			bool animated;
			bool loopingAnimation;

			string currentAnimation; // when there is an animation occuring, this is the name of the animation
			int animationcurrentFrame; // appended to the current animation name to get the current sprite
			int animationLoopTime; // the frame to reach before setting current frame back to 0

		};

		


		class conversationStarter // a class created in the function "create conversation starter" that then holds all the npc info
		{


			public: 
			std::string cutsceneStarterSprite; // the sprite that the cutscene starter itself uses
			std::string ScriptRef; // a string reference to the csv with the script in it
		
			int xpos; // the x position of the conversation starter
			int ypos; // the y position of the conversation starter

			std::string cutsceneName; // the name of the cutscene, used to determine 
			
			
			
			cutsceneNPC cutsceneCharacters[10];

	

			
			


		};

		
		float speed = 3;
		float maxSpeed = 5;
		float friction = 1;
		MyEngineSystem textEngine;
		conversationStarter currentConversationIndividual;
		conversationStarter allConversationStarters[15]; // a list of all conversation starters
		std::tuple <std::string, int, int, std::string, std::string, int, int, bool> updateCutsceneInfo;

		std::string imageRefs[99];

		void loadImages();
		void updateSprites();
		void updateNPCs(bool readOrWrite);// called before drawing cutscene NPCs, sets their required values to be identical to the one in the engine system
		
};



#endif