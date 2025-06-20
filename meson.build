project('ps4',
        'c',
        version : '1.0.0B',
        license : 'GPLv2+',
        default_options : [
          'c_std=gnu99',
          'prefix=/usr',
          'sysconfdir=/etc',
          'localstatedir=/var',
        ],
        meson_version : '>= 0.61')

libalpm_version = '15.0.0'

cc = meson.get_compiler('c')

# commandline options
PREFIX = get_option('prefix')
DATAROOTDIR = join_paths(PREFIX, get_option('datarootdir'))
SYSCONFDIR = join_paths(PREFIX, get_option('sysconfdir'))
LOCALSTATEDIR = join_paths(PREFIX, get_option('localstatedir'))
LOCALEDIR = join_paths(PREFIX, get_option('localedir'))
ROOTDIR = get_option('root-dir')
BINDIR = join_paths(PREFIX, get_option('bindir'))
MANDIR = join_paths(PREFIX, get_option('mandir'))
BUILDSCRIPT = get_option('buildscript')
LIBPS4MKPKGDIR = join_paths(PREFIX, DATAROOTDIR, 'ps4mkpkg')
PKGDATADIR = join_paths(PREFIX, DATAROOTDIR, meson.project_name())
KEYRINGDIR = join_paths(PREFIX, DATAROOTDIR, get_option('keyringdir'))

PYTHON = import('python').find_installation('python3')
LDCONFIG = get_option('ldconfig')
MESON_MAKE_SYMLINK = join_paths(meson.project_source_root(), 'build-aux/meson-make-symlink.sh')

BASH = find_program('bash4', 'bash')
if BASH.found()
  bash_version = run_command(BASH, '-c', 'IFS=.; echo "${BASH_VERSINFO[*]:0:3}"', check: true).stdout()

  have_bash = bash_version.version_compare('>= 4.4.0')
endif
if not have_bash
  error('bash >= 4.4.0 is required for ps4 scripts.')
endif

bashcompletion = dependency('bash-completion', required : false)
if bashcompletion.found()
  BASHCOMPDIR = bashcompletion.get_variable(pkgconfig: 'completionsdir')
else
  BASHCOMPDIR = join_paths(DATAROOTDIR, 'bash-completion/completions')
endif

if get_option('use-git-version')
  PACKAGE_VERSION = run_command(
      find_program('git'),
      'describe',
      '--abbrev=4',
      '--dirty',
      check: true).stdout().strip().strip('v')
else
  PACKAGE_VERSION = meson.project_version()
endif

conf = configuration_data()
conf.set('_GNU_SOURCE', true)
conf.set_quoted('PACKAGE',  meson.project_name())
conf.set_quoted('PACKAGE_VERSION', PACKAGE_VERSION)
conf.set_quoted('LOCALEDIR', LOCALEDIR)
conf.set_quoted('SCRIPTLET_SHELL', get_option('scriptlet-shell'))
conf.set_quoted('LDCONFIG', LDCONFIG)
conf.set_quoted('LIB_VERSION', libalpm_version)
conf.set_quoted('SYSHOOKDIR', join_paths(DATAROOTDIR, 'libalpm/hooks/'))
conf.set_quoted('CONFFILE', join_paths(SYSCONFDIR, 'ps4.conf'))
conf.set_quoted('DBPATH', join_paths(LOCALSTATEDIR, 'lib/ps4/'))
conf.set_quoted('GPGDIR', join_paths(SYSCONFDIR, 'ps4.d/gnupg/'))
conf.set_quoted('LOGFILE', join_paths(LOCALSTATEDIR, 'log/ps4.log'))
conf.set_quoted('CACHEDIR', join_paths(LOCALSTATEDIR, 'cache/ps4/pkg/'))
conf.set_quoted('HOOKDIR', join_paths(SYSCONFDIR, 'ps4.d/hooks/'))
conf.set_quoted('ROOTDIR', ROOTDIR)

libintl = dependency('', required: false)
if get_option('i18n')
  libintl = dependency('intl', static: get_option('buildstatic'))
  conf.set('ENABLE_NLS', 1)
