project('clox', 'c', version: '0.01', default_options: [
  'c_std=c99',
  'buildtype=debug'])


cc = meson.get_compiler('c')
readline = cc.find_library('readline')

dependencies = [readline]

clox_src = files(
  'chunk.c',
  'compiler.c',
  'debug.c',
  'main.c',
  'memory.c',
  'scanner.c',
  'value.c',
  'vm.c',
)

executable('clox', clox_src, dependencies: dependencies)
