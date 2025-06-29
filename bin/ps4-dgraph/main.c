/*-
 * Copyright (c) 2010-2019 Juan Romero Pardines.
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
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdarg.h>
#include <unistd.h>
#include <inttypes.h>
#include <assert.h>
#include <getopt.h>

#include <ps4.h>
#include "queue.h"

#ifdef __clang__
#pragma clang diagnostic ignored "-Wformat-nonliteral"
#endif

#ifndef __arraycount
# define __arraycount(a) (sizeof(a) / sizeof(*(a)))
#endif

#define _DGRAPH_CFFILE	"ps4-dgraph.conf"

/*
 * Properties written to default configuration file.
 */
struct defprops {
	const char *sect;
	const char *prop;
	const char *val;
} dfprops[] = {
	{ .sect = "graph", .prop = "rankdir", .val = "LR" },
	{ .sect = "graph", .prop = "ranksep", .val = ".1" },
	{ .sect = "graph", .prop = "nodesep", .val = ".1" },
	{ .sect = "graph", .prop = "splines", .val = "polyline" },
	{ .sect = "graph", .prop = "ratio", .val = "compress" },

	{ .sect = "edge", .prop = "constraint", .val = "true" },
	{ .sect = "edge", .prop = "arrowhead", .val = "vee" },
	{ .sect = "edge", .prop = "arrowsize", .val = ".4" },
	{ .sect = "edge", .prop = "fontname", .val = "Sans" },
	{ .sect = "edge", .prop = "fontsize", .val = "8" },

	{ .sect = "node", .prop = "height", .val = ".1" },
	{ .sect = "node", .prop = "width", .val = ".1" },
	{ .sect = "node", .prop = "shape", .val = "ellipse" },
	{ .sect = "node", .prop = "fontname", .val = "Sans" },
	{ .sect = "node", .prop = "fontsize", .val = "8" },

	{ .sect = "node-sub", .prop = "main-style", .val = "filled" },
	{ .sect = "node-sub", .prop = "main-fillcolor", .val = "darksalmon" },
	{ .sect = "node-sub", .prop = "style", .val = "filled" },
	{ .sect = "node-sub", .prop = "fillcolor", .val = "yellowgreen" },
	{ .sect = "node-sub", .prop = "opt-style", .val = "filled" },
	{ .sect = "node-sub", .prop = "opt-fillcolor", .val = "grey" }
};

struct pkgdep {
	SLIST_ENTRY(pkgdep) pkgdep_entries;
	unsigned int idx;
	const char *pkgver;
	ps4_array_t provides;
};

static ps4_dictionary_t confd;
static SLIST_HEAD(pkgdep_head, pkgdep) pkgdep_list =
    SLIST_HEAD_INITIALIZER(pkgdep_list);

static void __attribute__((noreturn))
die(const char *fmt, ...)
{
	va_list ap;
	int save_errno = errno;

	va_start(ap, fmt);
	fprintf(stderr, "ps4-dgraph: ERROR ");
	vfprintf(stderr, fmt, ap);
	if (save_errno)
		fprintf(stderr, " (%s)\n", strerror(save_errno));
	else
		fprintf(stderr, "\n");
	va_end(ap);
	exit(EXIT_FAILURE);
}

static void __attribute__((noreturn))
usage(bool fail)
{
	fprintf(stdout,
	"Usage: ps4-dgraph [OPTIONS] [MODE] <pkgname>\n\n"
	"OPTIONS\n"
	" -C, --config <dir>        Path to confdir (ps4.d)\n"
	" -c, --graph-config <file> Path to the graph configuration file\n"
	" -d, --debug               Debug mode shown to stderr\n"
	" -h, --help                Show usage\n"
	" -M, --memory-sync         Remote repository data is fetched and stored\n"
	"                           in memory, ignoring on-disk repodata archives.\n"
	" -r, --rootdir <dir>       Full path to rootdir\n"
	" -R, --repository          Enable repository mode. This mode explicitly\n"
	"                           looks for packages in repositories.\n"
	"MODE\n"
	" -g, --gen-config          Generate a configuration file\n"
	" -f, --fulldeptree         Generate a dependency graph\n"
	" -m, --metadata            Generate a metadata graph (default mode)\n");
	exit(fail ? EXIT_FAILURE : EXIT_SUCCESS);
}

