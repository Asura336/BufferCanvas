#include <iostream>
#include <conio.h>
#include <ctime>
#include <memory>
#include "BufferCanvas.h"


const int space_type = 2;  // 四角有长宽为 2 的空白


/**游戏用到的图标文件**/
char* ICON_FILES[] = {(char*)"./icon/cell.txt", (char*)"./icon/shell_0.txt",
                     (char*)"./icon/shell_1.txt", (char*)"./icon/title.txt",
                     (char*)"./icon/white0.txt", (char*)"./icon/white1.txt",
                     (char*)"./icon/white2.txt", (char*)"./icon/white3.txt",
                     (char*)"./icon/white4.txt", (char*)"./icon/white5.txt",
                     (char*)"./icon/white6.txt", (char*)"./icon/white7.txt",
                     (char*)"./icon/white8.txt", (char*)"./icon/white9.txt"};
int ICON_FILES_LEN = 14;


enum GameMsg
{
    NORMAL,
    NEED_RESTART,
    HELP,
    SNAPSHOT_NORMAL,
    SNAPSHOT_HELP,
    QUIT
};


class Pad
{
    /* 用于描述棋盘
     * 表示为一系列 Mono 的左上角点位置
     */
private:
    int** body;
public:
    int width, height;
    int score;

    Pad(int width=40, int height=40)
    {
        this->width = width;
        this->height = height;
        this->score = 0;

        body = (int**)calloc(this->height, sizeof(int*));
        for (int i = 0; i < this->height; i++)
        {
            body[i] = (int*)calloc(this->width, sizeof(int));
        }
    }

    virtual ~Pad()
    {
        for (int i = 0; i < height; i++)
        {
            free(body[i]);
            body[i] = 0;
        }
        free(body);
        body = 0;
    }

    void print_to_mono(Mono* mono, Rect* rect_size)
    {
        /* 传入的 rect_size 保存落子的宽高信息
         * 函数将 Pad 数组的内容反映到 Mono 的图案上
         */

        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                rect_size->position_x = rect_size->height * i;
                rect_size->position_y = rect_size->width * j;

                switch (body[i][j])
                {
                    case 0: break;
                    case 1: mono->get_icon(ICON_FILES[4], rect_size, true); break;
                    case 2: mono->get_icon(ICON_FILES[5], rect_size, true); break;
                    case 3: mono->get_icon(ICON_FILES[6], rect_size, true); break;
                    case 4: mono->get_icon(ICON_FILES[7], rect_size, true); break;
                    case 5: mono->get_icon(ICON_FILES[8], rect_size, true); break;
                    case 6: mono->get_icon(ICON_FILES[9], rect_size, true); break;
                    case 7: mono->get_icon(ICON_FILES[10], rect_size, true); break;
                    case 8: mono->get_icon(ICON_FILES[11], rect_size, true); break;
                    case 9: mono->get_icon(ICON_FILES[12], rect_size, true); break;
                    case 10: mono->get_icon(ICON_FILES[13], rect_size, true); break;
                    default:mono->get_icon(ICON_FILES[14], rect_size, true); break;
                }
            }
        }
        return;
    }

    void set_point(int x, int y, int type)
    {
        /* 传入棋盘坐标点 (x, y) 和输入内容 type
         * 改变棋盘点记录的内容
         */
        this->body[x][y] = type;
        return;
    }

    int get_point(int x, int y)
    {
        return this->body[x][y];
    }

    void init()
    {
        score = 0;
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                if ((i >= space_type && i < height - space_type) ||
                    (j >= space_type && j < height - space_type))
                {
                    set_point(i, j, 1);
                }
            }
        }
        set_point(height/2, width/2, 0);
    }

    bool can_jump(int x, int y, int horizontal, int vertical)
    {
        /* The value of horizontal and vertical is -1, 0 or 1 respectively.
         */

        if ((horizontal != 0 && vertical != 0) ||
            (horizontal == 0 && vertical == 0))
            return false;

        if (x+2*vertical < 0 || x+2*vertical >= height ||
            y+2*horizontal < 0 || y+2*horizontal >= width)  // 防越界
            return false;

        //if not body[x + 2*vertical][y + 2*horizontal] in active_area:
        if ((x+2*vertical) < space_type && (y+2*horizontal) < space_type)  // 左上非活动区
            return false;
        if ((x+2*vertical) < space_type && (y+2*horizontal) >= width - space_type)  // 右上非活动区
            return false;
        if ((x+2*vertical) >= height - space_type && (y+2*horizontal) < space_type)  // 左下非活动区
            return false;
        if ((x+2*vertical) >= height - space_type && (y+2*horizontal) >= width - space_type)  // 右下非活动区
            return false;

        if (body[x][y] != 0 &&
            body[x + 1 * vertical][y + 1 * horizontal] != 0 &&
            body[x + 2 * vertical][y + 2 * horizontal] == 0)
        {
            return true;
        }
        else
            return false;
    }

    bool continuable()
    {
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                if (get_point(i, j) != 0)
                {
                    if (can_jump(i, j, -1, 0) ||
                        can_jump(i, j, 1, 0) ||
                        can_jump(i, j, 0, -1) ||
                        can_jump(i, j, 0, 1))
                        return true;
                }
            }
        }
        return false;
    }

    void jump(int x, int y, int horizontal, int vertical)
    {
        if (can_jump(x, y, horizontal, vertical))
        {
            score += get_point(x, y);

            set_point(x + 2 * vertical, y + 2 * horizontal, get_point(x, y)+1);
            set_point(x, y, 0);
            set_point(x + 1 * vertical, y + 1 * horizontal, 0);
        }
    }
};


