#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

/* MACROS */
#define DISP_ERR(e, s) {\
    int err = e;\
    if (err == ERR) { fprintf(stderr, "%d: ERROR: Function returned ERR: " #e s "\n", __LINE__); }\
}

/* DEFINES */
#define HOUR 60
#define QUARTER (HOUR / 4)
#define HALFHOUR (HOUR / 2)
#define LINESTEP QUARTER /* how many minutes a line represents */
#define DAY_START (0 * HOUR)
#define DAY_END (23 * HOUR)
#define DAY_VIRT_HEIGHT ((DAY_END - DAY_START + HOUR) / LINESTEP) /* + HOUR because inclusive */
#define DAY_PHYS_WIDTH 40 /* physical width of day ScrollWin */
#define SCROLLWIN_PADDING 1
/* additional ncurses colors */
#define COLOR_WHITE_BRIGHT 15
/* ncurses color pairs */
#define PAIR_SLOT_HEADER 1
#define PAIR_SLOT_BG 2

/* TYPE DEFINITIONS */
typedef int Minute;
typedef int Hour;

/* TODO: how to deal with different day lengths in a week ?
 * just ban it ? seems easiest and most sensical) */
/* the container window is merely a border to the pad (if padding >= 1) */
typedef struct {
    WINDOW* container;
    WINDOW* pad;
    int offset;
    int container_dirty;
} ScrollWin;

typedef struct {
    Minute start_time;
    Minute duration;
    char* msg;
} Slot;

typedef struct {
    ScrollWin* win;
    int slot_count;
    Slot* slots;
} Day;

typedef struct {
    int day_count;
    Day* days;
} Week;

/* FUNCTION DECLARATIONS */
/* general */
void ncurses_init(void);
void loop(void);
/* slots */
Slot slot_create(Minute start_time, Minute duration, char const * const msg);
void slot_destroy(Slot slot);
void slot_draw(ScrollWin* win, Slot slot);
/* days */
Day day_create(int slot_count, Slot* slots, int virt_height,
               int day_count, int index);
void day_destroy(Day day);
void day_draw(Day day);
/* weeks */
void week_destroy(Week week);
void week_draw(Week week);
/* scrollwins */
ScrollWin* scrollwin_create(int virt_height, int phys_height, int phys_width,
                           int begin_y, int begin_x);
void scrollwin_destroy(ScrollWin* win);
void scrollwin_draw(ScrollWin* win);
void scrollwin_clear_inner(ScrollWin* win);
void scrollwin_draw_slot_header(ScrollWin* win, Slot slot);
void scrollwin_draw_slot_bg(ScrollWin* win, Slot slot);
char* scrollwin_format_slot_header(ScrollWin* win, Slot slot);
void scrollwin_scroll(ScrollWin* win, int delta);
/* scrollwin gets */
int scrollwin_get_begin_y(ScrollWin* win);
int scrollwin_get_begin_x(ScrollWin* win);
int scrollwin_get_phys_height(ScrollWin* win);
int scrollwin_get_phys_width(ScrollWin* win);
int scrollwin_get_virt_height(ScrollWin* win);
int scrollwin_get_virt_width(ScrollWin* win);
/* utility */
int min_to_line(Minute m);
Hour min_to_hour(Minute m);

