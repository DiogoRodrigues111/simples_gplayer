/*=================================
 * Simples MediaPlayer
 * Autor: Diogo Roessler - 2020
 =================================*/
#include <stdio.h>

#include <gst/gst.h>
#include <gst/gstdevice.h>
#include <gst/video/video.h>
#include <gst/video/videooverlay.h>

#include <gtk/gtk.h>

#ifdef _WIN32
    #warning "USUARIO OPERANDO EM WINDOWS"
#endif

#define __aligned(X)            __attribute__((__aligned__(X)))
#define SUCCESS                 0  
#define false                   0
#define true                    1
#define FAIL                    ((void*)-1)

//============
// Struct
//============
enum _boolean {
    False=0,
    True=1
};
typedef enum _boolean bool_t;

enum _data_play_t {
    G_PLAY_FLAG_VIDEO = (1 << 0),    //Video output 
    G_PLAY_FLAG_AUDIO = (1 << 1),    //Audio output
    G_PLAY_FLAG_TEXT  = (1 << 2)     //Subtitle output
};
typedef enum _data_play_t *data_play_t;

enum types_responses_ok_t {
    NONE         = -1,
    REJECT       = -2,
    ACCEPT       = -3,
    DELETE_EVENT = -4,
    OK           = -5,
    CANCEL       = -6,
    CLOSE        = -7,
    YES          = -8,
    NO           = -9,
    APPLY        = -10,
    HELP         = -11
};

struct g_window_t {
    GtkWidget *gwin;
}*pwindow;

struct stream_t {
    GstElement *elem;
    GstDevice *device;
    GstEvent *gevent;
    GstContext *gcontext;
    GstStream *gstream;
    GstObject *gobj;
    GMainLoop *gMainLoop;
    GstStateChangeReturn ret;
    GIOChannel *ch;
    GstBus *bus;

    unsigned int g_video;
    unsigned int g_audio;
    unsigned int g_text;

    unsigned int g_cur_video;
    unsigned int g_cur_audio;
    unsigned int g_cur_text;
}*stream __aligned(16);

struct file_t {
    FILE* fp;
    const char* filename;
}file __aligned(16);

struct object_t {
    #define __null              0
};

struct open_dialog_t {
    GtkWidget *file_chooser;
    const char *filename;
}*open_dialog __aligned(16);

struct dialog_message_t {
    GtkMessageDialog *mbox;
    int type;
    const char* text;
    unsigned short button;
}*message __aligned(16);

//============
// globals rotinas
//============

void g_init_gtk(
    struct g_window_t *pwindow,
    const char *title,
    unsigned int sizex, unsigned int sizey);

void g_check_gtk(void);
void g_exit_gtk(void);
void g_main_gtk(void);
bool_t g_signal_gtk(struct g_window_t *pwindow);

FILE* g_read_stream(struct open_dialog_t *open_dialog, struct file_t *file);
FILE* g_open_stream(struct object_t *obj, struct stream_t *stream);
void g_check_listen(void);
int g_listen(struct stream_t *stream);
gpointer g_init_gstreamer(struct stream_t *stream);

const char* g_get_filename(struct open_dialog_t *open_dialog);
void g_return_messages_box(struct dialog_message_t *message);
GtkDialog* g_get_init_dialog(void);
int g_make_run_dialog(void);
void g_message_box(const char* format, ...);

// Definição de teclado
static bool_t handle_message(GstBus *bus, GstMessage *msg, struct stream_t *stream);
static bool_t handle_keyboard(GIOChannel *source, GIOCondition cond, struct stream_t *stream);

//============
// Rotinas
//============

// Definição de teclado
static 
bool_t handle_message(
    GstBus *bus, GstMessage *msg, struct stream_t *stream)
{
    return false;
}

static
bool_t handle_keyboard(
    GIOChannel *source, GIOCondition cond, struct stream_t *stream)
{
    return false;
}

// Inicia as caixas de dialog
GtkDialog * 
g_get_init_dialog(void)
{
    struct open_dialog_t *chooser;
    GtkDialog *d = NULL;
    
    chooser =
    (struct open_dialog_t *)malloc(sizeof(struct open_dialog_t*));
    
    chooser->file_chooser = 
    gtk_file_chooser_dialog_new(
        "Select Media", NULL /* GTK_WINDOW(chooser->file_chooser) */,
        GTK_FILE_CHOOSER_ACTION_OPEN, "OK", OK, "CANCEL", CANCEL, NULL);

    free(chooser);
    
    return d;
}

