/*
#include <X11/Xlib.h>
#include <stdio.h>
#include <unistd.h>

int main()
{
	Display *display = XOpenDisplay(NULL);
	int screen = DefaultScreen(display);
	Window root = RootWindow(display,screen);

	int width = DisplayWidth(display,screen);
	int height = DisplayHeight(display,screen);
	XImage *img = XGetImage(display,root,0,0,width,height,XAllPlanes(),ZPixmap);

	Window widget = XCreateSimpleWindow(display,root,0,0,width,height,2,
		BlackPixel(display,screen),WhitePixel(display,screen));
	XMapWindow(display,widget);
	//screen = DefaultScreen(display);
	XFlush(display);

	GC gc = XCreateGC(display,widget,0,0);
	Pixmap pixmap = XCreatePixmap(display,widget,width,height,DefaultDepth(display,screen));
	
	XPutImage(display,pixmap,gc,img,0,0,0,0,width,height);
	XCopyPlane(display,pixmap,width,gc,0,0,width,height,0,0,1);

	sleep(5);
	XCloseDisplay(display);
	return 0;
}
*/

#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    Display *display = XOpenDisplay(NULL);
    int screen_num = DefaultScreen(display);
    int width = DisplayWidth(display,screen_num), height = DisplayHeight(display,screen_num);

    Window window = XCreateSimpleWindow(display, RootWindow(display, 0), 0, 0, width, height, 1, 0, 0);
    XImage *ximage = XGetImage(display,RootWindow(display,screen_num),0,0,width,height,XAllPlanes(),ZPixmap);
    XSelectInput(display, window, ButtonPressMask | ExposureMask);
    XMapWindow(display, window);
    while(1)
    {
    	XEvent ev;
    	XNextEvent(display, &ev);
    	switch(ev.type)
    	{
    		case Expose:
        		XPutImage(display, window, DefaultGC(display, 0), ximage, 0, 0, 0, 0, width, height);
        		break;
    		case ButtonPress:
        		exit(0);
    	}
    }
    return 0;
}