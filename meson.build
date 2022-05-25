project('clox', 'c', version: '0.01', default_options: [
  'c_std=c99',
  'buildtype=debug'])

clisp_src = files(
  'chunk.h',
  'common.h',
  'debug.h',
  'memory.h',
  'chunk.c',
  'debug.c',
  'main.c',
  'memory.c',
)

executable('clox', clisp_src)
