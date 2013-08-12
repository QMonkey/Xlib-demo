/*
 * simple-text.c - demonstrate drawing of text strings on a window. All
 *		   drawings are done in black color over a white background.
 */

#include <X11/Xlib.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>		/* getenv(), etc. */
#include <unistd.h>		/* sleep(), etc.  */

/*
 * function: create_simple_window. Creates a window with a white background
 *           in the given size.
 * input:    display, size of the window (in pixels), and location of the window
 *           (in pixels).
 * output:   the window's ID.
 * notes:    window is created with a black border, 2 pixels wide.
 *           the window is automatically mapped after its creation.
 */
Window
create_simple_window(Display* display, int width, int height, int x, int y)
{
  int screen_num = DefaultScreen(display);
  int win_border_width = 2;
  Window win;

  /* create a simple window, as a direct child of the screen's */
  /* root window. Use the screen's black and white colors as   */
  /* the foreground and background colors of the window,       */
  /* respectively. Place the new window's top-left corner at   */
  /* the given 'x,y' coordinates.                              */
  win = XCreateSimpleWindow(display, RootWindow(display, screen_num),
                            x, y, width, height, win_border_width,
                            BlackPixel(display, screen_num),
                            WhitePixel(display, screen_num));

  /* make the window actually appear on the screen. */
  XMapWindow(display, win);

  /* flush all pending requests to the X server. */
  XFlush(display);

  return win;
}

GC
create_gc(Display* display, Window win, int reverse_video)
{
  GC gc;				/* handle of newly created GC.  */
  unsigned long valuemask = 0;		/* which values in 'values' to  */
					/* check when creating the GC.  */
  XGCValues values;			/* initial values for the GC.   */
  unsigned int line_width = 2;		/* line width for the GC.       */
  int line_style = LineSolid;		/* style for lines drawing and  */
  int cap_style = CapButt;		/* style of the line's edje and */
  int join_style = JoinBevel;		/*  joined lines.		*/
  int screen_num = DefaultScreen(display);

  gc = XCreateGC(display, win, valuemask, &values);
  if (gc < 0) {
	fprintf(stderr, "XCreateGC: \n");
  }

  /* allocate foreground and background colors for this GC. */
  if (reverse_video) {
    XSetForeground(display, gc, WhitePixel(display, screen_num));
    XSetBackground(display, gc, BlackPixel(display, screen_num));
  }
  else {
    XSetForeground(display, gc, BlackPixel(display, screen_num));
    XSetBackground(display, gc, WhitePixel(display, screen_num));
  }

  /* define the style of lines that will be drawn using this GC. */
  XSetLineAttributes(display, gc,
                     line_width, line_style, cap_style, join_style);

  /* define the fill style for the GC. to be 'solid filling'. */
  XSetFillStyle(display, gc, FillSolid);

  return gc;
}

void
main(int argc, char* argv[])
{
  Display* display;		/* pointer to X Display structure.           */
  int screen_num;		/* number of screen to place the window on.  */
  Window win;			/* pointer to the newly created window.      */
  unsigned int display_width,
               display_height;	/* height and width of the X display.        */
  unsigned int win_width,
	       win_height;	/* height and width for the new window.      */
  char *display_name = getenv("DISPLAY");  /* address of the X display.      */
  GC gc;			/* GC (graphics context) used for drawing    */
				/*  in our window.			     */
  XFontStruct* font_info;       /* Font structure, used for drawing text.    */
//  char* font_name = "*-helvetica-*-12-*"; /* font to use for drawing text.   */
  char *font_name = "fixed";

  /* open connection with the X server. */
  display = XOpenDisplay(display_name);
  if (display == NULL) {
    fprintf(stderr, "%s: cannot connect to X server '%s'\n",
            argv[0], display_name);
    exit(1);
  }

  /* get the geometry of the default screen for our display. */
  screen_num = DefaultScreen(display);
  display_width = DisplayWidth(display, screen_num);
  display_height = DisplayHeight(display, screen_num);

  /* make the new window occupy 1/9 of the screen's size. */
  win_width = (display_width / 3);
  win_height = (display_height / 3);

  printf("display width - '%d'; height - '%d'\n", display_width, display_height);
  printf("window width - '%d'; height - '%d'\n", win_width, win_height);

  /* create a simple window, as a direct child of the screen's   */
  /* root window. Use the screen's white color as the background */
  /* color of the window. Place the new window's top-left corner */
  /* at the given 'x,y' coordinates.                             */
  win = create_simple_window(display, win_width, win_height, 0, 0);

  /* allocate a new GC (graphics context) for drawing in the window. */
  gc = create_gc(display, win, 0);
  XSync(display, False);

  /* try to load the given font. */
  font_info = XLoadQueryFont(display, font_name);
  if (!font_info) {
      fprintf(stderr, "XLoadQueryFont: failed loading font '%s'\n", font_name);
      exit(-1);
  }

  /* assign the given font to our GC. */
  XSetFont(display, gc, font_info->fid);

  {
    /* variables used for drawing the text strings. */
    int x, y;
    char* text_string;
    int string_width;
    int font_height;

    /* find the height of the characters drawn using this font.        */
    font_height = font_info->ascent + font_info->descent;

    /* draw a "hello world" string on the top-left side of our window. */
    text_string = "hello world";
    x = 0;
    y = font_height;
    XDrawString(display, win, gc, x, y, text_string, strlen(text_string));

    /* draw a "middle of the road" string in the middle of our window. */
    text_string = "middle of the road";
    /* find the width, in pixels, of the text that will be drawn using */
    /* the given font.                                                 */
    string_width = XTextWidth(font_info, text_string, strlen(text_string));
    x = (win_width - string_width) / 2;
    y = (win_height + font_height) / 2;
    XDrawString(display, win, gc, x, y, text_string, strlen(text_string));
  }

  /* flush all pending requests to the X server. */
  XFlush(display);

  /* make a delay for a short period. */
  sleep(4);

  /* close the connection to the X server. */
  XCloseDisplay(display);
}
