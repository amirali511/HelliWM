/*
Minimalist and light-weight bar for XCB + Minimalist and light-weight task window for XCB
*/
#include "def.h"

/*
  Now
*/
static string
now () 
{
  /*
    Using the time object to output the time
  */
	time_t n;
	struct tm * real;
	n = time (NULL);
	real = localtime (&n);
	string t1 = malloc (10);
  if (!t1) {
    PANIC ("Could not allocate memory for the time string, aborting...\n", !t1);
  }
	strftime (t1, 10, "%H:%M", real);
	return t1;
}

/*
  Battery status
*/
static string
battery ()
{
  FILE * fp;
  char buff[10];
  static char result[4];
  int percentage;
  fp = fopen ("/sys/class/power_supply/BAT0/capacity", "r");
  if (fp == NULL) {
    PANIC ("File is null, aborting...\n", fp == NULL);
  }

  if (fgets (buff, sizeof(buff), fp) != NULL) {
    int percentage = strtol (buff, 
                             NULL, 
                             10);
    
    if (percentage == 100) {
      if (sprintf (result, 
               "%d", 
               percentage) < 0) {
                PANIC ("Could not write to the result buffer, aborting...\n", -1 == -1);
              }
      
      if (strcat (result, "%") == NULL) {
        PANIC ("Could not concatenate the string, aborting...\n", 0 == 0);
      }
    } else {
      if (sprintf (result, 
               "%d", 
               percentage) < 0) {
                PANIC ("Could not write to the result buffer, aborting...\n", -1 == -1);
              }

      if (strcat (result, "% ") == NULL) {
        PANIC ("Could not concatnate the string, aborting...\n", 0 == 0);
      }
    }
  } else {
    PANIC ("Could not extract to the buffer, aborting...\n", 1 == 1);
  }
  
  if (fclose (fp) != 0) {
    PANIC ("Could not close the file, aborting...\n", 0 == 0);
  }

  return result;
}

/*
  Creating the bar
*/
static uint32_t 
create_bar (xcb_connection_t * conn,
            xcb_screen_t       scr,
            uint32_t           scrID)
{
	/*
    Create the top bar as a separate window
	*/
  uint32_t values2[1];
  values2[0] = BBG;
	uint32_t barID = xcb_generate_id (conn);
	
  cookie = xcb_create_window (conn,
                              XCB_COPY_FROM_PARENT,
                              barID,
                              scrID,
                              0, 0,
                              scr.width_in_pixels, 20,
                              0,
                              XCB_WINDOW_CLASS_INPUT_OUTPUT,
                              scr.root_visual,
                              XCB_CW_BACK_PIXEL, values2);
                                      
  if (!barID) {
    PANIC ("Could not create the bar, aborting...\n", !barID);
  }
  if (xcb_request_check (conn, cookie)) {
    PANIC ("Could not create the bar, aborting...\n", xcb_request_check (conn, cookie));
  }

  cookie = xcb_map_window (conn, barID);
  if (xcb_request_check (conn, cookie)) {
    PANIC ("Could not map the bar, aborting...\n", xcb_request_check (conn, cookie));
  }
  if (xcb_flush (conn) <= 0) {
    PANIC ("Could not flush the connection, aborting...\n", xcb_flush (conn) <= 0);
  }
  return barID;
}

