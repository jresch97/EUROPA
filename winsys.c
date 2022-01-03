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
#include "xlibwinsys.h"

static const WINSYS* const WSS[] = {
#ifdef PLATFORM_LINUX
        &XLIBWINSYS,
#endif
        NULL
};

const WINSYS* winsys (const char *name)
{
        // lookup by name
        return NULL;
}

int winsysinit (const WINSYS *sys)
{
        assert(sys != NULL);
        return sys->drv.winsysinitcb(sys);
}

void winsysterm (const WINSYS *sys)
{
        assert(sys != NULL);
        sys->drv.winsystermcb(sys);
}

void winsyspoll (const WINSYS *sys)
{
        assert(sys != NULL);
        sys->drv.winsyspollcb(sys);
}

const WINSYS* defwinsys ()
{
        static const WINSYS* dws = NULL;
        if (!dws) dws = WSS[0];
        return dws;
}

int defwinsysinit()
{
        return winsysinit(defwinsys());
}

void defwinsysterm()
{
        winsysterm(defwinsys());
}

void defwinsyspoll()
{
        winsyspoll(defwinsys());
}