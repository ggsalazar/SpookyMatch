project('SpookyMatch!', ['cpp', 'c'],
          version: '0.5.0',
    meson_version: '>= 0.62.0',
  default_options: [ 'warning_level=2', 'werror=false', 'cpp_std=gnu++2a', ],
)

#The following command can be run in bash to help debug build graphs
#meson introspect builddir --targets

sources = []

subdir('src')

deps = [
dependency('sdl3', required : true),
dependency('sdl3-image', required : true),
dependency('sdl3-ttf', required : true),
dependency('nlohmann_json', required : true)
]

executable('SpookyMatch!', sources,
  dependencies: deps,
  install: true,
)
