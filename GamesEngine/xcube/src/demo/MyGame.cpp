#include "MyGame.h" // this handles object interactions, as well as pc movement 
#include "../engine/MyEngineSystem.h" 
#include <tuple> // a tuple stores multiple values, used to return multiple things in one function
#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>
#include <sstream>

#include <Windows.h>
#include "SDL_image.h"



#include <cstdio> // added during testing

// create an img path for every image


using namespace std;



MyGame::MyGame() : AbstractGame(), score(0), lives(3), numKeys(5), gameWon(false), box(5, 5, 30, 30) {
	TTF_Font * font = ResourceManager::loadFont("res/fonts/COMICSANS.TTF", 14);
	gfx->useFont(font);
	gfx->setVerticalSync(true);
	//gfx->createTextureFromString("sprites/RotatingRectangleSheet.png", BACKGROUND_BLUE);

	

	timeSinceLastInteract1 = 0;
	timeSinceLastInteract2 = 0;
	timeSinceLastInteract3 = 0;
	timeSinceLastUpdate = 0;
	textScrollSpeed = 0; // counts down while text is happening

	updateCutsceneInfo = std::make_tuple("blank", 0, 0, "blank", "blank", 0, 0, false);// in order, it is: the text, the X coordinate, the Y Coordinate, the font the player is using, any sfx to play, delay before next trigger and the font size
	
	createConversationStarter(0, "CSVs/rpgTestConvo.csv", 5, 5, "sprites/something", "testCutscene"); // dont forget to create a sprite for the cutscene starter
	currentConversationIndividual = allConversationStarters[0]; // sets the current conversation starter to be the first one for testing purposes, later on want to have it be set when the player pressed interact near the interactibles position

	
	loadImages();
	

}

MyGame::~MyGame() {

}

void MyGame::loadImages()
{




	gfx->loadImages();

}

void MyGame::handleKeyEvents() {
	
	// handles inputs,

	//WASD is movement

	//ESCAPE currently enters a cutscene

	if (eventSystem->isPressed(Key::UP)) // default input key is W
	{
		velocity.y = velocity.y -speed;
	}

	if (eventSystem->isPressed(Key::DOWN))// default input key is S
	{
		velocity.y = velocity.y + speed;
	}

	if (eventSystem->isPressed(Key::LEFT)) // default input key is A
	{
		velocity.x = velocity .x -speed;
	}

	if (eventSystem->isPressed(Key::RIGHT))// default input key is D
	{
		velocity.x = velocity.x +speed;
	}


	if (eventSystem->isPressed(Key::INTERACT1))// default input key is E
	{

		if (timeSinceLastInteract1 > 100)// button was tapped
		{

			timeSinceLastInteract1 = 0;
		//	std::cout << "interact pressed" << std::endl;

			if (!textEngine.InCutscene)
			{
				cutsceneStart(); // starts cutscene (later on will appear in nearness check area)
			}
			
			





		}else if (timeSinceLastInteract1 > 50) // button is being held 
		{
			timeSinceLastInteract1 = 0;
		//	std::cout << "interact held" << std::endl;
		}
				
	}
	


	if (eventSystem->isPressed(Key::INTERACT2))// default input key is F
	{
		
		if (timeSinceLastInteract2 > 100)// button was tapped
		{

			timeSinceLastInteract2 = 0;
		//	std::cout << "secondary interact pressed" << std::endl;

			if (std::get<7>(updateCutsceneInfo)) // the button to forward dialogue, if it needs forwarding
			{
				std::get<7>(updateCutsceneInfo) = false;
				textEngine.newLine();
			//	cout << "playing new line";

				 // once its played the newline function, switch linefinished to false

			}

		}
		else if (timeSinceLastInteract2 > 50) // button is being held
		{
			timeSinceLastInteract2 = 0;
		//	std::cout << "secondary interact held" << std::endl;
		}


		
	}

	if (eventSystem->isPressed(Key::INTERACT3))// default input key is G
	{

		if (timeSinceLastInteract3 > 100)// button was tapped
		{

			timeSinceLastInteract3 = 0;
		//	std::cout << "tertiary interact pressed" << std::endl;

		}
		else if (timeSinceLastInteract3 > 50) // button is being held
		{
			timeSinceLastInteract3 = 0;
		//	std::cout << "tertiary interact held" << std::endl;
		}
	
	}
}

