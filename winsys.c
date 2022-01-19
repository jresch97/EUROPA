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

#include <assert.h>
#include <stdlib.h>

#include "winsys.h"
#include "xwinsys.h"
#include "winsys32.h"
#include "platform.h"

static const WINSYS* const W[] = {
#ifdef PLATFORM_LINUX
        &XWINSYS,
#endif
#ifdef PLATFORM_WIN32
        &WINSYS32,
#endif
        NULL
};

const WINSYS *winsysn(const char *name)
{
        return NULL;
}

const WINSYS *winsysd()
{
        static const WINSYS *d = NULL;
        if (!d) d = W[0];
        return d;
}

bool wininit()
{
        return wininit0();
}

bool wininit0()
{
        return wininit1(winsysd());
}

bool wininit1(const WINSYS *sys)
{
        assert(sys != NULL);
        return sys->drv.init(sys);
}

void winterm()
{
        winterm0();
}

void winterm0()
{
        winterm1(winsysd());
}

void winterm1(const WINSYS *sys)
{
        assert(sys != NULL);
        sys->drv.term(sys);
}

void winpoll()
{
        winpoll0();
}

void winpoll0()
{
        winpoll1(winsysd());
}

void winpoll1(const WINSYS *sys)
{
        assert(sys != NULL);
        sys->drv.poll(sys);
}