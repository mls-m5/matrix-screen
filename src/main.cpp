
#include "sdlpp/events.hpp"
#include "sdlpp/render.hpp"
#include "sdlpp/ttf.hpp"
#include "sdlpp/window.hpp"
#include <filesystem>
#include <iostream>

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

    ttf::init();

    if (!std::filesystem::exists("data/Ubuntu-Regular.ttf")) {
        std::cerr << "font does not exist on location\n";
    }

    auto f = ttf::Font{"data/Ubuntu-Regular.ttf", 100};

    if (!f) {
        std::cerr << "could not load font\n";
        std::exit(1);
    }

    auto s = f.renderGlyphBlended('b', {255, 255, 255, 255});

    if (!s) {
        std::cerr << "could not create surface from glyph\n";
        std::exit(1);
    }

    auto texture = renderer.createTextureFromSurface(s);

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

        auto rect = sdl::Rect{0, 0, s->w, s->h};

        texture.colorMod(100, 10, 10);
        renderer.copy(texture, rect, rect);

        renderer.present();
    }
}
