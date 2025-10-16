#pragma once
#include <filesystem>
#include <SDL3_ttf/SDL_ttf.h>

class Font {
public:
  Font() = default;
  Font(const std::string& file, uint size) {
    std::string path = "../assets/Fonts/" + file;
    font = TTF_OpenFont((path + ".ttf").c_str(), size);

    if (!font) font = TTF_OpenFont((path + ".otf").c_str(), size);

    if (!font)
      std::cout << "Could not load font from file: " << path << "! SDL Error: " << SDL_GetError() << '\n';
  }
  ~Font() {
	  //May not need to be called because of copy construction bullshit,
      //But the more likely cause is Engine calls TTF_Quit() before everything else is destroyed, causing this to segfault
	  //TTF_CloseFont(font);
  }

  [[nodiscard]] inline TTF_Font* GetFont() const { return font; }

private:
	TTF_Font* font = nullptr;
};