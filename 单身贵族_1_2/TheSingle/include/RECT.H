#ifndef RECT_H
#define RECT_H


class Rect
{
    public:
        int position_x, position_y;
        int height, width;

        Rect(int width_input=0, int height_input=0);
        virtual ~Rect();

        virtual void move_to(int pos_x, int pos_y);
};

#endif // RECT_H
