/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2014  The ManaPlus Developers
 *
 *  This file is part of The ManaPlus Client.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/*      _______   __   __   __   ______   __   __   _______   __   __
 *     / _____/\ / /\ / /\ / /\ / ____/\ / /\ / /\ / ___  /\ /  |\/ /\
 *    / /\____\// / // / // / // /\___\// /_// / // /\_/ / // , |/ / /
 *   / / /__   / / // / // / // / /    / ___  / // ___  / // /| ' / /
 *  / /_// /\ / /_// / // / // /_/_   / / // / // /\_/ / // / |  / /
 * /______/ //______/ //_/ //_____/\ /_/ //_/ //_/ //_/ //_/ /|_/ /
 * \______\/ \______\/ \_\/ \_____\/ \_\/ \_\/ \_\/ \_\/ \_\/ \_\/
 *
 * Copyright (c) 2004 - 2008 Olof Naessén and Per Larsson
 *
 *
 * Per Larsson a.k.a finalman
 * Olof Naessén a.k.a jansem/yakslem
 *
 * Visit: http://guichan.sourceforge.net
 *
 * License: (BSD)
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name of Guichan nor the names of its contributors may
 *    be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef RENDER_SDL2GRAPHICS_H
#define RENDER_SDL2GRAPHICS_H

#ifdef USE_SDL2

#include "render/graphics.h"

#include "localconsts.h"

class Image;
class ImageCollection;
class ImageVertexes;
class MapLayer;

struct SDL_Surface;

/**
 * A central point of control for graphics.
 */
class SDLGraphics final : public Graphics
{
    public:
        /**
         * Constructor.
         */
        SDLGraphics();

        A_DELETE_COPY(SDLGraphics)

        /**
         * Destructor.
         */
        ~SDLGraphics();

        void _beginDraw() override final;

        void _endDraw() override final;

        bool pushClipArea(Rectangle rect) override final;

        void popClipArea() override final;

        bool drawRescaledImage(const Image *const image,
                               int dstX, int dstY,
                               const int desiredWidth,
                               const int desiredHeight) override final;

        void drawPattern(const Image *const image,
                         const int x, const int y,
                         const int w, const int h) override final;

        void inline drawPatternInline(const Image *const image,
                                      const int x, const int y,
                                      const int w, const int h);

        void drawRescaledPattern(const Image *const image,
                                 const int x, const int y,
                                 const int w, const int h,
                                 const int scaledWidth,
                                 const int scaledHeight) override final;

        void calcPattern(ImageVertexes *const vert,
                         const Image *const image,
                         const int x, const int y,
                         const int w, const int h) const override final;

        void calcPattern(ImageCollection *const vert,
                         const Image *const image,
                         const int x, const int y,
                         const int w, const int h) const override final;

        void calcTileVertexes(ImageVertexes *const vert,
                              const Image *const image,
                              int x, int y) const override final;

        void calcTileSDL(ImageVertexes *const vert,
                         int x, int y) const override final;

        void calcTileCollection(ImageCollection *const vertCol,
                                const Image *const image,
                                int x, int y) override final;

        void drawTileVertexes(const ImageVertexes *const vert) override final;

        void drawTileCollection(const ImageCollection *const vertCol)
                                override final;

        void updateScreen() override final;

        SDL_Surface *getScreenshot() override final A_WARN_UNUSED;

        bool drawNet(const int x1, const int y1,
                     const int x2, const int y2,
                     const int width, const int height) override final;

        void calcWindow(ImageCollection *const vertCol,
                        const int x, const int y,
                        const int w, const int h,
                        const ImageRect &imgRect) override final;

        void fillRectangle(const Rectangle &rect) override final;

        void drawRectangle(const Rectangle &rect) override final;

        void drawPoint(int x, int y) override final;

        void drawLine(int x1, int y1, int x2, int y2) override final;

        bool setVideoMode(const int w, const int h,
                          const int scale,
                          const int bpp,
                          const bool fs,
                          const bool hwaccel,
                          const bool resize,
                          const bool noFrame) override final;

        void setRendererFlags(const uint32_t flags)
        { mRendererFlags = flags; }

        bool drawImage(const Image *const image,
                       int dstX, int dstY) override final;

        void drawImageCached(const Image *const image,
                             int x, int y) override final;

        void drawPatternCached(const Image *const image,
                               const int x, const int y,
                               const int w, const int h) override final;

        void completeCache() override final;

        /**
         * Draws a rectangle using images. 4 corner images, 4 side images and 1
         * image for the inside.
         */
        void drawImageRect(int x, int y, int w, int h,
                           const ImageRect &imgRect);

    protected:
        uint32_t mRendererFlags;
        uint32_t mOldPixel;
        int mOldAlpha;

    private:
        void inline calcImageRect(ImageVertexes *const vert,
                                  int x, int y,
                                  int w, int h,
                                  const ImageRect &imgRect);

        void inline calcPatternInline(ImageVertexes *const vert,
                                      const Image *const image,
                                      const int x, const int y,
                                      const int w, const int h) const;

        void inline calcTileVertexesInline(ImageVertexes *const vert,
                                           const Image *const image,
                                           int x, int y) const;

        bool inline drawImageInline(const Image *const image,
                                    int dstX, int dstY);
};

#endif  // USE_SDL2
#endif  // RENDER_SDL2GRAPHICS_H