class Controler
{
private:
    static void icon_pos_to(Mono* icon, Rect* point);
    static void icon_move(Pad* pad, Rect* point, int horizontal=0, int vertical=0, int step=1);
public:
    friend class Message;
    int messenger;
    Controler()
    {
        this->messenger = NORMAL;
    }
    void control(Pad* pad, Rect* rect, Mono* mono);
    void control_help();
    void single_init(Pad* pad, Mono* mono, Rect* mono_rect);
};

void Controler::icon_pos_to(Mono* icon, Rect* point)
{
    /* 换算 pad 上点的坐标至 icon 的位置
     */
    int pos_x = point->position_x * icon->height;
    int pos_y = point->position_y * icon->width;
    icon->move_to(pos_x, pos_y);
    return;
}

void Controler::icon_move(Pad* pad, Rect* point, int horizontal, int vertical, int step)
{
    /* 将 Pad 对象上的点关联到 Mono::move_to()
     * 接收水平和垂直移动信息，改变点在 pad 上的坐标
     */

    int space_width = space_type;
    if (horizontal < 0)
    {
        if (point->position_x < space_width || point->position_x >= pad->height - space_width)
            point->position_y = (point->position_y - step < space_width)? point->position_y: point->position_y - step;
        else
            point->position_y = (point->position_y - step < 0)? point->position_y: point->position_y - step;
    }
    if (horizontal > 0)
    {
        if (point->position_x < space_width || point->position_x >= pad->height - space_width)
            point->position_y = (point->position_y + step >= pad->width - space_width)? point->position_y: point->position_y + step;
        else
            point->position_y = (point->position_y + step >= pad->width)? point->position_y: point->position_y + step;
    }
    if (vertical < 0)
    {
        if (point->position_y < space_width || point->position_y >= pad->width - space_width)
            point->position_x = (point->position_x - step < space_width)? point->position_x: point->position_x - step;
        else
            point->position_x = (point->position_x - step < 0)? point->position_x: point->position_x - step;
    }
    if (vertical > 0)
    {
        if (point->position_y < space_width || point->position_y >= pad->width - space_width)
            point->position_x = (point->position_x + step >= pad->height - space_width)? point->position_x: point->position_x + step;
        else
            point->position_x = (point->position_x + step >= pad->height)? point->position_x: point->position_x + step;
    }
    return;
}

