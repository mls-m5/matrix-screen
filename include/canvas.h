#pragma once

#include "charactercache.h"
#include "sdlpp/render.hpp"
#include "sdlpp/texture.hpp"
#include <vector>

namespace matscreen {

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

} // namespace matscreen
