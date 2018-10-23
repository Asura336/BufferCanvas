#include "Mono.h"


Mono::Mono(int width_input, int height_input): Rect(width_input, height_input)
{
    surface = new char*[height_input];
    for (int i = 0; i < height_input; i++)
    {
        surface[i] = new char[width_input];
    }
    mono_type = 0;
    active = true;
}

Mono::~Mono()
{
    for (int i = 0; i < height; i++)
    {
        delete[] surface[i];
        surface[i] = 0;
    }
    delete[] surface;
    surface = 0;
}

bool Mono::point_in_mono(int pos_x, int pos_y)
{
    return (pos_x >= this->position_x && pos_x < this->position_x + this->height
            && pos_y >= this->position_y && pos_y < this->position_y + this->width);
}

void Mono::get_icon(const char* file_name)
{
    FILE *file_in = fopen(file_name, "r");
    if (file_in == NULL)
    {
        fprintf(stderr, "The file \"%s\" has lost.\n", file_name);
        exit(-1);
    }
    char c = '\0';
    int i = 0, j = 0;
    while (c != EOF && i != height)
    {
        c = fgetc(file_in);
        if (c != '\n')
        {
            if (j < width)
            {
                if (point_in_mono(i, j))
                {
                    surface[i][j] = c;
                }
            }
            j++;
        }
        else if (c == '\n')
        {
            i++;
            j = 0;
        }
    }
    fclose(file_in);
    return;
}

void Mono::get_icon(const char* file_name, int pos_i_input, int pos_j_input, int icon_width)
{
    FILE* picon = fopen(file_name, "r");
    if (picon == NULL)
    {
        fprintf(stderr, "The file \"%s\" has lost.\n", file_name);
        exit(-1);
    }
    int i = 0, j = 0;
    char c = '\0';
    while (c != EOF && i < icon_width)
    {
        c = fgetc(picon);
        if (c != '\n' && j < icon_width)
        {
            if (point_in_mono(pos_i_input + i, pos_j_input + j))
            {
                surface[pos_i_input + i][pos_j_input + j] = c;
            }
            j++;
        }
        else if (c == '\n')
        {
            i++;
            j = 0;
        }
    }
    fclose(picon);
    return;
}

void Mono::get_icon(const char* file_name, Rect* rect)
{
    FILE* picon = fopen(file_name, "r");
    if (picon == NULL)
    {
        fprintf(stderr, "The file \"%s\" has lost.\n", file_name);
        exit(-1);
    }
    int i = 0, j = 0;
    char c = '\0';
    while (c != EOF && i < rect->height)
    {
        c = fgetc(picon);
        if (c != '\n' && j < rect->width)
        {
            if (point_in_mono(rect->position_x + i, rect->position_y + j))
            {
                surface[rect->position_x + i][rect->position_y + j] = c;
            }
            j++;
        }
        else if (c == '\n')
        {
            i++;
            j = 0;
        }
    }
    fclose(picon);
    return;
}

void Mono::get_icon(const char* file_name, Rect* rect, bool transparence)
{
    FILE* picon = fopen(file_name, "r");
    if (picon == NULL)
    {
        fprintf(stderr, "The file \"%s\" has lost.\n", file_name);
        exit(-1);
    }
    int i = 0, j = 0;
    char c = '\0';
    while (c != EOF && i < rect->height)
    {
        c = fgetc(picon);
        if (c != '\n' && j < rect->width)
        {
            if (point_in_mono(rect->position_x + i, rect->position_y + j))
            {
                if (!transparence || c != ' ')
                    surface[rect->position_x + i][rect->position_y + j] = c;
            }
            j++;
        }
        else if (c == '\n')
        {
            i++;
            j = 0;
        }
    }
    fclose(picon);
    return;
}

void Mono::get_icon_roll(const char* file_name, const Rect* rect, int right_range, int down_range)
{
    FILE* picon = fopen(file_name, "r");
    if (picon == NULL)
    {
        fprintf(stderr, "The file \"%s\" has lost.\n", file_name);
        exit(-1);
    }
    Rect icon_rect = *rect;
    for (int i = 0; i < down_range; i++)
    {
        for (int j = 0; j < right_range; j++)
        {
            get_icon(file_name, &icon_rect);
            icon_rect.position_y += rect->width;
        }
        icon_rect.position_y = rect->position_y;
        icon_rect.position_x += rect->height;
    }

    fclose(picon);
    return;
}

void Mono::print_line(const char* ori_str, int x, int y)
{
    char c = '\0';
    int dx = 0, dy = 0;
    for (unsigned int i = 0; i < strlen(ori_str); i++)
    {
        c = ori_str[i];
        if (c == '\n')
        {
            dy = 0;
            dx++;
        }
        else if (point_in_mono(x+dx, y+dy))
        {
            this->surface[x+dx][y+dy++] = c;
        }
    }
    return;
}

void Mono::print_file(const char* file_name)
{
    FILE* fp = fopen(file_name, "a");

    for (int i = 0; i < this->height; i++)
    {
        fputs(this->surface[i], fp);
        fputs("\r\n", fp);
    }

    fclose(fp);
    return;
}

void Mono::_show()
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            putchar(surface[i][j]);
        }
        putchar('\n');
    }
}

void Mono::update(Mono* mono, bool transparence)
{
    if (!mono->active)
    {
        return;
    }
    int pos_x, pos_y;
    for (int i = 0; i < mono->height; i++)
    {
        for (int j = 0; j < mono->width; j++)
        {
            pos_x = mono->position_x + i;
            pos_y = mono->position_y + j;
            if (point_in_mono(pos_x, pos_y))
            {
                if (!transparence || mono->surface[i][j] != ' ')
                    this->surface[pos_x][pos_y] = mono->surface[i][j];
            }
        }
    }
}

void Mono::clean(char c)
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            this->surface[i][j] = c;
        }
    }
    return;
}

void Mono::clean(int up, int down, int left, int right)
{
    for (int i = up; i < down; i++)
    {
        for (int j = left; j < right; j++)
        {
            surface[i][j] = ' ';
        }
    }
    return;
}

void Mono::clean(Rect* rect, char c)
{
        for (int i = 0; i < rect->height; i++)
        {
            for (int j = 0; j < rect->width; j++)
            {
                surface[rect->position_x + i][rect->position_y + j] = c;
            }
        }
    return;
}

void Mono::move_to(int pos_x, int pos_y)
{
    position_x = pos_x;
    position_y = pos_y;
    return;
}