void MyGame::update() {
	

	
	
		

	for (auto key : gameKeys) {
		if (key->isAlive && box.contains(key->pos)) {
			score += 200;
			key->isAlive = false;
			numKeys--;
		}
		
	} // keeping this as a basic reminder for how to figure if the player is near something     

	if (!textEngine.InCutscene)
	{

		box.x += velocity.x;
			box.y += velocity.y;

		if (velocity.x < 0)
			{
				velocity.x = velocity.x + friction;
				if (velocity.x > 0)
				{
					velocity.x = 0;
				}
			}
			else
			{
				velocity.x = velocity.x - friction;
				if (velocity.x < 0)
				{
					velocity.x = 0;
				}
			}

			if (velocity.x > maxSpeed)
			{
				velocity.x = maxSpeed;
			}

			if (velocity.x < -maxSpeed)
			{
				velocity.x = -maxSpeed;
			}
	
			if (velocity.y < 0)
			{
				velocity.y = velocity.y + friction;
				if (velocity.y > 0)
				{
					velocity.y = 0;
				}
			}
			else
			{
				velocity.y = velocity.y - friction;
				if (velocity.y < 0)
				{
					velocity.y = 0;
				}
			}
	
			if (velocity.y > maxSpeed)
			{
				velocity.y = maxSpeed;
			}

			if (velocity.y < -maxSpeed)
			{
				velocity.y = -maxSpeed;
			}
	}
	else
	{
		velocity.x = 0;
		velocity.y = 0;
	}
	

	
	// increments the cooldown times for things
	timeSinceLastInteract1 = timeSinceLastInteract1 + SDL_GetTicks() - timeSinceLastUpdate;
	timeSinceLastInteract2 = timeSinceLastInteract2 + SDL_GetTicks() - timeSinceLastUpdate;
	timeSinceLastInteract3 = timeSinceLastInteract3 + SDL_GetTicks() - timeSinceLastUpdate;


	// updates the number to compare with
	timeSinceLastUpdate = SDL_GetTicks();

}

void MyGame::render() {
	

	gfx->clearScreen();// clears all renders in gfx



	if (!textEngine.InCutscene)// if not in a cutscene, render objects that should be rendered
	{

		// render the player character
				gfx->setDrawColor(SDL_COLOR_RED);
				gfx->drawRect(box);
				gfx->setDrawColor(SDL_COLOR_YELLOW);

	}
	else
	{ // render objects that are cutscene exclusive (aka, the current actors of the scene)

		

		
		for (cutsceneNPC NPC : currentConversationIndividual.cutsceneCharacters)// loops through each cutsceneNPC in the current scene
		{

			


		}


	}

	if (textEngine.InCutscene)
	{
		for (int i = 0; i < 9; i++)// go through each npc in the current conversation, and make sure they are identical to their engine system counterpart
		{

			//gfx->loadImages();
			updateNPCs(true);
			

		}

	}

	// then render things that should be rendered regardless of cutscene state



	// old code for rendering pickups, keeping for now in case I want to double check it
	for (auto key : gameKeys)
				if (key->isAlive)
					gfx->drawCircle(key->pos, 5);
}

void MyGame::renderUI() {

	/*
	gfx->setDrawColor(SDL_COLOR_AQUA);
	std::string scoreStr = std::to_string(score);
	gfx->drawText(scoreStr, 780 - scoreStr.length() * 50, 25);

	
		gfx->drawText("YOU WON", 250, 500);


		old draw ui indicator

	*/
	
	
	updateSprites();

	if (textEngine.InCutscene)
	{
		
		

		

		if (textScrollSpeed <= 0)//implements a delay of ticks before repeating the update
		{
		
					

				updateCutsceneInfo = textEngine.updateText();
				
				//ideally would have the changed font/ fontsize right here later.
				//TTF_Font* font = ResourceManager::loadFont(std::get<3>(updateCutsceneInfo), std::get<6>(updateCutsceneInfo));// creates a font from reference point 3, of size from reference point 6

				if (std::get<0>(updateCutsceneInfo) != "")//checks to make sure the string isnt empty
				{
					gfx->drawText(std::get<0>(updateCutsceneInfo), std::get<1>(updateCutsceneInfo), std::get<2>(updateCutsceneInfo));// types text with string value 0, at coords 1 and 2
				}
				

				textScrollSpeed = std::get<5>(updateCutsceneInfo);// sets scroll cd to section 5

					
		}
		else
		{

			gfx->drawText(std::get<0>(updateCutsceneInfo), std::get<1>(updateCutsceneInfo), std::get<2>(updateCutsceneInfo));
			textScrollSpeed -= 1;
		}

		

		
		

	}

	
//	gfx->loadImages();
	gfx->renderImage();

	//gfx->renderPresent();


}

