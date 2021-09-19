#pragma once

#include "sdlpp/render.hpp"
#include "sdlpp/ttf.hpp"
#include <iostream>
#include <map>

namespace matscreen {

struct CharacterCache {
    int charWidth = 0;
    int charHeight = 0;
    ttf::Font font;
    std::map<uint32_t, sdl::Texture> textures;

    ttf::Font loadFont(const char *filename, int size) {
        ttf::init();
        return ttf::Font{filename, size};
    }

    CharacterCache()
        : font{ttf::Font{loadFont("data/UbuntuMono-Regular.ttf", 20)}} {

        if (!font) {
            std::cerr << "could not load font\n";
            std::exit(1);
        }
        {
            auto a = font.renderGlyphBlended('a', {});
            charWidth = a->w;
            charHeight = a->h;
        }
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
};

} // namespace matscreen
