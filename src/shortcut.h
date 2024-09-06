#define HSETUP()\
  xcb_keycode_t firefox_keycode = xcb_key_symbols_get_keycode (keysyms, XK_F)[0];\
  xcb_grab_key (connection, 1, screen->root, MOD, firefox_keycode, XCB_GRAB_MODE_ASYNC, XCB_GRAB_MODE_ASYNC);\
  xcb_keycode_t deepin_calculator_keycode = xcb_key_symbols_get_keycode (keysyms, XK_Q)[0];\
  xcb_grab_key (connection, 1, screen->root, MOD, deepin_calculator_keycode, XCB_GRAB_MODE_ASYNC, XCB_GRAB_MODE_ASYNC);\
  xcb_keycode_t kitty_keycode = xcb_key_symbols_get_keycode (keysyms, XK_K)[0];\
  xcb_grab_key (connection, 1, screen->root, MOD, kitty_keycode, XCB_GRAB_MODE_ASYNC, XCB_GRAB_MODE_ASYNC);


#define HHANDLE()\
  else if ((key_press_event->state & MOD) && (key_press_event->detail == firefox_keycode)) { run ("firefox"); }\
  else if ((key_press_event->state & MOD) && (key_press_event->detail == deepin_calculator_keycode)) { run ("deepin_calculator"); }\
  else if ((key_press_event->state & MOD) && (key_press_event->detail == kitty_keycode)) { run ("kitty"); }

