#pragma once

enum class IconType {
	Candy = 0,
	Ghost = 1,
	Mummy = 2,
	Pumpkin = 3,
	Skeleton = 4,
	Spider = 5,
	Witch = 6,
	Zombie = 7
};

//Engine
enum class Scene {
	Game,
	Title
};

enum class MenuName {
	Confirm_Quit,
	GO,
	Main,
	Options,
	Options_I,
	Pause,
};

enum class UIElem {
	NONE,
	//Buttons
	Apply,
	Back,
	Options,
	Play,
	Quit,
	Resume,
	Title,
	//Pickers
	Resolution
};