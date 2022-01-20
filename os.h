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

#ifndef EUROPA_OS_H
#define EUROPA_OS_H

/**
 * 
 * The OS module is concerned with providing information about the operating
 * system and device hardware.
 * 
 */

int         osinit ();
const char* osname ();
const char* osrels ();
const char* osvers ();
const char* osarch ();
const char* osmach ();
const char* osuser ();

#endif
