#include "MyEngineSystem.h"
#include <tuple>// a tuple stores multiple values, used to return multiple things in one function
#include <fstream>
#include <iostream>
#include <sstream>



using namespace std;


MyEngineSystem::MyEngineSystem()
{

	
	defaultScrollSpeed = 1;
	currentScriptLine = 0;
	waitForCommand = false;
	InCutscene = false;
	CurrentScript = "blank";
	CharactersExported = 0;
	expectedCharacterReach = 0; // initializing, this is used to determine how far into a block it should be reading
	dialogueorAfter = false; // if false, reading script, otherwise reading commands
	currentLineChar= 0; // current character being read

	updateCutsceneInfo = std::make_tuple("blank", 0, 0, "blank", "blank", 0, 0, false); // declares the update knowledge tuple as having blank variables
	// reminder: in order this is  the text, the X coordinate, the Y Coordinate, the font the player is using, any sfx to play, and the delay for next update.

	lineFinished = false;

	


}



void MyEngineSystem::enterCutscene(std::string scriptRef) {
	
		
	

		std::cout << "In Cutscene" << std::endl;
		InCutscene = true;
		expectedCharacterReach = 0;

		std::cout << "the script reference is " << scriptRef << std::endl;

		CurrentScript = scriptRef;

		//scriptRef = "CSVs/rpgTestConvo.csv"; // just while testing, lets set things to the one csv
		
		ifstream file(CurrentScript);
		if (!file.is_open()) {
			cerr << "Error opening file!" << endl;

		}

		// Define a 2D array to store the CSV data
		
		

		// Define a 2D array to store the CSV data
		
		
		
		string data[99][99];
		string line;
		int row = 0;
		// Store the CSV data from the CSV file to the 2D array
		while (getline(file, line) && row < 98) {
			stringstream ss(line);
			string cell;
			int col = 0;
			while (getline(ss, cell, ',') && col < 3) {
				data[row][col] = cell;

				
				

				if (data[row][col] == "")
				{
					data[row][col] = "blank";
				}

				cout << data[row][col];
				col++;			
			}
			cout << endl;
			row++;
		}
		// close the file after read opeartion is complete
		file.close();

		// Print the data stored in the 2D array
		
		
		for (int i = 0; i < 90; i++) {
			for (int j = 0; j < 15 && !data[i][j].empty();j++) 
			{
				
				currentCutsceneLines[i][j] = data[i][j];
				
			

				//cout << currentCutsceneLines[i][j] << " ";
			}
			cout << std::endl;
		//	cout << endl;
		}

	//	cout << "escaped loop" << endl;

		
		//	std::cout << "not in cutscene" << std::endl;

		
		
		
		return;
	
}

