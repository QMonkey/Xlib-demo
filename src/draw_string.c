#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc,char **argv)
{
	Display *display = XOpenDisplay(NULL);
	int screen_num = DefaultScreen(display);
	Window root = RootWindow(display,screen_num);

	Window win = XCreateSimpleWindow(display,root,0,0,512,512,2,
		WhitePixel(display,screen_num),BlackPixel(display,screen_num));

	XMapWindow(display,win);
	XSelectInput(display,win,ExposureMask | KeyPressMask);
	GC gc = XCreateGC(display,win,0,NULL);
	char *hello = "Hello! Nice to meet you!";
	XEvent event;
	while(1)
	{
		XNextEvent(display,&event);
		switch(event.type)
		{
			case Expose:
				XSetForeground(display,gc,WhitePixel(display,screen_num));
				XDrawString(display,win,gc,10,10,hello,strlen(hello));
				break;
			case KeyPress:
				if(event.xkey.keycode == XKeysymToKeycode(display,XK_Escape))
				{
					exit(0);
				}
		}
	}
	XCloseDisplay(display);
	return 0;
}