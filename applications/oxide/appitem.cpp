#include <QDebug>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>

#include "appitem.h"
#include "mxcfb.h"

#define DISPLAYWIDTH 1404
#define DISPLAYHEIGHT 1872
#define TEMP_USE_REMARKABLE_DRAW 0x0018

const size_t screen_size = DISPLAYWIDTH * DISPLAYHEIGHT * sizeof(uint16_t);
static char* privateBuffer = new char[screen_size];


void redraw_screen(int fd){
    mxcfb_update_data update_data;
    mxcfb_rect update_rect;
    update_rect.top = 0;
    update_rect.left = 0;
    update_rect.width = DISPLAYWIDTH;
    update_rect.height = DISPLAYHEIGHT;
    update_data.update_marker = 0;
    update_data.update_region = update_rect;
    update_data.waveform_mode = WAVEFORM_MODE_AUTO;
    update_data.update_mode = UPDATE_MODE_FULL;
    update_data.dither_mode = EPDC_FLAG_USE_DITHERING_MAX;
    update_data.temp = TEMP_USE_REMARKABLE_DRAW;
    update_data.flags = 0;
    ioctl(fd, MXCFB_SEND_UPDATE, &update_data);
}


bool AppItem::ok(){
    return !_name.isEmpty() && !_call.isEmpty();
}

void AppItem::execute(){
    qDebug() << "Setting termfile /tmp/.terminate";
    std::ofstream termfile;
    termfile.open("/tmp/.terminate");
    termfile << _term.toStdString() << std::endl;
    qDebug() << "Copying screen...";
    int frameBufferHandle = open("/dev/fb0", O_RDWR);
    char* frameBuffer = (char*)mmap(0, screen_size, PROT_READ | PROT_WRITE, MAP_SHARED, frameBufferHandle, 0);
    memcpy(privateBuffer, frameBuffer, screen_size);
    qDebug() << "Running application...";
    system(_call.toUtf8());
    qDebug() << "Flushing touch buffer...";
    inputManager->clear_touch_buffer();
    qDebug() << "Recalling screen...";
    memcpy(frameBuffer, privateBuffer, screen_size);
    redraw_screen(frameBufferHandle);
    close(frameBufferHandle);
}
