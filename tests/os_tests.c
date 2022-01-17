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

#include <stdio.h>
#include <stdlib.h>
#include <os.h>

int main(int argc, char *argv[])
{
        const char *name, *rels, *vers, *arch, *mach, *user;

        /* Implicitly called but can be called if desired. */
        if (!osinit()) return EXIT_FAILURE;

        name = osname(); /* Gets OS name such as "Linux". */
        rels = osrels(); /* Gets OS release such as "Vista SP2". */
        vers = osvers(); /* Gets OS kernel version such as "X.Y.Z". */
        arch = osarch(); /* Gets OS architecture such as "x86_64". */
        mach = osmach(); /* Gets OS machine/network name. */
        user = osuser(); /* Gets OS login username. */

        printf("osname=%s\n", name);
        printf("osrels=%s\n", rels);
        printf("osvers=%s\n", vers);
        printf("osarch=%s\n", arch);
        printf("osmach=%s\n", mach);
        printf("osuser=%s\n", user);

        return EXIT_SUCCESS;
}
