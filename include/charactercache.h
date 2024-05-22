#pragma once

#include "sdlpp/render.hpp"
#include "sdlpp/ttf.hpp"
#include <iostream>
#include <map>
#include <stdexcept>

namespace matscreen {

struct CharacterCache {
    int charWidth = 0;
    int charHeight = 0;
    ttf::Font font;
    std::map<uint32_t, sdl::Texture> textures;
    std::string filename;
    int fontSize = 0;

    /// Returns font to be usable in constructor
    ttf::Font loadFont(std::string filename, int size) {
        textures.clear();
        this->filename = filename;
        this->fontSize = size;
        ttf::init();
        return ttf::Font{filename.c_str(), size};
    }

    CharacterCache(const std::string &filename, int fontSize)
        : font{ttf::Font{loadFont(filename.c_str(), fontSize)}}
        , filename{filename}
        , fontSize{fontSize} {
        refreshFontSize();
    }

    void resize(int fontSize) {
        textures.clear();
        font = loadFont(filename, fontSize);
        refreshFontSize();
    }

    uint32_t str2i(std::string_view str) {
        int ret = 0;
        for (size_t i = 0; i < str.size(); ++i) {
            ret = static_cast<uint32_t>(str.at(i)) << 8 * i;
        }
        return ret;
    }

    sdl::TextureView getCharacter(sdl::RendererView renderer,
                                  std::string_view c) {
        auto str = std::string{c.substr(0, 4)};
        auto i = str2i(c);

        if (auto f = textures.find(i); f != textures.end()) {
            return {f->second};
        }

        auto s = font.renderUTF8Blended(str.c_str(), {255, 255, 255, 255});
        return {textures[i] = renderer.createTextureFromSurface(s)};
    }

private:
    void refreshFontSize() {
        if (!font) {
            std::cerr << "could not load font " << filename << "\n";
            std::exit(1);
        }
        {
            auto a = font.renderGlyphBlended('a', {});
            charWidth = a->w;
            charHeight = a->h;
        }
        textures.clear();
    }
};

} // namespace matscreen
