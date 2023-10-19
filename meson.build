project('my_malloc', 'c', default_options: ['c_std=gnu99', 'warning_level=3'])


subdir('inc')
subdir('lib')
my_malloc_dep = declare_dependency(
  link_with: my_malloc_lib,
  include_directories: my_malloc_headers)


subdir('src')
