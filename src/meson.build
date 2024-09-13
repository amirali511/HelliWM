project('helliwm', 'c',
        version : '0.4',
        default_options : ['warning_level=everything'])

xcb = dependency('xcb')
xcb_keysyms = dependency('xcb-keysyms')
xcb_cursor = dependency('xcb-cursor')
xcb_image = dependency('xcb-image')
X11 = dependency('x11')

parse = custom_target('awk parser',
                      input: '.wmrc',
                      output: 'config.h',
                      command: ['awk', '-f', '../parser.awk', '@INPUT@'],
                      capture: true)

executable('helliwm',
           'helliwm.c',
           'panels.h',
           parse,
           dependencies : [xcb, xcb_cursor, xcb_keysyms, xcb_image, X11],
           install : true)
