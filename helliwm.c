/*
	HelliWM version 0.2
	Created by Amirali Rahmani Vatan Khah
	https://github.com/amirali511		
*/ 

/*
	Includes
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
#include <signal.h>
#include "bar.h"
#include "config.h"

/* 
	Panic function 
*/
#ifndef PANIC
#define PANIC(msg, cond) if (cond) { perror(msg); assert (!cond); }
#endif

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
const char * helli_wm_version = "0.2";
static xcb_screen_t * screen;

/*
	Window size struct
*/
typedef struct window_size {
	uint32_t width;
	uint32_t height;
} window_size;

/*
	Bar ID type
*/
typedef uint32_t barID;

/*
	Function prototypes
*/
static void 						close_wm ();
static window_size * 		tile_values (xcb_screen_t * screen, uint32_t * IDs);
static uint32_t 				create_cursor (xcb_connection_t * conn, xcb_screen_t * screen);
static void 						runner (char * name);
static void 						handle_key_press_event (xcb_connection_t * conn, xcb_key_press_event_t * event, xcb_keycode_t exit_keycode, xcb_keycode_t dmenu_keycode, xcb_keycode_t win_dest_keycode);
static void 						close_focus_window (xcb_connection_t * conn);

/*
	Main function
*/
int
main (void)
{
	/*
		Create the screen as a window using setups and connections
	*/
	int screen_number;
	connection = xcb_connect (NULL, &screen_number);
	if (xcb_connection_has_error (connection)) {
		PANIC ("Could not connect to x server, aborting...\n", xcb_connection_has_error (connection));
	}
	const xcb_setup_t * 				        setup = xcb_get_setup (connection);
	xcb_screen_iterator_t 	  screen_iterator = xcb_setup_roots_iterator (setup);
	xcb_screen_t * 				   	   			 screen = screen_iterator.data;
	if (setup == NULL || screen == NULL || &screen_iterator == NULL) {
		PANIC ("Could not get the screen, aborting...\n", (setup == NULL || screen == NULL));
	}

	/*
		Xcb keysyms and keycodes setup for exiting and also dmenu_run
	*/
	xcb_key_symbols_t * 		 		keysyms = xcb_key_symbols_alloc (connection);
	xcb_keycode_t 	    	 exit_keycode = xcb_key_symbols_get_keycode(keysyms, XK_Q)[0];
	xcb_keycode_t 	   		dmenu_keycode = xcb_key_symbols_get_keycode(keysyms, XK_R)[0];
	xcb_keycode_t 	   win_dest_keycode = xcb_key_symbols_get_keycode(keysyms, XK_C)[0];
	if (keysyms == NULL || exit_keycode == NULL || dmenu_keycode == NULL || win_dest_keycode == NULL) {
		PANIC ("Could not get the keysyms or keycodes, aborting...\n", (keysyms == NULL || exit_keycode == 0 || dmenu_keycode == 0 || win_dest_keycode == 0));
	}

	cookie = xcb_grab_key(connection, 1, screen->root, XCB_MOD_MASK_1, exit_keycode, XCB_GRAB_MODE_ASYNC, XCB_GRAB_MODE_ASYNC);
	if (xcb_request_check (connection, cookie)) {
		PANIC ("Could not grab the exit key, aborting...\n", xcb_request_check (connection, cookie));
	}

	cookie = xcb_grab_key(connection, 1, screen->root, XCB_MOD_MASK_1, dmenu_keycode, XCB_GRAB_MODE_ASYNC, XCB_GRAB_MODE_ASYNC);
	if (xcb_request_check (connection, cookie)) {
		PANIC ("Could not grab the dmenu key, aborting...\n", xcb_request_check (connection, cookie));
	}

	cookie = xcb_grab_key(connection, 1, screen->root, XCB_MOD_MASK_1, win_dest_keycode, XCB_GRAB_MODE_ASYNC, XCB_GRAB_MODE_ASYNC);
	if (xcb_request_check (connection, cookie)) {
		PANIC ("Could not grab the close window key, aborting...\n", xcb_request_check (connection, cookie));
	}
	
	xcb_key_symbols_free(keysyms);
	/*
		Cursor and screen setup
	*/
	uint32_t 	screenID = xcb_generate_id (connection);
	uint32_t 		 	 cid = create_cursor (connection, screen);
	if (screenID == -1 || cid == -1) {
		PANIC ("Could not create the screen or cursor, aborting...\n", (screenID == 0 || cid == 0));
	}
	
	/*
		Screen values
	*/
	uint32_t values[4];
	values[0] = 0xeb8650;
	values[1] = XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_BUTTON_PRESS;
	values[2] = cid;
	values[3] = XCB_EVENT_MASK_KEY_PRESS;
	
	/*
		Creating the screen as a window
	*/
	cookie = xcb_create_window (connection,
															XCB_COPY_FROM_PARENT,
															screenID,
															screen->root,
															0, 0,
															screen->width_in_pixels, screen->height_in_pixels,
															0,
															XCB_WINDOW_CLASS_INPUT_OUTPUT,
															screen->root_visual,
															XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK | XCB_CW_CURSOR | XCB_CW_EVENT_MASK, values);
	if (xcb_request_check (connection, cookie)) {
		PANIC ("Could not create the screen, aborting...\n", xcb_request_check (connection, cookie));
	}					
	
	cookie = xcb_map_window (connection, screenID);
  if (xcb_request_check (connection, cookie)) {
		PANIC ("Could not map the screen, aborting...\n", xcb_request_check (connection, cookie));
	}
	
	if (xcb_flush (connection) <= 0) {
		PANIC ("Could not flush the connection, aborting...\n", (xcb_flush (connection) <= 0));
	}
	
	/*
		Creating the bar with the bar module
	*/
	barID bar = create_bar (connection,
													* screen,
													screenID);


	/*
		Generic event or the Stem Cell
	*/
	xcb_generic_event_t * generic_event;

	/*
		The event loop
	*/
	while (1) {

		/*
			Writing the time and name on the bar
		*/
		write_on_bar (connection,
   						 		bar,
   						 		* screen);
		/*
			Event fetch
		*/
		generic_event = xcb_poll_for_event (connection);

		/*
			Tiling windows with the query tree of windows
		*/	
		xcb_query_tree_cookie_t tree_cookie = xcb_query_tree(connection, screen->root);
		xcb_query_tree_reply_t * tree_reply = xcb_query_tree_reply(connection, tree_cookie, NULL);
		
		
		if (tree_reply) {
    	xcb_window_t * children = xcb_query_tree_children(tree_reply);
    	int num_children = xcb_query_tree_children_length(tree_reply);

			window_size * window_size_list = tile_values (screen, children);

    	free(tree_reply);
		}

		/*
			Event check
		*/
		if (generic_event != NULL) {
			switch (generic_event->response_type & ~0x80) {
				case XCB_EXPOSE: {
				
					

					break;
				}
				/*
					Keyboard shortcuts handling
				*/
				case XCB_KEY_PRESS: {
					/*
						Handler code
					*/
					xcb_key_press_event_t * key_press_event = (xcb_key_press_event_t *) generic_event;
          handle_key_press_event (connection, key_press_event, exit_keycode, dmenu_keycode, win_dest_keycode);
				}
				default: {

					break;
				}
			}
		}

		/*
			Destroy the Stem Cell
		*/
		free (generic_event);
	}

	
	/*
		End the program
	*/
	cookie = xcb_free_cursor (connection, cid);
	if (xcb_request_check (connection, cookie)) {
		PANIC ("Could not free the cursor, aborting...\n", xcb_request_check (connection, cookie));
	}
	
	close_wm ();
	return EXIT_SUCCESS;
}


