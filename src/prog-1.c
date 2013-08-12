// Written by Ch. Tronche (http://tronche.lri.fr:8000/)
// Copyright by the author. This is unmaintained, no-warranty free software. 
// Please use freely. It is appreciated (but by no means mandatory) to
// acknowledge the author's contribution. Thank you.
// Started on Thu Jun 26 23:29:03 1997

//
// Xlib tutorial: 1st program
// Make a window appear on the screen.
//

#include <X11/Xlib.h> // Every Xlib program must include this
#include <assert.h>   // I include this to test return values the lazy way
#include <unistd.h>   // So we got the profile for 10 seconds

#define NIL (0)       // A name for the void pointer

main()
{
      Display *dpy = XOpenDisplay(NIL);
      assert(dpy);
      Window w = XCreateWindow(dpy, DefaultRootWindow(dpy), 0, 0, 
			       200, 100, 0, 
			       CopyFromParent, CopyFromParent, CopyFromParent,
			       NIL, 0);
      XMapWindow(dpy, w);
      XFlush(dpy);
      sleep(10);
}
