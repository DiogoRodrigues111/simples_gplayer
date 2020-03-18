#pragma once

/*=================================
 * Simples MediaPlayer
 * Autor: Diogo Roessler - 2020
 ================================= */

#include "../inc/media.h"
#include "../inc/opened.h"

struct g_window_t {
    GtkWidget *gwin;
}*pwindow __aligned(16);

void g_init_gtk(
    struct g_window_t *pwindow,
    const char *title,
    unsigned int sizex, unsigned int sizey);

void g_check_gtk(void);
void g_exit_gtk(void);
void g_main_gtk(void);
