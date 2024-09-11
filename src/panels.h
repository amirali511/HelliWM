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
