#pragma once

/*=================================
 * Simples MediaPlayer
 * Autor: Diogo Roessler - 2020
 ================================= */
#include <gst/gst.h>
#include <gst/gstdevice.h>
#include <gst/video/video.h>
#include <gst/video/videooverlay.h>

#include <stdio.h>

#include "../inc/opened.h"

struct stream_t {
    GstElement *ele;
    GstDevice *device;
    GstEvent *gevent;
    GstContext *gcontext;
    GstStream *gstream;
    GstObject *gobj;
}*stream __aligned(16);

struct file_t {
    FILE* fp;
    const char* filename;
}file __aligned(16);

struct object_t {
    #define __null              0
};

FILE* g_read_stream(struct open_dialog_t *open_dialog, struct file_t *file);
FILE* g_open_stream(struct object_t *obj, struct stream_t *stream);
void g_check_listen(void);
void g_listen(struct stream_t *stream);
void g_init_gstreamer(struct stream_t *stream);