endif

# dependencies
libarchive = dependency('libarchive',
                        version : '>=3.0.0',
                        static : get_option('buildstatic'))
if libarchive.version().version_compare('>=3.6.0')
  bsdtar_no_read_sparse = '--no-read-sparse'
else
  bsdtar_no_read_sparse = ''
endif

libcurl = dependency('libcurl',
                     version : '>=7.55.0',
                     required : get_option('curl'),
                     static : get_option('buildstatic'))
conf.set('HAVE_LIBCURL', libcurl.found())

needed_gpgme_version = '>=1.12.0'
gpgme = dependency('gpgme',
                   version : needed_gpgme_version,
                   required : get_option('gpgme'),
                   static : get_option('buildstatic'),
                   not_found_message : 'gpgme @0@ is needed for GPG signature support'.format(needed_gpgme_version))
conf.set('HAVE_LIBGPGME', gpgme.found())

want_crypto = get_option('crypto')
if want_crypto == 'openssl'
  libcrypto = dependency('libcrypto', static : get_option('buildstatic'),
    not_found_message : 'openssl support requested but not found')
  crypto_provider = libcrypto
  conf.set10('HAVE_LIBSSL', true)
elif want_crypto == 'nettle'
  libnettle = dependency('nettle', static : get_option('buildstatic'),
    not_found_message : 'nettle support requested but not found')
  crypto_provider = libnettle
  conf.set10('HAVE_LIBNETTLE', true)
else
  error('unhandled crypto value @0@'.format(want_crypto))
endif

libseccomp = dependency('libseccomp',
                        static : get_option('buildstatic'),
                        required : false)
conf.set('HAVE_LIBSECCOMP', libseccomp.found())
foreach header : [
    'linux/landlock.h',
    'mntent.h',
    'sys/mnttab.h',
    'sys/mount.h',
    'sys/param.h',
    'sys/prctl.h',
    'sys/statvfs.h',
    'sys/types.h',
    'sys/ucred.h',
    'termios.h',
  ]
  if cc.has_header(header)
    conf.set('HAVE_' + header.underscorify().to_upper(), true)
  endif
endforeach

foreach sym : [
    'getmntent',
    'getmntinfo',
    'strndup',
    'strnlen',
    'strsep',
    'swprintf',
    'tcflush',
  ]
  have = cc.has_function(sym, args : '-D_GNU_SOURCE')
  if have
    conf.set10('HAVE_' + sym.to_upper(), have)
  endif
endforeach

foreach member : [
    ['struct stat', 'st_blksize', '''#include <sys/stat.h>'''],
    ['struct statvfs', 'f_flag', '''#include <sys/statvfs.h>'''],
    ['struct statfs', 'f_flags', '''#include <sys/param.h>
                                    #include <sys/mount.h>'''],
  ]
  have = cc.has_member(member[0], member[1], prefix : member[2])
  conf.set('HAVE_' + '_'.join([member[0], member[1]]).underscorify().to_upper(), have)
endforeach

foreach type : [
    # type       # program prefix              # fallback
    ['mode_t',   '''#include <sys/types.h>''', 'unsigned int'],
    ['uid_t',    '''#include <sys/types.h>''', 'unsigned int'],
    ['off_t',    '''#include <sys/types.h>''', 'signed int'],
    ['pid_t',    '''#include <sys/types.h>''', 'signed int'],
    ['size_t',   '''#include <sys/types.h>''', 'unsigned int'],
    ['ssize_t',  '''#include <sys/types.h>''', 'signed int'],
    ['int64_t',  '''#include <stdint.h>''',    'signed long int'],
  ]
  if not cc.has_type(type[0], prefix: type[1])
    conf.set(type[0], type[2])
  endif
endforeach


if conf.has('HAVE_GETMNTINFO')
  if cc.compiles('''
      #include <sys/types.h>
      #include <sys/statvfs.h>

      void func() {
        struct statvfs *buf;
        getmntinfo(&buf, 0);
      }
      ''')
      conf.set('FSSTATSTYPE', 'struct statvfs')
  else
      conf.set('FSSTATSTYPE', 'struct statfs')
  endif
