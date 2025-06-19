/*
 *  check.h
 *
 *   Copyright (c) 2013-2025 OG Dev team of this fork gose to the  Pacman Development Team <pacman-dev@lists.archlinux.org>
 *   Copyright (c) 2025 ps4 Development Team <tigerclips1-ps4dev-team@ps4jaguarlinux.site>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef PM_CHECK_H
#define PM_CHECK_H

#include <alpm.h>

int check_pkg_fast(alpm_pkg_t *pkg);
int check_pkg_full(alpm_pkg_t *pkg);

#endif /* PM_CHECK_H */
