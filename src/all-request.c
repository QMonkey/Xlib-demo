// Written by Ch. Tronche (http://tronche.com/)
// Copyright by the author. This is unmaintained, no-warranty free software. 
// Please use freely. It is appreciated (but by no means mandatory) to
// acknowledge the author's contribution. Thank you.
//
//  Copyright (C) 1998-2000 by Christophe Tronche ("the author"). All
//  Rights Reserved.

//  Permission is hereby granted, free of charge, to any person obtaining
//  a copy of this software and associated documentation files (the
//  "Software"), to deal in the Software without restriction, including
//  without limitation the rights to use, copy, modify, merge, publish,
//  distribute, sublicense, and/or sell copies of the Software, and to
//  permit persons to whom the Software is furnished to do so, subject to
//  the following conditions:

//  The above copyright notice and this permission notice shall be
//  included in all copies or substantial portions of the Software.

//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
//  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
//  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
//  ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
//  OTHER DEALINGS IN THE SOFTWARE.
//
// Started on Sat May  6 18:32:40 2000
//
// v1.0, Sat Jun  3 12:05:16 MET 2000
//
// This programs does nothing useful. It was just written so as to generate every X11
// request in order to test the X11 decoder for ethereal (see http://ethereal.zing.org/).
//
// To compile try something like: g++ all-request.cc -lX11 
// or may be g++ all-request.cc -L/usr/X11R6/lib -lX11
//

#include <X11/Xatom.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <unistd.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

#define NIL (0)

#define SIZEOF(x) (sizeof(x) / (sizeof((x)[0])))

static const char longString[] = "Because our senses sometimes fool us, I decided to believe there was nothing as they made us imagine. (...) And, noticing that this truth: \"I think, therefore I am\", was so firm and so assured that none of the most extravagant suppositions of the Skeptical ones couldn't took it in default, I decided to receive it as the first principle of the philosophy I was searching for. --- René Descartes, discours de la méthode.";

static int errorHandler(Display *dpy, XErrorEvent *error)
{
      char buffer1[1024];
      XGetErrorText(dpy, error -> error_code, buffer1, 1024);
      fprintf(stderr, 
	      "_X Error of failed request:  %s\n"
	      "_  Major opcode of failed request: % 3d (???)\n"
	      "_  Serial number of failed request:% 5d\n"
	      "_  Current serial number in output stream:?????\n",
	      buffer1,
	      error -> request_code,
	      error -> serial);
      return 0;
}

