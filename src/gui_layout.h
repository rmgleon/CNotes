// barra de menu en la parte arriba
void layout(struct nk_context *ctx) { 
      
    // iniciar la ventana del menú en la parte superior de la ventana
    if (nk_begin(ctx, "Menu", nk_rect(0, 0,7680, 35), 0)) {  //7680 = 8k
        // Iniciar la barra de menú
        nk_menubar_begin(ctx);
        {
            // Definir una fila estática de 30 de altura, con elementos de 40 de ancho y 5 de espacio
            nk_layout_row_static(ctx, 25, 45, 5);

            // Comenzar el menú "File"
            if (nk_menu_begin_label(ctx, "Archivo", NK_TEXT_LEFT, nk_vec2(200, 200))) {
                // Definir una fila dinámica de 25 de altura con 1 elemento por fila
                nk_layout_row_dynamic(ctx, 20, 1);

                if (nk_menu_item_label(ctx, "Guardar", NK_TEXT_LEFT)) {       
                    strcpy(title_save_buf, title);
                    compressBuffer(text, title_save_buf);
                    add_hash_node(title);
                    save_list_nodes(&list);
                }
                if (nk_menu_item_label(ctx, "Salir", NK_TEXT_LEFT)) {
                    // Acción para "Exit"
                    exit(0);
                }
                // Terminar el menú "File"
                nk_menu_end(ctx);
            }
              
            // Comenzar el menú "THEMEs"
            if (nk_menu_begin_label(ctx, "Temas  ", NK_TEXT_LEFT, nk_vec2(200, 200))) {
                // Definir una fila dinámica de 25 de altura con 1 elemento por fila
                nk_layout_row_dynamic(ctx, 20, 1);

                // Definir elementos del menú
                if (nk_menu_item_label(ctx, " Negro", NK_TEXT_LEFT)) {
                    
                     set_style(ctx, THEME_BLACK);
                }

                if (nk_menu_item_label(ctx, " Blanco", NK_TEXT_LEFT)) {
                    
                     set_style(ctx, THEME_WHITE);
                }

                if (nk_menu_item_label(ctx, " Rojo", NK_TEXT_LEFT)) {
                    
                     set_style(ctx, THEME_RED);
                }
          
               if (nk_menu_item_label(ctx, " Azul", NK_TEXT_LEFT)) {

                     set_style(ctx, THEME_BLUE);
                }

                 if (nk_menu_item_label(ctx, " Oscuro", NK_TEXT_LEFT)) {

                     set_style(ctx, THEME_DARK);
                }

                 if (nk_menu_item_label(ctx, " Dracula", NK_TEXT_LEFT)) {

                     set_style(ctx, THEME_DRACULA);
                }

                // Terminar el menú "THEMES"
                nk_menu_end(ctx);
            }
        }
        // Terminar la barra de menú
        nk_menubar_end(ctx);
    }
    // Terminar la ventana del menú
    nk_end(ctx);
}
