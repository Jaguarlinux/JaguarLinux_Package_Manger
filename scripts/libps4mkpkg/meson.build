libps4mkpkg_modules = [
  { 'name' : 'autodep',       'has_subdir' : true },
  { 'name' : 'buildenv',      'has_subdir' : true },
  { 'name' : 'executable',    'has_subdir' : true },
  { 'name' : 'integrity',     'has_subdir' : true },
  { 'name' : 'lint_config',   'has_subdir' : true },
  { 'name' : 'lint_package',  'has_subdir' : true },
  { 'name' : 'lint_ps4pkgbuild', 'has_subdir' : true },
  { 'name' : 'reproducible',  'has_subdir' : true },
  { 'name' : 'source',        'has_subdir' : true },
  { 'name' : 'srcinfo',                           },
  { 'name' : 'tidy',          'has_subdir' : true },
  { 'name' : 'util',          'has_subdir' : true },
]

foreach module : libps4mkpkg_modules
  custom_target(
    'libps4mkpkg_@0@'.format(module['name']),
    command : [ SCRIPT_EDITOR, '@INPUT@', '@OUTPUT@' ],
    input : '@0@.sh.in'.format(module['name']),
    output : '@BASENAME@',
    install : true,
    install_dir : join_paths(get_option('datadir'), 'ps4mkpkg'))

  if module.get('has_subdir', false)
    subdir(module['name'])
  endif
endforeach