main()
{
      Window w2;

      Display *dpy = XOpenDisplay(NIL);
      assert(dpy);
      Screen *scr = DefaultScreenOfDisplay(dpy);

      // CreateWindow

      Window w = XCreateWindow(dpy, RootWindowOfScreen(scr), 10, 100, 200, 300, 0, CopyFromParent,
			       CopyFromParent, CopyFromParent,
			       0, NIL);

      XDestroyWindow(dpy, w);

      // CreateWindow with arguments

      XSetWindowAttributes swa;
      swa.background_pixel = WhitePixelOfScreen(scr);
      swa.bit_gravity = NorthWestGravity;
      swa.border_pixel = BlackPixelOfScreen(scr);
      swa.colormap = DefaultColormapOfScreen(scr);
      swa.cursor = None;
      swa.win_gravity = NorthGravity;

      w = XCreateWindow(dpy, RootWindowOfScreen(scr), 10, 100, 200, 300, 0, CopyFromParent,
			CopyFromParent, CopyFromParent,
			CWBackPixel | CWBitGravity | CWBorderPixel | CWColormap | CWCursor | CWWinGravity, 
			&swa);
      
      // CreateWindow with other arguments

      XDestroyWindow(dpy, w);

      Pixmap pixmap = XCreatePixmap(dpy, RootWindowOfScreen(scr), 45, 25, DefaultDepthOfScreen(scr));
      assert(pixmap);

      swa.background_pixmap = pixmap;
      swa.border_pixmap = pixmap;

      w = XCreateWindow(dpy, RootWindowOfScreen(scr), 10, 100, 200, 300, 0, CopyFromParent,
			CopyFromParent, CopyFromParent,
			CWBackPixmap | CWBorderPixmap,
			&swa);
      
      // ChangeWindowAttributes

      swa.backing_planes = 0x1;
      swa.backing_pixel = WhitePixelOfScreen(scr);
      swa.save_under = True;
      swa.event_mask = KeyPressMask | KeyReleaseMask;
      swa.do_not_propagate_mask = ButtonPressMask | Button4MotionMask;
      swa.override_redirect = False;
      XChangeWindowAttributes(dpy, w, CWBackingPlanes | CWBackingPixel | CWSaveUnder | CWEventMask
			      | CWDontPropagate | CWOverrideRedirect, &swa);

      // GetWindowAttributes

      XWindowAttributes wa;
      Status success = XGetWindowAttributes(dpy, w, &wa);

      // DestroyWindow (done)

      // DestroySubwindows

      w2 = XCreateWindow(dpy, w, 20, 30, 40, 50, 3, CopyFromParent, CopyFromParent, CopyFromParent, 0, NIL);
      XDestroySubwindows(dpy, w);

      // ChangeSaveSet

//        Display *dpy2 = XOpenDisplay(NIL);
//        assert(dpy2);
//        XAddToSaveSet(dpy2, w);
//        XCloseDisplay(dpy2);

      // ReparentWindow

      w2 = XCreateWindow(dpy, RootWindowOfScreen(scr), 20, 30, 40, 50, 3, 
			 CopyFromParent, CopyFromParent, CopyFromParent, 0, NIL);
      XReparentWindow(dpy, w2, w, 10, 5);

      // MapWindow

      XMapWindow(dpy, w);

      // MapSubwindows
      
      XMapSubwindows(dpy, w);

      // UnmapWindow
      
      XUnmapWindow(dpy, w);

      // UnmapSubwindows

      XMapWindow(dpy, w);
      XUnmapSubwindows(dpy, w2);
      XMapSubwindows(dpy, w);

      // ConfigureWindow

      Window w3 = XCreateWindow(dpy, w, 10, 50, 100, 10, 2,
			 CopyFromParent, CopyFromParent, CopyFromParent, 0, NIL);

      XMapWindow(dpy, w3);

      XWindowChanges wc;
      wc.x = -5;
      wc.y = -10;
      wc.width = 50;
      wc.height = 40;
      wc.border_width = 7;
      wc.sibling = w2;
      wc.stack_mode = Opposite;
      XConfigureWindow(dpy, w3, 
		       CWX | CWY | CWWidth | CWHeight | CWBorderWidth | CWSibling | CWStackMode, 
		       &wc);

      // CirculateWindow

      XCirculateSubwindows(dpy, w, RaiseLowest);

      // GetGeometry

      Window root;
      int x, y;
      unsigned width, height, border_width, depth;
      XGetGeometry(dpy, w, &root, &x, &y, &width, &height, &border_width, &depth);

      // QueryTree

      Window parent;
      Window *children;
      unsigned nchildren;
      success = XQueryTree(dpy, w, &root, &parent, &children, &nchildren);
      XFree(children);

      // InternAtom

      Atom a = XInternAtom(dpy, "WM_PROTOCOLS", True);

      // GetAtomName

      char *string = XGetAtomName(dpy, XA_PRIMARY);
      XFree(string);

      // ChangeProperty

      XStoreName(dpy, w, "test window");

      // DeleteProperty

      XDeleteProperty(dpy, w, XA_WM_NAME);

      // GetProperty
      // TODO

      // ListProperties

      int num_prop;
      Atom *list = XListProperties(dpy, w, &num_prop);
      XFree(list);

      // SetSelectionOwner

      XSetSelectionOwner(dpy, XA_PRIMARY, w, 12000);
      XSetSelectionOwner(dpy, XA_SECONDARY, w, CurrentTime);

      // GetSelectionOwner

      Window wx = XGetSelectionOwner(dpy, XA_PRIMARY);

      // ConvertSelection

      XConvertSelection(dpy, XA_SECONDARY, XA_CURSOR, XA_POINT, w, CurrentTime);

      // SendEvent

      // GrabPointer

      fprintf(stderr,"Grabbing\n");
      int res = XGrabPointer(dpy, w, False, Button5MotionMask | PointerMotionHintMask,
			     GrabModeSync, GrabModeAsync, w, None, CurrentTime);
      XSync(dpy, False);
//      sleep(5);

      // UngrabPointer

      fprintf(stderr,"Ungrabbing\n");
      XUngrabPointer(dpy, CurrentTime);

      // GrabButton

      XGrabButton(dpy, 3, ShiftMask | ControlMask, w, False, PointerMotionHintMask | Button2MotionMask, 
		  GrabModeAsync, GrabModeSync, None, None);
		  
      XGrabButton(dpy, 2, AnyModifier, w, False, PointerMotionHintMask | Button2MotionMask, 
		  GrabModeAsync, GrabModeSync, None, None);
		  
      // UngrabButton

      XUngrabButton(dpy, 2, LockMask, w);

      // ChangeActivePointerGrab

      XChangeActivePointerGrab(dpy, ButtonPressMask, None, CurrentTime);

      // GrabKeyboard

      XGrabKeyboard(dpy, w, True, GrabModeSync, GrabModeSync, 12000);

      // UngrabKeyboard

      XUngrabKeyboard(dpy, 13000);

      // GrabKey

      XGrabKey(dpy, XKeysymToKeycode(dpy, XK_Tab), ShiftMask | Mod3Mask, w, True, GrabModeSync,
	       GrabModeSync);

      // UngrabKey

      XUngrabKey(dpy, AnyKey, AnyModifier, w);

      // AllowEvents

      XAllowEvents(dpy, AsyncPointer, 14000);

      // GrabServer

      XGrabServer(dpy);

      // UngrabServer

      XUngrabServer(dpy);

      // QueryPointer

      Window child;
      int root_x, root_y, win_x, win_y;
      unsigned mask;
      Bool bres = XQueryPointer(dpy, w, &root, &child, &root_x, &root_y, &win_x, &win_y, &mask);

      // GetMotionEvents

      int nevents;
      XGetMotionEvents(dpy, w, 15000, 16000, &nevents);

      // TranslateCoordinates

      int dest_x, dest_y;

      XTranslateCoordinates(dpy, w, w2, 10, 20, &dest_x, &dest_y, &child);

      // WarpPointer

      XWarpPointer(dpy, w, w2, 0, 0, 100, 100, 20, 30);

      // SetInputFocus

      XSetInputFocus(dpy,w, RevertToPointerRoot, 17000);
      XSetInputFocus(dpy, PointerRoot, RevertToPointerRoot, 17000);

      // GetInputFocus

      Window focus;
      int revert_to;
      XGetInputFocus(dpy, &focus, &revert_to);

      // QueryKeymap

      char keys_return[32];
      XQueryKeymap(dpy, keys_return);

      // OpenFont

      Font fid = XLoadFont(dpy, "cursor");

      // CloseFont

      XUnloadFont(dpy, fid);

      // QueryFont

      XFontStruct *fs = XLoadQueryFont(dpy, "cursor");
      assert(fs);

      // QueryTextExtents

      int direction, font_ascent, font_descent;
      XCharStruct overall;
      XQueryTextExtents(dpy, fs -> fid, "toto", 4, &direction, &font_ascent, &font_descent, &overall);
      XQueryTextExtents(dpy, fs -> fid, "odd__length", 11, &direction, &font_ascent, &font_descent, &overall);

      XChar2b c2bs;
      c2bs.byte1 = '$';
      c2bs.byte2 = 'B';
      XQueryTextExtents16(dpy, fs -> fid, &c2bs, 1, &direction, &font_ascent, &font_descent, &overall);

      XQueryTextExtents(dpy, fs -> fid, longString, strlen(longString), &direction, &font_ascent, 
			&font_descent, &overall);

      // ListFonts

      int actual_count;
      char **fontList = XListFonts(dpy, "*", 100, &actual_count);
      XFree((char *)fontList);

      // ListFontsWithInfo

      int count;
      XFontStruct *info;
      char **names = XListFontsWithInfo(dpy, "*", 100, &count, &info);
      XFreeFontInfo(names, info, count);

      // SetFontPath
      // GetFontPath

      int npaths;
      char **charList = XGetFontPath(dpy, &npaths);

      char **charList2 = new char *[npaths + 1];
      memcpy(charList2, charList, npaths * sizeof(char *));
      charList2[npaths] = charList2[0];

      XSetFontPath(dpy, charList2, npaths + 1);
      XSetFontPath(dpy, charList, npaths); // Reset to some reasonnable value

      XFreeFontPath(charList);
      delete [] charList2;

      // CreatePixmap

      Pixmap pix2 = XCreatePixmap(dpy, w, 100, 200, DefaultDepthOfScreen(scr));

      // FreePixmap

      XFreePixmap(dpy, pix2);

      // CreateGC

      Pixmap bitmap = XCreateBitmapFromData(dpy, RootWindowOfScreen(scr), 
					    "\000\000\001\000\000\001\000\000\001\377\377\377", 3, 4);

      XGCValues gcv;
      gcv.function = GXand;
      gcv.plane_mask = 0x1;
      gcv.foreground = WhitePixelOfScreen(scr);
      gcv.background = BlackPixelOfScreen(scr);
      gcv.line_width = 2;
      gcv.line_style = LineDoubleDash;
      gcv.cap_style = CapProjecting;
      gcv.join_style = JoinRound;
      gcv.fill_style = FillStippled;
      gcv.fill_rule = EvenOddRule;
      gcv.arc_mode = ArcPieSlice;
      gcv.tile = pixmap;
      gcv.stipple = bitmap;
      gcv.ts_x_origin = 3;
      gcv.ts_y_origin = 4;
      gcv.font = fs -> fid;
      gcv.subwindow_mode = ClipByChildren;
      gcv.graphics_exposures = True;
      gcv.clip_x_origin = 5;
      gcv.clip_y_origin = 6;
      gcv.clip_mask = bitmap;
      gcv.dash_offset = 1;
      gcv.dashes = 0xc2;
      
      GC gc = XCreateGC(dpy, w, 
			  GCFunction | GCPlaneMask | GCForeground | GCBackground | GCLineWidth
			| GCLineStyle | GCCapStyle | GCJoinStyle | GCFillStyle | GCFillRule | GCTile
			| GCStipple | GCTileStipXOrigin | GCTileStipYOrigin | GCFont | GCSubwindowMode
			| GCGraphicsExposures | GCClipXOrigin | GCClipYOrigin | GCClipMask | GCDashOffset
			| GCDashList | GCArcMode,
			&gcv);

      // ChangeGC

      gcv.function = GXandReverse;

      // Only a few of these should appear, since the values are cached on the client side by the Xlib.

      XChangeGC(dpy, gc, GCFunction | GCLineStyle | GCStipple | GCGraphicsExposures | GCDashList, &gcv);

      // CopyGC
      
      GC gc2 = XCreateGC(dpy, w, 0, NIL);
      XCopyGC(dpy, gc, GCFunction | GCLineStyle | GCStipple | GCGraphicsExposures | GCDashList, gc2);

      // SetDashes

      XSetDashes(dpy, gc, 3, "\001\377\001", 3);

      // SetClipRectangles

      XRectangle rectangles[] = { { 10, 20, 30, 40 }, { 100, 200, 5, 3 }, { -5, 1, 12, 24 } };
      XSetClipRectangles(dpy, gc, 12, 9, rectangles, SIZEOF(rectangles), Unsorted);

      // FreeGC

	    // done already

      // ClearArea

      XClearArea(dpy, w, 30, 10, 10, 100, False);

      // CopyArea

      XCopyArea(dpy, w, pixmap, gc, 0, 0, 100, 100, 10, 10);

      // CopyPlane

      // This won't work if the Screen doesn't have at least 3 planes
      XCopyPlane(dpy, pixmap, w, gc, 20, 10, 40, 30, 0, 0, 0x4);

      // PolyPoint

      XDrawPoint(dpy, w, gc, 1, 2);

      XPoint points[] = { { 3, 4 }, { 5, 6 } };
      XDrawPoints(dpy, w, gc, points, SIZEOF(points), CoordModeOrigin);

      // PolyLine

      XDrawLines(dpy, w, gc, points, SIZEOF(points), CoordModePrevious);

      // PolySegment

      XSegment segments[] = { { 7, 8, 9, 10 }, { 11, 12, 13, 14 }, { 15, 16, 17, 18 } };
      XDrawSegments(dpy, w, gc, segments, SIZEOF(segments));

      // PolyRectangle

      XDrawRectangles(dpy, w, gc, rectangles, SIZEOF(rectangles));

      // PolyArc

      XArc arcs[] = { { 10, 20, 30, 40, 50, 60 }, { -70, 80, 90, 100, 110, 120 }, 
		      { 10, 20, 30, 40, 50, -30 } };

      XDrawArcs(dpy, w, gc, arcs, SIZEOF(arcs));

      // FillPoly

      XFillPolygon(dpy, w, gc, points, SIZEOF(points), Convex, CoordModePrevious);

      // PolyFillRectangle
      
      XFillRectangles(dpy, w, gc, rectangles, SIZEOF(rectangles));

      // PolyFillArc
      
      XFillArcs(dpy, w, gc, arcs, SIZEOF(arcs));

      // PutImage
      // GetImage

      XImage *image = XGetImage(dpy, w, 10, 20, 40, 30, AllPlanes, ZPixmap);
      XPutImage(dpy, w, gc, image, 0, 0, 50, 60, 40, 30);
      XSync(dpy, False); // Make the next request starts at the beginning of a packet

      // PolyText8

#define STRINGL(x) x, strlen(x)
      XTextItem items[] = { STRINGL("toto"), -3, fs -> fid, 
			    STRINGL("titi"), 3, None,
			    STRINGL("tutu"), 0, fs -> fid };
      XDrawText(dpy, w, gc, 10, 10, items, SIZEOF(items));

      XTextItem items2[] = { STRINGL("totox"), -3, fs -> fid, 
			    STRINGL("titi"), 3, None,
			    STRINGL("tutu"), 0, fs -> fid };
      XDrawText(dpy, w, gc, 10, 10, items2, SIZEOF(items2));

      // PolyText16

      XChar2b c2b2[] = { 0, 't', 0, 'x' };

      XTextItem16 items16[] = { { &c2bs, 1, -5, None }, { NULL, 0, 0, None }, { c2b2, 2, 0, fs -> fid } };
      XDrawText16(dpy, w, gc, 10, 0, items16, SIZEOF(items16));

      // ImageText8

      XDrawImageString(dpy, w, gc, 10, 10, "toto", 4);

      // ImageText16

      XDrawImageString16(dpy, w, gc, 10, 10, &c2bs, 1);
      XDrawImageString16(dpy, w, gc, 10, 20, c2b2, 2);

      // CreateColormap
      // Don't forget to tell the kids how it was when we had only 8 bits per pixel.

      Colormap colormap = XCreateColormap(dpy, w, DefaultVisualOfScreen(scr), None);

      // FreeColormap

      XFreeColormap(dpy, colormap);
      colormap = XCreateColormap(dpy, w, DefaultVisualOfScreen(scr), None);

      // CopyColormapAndFree

      Colormap colormap2 = XCopyColormapAndFree(dpy, colormap);

      // InstallColormap

      XInstallColormap(dpy, colormap2);

      // UninstallColormap

      XUninstallColormap(dpy, colormap2);

      // ListInstalledColormaps

      int num;
      Colormap *colormapList = XListInstalledColormaps(dpy, w, &num);

      // AllocColor

      XColor screen;
      screen.red = 0;
      screen.green = 32767;
      screen.blue = 65535;
      screen.flags = DoRed | DoGreen | DoBlue;
      success = XAllocColor(dpy, colormap, &screen);

      // AllocNamedColor

      XColor screen2, exact;
      success = XAllocNamedColor(dpy, colormap, "Wheat", &screen2, &exact);

      // AllocColorCells

      unsigned long plane_masks, pixels;
      success = XAllocColorCells(dpy, colormap, False, &plane_masks, 1, &pixels, 1);

      // AllocColorPlanes

      unsigned long rmask, gmask, bmask;
      success = XAllocColorPlanes(dpy, colormap, False, &pixels, 1, 0, 0, 0, &rmask, &gmask, &bmask);

      // FreeColors

      unsigned long pixels2[2] = { screen.pixel, screen2.pixel };
      XFreeColors(dpy, colormap, pixels2, 2, 0);

      // StoreColors

      success = XAllocColorCells(dpy, colormap, False, NIL, 0, pixels2, 2);

      // On many contemporary (that is, year 2000) video cards, you can't allocate read / write cells
      // I want my requests to be sent, however.
      if (!success) {
	    XSetErrorHandler(errorHandler);
      }

      XColor colors[2];
      colors[0] = screen;  colors[0].pixel = pixels2[0];
      colors[1] = screen2; colors[1].pixel = pixels2[1];
      XStoreColors(dpy, colormap, colors, 2);

      // StoreNamedColor

      XStoreNamedColor(dpy, colormap, "Wheat", colors[0].pixel, DoBlue);

      XSync(dpy, False);
      XSetErrorHandler(NIL); // Restore the default handler

      // QueryColors

      screen2.pixel = WhitePixelOfScreen(scr);
      XQueryColor(dpy, colormap, &screen2);

      // LookupColor

      success = XLookupColor(dpy, colormap, "DarkCyan", &exact, &screen);

      // CreateCursor

      Cursor cursor = XCreatePixmapCursor(dpy, pixmap, None, &exact, colors, 10, 10);

      // CreateGlyphCursor
      
      Cursor cursor2 = XCreateGlyphCursor(dpy, fs -> fid, fs -> fid, 'X', 0, &exact, colors);

      // FreeCursor
      
      XFreeCursor(dpy, cursor2);

      // RecolorCursor

      XRecolorCursor(dpy, cursor, colors, &exact);

      // QueryBestSize

      success = XQueryBestSize(dpy, CursorShape, RootWindowOfScreen(scr), 100, 20, &width, &height);

      // QueryExtension

      int major_opcode, first_event, first_error;
      XQueryExtension(dpy, "toto", &major_opcode, &first_event, &first_error);

      // ListExtensions

      int nextensions;
      char **extensionList = XListExtensions(dpy, &nextensions);
      for(char **p = extensionList; nextensions; nextensions--, p++) 
            fprintf(stdout,"%p\n",*p);
      XFree(extensionList);

      // ChangeKeyboardMapping
      // GetKeyboardMapping

      int min_keycodes, max_keycodes;
      XDisplayKeycodes(dpy, &min_keycodes, &max_keycodes);

      int keysyms_per_keycode;
      KeySym *keysyms = XGetKeyboardMapping(dpy, min_keycodes, max_keycodes - min_keycodes + 1,
					    &keysyms_per_keycode);
      XChangeKeyboardMapping(dpy, min_keycodes, keysyms_per_keycode, keysyms, 
			     max_keycodes - min_keycodes + 1);

      // ChangeKeyboardControl
      // GetKeyboardControl

      XKeyboardState keyboardState;
      XGetKeyboardControl(dpy, &keyboardState);

      XKeyboardControl keyboardValues;
      keyboardValues.key_click_percent = keyboardState.key_click_percent;
      keyboardValues.bell_percent = keyboardState.bell_percent;
      keyboardValues.bell_pitch = keyboardState.bell_pitch;
      keyboardValues.bell_duration = keyboardState.bell_duration;
      keyboardValues.led = 1;
      keyboardValues.led_mode = LedModeOn;
      keyboardValues.key = min_keycodes;
      keyboardValues.auto_repeat_mode = AutoRepeatModeDefault;
      XChangeKeyboardControl(dpy, 
			       KBKeyClickPercent | KBBellPercent | KBBellPitch | KBBellDuration
			     | KBLed | KBLedMode | KBKey | KBAutoRepeatMode,
			     &keyboardValues);

      // Bell

      XBell(dpy, 90);

      // ChangePointerControl
      // GetPointerControl

      int accel_numerator, accel_denominator, threshold;
      XGetPointerControl(dpy, &accel_numerator, &accel_denominator, &threshold);

      XChangePointerControl(dpy, True, True, accel_numerator, accel_denominator, threshold);

      // SetScreenSaver
      // GetScreenSaver

      int timeout, interval, prefer_blanking, allow_exposures;
      XGetScreenSaver(dpy, &timeout, &interval, &prefer_blanking, &allow_exposures);
      XSetScreenSaver(dpy, timeout, interval, prefer_blanking, allow_exposures);

      // ChangeHosts
      // ListHosts

      int nhosts;
      Bool state;
      XHostAddress *hostList = XListHosts(dpy, &nhosts, &state);

      XHostAddress host;
      host.family = FamilyInternet;
      host.length = 4;
      host.address = "\001\002\003\004";
      XAddHost(dpy, &host);

      // SetAccessControl

      XSetAccessControl(dpy, EnableAccess);

      // SetCloseDownMode

      XSetCloseDownMode(dpy, RetainTemporary);

      // KillClient

      XKillClient(dpy, AllTemporary);

      // RotateProperties

      Atom properties[] = { XInternAtom(dpy, "CUT_BUFFER0", False), 
			    XInternAtom(dpy, "CUT_BUFFER1", False),
			    XInternAtom(dpy, "CUT_BUFFER2", False) };
      XRotateWindowProperties(dpy, RootWindowOfScreen(scr), properties, SIZEOF(properties), -1);

      // ForceScreenSaver

      XForceScreenSaver(dpy, ScreenSaverReset);

      // SetPointerMapping
      // GetPointerMapping

      unsigned char map[64];
      int map_length = XGetPointerMapping(dpy, map, 64);
      XSetPointerMapping(dpy, map, map_length);

      // SetModifierMapping
      // GetModifierMapping

      XModifierKeymap *modmap = XGetModifierMapping(dpy);
      XSetModifierMapping(dpy, modmap);

      // NoOperation

      XNoOp(dpy);

      for(;;) {
	    XEvent e;
	    XNextEvent(dpy, &e);
          fprintf(stdout,"Got an event of type %d\n",e.type);
	 //   cout << "Got an event of type " << e.type << endl;
      }
}
