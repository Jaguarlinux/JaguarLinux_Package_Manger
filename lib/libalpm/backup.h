/*
 *  backup.h
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
#ifndef ALPM_BACKUP_H
#define ALPM_BACKUP_H

#include "alpm_list.h"
#include "alpm.h"

int _alpm_split_backup(const char *string, alpm_backup_t **backup);
alpm_backup_t *_alpm_needbackup(const char *file, alpm_pkg_t *pkg);
void _alpm_backup_free(alpm_backup_t *backup);
alpm_backup_t *_alpm_backup_dup(const alpm_backup_t *backup);

#endif /* ALPM_BACKUP_H */
