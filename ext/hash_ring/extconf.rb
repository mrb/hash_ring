require 'mkmf'

extension_name = 'hash_ring'

dir_config(extension_name)

abort "libhashring is required" unless have_library("hashring")

create_makefile("hash_ring/hash_ring")
