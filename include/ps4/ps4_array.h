/*     $NetBSD: prop_array.h,v 1.8 2008/09/11 13:15:13 haad Exp $    */

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

#ifndef _PS4_ARRAY_H_
#define	_PS4_ARRAY_H_

#include <stdint.h>
#include <ps4/ps4_object.h>

typedef struct _prop_array *ps4_array_t;

#ifdef __cplusplus
extern "C" {
#endif

ps4_array_t	ps4_array_create(void);
ps4_array_t	ps4_array_create_with_capacity(unsigned int);

ps4_array_t	ps4_array_copy(ps4_array_t);
ps4_array_t	ps4_array_copy_mutable(ps4_array_t);

unsigned int	ps4_array_capacity(ps4_array_t);
unsigned int	ps4_array_count(ps4_array_t);
bool		ps4_array_ensure_capacity(ps4_array_t, unsigned int);

void		ps4_array_make_immutable(ps4_array_t);
bool		ps4_array_mutable(ps4_array_t);

ps4_object_iterator_t ps4_array_iterator(ps4_array_t);

ps4_object_t	ps4_array_get(ps4_array_t, unsigned int);
bool		ps4_array_set(ps4_array_t, unsigned int, ps4_object_t);
bool		ps4_array_add(ps4_array_t, ps4_object_t);
bool		ps4_array_add_first(ps4_array_t, ps4_object_t);
void		ps4_array_remove(ps4_array_t, unsigned int);

bool		ps4_array_equals(ps4_array_t, ps4_array_t);

char *		ps4_array_externalize(ps4_array_t);
ps4_array_t	ps4_array_internalize(const char *);

bool		ps4_array_externalize_to_file(ps4_array_t, const char *);
bool		ps4_array_externalize_to_zfile(ps4_array_t, const char *);
ps4_array_t	ps4_array_internalize_from_file(const char *);
ps4_array_t	ps4_array_internalize_from_zfile(const char *);

/*
 * Utility routines to make it more convenient to work with values
 * stored in dictionaries.
 */
bool		ps4_array_get_bool(ps4_array_t, unsigned int,
					 bool *);
bool		ps4_array_set_bool(ps4_array_t, unsigned int,
					 bool);

bool		ps4_array_get_int8(ps4_array_t, unsigned int,
					 int8_t *);
bool		ps4_array_get_uint8(ps4_array_t, unsigned int,
					  uint8_t *);
bool		ps4_array_set_int8(ps4_array_t, unsigned int,
					 int8_t);
bool		ps4_array_set_uint8(ps4_array_t, unsigned int,
					  uint8_t);

bool		ps4_array_get_int16(ps4_array_t, unsigned int,
					  int16_t *);
bool		ps4_array_get_uint16(ps4_array_t, unsigned int,
					   uint16_t *);
bool		ps4_array_set_int16(ps4_array_t, unsigned int,
					  int16_t);
bool		ps4_array_set_uint16(ps4_array_t, unsigned int,
					   uint16_t);

bool		ps4_array_get_int32(ps4_array_t, unsigned int,
					  int32_t *);
bool		ps4_array_get_uint32(ps4_array_t, unsigned int,
					   uint32_t *);
bool		ps4_array_set_int32(ps4_array_t, unsigned int,
					  int32_t);
bool		ps4_array_set_uint32(ps4_array_t, unsigned int,
					   uint32_t);

bool		ps4_array_get_int64(ps4_array_t, unsigned int,
					  int64_t *);
bool		ps4_array_get_uint64(ps4_array_t, unsigned int,
					   uint64_t *);
bool		ps4_array_set_int64(ps4_array_t, unsigned int,
					  int64_t);
bool		ps4_array_set_uint64(ps4_array_t, unsigned int,
					   uint64_t);

bool		ps4_array_add_int8(ps4_array_t, int8_t);
bool		ps4_array_add_uint8(ps4_array_t, uint8_t);

bool		ps4_array_add_int16(ps4_array_t, int16_t);
bool		ps4_array_add_uint16(ps4_array_t, uint16_t);

bool		ps4_array_add_int32(ps4_array_t, int32_t);
bool		ps4_array_add_uint32(ps4_array_t, uint32_t);

bool		ps4_array_add_int64(ps4_array_t, int64_t);
bool		ps4_array_add_uint64(ps4_array_t, uint64_t);

bool		ps4_array_get_cstring(ps4_array_t, unsigned int,
					     char **);
bool		ps4_array_set_cstring(ps4_array_t, unsigned int,
					    const char *);
bool		ps4_array_add_cstring(ps4_array_t, const char *);
bool		ps4_array_add_cstring_nocopy(ps4_array_t,
						const char *);
bool		ps4_array_get_cstring_nocopy(ps4_array_t,
                                                   unsigned int,
						   const char **);
bool		ps4_array_set_cstring_nocopy(ps4_array_t,
						   unsigned int,
						   const char *);
bool		ps4_array_add_and_rel(ps4_array_t, ps4_object_t);

#ifdef __cplusplus
}
#endif

#endif /* _PS4_ARRAY_H_ */
