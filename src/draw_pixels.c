#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <unistd.h>
#include <malloc.h>
#include <stdio.h>

int main(int argc,char **argv)
{
	Display *display = XOpenDisplay(NULL);
	int screen_num = DefaultScreen(display);
	Window root = RootWindow(display,screen_num);
	Visual *visual = DefaultVisual(display,screen_num);

	char *data = (char*)malloc(256*256*4);

	XImage *img = XCreateImage(display,visual,DefaultDepth(display,screen_num),ZPixmap,
		0,data,256,256,32,0);
	int count = 256 * 256;
	for (int i = 0; i < count; ++i)
	{
		XAddPixel(img,(long)i);
	}
	fprintf(stdout,"end\n");

	Window win = XCreateSimpleWindow(display,root,50,50,256,256,1,0,0);
	XSelectInput(display,win,ExposureMask);
	XMapWindow(display,win);

	XEvent event;
	while(1)
	{
		XNextEvent(display,&event);
		if(event.type == Expose)
		{
			XPutImage(display,win,DefaultGC(display,screen_num),img,0,0,0,0,256,256);
		}
	}
	sleep(10);
	XCloseDisplay(display);
	return 0;
}