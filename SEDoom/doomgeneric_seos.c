#include <string.h>
#include "doomkeys.h"
#include "m_argv.h"
#include "doomgeneric.h"
#include <stdio.h>
#include <unistd.h>

#include <stdbool.h>
#include <ctype.h>
#include <segui.h>
#include "user_syscall.h"

#define KEYQUEUE_SIZE 16

// If Doom lags then change this to 0 and recompile
#define SEDOOM_USE_SYSCALL_TICK_SLEEP 1

#define KB_RETURN 0x1C
#define KB_ESC 0x01

#define _KF(x) (x + 0x3B)

#define KB_F1 _KF(1)
#define KB_F2 _KF(2)
#define KB_F3 _KF(3)
#define KB_F4 _KF(4) 
#define KB_F5 _KF(5)
#define KB_F6 _KF(6)
#define KB_F7 _KF(7) 
#define KB_F8 _KF(8) 
#define KB_F9 _KF(9)
#define KB_F10 _KF(10)

#define KB_F11 0x57
#define KB_F12 0x58

#define KB_CTRL 0x1D
#define KB_ALT 0x38

#define KB_LFT 0x4B
#define KB_RGT 0x4D
#define KB_UPP 0x48
#define KB_DWN 0x50

#define KB_SPACE 0x39

#define KB_EQUALS 0x0D
#define KB_MINUS 0x0C

#define KB_LSHIFT 0x2A
#define KB_RSHIFT 0x36


static unsigned short s_KeyQueue[KEYQUEUE_SIZE];
static unsigned int s_KeyQueueWriteIndex = 0;
static unsigned int s_KeyQueueReadIndex = 0;

window_t *window = NULL;

bool isShift = 0;

