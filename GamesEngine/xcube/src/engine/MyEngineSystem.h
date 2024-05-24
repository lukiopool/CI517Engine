#ifndef __MY_ENGINE_H__
#define __MY_ENGINE_H__

#include "EngineCommon.h"
#include <tuple> // a tuple stores multiple values, used to return multiple things in one function



class MyEngineSystem {
	friend class XCube2Engine; // I dont remember what this is for but the original comment just said "conversations"



	

public:
	
	bool InCutscene;
	std::string CurrentScript;
	int CharactersExported; //the amount of characters that have already been looped through in a line of dialogue
	
	bool lineFinished;
	int defaultScrollSpeed;
	int currentScriptLine;
	char currentLineChar;
	bool dialogueorAfter; // checks whether to be reading column 2 or 3, false is dialogue, true is after
	int expectedCharacterReach; // the expected character that should be implemented, increased by one every time the script is looped through
	std::string currentCutsceneLines[99][99];// creates a 2d array for all the lines in the specific script MUST specify std, as compiler gets confused since not everything has it included automatically First value is column, second is row
	
	
	

	void enterCutscene(std::string scriptRef);
	void exitCutscene();
	void newLine();
	void runCommands(std::string commandToRun, int commandStart, int commandEnd, bool whichSection); // runs commands based on the string, sending along the start and end point of the command as well as which column was read from so it may be removed
	bool waitForCommand; // false usually, set to true before a command is read, then set to false after the command


	std::tuple<std::string, int, int, std::string, std::string, int, int, bool> updateText(); //

	std::tuple <std::string, int, int, std::string, std::string, int, int, bool> updateCutsceneInfo;// in order, it is: the text, the X coordinate, the Y Coordinate, the font the player is using, any sfx to play, delay before next trigger, font size, if the line has been fully read or not

	

		std::tuple<std::string, int, int, bool, std::string> updateCutsceneNPC(int characterInQuestion);// a function that passes all the values of cutscenecharacter in question



		class cutsceneNPC // class for cutscene NPCS, have values for their current animation, their intended x and y position, whether they should be animated, and if they are animated, has an array with the animation sprites
		{
		public:
			std::string currentSprite;
			std::string characterName;
			int xpos;
			int ypos;
			bool animated;
			bool loopingAnimation;

			std::string currentAnimation; // when there is an animation occuring, this is the name of the animation
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

			std::string cutsceneName; // the name of the cutscene, used to determine some references



			cutsceneNPC cutsceneCharacters[10];







		};

		std::string imagesToLoad[99];

		conversationStarter CutsceneConvoStarter;


		void loadImages();

	MyEngineSystem(); // construction function
};







#endif