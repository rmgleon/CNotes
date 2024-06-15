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
