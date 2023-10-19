mem_size = get_option('mem_size').to_string()
def_mem_size = ''.join(['-DMEM_SIZE=', mem_size])
message(def_mem_size)

my_malloc_lib = static_library('my_malloc', ['malloc.c', 'os_mem.c'], include_directories: my_malloc_headers, c_args: [def_mem_size])