elif conf.has('HAVE_STRUCT_STATVFS_F_FLAG')
  conf.set('FSSTATSTYPE', 'struct statvfs')
elif conf.has('HAVE_STRUCT_STATFS_F_FLAGS')
  conf.set('FSSTATSTYPE', 'struct statfs')
endif

if get_option('debug')
  extra_cflags = [
    '-pedantic',
    '-Wcast-align',
    '-Wclobbered',
    '-Wempty-body',
    '-Wfloat-equal',
    '-Wformat-nonliteral',
    '-Wformat-security',
    '-Wignored-qualifiers',
    '-Wimplicit-fallthrough',
    '-Winit-self',
    '-Wlogical-op',
    '-Wmissing-declarations',
    '-Wmissing-field-initializers',
    '-Wmissing-parameter-type',
    '-Wmissing-prototypes',
    '-Wold-style-declaration',
    '-Woverride-init',
    '-Wpointer-arith',
    '-Wredundant-decls',
    '-Wshadow',
    '-Wsign-compare',
    '-Wstrict-aliasing',
    '-Wstrict-overflow=5',
    '-Wstrict-prototypes',
    '-Wtype-limits',
    '-Wuninitialized',
    '-Wunused-but-set-parameter',
    '-Wunused-parameter',
    '-Wunused-result',
    '-Wwrite-strings',
  ]
  add_project_arguments(cc.get_supported_arguments(extra_cflags), language : 'c')

  conf.set('PS4_DEBUG', 1)
endif

config_h = configure_file(
        output : 'config.h',
        configuration : conf)
add_project_arguments('-include', 'config.h', language : 'c')

filecmd = 'file'
inodecmd = 'stat -c \'%i %n\''
strip_binaries = '--strip-all'
strip_shared = '--strip-unneeded'
strip_static = '--strip-debug'

file_seccomp = get_option('file-seccomp')
# meson-git has find_program('file', required: false, version: '>=5.38')
filever = run_command('sh', '-c', 'file --version | sed -n "s/^file-\(.*\)/\\1/p"', check: true).stdout()
if file_seccomp.enabled() or ( file_seccomp.auto() and filever.version_compare('>= 5.38') )
  filecmd = 'file -S'
endif

os = host_machine.system()
if os.startswith('darwin')
  inodecmd = '/usr/bin/stat -f \'%i %N\''
  strip_binaries = ''
  strip_shared = '-s'
  strip_static = '-s'
elif os.contains('bsd') or os == 'dragonfly'
  inodecmd = 'stat -f \'%i %N\''
endif

chost = run_command(cc, '-dumpmachine', check: true).stdout().strip()
carch = chost.split('-')[0]

# annoyingly, we have to maintain two sets of configuration_data which is
# largely identical, but which distinguishes between quoting needs.
substs = configuration_data()
substs.set('CARCH', carch)
substs.set('CHOST', chost)
substs.set('PKGEXT', get_option('pkg-ext'))
substs.set('SRCEXT', get_option('src-ext'))
substs.set('ROOTDIR', ROOTDIR)
substs.set('LOCALEDIR', LOCALEDIR)
substs.set('sysconfdir', SYSCONFDIR)
substs.set('localstatedir', LOCALSTATEDIR)
substs.set('PKGDATADIR', PKGDATADIR)
substs.set('KEYRINGDIR', KEYRINGDIR)
substs.set('PREFIX', PREFIX)
substs.set('BASH', BASH.full_path())
substs.set('PACKAGE_VERSION', PACKAGE_VERSION)
substs.set('PACKAGE_NAME', meson.project_name())
substs.set('BUILDSCRIPT', BUILDSCRIPT)
substs.set('TEMPLATE_DIR', get_option('ps4mkpkg-template-dir'))
substs.set('DEBUGSUFFIX', get_option('debug-suffix'))
substs.set('INODECMD', inodecmd)
substs.set('FILECMD', filecmd)
substs.set('LIBPS4MKPKGDIR', LIBPS4MKPKGDIR)
substs.set('STRIP_BINARIES', strip_binaries)
substs.set('STRIP_SHARED', strip_shared)
substs.set('STRIP_STATIC', strip_static)
substs.set('BSDTAR_NO_READ_SPARSE', bsdtar_no_read_sparse)

