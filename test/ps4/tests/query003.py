self.description = "Query search for a package"

p = pmpkg("foobar")
p.files = ["bin/foobar"]
p.groups = ["group1", "group2"]
self.addpkg2db("local", p)

self.args = "-Qs %s" % p.name

self.addrule("PS4_RETCODE=0")
self.addrule("PS4_OUTPUT=^local/%s" % p.name)
