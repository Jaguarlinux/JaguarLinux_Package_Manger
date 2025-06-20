/*
 *  trans.h
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
#ifndef ALPM_TRANS_H
#define ALPM_TRANS_H

#include "alpm.h"

typedef enum _alpm_transstate_t {
	STATE_IDLE = 0,
	STATE_INITIALIZED,
	STATE_PREPARED,
	STATE_DOWNLOADING,
	STATE_COMMITTING,
	STATE_COMMITTED,
	STATE_INTERRUPTED
} alpm_transstate_t;

/* Transaction */
typedef struct _alpm_trans_t {
	/* bitfield of alpm_transflag_t flags */
	int flags;
	alpm_transstate_t state;
	alpm_list_t *unresolvable;  /* list of (alpm_pkg_t *) */
	alpm_list_t *add;           /* list of (alpm_pkg_t *) */
	alpm_list_t *remove;        /* list of (alpm_pkg_t *) */
	alpm_list_t *skip_remove;   /* list of (char *) */
} alpm_trans_t;

void _alpm_trans_free(alpm_trans_t *trans);
/* flags is a bitfield of alpm_transflag_t flags */
int _alpm_trans_init(alpm_trans_t *trans, int flags);
int _alpm_runscriptlet(alpm_handle_t *handle, const char *filepath,
		const char *script, const char *ver, const char *oldver, int is_archive);

#endif /* ALPM_TRANS_H */
