#pragma once

/*=================================
 * Simples MediaPlayer
 * Autor: Diogo Roessler - 2020
 =================================*/

#include <gtk/gtk.h>

#define __aligned(X)            __attribute__((__aligned__(X)))

struct open_dialog_t {
    GtkDialog *dialog;
    const char *filename;
}*open_dialog __aligned(16);

struct dialog_message_t {
    GtkMessageDialog *mbox;
    unsigned short type;
    const char* text;
    unsigned short button;
}*message __aligned(16);

enum types_responses_ok_t {
    CANCEL =0,
    OK =1,
    RETRY =2
};

const char* g_get_filename(struct open_dialog_t *open_dialog);
void g_return_messages_box(struct dialog_message_t *message);
void g_get_init_dialog(void);