subdir('lib/libalpm')
subdir('src/common')
subdir('src/ps4')
subdir('src/util')
subdir('scripts')

# Internationalization
if get_option('i18n')
  i18n = import('i18n')
  subdir('lib/libalpm/po')
  subdir('src/ps4/po')
  subdir('scripts/po')
endif

want_doc = get_option('doc')
ASCIIDOC = find_program('asciidoc', required : want_doc)
A2X = find_program('a2x', required : want_doc)
build_doc = A2X.found() and not want_doc.disabled()
if build_doc
  subdir('doc')
endif

includes = include_directories('src/common', 'lib/libalpm')

libcommon = static_library(
  'common',
  libcommon_sources,
  include_directories : includes,
  gnu_symbol_visibility : 'hidden',
  install : false)

alpm_deps = [crypto_provider, libarchive, libcurl, libintl, libseccomp, gpgme]

libalpm_a = static_library(
  'alpm_objlib',
  libalpm_sources,
  # https://github.com/mesonbuild/meson/issues/3937
  objects : libcommon.extract_all_objects(recursive: true),
  include_directories : includes,
  gnu_symbol_visibility : 'hidden',
  dependencies : alpm_deps)

libalpm = library(
  'alpm',
  version : libalpm_version,
  objects: libalpm_a.extract_all_objects(recursive: true),
  include_directories : includes,
  dependencies : alpm_deps,
  install : true)

install_headers(
  'lib/libalpm/alpm.h',
  'lib/libalpm/alpm_list.h')

pkgconfig = import('pkgconfig')
pkgconfig.generate(
  libalpm,
  name : 'libalpm',
  description : 'ps4 is a fork of  Arch Linux package management library',
  version : libalpm_version,
  url : 'https://github.com/Jaguarlinux/JaguarLinux_Package_Manger',
  extra_cflags: [ '-D_FILE_OFFSET_BITS=64' ],
)

ps4_bin = executable(
  'ps4',
  ps4_sources,
  include_directories : includes,
  link_with : [libalpm, libcommon],
  dependencies : [libarchive],
  install : true,
)

executable(
  'ps4-conf',
  ps4_conf_sources,
  include_directories : includes,
  link_with : [libalpm, libcommon],
  dependencies : [libarchive],
  install : true,
)

executable(
  'testpkg',
  testpkg_sources,
  include_directories : includes,
  link_with : [libalpm],
  dependencies : [libarchive],
  install : true,
)

executable(
  'vercmp',
  vercmp_sources,
  include_directories : includes,
  link_with : [libalpm_a],
  install : true,
)

foreach wrapper : script_wrappers
  cdata = configuration_data()
  cdata.set_quoted('BASH', BASH.full_path())
  cdata.set_quoted('BUILDDIR', wrapper[2])
  cdata.set_quoted('REAL_PROGPATH', wrapper[1].full_path())

  # Create a wrapper script that bootstraps the real script within the build
  # directory. Use configure_file instead of a custom_target to ensure that
  # permissions on the input script wrapper are preserved.
  configure_file(
    input : join_paths(meson.project_source_root(), 'build-aux', 'script-wrapper.sh.in'),
    output : wrapper[0],
    configuration : cdata)
endforeach

configure_file(
  input : 'etc/ps4mkpkg.conf.in',
  output : 'ps4mkpkg.conf',
  configuration : substs,
  install_dir : SYSCONFDIR)

configure_file(
  input : 'etc/ps4mkpkg.conf.d/fortran.conf.in',
  output : 'fortran.conf',
  configuration : substs,
  install_dir : join_paths(SYSCONFDIR, 'ps4mkpkg.conf.d/'))

configure_file(
  input : 'etc/ps4mkpkg.conf.d/rust.conf.in',
  output : 'rust.conf',
  configuration : substs,
  install_dir : join_paths(SYSCONFDIR, 'ps4mkpkg.conf.d/'))

