#pragma once

#include "canvas.h"
#include "charactercache.h"
#include "sdlpp/pixels.hpp"

namespace matscreen {

struct MatrixScreen {
    CharacterCache cache;
    Canvas canvas;

    MatrixScreen(int width,
                 int height,
                 std::string fontFilename,
                 int fontSize = 12)
        : cache{fontFilename, fontSize}
        , canvas{width, height} {}

    // Render the whole screen at once
    void render(sdl::RendererView renderer) {
        render(renderer, 0, 0, {0, 0, canvas.width, canvas.height});
    }

    sdl::Rect getDstRect(
        int x, int y, sdl::Rect rect, int targetX, int targetY) {

        return sdl::Rect{(x - rect.x) * cache.charWidth + targetX,
                         (y - rect.y) * cache.charHeight + targetY,
                         cache.charWidth,
                         cache.charHeight};
    }

    void render(sdl::RendererView renderer,
                int targetX,
                int targetY,
                sdl::Rect rect) {
        for (int y = rect.y; y < rect.y + rect.h; ++y) {
            for (int x = rect.x; x < rect.x + rect.w; ++x) {
                auto dstRect = getDstRect(x, y, rect, targetX, targetY);

                auto cell = canvas.at(x, y);

                renderer.drawColor(cell.bg);
                renderer.fillRect(dstRect);

                if (!cell.texture) {
                    continue;
                }

                cell.texture.colorMod(cell.fg);
                renderer.copy(cell.texture, dstRect);
            }
        }
    }

    /// Render a single cell highlighted
    void renderCursor(sdl::RendererView renderer,
                      sdl::Rect rect,
                      int cursorX,
                      int cursorY) {
        if (!canvas.isInside(cursorX, cursorY)) {
            return;
        }
        auto cell = canvas.at(cursorX, cursorY);

        auto fg = cell.bg;
        auto bg = cell.fg;

        renderer.drawColor(bg);
        auto dstRect = getDstRect(cursorX, cursorY, rect, 0, 0);
        renderer.fillRect(dstRect);
        cell.texture.colorMod(fg);
        renderer.copy(cell.texture, dstRect);
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

    void resizeFont(int fontSize) {
        cache.resize(fontSize);
    }

    int fontSize() const {
        return cache.fontSize;
    }

    auto at(int x, int y) -> CanvasCell & {
        return canvas.at(x, y);
    }
};

} // namespace matscreen