static const char *
convert_proptype_to_string(ps4_object_t obj)
{
	switch (ps4_object_type(obj)) {
	case PS4_TYPE_ARRAY:
		return "array";
	case PS4_TYPE_BOOL:
		return "bool";
	case PS4_TYPE_DICTIONARY:
		return "dictionary";
	case PS4_TYPE_DICT_KEYSYM:
		return "dictionary key";
	case PS4_TYPE_NUMBER:
		return "integer";
	case PS4_TYPE_STRING:
		return "string";
	case PS4_TYPE_DATA:
		return "data";
	default:
		return NULL;
	}
}

static ps4_dictionary_t
create_defconf(void)
{
	ps4_dictionary_t d, d2;
	struct defprops *dfp;

	d = ps4_dictionary_create();

	d2 = ps4_dictionary_create();
	ps4_dictionary_set(d, "graph", d2);
	ps4_object_release(d2);

	d2 = ps4_dictionary_create();
	ps4_dictionary_set(d, "edge", d2);
	ps4_object_release(d2);

	d2 = ps4_dictionary_create();
	ps4_dictionary_set(d, "node", d2);
	ps4_object_release(d2);

	d2 = ps4_dictionary_create();
	ps4_dictionary_set(d, "node-sub", d2);
	ps4_object_release(d2);

	for (unsigned int i = 0; i < __arraycount(dfprops); i++) {
		dfp = &dfprops[i];
		d2 = ps4_dictionary_get(d, dfp->sect);
		ps4_dictionary_set_cstring_nocopy(d2, dfp->prop, dfp->val);
	}

	return d;
}

static void
generate_conf_file(void)
{
	ps4_dictionary_t d;

	d = create_defconf();
	if (ps4_dictionary_externalize_to_file(d, _DGRAPH_CFFILE) == false) {
		ps4_object_release(d);
		die("couldn't write conf_file to %s", _DGRAPH_CFFILE);
	}
	ps4_object_release(d);
	printf("Wrote configuration file: %s\n", _DGRAPH_CFFILE);
}

static void
write_conf_property_on_stream(FILE *f, const char *section)
{
	ps4_array_t allkeys, allkeys2;
	ps4_dictionary_keysym_t dksym, dksym2;
	ps4_object_t keyobj, keyobj2;
	const char *cf_val, *keyname, *keyname2;

	/*
	 * Iterate over the main dictionary.
	 */
	allkeys = ps4_dictionary_all_keys(confd);
	for (unsigned int i = 0; i < ps4_array_count(allkeys); i++) {
		dksym = ps4_array_get(allkeys, i);
		keyname = ps4_dictionary_keysym_cstring_nocopy(dksym);
		keyobj = ps4_dictionary_get_keysym(confd, dksym);
		if (strcmp(keyname, section))
			continue;

		/*
		 * Iterate over the dictionary sections [edge/graph/node].
		 */
		allkeys2 = ps4_dictionary_all_keys(keyobj);
		for (unsigned int x = 0; x < ps4_array_count(allkeys2); x++) {
			dksym2 = ps4_array_get(allkeys2, x);
			keyname2 = ps4_dictionary_keysym_cstring_nocopy(dksym2);
			keyobj2 = ps4_dictionary_get_keysym(keyobj, dksym2);

			cf_val = ps4_string_cstring_nocopy(keyobj2);
			fprintf(f, "%s=\"%s\"", keyname2, cf_val);
			if (x + 1 >= ps4_array_count(allkeys2))
				continue;

			fprintf(f, ",");
		}
	}
}

static char *
strip_dashes_from_key(const char *str)
{
	char *p;
	size_t i;

	p = strdup(str);
	if (p == NULL)
		die("%s alloc p", __func__);

	for (i = 0; i < strlen(p); i++) {
		if (p[i] == '-')
			p[i] = '_';
	}
	return p;
}

