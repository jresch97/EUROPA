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

#include "font.h"

#include <assert.h>
#include <stdlib.h>

#include "ftfontsys.h"

static const FONTSYS* const FSS[] = {
        &FTFONTSYS,
        NULL
};

const FONTSYS *fontsysd()
{
        static const FONTSYS *dfs = NULL;
        if (!dfs) dfs = FSS[0];
        return dfs;
}

const FONTSYS *fontsysn(const char *name)
{
        return NULL;
}

int fontinit()
{
        return fontinits(fontsysd());
}

int fontinits(const FONTSYS *sys)
{
        assert(sys != NULL);
        return sys->drv.init(sys);
}

void fontterm()
{
        fontterms(fontsysd());
}

void fontterms(const FONTSYS *sys)
{
        assert(sys != NULL);
        sys->drv.term(sys);
}

FONT *fontalloc(const char *family, const char *style, int pt)
{
        /* TODO: Lookup font on system. */
        /* return fontload(); */
        return NULL;
}

FONT *fontload(const char *path, int pt)
{
        FONT *font;
        assert(path != NULL);
        assert(pt > 0);
        font = malloc(sizeof(*font));
        if (!font) goto errret;
        font->sys  = fontsysd();
        if (!font->sys) goto errff;
        font->family = font->style = NULL;
        font->path   = path;
        font->pt     = pt;
        if (!font->sys->drv.fontalloc(font)) goto errff;
        return font;
errff:  free(font);
errret: return NULL;
}

void fontfree(FONT *font)
{
        if (font) {
                if (font->sys && font->sys->drv.fontfree) {
                        font->sys->drv.fontfree(font);
                }
                free(font);
        }
}
