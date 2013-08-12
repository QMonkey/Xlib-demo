/*
 * simple-wm-hints.c - demonstrate usage of window-manager hints.
 */

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <stdio.h>
#include <stdlib.h>		/* getenv(), etc. */
#include <unistd.h>		/* sleep(), etc.  */

/* include the definition of the bitmap in our program. */
#include "icon.bmp"

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

  XSetTransientForHint(display, win, RootWindow(display, screen_num));

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
  printf("window width - '%d'; height - '%d'\n", win_width, win_height);

  /* create a simple window, as a direct child of the screen's   */
  /* root window. Use the screen's white color as the background */
  /* color of the window. Place the new window's top-left corner */
  /* at the given 'x,y' coordinates.                             */
  win = create_simple_window(display, win_width, win_height, 0, 0);

  /* allocate a new GC (graphics context) for drawing in the window. */
  gc = create_gc(display, win, 0);
  XSync(display, False);

  {
    /* This variable will store the window name property. */
    XTextProperty window_name_property;
    /* This variable will store the icon name property. */
    XTextProperty icon_name_property;
    /* pointer to the size hints structure. */
    XSizeHints* win_size_hints;
    /* pointer to the WM hints structure. */
    XWMHints* win_hints;
    /* pixmap used to store the icon's image. */
    Pixmap icon_pixmap;

    /* This window name and icon name strings. */
    char* window_name = "hello, world";
    char* icon_name = "small world";

    /* translate the window name string into an X property. */
    int rc = XStringListToTextProperty(&window_name,
                                       1,
                                       &window_name_property);
    /* check the success of the translation. */
    if (rc == 0) {
        fprintf(stderr, "XStringListToTextProperty - out of memory\n");
        exit(1);
    }
    /* translate the icon name string into an X property. */
    rc = XStringListToTextProperty(&icon_name,
                                   1,
                                   &icon_name_property);
    /* check the success of the translation. */
    if (rc == 0) {
        fprintf(stderr, "XStringListToTextProperty - out of memory\n");
        exit(1);
    }

    /* set the window name and icon name of our window. */
    XSetWMName(display, win, &window_name_property);
    XSetWMIconName(display, win, &icon_name_property);

    /* allocate a size hints structure. */
    win_size_hints = XAllocSizeHints();
    if (!win_size_hints) {
        fprintf(stderr, "XAllocSizeHints - out of memory\n");
        exit(1);
    }

    /* initialize the structure appropriatly. */
    /* first, specify which size hints we want to fill in. */
    /* in our case - setting the minimal size as well as the initial size. */
    win_size_hints->flags = PSize | PMinSize;
    /* next, specify the desired limits.                             */
    /* in our case - make the window's size at least 300x200 pixels. */
    /* and make its initial size 400x250.                            */
    win_size_hints->min_width = 300;
    win_size_hints->min_height = 200;
    win_size_hints->base_width = 400;
    win_size_hints->base_height = 250;

    /* pass the size hints to the window manager. */
    XSetWMNormalHints(display, win, win_size_hints);
    
    /* finally, we can free the size hints structure. */
    XFree(win_size_hints);

    /* load the given bitmap data and create an X pixmap contianing it. */
    icon_pixmap = XCreateBitmapFromData(display,
                                        win,
                                        icon_bitmap_bits,
                                        icon_bitmap_width,
                                        icon_bitmap_height);
    if (!icon_pixmap) {
        fprintf(stderr, "XCreateBitmapFromData - error creating pixmap\n");
        exit(1);
    }

    /* allocate a WM hints structure. */
    win_hints = XAllocWMHints();
    if (!win_hints) {
        fprintf(stderr, "XAllocWMHints - out of memory\n");
        exit(1);
    }

    /* initialize the structure appropriatly. */
    /* first, specify which size hints we want to fill in.  */
    /* in our case - setting the icon's pixmap, setting the */
    /* state hint as well as the icon position hint.        */
    win_hints->flags = IconPixmapHint | StateHint | IconPositionHint;
    /* next, specify the desired ihnts data.           */
    /* in our case - supply the icon's desired pixmap. */
    /* make the window's initial state be iconized,    */
    /* and set the icon position to the top-left part  */
    /* of the screen.                                  */
    win_hints->icon_pixmap = icon_pixmap;
    win_hints->initial_state = IconicState;
    win_hints->icon_x = 0;
    win_hints->icon_y = 0;

    /* pass the hints to the window manager. */
    XSetWMHints(display, win, win_hints);

    /* finally, we can free the WM hints structure. */
    XFree(win_hints);
  }

  /* flush all pending requests to the X server. */
  XFlush(display);

  /* make a delay for a short period. */
  sleep(25);

  /* close the connection to the X server. */
  XCloseDisplay(display);
}