void Controler::control(Pad* pad, Rect* rect, Mono* mono)
{
    /* 按键映射
     * 传入棋盘，拾取器约束点和拾取器
     */

    int horizontal = 0, vertical = 0;
    int push_h = 0, push_v = 0;

    HANDLE input_handle = GetStdHandle(STD_INPUT_HANDLE);
    INPUT_RECORD key;
    DWORD key_reverse;
    ReadConsoleInputA(input_handle, &key, 1, &key_reverse);
    if (key.EventType == KEY_EVENT && key.Event.KeyEvent.bKeyDown)
    switch (key.Event.KeyEvent.wVirtualKeyCode)
    {
        case VK_UP: vertical = -1; break;
        case VK_DOWN: vertical = 1; break;
        case VK_LEFT: horizontal = -1; break;
        case VK_RIGHT: horizontal = 1; break;

        case 'W': push_v = -1; break;
        case 'S': push_v = 1; break;
        case 'A': push_h = -1; break;
        case 'D': push_h = 1; break;
        //case VK_SPACE: mono->mono_type = (mono->mono_type + 1) % 2; break;
        case 'C': this->messenger = NEED_RESTART; break;
        case 'Q': this->messenger = QUIT; break;
        case 'H': this->messenger = HELP; break;
        case 'R': this->messenger = SNAPSHOT_NORMAL; break;
        default: break;
    }
    //pad->single_change(rect->position_x, rect->position_y, push_h, push_v);
    pad->jump(rect->position_x, rect->position_y, push_h, push_v);

    icon_move(pad, rect, horizontal, vertical);
    icon_pos_to(mono, rect);

    return;
}

void Controler::control_help()
{
    HANDLE input_handle = GetStdHandle(STD_INPUT_HANDLE);
    INPUT_RECORD key;
    DWORD key_reverse;
    ReadConsoleInputA(input_handle, &key, 1, &key_reverse);
    if (key.EventType == KEY_EVENT && key.Event.KeyEvent.bKeyDown)
    switch (key.Event.KeyEvent.wVirtualKeyCode)
    {
        case 'H': this->messenger = NORMAL; break;
        case 'C': this->messenger = NEED_RESTART; break;
        case 'Q': this->messenger = QUIT; break;
        case 'R': this->messenger = SNAPSHOT_HELP; break;
        default: break;
    }
    return;
}

void Controler::single_init(Pad* pad, Mono* mono, Rect* mono_rect)
{
    pad->init();  // 棋盘初始化
    //mono->get_icon("./icon/shell_0.txt");  // 初始化拾取器
    mono_rect->move_to(pad->height/2, pad->width/2);
    icon_pos_to(mono, mono_rect);
    this->messenger = NORMAL;
    return;
}


class Message
{
private:
    static bool file_online(const char* filename)
    {
        bool reserve = true;
        FILE* fp = fopen(filename, "r");
        if (fp == NULL)
            reserve = false;
        else
            fclose(fp);
        return reserve;
    }
public:
    static void print_log(Mono* mono, Pad* pad);

    static void welcome_text(BufferCanvas* hud, Mono* screen, Mono* buffer);

    static void check_files(BufferCanvas* hud, Mono* screen, Mono* buffer);

    static void help_text(BufferCanvas* hud, Mono* screen, Mono* buffer, int up_tab, int left_tab);

    static void quit_text(BufferCanvas* hud, Mono* screen, Mono* buffer)
    {
        Rect welcome_text_rect = Rect(35, 11);
        welcome_text_rect.position_x = 3;
        screen->clean();
        screen->get_icon(ICON_FILES[3], &welcome_text_rect, true);
        screen->print_line("Quit...", screen->height/2, 4);
        hud->screen_show(screen, buffer);
        Sleep(750);
        return;
    }

    static std::string get_data_name()
    {
        std::string head = "singles";

        time_t lt = time(NULL);
        struct tm* ptm = localtime(&lt);
        char tmp[20] = {0};
        strftime(tmp, sizeof(tmp), "%Y%b%d%H%M%S", ptm);
        return head + tmp + ".txt";
    }
};

void Message::check_files(BufferCanvas* canvas, Mono* screen, Mono* buffer)
{
    int start_x = 3, start_y = 4;
    int line_step = 2;
    bool all_exist = true;

    screen->clean();

    screen->print_line("Check files...", 1, start_y);

    for (int i = 0; i < ICON_FILES_LEN; i++)
    {
        canvas->set_info(0);
        screen->print_line(ICON_FILES[i], start_x, start_y);
        if (!file_online(ICON_FILES[i]))
        {
            screen->print_line("<missing>",start_x, start_y + 2 + strlen(ICON_FILES[i]));
            all_exist = false;
        }
        else
            screen->print_line("<exist>",start_x, start_y + 2 + strlen(ICON_FILES[i]));

        canvas->screen_show(screen, buffer);
        Sleep(50);
        start_x += line_step;
    }

    if (all_exist)
    {
        screen->print_line("Complete.", start_x, start_y);
        canvas->screen_show(screen, buffer);
        Sleep(750);

        return;
    }
    else
    {
        screen->print_line("<!> Some thing is wrong,", start_x, start_y);
        screen->print_line("press Enter to quit.", start_x + 2, start_y + 4);
        canvas->screen_show(screen, buffer);
        getchar();
        exit(404);
    }
}