static void
parse_array_in_pkg_dictionary(FILE *f, ps4_dictionary_t plistd,
			      ps4_dictionary_t sub_confd,
			      ps4_array_t allkeys)
{
	ps4_dictionary_keysym_t dksym;
	ps4_object_t keyobj, sub_keyobj;
	const char *tmpkeyname, *cfprop = NULL;
	char *keyname;

	for (unsigned int i = 0; i < ps4_array_count(allkeys); i++) {
		dksym = ps4_array_get(allkeys, i);
		tmpkeyname = ps4_dictionary_keysym_cstring_nocopy(dksym);
		keyobj = ps4_dictionary_get_keysym(plistd, dksym);
		keyname = strip_dashes_from_key(tmpkeyname);

		fprintf(f, "	main -> %s [label=\"%s\"];\n",
		    keyname, convert_proptype_to_string(keyobj));

		/*
		 * Process array objects.
		 */
		if (ps4_object_type(keyobj) == ps4_TYPE_ARRAY) {
			for (unsigned int x = 0; x < ps4_array_count(keyobj); x++) {
				sub_keyobj = ps4_array_get(keyobj, x);
				if (ps4_object_type(sub_keyobj) == ps4_TYPE_STRING) {
					/*
					 * Process arrays of strings.
					 */
					fprintf(f, "	%s -> %s_%u_string "
					    "[label=\"string\"];\n",
					    keyname, keyname, x);
					ps4_dictionary_get_cstring_nocopy(sub_confd,
					    "style", &cfprop);
					fprintf(f, "	%s_%u_string [style=\"%s\",",
					    keyname, x, cfprop);
					ps4_dictionary_get_cstring_nocopy(sub_confd,
					    "fillcolor", &cfprop);
					fprintf(f, "fillcolor=\"%s\","
					    "label=\"%s\"];\n", cfprop,
					    ps4_string_cstring_nocopy(sub_keyobj));
				}
			}
			free(keyname);
			continue;
		}
		fprintf(f, "	%s -> %s_value;\n", keyname, keyname);
		ps4_dictionary_get_cstring_nocopy(sub_confd, "style", &cfprop);
		fprintf(f, "	%s_value [style=\"%s\",", keyname, cfprop);
		ps4_dictionary_get_cstring_nocopy(sub_confd,
		    "fillcolor", &cfprop);
		fprintf(f, "fillcolor=\"%s\"", cfprop);

		/*
		 * Process all other object types...
		 */
		switch (ps4_object_type(keyobj)) {
		case PS4_TYPE_BOOL:
			fprintf(f, ",label=\"%s\"",
			    ps4_bool_true(keyobj) ? "true" : "false");
			break;
		case PS4_TYPE_DATA:
			fprintf(f, ",label=\"%zu bytes\"", ps4_data_size(keyobj));
			break;
		case ps4_TYPE_NUMBER:
			fprintf(f, ",label=\"%"PRIu64" bytes\"",
			    ps4_number_unsigned_integer_value(keyobj));
			break;
		case PS4_TYPE_STRING:
			fprintf(f, ",label=\"%s\"", ps4_string_cstring_nocopy(keyobj));
			break;
		default:
			break;
		}
		fprintf(f, "];\n");
		free(keyname);
	}
}

static void
process_fulldeptree(struct ps4_handle *xhp, FILE *f,
		ps4_dictionary_t pkgd, ps4_array_t rdeps,
		bool repomode)
{
	ps4_array_t rpkgrdeps;
	struct pkgdep *pd;
	const char *pkgver;
	unsigned int i, x;

	ps4_dictionary_get_cstring_nocopy(pkgd, "pkgver", &pkgver);

	i = ps4_array_count(rdeps);
	while (i--) {
		ps4_dictionary_t rpkgd;
		const char *pkgdep = NULL;
		unsigned int pkgidx = 0;
		bool found = false;

		ps4_array_get_cstring_nocopy(rdeps, i, &pkgdep);
		if (strcmp(pkgdep, pkgver) == 0)
			continue;

		SLIST_FOREACH(pd, &pkgdep_list, pkgdep_entries) {
			if (strcmp(pd->pkgver, pkgdep) == 0) {
				found = true;
				break;
			}
			pkgidx++;
		}
		if (repomode) {
			rpkgd = ps4_rpool_get_pkg(xhp, pkgdep);
		} else {
			rpkgd = ps4_pkgdb_get_pkg(xhp, pkgdep);
		}
		assert(rpkgd);
		if (!found) {
			pd = malloc(sizeof(*pd));
			assert(pd);
			pd->idx = pkgidx;
			pd->pkgver = pkgdep;
			pd->provides = ps4_dictionary_get(rpkgd, "provides");
			SLIST_INSERT_HEAD(&pkgdep_list, pd, pkgdep_entries);
		}
		rpkgrdeps = ps4_dictionary_get(rpkgd, "run_depends");
		for (x = 0; x < ps4_array_count(rpkgrdeps); x++) {
			struct pkgdep *ppd;
			const char *rpkgdep = NULL;

			ps4_array_get_cstring_nocopy(rpkgrdeps, x, &rpkgdep);
			SLIST_FOREACH(ppd, &pkgdep_list, pkgdep_entries) {
				if (ps4_pkgpattern_match(ppd->pkgver, rpkgdep))
					fprintf(f, "\t%u -> %u;\n", pkgidx, ppd->idx);
				else if (ppd->provides &&
					ps4_match_virtual_pkg_in_array(ppd->provides, rpkgdep))
					fprintf(f, "\t%u -> %u;\n", pkgidx, ppd->idx);
			}
		}
		fprintf(f, "\t%u [label=\"%s\"", pkgidx, pkgdep);
		if (repomode && ps4_pkgdb_get_pkg(xhp, pkgdep))
			fprintf(f, ",style=\"filled\",fillcolor=\"yellowgreen\"");

		fprintf(f, "]\n");
	}
	i = 0;
	SLIST_FOREACH(pd, &pkgdep_list, pkgdep_entries)
		++i;

	fprintf(f, "\t%u [label=\"%s\",style=\"filled\",fillcolor=\"darksalmon\"];\n", i, pkgver);
	rpkgrdeps = ps4_dictionary_get(pkgd, "run_depends");
	for (x = 0; x < ps4_array_count(rpkgrdeps); x++) {
		const char *rpkgdep = NULL;

		ps4_array_get_cstring_nocopy(rpkgrdeps, x, &rpkgdep);
		SLIST_FOREACH(pd, &pkgdep_list, pkgdep_entries) {
			if (ps4_pkgpattern_match(pd->pkgver, rpkgdep))
				fprintf(f, "\t%u -> %u;\n", i, pd->idx);
			else if (pd->provides &&
				ps4_match_virtual_pkg_in_array(pd->provides, rpkgdep))
				fprintf(f, "\t%u -> %u;\n", i, pd->idx);
		}
	}
}

