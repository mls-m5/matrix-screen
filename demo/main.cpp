
#include "canvas.h"
#include "matrixscreen.h"
#include "sdlpp/events.hpp"
#include "sdlpp/render.hpp"
#include "sdlpp/ttf.hpp"
#include "sdlpp/window.hpp"
#include <cstring>
#include <filesystem>
#include <iostream>
#include <map>
#include <vector>

using namespace matscreen;

int main(int argc, char **argv) {
    auto screenSize = sdl::Dims{300, 200};

    auto window = sdl::Window{"matrix view",
                              SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED,
                              screenSize.w,
                              screenSize.h,
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

    auto screen = MatrixScreen{screenSize.w, screenSize.h};

    for (auto event = sdl::waitEvent(); event.type != SDL_QUIT;
         event = sdl::waitEvent()) {
        std::cerr << event.type << ":" << event.window.event << " event\n";

        switch (event.type) {
        case SDL_WINDOWEVENT:
            if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                int width = event.window.data1;
                int height = event.window.data2;

                screen.resize(width, height);
                renderer.present();
            }
            break;
        }

        // Clear
        renderer.drawColor(30, 30, 30, 255);
        renderer.fillRect();

        {
            auto &c = screen.at(1, 1);
            c.texture = screen.cache.getCharacter(renderer, "ä");
            c.fg = sdl::White;
            c.bg = sdl::Red;
        }

        {
            auto &c = screen.at(1, 4);
            c.texture = screen.cache.getCharacter(renderer, "o");
            c.fg = sdl::White;
            c.bg = sdl::Blue;
        }

        screen.render(renderer);

        renderer.present();
    }
}