/*
	Cursor maker
*/
static uint32_t
create_cursor (xcb_connection_t * conn, xcb_screen_t * screen)
{
	/*
		Create the cursor context and load the cursor
	*/
	xcb_cursor_context_t * ctx;
	if (xcb_cursor_context_new (conn, screen, &ctx)) {
		PANIC ("Could not create the cursor context\n", !ctx);
	}
	xcb_cursor_t cid = xcb_cursor_load_cursor (ctx, "arrow");
	return cid;
}

/*
	Close the WM
*/
static void
close_wm ()
{
	if (connection != NULL) {
		xcb_disconnect (connection);
		exit(0);
	} else {
		PANIC ("Could not close the wm, it's already closed\n", !connection);
		exit(0);
	}
}


/*
	Tiling algorithm
*/ 
static window_size *
tile_values (xcb_screen_t * screen, uint32_t * IDs)
{
    /*
        Screen dimensions
    */
    int screen_height, screen_width;
    screen_height = screen->height_in_pixels;
    screen_width = screen->width_in_pixels;
		if (screen == NULL) {
			PANIC ("Screen is null\n", !screen);
		}

    /*
        Tile the windows on the screen
    */
    int len = sizeof (IDs) / sizeof (uint32_t);
    window_size * window_size_list = malloc(len * sizeof(window_size));
    if (!window_size_list) {
			PANIC ("Memory allocation failed in tile_values", !window_size_list);
		}
		/*
			Dividing the screen into separate halves to tile the windows
		*/
    for (int iterator = 0; iterator < len; iterator++) {
        if (iterator % 2 == 0) {
            screen_width = screen_width / 2;
            window_size_list[iterator].width = screen_width;
            window_size_list[iterator].height = screen_height;
        }
        else {
            screen_height = screen_height / 2;
            window_size_list[iterator].width = screen_width;
            window_size_list[iterator].height = screen_height;
        }
    }
    return window_size_list;
}

