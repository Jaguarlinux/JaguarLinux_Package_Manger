/*
 *  util-common.h
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

#ifndef PM_UTIL_COMMON_H
#define PM_UTIL_COMMON_H

#include <stdio.h>
#include <sys/stat.h> /* struct stat */

char *hex_representation(const unsigned char *bytes, size_t size);
const char *mbasename(const char *path);
char *mdirname(const char *path);

int llstat(char *path, struct stat *buf);

char *safe_fgets(char *s, int size, FILE *stream);

void wordsplit_free(char **ws);
char **wordsplit(const char *str);

size_t strtrim(char *str);

#ifndef HAVE_STRNDUP
char *strndup(const char *s, size_t n);
#endif

#define ARRAYSIZE(a) (sizeof (a) / sizeof (a[0]))

#endif /* PM_UTIL_COMMON_H */
