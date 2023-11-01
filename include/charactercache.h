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

    ttf::Font loadFont(std::string filename, int size) {
        textures.clear();
        this->filename = filename;
        ttf::init();
        return ttf::Font{filename.c_str(), size};
    }

    CharacterCache(const std::string &filename, int fontSize)
        : font{ttf::Font{loadFont(filename.c_str(), fontSize)}}
        , filename{filename} {
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
        //        if (!s) {
        //            // TODO: Handle this better!!
        //            throw std::runtime_error{"failed to create character '" +
        //                                     std::string{c} + "'"};
        //        }
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
    }
};

} // namespace matscreen
