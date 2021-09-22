#pragma once

#include "canvas.h"

namespace matscreen {

struct MatrixScreen {
    CharacterCache cache;
    Canvas canvas;

    MatrixScreen(int width, int height, std::string fontFilename)
        : cache{fontFilename}
        , canvas{width, height} {}

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

    void resize(int width, int height) {
        auto newCanvas = Canvas{width, height};

        for (int y = 0; y < std::min(canvas.height, newCanvas.height); ++y) {
            for (int x = 0; x < std::min(canvas.width, newCanvas.width); ++x) {
                newCanvas.at(x, y) = canvas.at(x, y);
            }
        }

        canvas = std::move(newCanvas);
    }

    auto at(int x, int y) -> CanvasCell & {
        return canvas.at(x, y);
    }
};

} // namespace matscreen
