#include "BufferCanvas.h"

BufferCanvas::BufferCanvas()
{
    //ctor
}

BufferCanvas::~BufferCanvas()
{
    //dtor
}

void BufferCanvas::init(int screen_width, int screen_height)
{
    _std = GetStdHandle(STD_OUTPUT_HANDLE);
    _buffer = CreateConsoleScreenBuffer(
                GENERIC_READ|
                GENERIC_WRITE,
                FILE_SHARE_READ|
                FILE_SHARE_WRITE,
                NULL,
                CONSOLE_TEXTMODE_BUFFER,
                NULL);
    SetConsoleActiveScreenBuffer(_buffer);

    // 设置控制台窗口大小，疑似没用
    COORD screen_size;
    screen_size.X = screen_width;
    screen_size.Y = screen_height;
    SetConsoleScreenBufferSize(_std, screen_size);
    SetConsoleScreenBufferSize(_buffer, screen_size);
    return;
}

void BufferCanvas::set_info(int ord)
{
    // 设置光标为不可见
    CONSOLE_CURSOR_INFO cc_info;
    cc_info.dwSize = 1;
    cc_info.bVisible = (ord == 0)? 0: 1;
    SetConsoleCursorInfo(_buffer, &cc_info);
    SetConsoleCursorInfo(_std, &cc_info);
}

void BufferCanvas::move_info(short int x, short int y)
{
    COORD coordinate = {x, y};
    SetConsoleCursorPosition(_std, coordinate);
}

void BufferCanvas::screen_show(Mono* screen, Mono* buffer)
{
    screen->_show();

    COORD coord = {0, 0};
    DWORD bytes = 0;
    SetConsoleCursorPosition(_std, coord);
    for (int i = 0; i < screen->height; i++)
    {
        ReadConsoleOutputCharacterA(_std, buffer->surface[i], buffer->width, coord, &bytes);
        coord.Y++;
    }

    COORD output_coord = {0, 0};
    DWORD output_bytes = 0;
    SetConsoleCursorPosition(_buffer, output_coord);
    for (int i = 0; i < screen->height; i++)
    {
        WriteConsoleOutputCharacterA(_buffer, buffer->surface[i], buffer->width, output_coord, &output_bytes);
        output_coord.Y++;
    }

    return;
}