std::tuple<std::string, int, int, std::string, std::string, int, int, bool> MyEngineSystem::updateText() // in order, it is: the text, the X coordinate, the Y Coordinate, the font the player is using, any sfx to play, the gibbering (aka, a sound effect played after every letter)
{

	

	std::string textToReturn;
	//std::string fontRef;
	std::string SFXToPlay;
	std::string typeFont;
	int textStartXCoord;
	int textStartYCoord;
	int scrollSpeed;
	int fontSize = 1;

	textStartYCoord = 150;
	textStartXCoord = 150;
	scrollSpeed = 0; // initialize the scroll speed
	typeFont = "res/fonts/COMICSANS.TTF"; // setting the font reference atm


	

		

		string textToDisplay;
		string currentLineText;
		bool commandIncoming; // set to true when a { is detected, then saves text to "textincommand" to compare for whatever function needs outputting
		string textInCommand; // text in a command line when applicable
		float currentCharacterReach; // the amount of characters that have been looped through
		bool lineComplete; // set each loop of the line to true, anything that causes a return sets false, if its still true, line is complete

		float commandStart; // the start in the string where the command was
		float commandEnd; // the end of the string where the command was

		commandIncoming = false; 

		currentCharacterReach = 0;

		

	//cout << dialogueorAfter << endl;
		if (!dialogueorAfter)// if false, reading dialogue, if true, reading post dialogue commands
		{
			currentLineText = currentCutsceneLines[currentScriptLine][1];

		//	cout << "reading dialogue" << endl;
		}
		else
		{
			currentLineText = currentCutsceneLines[currentScriptLine][2];
			//cout << "reading commands" << endl;
		}
		
		// congrats! now that you have the text of the csv (ONLY TOOK YOU AN ENTIRE F****ING DAY, you can now loop through it to trigger functions :3

		
		
		lineComplete = true;

		for (std::string::iterator it = currentLineText.begin(); it != currentLineText.end(); ++it) { // loops through characters in a string. Note that any functions triggered in dialogue will trigger EVERY FRAME unless said text is removed from the "currentcutscenelines" respective variable. if the conversation were to be reloaded, then it would return, as it does not edit the csv

			currentCharacterReach += 1;

			//cout << currentLineChar << endl;

			lineComplete = true;
			currentLineChar = *it; // sets currentlinechar to the current character

			if (commandIncoming) // if the boolean command incoming is true, then add characters to text in command until } is reached, then compare to requirements
			{

				if (currentLineChar != '}') // if the line is not the end of the command
				{

					if (!waitForCommand)
					{
						textInCommand = textInCommand + *it;//write down the character in the command
					}
					else
					{
						currentCharacterReach -= 1;
					}
					
					
					

				}
				else //
				{

					if (!waitForCommand)
					{

						waitForCommand = true;
						commandEnd = currentCharacterReach; // sets the end value of the command, this is used to remove the command to prevent repeats
						runCommands(textInCommand, commandStart, commandEnd, dialogueorAfter); // passes the command to the comparer function, as well as the position of the command so it can be removed afterwards
						currentCharacterReach = 0;
						textInCommand = "";
					}
					else
					{
						currentCharacterReach -= 1;
					}
				
					

					

				}
				
				


			}
			else // if commands are not being read this loop, add the dialogue to 
			{

				

				if ( currentLineChar == '{' )
				{

					commandIncoming = true;
					commandStart = currentCharacterReach;

				}
				else
				{
					commandIncoming = false;

					textToDisplay += *it; // adds the current character to the text
					

					if (expectedCharacterReach <=  currentCharacterReach)
					{
						expectedCharacterReach = currentCharacterReach + 1;
						lineComplete = false;
						//cout << textToDisplay << endl;
						
						break;
					}
					else // if it is the end of the line, do something (here mostly for testing)
					{
						
					}
				}
				
				
			}
		}

		if (lineComplete) // if the line is done reading
		{
			if (!dialogueorAfter)
			{
				lineFinished = true; // if reading dialogue, set linefinished to true, then wait for player input to read next line of dialogue
			}
			else//if reading post dialogue, just go ahead and grab the new line
			{
				MyEngineSystem::newLine();
			}
				
			



		}


		if (scrollSpeed == 0)
		{
			scrollSpeed = defaultScrollSpeed;
		}
		


		textToReturn = textToDisplay;
		get<0>(updateCutsceneInfo) = textToReturn; // gets the word value in the tuple and sets it
		get<1>(updateCutsceneInfo) = textStartXCoord; // gets the x value for the text and sets it
		get<2>(updateCutsceneInfo) = textStartYCoord; // gets the Y value for the text and sets it
		get<3>(updateCutsceneInfo) = typeFont; // gets the font for the line and sets it
		get<4>(updateCutsceneInfo) = SFXToPlay; // gets any required sfx and sets it
		get<5>(updateCutsceneInfo) = scrollSpeed; // sets the scroll speed for next tick
		get<6>(updateCutsceneInfo) = fontSize; // sets the font size
		get<7>(updateCutsceneInfo) = lineFinished;
		
	
		
		
	return updateCutsceneInfo;

}



void MyEngineSystem::exitCutscene()
{

	
	updateCutsceneInfo = std::make_tuple("blank", 0, 0, "blank", "blank", 0, 0, false);
	InCutscene = false;

	return;
}

void MyEngineSystem::newLine() // triggers when a new line is being loaded, whether that be dialogue line or post dialogue command line
{
	expectedCharacterReach = 0;
	
	dialogueorAfter = !dialogueorAfter;// tells the main loop to read the other column to what it is currently reading
	cout << "flipping lines" << endl;

}

