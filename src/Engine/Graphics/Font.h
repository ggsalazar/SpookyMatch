#pragma once
#include <filesystem>
#include <SDL3/SDL_render.h>
#include <SDL3_ttf/SDL_ttf.h>

class Font {
public:
  Font() = default;
  Font(const std::string& file, uint size) {
    //WINDOWS/VISUAL STUDIO METHOD
    //std::string path = "assets/Fonts/" + file + ".ttf";
    //LINUX/MESON METHOD
    std::string path = "Projects/SpookyMatch/assets/Fonts/" + file + ".ttf";
    font = TTF_OpenFont(path.c_str(), size);

    if (!font) {
      path = "Projects/SpookyMatch/assets/Fonts/" + file + ".otf";
      font = TTF_OpenFont(path.c_str(), size);
    }

    if (!font)
      std::cout << "Could not load font from file: " << path << "! SDL Error: " << SDL_GetError() << "\n";
  }
  ~Font() {
	  //Likely does not need to be called because of copy construction bullshit
	  //TTF_CloseFont(font);
  }

	inline TTF_Font* GetFont() const { return font; }

private:
	TTF_Font* font = nullptr;
};
