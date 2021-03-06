/* Copyright (C) KolibriOS team 2004-2021. All rights reserved. */
/* Distributed under terms of the GNU General Public License    */

/* This file contains basic wrappers over KolibriOS system calls. */
/* See sysfuncs.txt file for details. */

/*
 * This file was created with you in mind. Lest you reinvent the wheel. 
 * If for some reason there is not enough wrapper add! I ask you to stick to the same style: snake_case. 
 * Structure names must start with "ksys_" and end with "_t". 
 * All wrappers must start with the "_ksys_" prefix. 
 * I consider it mandatory to place the wrappers in the correct order in the official documentation. 
 * Enjoy writing your code :)

 * Warning! The end of the file is the old definitions of function/structure names. 
 * They are for compatibility... Better not to use them. 
*/
 
#ifndef _KSYS_H_
#define _KSYS_H_

#include <stddef.h>

#define asm_inline __asm__ __volatile__

/*============== General structures ==============*/

#pragma pack(push,1)

typedef struct {
    uint8_t blue;
    uint8_t green;
    uint8_t red;
}rgb_t;

typedef union {
    uint32_t val;
    struct{
        uint8_t hour;
        uint8_t min;
        uint8_t sec;
        uint8_t _zero;
    };
}ksys_time_t;

typedef union {
    uint32_t val;
    struct{
        uint8_t  year;
        uint8_t  month;
        uint8_t  day;
        uint8_t  _zero;
    };
}ksys_date_t;

typedef union{
    uint32_t val;
    struct{
        uint16_t  x;
        uint16_t  y;
    };
}ksys_pos_t;

typedef union ksys_oskey_t{
    uint32_t val;
    struct{
        uint8_t state;
        uint8_t code;
        uint8_t ctrl_key;
    };
}ksys_oskey_t;

typedef struct{
    void *data;
    size_t size;
}ksys_ufile_t;


typedef struct{
    uint32_t            p00;
    union{
        uint64_t        p04; 
        struct {
            uint32_t    p04dw;
            uint32_t    p08dw;
        };
    };
    uint32_t            p12;
    union {
        uint32_t        p16;
        const char     *new_name;
        void           *bdfe;
        void           *buf16;
        const void     *cbuf16;
    };
    char                p20;
    const char         *p21;
}ksys70_t;

typedef struct {
    uint32_t attributes;
    uint32_t name_cp;
    ksys_time_t creation_time;
    ksys_date_t creation_date;
    ksys_time_t last_access_time;
    ksys_date_t last_access_date;
    ksys_time_t last_modification_time;
    ksys_date_t last_modification_date;
    unsigned long long size;
    char name[0];
}ksys_bdfe_t;

typedef struct {
  int cpu_usage;             //+0
  int window_pos_info;       //+4
  short int reserved1;       //+8
  char name[12];             //+10
  int memstart;              //+22
  int memused;               //+26
  int pid;                   //+30
  int winx_start;            //+34
  int winy_start;            //+38
  int winx_size;             //+42
  int winy_size;             //+46
  short int slot_info;       //+50
  short int reserved2;       //+52
  int clientx;               //+54
  int clienty;               //+58
  int clientwidth;           //+62
  int clientheight;          //+66
  unsigned char window_state;//+70
  char reserved3[1024-71];   //+71
}ksys_thread_t;

typedef unsigned int ksys_color_t;

typedef struct{
    ksys_color_t frame_area;
    ksys_color_t grab_bar;
    ksys_color_t grab_bar_button; 
    ksys_color_t grab_button_text;
    ksys_color_t grab_text;
    ksys_color_t work_area;
    ksys_color_t work_button;
    ksys_color_t work_button_text;
    ksys_color_t work_text;
    ksys_color_t work_graph;
}ksys_colors_table_t;

typedef struct{
    unsigned pid;      // PID of sending thread
    unsigned datalen;  // data bytes
    char     *data;    // data begin
}ksys_ipc_msg;
 
typedef struct{
    unsigned lock;              // nonzero is locked
    unsigned used;              // used bytes in buffer
    ksys_ipc_msg *data;         // data begin
}ksys_ipc_buffer;

typedef struct {
    char* func_name;
    void* func_ptr;
}ksys_dll_t;

typedef unsigned ksys_drv_hand_t;

typedef struct{
    ksys_drv_hand_t handler;
    unsigned func_num;
    void* in_data_ptr;
    unsigned in_data_size;
    void* out_data_ptr;
    unsigned out_data_size;
}ksys_ioctl_t;

typedef struct{
    char  key[64];
    char path[64];
}ksys_dir_key_t;

#pragma pack(pop)

typedef rgb_t ksys_bitmap_t;

