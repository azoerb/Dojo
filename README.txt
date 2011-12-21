///////////////////////////////////////////////////////////////////////////////
// CS 679 - COMPUTER GAME TECHNOLOGY PROJECT 3
///////////////////////////////////////////////////////////////////////////////

Group 7 - Dojo Hero

Josh Slauson
Zachary Ovanin
Andrew Zoerb
Samuel Wasmundt
Nick Pjevach

///////////////////////////////////////////////////////////////////////////////

How To Play:
	- Wait for images to load
	- Read instructions
	- Hit spacebar to continue
	- Choose your challenge
	
///////////////////////////////////////////////////////////////////////////////

Credit:
	Sounds:
	Images:

///////////////////////////////////////////////////////////////////////////////

Source Code:

Action.h/Action.cpp
	Class for basic actions.
Animation.h/Animation.cpp
	Class to draw animation frame by frame.
Character.h
	Parent class of Enemy/Player.
ColumnObject.h
	Parent class of Goal/Target.
ComboAction.h/ComboAction.cpp
	Class for combo actions.
Constants.h
	Contains all constants used throughout code.
Controller.h/Controller.cpp
	This is where most stuff happens. It runs in a loop displaying actions and getting user input.
Enemy.h/Enemy.cpp
	Class for enemy.
GameMap.h
	Unused class for our game map idea.
Goal.h/Goal.cpp
	Class for goals on the bottom of the screen.
Location.h
	Unused class relating to game map idea.
Player.h/Player.cpp
	Class for player.
SoundManager.h/SoundManager.cpp
	Class for managing sound effects and music.
Target.h/Target.cpp
	Class for each individual target that must be hit.
TargetSet.h/TargetSet.cpp
	Class for a set of multiple targets that result in an action.
Technique.h/Technique.cpp
	Unused class relating to shop idea.
TechShop.h/TechShop.cpp
	Unused class for our shop idea.