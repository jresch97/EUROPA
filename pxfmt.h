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

typedef struct PXFMT {
        const char* name;
        int r, g, b, a, bypp, bipp;
} PXFMT;

static const PXFMT RGB24  = { "RGB24",    0xff0000,   0x00ff00,   0x0000ff, 0, 3, 24};
static const PXFMT BGR24  = { "BGR24",    0x0000ff,   0x00ff00,   0xff0000, 0, 3, 24 };
static const PXFMT RGB32  = { "RGB32",  0xff000000, 0x00ff0000, 0x0000ff00, 0, 4, 32 };
static const PXFMT BGR32  = { "BGR32",  0x0000ff00, 0x00ff0000, 0xff000000, 0, 4, 32 };
static const PXFMT RGBA32 = { "RGBA32", 0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff, 4, 32 };
static const PXFMT BGRA32 = { "BGRA32", 0x0000ff00, 0x00ff0000, 0xff000000, 0x000000ff, 4, 32 };

#endif