enum KSYS_FS_ERRORS {
    KSYS_FS_ERR_SUCCESS = 0,  // Success
    KSYS_FS_ERR_1       = 1,  // Base and/or partition of a hard disk is not defined (fn21.7 & fn21.8)
    KSYS_FS_ERR_2       = 2,  // Function is not supported for the given file system
    KSYS_FS_ERR_3       = 3,  // Unknown file system
    KSYS_FS_ERR_4       = 4,  // Reserved, is never returned in the current implementation
    KSYS_FS_ERR_5       = 5,  // File not found
    KSYS_FS_ERR_EOF     = 6,  // End of file, EOF
    KSYS_FS_ERR_7       = 7,  // Pointer lies outside of application memory
    KSYS_FS_ERR_8       = 8,  // Disk is full
    KSYS_FS_ERR_9       = 9,  // FAT table is destroyed
    KSYS_FS_ERR_10      = 10, // Access denied
    KSYS_FS_ERR_11      = 11 // Device error
};

enum KSYS_EVENTS {
    KSYS_EVENT_NONE = 0,     /* Event queue is empty */
    KSYS_EVENT_REDRAW = 1,   /* Window and window elements should be redrawn */
    KSYS_EVENT_KEY = 2,      /* A key on the keyboard was pressed */
    KSYS_EVENT_BUTTON = 3,   /* A button was clicked with the mouse */
    KSYS_EVENT_DESKTOP = 5,  /* Desktop redraw finished */
    KSYS_EVENT_MOUSE = 6,    /* Mouse activity (movement, button press) was detected */
    KSYS_EVENT_IPC = 7,      /* Interprocess communication notify */
    KSYS_EVENT_NETWORK = 8,  /* Network event */
    KSYS_EVENT_DEBUG = 9,    /* Debug subsystem event */
    KSYS_EVENT_IRQBEGIN = 16 /* 16..31 IRQ0..IRQ15 interrupt =IRQBEGIN+IRQn */
};

enum KSYS_FILE_ENCODING{
    KSYS_FILE_CP866 =1,
    KSYS_FILE_UTF16LE = 2,
    KSYS_FILE_UTF8 = 3
};

static inline
int __strcmp(const char * s1, const char * s2 )
{
    while ((*s1) && (*s1 == *s2)){ ++s1; ++s2; }
    return(*(unsigned char*)s1 - *(unsigned char *)s2);
}

/* ####################################################################### */
/* ############### ?? wrappers for system calls Kolibri OS ################ */
/* ####################################################################### */


/*=============== Function -1 - terminate thread/process ===============*/

static inline
void _ksys_exit(){
    asm_inline("int $0x40"::"a"(-1));
}

/*============== Function 0 - define and draw the window. ==============*/

static inline 
void _ksys_create_window(uint32_t x, uint32_t y, uint32_t w, uint32_t h, const char *name, ksys_color_t workcolor, uint32_t style) {
    asm_inline(
        "int $0x40"
        ::"a"(0),
        "b"((x << 16) | ((w-1) & 0xFFFF)),
        "c"((y << 16) | ((h-1) & 0xFFFF)),
        "d"((style << 24) | (workcolor & 0xFFFFFF)),
        "D"(name),
        "S"(0) 
        :"memory"
     );
};

/*================ Function 1 - put pixel in the window. ===============*/

static inline
void _ksys_draw_pixel(uint32_t x, uint32_t y, ksys_color_t color){
    asm_inline(
        "int $0x40"
        ::"a"(1), "b"(x), "c"(y), "d"(color)
    );
}

/*============ Function 2 - get the code of the pressed key. ===========*/

static inline 
ksys_oskey_t _ksys_get_key(void){
    ksys_oskey_t val;
    asm_inline(
        "int $0x40"
        :"=a"(val)
        :"a"(2)
    );
    return val;
}

/*==================== Function 3 - get system time. ===================*/

static inline
ksys_time_t _ksys_get_time(){
    ksys_time_t c_time;
    asm_inline(
        "int $0x40"
        :"=a"(c_time)
        :"a"(3)
        :"memory"
    );
    return c_time;
}

/*=================== Function 4 - draw text string. ===================*/

static inline
void _ksys_draw_text(const char *text, uint32_t x, uint32_t y, uint32_t len, ksys_color_t color) {
   asm_inline(
        "int $0x40"
        ::"a"(4),"d"(text),
        "b"((x << 16) | y),
        "S"(len),"c"(color)
        :"memory"
    );
}

/*========================= Function 5 - delay. ========================*/

static inline 
void _ksys_delay(uint32_t time){
    asm_inline(
        "int $0x40"
        ::"a"(5), "b"(time)
        :"memory"
    );
}

/*=============== Function 7 - draw image in the window. ===============*/

static inline
void _ksys_draw_bitmap(void *bitmap, int x, int y, int w, int h)
{
    asm_inline(
        "int $0x40"
        ::"a"(7), "b"(bitmap),
        "c"((w << 16) | h),
        "d"((x << 16) | y)
        :"memory"
    );
}

