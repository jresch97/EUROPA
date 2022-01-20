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

#ifndef EUROPA_FREETYPE_H
#define EUROPA_FREETYPE_H

#include "font.h"

int    ftinit     ();
void   ftterm     ();
int    ftfntalloc (FONT *font);
void   ftfntfree  (FONT *font);
GLYPH *ftglyalloc (FONT *font, int code);

static const FONTSYS FREETYPE = {
        "freetype",
        {
                &ftinit,
                &ftterm,
                &ftfntalloc,
                &ftfntfree,
                &ftglyalloc
        }
};

#endif
