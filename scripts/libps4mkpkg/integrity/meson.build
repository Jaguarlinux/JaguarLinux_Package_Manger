libps4mkpkg_module = 'integrity'

sources = [
  'generate_checksum.sh.in',
  'generate_signature.sh.in',
  'verify_checksum.sh.in',
  'verify_signature.sh.in',
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
