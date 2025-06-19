libps4mkpkg_module = 'executable'

sources = [
  'ccache.sh.in',
  'checksum.sh.in',
  'distcc.sh.in',
  'fakeroot.sh.in',
  'debugedit.sh.in',
  'gpg.sh.in',
  'gzip.sh.in',
  'ps4.sh.in',
  'strip.sh.in',
  'sudo.sh.in',
  'vcs.sh.in',
]

foreach src : sources
  output_dir = join_paths(get_option('datadir'), 'ps4mkpkg', libps4mkpkg_module)

  custom_target(
    libps4mkpkg_module + '_' + src.underscorify(),
    command : [ SCRIPT_EDITOR, '@INPUT@', '@OUTPUT@' ],
    input : src,
    output : '@BASENAME@',
    install : true,
    install_dir : output_dir)
endforeach
