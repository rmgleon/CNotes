/* nuklear - v1.32.0 - public domain */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <limits.h>
#include <math.h>
#include <sys/time.h>
#include <unistd.h>
#include <time.h>
#include <X11/Xatom.h> // Para el portapapeles

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_IMPLEMENTATION
#define NK_XLIB_IMPLEMENTATION
#include "../nuklear/nuklear.h"
#include "../nuklear/nuklear_xlib.h"

// Hecho por nosotros
#include "file_actions.h"
#include "list_actions.h"


#define DTIME           20
#define WINDOW_WIDTH    800
#define WINDOW_HEIGHT   600

// Variables globales

char text[1024 * 16] = {0}; /* Buffer for text input */
char title[100] = {0}; // Titulo
int text_len = 0;
int title_len = 0;
char title_save_buf[100];


typedef struct XWindow XWindow;
struct XWindow {
    Display *dpy;
    Window root;
    Visual *vis;
    Colormap cmap;
    XWindowAttributes attr;
    XSetWindowAttributes swa;
    Window win;
    int screen;
    XFont *font;
    unsigned int width;
    unsigned int height;
    Atom wm_delete_window;
    Atom clipboard; // Atom para el portapapeles
    Atom targets;   // Atom para los objetivos del portapapeles
    Atom utf8_string; // Atom para UTF8_STRING
};

void cleanup(XWindow *xw);

void layout(struct nk_context *ctx, int windowWidth) { // barra de menu en la parte arriba
      
    // Iniciar la ventana del menú en la parte superior de la ventana
    if (nk_begin(ctx, "Menu", nk_rect(0, 0, windowWidth, 40), 0)) {
        // Iniciar la barra de menú
        nk_menubar_begin(ctx);
        {
            // Definir una fila estática de 30 de altura, con elementos de 40 de ancho y 5 de espacio
            nk_layout_row_static(ctx, 30, 40, 5);

            // Comenzar el menú "File"
            if (nk_menu_begin_label(ctx, "File", NK_TEXT_LEFT, nk_vec2(200, 200))) {
                // Definir una fila dinámica de 25 de altura con 1 elemento por fila
                nk_layout_row_dynamic(ctx, 25, 1);

                // Definir elementos del menú
                if (nk_menu_item_label(ctx, "New", NK_TEXT_LEFT)) {
                    // Acción para "New"
                }
                if (nk_menu_item_label(ctx, "Open", NK_TEXT_LEFT)) {
                    // Acción para "Open"
                }
                if (nk_menu_item_label(ctx, "Save", NK_TEXT_LEFT)) {       
                    strcpy(title_save_buf, title);
                    save_text(text, title_save_buf);
                }
                if (nk_menu_item_label(ctx, "Exit", NK_TEXT_LEFT)) {
                    // Acción para "Exit"
                    exit(0);
                }
                // Terminar el menú "File"
                nk_menu_end(ctx);
            }
            
            // Comenzar el menú "Search"
            if (nk_menu_begin_label(ctx, "Search", NK_TEXT_LEFT, nk_vec2(200, 200))) {
                // Definir una fila dinámica de 25 de altura con 1 elemento por fila
                nk_layout_row_dynamic(ctx, 25, 1);

                // Definir elementos del menú
                if (nk_menu_item_label(ctx, "Search", NK_TEXT_LEFT)) {
                    // Acción para "Search"
                }

                if (nk_menu_item_label(ctx, "hash search", NK_TEXT_LEFT)) {
                // Acción para "Search"
                }

                // Terminar el menú "Search"
                nk_menu_end(ctx);
            }
        }
        // Terminar la barra de menú
        nk_menubar_end(ctx);
    }
    // Terminar la ventana del menú
    nk_end(ctx);
}

static void die(const char *fmt, ...){
    va_list ap;
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);
    fputs("\n", stderr);
    exit(EXIT_FAILURE);
}

static long timestamp(void){
    struct timeval tv;
    if (gettimeofday(&tv, NULL) < 0) return 0;
    return (long)((long)tv.tv_sec * 1000 + (long)tv.tv_usec/1000);
}

