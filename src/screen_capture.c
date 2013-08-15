#include <X11/Xlib.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    Display *display = XOpenDisplay(NULL);
    int screen_num = DefaultScreen(display);
    int width = DisplayWidth(display,screen_num), height = DisplayHeight(display,screen_num);

    Window window = XCreateSimpleWindow(display, RootWindow(display, screen_num), 0, 0, width, height, 1, 0, 0);
/*
    struct timeval start,end;
    gettimeofday(&start,NULL);
*/
    XImage *ximage = XGetImage(display,RootWindow(display,screen_num),0,0,width,height,XAllPlanes(),ZPixmap);
/*
    gettimeofday(&end,NULL);
    int timeuse = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    fprintf(stdout,"%d\n",timeuse);
  */
    XSelectInput(display, window, ButtonPressMask | ExposureMask);
    XMapWindow(display, window);
    XEvent ev;
    while(1)
    {
    	XNextEvent(display, &ev);
    	switch(ev.type)
    	{
    		case Expose:
        		XPutImage(display, window, DefaultGC(display, screen_num), ximage, 0, 0, 0, 0, width, height);
        		break;
    		case ButtonPress:
        		exit(0);
    	}
    }
    return 0;
}