/*=============== Function 8 - define/delete the button. ===============*/

static inline
void _ksys_define_button(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t id, ksys_color_t color) {
   asm_inline(
        "int $0x40"
        ::"a"(8),
        "b"((x<<16)+w),
        "c"((y<<16)+h),
        "d"(id),
        "S"(color)
    );
};

static inline
void _ksys_delete_button(uint32_t id){
    asm_inline(
        "int $0x40"
        ::"a"(8),"d"(id & 0x00FFFFFF | 0x80000000)
    );
} 

/*============ Function 9 - information on execution thread. ===========*/

static inline 
int _ksys_thread_info(ksys_thread_t* table, int slot){
    int val;
    asm_inline(
        "int $0x40"
        :"=a"(val)
        :"a"(9), "b"(table), "c"(slot)
        :"memory"
    );
    return val;
}

/*==================== Function 10 - wait for event. ===================*/

static inline
uint32_t _ksys_wait_event(void){
    uint32_t val;
    asm_inline(
        "int $0x40"
        :"=a"(val)
        :"a"(10)
    );
    return val;
}

/*=============== Function 11 - check for event, no wait. ==============*/

static inline 
uint32_t _ksys_check_event(void){
    uint32_t val;
    asm_inline(
        "int $0x40"
        :"=a"(val)
        :"a"(11)
    );
    return val;
}

/*=============== Function 12 - begin/end window redraw. ===============*/

static inline 
void _ksys_start_draw(){
   asm_inline("int $0x40"::"a"(12),"b"(1));
}

static inline 
void _ksys_end_draw(){
    asm_inline("int $0x40" ::"a"(12),"b"(2));
}

/*============ Function 13 - draw a rectangle in the window. ===========*/

static inline
void _ksys_draw_bar(uint32_t x, uint32_t y, uint32_t w, uint32_t h, ksys_color_t color){
    asm_inline(
        "int $0x40"
        ::"a"(13), "d"(color),
        "b"((x << 16) | w),
        "c"((y << 16) | h)
    );
}

/*=================== Function 14 - get screen size. ===================*/

static inline
ksys_pos_t _ksys_screen_size(){
	ksys_pos_t size;
    ksys_pos_t size_tmp;
    asm_inline(
        "int $0x40"
        :"=a"(size_tmp)
        :"a"(14)
    );
    size.x = size_tmp.y;
    size.y = size_tmp.x; 
    return size;
}

/*== Function 15, subfunction 1 - set a size of the background image. ==*/

static inline
void _ksys_bg_set_size(uint32_t w, uint32_t h){
    asm_inline(
        "int $0x40"
        ::"a"(15), "b"(1), "c"(w), "d"(h)
    );
}

/*=== Function 15, subfunction 2 - put pixel on the background image. ==*/

static inline
void _ksys_bg_put_pixel(uint32_t x, uint32_t y, uint32_t w, ksys_color_t color){
    asm_inline(
        "int $0x40"
        ::"a"(15), "b"(2), "c"((x+y*w)*3), "d"(color)
    );
}

/*=========== Function 15, subfunction 3 - redraw background. ==========*/

static inline
void _ksys_bg_redraw(void){
    asm_inline(
        "int $0x40"
        ::"a"(15), "b"(3)
    );
}

/*== Function 15, subfunction 4 - set drawing mode for the background. =*/

enum KSYS_BG_MODES{
    KSYS_BG_MODE_PAVE=1,
    KSYS_BG_MODE_STRETCH=2
};

static inline 
void _ksys_bg_set_mode(uint32_t mode){
    asm_inline(
        "int $0x40"
        ::"a"(15), "b"(4), "c"(mode)
    );
}

/*===================== Function 15, subfunction 5 =====================*/
/*============ Put block of pixels on the background image. ============*/

static inline 
void _ksys_bg_put_bitmap(ksys_bitmap_t* bitmap, size_t bitmap_size, uint32_t x, uint32_t y, uint32_t w){
    asm_inline(
        "int $0x40"
        ::"a"(15), "b"(5), "c"(bitmap), "d"((x+y*w)*3), "S"(bitmap_size)
    );
}

/*===================== Function 15, subfunction 6 =====================*/
/*======= Map background data to the address space of process. ==========*/

static inline
ksys_bitmap_t* _ksys_bg_get_map()
{
    ksys_bitmap_t *bitmap;
    asm_inline(
        "int $0x40"
        :"=a"(bitmap)
        :"a"(15), "b"(6)
    );
    return bitmap;
}

/*===== Function 15, subfunction 7 - close mapped background data. =====*/

static inline
int _ksys_bg_close_map(ksys_bitmap_t* bitmap)
{   
    int status; // 1 - OK, 0 - ERROR
    asm_inline(
        "int $0x40"
        :"=a"(status)
        :"a"(15), "b"(7), "c"(bitmap)
    );
    return status;
}

