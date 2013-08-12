/* first include the standard headers that we're likely to need */
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xresource.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char ** argv)
{
	int screen_num, width, height;
	unsigned long background, border;
	Window win;
	XEvent ev;
	Display *dpy;

	/* First connect to the display server, as specified in the DISPLAY 
	environment variable. */
	dpy = XOpenDisplay(NULL);
	if (!dpy)
	{
		fprintf(stderr, "unable to connect to display");
		return 7;
	}

	/* these are macros that pull useful data out of the display object */
	/* we use these bits of info enough to want them in their own variables */
	screen_num = DefaultScreen(dpy);
	background = BlackPixel(dpy, screen_num);
	border = WhitePixel(dpy, screen_num);

	width = 40; /* start with a small window */
	height = 40;

	win = XCreateSimpleWindow(dpy, DefaultRootWindow(dpy), /* display, parent */
		0,0, /* x, y: the window manager will place the window elsewhere */
		width, height, /* width, height */
		2, border, /* border width & colour, unless you have a window manager */
		background); /* background colour */

	/* tell the display server what kind of events we would like to see */
	XSelectInput(dpy, win, ButtonPressMask|StructureNotifyMask );

	/* okay, put the window on the screen, please */
	XMapWindow(dpy, win);

	/* as each event that we asked about occurs, we respond.  In this
	 * case we note if the window's shape changed, and exit if a button
	 * is pressed inside the window */
	while(1)
	{
		XNextEvent(dpy, &ev);
		switch(ev.type)
		{
		case ConfigureNotify:
			if (width != ev.xconfigure.width || height != ev.xconfigure.height)
			{
				width = ev.xconfigure.width;
				height = ev.xconfigure.height;
				printf("Size changed to: %d by %d", width, height);
			}
			break;
		case ButtonPress:
			XCloseDisplay(dpy);
			break;
		}
	}
	return 0;
}