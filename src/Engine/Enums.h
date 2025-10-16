#pragma once

enum class GameMode {
	NONE,
	Infinite,
	Moves,
	Time
};

enum class IconType {
	Bat = 0,
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

enum class Widget {
	NONE,
	//Buttons
	Apply,
	Back,
	Infinite,
	Options,
	Play,
	Quit,
	Resume,
	Title,
	//Pickers
	Resolution,
	Moves_P,
	Music_P,
	Time_P,
	//Sliders
	Msc_Vol
};

//Audio
enum class Song {
	NONE = -1,
	Ghoulish_Fun = 0,
	Spooky_Enchantment = 1,
	Trick_or_Treat = 2
};