void Message::print_log(Mono* mono, Pad* pad)
{
    mono->print_line("Remove\n\nall of\n\nTWINS!", 1, 1);
    mono->print_line("Press \'H\'\n\nto view\n\nhelp text,\n\nif you\n\nneed it.", 1, 25);
    mono->print_line("Arrow keys\n\nfor move,\n\nW A S D\n\nfor jump.", 26, 1);
    mono->print_line("Score:", 26, 25);
    char score[5] = {0};
    sprintf(score, "%3d", pad->score);
    mono->print_line(score, 28, 27);
    mono->print_line("Able to\n\njump: ", 30, 25);

    if (pad->continuable())
        mono->print_line("True", 32, 31);
    else
        mono->print_line("False", 32, 30);
}

void Message::welcome_text(BufferCanvas* canvas, Mono* screen, Mono* buffer)
{
    Rect welcome_text_rect = Rect(35, 11);
    welcome_text_rect.position_x = 3;
    screen->clean();
    screen->get_icon(ICON_FILES[3], &welcome_text_rect, true);

    screen->print_line("Welcome, my friend.", 17, 10);
    screen->print_line("Press Enter to start.", 19, 9);

    canvas->set_info(0);
    canvas->screen_show(screen, buffer);

    while (getchar() != '\n') ;

    screen->print_line("Just wait a second...", 21, 7);
    canvas->set_info(0);
    canvas->screen_show(screen, buffer);
    Sleep(1000);

    screen->print_line("Down, let's go.", 23, 10);
    canvas->set_info(0);
    canvas->screen_show(screen, buffer);
    Sleep(500);

    return;
}

void Message::help_text(BufferCanvas* canvas, Mono* screen, Mono* buffer, int up_tab, int left_tab)
{
    screen->print_line("Game rule:", up_tab + 1, left_tab + 1);
    screen->print_line("Move cursor: Arrows", up_tab + 3, left_tab + 3);
    screen->print_line("Jump: [W, A, S, D]", up_tab + 5, left_tab + 3);
    screen->print_line("Help or back game: H", up_tab + 7, left_tab + 3);
    screen->print_line("Snapshot: R", up_tab + 9, left_tab + 3);
    screen->print_line("Restart: C", up_tab + 11, left_tab + 3);
    screen->print_line("Quit: Q. And you soft egg.", up_tab + 13, left_tab + 3);
    screen->print_line("Now go, too many twins", up_tab + 15, left_tab + 3);
    screen->print_line("here and you must let them", up_tab + 17, left_tab + 1);
    screen->print_line("jump each other, be single.", up_tab + 19, left_tab + 1);
    screen->print_line("One piece will be stronger", up_tab + 21, left_tab + 1);
    screen->print_line("after it jump over another.", up_tab + 23, left_tab + 1);
}


