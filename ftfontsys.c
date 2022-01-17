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

#include "ftfontsys.h"

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

int ftfontalloc(FONT *font)
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
        err = FT_Set_Char_Size(fd->face, 0, font->pt, 0, 0);
        if (err) goto errdf;
        font->family = fd->face->family_name;
        font->style  = fd->face->style_name;
        return 1;
errdf:  if (fd->face) FT_Done_Face(fd->face);
errffd: if (fd) free(fd);
errret: return 0;
}

void ftfontfree(FONT *font)
{
        FTFONTDAT *fd;
        if (font) {
                fd = FTFD(font);
                if (fd) {
                        if (fd->face) FT_Done_Face(fd->face);
                        free(fd);
                }
        }
}
