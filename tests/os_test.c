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


#include <os.h>

int main(int argc, char *argv[])
{
        const char *osvers, *osname;

        // Implicitly called but can be called if desired.
        if (!osinit()) return EXIT_FAILURE;

        osname = osname(); // Gets OS name such as "Microsoft Windows 7" or "Linux".
        osvers = osvers(); // Gets OS kernel version such as "(Windows/NT) 6.1" or "(Linux) X.Y.Z".

        // Display.
        printf("osvers=%s\n", osvers);
        printf("osname=%s\n", osname);

        return EXIT_SUCCESS;
}