/*
  Include and define and declare global vars
*/

#include <xcb/xcb.h>
#include <xcb/xproto.h>
#include <xcb/xcb_cursor.h>
#include <xcb/xcb_keysyms.h>
#include <X11/keysym.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include "processed_config.h"
#include "shortcut.h"

#ifndef PANIC
#define PANIC(msg, cond) if (cond) { perror(msg); assert (!cond); }
#endif

/*
	String type
*/
typedef char * string;

/*
	Global cookie to check for errors, Stem Cell
*/
xcb_void_cookie_t cookie;

/*
	Global generic error, Stem Cell
*/
xcb_generic_error_t * error;

/*
	First and global variables
*/
static xcb_connection_t * connection;
static xcb_screen_t * screen;

/*
	Bar ID type
*/
typedef uint32_t barID;