/*===================== Function 15, subfunction 9 =====================*/
/*============= Redraws a rectangular part of the background ===========*/

static inline
void _ksys_bg_redraw_bar(ksys_pos_t angle1, ksys_pos_t angle2)
{
    asm_inline(
        "int $0x40"
        ::"a"(15), "b"(9), 
        "c"(angle1.x*(1<<16)+angle2.x),
        "d"(angle1.y*(1<<16)+angle2.y)
    );
}

/*=============== Function 16 - save ramdisk on a floppy. ==============*/

static inline
int _ksys_save_ramdisk_fd(uint32_t floppy_id)
{
    int status; // 0 - OK, 1 - ERROR
    asm_inline(
        "int $0x40"
        :"=a"(status)
        :"a"(16), "b"(floppy_id)
    );
    return status;    
}

/*======= Function 17 - get the identifier of the pressed button. ======*/

static inline
uint32_t _ksys_get_button()
{
    unsigned val;
    asm_inline(
        "int $0x40"
        :"=a"(val)
        :"a"(17)
    );
    return val>>8; 
}

/*===================== Function 18, subfunction 1 =====================*/
/*============= Make deactive the window of the given thread. ==========*/

static inline 
void _ksys_unfocus_window(int slot)
{
    asm_inline(
        "int $0x40"
        ::"a"(18), "b"(1), "c"(slot)
    );
}

/*= Function 18, subfunction 2 - terminate process/thread by the slot. =*/

static inline 
void _ksys_kill_by_slot(int slot)
{
    asm_inline(
        "int $0x40"
        ::"a"(18), "b"(2), "c"(slot)
    );
}

/*===================== Function 18, subfunction 3 =====================*/
/*============= Make active the window of the given thread. ============*/

static inline 
void _ksys_focus_window(int slot){
    asm_inline(
        "int $0x40"
        ::"a"(18), "b"(3), "c"(slot)
    );
}

/*===================== Function 18, subfunction 4 =====================*/
/*=========== Get counter of idle time units per one second. ===========*/

static inline
uint32_t  _ksys_get_idle(void){
    uint32_t sec;
    asm_inline(
        "int $0x40"
        :"=a"(sec)
        :"a"(18), "b"(4)
    );
    return sec;
}

/*========== Function 18, subfunction 5 - get CPU clock rate. ==========*/
/*================ modulo 2^32 clock ticks = 4GHz ======================*/

static inline
uint32_t _ksys_get_cpu_clock(void){
    uint32_t clock;
    asm_inline(
        "int $0x40"
        :"=a"(clock)
        :"a"(18), "b"(5)
    );
    return clock;
}

/* Function 18, subfunction 6 - save ramdisk to the file on hard drive. */

static inline
uint32_t _ksys_save_ramdisk_hd(const char* ramdisk_path){
    uint32_t fs_err;
    asm_inline(
        "int $0x40"
        :"=a"(fs_err)
        :"a"(18), "b"(6), "c"(ramdisk_path)
    );
    return fs_err;
}

/* Function 18, subfunction 9 - system shutdown with the parameter. */

enum KSYS_SHD_PARAM {
    KSYS_SHD_POWEROFF    = 2,
    KSYS_SHD_REBOOT      = 3,
    KSYS_SHD_RESTART_KRN = 4
};

static inline
void _ksys_shutdown(uint32_t shd_param){
    asm_inline(
        "int $0x40"
        ::"a"(18), "b"(9), "c"(shd_param)
    );
}

/*========= Function 18, subfunction 16 - get size of free RAM. ========*/

static inline
size_t _ksys_get_ram_size(void){
    size_t size;
    asm_inline(
        "int $0x40"
        :"=a"(size)
        :"a"(18), "b"(16) 
    );
    return size;
}

/*======== Function 18, subfunction 17 - get full amount of RAM. =======*/

static inline
size_t _ksys_get_full_ram(void){
    size_t size;
    asm_inline(
        "int $0x40"
        :"=a"(size)
        :"a"(18), "b"(17) 
    );
    return size;
}

/*===================== Function 18, subfunction 18 ====================*/
/*============= Terminate process/thread by the identifier. ============*/

static inline
void _ksys_kill_by_pid(uint32_t PID)
{
    asm_inline(
        "int $0x40"
        ::"a"(18), "b"(18), "c"(PID)
    );
}

/*===================== Function 18, subfunction 21 ====================*/
/*=====Get the slot number of the process / thread by identifier.. =====*/

static inline 
int _ksys_get_thread_slot(int PID){
    int val;
    asm_inline(
        "int $0x40"
        :"=a"(val)
        :"a"(18), "b"(21), "c"(PID)
    );
    return val;
}

/*============= Function 23 - wait for event with timeout. =============*/