// Roda o dialog
int 
g_make_run_dialog(void)
{
    int i;
    struct open_dialog_t *op;
    
    op = (struct open_dialog_t *)malloc(sizeof(struct open_dialog_t*));
    
    i = gtk_dialog_run(g_get_init_dialog());
    
    switch(i) {
        case OK:
            g_get_filename(op);
            return 1;
        break;
        
        case CANCEL:
            return 0;
        break;
    }
    
    free(op);
    
    return SUCCESS;
}

// Obtém o nome da caixa de dialogo
const char* 
g_get_filename(struct open_dialog_t *open_dialog)
{
    open_dialog->filename = gtk_file_chooser_get_filename(
        GTK_FILE_CHOOSER(open_dialog->file_chooser));
    
    return open_dialog->filename;
}

void 
g_return_messages_box(struct dialog_message_t *message)
{
    switch(message->type) {
        FOUND:
        case OK:
        break;
        ;

        case CANCEL:
        break;

        case ACCEPT:
            goto FOUND;
        break;
    }
}

// Obtém a leitura do arquivo
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

// Message do sistema
void 
g_message_box(const char* format, ...)
{
    g_printerr("%s", format);
}

// Inicia o gstreamer
gpointer
g_init_gstreamer(
    struct stream_t *stream)
{
    stream->elem = gst_element_factory_make("playbin", "playbin");

    if(!stream->elem) {
        g_message_box("%s", "Errors in: %i, %s, %s",
         __LINE__, __FUNCTION__, __FILE__);

        return FAIL;
    }
    return NULL;
}

// Abre a gstreamer com o arquivo
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

// Obtém uma checagem de arquivo caso não tenha sucesso,
// em abrir o arquivo ou leitura corretamente.
void 
g_check_listen(void)
{
    struct stream_t *stream;
    struct open_dialog_t *getfilename;
    int flags;
    
    stream = (struct stream_t *)malloc(sizeof(struct stream_t *));
    getfilename = (struct open_dialog_t *)malloc(sizeof(struct open_dialog_t *));

    g_object_set (
        g_init_gstreamer(stream),
         "uri", g_get_filename(getfilename));
    
    g_object_get (stream->elem, "flags", &flags, NULL);
    
    flags |= G_PLAY_FLAG_VIDEO | G_PLAY_FLAG_AUDIO;
    flags &= ~G_PLAY_FLAG_TEXT;
    
    g_object_set (stream->elem, "flags", flags, NULL);
    g_object_set (stream->elem, "connection-speed", 56, NULL);
    
    stream->bus = gst_element_get_bus (stream->elem);
    gst_bus_add_watch (stream->bus, (GstBusFunc)handle_message, &stream);
    
    free(stream);
    free(getfilename);
}

// Manda a informação pro gstreamer
int
g_listen(struct stream_t *stream)
{
    // Playback
    stream->ret = gst_element_set_state (stream->elem, GST_STATE_PLAYING);
    if (stream->ret == GST_STATE_CHANGE_FAILURE) {
        g_printerr ("Unable to set the pipeline to the playing state.\n");
        gst_object_unref (stream->elem);
        return -1;
    }
    
    return SUCCESS;
}

// Signals back
bool_t 
g_signal_gtk(struct g_window_t *pwindow)
{
    return false;
}

// Inicia o gtk
void 
g_init_gtk(
    struct g_window_t *pwindow,
    const char *title,
    unsigned int sizex, unsigned int sizey)
{
    //pwindow->gwin = gtk_window_new(GTK_WINDOW_TOPLEVEL);
}

// obtém uma checagem para o gtk caso não seja bem sucedido.
void 
g_check_gtk(void)
{
    
}

// exit rotina, gtk.
void 
g_exit_gtk(void)
{
    
}

// Obtém os resultado,
// do check_gtk, init_gtk, e exit entre outras rotinas adicionais.
void 
g_main_gtk(void)
{
    
}

int 
main (int argc, char* argv[]) 
{    
    struct stream_t *stream;
    stream = (struct stream_t *)malloc(sizeof(struct stream_t*));
    
    //gtk_init(&argc, &argv);
    gst_init(&argc, &argv);

    //Instancia file_chooser
    //g_make_run_dialog();
    
    //Obtém values
    
    /* cria um glib p/ exec isto */
    stream->gMainLoop = g_main_loop_new (NULL, FALSE);
    g_main_loop_run (stream->gMainLoop);

    /* Free */
    g_main_loop_unref (stream->gMainLoop);
    g_io_channel_unref (stream->ch);
    gst_object_unref (stream->bus);
    gst_element_set_state (stream->elem, GST_STATE_NULL);
    gst_object_unref (stream->elem);
    free(stream);
    
    //gtk_main();
    return SUCCESS;
}
