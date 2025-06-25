self.description = "Hook using multiple 'Exec's"

self.add_hook("hook",
        """
        [Trigger]
        Type = Package
        Operation = Install
        Target = foo

        [Action]
        When = PostTransaction
        Exec = /bin/date
        Exec = /bin/date
        """);

sp = pmpkg("foo")
self.addpkg2db("sync", sp)

self.args = "-S foo"

self.addrule("PS4_RETCODE=0")
self.addrule("PS4_OUTPUT=warning.*overwriting previous definition of Exec")
