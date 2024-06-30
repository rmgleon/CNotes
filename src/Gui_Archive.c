// Ventana de archivo
void Archive(struct nk_context *ctx){

     if (nk_begin(ctx, "Archive", nk_rect(1, WINDOW_HEIGHT_OFFSET, WINDOW_WIDTH-(WINDOW_WIDTH*80/100), WINDOW_HEIGHT - WINDOW_HEIGHT_OFFSET),
        NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE | NK_WINDOW_MINIMIZABLE |NK_WINDOW_TITLE)){    
                
        nk_layout_row_begin(ctx, NK_DYNAMIC,WINDOW_HEIGHT - WINDOW_HEIGHT_OFFSET, 2); // ventana dianmica , el 2 representa los modos , puede ser 1 o 2 o3 o4
        nk_layout_row_push(ctx,0); // para ajustado de tamaño 

        if (nk_group_begin(ctx, "Archives", NK_WINDOW_BORDER)) {

            // Cuadro de texto para búsqueda
            static char search_buffer[256] = "";
            nk_layout_row_dynamic(ctx, 25, 1);
            nk_label(ctx, "Buscar:", NK_TEXT_LEFT);
            nk_edit_string_zero_terminated(ctx, NK_EDIT_FIELD, search_buffer, sizeof(search_buffer), nk_filter_default);

            // Recorrer la lista y mostrar los elementos filtrados
            list_node *aux = list;

            while(aux != NULL) {

                // Verificar si el título actual contiene el texto de búsqueda
                if (strstr(aux->titulo, search_buffer) != NULL) {

                    if(nk_button_label(ctx, aux->titulo)){ // Al hacer click llama a la funcion para abrir el texto
                        strcpy(title, aux->titulo);
                        title_len = strlen(title);

                        if(decompressBuffer(text, aux->titulo) == 0){
                            delete_hash_node(aux->titulo, hash_table, &list);
                        }

                        text_len = strlen(text);
                    }       
                }
                        aux = aux->sig;
            }
            nk_group_end(ctx);
        }
        nk_layout_row_end(ctx);

    } nk_end(ctx);

}
