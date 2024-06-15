// inicio de gui

// ventana de texto e titulo
int WINDOW_HEIGHT_OFFSET = 35;


void Text_title(struct nk_context *ctx){

    if (nk_begin(ctx, "Text Editor", nk_rect(WINDOW_WIDTH-(WINDOW_WIDTH*80/100), WINDOW_HEIGHT_OFFSET, WINDOW_WIDTH-(WINDOW_WIDTH*40/100), WINDOW_HEIGHT - WINDOW_HEIGHT_OFFSET), NK_WINDOW_BORDER
    | NK_WINDOW_MOVABLE | NK_WINDOW_TITLE | NK_WINDOW_SCALABLE | NK_WINDOW_MINIMIZABLE)){    
                
        nk_layout_row_dynamic(ctx, 25, 1);
        nk_edit_string(ctx, NK_EDIT_BOX, title, &title_len, sizeof(title), nk_filter_default);

        nk_layout_row_begin(ctx, NK_DYNAMIC,WINDOW_HEIGHT - WINDOW_HEIGHT_OFFSET -115, 1);
                
        nk_layout_row_push(ctx, 0); // Push 70% of the space for the text editor
        nk_edit_string(ctx, NK_EDIT_BOX | NK_EDIT_MULTILINE | NK_EDIT_AUTO_SELECT, text, &text_len, sizeof(text), nk_filter_default);
        

        nk_layout_row_end(ctx);

    }
       
    nk_layout_row_static(ctx, 30, 80, 2);
    if (nk_button_label(ctx, "Button")){
        strcpy(title_save_buf, title);
        save_text(text, title_save_buf);
    }
    nk_end(ctx);
    
}
// Ventana de archivo
void Archive(struct nk_context *ctx){

     if (nk_begin(ctx, "Archive", nk_rect(1, WINDOW_HEIGHT_OFFSET, WINDOW_WIDTH-(WINDOW_WIDTH*80/100), WINDOW_HEIGHT - WINDOW_HEIGHT_OFFSET),
        NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE | NK_WINDOW_MINIMIZABLE |NK_WINDOW_TITLE)){    
                
        nk_layout_row_begin(ctx, NK_DYNAMIC,WINDOW_HEIGHT - WINDOW_HEIGHT_OFFSET, 2); // ventana dianmica , el 2 representa los modos , puede ser 1 o 2 o3 o4
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

    } nk_end(ctx);

    
}

// barra de menu en la parte arriba
void layout(struct nk_context *ctx) { 
      
    // Iniciar la ventana del menú en la parte superior de la ventana
    if (nk_begin(ctx, "Menu", nk_rect(0, 0,7680, 35), 0)) {  //7680 = 8k
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
            if (nk_menu_begin_label(ctx, "Themes  ", NK_TEXT_LEFT, nk_vec2(200, 200))) {
                // Definir una fila dinámica de 25 de altura con 1 elemento por fila
                nk_layout_row_dynamic(ctx, 20, 1);

                // Definir elementos del menú
                if (nk_menu_item_label(ctx, " THEME_BLACK", NK_TEXT_LEFT)) {
                    
                     set_style(ctx, THEME_BLACK);
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
            if (nk_menu_begin_label(ctx, "Tools  ", NK_TEXT_LEFT, nk_vec2(200, 200))) {
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

    layout(ctx); // Llamada a la función de diseño del menú
    Text_title(ctx); // llama funcion de espacio de escritura de texto y titulo
    Archive(ctx); // llama a la funcion de ventana visualizador de archivos txt

}

// fin de gui


