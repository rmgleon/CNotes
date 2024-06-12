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
#include "../src/file_actions.h"
//#include "../src/style.c"

#define DTIME           20
#define WINDOW_WIDTH    1248
#define WINDOW_HEIGHT   640

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

void cleanup(XWindow *xw){
    nk_xfont_del((*xw).dpy, (*xw).font);
    nk_xlib_shutdown();
    XUnmapWindow((*xw).dpy, (*xw).win);
    XFreeColormap((*xw).dpy, (*xw).cmap);
    XDestroyWindow((*xw).dpy, (*xw).win);
    XCloseDisplay((*xw).dpy);
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

// style table
enum theme {THEME_BLACK, THEME_WHITE, THEME_RED, THEME_BLUE, THEME_DARK, THEME_DRACULA};

static void set_style(struct nk_context *ctx, enum theme theme)
{
    struct nk_color table[NK_COLOR_COUNT];
    if (theme == THEME_WHITE) {
        table[NK_COLOR_TEXT] = nk_rgba(70, 70, 70, 255);
        table[NK_COLOR_WINDOW] = nk_rgba(175, 175, 175, 255);
        table[NK_COLOR_HEADER] = nk_rgba(175, 175, 175, 255);
        table[NK_COLOR_BORDER] = nk_rgba(0, 0, 0, 255);
        table[NK_COLOR_BUTTON] = nk_rgba(185, 185, 185, 255);
        table[NK_COLOR_BUTTON_HOVER] = nk_rgba(170, 170, 170, 255);
        table[NK_COLOR_BUTTON_ACTIVE] = nk_rgba(160, 160, 160, 255);
        table[NK_COLOR_TOGGLE] = nk_rgba(150, 150, 150, 255);
        table[NK_COLOR_TOGGLE_HOVER] = nk_rgba(120, 120, 120, 255);
        table[NK_COLOR_TOGGLE_CURSOR] = nk_rgba(175, 175, 175, 255);
        table[NK_COLOR_SELECT] = nk_rgba(190, 190, 190, 255);
        table[NK_COLOR_SELECT_ACTIVE] = nk_rgba(175, 175, 175, 255);
        table[NK_COLOR_SLIDER] = nk_rgba(190, 190, 190, 255);
        table[NK_COLOR_SLIDER_CURSOR] = nk_rgba(80, 80, 80, 255);
        table[NK_COLOR_SLIDER_CURSOR_HOVER] = nk_rgba(70, 70, 70, 255);
        table[NK_COLOR_SLIDER_CURSOR_ACTIVE] = nk_rgba(60, 60, 60, 255);
        table[NK_COLOR_PROPERTY] = nk_rgba(175, 175, 175, 255);
        table[NK_COLOR_EDIT] = nk_rgba(150, 150, 150, 255);
        table[NK_COLOR_EDIT_CURSOR] = nk_rgba(0, 0, 0, 255);
        table[NK_COLOR_COMBO] = nk_rgba(175, 175, 175, 255);
        table[NK_COLOR_CHART] = nk_rgba(160, 160, 160, 255);
        table[NK_COLOR_CHART_COLOR] = nk_rgba(45, 45, 45, 255);
        table[NK_COLOR_CHART_COLOR_HIGHLIGHT] = nk_rgba( 255, 0, 0, 255);
        table[NK_COLOR_SCROLLBAR] = nk_rgba(180, 180, 180, 255);
        table[NK_COLOR_SCROLLBAR_CURSOR] = nk_rgba(140, 140, 140, 255);
        table[NK_COLOR_SCROLLBAR_CURSOR_HOVER] = nk_rgba(150, 150, 150, 255);
        table[NK_COLOR_SCROLLBAR_CURSOR_ACTIVE] = nk_rgba(160, 160, 160, 255);
        table[NK_COLOR_TAB_HEADER] = nk_rgba(180, 180, 180, 255);
        nk_style_from_table(ctx, table);
    } else if (theme == THEME_RED) {
        table[NK_COLOR_TEXT] = nk_rgba(190, 190, 190, 255);
        table[NK_COLOR_WINDOW] = nk_rgba(30, 33, 40, 215);
        table[NK_COLOR_HEADER] = nk_rgba(181, 45, 69, 220);
        table[NK_COLOR_BORDER] = nk_rgba(51, 55, 67, 255);
        table[NK_COLOR_BUTTON] = nk_rgba(181, 45, 69, 255);
        table[NK_COLOR_BUTTON_HOVER] = nk_rgba(190, 50, 70, 255);
        table[NK_COLOR_BUTTON_ACTIVE] = nk_rgba(195, 55, 75, 255);
        table[NK_COLOR_TOGGLE] = nk_rgba(51, 55, 67, 255);
        table[NK_COLOR_TOGGLE_HOVER] = nk_rgba(45, 60, 60, 255);
        table[NK_COLOR_TOGGLE_CURSOR] = nk_rgba(181, 45, 69, 255);
        table[NK_COLOR_SELECT] = nk_rgba(51, 55, 67, 255);
        table[NK_COLOR_SELECT_ACTIVE] = nk_rgba(181, 45, 69, 255);
        table[NK_COLOR_SLIDER] = nk_rgba(51, 55, 67, 255);
        table[NK_COLOR_SLIDER_CURSOR] = nk_rgba(181, 45, 69, 255);
        table[NK_COLOR_SLIDER_CURSOR_HOVER] = nk_rgba(186, 50, 74, 255);
        table[NK_COLOR_SLIDER_CURSOR_ACTIVE] = nk_rgba(191, 55, 79, 255);
        table[NK_COLOR_PROPERTY] = nk_rgba(51, 55, 67, 255);
        table[NK_COLOR_EDIT] = nk_rgba(51, 55, 67, 225);
        table[NK_COLOR_EDIT_CURSOR] = nk_rgba(190, 190, 190, 255);
        table[NK_COLOR_COMBO] = nk_rgba(51, 55, 67, 255);
        table[NK_COLOR_CHART] = nk_rgba(51, 55, 67, 255);
        table[NK_COLOR_CHART_COLOR] = nk_rgba(170, 40, 60, 255);
        table[NK_COLOR_CHART_COLOR_HIGHLIGHT] = nk_rgba( 255, 0, 0, 255);
        table[NK_COLOR_SCROLLBAR] = nk_rgba(30, 33, 40, 255);
        table[NK_COLOR_SCROLLBAR_CURSOR] = nk_rgba(64, 84, 95, 255);
        table[NK_COLOR_SCROLLBAR_CURSOR_HOVER] = nk_rgba(70, 90, 100, 255);
        table[NK_COLOR_SCROLLBAR_CURSOR_ACTIVE] = nk_rgba(75, 95, 105, 255);
        table[NK_COLOR_TAB_HEADER] = nk_rgba(181, 45, 69, 220);
        nk_style_from_table(ctx, table);
    } else if (theme == THEME_BLUE) {
        table[NK_COLOR_TEXT] = nk_rgba(20, 20, 20, 255);
        table[NK_COLOR_WINDOW] = nk_rgba(202, 212, 214, 215);
        table[NK_COLOR_HEADER] = nk_rgba(137, 182, 224, 220);
        table[NK_COLOR_BORDER] = nk_rgba(140, 159, 173, 255);
        table[NK_COLOR_BUTTON] = nk_rgba(137, 182, 224, 255);
        table[NK_COLOR_BUTTON_HOVER] = nk_rgba(142, 187, 229, 255);
        table[NK_COLOR_BUTTON_ACTIVE] = nk_rgba(147, 192, 234, 255);
        table[NK_COLOR_TOGGLE] = nk_rgba(177, 210, 210, 255);
        table[NK_COLOR_TOGGLE_HOVER] = nk_rgba(182, 215, 215, 255);
        table[NK_COLOR_TOGGLE_CURSOR] = nk_rgba(137, 182, 224, 255);
        table[NK_COLOR_SELECT] = nk_rgba(177, 210, 210, 255);
        table[NK_COLOR_SELECT_ACTIVE] = nk_rgba(137, 182, 224, 255);
        table[NK_COLOR_SLIDER] = nk_rgba(177, 210, 210, 255);
        table[NK_COLOR_SLIDER_CURSOR] = nk_rgba(137, 182, 224, 245);
        table[NK_COLOR_SLIDER_CURSOR_HOVER] = nk_rgba(142, 188, 229, 255);
        table[NK_COLOR_SLIDER_CURSOR_ACTIVE] = nk_rgba(147, 193, 234, 255);
        table[NK_COLOR_PROPERTY] = nk_rgba(210, 210, 210, 255);
        table[NK_COLOR_EDIT] = nk_rgba(210, 210, 210, 225);
        table[NK_COLOR_EDIT_CURSOR] = nk_rgba(20, 20, 20, 255);
        table[NK_COLOR_COMBO] = nk_rgba(210, 210, 210, 255);
        table[NK_COLOR_CHART] = nk_rgba(210, 210, 210, 255);
        table[NK_COLOR_CHART_COLOR] = nk_rgba(137, 182, 224, 255);
        table[NK_COLOR_CHART_COLOR_HIGHLIGHT] = nk_rgba( 255, 0, 0, 255);
        table[NK_COLOR_SCROLLBAR] = nk_rgba(190, 200, 200, 255);
        table[NK_COLOR_SCROLLBAR_CURSOR] = nk_rgba(64, 84, 95, 255);
        table[NK_COLOR_SCROLLBAR_CURSOR_HOVER] = nk_rgba(70, 90, 100, 255);
        table[NK_COLOR_SCROLLBAR_CURSOR_ACTIVE] = nk_rgba(75, 95, 105, 255);
        table[NK_COLOR_TAB_HEADER] = nk_rgba(156, 193, 220, 255);
        nk_style_from_table(ctx, table);
    } else if (theme == THEME_DARK) {
        table[NK_COLOR_TEXT] = nk_rgba(210, 210, 210, 255);
        table[NK_COLOR_WINDOW] = nk_rgba(57, 67, 71, 215);
        table[NK_COLOR_HEADER] = nk_rgba(51, 51, 56, 220);
        table[NK_COLOR_BORDER] = nk_rgba(46, 46, 46, 255);
        table[NK_COLOR_BUTTON] = nk_rgba(48, 83, 111, 255);
        table[NK_COLOR_BUTTON_HOVER] = nk_rgba(58, 93, 121, 255);
        table[NK_COLOR_BUTTON_ACTIVE] = nk_rgba(63, 98, 126, 255);
        table[NK_COLOR_TOGGLE] = nk_rgba(50, 58, 61, 255);
        table[NK_COLOR_TOGGLE_HOVER] = nk_rgba(45, 53, 56, 255);
        table[NK_COLOR_TOGGLE_CURSOR] = nk_rgba(48, 83, 111, 255);
        table[NK_COLOR_SELECT] = nk_rgba(57, 67, 61, 255);
        table[NK_COLOR_SELECT_ACTIVE] = nk_rgba(48, 83, 111, 255);
        table[NK_COLOR_SLIDER] = nk_rgba(50, 58, 61, 255);
        table[NK_COLOR_SLIDER_CURSOR] = nk_rgba(48, 83, 111, 245);
        table[NK_COLOR_SLIDER_CURSOR_HOVER] = nk_rgba(53, 88, 116, 255);
        table[NK_COLOR_SLIDER_CURSOR_ACTIVE] = nk_rgba(58, 93, 121, 255);
        table[NK_COLOR_PROPERTY] = nk_rgba(50, 58, 61, 255);
        table[NK_COLOR_EDIT] = nk_rgba(50, 58, 61, 225);
        table[NK_COLOR_EDIT_CURSOR] = nk_rgba(210, 210, 210, 255);
        table[NK_COLOR_COMBO] = nk_rgba(50, 58, 61, 255);
        table[NK_COLOR_CHART] = nk_rgba(50, 58, 61, 255);
        table[NK_COLOR_CHART_COLOR] = nk_rgba(48, 83, 111, 255);
        table[NK_COLOR_CHART_COLOR_HIGHLIGHT] = nk_rgba(255, 0, 0, 255);
        table[NK_COLOR_SCROLLBAR] = nk_rgba(50, 58, 61, 255);
        table[NK_COLOR_SCROLLBAR_CURSOR] = nk_rgba(48, 83, 111, 255);
        table[NK_COLOR_SCROLLBAR_CURSOR_HOVER] = nk_rgba(53, 88, 116, 255);
        table[NK_COLOR_SCROLLBAR_CURSOR_ACTIVE] = nk_rgba(58, 93, 121, 255);
        table[NK_COLOR_TAB_HEADER] = nk_rgba(48, 83, 111, 255);
        nk_style_from_table(ctx, table);
    } else if (theme == THEME_DRACULA) {
        struct nk_color background = nk_rgba(40, 42, 54, 255);
        struct nk_color currentline = nk_rgba(68, 71, 90, 255);
        struct nk_color foreground = nk_rgba(248, 248, 242, 255);
        struct nk_color comment = nk_rgba(98, 114, 164, 255);
        struct nk_color pink = nk_rgba(255, 121, 198, 255);
        struct nk_color purple = nk_rgba(189, 147, 249, 255);
        table[NK_COLOR_TEXT] = foreground;
        table[NK_COLOR_WINDOW] = background;
        table[NK_COLOR_HEADER] = currentline;
        table[NK_COLOR_BORDER] = currentline;
        table[NK_COLOR_BUTTON] = currentline;
        table[NK_COLOR_BUTTON_HOVER] = comment;
        table[NK_COLOR_BUTTON_ACTIVE] = purple;
        table[NK_COLOR_TOGGLE] = currentline;
        table[NK_COLOR_TOGGLE_HOVER] = comment;
        table[NK_COLOR_TOGGLE_CURSOR] = pink;
        table[NK_COLOR_SELECT] = currentline;
        table[NK_COLOR_SELECT_ACTIVE] = comment;
        table[NK_COLOR_SLIDER] = background;
        table[NK_COLOR_SLIDER_CURSOR] = currentline;
        table[NK_COLOR_SLIDER_CURSOR_HOVER] = comment;
        table[NK_COLOR_SLIDER_CURSOR_ACTIVE] = comment;
        table[NK_COLOR_PROPERTY] = currentline;
        table[NK_COLOR_EDIT] = currentline;
        table[NK_COLOR_EDIT_CURSOR] = foreground;
        table[NK_COLOR_COMBO] = currentline;
        table[NK_COLOR_CHART] = currentline;
        table[NK_COLOR_CHART_COLOR] = comment;
        table[NK_COLOR_CHART_COLOR_HIGHLIGHT] = purple;
        table[NK_COLOR_SCROLLBAR] = background;
        table[NK_COLOR_SCROLLBAR_CURSOR] = currentline;
        table[NK_COLOR_SCROLLBAR_CURSOR_HOVER] = comment;
        table[NK_COLOR_SCROLLBAR_CURSOR_ACTIVE] = purple;
        table[NK_COLOR_TAB_HEADER] = currentline;
        nk_style_from_table(ctx, table);
    } else {
        nk_style_default(ctx);
    }
}
// inicio de gui

// ventana de texto e titulo
void Text_title(struct nk_context *ctx){

    if (nk_begin(ctx, "Text Editor", nk_rect(0, 35, WINDOW_WIDTH-(WINDOW_WIDTH*30/100), WINDOW_HEIGHT - 35), NK_WINDOW_BORDER
    | NK_WINDOW_MOVABLE | NK_WINDOW_TITLE | NK_WINDOW_SCALABLE | NK_WINDOW_MINIMIZABLE)){    
                
        nk_layout_row_dynamic(ctx, 25, 1);
        nk_edit_string(ctx, NK_EDIT_BOX, title, &title_len, sizeof(title), nk_filter_default);

        nk_layout_row_begin(ctx, NK_DYNAMIC,WINDOW_HEIGHT - 35, 1); // Begin a new row with dynamic width
        //nk_layout_row_dynamic(ctx, 400, 1); // Ensure enough space for the tex
                
        nk_layout_row_push(ctx, 0); // Push 70% of the space for the text editor
        nk_edit_string(ctx, NK_EDIT_BOX | NK_EDIT_MULTILINE | NK_EDIT_AUTO_SELECT, text, &text_len, sizeof(text), nk_filter_default);
        

        nk_layout_row_end(ctx);

    }
       
    nk_layout_row_static(ctx, 30, 80, 2);
    if (nk_button_label(ctx, "Button")){
        strcpy(title_save_buf, title);
        save_text(text, title_save_buf);
    }
 //   if (nk_window_is_hidden(ctx, "Text Editor")){ 
  //      break; se deberia implementar en while
    nk_end(ctx);
    
}
// Ventana de archivo
void Archive(struct nk_context *ctx){

     if (nk_begin(ctx, "Archive", nk_rect(WINDOW_WIDTH-(WINDOW_WIDTH*30/100), 35, WINDOW_WIDTH-(WINDOW_WIDTH*70/100), WINDOW_HEIGHT - 35),
        NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE | NK_WINDOW_MINIMIZABLE |NK_WINDOW_TITLE)){    
                
        nk_layout_row_begin(ctx, NK_DYNAMIC,WINDOW_HEIGHT - 50, 2); // ventana dianmica , el 2 representa los modos , puede ser 1 o 2 o3 o4
        nk_layout_row_push(ctx,0); // para ajustado de tamaño 

        if (nk_group_begin(ctx, "Archives", NK_WINDOW_BORDER)) {

            int i = 0;
            char buffer[64];

            nk_layout_row_dynamic(ctx, 0, 2);

            for (i = 0; i < 3; ++i) {

                sprintf(buffer, "%s", "asdasd.txt");

                if(nk_button_label(ctx, buffer)){

                    load_text(buffer);
                }

            }nk_group_end(ctx);

        }nk_layout_row_end(ctx);
   //     if (nk_window_is_hidden(ctx, "Text Editor")){ 
  //      break;  deberia implemntar en while
    } nk_end(ctx);

    
}

// barra de menu en la parte arriba
void layout(struct nk_context *ctx, int windowWidth) { 
      
    // Iniciar la ventana del menú en la parte superior de la ventana
    if (nk_begin(ctx, "Menu", nk_rect(0, 0, windowWidth, 35), 0)) {
        // Iniciar la barra de menú
        nk_menubar_begin(ctx);
        {
            // Definir una fila estática de 30 de altura, con elementos de 40 de ancho y 5 de espacio
            nk_layout_row_static(ctx, 25, 45, 5);

            // Comenzar el menú "File"
            if (nk_menu_begin_label(ctx, "File", NK_TEXT_LEFT, nk_vec2(200, 200))) {
                // Definir una fila dinámica de 25 de altura con 1 elemento por fila
                nk_layout_row_dynamic(ctx, 20, 1);

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
            if (nk_menu_begin_label(ctx, "Search  ", NK_TEXT_LEFT, nk_vec2(200, 200))) {
                // Definir una fila dinámica de 25 de altura con 1 elemento por fila
                nk_layout_row_dynamic(ctx, 20, 1);

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

              
            // Comenzar el menú "THEMEs"
            if (nk_menu_begin_label(ctx, "themes  ", NK_TEXT_LEFT, nk_vec2(200, 200))) {
                // Definir una fila dinámica de 25 de altura con 1 elemento por fila
                nk_layout_row_dynamic(ctx, 20, 1);

                // Definir elementos del menú
                if (nk_menu_item_label(ctx, " THEME_BLACK", NK_TEXT_LEFT)) {
                    
                     set_style(ctx, THEME_RED);
                }

                if (nk_menu_item_label(ctx, " THEME_WHITE", NK_TEXT_LEFT)) {
                    
                     set_style(ctx, THEME_WHITE);
                }

                if (nk_menu_item_label(ctx, " THEME_RED", NK_TEXT_LEFT)) {
                    
                     set_style(ctx, THEME_RED);
                }
          
               if (nk_menu_item_label(ctx, " THEME_BLUE", NK_TEXT_LEFT)) {

                     set_style(ctx, THEME_BLUE);
                }

                 if (nk_menu_item_label(ctx, " THEME_DARK ", NK_TEXT_LEFT)) {

                     set_style(ctx, THEME_DARK);
                }

                 if (nk_menu_item_label(ctx, " THEME_DRACULA", NK_TEXT_LEFT)) {

                     set_style(ctx, THEME_DRACULA);
                }

                // Terminar el menú "THEMES"
                nk_menu_end(ctx);
            }

            // Comenzar el menú "Herramientas"
            if (nk_menu_begin_label(ctx, "Herramientas  ", NK_TEXT_LEFT, nk_vec2(200, 200))) {
                // Definir una fila dinámica de 25 de altura con 1 elemento por fila
                nk_layout_row_dynamic(ctx, 20, 1);

                // Definir elementos del menú
                if (nk_menu_item_label(ctx, "Text Editor", NK_TEXT_LEFT)) {

                    Text_title(ctx); // llama funcion de espacio de escritura de texto y titulo

                }
                if (nk_menu_item_label(ctx, "Archive windows", NK_TEXT_LEFT)) {

                    Archive(ctx); // llama a la funcion de ventana visualizador de archivos txt
                    
                }
                // Terminar el menú "Herramientas"
                nk_menu_end(ctx);
            }
        }
        // Terminar la barra de menú
        nk_menubar_end(ctx);
    }
    // Terminar la ventana del menú
    nk_end(ctx);
}

// gui main me encantaria llamarle renegator3000
void GUI(struct nk_context *ctx){

    layout(ctx,WINDOW_WIDTH); // Llamada a la función de diseño del menú
    Text_title(ctx); // llama funcion de espacio de escritura de texto y titulo
    Archive(ctx); // llama a la funcion de ventana visualizador de archivos txt

}

// fin de gui

int main(void){
    long dt;
    long started;
    int running = 1;
    XWindow xw;
    struct nk_context *ctx;
    char text[1024 * 16] = {0}; /* Buffer for text input */
    char title[100] = {0}; // Titulo
    int text_len = 0;
    int title_len = 0;
    char title_save_buf[100];

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
    char title_placeholder[]={"Titulo:"};
    strcpy(title, title_placeholder);
    title_len=sizeof(title_placeholder);

    while (running)
    {
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
                   
                } else if ((evt.xkey.state & ControlMask) && keysym == XK_v) {
                   
                    }
                }
            }
            nk_xlib_handle_event(xw.dpy, xw.screen, xw.win, &evt);
        
        nk_input_end(ctx);

        /* GUI */
        GUI(ctx);

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
