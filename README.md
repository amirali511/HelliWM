# Documentation for HelliWM

  HHH     HHH   EEEEEEEEE   LLL         LLL         III   WWW              WWWWWW              WWW   MMMMMM               MMMMMM
  HHH     HHH   EEEEEEEEE   LLL         LLL         III    WWW            WWW  WWW            WWW    MMM  MMM            MMM MMM
  HHH     HHH   EEE         LLL         LLL         III     WWW          WWW    WWW          WWW     MMM   MMM          MMM  MMM
  HHHHHHHHHHH   EEEEEEEEE   LLL         LLL         III      WWW        WWW      WWW        WWW      MMM    MMM        MMM   MMM
  HHHHHHHHHHH   EEEEEEEEE   LLL         LLL         III       WWW      WWW        WWW      WWW       MMM     MMM      MMM    MMM
  HHH     HHH   EEE         LLL         LLL         III        WWW    WWW          WWW    WWW        MMM      MMM    MMM     MMM
  HHH     HHH   EEEEEEEEE   LLLLLLLLL   LLLLLLLLL   III         WWW  WWW            WWW  WWW         MMM       MMM  MMM      MMM
  HHH     HHH   EEEEEEEEE   LLLLLLLLL   LLLLLLLLL   III          WWWWWW              WWWWWW          MMM        MMMMMM       MMM

## Simple explanation
HelliWM is a window manager built using c and xcb library, but with better options and better code and being customizable. It is built by me, Amirali Rahmani Vatan Khah, a fourteen year old boy. Hope you enjoy!

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
awk -f parser.awk
python3 Generator.py
meson setup build
cp config.h build/config.h
ninja -C build
sudo ninja -C build install
```
You can also run the `install.sh` script to install it automatically.

## Running
Run the wm by:
```sh
helliwm
```
## Uninstalling the window manager
Remove it by:
```sh
sudo ninja -C build uninstall
```

## Guides on the syntax of .WMRC
The first token `define` defines the indentifier word after it or the variable name.

Then any definition mark such as `=` or `:` can be put to continue (any other character or word which does not include a space is also possible).

Finally specify the value desired to apply and then the default value (also one value at time can be specified meaning that if you don't want to define anything and want to use the default value, `_` can be put for the value and vice versa).

Always the default value is `DEFAULT` if you cannot remember.

Lastly comments are defined with the syntax `% This is a comment`.

Always remember to put 2 empty lines on the end of the wmrc file and no empty lines in between.

## Important note on the build file
Write the complete path for awk parser yourself in the build file instead of the one written by myself, it may be different for you.

## WMRC fields
`MOD` : for mod keys like alt or super or ctrl (values are XCB_MOD_MASK_1 or XCB_MOD_MASK_4 or XCB_MOD_MASK_CONTROL).

`BG` : background color (values should be in hex).

`BBG` : bar background color (values should be in hex).

`TC` : text color (values should be in hex).

`QK` : quit key (values can be in the form XK_SOMETHING like XK_Q).

`LK` : launcher key (values can be in the form XK_SOMETHING like XK_S).

`CK` : close key (values can be in the form XK_SOMETHING like XK_C).

`TBG` : Taskbar background color (values should be in hex).

## Guides on the syntax of .SHORTCUT file
The only point is that you have to write the program names on the first line with spaces and the corresponding shortcuts on the second line with spaces.


## Note about dmenu
You still can use dmenu as a launcher, but it won't be in the correct form and thus, is not recommended.