void MyEngineSystem::runCommands(std::string commandToRun, int commandStart, int commandEnd, bool whichSection)
{


	if (commandToRun.substr(commandStart-1, commandEnd-commandStart).find("readline_") != string::npos)//if the string has the quote "readline" in it, then do this. Note that .find will return "npos" if there is no valid answer, so if it does NOT have this quote in it, it will not trigger
	{
		cout << "this has readline in it" << endl;
	
		int newLine;

		cout << commandToRun.substr(9, 999) << endl;
	
		newLine = stoi(commandToRun.substr(9, 999));

		// ----------------- erases the section, should be repeated for basically every if statement, but position varies
		if (!whichSection)// if false, reading dialogue, if true, reading post dialogue commands
		{
			
			cout << "deleting the column 1 line " + currentCutsceneLines[currentScriptLine][1].substr(commandStart-1, commandEnd) << endl;
			currentCutsceneLines[currentScriptLine][1].erase(commandStart-1, commandEnd); // erases the part of the string from command start to command end

		//	cout << "new line is " + currentCutsceneLines[currentScriptLine][1] << endl;
			
		}
		else
		{
			cout << "deleting the column 2 line " + currentCutsceneLines[currentScriptLine][2].substr(commandStart-1, commandEnd) << endl;
			currentCutsceneLines[currentScriptLine][2].erase(commandStart-1, commandEnd); // erases the part of the string from command start to command end, note the -1 and +1, there to include the {} that are otherwise ignored
			cout << "not crashed yet" << endl;

		//	cout << "new line is " + currentCutsceneLines[currentScriptLine][2] << endl; // posts the new line, should be with the bracket completely removed
		

		}
		// with the command removed from the main array, lets complete it
		
		cout << "this is the line when it sets the script line" << endl;
		currentScriptLine = newLine; // sets the current script line to the read value. stoi is used to convert the value from string to int, substr creates a substring from index 10 (just after the _) to the end, with a maximum value of 999 lines in a script
	

		waitForCommand = false;
	}
	else if (commandToRun.substr(commandStart-1, commandEnd+1 - commandStart).find("endConversation") != string::npos)// if the command is to end the conversation
	{

		for (int i = 0; i < 10; i++)
		{
			CutsceneConvoStarter.cutsceneCharacters->xpos = -500;
		}
		
		InCutscene = false;
		currentScriptLine =0; // sets the current script line to the start
		waitForCommand = false;

	}
	else if (commandToRun.substr(commandStart, commandEnd - commandStart).find("wait") != string::npos)
	{

		if (whichSection)// if false, reading dialogue, if true, reading post dialogue commands
		{


			currentCutsceneLines[currentScriptLine][1].erase(commandStart - 1, (commandEnd + 1 - commandStart)); // erases the part of the string from command start to command end

		//	cout << "new line is " + currentCutsceneLines[currentScriptLine][1] << endl;
		
		}
		else
		{
			currentCutsceneLines[currentScriptLine][2].erase(commandStart - 1, (commandEnd + 1 - commandStart)); // erases the part of the string from command start to command end, note the -1 and +1, there to include the {} that are otherwise ignored

		//	cout << "new line is " + currentCutsceneLines[currentScriptLine][2] << endl; // posts the new line, should be with the bracket completely removed

		}

		// do wait stuff
		
		
		waitForCommand = false;

	}
	else if (commandToRun.substr(commandStart-1, commandEnd+1 - commandStart).find("do_") != string::npos)
	{

		cout << "this has do in it 1.0" << endl;
	
		cout << commandToRun.substr(commandStart, commandEnd - commandStart) << endl;

		std::string characterTarget;
		int secondUnderscore;
		std::string animationTarget;
		

		secondUnderscore = commandToRun.find("_", 3);


		for (int i = 3; i < secondUnderscore; i++)// determines character name
		{
			//cout << i << endl;
			characterTarget = characterTarget + commandToRun[i];
		}

		for (int h = secondUnderscore+1; h < commandEnd-1; h++)
		{
			animationTarget = animationTarget + commandToRun[h];// figures out the name of the desired sprite
		}

		for (int e = 0; e < 10; e++)
		{
			
			if (CutsceneConvoStarter.cutsceneCharacters->characterName == characterTarget)
			{
				CutsceneConvoStarter.cutsceneCharacters->currentSprite = animationTarget;
				cout << "The new sprite for "+ CutsceneConvoStarter.cutsceneCharacters->characterName + " is " + animationTarget << endl;
			}

		}
		//cout << characterTarget << endl;



		// once sorted, remove command

		//cout << commandStart << endl;
		//cout << commandEnd << endl;

		if (!whichSection)// if false, reading dialogue, if true, reading post dialogue commands
		{

			cout << "old line is " + currentCutsceneLines[currentScriptLine][1] << endl;
			currentCutsceneLines[currentScriptLine][1].erase(commandStart-1, (commandEnd + 1 - commandStart)); // erases the part of the string from command start to command end

			cout << "new line is " + currentCutsceneLines[currentScriptLine][1] << endl;
			cout << "this is a line in the speech section" << endl;
		}
		else
		{
			cout << "old line is " + currentCutsceneLines[currentScriptLine][2] << endl;
			currentCutsceneLines[currentScriptLine][2].erase(commandStart-1 , (commandEnd + 1 - commandStart)); // erases the part of the string from command start to command end, note the -1 and +1, there to include the {} that are otherwise ignored

			cout << "new line is " + currentCutsceneLines[currentScriptLine][2] << endl; // posts the new line, should be with the bracket completely removed
			
		}

		
		

	}
	else if (commandToRun.find("do_") != string::npos) // should be making sure its within bounds, fix later
	{

		cout << "this has do in it 2.0" << endl;

		cout << commandToRun.substr(commandStart-2, commandEnd+2 - commandStart) << endl;

		std::string characterTarget;
		int secondUnderscore;
		std::string animationTarget;


		secondUnderscore = commandToRun.find("_", 3);
		


		for (int i = 3; i < secondUnderscore+1; i++)// determines character name
		{
			//cout << i << endl;
			cout << characterTarget << endl;
			characterTarget = characterTarget + commandToRun[i];
		}

		for (int h = secondUnderscore + 1; h < commandEnd - 2; h++)
		{
			
			animationTarget = animationTarget + commandToRun[h];// figures out the name of the desired sprite
			cout << animationTarget << endl;
		}

		for (int e = 0; e < 10; e++)
		{

			cout << "doing for loop" << endl;

			cout << CutsceneConvoStarter.cutsceneCharacters[e].characterName.find(characterTarget) << endl;
			cout << CutsceneConvoStarter.cutsceneCharacters[e].characterName << endl;

			if (CutsceneConvoStarter.cutsceneCharacters[e].characterName == "Tom")
			{
				CutsceneConvoStarter.cutsceneCharacters->currentSprite = animationTarget;
				cout << "The new sprite for " + CutsceneConvoStarter.cutsceneCharacters->characterName + " is " + animationTarget << endl;
				break;
			}

		}
		//cout << characterTarget << endl;



		// once sorted, remove command

		//cout << commandStart << endl;
		//cout << commandEnd << endl;
		cout << "left the loops" << endl;

		if (!whichSection)// if false, reading dialogue, if true, reading post dialogue commands
		{

			cout << "old line is " + currentCutsceneLines[currentScriptLine][1] << endl;
			currentCutsceneLines[currentScriptLine][1].erase(commandStart-2, (commandEnd + 2 - commandStart)); // erases the part of the string from command start to command end

			cout << "new line is " + currentCutsceneLines[currentScriptLine][1] << endl;
			cout << "this is a line in the speech section" << endl;
		}
		else
		{
			cout << "old line is " + currentCutsceneLines[currentScriptLine][2] << endl;
			currentCutsceneLines[currentScriptLine][2].erase(commandStart-2, (commandEnd + 2 - commandStart)); // erases the part of the string from command start to command end, note the -1 and +1, there to include the {} that are otherwise ignored

			cout << "new line is " + currentCutsceneLines[currentScriptLine][2] << endl; // posts the new line, should be with the bracket completely removed

		}




		}
	else
	{
		cout << "no command found" << endl;
		cout << commandToRun << endl;
		cout << "the substring is " + commandToRun.substr(commandStart-1, commandEnd+1 - commandStart) << endl;
		cout << "the substring without starting deduction is " + commandToRun.substr(commandStart-2 , commandEnd + 2 - commandStart) << endl;
		

	}

		
	waitForCommand = false;
	
}

std::tuple<std::string, int, int, bool, string> updateCutsceneNPC(int characterInQuestion)
{


	std::tuple<std::string, int, int, bool, string> characterReferences;


//	get<0>(characterReferences) = ;


	return characterReferences;

}