project('clox', 'c', default_options: [
  'c_std=c99',
  'buildtype=debug'])

clisp_src = files(
  'main.c',
  'common.h',
)

cc = meson.get_compiler('c')

executable('clox', clisp_src)
