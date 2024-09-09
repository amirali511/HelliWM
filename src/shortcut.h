#define HSETUP()\
  xcb_keycode_t firefox_keycode = xcb_key_symbols_get_keycode (keysyms, XK_F)[0];\
  cookie = xcb_grab_key (connection, 1, screen->root, MOD, firefox_keycode, XCB_GRAB_MODE_ASYNC, XCB_GRAB_MODE_ASYNC);\
  if (firefox_keycode == NULL) PANIC ("The firefox keycode is NULL, aborting...\n", firefox_keycode == 0);\
	 if (xcb_request_check (connection, cookie)) PANIC ("Could not grab the firefox key, aborting...\n", xcb_request_check (connection, cookie));\
  xcb_keycode_t deepin_calculator_keycode = xcb_key_symbols_get_keycode (keysyms, XK_Q)[0];\
  cookie = xcb_grab_key (connection, 1, screen->root, MOD, deepin_calculator_keycode, XCB_GRAB_MODE_ASYNC, XCB_GRAB_MODE_ASYNC);\
  if (deepin_calculator_keycode == NULL) PANIC ("The deepin_calculator keycode is NULL, aborting...\n", deepin_calculator_keycode == 0);\
	 if (xcb_request_check (connection, cookie)) PANIC ("Could not grab the deepin_calculator key, aborting...\n", xcb_request_check (connection, cookie));\
  xcb_keycode_t kitty_keycode = xcb_key_symbols_get_keycode (keysyms, XK_K)[0];\
  cookie = xcb_grab_key (connection, 1, screen->root, MOD, kitty_keycode, XCB_GRAB_MODE_ASYNC, XCB_GRAB_MODE_ASYNC);\
  if (kitty_keycode == NULL) PANIC ("The kitty keycode is NULL, aborting...\n", kitty_keycode == 0);\
	 if (xcb_request_check (connection, cookie)) PANIC ("Could not grab the kitty key, aborting...\n", xcb_request_check (connection, cookie));\
  xcb_keycode_t nemo_keycode = xcb_key_symbols_get_keycode (keysyms, XK_N)[0];\
  xcb_grab_key (connection, 1, screen->root, MOD, nemo_keycode, XCB_GRAB_MODE_ASYNC, XCB_GRAB_MODE_ASYNC);\
  if (nemo_keycode == NULL) PANIC ("The nemo keycode is NULL, aborting...\n", nemo_keycode == 0);\
	 if (xcb_request_check (connection, cookie)) PANIC ("Could not grab the nemo key, aborting...\n", xcb_request_check (connection, cookie));


#define HHANDLE()\
  else if ((key_press_event->state & MOD) && (key_press_event->detail == firefox_keycode)) { run ("firefox"); }\
  else if ((key_press_event->state & MOD) && (key_press_event->detail == deepin_calculator_keycode)) { run ("deepin_calculator"); }\
  else if ((key_press_event->state & MOD) && (key_press_event->detail == kitty_keycode)) { run ("kitty"); }\
  else if ((key_press_event->state & MOD) && (key_press_event->detail == nemo_keycode)) { run ("nemo"); }

