require 'mkmf'

extension_name = 'hash_ring'

dir_config(extension_name)

have_library("hashring")

create_makefile("hash_ring/hash_ring")