/*
  Writing the current user's name and the time on the bar
*/
static uint32_t write_on_bar (xcb_connection_t * conn,
                              uint32_t           barID,
                              xcb_screen_t       scr)
{
  /*
    Font object
  */
  xcb_font_t font = xcb_generate_id (conn);
  if (font == -1) {
    PANIC ("Could not create the font, aborting...\n", font == -1);
  }
  cookie = xcb_open_font (conn,
                          font,
                          sizeof ("9x15"),
                          "9x15");
  if (xcb_request_check (conn, cookie)) {
    PANIC ("Could not open the font, aborting...\n", xcb_request_check (conn, cookie));
  }
  /*
    XCB gcontext
  */
  xcb_gcontext_t gcID = xcb_generate_id (conn);
  if (gcID == -1) {
    PANIC ("Could not create the gc, aborting...\n", gcID == -1);
  }
  uint32_t values3[] = {TC, BBG, font};
      
  cookie = xcb_create_gc (conn, 
                          gcID, 
                          barID, 
                          XCB_GC_FOREGROUND | XCB_GC_BACKGROUND | XCB_GC_FONT, 
                          values3);
  if (xcb_request_check (conn, cookie)) {
    PANIC ("Could not create the gc, aborting...\n", xcb_request_check (conn, cookie));
  }
  /*
    Writer
  */
  cookie = xcb_image_text_8 (conn,
                             5,
                             barID,
                             gcID,
                             scr.width_in_pixels / 2 - 20, 15,
                             now ());
  if (xcb_request_check (conn, cookie)) {
    PANIC ("Could not write on the bar, aborting...\n", xcb_request_check (conn, cookie));
  }
  cookie = xcb_image_text_8 (conn,
                             sizeof (getlogin ()) - 1,
                             barID,
                             gcID,
                             10, 15,
                             getlogin ());
  if (xcb_request_check (conn, cookie)) {
    PANIC ("Could not write on the bar, aborting...\n", xcb_request_check (conn, cookie));
  }

  cookie = xcb_image_text_8 (conn,
                             sizeof (battery ()) - 1,
                             barID,
                             gcID,
                             scr.width_in_pixels - 35, 15,
                             battery ());
  if (xcb_request_check (conn, cookie)) {
    PANIC ("Could not write on the bar, aborting...\n", xcb_request_check (conn, cookie));
  }

  cookie = xcb_map_window (conn, barID);
  if (xcb_request_check (conn, cookie)) {
    PANIC ("Could not map the bar, aborting...\n", xcb_request_check (conn, cookie));
  }
  if (xcb_flush (conn) <= 0) {
    PANIC ("Could not flush the connection, aborting...\n", xcb_flush (conn) <= 0);
  }
  return font;   
}

static uint32_t
create_task_window (xcb_connection_t * conn,
                xcb_screen_t       scr,
                uint32_t scrID)
{
  uint32_t taskID = xcb_generate_id (conn);
  uint32_t values[1];
  values[0] = TBG;
  cookie = xcb_create_window (conn,
                              XCB_COPY_FROM_PARENT,
                              taskID,
                              scrID,
                              0, scr.height_in_pixels - 40,
                              scr.width_in_pixels, 40,
                              0,
                              XCB_WINDOW_CLASS_INPUT_OUTPUT,
                              scr.root_visual,
                              XCB_CW_BACK_PIXEL, values);
  
  if (xcb_request_check (conn, cookie)) {
    PANIC ("Could not create the task window, aborting...\n", xcb_request_check (conn, cookie));
  }

  if (!taskID) {
    PANIC ("Could not create the task window, aborting...\n", !taskID);
  }

  cookie = xcb_map_window (conn, taskID);
  if (xcb_request_check (conn, cookie)) {
    PANIC ("Could not map the task window, aborting...\n", xcb_request_check (conn, cookie));
  }

  if (xcb_flush (conn) <= 0) {
    PANIC ("Could not flush the connection, aborting...\n", xcb_flush (conn) <= 0);
  }

  return taskID;
}

