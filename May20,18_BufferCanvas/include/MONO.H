#ifndef MONO_H
#define MONO_H

#include <cstdio>
#include <cstdlib>
#include <string.h>
#include <Rect.h>


class Mono : public Rect
{
    private:
        Mono(Mono &mono){}
        char** surface;
    public:
        friend class BufferCanvas;
        int mono_type;
        bool active;

        Mono(int width_input=40, int height_input=40);
        virtual ~Mono();

        bool point_in_mono(int pos_x, int pos_y);

        // 上传文本文件内容作为 Mono 对象的 surface 内容，无对齐检查
        void get_icon(const char* file_name);


        // 将文件图标上传至 Mono 对象的指定位置。传入文件名，左上角点(i 行 j 列)、图标宽
        void get_icon(const char* file_name, int pos_i_input, int pos_j_input, int icon_width);
        // 将文件图标上传至 Mono 对象的指定位置。传入文件名，图标显示区域 (按 Rect 对象)
        void get_icon(const char* file_name, Rect* rect);
        void get_icon(const char* file_name, Rect* rect, bool transparence);

        // 以平铺形式放置图标。传入图标文件、左上角单元矩形、向右及向下延伸次数
        void get_icon_roll(const char* file_name, const Rect* rect, int right_range=0, int down_range=0);

        // 在 Mono 对象的指定位置上放置文字
        void print_line(const char* ori_str, int x, int y);

        // 将显示内容输出到指定文件
        void print_file(const char* file_name);

        void _show();

        // 从 mono 上传至当前 Mono 对象，transparence 为真时忽略空格
        void update(Mono* mono, bool transparence=false);

        void clean(char c=' ');

        // 清空指定区域
        void clean(int up, int down, int left, int right);

        void clean(Rect* rect, char c=' ');

        // 设置左上角点位置
        void move_to(int pos_x, int pos_y);
};

#endif // MONO_H
