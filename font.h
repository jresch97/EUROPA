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

#ifndef EUROPA_FONT_H
#define EUROPA_FONT_H

#include "surface.h"
#include "hashtabx.h"

#define GLYPHSH(code) (code)

typedef struct GLYPH {
        int      code;
        int      xadv, yadv, left, top;
        SURFACE *surf;
} GLYPH;

HASHTABX(GLYPHT, GLYPHNODE, glypht, GLYPH*, int, int, int, GLYPHSH, 2)

typedef struct FONT FONT;

typedef struct FONTSYS {
        const char *name;
        const struct {
                int    (*fntinit)  ();
                void   (*fntterm)  ();
                int    (*fntalloc) (FONT *font);
                void   (*fntfree)  (FONT *font);
                GLYPH *(*glyalloc) (FONT *font, int code);
        } drv;
} FONTSYS;

struct FONT {
        const FONTSYS *sys;
        const char    *path, *family, *style;
        int            pt;
        GLYPHT        *glyphs;
        void          *dat;
};

const FONTSYS *fntsysd  ();
const FONTSYS *fntsysn  (const char *name);
int            fntinit  ();
int            fntinit0 ();
int            fntinit1 (const FONTSYS *sys);
void           fntterm  ();
void           fntterm0 ();
void           fntterm1 (const FONTSYS *sys);
FONT          *fntalloc (const char *family, const char *style, int pt);
FONT          *fntload  (const char *path, int pt);
void           fntfree  (FONT *font);
GLYPH         *fntglyph (FONT *font, int code);
GLYPH         *glyalloc (FONT *font, int code);

#endif