static void
write_on_task_window (xcb_connection_t * conn,
                      uint32_t scrID,
                      xcb_screen_t scr,
                      uint32_t taskID)
{
  xcb_intern_atom_cookie_t icon_cookie = xcb_intern_atom (conn, 0, sizeof ("_NET_WM_ICON"), "_NET_WM_ICON");
  xcb_intern_atom_reply_t * icon_reply = xcb_intern_atom_reply (conn, icon_cookie, &error);
  if (error) {
    PANIC ("Could not get the icon atom, aborting...\n", error);
  }
  xcb_atom_t XCB_ATOM_WM_ICON = icon_reply->atom;
  free (icon_reply);

  xcb_query_tree_cookie_t tree_cookie = xcb_query_tree (conn, scr.root);
  xcb_query_tree_reply_t * tree_reply = xcb_query_tree_reply (conn, tree_cookie, &error);
  
  if (error) {
    PANIC ("Could not get the list of windows, aborting...\n", error);
  }
  if (tree_reply) {
    xcb_window_t * children = xcb_query_tree_children (tree_reply);
    int num_children = xcb_query_tree_children_length(tree_reply) - 3;
    int x_offset = 5;
    for (int i = 0; i < num_children; i++) {
      xcb_get_property_cookie_t property_cookie = xcb_get_property (conn,
                                                                    0,
                                                                    children[i],
                                                                    XCB_ATOM_WM_ICON,
                                                                    XCB_ATOM_CARDINAL,
                                                                    0,
                                                                    UINT32_MAX);

      xcb_get_property_reply_t * property_reply = xcb_get_property_reply (conn,
                                                                          property_cookie,
                                                                          &error);
      if (error) {
        PANIC ("Could not get the property of windows, aborting...\n", error);
      }  

      if (property_reply && xcb_get_property_value_length (property_reply) > 0) {
        
        uint32_t * data = xcb_get_property_value (property_reply);
        // int width, height = data[0], data[1];
        int pixmapID = xcb_generate_id (conn);
        
        if (!pixmapID) {
          PANIC ("Could not create the pixmapID, aborting...\n", !pixmapID);
        }

        cookie = xcb_create_pixmap (conn,
                                    XCB_COPY_FROM_PARENT,
                                    pixmapID,
                                    scr.root,
                                    32,
                                    32);
        if (xcb_request_check (conn, cookie)) {
          PANIC ("Could not create the pixmap, aborting...\n", xcb_request_check (conn, cookie));
        }

        xcb_gcontext_t gc = xcb_generate_id (conn);
        if (!gc) {
          PANIC ("Could not create the gc, aborting...\n", !gc);
        }

        cookie = xcb_create_gc (conn,
                                gc,
                                pixmapID,
                                0,
                                NULL);

        if (xcb_request_check (conn, cookie)) {
          PANIC ("Could not create the gc, aborting...\n", xcb_request_check (conn, cookie));
        }

        xcb_image_t * icon = xcb_image_create_native (conn,
                                                      32, 32,
                                                      XCB_IMAGE_FORMAT_Z_PIXMAP,
                                                      XCB_COPY_FROM_PARENT,
                                                      NULL,
                                                      32 * 32 * 4,
                                                      (uint8_t *) (data + 2));
                                                    
        if (icon == NULL) {
          PANIC ("Could not create the image, aborting...\n", icon == NULL);
        }

        cookie = xcb_image_put (conn, pixmapID, gc, icon, 0, 0, 0);
        if (xcb_request_check (conn, cookie)) {
          PANIC ("Could not write the icon on the pixmap, aborting...\n", xcb_request_check (conn, cookie));
        }
        
        cookie = xcb_copy_area (conn, 
                                pixmapID, 
                                taskID, 
                                gc, 
                                0, 0, 
                                x_offset, 
                                scr.height_in_pixels - 36, 
                                32, 32);
        if (xcb_request_check (conn, cookie)) {
          PANIC ("Could not copy the pixmap to the screen, aborting...\n", xcb_request_check (conn, cookie));
        }

        x_offset += 37;
        
        cookie = xcb_free_pixmap (conn, pixmapID);
        if (xcb_request_check (conn, cookie)) {
          PANIC ("Could not free the pixmap, aborting...\n", xcb_request_check (conn, cookie));
        }

        cookie = xcb_free_gc (conn, gc);
        if (xcb_request_check (conn, cookie)) {
          PANIC ("Could not free the gc, aborting...\n", xcb_request_check (conn, cookie));
        }
      }                  
    }
  }
  if (xcb_flush (conn) <= 0) PANIC ("Could not flush the connection, aborting...\n", xcb_flush (conn) <= 0);
}
