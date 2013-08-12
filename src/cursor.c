/*
 * cursor.c - demonstrate creating mouse cursors and attaching them to
 *                    a window.
 */

#include <X11/Xlib.h>
#include <X11/Xutil.h>		/* BitmapOpenFailed, etc.    */
#include <X11/cursorfont.h>	/* pre-defined crusor shapes */

#include <stdio.h>
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
  unsigned int width, height;	/* height and width for the new window.      */
  char *display_name = getenv("DISPLAY");  /* address of the X display.      */
  GC gc;			/* GC (graphics context) used for drawing    */
				/*  in our window.			     */
  Cursor font_cursor,		/* handles for the cursors we will create.   */
         icon_cursor;

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
  width = (display_width / 3);
  height = (display_height / 3);
  printf("window width - '%d'; height - '%d'\n", width, height);

  /* create a simple window, as a direct child of the screen's   */
  /* root window. Use the screen's white color as the background */
  /* color of the window. Place the new window's top-left corner */
  /* at the given 'x,y' coordinates.                             */
  win = create_simple_window(display, width, height, 0, 0);

  /* allocate a new GC (graphics context) for drawing in the window. */
  gc = create_gc(display, win, 0);
  XSync(display, False);

  /* play with cursors... */
  {
    /* this variable will contain the ID of the newly created pixmap.    */
    Pixmap bitmap;
    /* these variables will contain the dimensions of the loaded bitmap. */
    unsigned int bitmap_width, bitmap_height;
    /* these variables will contain the location of the hotspot of the   */
    /* loaded bitmap.                                                    */
    int hotspot_x, hotspot_y;

    /* load the bitmap found in the file "icon.bmp", create a pixmap     */
    /* containing its data in the server, and put its ID in the 'bitmap' */
    /* variable.                                                         */
    int rc = XReadBitmapFile(display, win,
                             "icon.bmp",
                             &bitmap_width, &bitmap_height,
                             &bitmap,
                             &hotspot_x, &hotspot_y);
    /* check for failure or success. */
    switch (rc) {
        case BitmapOpenFailed:
            fprintf(stderr, "XReadBitmapFile - could not open file 'icon.bmp'.\n");
	    exit(1);
            break;
        case BitmapFileInvalid:
            fprintf(stderr,
                    "XReadBitmapFile - file '%s' doesn't contain a valid bitmap.\n",
                    "icon.bmp");
	    exit(1);
            break;
        case BitmapNoMemory:
            fprintf(stderr, "XReadBitmapFile - not enough memory.\n");
	    exit(1);
            break;
    }

    /* create a 'watch' cursor. */
    font_cursor = XCreateFontCursor(display, XC_watch);
    /* attach this cursor to our window. */
    XDefineCursor(display, win, font_cursor);
    XSync(display, False);

    /* make a short delay. */
    sleep(3);

    /* detach this cursor from our window. */
    XUndefineCursor(display, win);

    /* create a 'box' cursor. */
    font_cursor = XCreateFontCursor(display, XC_box_spiral);
    /* attach this cursor to our window. */
    XDefineCursor(display, win, font_cursor);
    XSync(display, False);

    /* make a short delay. */
    sleep(3);

    /* detach this cursor from our window. */
    XUndefineCursor(display, win);

    /* create a cursor out of our icon's pixmap. */
    {
      /* first, define forground and background colors for the cursor. */
      XColor cursor_fg, cursor_bg;

      /* access the default color map of our screen. */
      Colormap screen_colormap =
			DefaultColormap(display, DefaultScreen(display));

      /* allocate black and while colors. */
      Status rc = XAllocNamedColor(display,
                                   screen_colormap,
                                   "black",
                                   &cursor_fg,
                                   &cursor_fg);
      if (rc == 0) {
          fprintf(stderr, "XAllocNamedColor - canot allocate 'black' ??!!??\n");
          exit(1);
      }
      rc = XAllocNamedColor(display,
                            screen_colormap,
                            "white",
                            &cursor_bg,
                            &cursor_bg);
      if (rc == 0) {
          fprintf(stderr, "XAllocNamedColor - canot allocate 'white' ??!!??\n");
          exit(1);
      }

      /* finally, generate the cursor. make the 'hot spot' be close to the */
      /* top-left corner of the cursor - location (x=5, y=4). */
      icon_cursor = XCreatePixmapCursor(display, bitmap, bitmap,
				        &cursor_fg, &cursor_bg,
                                        5, 4);
    }

    /* attach the icon cursor to our window. */
    XDefineCursor(display, win, icon_cursor);
    XSync(display, False);

    /* make a short delay. */
    sleep(4);

    /* detach this cursor from our window. */
    XUndefineCursor(display, win);

    /* finally, free our icon pixmap. */
    XFreePixmap(display, bitmap);
  }

  /* flush all pending requests to the X server. */
  XFlush(display);

  /* make a delay for a short period. */
  sleep(4);

  /* close the connection to the X server. */
  XCloseDisplay(display);
}
