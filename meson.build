project('clox', 'c', version: '0.01', default_options: [
  'c_std=c99',
  'buildtype=debug'])

clox_src = files(
  'chunk.h',
  'common.h',
  'debug.h',
  'memory.h',
  'value.h',
  'chunk.c',
  'debug.c',
  'main.c',
  'memory.c',
  'value.c',
)

executable('clox', clox_src)
