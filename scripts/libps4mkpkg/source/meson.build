libps4mkpkg_module = 'source'

sources = [
  'bzr.sh.in',
  'file.sh.in',
  'fossil.sh.in',
  'git.sh.in',
  'hg.sh.in',
  'local.sh.in',
  'svn.sh.in',
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