//#define DEBUG
int main(void)
{
    using std::unique_ptr;
    int pad_width = 7, pad_height = 7;  // 棋盘的宽和高
    int pad_type = 5;  // 棋盘拾取器的边长

    int screen_width = pad_width * pad_type, screen_height = pad_height * pad_type;
    unique_ptr<Mono> screen(new Mono(screen_width, screen_height));
    unique_ptr<Mono> screen_buffer(new Mono(screen_width, screen_height));

    unique_ptr<BufferCanvas> canvas(new BufferCanvas());
    canvas->init(screen_width, screen_height);

    unique_ptr<Pad> pad(new Pad(pad_width, pad_height));  // 棋盘

    unique_ptr<Mono> shell(new Mono(pad_type, pad_type));  // 拾取器
    shell->get_icon(ICON_FILES[2]);  // 初始化拾取器
    unique_ptr<Rect> shell_rect(new Rect());  // 约束拾取器的点，本身受 pad 宽高约束
    unique_ptr<Rect> icon_size(new Rect(pad_type, pad_type));  // 保存棋子和拾取器的尺寸

    unique_ptr<Controler> controler(new Controler);

    /**游戏准备完成**/
#ifndef DEBUG
    // ADD INIT TEXT
    Message::check_files(canvas.get(), screen.get(), screen_buffer.get());
    // ADD WELCOME TEXT
    Message::welcome_text(canvas.get(), screen.get(), screen_buffer.get());
#endif // DEBUG
    /**进入游戏实现**/
    controler->messenger = NEED_RESTART;
    Rect pad_cell = Rect(pad_type, pad_type);  // 背景图案相关
    while (true)
    {
        if (kbhit())
        {
            switch (controler->messenger)
            {
            case NORMAL:
                controler->control(pad.get(), shell_rect.get(), shell.get());
                break;
            case HELP:
                controler->control_help();
                break;
            default:
                return -1;
            }
        }

        int up_tab = 4, left_tab = 3;  // case HELP

        switch (controler->messenger)
        {
        case NEED_RESTART:  // Restart game
            controler->single_init(pad.get(), shell.get(), shell_rect.get());
            break;
        case QUIT:  // Quit game.
            Message::quit_text(canvas.get(), screen.get(), screen_buffer.get());
            return 0;
        case SNAPSHOT_NORMAL:  //Save snapshot in file, back to NORMAL
            // **保持游戏画面显示
            canvas->set_info(0);
            pad->print_to_mono(screen.get(), icon_size.get());
            Message::print_log(screen.get(), pad.get());
            canvas->screen_show(screen.get(), screen_buffer.get());
            // **保持游戏画面显示
            screen->print_file(Message::get_data_name().c_str());
            screen->clean();
            canvas->screen_show(screen.get(), screen_buffer.get());
            Sleep(330);
            controler->messenger = NORMAL;
            break;
        case SNAPSHOT_HELP:  //Save snapshot in file, back to HELP
            // **保持游戏画面显示
            canvas->set_info(0);
            pad->print_to_mono(screen.get(), icon_size.get());
            Message::print_log(screen.get(), pad.get());
            screen->clean(up_tab, pad_height*pad_type-up_tab, left_tab, pad_width*pad_type-left_tab);
            Message::help_text(canvas.get(), screen.get(), screen_buffer.get(), up_tab, left_tab);
            canvas->screen_show(screen.get(), screen_buffer.get());
            // **保持游戏画面显示
            screen->print_file(Message::get_data_name().c_str());
            screen->clean();
            canvas->screen_show(screen.get(), screen_buffer.get());
            Sleep(330);
            controler->messenger = HELP;
            break;
        case HELP:  // Show help messages
            // **保持游戏画面显示
            canvas->set_info(0);
            pad->print_to_mono(screen.get(), icon_size.get());
            Message::print_log(screen.get(), pad.get());
            // **保持游戏画面显示

            screen->clean(up_tab, pad_height*pad_type-up_tab, left_tab, pad_width*pad_type-left_tab);
            Message::help_text(canvas.get(), screen.get(), screen_buffer.get(), up_tab, left_tab);
            canvas->screen_show(screen.get(), screen_buffer.get());
            break;
        case NORMAL:  // 表达游戏界面
            canvas->set_info(0);
            pad->print_to_mono(screen.get(), icon_size.get());
            screen->update(shell.get(), true);
            canvas->screen_show(screen.get(), screen_buffer.get());
            screen->clean();
            screen->get_icon_roll(ICON_FILES[0], &pad_cell, pad_width, pad_height);

            screen->clean(0, pad_type * space_type,
                          0, pad_type * space_type);
            screen->clean(0, pad_type * space_type,
                          pad_type * (pad_width - space_type), pad_type * pad_width);
            screen->clean(pad_type * (pad_height - space_type),
                          pad_type * pad_height, 0, pad_type * space_type);
            screen->clean(pad_type * (pad_height - space_type),
                          pad_type * pad_height, pad_type * (pad_width - space_type), pad_type * pad_width);

            Message::print_log(screen.get(), pad.get());  // 显示文字信息
            break;
        default:
            return 65535;
        }
    }

    return 0;
}
