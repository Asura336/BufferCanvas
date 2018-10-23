#include "Rect.h"

Rect::Rect(int width_input, int height_input)
{
    position_x = 0;
    position_y = 0;
    height = height_input;
    width = width_input;
}

Rect::~Rect()
{
    //dtor
}

void Rect::move_to(int pos_x, int pos_y)
{
    position_x = pos_x;
    position_y = pos_y;
    return;
}
