/*
 *  ps4.h
 *
 *   Copyright (c) 2013-2025 OG Dev team of this fork gose to the  ps4 Development Team <ps4-dev@lists.archlinux.org>
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
#ifndef PM_PS4_H
#define PM_PS4_H

#include <alpm_list.h>

#define PS4_CALLER_PREFIX "PS4"

/* database.c */
int ps4_database(alpm_list_t *targets);
/* deptest.c */
int ps4_deptest(alpm_list_t *targets);
/* files.c */
int ps4_files(alpm_list_t *files);
/* query.c */
int ps4_query(alpm_list_t *targets);
/* remove.c */
int ps4_remove(alpm_list_t *targets);
/* sync.c */
int ps4_sync(alpm_list_t *targets);
int sync_prepare_execute(void);
/* upgrade.c */
int ps4_upgrade(alpm_list_t *targets);

#endif /* PM_PS4_H */
