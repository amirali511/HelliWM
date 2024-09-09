/*
Minimalist and light-weight bar for XCB
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
    PANIC ("Could not allocate memory for the time string\n", !t1);
  }
	strftime (t1, 10, "%H:%M", real);
	return t1;
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
                              0, scr.height_in_pixels - 30,
                              scr.width_in_pixels, 30,
                              0,
                              XCB_WINDOW_CLASS_INPUT_OUTPUT,
                              scr.root_visual,
                              XCB_CW_BACK_PIXEL, values2);
                                      
  if (!barID) {
    PANIC ("Could not create the bar\n", !barID);
  }
  if (xcb_request_check (conn, cookie)) {
    PANIC ("Could not create the bar\n", xcb_request_check (conn, cookie));
  }

  cookie = xcb_map_window (conn, barID);
  if (xcb_request_check (conn, cookie)) {
    PANIC ("Could not map the bar\n", xcb_request_check (conn, cookie));
  }
  if (xcb_flush (conn) <= 0) {
    PANIC ("Could not flush the connection\n", xcb_flush (conn) <= 0);
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
    PANIC ("Could not create the font\n", font == -1);
  }
  cookie = xcb_open_font (conn,
                          font,
                          sizeof ("6x13"),
                          "6x13");
  if (xcb_request_check (conn, cookie)) {
    PANIC ("Could not open the font\n", xcb_request_check (conn, cookie));
  }
  /*
    XCB gcontext
  */
  xcb_gcontext_t gcID = xcb_generate_id (conn);
  if (gcID == -1) {
    PANIC ("Could not create the gc\n", gcID == -1);
  }
  uint32_t values3[] = {TC, BBG, font};
      
  cookie = xcb_create_gc (conn, 
                          gcID, 
                          barID, 
                          XCB_GC_FOREGROUND | XCB_GC_BACKGROUND | XCB_GC_FONT, 
                          values3);
  if (xcb_request_check (conn, cookie)) {
    PANIC ("Could not create the gc\n", xcb_request_check (conn, cookie));
  }
  /*
    Writer
  */
  cookie = xcb_image_text_8 (conn,
                             5,
                             barID,
                             gcID,
                             scr.width_in_pixels / 2 - 20, 20,
                             now ());
  if (xcb_request_check (conn, cookie)) {
    PANIC ("Could not write on the bar\n", xcb_request_check (conn, cookie));
  }
  cookie = xcb_image_text_8 (conn,
                             sizeof (getlogin ()) - 1,
                             barID,
                             gcID,
                             10, 20,
                             getlogin ());
  if (xcb_request_check (conn, cookie)) {
    PANIC ("Could not write on the bar\n", xcb_request_check (conn, cookie));
  }

  cookie = xcb_map_window (conn, barID);
  if (xcb_request_check (conn, cookie)) {
    PANIC ("Could not map the bar\n", xcb_request_check (conn, cookie));
  }
  if (xcb_flush (conn) <= 0) {
    PANIC ("Could not flush the connection\n", xcb_flush (conn) <= 0);
  }
  return font;   
}
