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


#include <stdio.h>
#include <stdlib.h>
#include <os.h>

int main(int argc, char *argv[])
{
        const char *name, *vers, *arch;

        /* Implicitly called but can be called if desired. */
        if (!osinit()) return EXIT_FAILURE;

        name = osname(); /* Gets OS name such as "Linux". */
        vers = osvers(); /* Gets OS kernel version such as "X.Y.Z". */
        arch = osarch(); /* Gets OS architecture such as "x86_64". */

        // Display.
        printf("osname=%s\n", name);
        printf("osvers=%s\n", vers);
        printf("osarch=%s\n", arch);

        return EXIT_SUCCESS;
}