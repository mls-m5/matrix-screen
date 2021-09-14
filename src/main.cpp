
#include "sdlpp/events.hpp"
#include "sdlpp/render.hpp"
#include "sdlpp/ttf.hpp"
#include "sdlpp/window.hpp"
#include <cstring>
#include <filesystem>
#include <iostream>
#include <map>

namespace {

struct Screen {
    int width = 20;
    int height = 20;
    int charWidth = 0;
    int charHeight = 0;
    ttf::Font font;

    Screen()
        : font{ttf::Font{"data/UbuntuMono-Regular.ttf", 20}} {

        ttf::init();

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

    std::map<uint32_t, sdl::Texture> textures;

    sdl::Texture *getCharacter(sdl::Renderer &renderer, std::string_view c) {
        auto str = std::string{c.substr(0, 4)};
        auto i = uint32_t{0};
        std::memcpy(&i, str.data(), str.size());

        if (auto f = textures.find(i); f != textures.end()) {
            return &f->second;
        }

        auto s = font.renderUTF8Blended(str.c_str(), {255, 255, 255, 255});
        return &(textures[i] = renderer.createTextureFromSurface(s));
    }
};

} // namespace

int main(int argc, char **argv) {
    auto window = sdl::Window{"matrix view",
                              SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED,
                              300,
                              300,
                              SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE};

    if (!window) {
        std::cerr << "could not create sdl window\n";
        std::exit(1);
    }

    auto renderer = sdl::Renderer{
        window, SDL_RENDERER_ACCELERATED, SDL_RENDERER_PRESENTVSYNC};

    if (!renderer) {
        std::cerr << "could not create sdl renderer\n";
        std::exit(1);
    }

    auto f = ttf::Font{"data/UbuntuMono-Regular.ttf", 20};

    if (!f) {
        std::cerr << "could not load font\n";
        std::exit(1);
    }

    auto screen = Screen{};

    for (auto event = sdl::waitEvent(); event.type != SDL_QUIT;
         event = sdl::waitEvent()) {
        std::cerr << event.type << ":" << event.window.event << " event\n";

        switch (event.type) {
        case SDL_WINDOWEVENT:
            if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                renderer.drawColor(100, 100, 100, 100);
                renderer.drawLine(0, 0, event.window.data1, event.window.data2);
                renderer.present();
            }
            break;
        }

        renderer.drawColor(30, 30, 30, 255);
        renderer.fillRect();

        {
            auto &texture = screen.textures['a'];
            auto rect = sdl::Rect{0, 0, screen.charWidth, screen.charHeight};

            renderer.drawColor(100, 0, 0);
            renderer.fillRect(rect);
            texture.colorMod(0, 10, 10);
            renderer.copy(texture, rect, rect);
        }
        {
            //            auto &texture = screen.textures['x'];
            auto texture = screen.getCharacter(renderer, "ö");
            auto rect = sdl::Rect{
                0, screen.charHeight, screen.charWidth, screen.charHeight};
            renderer.fillRect(rect);
            texture->colorMod(255, 255, 255);
            renderer.copy(*texture, {}, &rect);
        }

        renderer.present();
    }
}
