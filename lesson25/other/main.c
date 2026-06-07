//#include "mystdio.h"
//#include "mystring.h"
//
//int main()
//{
//  mystrlen();
//
//  return 0;
//}


//#include <ncurses.h>
//#include <math.h>
//
//#define PI 3.14159265358979323846
//
//int main() 
//{
//    // 初始化 ncurses
//    initscr();
//    clear();
//    noecho();
//    curs_set(FALSE); // 隐藏光标
//
//    int width = COLS, height = LINES;
//    double t;
//    int x, y;
//
//    // 绘制心形曲线
//    for (t = 0; t < 2 * PI; t += 0.01) {
//        // 根据参数方程计算坐标
//        x = (int)(width / 2 + 16 * sin(t) * sin(t) * sin(t));
//        y = (int)(height / 2 - (13 * cos(t) - 5 * cos(2 * t) - 2 * cos(3 * t) - cos(4 * t)));
//
//        // 移动到计算出的位置并打印字符
//        mvaddch(y, x, '*');
//    }
//
//    refresh(); // 刷新屏幕以显示内容
//    getch(); // 等待用户按键
//    endwin(); // 结束并退出 ncurses
//
//    return 0;
//}



#include <ncurses.h>
#include <math.h>

#define PI 3.14159265358979323846

int main() {
    initscr();
    clear();
    noecho();
    curs_set(FALSE);

    int width = COLS, height = LINES;

    // 调整比例：横向拉伸，纵向压缩，使心形更宽更圆
    double scale_x = 20;  // 增大横向缩放
    double scale_y = 8;   // 减小纵向缩放
    double t = 0;

    for (t = 0; t <= 2 * PI; t += 0.05) {
        // 心形参数方程（经典心脏线变种）
        double x = scale_x * sin(t) * sin(t) * sin(t);
        double y = scale_y * (cos(t) - 0.5 * cos(2*t) - 0.2 * cos(3*t) - 0.1 * cos(4*t));

        int cx = width / 2 + (int)x;
        int cy = height / 2 - (int)y;  // y 轴反向（因为终端从上到下）

        if (cx >= 0 && cx < width && cy >= 0 && cy < height) {
            mvaddch(cy, cx, '*');
        }
    }

    refresh();
    getch();
    endwin();

    return 0;
}


