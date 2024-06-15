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
