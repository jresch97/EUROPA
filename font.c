/**
 *
 * Copyright (C) 2021 Jared B. Resch
 *
 * This file is part of EUROPA.
 * 
 * EUROPA is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 * 
 * EUROPA is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License for more details.
 * 
 * You should have received a copy of the GNU Affero General Public License
 * along with EUROPA. If not, see <https://www.gnu.org/licenses/>.
 *
 */

#include <assert.h>
#include <stdlib.h>

#include "font.h"
#include "freetype.h"

static const FONTSYS* const FSS[] = {
        &FREETYPE,
        NULL
};

const FONTSYS *fntsysd()
{
        static const FONTSYS *dfs = NULL;
        if (!dfs) dfs = FSS[0];
        return dfs;
}

const FONTSYS *fntsysn(const char *name)
{
        return NULL;
}

int fntinit()
{
        const FONTSYS *sys;
        sys = fntsysd();
        assert(sys != NULL);
        assert(sys->drv.fntinit != NULL);
        return sys->drv.fntinit();
}

int fntinit0()
{
        const FONTSYS *sys;
        sys = fntsysd();
        assert(sys != NULL);
        assert(sys->drv.fntinit != NULL);
        return sys->drv.fntinit();
}

int fntinit1(const FONTSYS *sys)
{
        assert(sys != NULL);
        assert(sys->drv.fntinit != NULL);
        return sys->drv.fntinit();
}

void fntterm()
{
        const FONTSYS *sys;
        sys = fntsysd();
        assert(sys != NULL);
        assert(sys->drv.fntterm != NULL);
        sys->drv.fntterm();
}

void fntterm0()
{
        const FONTSYS *sys;
        sys = fntsysd();
        assert(sys != NULL);
        assert(sys->drv.fntterm != NULL);
        sys->drv.fntterm();
}

void fntterm1(const FONTSYS *sys)
{
        assert(sys != NULL);
        assert(sys->drv.fntterm != NULL);
        sys->drv.fntterm();
}

FONT *fntalloc(const char *family, const char *style, int pt)
{
        /* TODO: Lookup font on system. */
        /* return fontload(); */
        return NULL;
}

FONT *fntload(const char *path, int pt)
{
        FONT *font;
        assert(path != NULL);
        assert(pt > 0);
        font = malloc(sizeof(*font));
        if (!font) {
                goto err;
        }
        font->sys = fntsysd();
        if (!font->sys) {
                goto errf;
        }
        font->family = font->style = NULL;
        font->path   = path;
        font->pt     = pt;
        font->glyphs = glyphtalloc(257);
        if (!font->glyphs) {
                goto errf;
        }
        assert(font->sys->drv.fntalloc != NULL);
        if (!font->sys->drv.fntalloc(font)) {
                goto errf;
        }
        return font;
errf:   free(font);
err:    return NULL;
}

void fntfree(FONT *font)
{
        if (font) {
                if (font->sys) {
                        assert(font->sys->drv.fntfree != NULL);
                        font->sys->drv.fntfree(font);
                }
                if (font->glyphs) {
                        glyphtfree(font->glyphs);
                }
                free(font);
        }
}

GLYPH *fntglyph(FONT *font, int code)
{
        GLYPH *g;
        assert(font != NULL);
        if (!glyphtget(font->glyphs, code, &g)) {
                g = glyalloc(font, code);
                if (!g) {
                        goto err;
                }
                if (!glyphtins(font->glyphs, code, g)) {
                        goto errf;
                }
        }
        return g;
errf:   free(g);
err:    return NULL;
}

GLYPH *glyalloc(FONT *font, int code)
{
        assert(font != NULL);
        assert(font->sys != NULL);
        assert(font->sys->drv.glyalloc != NULL);
        return font->sys->drv.glyalloc(font, code);
}
