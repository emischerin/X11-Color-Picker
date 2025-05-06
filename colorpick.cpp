#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <iostream>
#include <string>



void DrawTitle(Display* dpy,Window& w, GC& gc)
{
        XWindowAttributes xwattr;
        XGetWindowAttributes(dpy,w,&xwattr);

        int x_center = (xwattr.width / 2) - 20;
        int t_height = 15;

        std::string title = "X COLOR PICKER";
        XDrawString(dpy,w,gc,x_center + 3,t_height,title.c_str(),title.size());
        int bottom_right_x = x_center + 86;
        /*Border of title*/
        XDrawLine(dpy,w,gc,x_center - 4,0,x_center - 4,18);
        XDrawLine(dpy,w,gc,bottom_right_x + 6,0,bottom_right_x + 6,18);
        XDrawLine(dpy,w,gc,x_center - 4,18,bottom_right_x + 6,18);


}

void EnsureWindowOnTop(Display* dpy,Window& w, GC& gc)
{

}

void GetNPrintMousePos(Display* dpy,Window& w, GC& gc)
{
    Window root;
    Window child;
    int root_x,root_y;
    int child_x,child_y;
    unsigned int mask;

    XQueryPointer(dpy,w,&root,&child,&root_x,&root_y,&child_x,&child_y,&mask);

    const std::string block_title = "MOUSE POSITION";
    const std::string rx = "Root Window X:";
    const std::string ry = "Root Window y:";
    const std::string cx = "Window X:";
    const std::string cy = "Window Y:";

    std::string lrx = std::to_string(root_x); /*l for label*/
    std::string lry = std::to_string(root_y);
    std::string lcx = std::to_string(child_x);
    std::string lcy = std::to_string(child_y);

    XDrawString(dpy,w,gc,31,50,block_title.c_str(),block_title.size());

    XDrawString(dpy,w,gc,12,70,rx.c_str(),rx.size());
    XDrawString(dpy,w,gc,12,90,ry.c_str(),ry.size());
    XDrawString(dpy,w,gc,12,110,cx.c_str(),cx.size());
    XDrawString(dpy,w,gc,12,130,cy.c_str(),cy.size());

    XDrawString(dpy,w,gc,101,70,lrx.c_str(),lrx.size());
    XDrawString(dpy,w,gc,101,90,lry.c_str(),lry.size());
    XDrawString(dpy,w,gc,101,110,lcx.c_str(),lcx.size());
    XDrawString(dpy,w,gc,101,130,lcy.c_str(),lcy.size());

    XDrawLine(dpy,w,gc,9,35,130,35);
    XDrawLine(dpy,w,gc,9,35,9,140);
    XDrawLine(dpy,w,gc,10,52,130,52);
    XDrawLine(dpy,w,gc,130,35,130,140);
    XDrawLine(dpy,w,gc,10,140,130,140);

    const std::string sample = "SAMPLE";


}

void GetNPrintColorUnderCursor(Display* dpy,Window& w, GC& gc)
{
    Window root;
    Window child;
    int root_x,root_y;
    int child_x,child_y;
    unsigned int mask;

    XQueryPointer(dpy,w,&root,&child,&root_x,&root_y,&child_x,&child_y,&mask);

    XColor c;
    XImage *image;
    image = XGetImage (dpy, XRootWindow (dpy, XDefaultScreen (dpy)), root_x, root_y, 1, 1, AllPlanes, XYPixmap);
    if (image == NULL) {
        XCloseDisplay(dpy);
        return;
    }

    c.pixel = XGetPixel (image, 0, 0);
    XFree (image);
    XQueryColor (dpy, XDefaultColormap(dpy, XDefaultScreen (dpy)), &c);

    XSetForeground(dpy, gc, c.pixel);
    XFillRectangle(dpy,w,gc,150,200,100,100);

    XSetForeground(dpy,gc,0);

    const std::string rgb_color = "RGB COLOR";
    const std::string r = "R:";
    const std::string g = "G:";
    const std::string b = "B:";

    XDrawLine(dpy,w,gc,150,35,250,35);
    XDrawLine(dpy,w,gc,150,52,250,52);
    XDrawString(dpy,w,gc,175,50,rgb_color.c_str(),rgb_color.size());
    XDrawLine(dpy,w,gc,150,35,150,140);
    XDrawLine(dpy,w,gc,250,35,250,140);



    XDrawString(dpy,w,gc,155,65,r.c_str(),r.size());
    XDrawString(dpy,w,gc,155,80,g.c_str(),g.size());
    XDrawString(dpy,w,gc,155,95,b.c_str(),b.size());

    std::string rv = std::to_string(c.red);
    std::string gv = std::to_string(c.green);
    std::string bv = std::to_string(c.blue);

    XDrawString(dpy,w,gc,170,65,rv.c_str(),r.size());
    XDrawString(dpy,w,gc,170,80,gv.c_str(),g.size());
    XDrawString(dpy,w,gc,170,95,bv.c_str(),b.size());

    XSetForeground(dpy,gc,0);
    XDrawLine(dpy,w,gc,150,140,250,140);

}

int main(int argc,char** argv)
{
    Display * dpy = 0;
    Window w;
    XEvent ev;
    GC gc;
    int s;

    if(!(dpy = XOpenDisplay(0x0))) {
        std::cout << "DISPLAY FAILED TO OPEN!" << std::endl;
        return -1;
    }

    s = DefaultScreen(dpy);

    w = XCreateSimpleWindow(dpy,RootWindow(dpy,s),300,300,500,350,1,
                            BlackPixel(dpy,s),WhitePixel(dpy,s));

    XSelectInput(dpy, w, ExposureMask | KeyPressMask | PointerMotionMask);
    XMapWindow(dpy, w);

    gc = XCreateGC(dpy,w,0,0);

    // struct timespec rem;
    // struct timespec req = {0,160000000};


    for(;;)
    {
        XNextEvent(dpy, &ev);
        //XPeekEvent(dpy,&ev);

        if(ev.type == KeyPress){
            if(ev.xkey.keycode == 9){ /*ESCAPE*/
                goto exit;
            }

        }
        XClearWindow(dpy,w);
        DrawTitle(dpy,w,gc);
        GetNPrintMousePos(dpy,w,gc);
        GetNPrintColorUnderCursor(dpy,w,gc);
        XFlush(dpy);



        // nanosleep(&req,&rem);
        // if(rem.tv_nsec < 1000)
        //     nanosleep(&rem,NULL);



    }
exit:
    XCloseDisplay(dpy);
    return 0;

}
