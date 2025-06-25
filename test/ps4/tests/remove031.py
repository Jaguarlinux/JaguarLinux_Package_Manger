self.description = "Remove a package in HoldPkg"

p1 = pmpkg("foopkg")
self.addpkg2db("local", p1)

self.option["HoldPkg"] = ["???pkg"]

self.args = "-R %s" % p1.name

self.addrule("PS4_RETCODE=0")
self.addrule("PKG_EXIST=foopkg")
