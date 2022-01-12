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

#ifndef EUROPA_PXFMT_H
#define EUROPA_PXFMT_H

#define R24 (0xff0000)
#define G24 (0x00ff00)
#define B24 (0x0000ff)
#define A24 (0)
#define R32 (0xff000000)
#define G32 (0x00ff0000)
#define B32 (0x0000ff00)
#define A32 (0x000000ff)

typedef struct PXFMT {
        const char *name;
        int r, g, b, a, bypp, bipp;
} PXFMT;

static const PXFMT RGB24  = { "RGB24",  R24, G24, B24, A24, 3, 24 };
static const PXFMT BGR24  = { "BGR24",  B24, G24, R24, A24, 3, 24 };
static const PXFMT RGB32  = { "RGB32",  R24, G24, B24, A24, 4, 32 };
static const PXFMT BGR32  = { "BGR32",  B24, G24, R24, A24, 4, 32 };
static const PXFMT RGBA32 = { "RGBA32", R32, G32, B32, A32, 4, 32 };
static const PXFMT BGRA32 = { "BGRA32", B32, G32, R32, A32, 4, 32 };

#endif