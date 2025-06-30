/*-
 * Copyright (c) 2010-2015 Juan Romero Pardines.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *-
 */

#ifndef _PS4_API_IMPL_H_
#define _PS4_API_IMPL_H_

#include <assert.h>
#include "ps4.h"

/*
 * By default all public functions have default visibility, unless
 * visibility has been detected by configure and the HIDDEN definition
 * is used.
 */
#if HAVE_VISIBILITY
#define HIDDEN __attribute__ ((visibility("hidden")))
#else
#define HIDDEN
#endif

#include "queue.h"
#include "compat.h"

#ifndef __UNCONST
#define __UNCONST(a)	((void *)(uintptr_t)(const void *)(a))
#endif

#ifndef __arraycount
#define __arraycount(x) (sizeof(x) / sizeof(*x))
#endif

struct archive;
struct archive_entry;

/**
 * @private
 */
int HIDDEN dewey_match(const char *, const char *);
int HIDDEN ps4_pkgdb_init(struct ps4_handle *);
void HIDDEN ps4_pkgdb_release(struct ps4_handle *);
int HIDDEN ps4_pkgdb_conversion(struct ps4_handle *);
int HIDDEN ps4_array_replace_dict_by_name(ps4_array_t, ps4_dictionary_t,
		const char *);
int HIDDEN ps4_array_replace_dict_by_pattern(ps4_array_t, ps4_dictionary_t,
		const char *);
bool HIDDEN ps4_remove_pkg_from_array_by_name(ps4_array_t, const char *);
bool HIDDEN ps4_remove_pkg_from_array_by_pattern(ps4_array_t, const char *);
bool HIDDEN ps4_remove_pkg_from_array_by_pkgver(ps4_array_t, const char *);
void HIDDEN ps4_fetch_set_cache_connection(int, int);
void HIDDEN ps4_fetch_unset_cache_connection(void);
int HIDDEN ps4_entry_is_a_conf_file(ps4_dictionary_t, const char *);
int HIDDEN ps4_entry_install_conf_file(struct ps4_handle *, ps4_dictionary_t,
		ps4_dictionary_t, struct archive_entry *, const char *,
		const char *, bool);
ps4_dictionary_t HIDDEN ps4_find_virtualpkg_in_conf(struct ps4_handle *,
		ps4_dictionary_t, const char *);
ps4_dictionary_t HIDDEN ps4_find_pkg_in_dict(ps4_dictionary_t, const char *);
ps4_dictionary_t HIDDEN ps4_find_virtualpkg_in_dict(struct ps4_handle *,
		ps4_dictionary_t, const char *);
ps4_dictionary_t HIDDEN ps4_find_pkg_in_array(ps4_array_t, const char *,
		ps4_trans_type_t);
ps4_dictionary_t HIDDEN ps4_find_virtualpkg_in_array(struct ps4_handle *,
		ps4_array_t, const char *, ps4_trans_type_t);

/* transaction */
bool HIDDEN ps4_transaction_check_revdeps(struct ps4_handle *, ps4_array_t);
bool HIDDEN ps4_transaction_check_shlibs(struct ps4_handle *, ps4_array_t);
bool HIDDEN ps4_transaction_check_replaces(struct ps4_handle *, ps4_array_t);
bool HIDDEN ps4_transaction_check_conflicts(struct ps4_handle *, ps4_array_t);
bool HIDDEN ps4_transaction_store(struct ps4_handle *, ps4_array_t, ps4_dictionary_t, bool);
int HIDDEN ps4_transaction_init(struct ps4_handle *);
int HIDDEN ps4_transaction_files(struct ps4_handle *,
		ps4_object_iterator_t);
int HIDDEN ps4_transaction_fetch(struct ps4_handle *,
		ps4_object_iterator_t);
int HIDDEN ps4_transaction_pkg_deps(struct ps4_handle *, ps4_array_t, ps4_dictionary_t);
int HIDDEN ps4_transaction_internalize(struct ps4_handle *, ps4_object_iterator_t);

char HIDDEN *ps4_get_remote_repo_string(const char *);
int HIDDEN ps4_repo_sync(struct ps4_handle *, const char *);
int HIDDEN ps4_file_hash_check_dictionary(struct ps4_handle *,
		ps4_dictionary_t, const char *, const char *);
int HIDDEN ps4_file_exec(struct ps4_handle *, const char *, ...);
void HIDDEN ps4_set_cb_fetch(struct ps4_handle *, off_t, off_t, off_t,
		const char *, bool, bool, bool);
int HIDDEN ps4_set_cb_state(struct ps4_handle *, ps4_state_t, int,
		const char *, const char *, ...);
int HIDDEN ps4_unpack_binary_pkg(struct ps4_handle *, ps4_dictionary_t);
int HIDDEN ps4_remove_pkg(struct ps4_handle *, const char *, bool);
int HIDDEN ps4_register_pkg(struct ps4_handle *, ps4_dictionary_t);

char HIDDEN *ps4_archive_get_file(struct archive *, struct archive_entry *);
ps4_dictionary_t HIDDEN ps4_archive_get_dictionary(struct archive *,
		struct archive_entry *);
const char HIDDEN *vpkg_user_conf(struct ps4_handle *, const char *);

struct archive HIDDEN *ps4_archive_read_new(void);
int HIDDEN ps4_archive_read_open(struct archive *ar, const char *path);
int HIDDEN ps4_archive_read_open_remote(struct archive *ar, const char *url);
int HIDDEN ps4_archive_errno(struct archive *ar);

ps4_array_t HIDDEN ps4_get_pkg_fulldeptree(struct ps4_handle *,
		const char *, bool);
struct ps4_repo HIDDEN *ps4_regget_repo(struct ps4_handle *,
		const char *);
int HIDDEN ps4_conf_init(struct ps4_handle *);

#endif /* !_PS4_API_IMPL_H_ */
