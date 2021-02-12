{ name = "st"
, dependencies = [ "prelude", "tailrec", "partial", "console" ]
, packages = ../pure-c/package-sets/packages.dhall
, sources = [ "src/**/*.purs", "test/**/*.purs" ]
}
