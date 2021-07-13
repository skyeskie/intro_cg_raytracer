#include "Screen.h"
Screen::Screen(int x, int y)
    :window_wid(x), window_len(y)
{}


int Screen::get_len()
{
    return window_len;
}

int Screen::get_wid()
{
    return window_wid;
}
