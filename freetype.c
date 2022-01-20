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

#include "freetype.h"

#include <assert.h>
#include <ft2build.h>
#include FT_FREETYPE_H

#define FTASSERT() (assert(d.lib != NULL))
#define FTFD(font) ((FTFONTDAT*)font->dat)

typedef struct FTDAT {
        FT_Library lib;
} FTDAT;

typedef struct FTFONTDAT {
        FT_Face face;
} FTFONTDAT;

static FTDAT d;

int ftinit()
{
        int err;
        if (d.lib) return 1;
        err = FT_Init_FreeType(&d.lib);
        if (err) goto errret;
        return 1;
errret: d.lib = NULL;
        return 0;
}

void ftterm()
{
        if (d.lib) {
                FT_Done_FreeType(d.lib);
                d.lib = NULL;
        }
}

int ftfntalloc(FONT *font)
{
        int        err;
        FTFONTDAT *fd;
        FTASSERT();
        assert(font != NULL);
        fd = malloc(sizeof(*fd));
        if (!fd) goto errret;
        font->dat = fd;
        err = FT_New_Face(d.lib, font->path, 0, &fd->face);
        if (err) goto errffd;
        err = FT_Set_Char_Size(fd->face, 0, font->pt << 6, 0, 0);
        if (err) goto errdf;
        font->family = fd->face->family_name;
        font->style  = fd->face->style_name;
        return 1;
errdf:  if (fd->face) FT_Done_Face(fd->face);
errffd: if (fd) free(fd);
errret: return 0;
}

void ftfntfree(FONT *font)
{
        FTFONTDAT *fd;
        FTASSERT();
        if (font) {
                fd = FTFD(font);
                if (fd) {
                        if (fd->face) FT_Done_Face(fd->face);
                        free(fd);
                }
        }
}

GLYPH *ftglyalloc(FONT *font, int code)
{
        FTFONTDAT   *fd;
        GLYPH       *g;
        FT_UInt      gi;
        FT_GlyphSlot gs;
        int          ec;
        FTASSERT();
        assert(font != NULL);
        g = malloc(sizeof(*g));
        if (!g) {
                goto err;
        }
        fd = FTFD(font);
        gi = FT_Get_Char_Index(fd->face, code);
        ec = FT_Load_Glyph(fd->face, gi, FT_LOAD_DEFAULT);
        if (ec) {
                goto errf;
        }
        ec = FT_Render_Glyph(fd->face->glyph, FT_RENDER_MODE_NORMAL);
        if (ec) {
                goto errf;
        }
        gs = fd->face->glyph;
        g->code = code;
        g->xadv = gs->metrics.horiAdvance >> 6;
        g->yadv = gs->metrics.vertAdvance >> 6;
        g->left = gs->bitmap_left >> 6;
        g->top  = gs->bitmap_top >> 6;
        if (gs->bitmap.width > 0 && gs->bitmap.rows > 0) {
                g->surf = surfalloc(RGBA32, gs->bitmap.width, gs->bitmap.rows);
                memcpy(g->surf->px, gs->bitmap.buffer,
                       abs(gs->bitmap.pitch) * gs->bitmap.rows);
        }
        else {
                g->surf = NULL;
        }
        return g;
errf:   free(g);
err:    return NULL;
}