/*
	Run a program
*/
static void
runner (char * name)
{
	/*
		Fork the process and run the program
	*/
	pid_t pid = fork ();
	if (pid == -1) {
		PANIC ("Could not fork the process\n", pid);
	}
	else if (pid == 0) {
		if (execlp (name, NULL, NULL)) {
			PANIC ("Could not run the program\n", !execlp (name, NULL, NULL));
		}
	}
}

/*
	Handle key press events
*/
static void
handle_key_press_event (xcb_connection_t * conn, xcb_key_press_event_t * event, xcb_keycode_t exit_keycode, xcb_keycode_t dmenu_keycode, xcb_keycode_t win_dest_keycode)
{
	/*
		Exit the program: Alt + q
	*/
  if ((event->state & XCB_MOD_MASK_1) && (event->detail == exit_keycode)) {
    close_wm ();
    exit(0);
  }
	/*
		Launch Dmenu for the launcher: Alt + r
	*/
	else if ((event->state & XCB_MOD_MASK_1) && (event->detail == dmenu_keycode)) {
		runner("dmenu_run");
		return;
	}
	else if ((event->state & XCB_MOD_MASK_1) && (event->detail == win_dest_keycode)) {
		close_focus_window (conn);
		return;
	}
}

/*
	Closing the focus window
*/
static void
close_focus_window (xcb_connection_t * conn)
{
	xcb_query_pointer_cookie_t cookief;
	cookief = xcb_query_pointer (conn, screen->root);
	xcb_query_pointer_reply_t * reply = xcb_query_pointer_reply (conn, cookief, &error);
	if (error) {
		PANIC ("Could not get the input focus", error);
	}
	if (!reply) {
		PANIC ("Could not get the input focus", !reply);
	}
	
	/*
		Closing the window by first finding it
	*/
	xcb_window_t focus = reply->child;
	cookie = xcb_destroy_window (conn, focus);
	if (xcb_request_check (conn, cookie)) {
		PANIC ("Could not destroy the window, aborting...\n", xcb_request_check (conn, cookie));
	}
}