static void sleep_for(long t){
    struct timespec req;
    const time_t sec = (int)(t/1000);
    const long ms = t - (sec * 1000);
    req.tv_sec = sec;
    req.tv_nsec = ms * 1000000L;
    while(-1 == nanosleep(&req, &req));
}

// Función para copiar texto al portapapeles
void copy_to_clipboard(XWindow *xw, const char *text) {
    XSetSelectionOwner(xw->dpy, xw->clipboard, xw->win, CurrentTime);
    if (XGetSelectionOwner(xw->dpy, xw->clipboard) != xw->win) {
        fprintf(stderr, "Failed to set clipboard owner\n");
        return;
    }
    XChangeProperty(xw->dpy, xw->win, xw->clipboard, xw->utf8_string, 8,
                    PropModeReplace, (unsigned char*)text, strlen(text));
}

// Función para pegar texto del portapapeles
char* paste_from_clipboard(XWindow *xw) {
    Atom actual_type;
    int actual_format;
    unsigned long nitems, bytes_after;
    unsigned char *data = NULL;
    XConvertSelection(xw->dpy, xw->clipboard, xw->utf8_string, xw->clipboard, xw->win, CurrentTime);
    XGetWindowProperty(xw->dpy, xw->win, xw->clipboard, 0, LONG_MAX / 4, False, AnyPropertyType,
                       &actual_type, &actual_format, &nitems, &bytes_after, &data);
    return (char*)data;
}

