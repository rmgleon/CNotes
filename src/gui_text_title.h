int WINDOW_HEIGHT_OFFSET = 35;
int edit_mode = 1;

// ventana de texto e titulo

void text_title(struct nk_context *ctx){

    if (nk_begin(ctx, "Editor de texto:", nk_rect(WINDOW_WIDTH-(WINDOW_WIDTH*80/100), WINDOW_HEIGHT_OFFSET, WINDOW_WIDTH-(WINDOW_WIDTH*20/100), WINDOW_HEIGHT - WINDOW_HEIGHT_OFFSET), NK_WINDOW_BORDER
    | NK_WINDOW_MOVABLE | NK_WINDOW_TITLE | NK_WINDOW_SCALABLE | NK_WINDOW_MINIMIZABLE)){    
                
        nk_layout_row_dynamic(ctx, 25, 1);
        
        if (edit_mode) {
            nk_edit_string(ctx, NK_EDIT_FIELD, title, &title_len, sizeof(title), nk_filter_default);
        } else {
            nk_label_wrap(ctx, title);
        }

        nk_layout_row_begin(ctx, NK_DYNAMIC, WINDOW_HEIGHT - WINDOW_HEIGHT_OFFSET - 115, 1);  
        nk_layout_row_push(ctx, 0);

        if (edit_mode) {
            nk_edit_string(ctx, NK_EDIT_BOX | NK_EDIT_MULTILINE, text, &text_len, sizeof(text), nk_filter_default);
        } else {
            nk_label_wrap(ctx, text);
        }

        nk_layout_row_end(ctx);
    }
       
    nk_layout_row_static(ctx, 30, 80, 5);
    if (nk_button_label(ctx, "Guardar")) {
        //Comprueba que haya titulo o texto
        //No funciona bien lo que está en función compressBuffer()
        if (!text_len > 0 || !title_len > 0) {
            printf("Error al guardar archivo. Título o texto vacío.\n");
        } else {
            strcpy(title_save_buf, title);
            compressBuffer(text, title_save_buf);
            add_hash_node(title);
            save_list_nodes(&list);
        }
    }
    if (nk_button_label(ctx, "Alternar")) {
        edit_mode = !edit_mode;
    }
    nk_end(ctx);
}
