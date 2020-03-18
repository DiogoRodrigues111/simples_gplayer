/*=================================
 * Simples MediaPlayer
 * Autor: Diogo Roessler - 2020
 ================================= */
#include "../inc/media.h"

FILE *
g_read_stream(
    struct open_dialog_t *open_dialog,
    struct file_t *file)
{   
    open_dialog = (struct open_dialog_t *)malloc(sizeof(struct open_dialog_t *));
    file->fp = fopen(open_dialog->filename, "rb+");
    
    if(feof(file->fp)) {
        printf("errors in feof file, please contact developers %i, %s, %s",
               __LINE__, __FUNCTION__,  __FILE__ );
    }
    
    while(!feof(file->fp)) {
        continue;
    }
    
    fclose(file->fp);
    free(open_dialog);
    
    return file->fp;
}

void 
g_init_gstreamer(
    struct stream_t *stream)
{
    
}

FILE *
g_open_stream(
    struct object_t *obj,
    struct stream_t *stream)
{
    struct open_dialog_t *open_dialog;
    struct file_t *file;
    
    open_dialog = (struct open_dialog_t *)malloc(sizeof(struct open_dialog_t *));
    file = (struct file_t *)malloc(sizeof(struct file_t *));
    
    file->fp = g_read_stream(open_dialog, file);
    
    if(feof(file->fp)) {
        printf("errors in feof file, please contact developers %i, %s, %s",
               __LINE__, __FUNCTION__,  __FILE__ );
    }
    
    if(!feof(file->fp)) {
        //init gstreamer
        g_init_gstreamer(stream);
    }
    
    
    fclose(file->fp);
    free(open_dialog);
    
    return file->fp;
}

void 
g_check_listen(void)
{
    
}

void 
g_listen(struct stream_t *stream)
{
    
}