install_emptydir(join_paths(SYSCONFDIR, 'ps4mkpkg.d/'))

configure_file(
  input : 'etc/ps4.conf.in',
  output : 'ps4.conf',
  configuration : substs,
  install_dir : SYSCONFDIR)

install_data(
  'proto/PS4PKGBUILD-split.proto',
  'proto/PS4PKGBUILD-vcs.proto',
  'proto/PS4PKGBUILD.proto',
  'proto/proto.install',
  install_dir : join_paths(DATAROOTDIR, 'ps4'))

foreach path : [
	join_paths(LOCALSTATEDIR, 'lib/ps4/'),
	join_paths(LOCALSTATEDIR, 'cache/ps4/pkg/'),
	join_paths(DATAROOTDIR, 'ps4mkpkg-template/'),
	join_paths(DATAROOTDIR, 'libalpm/hooks/'),
	]
	meson.add_install_script('sh', '-c', 'mkdir -p "$DESTDIR/@0@"'.format(path))
endforeach

TEST_ENV = environment()
TEST_ENV.set('PMTEST_SCRIPTLIB_DIR', join_paths(meson.project_source_root(), 'scripts/library/'))
TEST_ENV.set('PMTEST_LIBPS4MKPKG_DIR', join_paths(meson.project_build_root(), 'scripts/libps4mkpkg/'))
TEST_ENV.set('PMTEST_UTIL_DIR', meson.project_build_root() + '/')
TEST_ENV.set('PMTEST_SCRIPT_DIR', join_paths(meson.project_build_root(), 'scripts/'))

subdir('test/ps4')
subdir('test/scripts')
subdir('test/util')

message('\n    '.join([
  '@0@ @1@'.format(meson.project_name(), meson.project_version()),
  'Build information:',
  '   prefix                  : @0@'.format(PREFIX),
  '   sysconfdir              : @0@'.format(SYSCONFDIR),
  '       conf file           : @0@'.format(join_paths(SYSCONFDIR, 'ps4.conf')),
  '   localstatedir           : @0@'.format(LOCALSTATEDIR),
  '       database dir        : @0@'.format(join_paths(LOCALSTATEDIR, 'lib/ps4/')),
  '       cache dir           : @0@'.format(join_paths(LOCALSTATEDIR, 'cache/ps4/pkg/')),
  '   keyringdir              : @0@'.format(KEYRINGDIR),
  '   compiler                : @0@ @1@'.format(cc.get_id(), cc.version()),
  '',
  '   Architecture            : @0@'.format(carch),
  '   Host Type               : @0@'.format(chost),
  '   File inode command      : @0@'.format(inodecmd),
  '   File seccomp command    : @0@'.format(filecmd),
  '   libalpm version         : @0@'.format(libalpm_version),
  '   ps4 version          : @0@'.format(PACKAGE_VERSION),
  '',
  'Directory and file information:',
  '   root working directory  : @0@'.format(ROOTDIR),
  '   package extension       : @0@'.format(get_option('pkg-ext')),
  '   source pkg extension    : @0@'.format(get_option('src-ext')),
  '   build script name       : @0@'.format(BUILDSCRIPT),
  '   template directory      : @0@'.format(get_option('ps4mkpkg-template-dir')),
  '',
  'Compilation options:',
  '  i18n support             : @0@'.format(get_option('i18n')),
  '  Build docs               : @0@'.format(build_doc),
  '  debug build              : @0@'.format(get_option('buildtype') == 'debug'),
  '  Use libcurl              : @0@'.format(conf.get('HAVE_LIBCURL')),
  '  Use GPGME                : @0@'.format(conf.get('HAVE_LIBGPGME')),
  '  Use OpenSSL              : @0@'.format(conf.has('HAVE_LIBSSL') and
                                            conf.get('HAVE_LIBSSL') == 1),
  '  Use nettle               : @0@'.format(conf.has('HAVE_LIBNETTLE') and
                                            conf.get('HAVE_LIBNETTLE') == 1),
  '',
]))
