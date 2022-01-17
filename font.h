/**
 *
 * Author: Jared B. Resch
 *
 * This file is part of EUROPA.
 *
 * EUROPA is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * EUROPA is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with EUROPA. If not, see <https://www.gnu.org/licenses/>.
 *
 */

#ifndef EUROPA_FONT_H
#define EUROPA_FONT_H

typedef struct FONT FONT;

typedef struct FONTSYS {
        const char *name;
        const struct {
                int  (*init)     ();
                void (*term)     ();
                int  (*fontalloc)(FONT *font);
                void (*fontfree) (FONT *font);
        } drv;
} FONTSYS;

struct FONT {
        const FONTSYS *sys;
        const char    *path, *family, *style;
        int            pt;
        void          *dat;
};

const FONTSYS *fontsysd ();
const FONTSYS *fontsysn (const char *name);
int            fontinit ();
int            fontinits(const FONTSYS *sys);
void           fontterm ();
void           fontterms(const FONTSYS *sys);
FONT          *fontalloc(const char *family, const char *style, int pt);
FONT          *fontload (const char *path, int pt);
void           fontfree (FONT *font);

#endif