char kcodeTochar(uint8_t scancode)
{
    char key = 0;
    if (scancode < 80)
    {
        switch (scancode)
        {
        case 0xFA:
            break;
        case 0x45:
        case 0xC5:
            break;
        case 0x2A:
        case 0x36:
            isShift = 1;
            break;
        case 0x0E:
            key = '\b';
            break;
        case 0x0F:
            key = '\t';
            break;
        case 0x29:
            if (!isShift)
                key = '`';
            else
                key = '~';
            break;
        case 0x02:
            if (!isShift)
                key = '1';
            else
                key = '!';
            break;
        case 0x03:
            if (!isShift)
                key = '2';
            else
                key = '@';
            break;
        case 0x04:
            if (!isShift)
                key = '3';
            else
                key = '#';
            break;
        case 0x05:
            if (!isShift)
                key = '4';
            else
                key = '$';
            break;
        case 0x06:
            if (!isShift)
                key = '5';
            else
                key = '%';
            break;
        case 0x07:
            if (!isShift)
                key = '6';
            else
                key = '^';
            break;
        case 0x08:
            if (!isShift)
                key = '7';
            else
                key = '&';
            break;
        case 0x09:
            if (!isShift)
                key = '8';
            else
                key = '*';
            break;
        case 0x0A:
            if (!isShift)
                key = '9';
            else
                key = '(';
            break;
        case 0x0B:
            if (!isShift)
                key = '0';
            else
                key = ')';
            break;
        case 0x0C:
            if (!isShift)
                key = '-';
            else
                key = '_';
            break;
        case 0x0D:
            if (!isShift)
                key = '=';
            else
                key = '+';
            break;

        case 0x10:
            if (!isShift)
                key = 'q';
            else
                key = 'Q';
            break;
        case 0x11:
            if (!isShift)
                key = 'w';
            else
                key = 'W';
            break;
        case 0x12:
            if (!isShift)
                key = 'e';
            else
                key = 'E';
            break;
        case 0x13:
            if (!isShift)
                key = 'r';
            else
                key = 'R';
            break;
        case 0x14:
            if (!isShift)
                key = 't';
            else
                key = 'T';
            break;
        case 0x15:
            if (!isShift)
                key = 'y';
            else
                key = 'Y';
            break;
        case 0x16:
            if (!isShift)
                key = 'u';
            else
                key = 'U';
            break;
        case 0x17:
            if (!isShift)
                key = 'i';
            else
                key = 'I';
            break;
        case 0x18:
            if (!isShift)
                key = 'o';
            else
                key = 'O';
            break;
        case 0x19:
            if (!isShift)
                key = 'p';
            else
                key = 'P';
            break;
        case 0x1A:
            if (!isShift)
                key = '[';
            else
                key = '{';
            break;
        case 0x1B:
            if (!isShift)
                key = ']';
            else
                key = '}';
            break;
        case 0x1C:
            if (!isShift)
                key = '\n';
            else
                key = '\n';
            break;

        case 0x1E:
            if (!isShift)
                key = 'a';
            else
                key = 'A';
            break;
        case 0x1F:
            if (!isShift)
                key = 's';
            else
                key = 'S';
            break;
        case 0x20:
            if (!isShift)
                key = 'd';
            else
                key = 'D';
            break;
        case 0x21:
            if (!isShift)
                key = 'f';
            else
                key = 'F';
            break;
        case 0x22:
            if (!isShift)
                key = 'g';
            else
                key = 'G';
            break;
        case 0x23:
            if (!isShift)
                key = 'h';
            else
                key = 'H';
            break;
        case 0x24:
            if (!isShift)
                key = 'j';
            else
                key = 'J';
            break;
        case 0x25:
            if (!isShift)
                key = 'k';
            else
                key = 'K';
            break;
        case 0x26:
            if (!isShift)
                key = 'l';
            else
                key = 'L';
            break;
        case 0x27:
            if (!isShift)
                key = ';';
            else
                key = ':';
            break;

        case 0x2C:
            if (!isShift)
                key = 'z';
            else
                key = 'Z';
            break;
        case 0x2D:
            if (!isShift)
                key = 'x';
            else
                key = 'X';
            break;
        case 0x2E:
            if (!isShift)
                key = 'c';
            else
                key = 'C';
            break;
        case 0x2F:
            if (!isShift)
                key = 'v';
            else
                key = 'V';
            break;
        case 0x30:
            if (!isShift)
                key = 'b';
            else
                key = 'B';
            break;
        case 0x31:
            if (!isShift)
                key = 'n';
            else
                key = 'N';
            break;
        case 0x32:
            if (!isShift)
                key = 'm';
            else
                key = 'M';
            break;
        case 0x33:
            if (!isShift)
                key = ',';
            else
                key = '<';
            break;
        case 0x34:
            if (!isShift)
                key = '.';
            else
                key = '>';
            break;
        case 0x35:
            if (!isShift)
                key = '/';
            else
                key = '?';
            break;
        case 0x39:
            if (!isShift)
                key = ' ';
            else
                key = ' ';
            break;
        case 0x2B:
            if (!isShift)
                key = '\\';
            else
                key = '|';
            break;
        case 0x3A:
            if (isShift)
            {
                isShift = false;
            }
            else
            {
                isShift = true;
            }
            break;
        default:
            key = 0;
            break;
        }
    }
    else if(scancode == 0xaa || scancode == 0xb6)
    {
        isShift = 0;
    }
    else
    {
    }

    return key;
}

static uint8_t convert_to_doomkey(uint8_t scancode)
{
    uint8_t ret = 0;
    switch(scancode)
    {
    case KB_UPP:
        ret = KEY_UPARROW;
        break;
    case KB_DWN:
        ret = KEY_DOWNARROW;
        break;
    case KB_LFT:
        ret = KEY_LEFTARROW;
        break;
    case KB_RGT:
        ret = KEY_RIGHTARROW;
        break;
    case KB_RETURN:
        ret = KEY_ENTER;
        break;
    case KB_ESC:
        ret = KEY_ESCAPE;
        break;
    case KB_CTRL:
        ret = KEY_FIRE;
        break;
        break;
    case KB_SPACE:
        ret = KEY_USE;
        break;
    case KB_LSHIFT:
    case KB_RSHIFT:
        ret = KEY_RSHIFT;
        break;
    case KB_EQUALS:
        ret = KEY_EQUALS;
        break;
    case KB_MINUS:
        ret = KEY_MINUS;
        break;
    case KB_F1:
        ret = KEY_F1;
        break;
    case KB_F2:
        ret = KEY_F2;
        break;
    case KB_F3:
        ret = KEY_F3;
        break;
    case KB_F4:
        ret = KEY_F4;
        break;
    case KB_F5:
        ret = KEY_F5;
        break;
    case KB_F6:
        ret = KEY_F6;
        break;
    case KB_F7:
        ret = KEY_F7;
        break;
    case KB_F8:
        ret = KEY_F8;
        break;
    case KB_F9:
        ret = KEY_F9;
        break;
    case KB_F10:
        ret = KEY_F10;
        break;
    case KB_F11:
        ret = KEY_F11;
        break;
    case KB_F12:
        ret = KEY_F12;
        break;
    default:
        ret = tolower(kcodeTochar(scancode));
        break;
    };
    return ret;
}