int main(void){
    // Startup Stuff
    system("mkdir notes");
    list_node* list=NULL;


    long dt;
    long started;
    int running = 1;
    XWindow xw;
    struct nk_context *ctx;

    /* X11 */
    memset(&xw, 0, sizeof xw);
    xw.dpy = XOpenDisplay(NULL);
    if (!xw.dpy) die("Could not open a display; perhaps $DISPLAY is not set?");
    xw.root = DefaultRootWindow(xw.dpy);
    xw.screen = XDefaultScreen(xw.dpy);
    xw.vis = XDefaultVisual(xw.dpy, xw.screen);
    xw.cmap = XCreateColormap(xw.dpy, xw.root, xw.vis, AllocNone);

    xw.swa.colormap = xw.cmap;
    xw.swa.event_mask =
        ExposureMask | KeyPressMask | KeyReleaseMask |
        ButtonPress | ButtonReleaseMask | ButtonMotionMask |
        Button1MotionMask | Button3MotionMask | Button4MotionMask | Button5MotionMask |
        PointerMotionMask | KeymapStateMask;
    xw.win = XCreateWindow(xw.dpy, xw.root, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0,
        XDefaultDepth(xw.dpy, xw.screen), InputOutput,
        xw.vis, CWEventMask | CWColormap, &xw.swa);

    XStoreName(xw.dpy, xw.win, "Text Editor");
    XMapWindow(xw.dpy, xw.win);
    xw.wm_delete_window = XInternAtom(xw.dpy, "WM_DELETE_WINDOW", False);
    XSetWMProtocols(xw.dpy, xw.win, &xw.wm_delete_window, 1);
    XGetWindowAttributes(xw.dpy, xw.win, &xw.attr);
    xw.width = (unsigned int)xw.attr.width;
    xw.height = (unsigned int)xw.attr.height;
    xw.clipboard = XInternAtom(xw.dpy, "CLIPBOARD", False);
    xw.targets = XInternAtom(xw.dpy, "TARGETS", False);
    xw.utf8_string = XInternAtom(xw.dpy, "UTF8_STRING", False);

    /* GUI */
    xw.font = nk_xfont_create(xw.dpy, "fixed");
    ctx = nk_xlib_init(xw.font, xw.dpy, xw.screen, xw.win, xw.width, xw.height);

    while (running){
        /* Input */
        XEvent evt;
        started = timestamp();
        nk_input_begin(ctx);
        while (XPending(xw.dpy)) {
            XNextEvent(xw.dpy, &evt);
            if (evt.type == ClientMessage) cleanup(&xw);
            if (XFilterEvent(&evt, xw.win)) continue;
            if (evt.type == KeyPress) {
                KeySym keysym = XLookupKeysym(&evt.xkey, 0);
                if ((evt.xkey.state & ControlMask) && keysym == XK_c) {
                    // Copiar texto al portapapeles
                    copy_to_clipboard(&xw, text);
                } else if ((evt.xkey.state & ControlMask) && keysym == XK_v) {
                    // Pegar texto del portapapeles
                    char *clipboard_text = paste_from_clipboard(&xw);
                    if (clipboard_text) {
                        strncat(text, clipboard_text, sizeof(text) - text_len - 1);
                        text_len = text_len + sizeof(clipboard_text);
                        XFree(clipboard_text);
                    }
                }
            }
            nk_xlib_handle_event(xw.dpy, xw.screen, xw.win, &evt);
        }
        nk_input_end(ctx);

        /* GUI */
        layout(ctx,WINDOW_WIDTH); // Llamada a la función de diseño del menú
       // layout2(ctx);

        if (nk_begin(ctx, "Text Editor", nk_rect(50, 50, 700, 700),
            NK_WINDOW_BORDER | NK_WINDOW_MOVABLE |
            NK_WINDOW_SCALABLE | NK_WINDOW_MINIMIZABLE |
            NK_WINDOW_TITLE)){    
                nk_layout_row_dynamic(ctx, 25, 1);
                nk_edit_string(ctx, NK_EDIT_BOX, title, &title_len, sizeof(title), nk_filter_default);

                nk_layout_row_begin(ctx, NK_DYNAMIC, 400, 2); // Begin a new row with dynamic width
                //nk_layout_row_dynamic(ctx, 400, 1); // Ensure enough space for the tex
                nk_layout_row_push(ctx, 0.7f); // Push 70% of the space for the text editor
                nk_edit_string(ctx, NK_EDIT_BOX | NK_EDIT_MULTILINE | NK_EDIT_AUTO_SELECT, text, &text_len, sizeof(text), nk_filter_default);

                nk_layout_row_push(ctx, 0.3f); // Push 30% of the space for the group


                if (nk_group_begin(ctx, "Group_With_Border", NK_WINDOW_BORDER)) {
                    int i = 0;
                    char buffer[64];
                    nk_layout_row_dynamic(ctx, 25, 2);
                    for (i = 0; i < 64; ++i) {
                        sprintf(buffer, "%08d", ((((i % 7) * 10) ^ 32)) + (64 + (i % 2) * 2));
                        nk_button_label(ctx, buffer);
                    }
                    nk_group_end(ctx);
                }
                nk_layout_row_end(ctx);

        }
       
       
        nk_layout_row_static(ctx, 30, 80, 2);
        if (nk_button_label(ctx, "Save")){
            strcpy(title_save_buf, title);
            save_text(text, title_save_buf);
        }
        if (nk_button_label(ctx, "Save to list")){
            add_list_node(title, &list);
        }
        if (nk_button_label(ctx, "Show list")){
            show_list_nodes(&list);
        }
        if (nk_button_label(ctx, "Save list.txt")){
            save_list_nodes(&list);
        }
        if (nk_button_label(ctx, "Load list.txt")){
            load_list_nodes(&list);
        }


        nk_end(ctx);
        if (nk_window_is_hidden(ctx, "Text Editor")) break;

        /* Draw */
        XClearWindow(xw.dpy, xw.win);
        nk_xlib_render(xw.win, nk_rgb(30, 30, 30));
        XFlush(xw.dpy);

        /* Timing */
        dt = timestamp() - started;
        if (dt < DTIME)
            sleep_for(DTIME - dt);
    }

}
void cleanup(XWindow *xw){
    nk_xfont_del((*xw).dpy, (*xw).font);
    nk_xlib_shutdown();
    XUnmapWindow((*xw).dpy, (*xw).win);
    XFreeColormap((*xw).dpy, (*xw).cmap);
    XDestroyWindow((*xw).dpy, (*xw).win);
    XCloseDisplay((*xw).dpy);
}