static inline
uint32_t _ksys_wait_event_timeout(uint32_t timeout){
    unsigned val;
    asm_inline(
        "int $0x40"
        :"=a"(val)
        :"a"(23), "b"(timeout)
    );
    return val;
}

/*=== Function 26, subfunction 9 - get the value of the time counter. ==*/

static inline
uint32_t _ksys_get_tick_count(){
    unsigned val;
    asm_inline(
        "int $0x40"
        :"=a"(val)
        :"a"(26),"b"(9)
    );
    return val;
}

/*===================== Function 26, subfunction 10 ====================*/
/*========== Get the value of the high precision time counter. =========*/

static inline
uint64_t  _ksys_get_ns_count(){
    uint64_t val;
    asm_inline(
        "int $0x40"
        :"=A"(val)
        :"a"(26), "b"(10)
    );
    return val;
}

/*=================== Function 29 - get system date. ===================*/

static inline
ksys_date_t _ksys_get_date(){
    ksys_date_t val;
    asm_inline("int $0x40":"=a"(val):"a"(29));
    return val;
}

/*===========+ Function 30 - work with the current folder.==============*/
/*--------- Subfunction 1 - set current folder for the thread. ---------*/

static inline 
void _ksys_setcwd(char* dir){
    asm_inline(
        "int $0x40"
        ::"a"(30), "b"(1), "c"(dir)
    );
}
 
/*--------- Subfunction 2 - get current folder for the thread. ---------*/

static inline 
int _ksys_getcwd(char* buf, int bufsize){
    register int val;
    asm_inline(
        "int $0x40"
        :"=a"(val):"a"(30), "b"(2), "c"(buf), "d"(bufsize)
    );
    return val;
}

/* ---- Subfunction 3 - install the add.system directory for the kernel ------*/

static inline
int _ksys_set_kernel_dir(ksys_dir_key_t *table){
    register int val;
    asm_inline(
        "int $0x40"
        :"=a"(val)
        :"a"(30), "b"(3), "c"(table)
        :"memory"
    );
    return val;
}

/*=================== Function 37 - work with mouse. ===================*/

enum KSYS_MOUSE_POS{
    KSYS_MOUSE_SCREEN_POS = 0,
    KSYS_MOUSE_WINDOW_POS = 1
};

static inline
ksys_pos_t _ksys_get_mouse_pos(int origin){
    ksys_pos_t pos;
    asm_inline(
        "int $0x40 \n\t"
        "rol $16, %%eax"
        :"=a"(pos)
        :"a"(37),"b"(origin)
    );
    return pos;
}

static inline
uint32_t _ksys_get_mouse_buttons(void){ // subfunction 2 - states of the mouse buttons
    uint32_t val;
    asm_inline(
        "int $0x40"
        :"=a"(val)
        :"a"(37),"b"(2)
    );
    return val;
}

static inline
uint32_t _ksys_get_mouse_eventstate(void){   // subfunction 3 - states and events of the mouse buttons 
    uint32_t val;
    asm_inline(
        "int $0x40"
        :"=a"(val)
        :"a"(37),"b"(3)
    );
    return val;
}

static inline
uint32_t _ksys_load_cursor(void *path, uint32_t flags) { // subfunction 4 - load cursor
    uint32_t val;
    asm_inline(
        "int $0x40"
        :"=a"(val)
        :"a"(37), "b"(4), "c"(path), "d"(flags)
        :"memory"
    );
    return val;
} 

static inline 
uint32_t _ksys_set_cursor(uint32_t cursor){ // subfunction 5 - set curso
    uint32_t old;
    asm_inline(
        "int $0x40"
        :"=a"(old)
        :"a"(37), "b"(5), "c"(cursor)
    );
    return old;
}

static inline
int _ksys_delete_cursor(uint32_t cursor){ // subfunction 6 - delete curso
    int ret;
    asm_inline(
        "int $0x40"
        :"=a"(ret)
        :"a"(37), "b"(6), "c"(cursor)
        :"memory"
    );
    return ret;
}

static inline
uint32_t _ksys_get_mouse_wheels(void)  // subfunction 7 - get scroll data
{
    uint32_t val;
    asm_inline(
        "int $0x40"
        :"=a"(val)
        :"a"(37),"b"(7)
    );
    return val;
}

/*=========== Function 40 - set the mask for expected events. ==========*/

enum KSYS_EVENT_MASK{
    KSYS_EVM_REDRAW = 1,
    KSYS_EVM_KEY    = 2,
    KSYS_EVM_BUTTON = 4,
    KSYS_EVM_EXIT   = 8,
    KSYS_EVM_BACKGROUND = 16,
    KSYS_EVM_MOUSE      = 32,
    KSYS_EVM_IPC        = 64,
    KSYS_EVM_STACK      = 128,
    KSYS_EVM_DEBUG      = 256,
    KSYS_EVM_STACK2     = 512,
    KSYS_EVM_MOUSE_FILTER  = 0x80000000,
    KSYS_EVM_CURSOR_FILTER = 0x40000000,
};