static void
create_dot_graph(struct ps4_handle *xhp,
		 FILE *f,
		 ps4_dictionary_t plistd,
		 bool repomode,
		 bool fulldepgraph)
{
	ps4_dictionary_t sub_confd;
	ps4_array_t allkeys, rdeps;
	const char *pkgver = NULL, *cfprop;

	ps4_dictionary_get_cstring_nocopy(plistd, "pkgver", &pkgver);

	/*
	 * Start filling the output file...
	 */
	fprintf(f, "/* Graph created for %s by ps4-graph %s */\n\n",
	    pkgver, PS4_RELVER);
	fprintf(f, "digraph pkg_dictionary {\n");

	/*
	 * Process the graph section in config file.
	 */
	fprintf(f, "	graph [");
	write_conf_property_on_stream(f, "graph");
	if (fulldepgraph)
		fprintf(f, ",label=\"[ps4] %s full dependency graph "
		    "[%s]\"];\n", pkgver, repomode ? "repo" : "pkgdb");
	else
		fprintf(f, ",label=\"[ps4] %s metadata properties\"];\n", pkgver);

	/*
	 * Process the edge section in config file.
	 */
	fprintf(f, "	edge [");
	write_conf_property_on_stream(f, "edge");
	fprintf(f, "];\n");

	/*
	 * Process the node section in config file.
	 */
	fprintf(f, "	node [");
	write_conf_property_on_stream(f, "node");
	fprintf(f, "];\n");

	if (fulldepgraph) {
		if (repomode) {
			rdeps = ps4_rpool_get_pkg_fulldeptree(xhp, pkgver);
		} else {
			rdeps = ps4_pkgdb_get_pkg_fulldeptree(xhp, pkgver);
		}
		if (rdeps == NULL) {
			if (errno == ENODEV)
				die("package depends on missing dependencies\n");
			else
				die("package dependencies couldn't be resolved (error %d)\n", errno);
		}

		process_fulldeptree(xhp, f, plistd, rdeps, repomode);
	} else {
		/*
		 * Process the node-sub section in config file.
		 */
		fprintf(f, "	main [");
		sub_confd = ps4_dictionary_get(confd, "node-sub");
		if (ps4_dictionary_get_cstring_nocopy(sub_confd, "main-style", &cfprop))
			fprintf(f, "style=%s,", cfprop);
		if (ps4_dictionary_get_cstring_nocopy(sub_confd, "main-fillcolor", &cfprop))
			fprintf(f, "fillcolor=\"%s\",", cfprop);

		fprintf(f, "label=\"Dictionary\"];\n");
		if (repomode) {
			rdeps = ps4_rpool_get_pkg_revdeps(xhp, pkgver);
		} else {
			rdeps = ps4_pkgdb_get_pkg_revdeps(xhp, pkgver);
		}
		if (ps4_array_count(rdeps))
			ps4_dictionary_set(plistd, "requiredby", rdeps);

		allkeys = ps4_dictionary_all_keys(plistd);
		parse_array_in_pkg_dictionary(f, plistd, sub_confd, allkeys);
	}
	/*
	 * Terminate the stream...
	 */
	fprintf(f, "}\n");
	fflush(f);
	fclose(f);
}

