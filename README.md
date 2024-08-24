# Documentation for HelliWM

## Simple explanation
HelliWM is a window manager built using c and xcb library, but with better options and better code and being customizable.

## Important note before we continue
The awk parser is currently unmaintained and it won't work properly, so edit the config.h header file yourself.

You can use the config.h file from the git repo.

## Install Dependencies
The dependencies used in the project are `xcb` and `xcb_cursor` as can be seen in the `meson.build` file.

You can install `xcb` from [here](https://xcb.freedesktop.org/dist/).

Also `xcb_cursor` can be installed from the libxcb-cursor or xcb-util-cursor.

Also having `awk` or `gawk`, `meson`, `ninja`, and `gcc` is required.

Finally you need to install dmenu, to use as a launcher. 

You can do so by their documentation for installing.

## Compile the source code to run
Execute in the source directory:
```bash
$ meson -C <name of the build directory>
$ ninja -C <name of the build directory>
$ sudo ninja -C <name of the build directory> install
```
## Running
Run the wm by:
```sh
$ helliwm
```
## Uninstalling the window manager
Remove it by:
```sh
$ sudo ninja -C build uninstall
```

## Guides on the syntax
(The awk parser is no longer maintained, so this part may not be useful, unless you want to know about wmrc)

The first token `define` defines the indentifier word after it or the variable name.

Then any definition mark such as `=` or `:` can be put to continue (any other character or word which does not include a space is also possible).

Finally specify the value desired to apply and then the default value (also one value at time can be specified meaning that if you don't want to define anything and want to use the default value, `_` can be put for the value and vice versa).

Always the default value is `DEFAULT` if you cannot remember.

Lastly comments are defined with the syntax `% This is a comment`.

Always remember to put an empty line on the end of the wmrc file.

## Important note on the build file
Write the complete path for awk parser yourself in the build file instead of the one written by myself, it may be different for you.

## WMRC fields
`mod` : for mod keys like alt or super or ctrl (values are XCB_MOD_MASK_1 or XCB_MOD_MASK_4 or XCB_MOD_MASK_CONTROL).

`bg` : background color (values should be in hex).

`bbg` : bar background color (values should be in hex).

`tc` : text color (values should be in hex).

`QK` : quit key (values can be in the form XK_SOMETHING like XK_Q).

`LK` : launcher key (values can be in the form XK_SOMETHING like XK_S).

`CK` : close key (values can be in the form XK_SOMETHING like XK_C).
