/*	$NetBSD: prop_dictionary.h,v 1.9 2008/04/28 20:22:51 martin Exp $	*/

/*-
 * Copyright (c) 2006 The NetBSD Foundation, Inc.
 * All rights reserved.
 *
 * This code is derived from software contributed to The NetBSD Foundation
 * by Jason R. Thorpe.
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
 * THIS SOFTWARE IS PROVIDED BY THE NETBSD FOUNDATION, INC. AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _PS4_DICTIONARY_H_
#define	_PS4_DICTIONARY_H_

#include <stdint.h>
#include <ps4/ps4_object.h>
#include <ps4/ps4_array.h>

typedef struct _prop_dictionary *ps4_dictionary_t;
typedef struct _prop_dictionary_keysym *ps4_dictionary_keysym_t;

#ifdef __cplusplus
extern "C" {
#endif

ps4_dictionary_t ps4_dictionary_create(void);
ps4_dictionary_t ps4_dictionary_create_with_capacity(unsigned int);

ps4_dictionary_t ps4_dictionary_copy(ps4_dictionary_t);
ps4_dictionary_t ps4_dictionary_copy_mutable(ps4_dictionary_t);

unsigned int	ps4_dictionary_count(ps4_dictionary_t);
bool		ps4_dictionary_ensure_capacity(ps4_dictionary_t,
						unsigned int);

void		ps4_dictionary_make_immutable(ps4_dictionary_t);

ps4_object_iterator_t ps4_dictionary_iterator(ps4_dictionary_t);
ps4_array_t	ps4_dictionary_all_keys(ps4_dictionary_t);

ps4_object_t	ps4_dictionary_get(ps4_dictionary_t, const char *);
bool		ps4_dictionary_set(ps4_dictionary_t, const char *,
				    ps4_object_t);
void		ps4_dictionary_remove(ps4_dictionary_t, const char *);

ps4_object_t	ps4_dictionary_get_keysym(ps4_dictionary_t,
					   ps4_dictionary_keysym_t);
bool		ps4_dictionary_set_keysym(ps4_dictionary_t,
					   ps4_dictionary_keysym_t,
					   ps4_object_t);
void		ps4_dictionary_remove_keysym(ps4_dictionary_t,
					      ps4_dictionary_keysym_t);

bool		ps4_dictionary_equals(ps4_dictionary_t, ps4_dictionary_t);

char *		ps4_dictionary_externalize(ps4_dictionary_t);
ps4_dictionary_t ps4_dictionary_internalize(const char *);

bool		ps4_dictionary_externalize_to_file(ps4_dictionary_t,
						    const char *);
bool		ps4_dictionary_externalize_to_zfile(ps4_dictionary_t,
						     const char *);
ps4_dictionary_t ps4_dictionary_internalize_from_file(const char *);
ps4_dictionary_t ps4_dictionary_internalize_from_zfile(const char *);

const char *	ps4_dictionary_keysym_cstring_nocopy(ps4_dictionary_keysym_t);

bool		ps4_dictionary_keysym_equals(ps4_dictionary_keysym_t,
					      ps4_dictionary_keysym_t);

/*
 * Utility routines to make it more convenient to work with values
 * stored in dictionaries.
 */
bool		ps4_dictionary_get_dict(ps4_dictionary_t, const char *,
					 ps4_dictionary_t *);
bool		ps4_dictionary_get_bool(ps4_dictionary_t, const char *,
					 bool *);
bool		ps4_dictionary_set_bool(ps4_dictionary_t, const char *,
					 bool);

bool		ps4_dictionary_get_int8(ps4_dictionary_t, const char *,
					 int8_t *);
bool		ps4_dictionary_get_uint8(ps4_dictionary_t, const char *,
					  uint8_t *);
bool		ps4_dictionary_set_int8(ps4_dictionary_t, const char *,
					 int8_t);
bool		ps4_dictionary_set_uint8(ps4_dictionary_t, const char *,
					  uint8_t);

bool		ps4_dictionary_get_int16(ps4_dictionary_t, const char *,
					  int16_t *);
bool		ps4_dictionary_get_uint16(ps4_dictionary_t, const char *,
					   uint16_t *);
bool		ps4_dictionary_set_int16(ps4_dictionary_t, const char *,
					  int16_t);
bool		ps4_dictionary_set_uint16(ps4_dictionary_t, const char *,
					   uint16_t);

bool		ps4_dictionary_get_int32(ps4_dictionary_t, const char *,
					  int32_t *);
bool		ps4_dictionary_get_uint32(ps4_dictionary_t, const char *,
					   uint32_t *);
bool		ps4_dictionary_set_int32(ps4_dictionary_t, const char *,
					  int32_t);
bool		ps4_dictionary_set_uint32(ps4_dictionary_t, const char *,
					   uint32_t);

bool		ps4_dictionary_get_int64(ps4_dictionary_t, const char *,
					  int64_t *);
bool		ps4_dictionary_get_uint64(ps4_dictionary_t, const char *,
					   uint64_t *);
bool		ps4_dictionary_set_int64(ps4_dictionary_t, const char *,
					  int64_t);
bool		ps4_dictionary_set_uint64(ps4_dictionary_t, const char *,
					   uint64_t);

bool		ps4_dictionary_get_cstring(ps4_dictionary_t, const char *,
					     char **);
bool		ps4_dictionary_set_cstring(ps4_dictionary_t, const char *,
					    const char *);

bool		ps4_dictionary_get_cstring_nocopy(ps4_dictionary_t,
						   const char *,
						   const char **);
bool		ps4_dictionary_set_cstring_nocopy(ps4_dictionary_t,
						   const char *,
						   const char *);
bool		ps4_dictionary_set_and_rel(ps4_dictionary_t,
					    const char *,
					    ps4_object_t);

#ifdef __cplusplus
}
#endif

#endif /* _PS4_DICTIONARY_H_ */
