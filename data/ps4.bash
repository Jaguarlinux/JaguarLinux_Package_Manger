_ps4_parse_help() {
	local IFS line word

	$1 --help 2>&1 | while IFS=$'\n' read -r line; do
		[[ $line == *([ $'\t'])-* ]] || continue

		IFS=$' \t,='
		for word in $line; do
			[[ $word == -* ]] || continue
			printf -- '%s\n' $word
		done
	done | sort | uniq
}

_ps4_all_packages() {
	ps4-query -Rs "$1*" | sed 's/^... \([^ ]*\)-.* .*/\1/'
}

_ps4_installed_packages() {
	ps4-query -l | sed 's/^.. \([^ ]*\)-.* .*/\1/'
}

_ps4_all_reply() {
	COMPREPLY=( $( compgen -W '$(_ps4_all_packages "$1")' -- "$1") )
}

_ps4_installed_reply() {
	COMPREPLY=( $( compgen -W '$(_ps4_installed_packages)' -- "$1") )
}

_ps4_complete() {
	local cur prev words cword

	_init_completion || return

	if [[ "$cur" == -* ]]; then
		COMPREPLY=( $( compgen -W '$( _ps4_parse_help "$1" )' -- "$cur") )
		return
	fi

	local common='C|-config|r|-rootdir'
	local morecommon="$common|c|-cachedir"

	local modes='auto manual hold unhold'
	local props='architecture
		archive-compression-type
		automatic-install
		build-options
		conf_files
		conflicts
		filename-sha256
		filename-size
		homepage
		install-date
		install-msg
		install-script
		installed_size
		license
		maintainer
		metafile-sha256
		packaged-with
		pkgver
		preserve
		provides
		remove-msg
		remove-script
		replaces
		repository
		shlib-provides
		shlib-requires
		short_desc
		source-revisions
		state'

	case $1 in
		ps4-dgraph)
			if [[ $prev != -@(c|o|r) ]]; then
				_ps4_installed_reply $cur
				return
			fi
			;;
		ps4-install)
			if [[ $prev != -@($morecommon) ]]; then
				_ps4_all_reply $cur
				return
			fi
			;;
		ps4-pkgdb)
			if [[ $prev == -@(m|-mode) ]]; then
				COMPREPLY=( $( compgen -W "$modes" -- "$cur") )
				return
			fi
			if [[ $prev != -@($common) ]]; then
				_ps4_installed_reply $cur
				return
			fi
			;;
		ps4-query)
			if [[ $prev == -@(p|-property) ]]; then
				COMPREPLY=( $( compgen -W "$props" -- "$cur") )
				return
			fi
			if [[ $prev != -@($morecommon|o|-ownedby) ]]; then
				local w
				for w in "${words[@]}"; do
					if [[ "$w" == -@(R|-repository) ]]; then
						_ps4_all_reply $cur
						return
					fi
				done
				_ps4_installed_reply $cur
				return
			fi
			;;
		ps4-reconfigure)
			if [[ $prev != -@($common) ]]; then
				_ps4_installed_reply $cur
				return
			fi
			;;
		ps4-remove)
			if [[ $prev != -@($morecommon) ]]; then
				_ps4_installed_reply $cur
				return
			fi
			;;
	esac

	_filedir
}

complete -F _ps4_complete ps4-checkvers ps4-create ps4-dgraph ps4-install \
	ps4-pkgdb ps4-query ps4-reconfigure ps4-remove ps4-rindex
