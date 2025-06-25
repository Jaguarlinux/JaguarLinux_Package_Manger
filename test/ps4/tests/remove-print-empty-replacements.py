self.description = "Print empty replacements when using -Rp"

p = pmpkg("a")
self.addpkg2db("local", p)

self.args = "-Rp --print-format 'foo%%f%%g%%h%%m' %s" % p.name

self.addrule("PS4_RETCODE=0")
self.addrule("PS4_OUTPUT=^foo$")
