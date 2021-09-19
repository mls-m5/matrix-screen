
#include "sdlpp/events.hpp"
#include "sdlpp/render.hpp"
#include "sdlpp/ttf.hpp"
#include "sdlpp/window.hpp"
#include <cstring>
#include <filesystem>
#include <iostream>
#include <map>
#include <vector>

namespace {

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
            if (!std::filesystem::exists("data/UbuntuMono-Regular.ttf")) {
                std::cerr << "font does not exist\n";
            }

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

struct CanvasCell {
    sdl::TextureView texture = {};

    sdl::Color fg = {255, 255, 255, 255};
    sdl::Color bg = {0, 0, 0, 255};
};

struct Canvas {
    int width = 0;
    int height = 0;
    std::vector<CanvasCell> values;

    Canvas(int width, int height)
        : width(width)
        , height(height) {
        values.resize(width * height);
    }

    auto at(int x, int y) -> CanvasCell & {
        return values.at(y * width + x);
    }
};

struct Screen {
    Canvas canvas;
    CharacterCache cache;

    Screen()
        : canvas{20, 10} {}

    void render(sdl::RendererView renderer) {
        for (int y = 0; y < canvas.height; ++y) {
            for (int x = 0; x < canvas.width; ++x) {
                auto rect = sdl::Rect{x * cache.charWidth,
                                      y * cache.charHeight,
                                      cache.charWidth,
                                      cache.charHeight};

                auto cell = canvas.at(x, y);

                renderer.drawColor(cell.bg);
                renderer.fillRect(rect);

                if (!cell.texture) {
                    continue;
                }

                cell.texture.colorMod(cell.fg);
                renderer.copy(cell.texture, rect);
            }
        }
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
            auto &c = screen.canvas.at(1, 1);
            c.texture = screen.cache.getCharacter(renderer, "ä");
            c.fg = sdl::White;
            c.bg = sdl::Red;
        }

        {
            auto &c = screen.canvas.at(1, 4);
            c.texture = screen.cache.getCharacter(renderer, "o");
            c.fg = sdl::White;
            c.bg = sdl::Blue;
        }

        screen.render(renderer);

        renderer.present();
    }
}