uint32_t ticks_since = 0;
uint32_t countdown = 0;

#if ! SEDOOM_USE_SYSCALL_TICK_SLEEP
void d_tickCallback()
{
    if(countdown != 0)
        countdown--;
    ticks_since += 1;
}
#endif

static void addKeyToQueue(int pressed, unsigned int keyCode)
{
    unsigned char key = convert_to_doomkey(keyCode);

    unsigned short keyData = (pressed << 8) | key;

    s_KeyQueue[s_KeyQueueWriteIndex] = keyData;
    s_KeyQueueWriteIndex++;
    s_KeyQueueWriteIndex %= KEYQUEUE_SIZE;
}

void handle_keys(char c, int isCTRL, int isALT, uint8_t keycode)
{
    if(keycode < 0x80)
    {
        if(keycode == 0x40)
        {
            memset(s_KeyQueue, 0, 16);
            s_KeyQueueWriteIndex = 0;
            s_KeyQueueReadIndex = 0;
            return;
        }
        addKeyToQueue(1, keycode);
    }
    else
    {
        addKeyToQueue(0, (keycode - 0x80));
    }
}

void DG_Init()
{
    window_info_t *info = malloc(sizeof(window_info_t));
    memset(info, 0, sizeof(window_info_t));
    info->width = DOOMGENERIC_RESX;
    info->height = DOOMGENERIC_RESY;
    info->x = 100;
    info->y = 100;
    strcpy(info->title, "SEDoom");
    info->bpp = 32;
    info->pixelformat = 0;
    window = create_window(info);
    SYSCALL_ATTACH(handle_keys);
}

void DG_DrawFrame()
{
    window_display(window, DG_ScreenBuffer);
}

void DG_SleepMs(uint32_t ms)
{
    if(ms == 1)
    {
        // ticks_since++;
        return;
    }
#if SEDOOM_USE_SYSCALL_TICK_SLEEP
    SYSCALL_SLEEP(ms);
#else
    uint32_t ticks_before = ticks_since;
    uint32_t end_ticks = ticks_since + ms;
    while(end_ticks > ticks_since)
    return;
#endif
}

int DG_GetKey(int* pressed, unsigned char* doomKey)
{
    if (s_KeyQueueReadIndex == s_KeyQueueWriteIndex){
        //key queue is empty
        return 0;
    }
    else
    {
        unsigned short keyData = s_KeyQueue[s_KeyQueueReadIndex];
        s_KeyQueueReadIndex++;
        s_KeyQueueReadIndex %= KEYQUEUE_SIZE;

        *pressed = keyData >> 8;
        *doomKey = keyData & 0xFF;

        return 1;
    }

  return 0;
}

void DG_SetWindowTitle(const char * title)
{
    window_change_title(window, title);
}

uint32_t DG_GetTicksMs()
{
#if SEDOOM_USE_SYSCALL_TICK_SLEEP
    uint32_t ret = 0;
    SYSCALL_GET_TICKS(ret);
    return ret;
#else
    uint32_t ret = ticks_since;
    ticks_since++;
    return ret;
#endif
}

int main(int argc, char **argv)
{
    doomgeneric_Create(argc, argv);

    while(1)
        doomgeneric_Tick();

    return 0;
}