int
main(int argc, char **argv)
{
	const char *shortopts = "C:c:dfghMmRr:V";
	const struct option longopts[] = {
		{ "config", required_argument, NULL, 'C' },
		{ "graph-config", required_argument, NULL, 'c' },
		{ "debug", no_argument, NULL, 'd' },
		{ "fulldeptree", no_argument, NULL, 'f' },
		{ "gen-config", no_argument, NULL, 'g' },
		{ "help", no_argument, NULL, 'h' },
		{ "memory-sync", no_argument, NULL, 'M' },
		{ "metadata", no_argument, NULL, 'm' },
		{ "repository", no_argument, NULL, 'R' },
		{ "rootdir", required_argument, NULL, 'r' },
		{ "version", no_argument, NULL, 'V' },
		{ NULL, 0, NULL, 0 },
	};
	ps4_dictionary_t plistd = NULL;
	struct ps4_handle xh;
	FILE *f = NULL;
	const char *pkg, *confdir, *conf_file, *rootdir;
	int c, rv, flags = 0;
	bool opmode, repomode, fulldepgraph;

	pkg = confdir = conf_file = rootdir = NULL;
	opmode = repomode = fulldepgraph = false;

	while ((c = getopt_long(argc, argv, shortopts, longopts, NULL)) != -1) {
		switch (c) {
		case 'C':
			/* ps4.d confdir */
			confdir = optarg;
			break;
		case 'c':
			/* Configuration file. */
			conf_file = optarg;
			break;
		case 'd':
			flags |= PS4_FLAG_DEBUG;
			break;
		case 'f':
			/* generate a full dependency graph */
			opmode = fulldepgraph = true;
			break;
		case 'g':
			/* Generate conf file. */
			generate_conf_file();
			exit(EXIT_SUCCESS);
		case 'h':
			usage(false);
			/* NOTREACHED */
		case 'M':
			flags |= PS4_FLAG_REPOS_MEMSYNC;
			break;
		case 'm':
			/* pkgdb metadata mode */
			opmode = true;
			break;
		case 'R':
			/* enable repository mode */
			opmode = repomode = true;
			break;
		case 'r':
			/* Set different rootdir. */
			rootdir = optarg;
			break;
		case 'v':
			printf("%s\n", PS4_RELVER);
			exit(EXIT_SUCCESS);
		case '?':
		default:
			usage(true);
			/* NOTREACHED */
		}
	}

	argc -= optind;
	argv += optind;

	if (!argc && !opmode) {
		usage(true);
		/* NOTREACHED */
	}
	pkg = *argv;
	if (!pkg) {
		usage(true);
		/* NOTREACHED */
	}

	/* Initialize libps4 */
	memset(&xh, 0, sizeof(xh));
	if (rootdir != NULL)
		ps4_strlcpy(xh.rootdir, rootdir, sizeof(xh.rootdir));
	if (confdir)
		ps4_strlcpy(xh.confdir, confdir, sizeof(xh.confdir));

	xh.flags = flags;
	if ((rv = ps4_init(&xh)) != 0)
		die("failed to initialize libps4: %s", strerror(rv));

	/*
	 * If -c not set and config file does not exist, use defaults.
	 */
	if (conf_file == NULL)
		conf_file = _DGRAPH_CFFILE;

	confd = ps4_plist_dictionary_from_file(conf_file);
	if (confd == NULL) {
		if (errno != ENOENT)
			die("cannot read conf file `%s'", conf_file);

		confd = create_defconf();
	}
	/*
	 * Internalize the plist file of the target installed package.
	 */
	if (repomode) {
		plistd = ps4_rpool_get_pkg(&xh, pkg);
	} else {
		plistd = ps4_pkgdb_get_pkg(&xh, pkg);
	}
	if (plistd == NULL)
		die("cannot find `%s' package", pkg);

	/*
	 * Create the output FILE.
	 */
	if ((f = fdopen(STDOUT_FILENO, "w")) == NULL)
		die("cannot open stdout");

	/*
	 * Create the dot(1) graph!
	 */
	create_dot_graph(&xh, f, plistd, repomode, fulldepgraph);
	exit(EXIT_SUCCESS);
}
