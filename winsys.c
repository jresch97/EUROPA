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

#include "winsys.h"

#include <assert.h>
#include <stdlib.h>

#include "platform.h"
#include "xwinsys.h"
#include "winsys32.h"

static const WINSYS* const WSS[] = {
#ifdef PLATFORM_LINUX
        &XWINSYS,
#endif
#ifdef PLATFORM_WIN32
        &WINSYS32,
#endif
        NULL
};

const WINSYS* winsysd()
{
        static const WINSYS* dws = NULL;
        if (!dws) dws = WSS[0];
        return dws;
}

const WINSYS* winsysn(const char *name)
{
        return NULL;
}

int wininit()
{
        return wininits(winsysd());
}

int wininits(const WINSYS *sys)
{
        assert(sys != NULL);
        return sys->drv.init(sys);
}

void winterm()
{
        winterms(winsysd());
}

void winterms(const WINSYS *sys)
{
        assert(sys != NULL);
        sys->drv.term(sys);
}

void winpoll()
{
        winpolls(winsysd());
}

void winpolls(const WINSYS *sys)
{
        assert(sys != NULL);
        sys->drv.poll(sys);
}
