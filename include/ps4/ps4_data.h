/*	$NetBSD: prop_data.h,v 1.3 2008/04/28 20:22:51 martin Exp $	*/

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

#ifndef _PS4_DATA_H_
#define	_PS4_DATA_H_

#include <stdint.h>
#include <sys/types.h>
#include <ps4/ps4_object.h>

typedef struct _prop_data *ps4_data_t;

#ifdef __cplusplus
extern "C" {
#endif

ps4_data_t	ps4_data_create_data(const void *, size_t);
ps4_data_t	ps4_data_create_data_nocopy(const void *, size_t);

ps4_data_t	ps4_data_copy(ps4_data_t);

size_t		ps4_data_size(ps4_data_t);

void *		ps4_data_data(ps4_data_t);
const void *	ps4_data_data_nocopy(ps4_data_t);

bool		ps4_data_equals(ps4_data_t, ps4_data_t);
bool		ps4_data_equals_data(ps4_data_t, const void *, size_t);

#ifdef __cplusplus
}
#endif

#endif /* _PS4_DATA_H_ */