static inline
uint32_t _ksys_set_event_mask(uint32_t mask){
    unsigned val;
    asm_inline(
        "int $0x40"
        :"=a"(val)
        :"a"(40), "b"(mask)
    );
    return val;
}

/*====================== Function 38 - draw line. ======================*/

static inline
void _ksys_draw_line(int xs, int ys, int xe, int ye, ksys_color_t color)
{
    asm_inline(
        "int $0x40"
        ::"a"(38), "d"(color),
        "b"((xs << 16) | xe),
        "c"((ys << 16) | ye)
    );
}

/*============= Function 47 - draw a number in the window. =============*/

static inline 
void _ksys_draw_number(int number, int x, int y, int len, ksys_color_t color){
    unsigned fmt;
    fmt = len << 16 | 0x80000000; // no leading zeros + width
    asm_inline(
        "int $0x40"
        ::"a"(47), "b"(fmt), "c"(number), "d"((x << 16) | y), "S"(color)
    );
}

static inline 
void _ksys_draw_number_bg(unsigned number, int x, int y, int len, ksys_color_t color, ksys_color_t bg){
    unsigned fmt;
    fmt = len << 16 | 0x80000000; // no leading zeros + width
    asm_inline(
        "int $0x40"
        ::"a"(47), "b"(fmt), "c"(number), "d"((x << 16) | y), "S"(color), "D"(bg)
    );
}

/*====== Function 48, subfunction 3 - get standard window colors. ======*/

static inline 
void _ksys_get_system_colors(ksys_colors_table_t *color_table)
{
    asm_inline(
       "int $0x40"
        ::"a"(48),"b"(3),"c"(color_table),"d"(40)
    );
}

/*============ Function 48, subfunction 4 - get skin height. ===========*/

static inline
uint32_t _ksys_get_skin_height(){
    unsigned height;
    asm_inline(
        "int $0x40"
        :"=a"(height)
        :"a"(48),"b"(4)
    );
    return height;
}

/*==================== Function 51 - create thread. ====================*/

static inline
int _ksys_create_thread(void* thread_entry, void* stack_top){
    int val;
    asm_inline(
        "int $0x40"
        :"=a"(val)
        :"a"(51), "b"(1), "c"(thread_entry), "d"(stack_top)
        :"memory"
    );
    return val;
}


/*==================== Function 54, subfunction 0 ======================*/
/*============== Get the number of slots in the clipboard. =============*/

enum KSYS_CLIP_ENCODING{
    KSYS_CLIP_UTF8 = 0,
    KSYS_CLIP_CP866 = 1,
    KSYS_CLIP_CP1251 = 2
};

enum KSYS_CLIP_TYPES{
    KSYS_CLIP_TEXT = 0,
    KSYS_CLIP_IMAGE = 1,
    KSYS_CLIP_RAW = 2
};

static inline
int _ksys_clip_num(){
    unsigned val;
    asm_inline(
        "int $0x40"
        :"=a"(val)
        :"a"(54), "b"(0)
    );
    return val;
}
 
/*==================== Function 54, subfunction 1 ======================*/
/*================= Read the data from the clipboard. ==================-*/

static inline
char* _ksys_clip_get(int n) {  // returned buffer must be freed by _ksys_free()
    char* val;
    asm_inline(
        "int $0x40"
        :"=a"(val)
        :"a"(54), "b"(1), "c"(n)
    );
    return val;
}
 
/*==================== Function 54, subfunction 2 ======================*/
/*================= Write the data to the clipboard. ===================*/

static inline
int _ksys_clip_set(int n, char *buffer){
    unsigned val;
    asm_inline(
        "int $0x40"
        :"=a"(val)
        :"a"(54), "b"(2), "c"(n), "d"(buffer)
        :"memory"
    );
    return val;
}
 
/*===================== Function 54, subfunction 3 =====================*/
/*================ Delete the last slot in the clipboard ===============*/

static inline
int _ksys_clip_pop(){
    unsigned val;
    asm_inline (
        "int $0x40"
        :"=a"(val)
        :"a"(54), "b"(3)
    );
    return val;
}
 
/*===================== Function 54, subfunction 4 =====================*/
/*===================== Alarm reset the lock buffer ====================*/

static inline
int _ksys_clip_unlock(){
    unsigned val;
    asm_inline(
        "int $0x40"
        :"=a"(val)
        :"a"(54), "b"(4)
    );
    return val;
}

/*============== Function 63 - work with the debug board. ==============*/

static inline
void _ksys_debug_putc(char c){
    asm_inline("int $0x40"::"a"(63), "b"(1), "c"(c));
}
 
static inline
void _ksys_debug_puts(char *s){
    unsigned i=0;
    while (*(s+i)){
        asm_inline ("int $0x40"::"a"(63), "b"(1), "c"(*(s+i)));
        i++;
    }
}