void MyGame::createConversationStarter(int position, string cutsceneRef, int Xpos, int yPos, string cutsceneStarterSprite, string cutsceneName)
{
	// sets the position reference for the starter in the array, where it is in worldspace, the sprite to represent it in worldspace, and the name of the cutscene (Mostly used so we can hardcode the NPCs in a cutscene.
	allConversationStarters[position].ScriptRef = cutsceneRef;
	allConversationStarters[position].xpos = Xpos;
	allConversationStarters[position].ypos= yPos;
	allConversationStarters[position].cutsceneStarterSprite = cutsceneStarterSprite;
	allConversationStarters[position].cutsceneName = cutsceneName;

	// creates the NPCs inside of the conversation starter.

	if (allConversationStarters[position].cutsceneName == "testCutscene")
	{
		// sets the first two characters names and current sprites to just be their names. animation and alternative sprites will use their name, then append bits to it to figure out the required sprite to draw
		allConversationStarters[position].cutsceneCharacters[0].characterName = "Tom";
		allConversationStarters[position].cutsceneCharacters[0].xpos = 255;
		allConversationStarters[position].cutsceneCharacters[0].ypos =255;
		allConversationStarters[position].cutsceneCharacters[0].currentSprite = "Tom";


		allConversationStarters[position].cutsceneCharacters[1].characterName = "Jeff";
		
		allConversationStarters[position].cutsceneCharacters[1].currentSprite = "Jeff";

	}

	

}



void MyGame::cutsceneStart()
{



	textEngine.enterCutscene(currentConversationIndividual.ScriptRef);
	updateNPCs(false);// write the NPCs for the text engine
	updateSprites();


}

void MyGame::updateNPCs(bool readOrWrite)
{


	if (readOrWrite) // if true, is reading, if false, is writing. Writing should trigger when the cutscene starts, reading should trigger far more often
	{

		// goes through the 10 potential cutscene characters, and makes the main system have the same values. 

		for (int f = 0; f < 10; f++)// for each cutscene character in mygame's cutscenecharacter list
		{
			
			currentConversationIndividual.cutsceneCharacters[f].currentSprite = textEngine.CutsceneConvoStarter.cutsceneCharacters[f].currentSprite;
			currentConversationIndividual.cutsceneCharacters[f].characterName = textEngine.CutsceneConvoStarter.cutsceneCharacters[f].characterName;
			currentConversationIndividual.cutsceneCharacters[f].xpos = textEngine.CutsceneConvoStarter.cutsceneCharacters[f].xpos;
			currentConversationIndividual.cutsceneCharacters[f].ypos = textEngine.CutsceneConvoStarter.cutsceneCharacters[f].ypos;
			currentConversationIndividual.cutsceneCharacters[f].animated = textEngine.CutsceneConvoStarter.cutsceneCharacters[f].animated;
			  
			 
		}

	}
	else
	{
		for (int i =0; i < 10; i++)// for each cutscene character in mygame's cutscenecharacter list
		{
			
			// goes through the 10 potential cutscene characters, and makes the engine system have the same values. 
			textEngine.CutsceneConvoStarter.cutsceneCharacters[i].currentSprite = currentConversationIndividual.cutsceneCharacters[i].currentSprite;
			textEngine.CutsceneConvoStarter.cutsceneCharacters[i].characterName = currentConversationIndividual.cutsceneCharacters[i].characterName;
			textEngine.CutsceneConvoStarter.cutsceneCharacters[i].xpos= currentConversationIndividual.cutsceneCharacters[i].xpos;
			textEngine.CutsceneConvoStarter.cutsceneCharacters[i].ypos = currentConversationIndividual.cutsceneCharacters[i].ypos;
			textEngine.CutsceneConvoStarter.cutsceneCharacters[i].animated = currentConversationIndividual.cutsceneCharacters[i].animated;
		
		}

	}
	

}

void MyGame::updateSprites()
{

	

		for (int i = 0; i < 70; i++) // loop through all images, and place them far out the view of the player
		{
			gfx->setImageLocation(i, -500, -500);
		}

		if (textEngine.InCutscene)
		{
			for (int i = 0; i < 70; i++) // was going to have this loop through using the length of the array its looping through as a reference but couldnt find a way to
					{

						for (int e = 0; e < 10; e++)// for each of the characters in the cutscene
						{

							if (currentConversationIndividual.cutsceneCharacters[e].currentSprite.find(gfx->imageName[i]) != string::npos)// if the names are the same
							{



								if (gfx->imageName[i] != "")
								{
									//cout << "setting " + currentConversationIndividual.cutsceneCharacters[e].characterName + " as x coord " + std::to_string(currentConversationIndividual.cutsceneCharacters[e].xpos) + " and y coord " + std::to_string(currentConversationIndividual.cutsceneCharacters[e].ypos) + " this is location " + to_string(i) << endl;
									gfx->setImageLocation(i, currentConversationIndividual.cutsceneCharacters[e].xpos, currentConversationIndividual.cutsceneCharacters[e].ypos);
								}



							}

						}




					}
		}
		

}