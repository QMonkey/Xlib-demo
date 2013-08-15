#include <X11/Xlib.h>
#include <stdio.h>

int main(int argc,char **argv)
{
	Display *display = XOpenDisplay(NULL);
	int screen_num = DefaultScreen(display);
	Window root = RootWindow(display,screen_num);

	Window win1 = XCreateSimpleWindow(display,root,0,0,500,500,1,0xffffff,0);
	Window win2 = XCreateSimpleWindow(display,root,100,100,1000,1000,1,0,0xffffff);

	XMapWindow(display,win1);
	XMapWindow(display,win2);
	XSelectInput(display,win1,ExposureMask | ButtonPressMask);
	XSelectInput(display,win2,ExposureMask | ButtonPressMask);

	XEvent event;
	while(1)
	{
		XNextEvent(display,&event);
		if(event.type == ButtonPress)
		{
			XUnmapWindow(display,event.xany.window);
			XDestroyWindow(display,event.xany.window);
		}
	}
	XCloseDisplay(display);
	return 0;
}