/*========= Function 66, subfunction 1 - set keyboard input mode. ==============*/

typedef enum KSYS_KEY_INPUT_MODE {
    KSYS_KEY_INPUT_MODE_ASCII = 0,
    KSYS_KEY_INPUT_MODE_SCANC = 1,
} ksys_key_input_mode_t;

static inline
void _ksys_set_key_input_mode(ksys_key_input_mode_t mode){
    asm_inline(
        "int $0x40"
        ::"a"(66),"b"(1),"c"(mode)
    );
}

/*========= Function 67 - change position/sizes of the window. =========*/

static inline 
void _ksys_change_window(int new_x, int new_y, int new_w, int new_h){
    asm_inline(
        "int $0x40"
        ::"a"(67), "b"(new_x), "c"(new_y), "d"(new_w),"S"(new_h)
    );
}

/*======== Function 68, subfunction 12 - allocate memory block. ========*/

static inline
void* _ksys_alloc(size_t size){
    void  *val;
    asm_inline(
        "int $0x40"
        :"=a"(val)
        :"a"(68),"b"(12),"c"(size)
    );
    return val;
}

/*========== Function 68, subfunction 13 - free memory block. ==========*/

static inline
int _ksys_free(void *mem){
    int val;
    asm_inline(
        "int $0x40"
        :"=a"(val)
        :"a"(68),"b"(13),"c"(mem)
    );
    return val;
}

/*============= Function 68, subfunction 16 - load driver. =============*/

static inline
ksys_drv_hand_t _ksys_load_driver(char *driver_name)
{
    ksys_drv_hand_t driver_h;
    asm_inline(
        "int $0x40"
        :"=a"(driver_h)
        :"a"(68), "b"(16), "c"(driver_name)
    );
    return driver_h;
}

/*============ Function 68, subfunction 17 - driver control. ===========*/

static inline
unsigned _ksys_driver_control(ksys_ioctl_t *ioctl)
{
    unsigned status;
    asm_inline(
        "int $0x40"
        :"=a"(status)
        :"a"(68), "b"(17), "c"(ioctl)
        :"memory"
    );
    return status;
}

/*== Function 68, subfunction 18 - subfunction 19 - load DLL (MS COFF) ==*/

static inline
ksys_dll_t* _ksys_dlopen(const char* path){
    ksys_dll_t *table;
    asm_inline(
        "int $0x40"
        :"=a"(table)
        :"a"(68),"b"(19), "c"(path)
        :"memory"
    );
    return table;
}

/* It is not a system call, it serves as an auxiliary tool*/

static inline
void* _ksys_dlsym(ksys_dll_t *table, const char* fun_name){
    unsigned i=0;
    while (1){
        if (!(table+i)->func_name){
            break;
        }else{
            if (!__strcmp(fun_name, (table+i)->func_name)){
                return (table+i)->func_ptr;
            }
        }
        i++;
    }
    return NULL;
}

/* Function 68, subfunction 20 - reallocate memory block.*/

static inline
void* _ksys_realloc(void *mem, size_t size){
    void *val;
    asm_inline(
        "int $0x40"
        :"=a"(val)
        :"a"(68),"b"(20),"c"(size),"d"(mem)
        :"memory"
    );
    return val;
}

/* Function 68, subfunction 21 - load driver by full name. */

static inline
ksys_drv_hand_t _ksys_load_driver_opt(char *driver_path, char *cmd_line)
{
    ksys_drv_hand_t driver_h;
    asm_inline(
        "int $0x40"
        :"=a"(driver_h)
        :"a"(68), "b"(21), "c"(driver_path), "d"(cmd_line)
    );
    return driver_h;
}

/*======== Function 68, subfunction 22 - open named memory area. =======*/

enum KSYS_SHM_MODE{
    KSYS_SHM_OPEN = 0x00,
    KSYS_SHM_OPEN_ALWAYS = 0x04,
    KSYS_SHM_CREATE = 0x08,
    KSYS_SHM_READ = 0x00,
    KSYS_SHM_WRITE = 0x01,
};

static inline
int _ksys_shm_open(char *name, int mode, int size, char **new_shm)
{
    int error;
    asm_inline(
        "int $0x40"
        :"=a"(*new_shm), "=d"(error)
        :"a"(68), "b"(22), "c"(name), "d"(size), "S"(mode)
    );
    return error;
}

/*======= Function 68, subfunction 23 - close named memory area. =======*/

static inline
void _ksys_shm_close(char *shm_name)
{
    asm_inline(
        "int $0x40":
        :"a"(68), "b"(23), "c"(shm_name)
    );
}

/*====== Function 68, subfunction 26 - release memory pages ============*/

