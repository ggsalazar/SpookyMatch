#pragma once

enum class GameMode {
	NONE,
	Moves,
	Time
};

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
	Choose_Game,
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
	Moves,
	Options,
	Play,
	Quit,
	Resume,
	Time,
	Title,
	//Pickers
	Resolution,
	Moves_P,
	Time_P
};