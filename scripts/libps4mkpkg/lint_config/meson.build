libps4mkpkg_module = 'lint_config'

sources = [
  'ext.sh.in',
  'nproc.sh.in',
  'packager.sh.in',
  'paths.sh.in',
  'source_date_epoch.sh.in',
  'variable.sh.in',
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
