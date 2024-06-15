int WINDOW_HEIGHT_OFFSET = 35;

// ventana de texto e titulo

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