static inline
int* _ksys_unmap(void *base, size_t offset, size_t size){
    int  *val;
    asm_inline(
        "int $0x40"
        :"=a"(val)
        :"a"(68),"b"(26),"c"(base),"d"(offset),"S"(size)
    );
    return val;
}

/*========== Function 68, subfunction 27 - load file ===================*/

static inline 
ksys_ufile_t _ksys_load_file(const char *path)
{
    ksys_ufile_t uf;
    asm_inline(
        "int $0x40"
        :"=a"(uf.data), "=d"(uf.size)
        :"a"(68), "b"(27),"c"(path)
        :"memory"
    );
    return uf;
}

/*==== Function 68, subfunction 28 - load file, specifying the encoding ===*/

static inline 
ksys_ufile_t _ksys_load_file_enc(const char *path, unsigned file_encoding)
{
    ksys_ufile_t uf;
    asm_inline(
        "int $0x40"
        :"=a"(uf.data), "=d"(uf.size)
        :"a"(68), "b"(28),"c"(path), "d"(file_encoding)
        :"memory"
    );
    return uf;
}

/*==== Function 70 - work with file system with long names support. ====*/

static inline
int _ksys_work_files(const ksys70_t *k)
{
    int status;
    asm_inline(
        "int $0x40"
        :"=a"(status)
        :"a"(70), "b"(k)
        :"memory"
    );
    return status;
}

/*====== Function 70, subfunction 0 - read file with long names support. ======*/

static inline
int _ksys_file_read_file(const char *name, unsigned long long offset, unsigned size, void *buf, unsigned *bytes_read)
{
    ksys70_t k;
    k.p00 = 0;
    k.p04 = offset;
    k.p12 = size;
    k.buf16 = buf;
    k.p20 = 0;
    k.p21 = name;
    int status;
    asm_inline(
        "int $0x40"
        :"=a"(status), "=b"(*bytes_read)
        :"a"(70), "b"(&k)
        :"memory"
    );
    return status;
}

/*===================== Function 70, subfunction 2 =====================*/
/*============ Create/rewrite file with long names support. ============*/

static inline 
int _ksys_file_create(const char* name){
    ksys70_t k;
    k.p00   = 2;
    k.p04dw = 0;
    k.p08dw = 0;
    k.p12   = 0;
    k.p21   = name;
    return _ksys_work_files(&k);
}

/*===================== Function 70, subfunction 3 =====================*/
/*=========== Write to existing file with long names support. ==========*/

static inline
int _ksys_file_write_file(const char *name, unsigned long long offset, unsigned size, const void *buf, unsigned *bytes_written)
{
    ksys70_t k;
    k.p00 = 3;
    k.p04 = offset;
    k.p12 = size;
    k.cbuf16 = buf;
    k.p20 = 0;
    k.p21 = name;
    int status;
    asm_inline(
        "int $0x40"
        :"=a"(status), "=b"(*bytes_written)
        :"a"(70), "b"(&k)
        :"memory"
    );
    return status;
}

/*========== Function 70, subfunction 5 - get information on file/folder. =====*/

static inline
int _ksys_file_get_info(const char *name, ksys_bdfe_t *bdfe)
{
    ksys70_t k;
    k.p00   = 5;
    k.p04dw = 0;
    k.p08dw = 0;
    k.p12   = 0;
    k.bdfe  = bdfe;
    k.p20   = 0;
    k.p21   = name;
    return _ksys_work_files(&k);
}

/*=========== Function 70, subfunction 7 - start application. ===========*/

static inline
int _ksys_exec(char *app_name, char *args)
{
    ksys70_t file_opt;
    file_opt.p00 = 7;
    file_opt.p04dw = 0;
    file_opt.p08dw = (unsigned)args;

    file_opt.p12 = 0;
    file_opt.p16 = 0;
    file_opt.p20 = 0;

    file_opt.p21 = app_name;
    return _ksys_work_files(&file_opt);
}

/*========== Function 70, subfunction 8 - delete file/folder. ==========*/

static inline
int _ksys_file_delete(const char *name)
{
    ksys70_t k;
    k.p00 = 8;
    k.p20 = 0;
    k.p21 = name;
    return _ksys_work_files(&k);
}

/*============= Function 70, subfunction 9 - create folder. ============*/

static inline
int _ksys_mkdir(const char *path)
{
    ksys70_t dir_opt;
    dir_opt.p00 = 9;
    dir_opt.p21 = path;
    return _ksys_work_files(&dir_opt);
}

/*============= Function 70, subfunction 10 - rename/move. =============*/

static inline
int _ksys_file_rename(const char *name, const char *new_name)
{
    ksys70_t k;
    k.p00 = 10;
    k.new_name = new_name;
    k.p20 = 0;
    k.p21 = name;
    return _ksys_work_files(&k);
}


/* ######### Old names of functions and structures. Do not use again! ##########*/

#define _ksys_get_event    _ksys_wait_event

#endif // _KSYS_H_
