#ifndef BUFFERCANVAS_H
#define BUFFERCANVAS_H
#include <windows.h>
#include "Mono.h"

class BufferCanvas
{
    public:
        HANDLE _std;
        HANDLE _buffer;

        BufferCanvas();
        virtual ~BufferCanvas();

        void init(int screen_width, int screen_height);
        void set_info(int ord=0);
        void move_info(short int x, short int y);
        // 双缓冲屏幕显示
        void screen_show(Mono* screen, Mono* buffer);
};

#endif // BUFFERHUD_H
