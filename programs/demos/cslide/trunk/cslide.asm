;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;                                          ;
;   Color Slider Control Demonstration     ;
;                                          ;
;   Compile with FASM for Kolibri          ;
;                                          ;
;   Author: Jason Delozier                 ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

use32

               org    0x0

               db     'MENUET01'              ; 8 byte id
               dd     0x01                    ; header version
               dd     START                   ; start of code
               dd     I_END                   ; size of image
               dd     0x1000                  ; memory for app
               dd     0x1000                  ; esp
               dd     0x0 , 0x0               ; I_Param , I_Icon

include 'lang.inc'
include '..\..\..\macros.inc'
include '..\..\..\KOSfuncs.inc'
START:                          ; start of execution

    call draw_window            ; at first, draw the window

still:
    call mouse_info

    mcall SF_WAIT_EVENT_TIMEOUT, 2

    cmp  eax,1                  ; redraw request ?
    je   red
    cmp  eax,2                  ; key in buffer ?
    je   key
    cmp  eax,3                  ; button in buffer ?
    je   button

    jmp  still

  red:                          ; redraw
    call draw_window
    jmp  still

  key:                          ; key
    mcall SF_GET_KEY            ; just read it and ignore
    jmp  still

  button:                       ; button
    mcall SF_GET_BUTTON         ; get id
    
    shr  eax,8

    cmp  eax,1                   ; button id=1 ?
    jne  noclose
    mcall SF_TERMINATE_PROCESS   ; close this program
  noclose:


  nofind:
    jmp still


;   *********************************************
;   *******  WINDOW DEFINITIONS AND DRAW ********
;   *********************************************


draw_window:
    mcall SF_REDRAW, SSF_BEGIN_DRAW
    mcall SF_CREATE_WINDOW, <100,200>, <100,200>, 0x14ffffff, , title
    mcall SF_REDRAW, SSF_END_DRAW
	
    call draw_slider_info

    xor  ecx,ecx
Draw_Controls_Loop:
    mov  ebp, [App_Controls+ecx]    ;get controls data location
    or   ebp,ebp
    jz   Draw_Controls_Done
    call dword [App_Controls+ecx+4] ;call controls draw function
    add  ecx, 12
    jmp  Draw_Controls_Loop
Draw_Controls_Done:

    ret


;***********************************************
;* Mouse Stuff
;***********************************************
mousey dw 0
mousex dw 0
mouseb dd 0

mouse_info:
   mcall SF_MOUSE_GET, SSF_WINDOW_POSITION
   mov ecx, eax           ;
   push ecx               ;
   mcall SF_MOUSE_GET, SSF_BUTTON
   cmp [mouseb], eax      ;compare old mouse states to new states
   jne redraw_mouse_info  ;
   cmp [mousey], cx       ;
   jne redraw_mouse_info  ;
   shr ecx, 16            ;
   cmp [mousex], cx       ;
   jne redraw_mouse_info  ;
   pop ecx                ;
ret                       ;return if no change in states


redraw_mouse_info:
   pop ecx
   mov [mouseb], eax      ;save new mouse states
   mov dword [mousey], ecx

   xor ecx, ecx
Check_Mouse_Over_Controls_Loop:
   mov ebp, [App_Controls+ecx]
   or  ebp,ebp
   jz Check_Mouse_Over_Controls_Loop_done
   movzx eax,word [ebp+2]
   cmp    ax, [mousex]
   ja  mouse_not_on_control
   movzx eax,word [ebp+6]
   cmp    ax, [mousey]
   ja  mouse_not_on_control
   movzx eax,word [ebp]
   add    ax, [ebp+2]
   cmp    ax, [mousex]
   jb  mouse_not_on_control
   movzx eax,word [ebp+4]
   add    ax, [ebp+6]
   cmp    ax, [mousey]
   jb  mouse_not_on_control
   call dword [App_Controls+ecx+8]
mouse_not_on_control:
   add ecx, 12
   jmp Check_Mouse_Over_Controls_Loop
Check_Mouse_Over_Controls_Loop_done:

ret




;***********************************************


draw_slider_info:
;Repaint value background
   mcall SF_DRAW_RECT, 144*65536+36, 72*65536+9, 0x00ffffff 
;Draw Color Box
   xor edx, edx
   movzx ecx,word [slider_1+12]
   mov dh, cl
   movzx ecx,word [slider_2+12]
   mov dl, cl
   shl edx, 8
   movzx ecx,word [slider_3+12]
   mov dl,cl
   mov ebx, 0x00860035
   mov ecx, 0x00590040
   mov eax, SF_DRAW_RECT
   mcall 
;draw current value of slider
   mov ecx, edx
   mov eax, SF_DRAW_NUMBER
   mov ebx, 0x00060100
   mov esi, 0
   mov edx, 144*65536+72
   mcall
ret


;**************************************
;*
;*   App Controls
;*
;**************************************

App_Controls:
     dd slider_1 , draw_slider, slider_mouse_over   ;
     dd slider_2 , draw_slider, slider_mouse_over   ;
     dd slider_3 , draw_slider, slider_mouse_over   ;
     dd 0 , 0          ; denotes last control do not delete

;**************************************
;*
;*   Slider data
;*
;**************************************

slider_1:
   dw  25  ;width         +0
   dw  10  ;x             +2
   dw  150 ;height        +4
   dw  30  ;y             +6
   dw  0   ;min           +8
   dw  255 ;max           +10
   dw  128 ;current       +12
   dw  1   ;small change  +14
   dw  5   ;big change    +16

slider_2:
   dw  25  ;width         +0
   dw  55  ;x             +2
   dw  150 ;height        +4
   dw  30  ;y             +6
   dw  0   ;min           +8
   dw  255 ;max           +10
   dw  128  ;current       +12
   dw  1   ;small change  +14
   dw  5   ;big change    +16

slider_3:
   dw  25  ;width         +0
   dw  100 ;x             +2
   dw  150 ;height        +4
   dw  30  ;y             +6
   dw  0   ;min           +8
   dw  255 ;max           +10
   dw  128 ;current       +12
   dw  1   ;small change  +14
   dw  5   ;big change    +16

;**************************************
;*
;*   Slider Code
;*
;**************************************

box_h dw 10  ;static slider box height

draw_slider:
   push eax ebx ecx edx
;Draw slider background
   mov   eax, SF_DRAW_RECT ;slider background
   mov   ebx, [ebp]      ;x start/width
   mov   ecx, [ebp+4]    ;y start/height
   mov   edx, 0x00EBEBEB ;color
   mcall                 ;draw bar
;Draw line for slide rail
   mov   eax, 38         ;draw vertical slide line
   movzx ebx,word [ebp]  ;x
   shr   ebx, 1          ;
   add    bx,word [ebp+2];
   push   bx             ;
   shl   ebx, 16         ;
   pop    bx             ;
   mov   ecx, [ebp+4]    ;y start / height
   add   ecx, 0x000A0000 ;
   add   ecx, [ebp+6]    ;y start
   sub   ecx, 10         ;
   mov   edx, 0x00       ;color
   mcall              ;
;Draw slider box
   movzx eax,word [ebp+4]  ;height
   sub   eax, 20           ;
   movzx ebx,word [ebp+10] ;max value
   sub    bx,word [ebp+8]  ;min value
   movzx ecx,word [ebp+12] ;
   call  slider_fpu_calc   ;EAX = ((EAX/EBX)*ECX)
   mov   ebx, [ebp]        ;x start / width
   movzx ecx,word [ebp+4]  ;height
   add    cx, [ebp+6]      ;y
   sub   ecx, 10           ;
   movzx edx, [box_h]      ;
   shr   edx, 1            ;
   sub   ecx, edx          ;
   sub   ecx, eax          ;*slide box y position
   shl   ecx, 16           ;
   mov    cx, [box_h]      ;height
   mov   eax, SF_DRAW_RECT ;draw bar sys function
   mov   edx, 0x00         ;color
   mcall               ;draw slider box
   pop edx ecx ebx eax
ret

slider_mouse_over:
   push eax ebx ecx edx
   cmp [mouseb], 1
   jne slider_mouse_over_done
   movzx eax,word [ebp+4]
   add    ax, [ebp+6]
   sub   eax, 10
   cmp [mousey], ax
   ja slider_mouse_min
   movzx eax,word [ebp+6]
   add   eax, 10
   cmp [mousey], ax
   jb slider_mouse_max
;determine new current value
   movzx eax,word  [ebp+10] ;slider max value
   sub    ax,word  [ebp+8]  ;slider min value
   movzx ebx,word [ebp+4]   ;slider height
   sub   ebx,20             ;rail size
   movzx ecx,word [mousey]  ;current mouse y pixel
   sub   cx,word  [ebp+6]   ;minus y start of slider
   sub   ecx, 10            ;minus pixels to top of rail
   call  slider_fpu_calc    ;EAX = ((EAX/EBX)*ECX)
   movzx ebx,word [ebp+10]  ;slider max
   sub   ebx,eax            ;*current calculated position
   jmp   slider_mouse_change;
slider_mouse_max:           ;
   movzx ebx,word [ebp+10]  ;get maximum value
   jmp slider_mouse_change  ;
slider_mouse_min:           ;
   movzx ebx,word [ebp+8]   ;get minimum value
slider_mouse_change:        ;
   mov   [ebp+12],bx        ;new slider current position
   call draw_slider         ;
   call draw_slider_info    ;
slider_mouse_over_done:     ;
   pop edx ecx ebx eax
ret


temp  dd 0   ;temp varibles used in fpu computations
temp2 dd 0
temp3 dd 0

slider_fpu_calc:
   mov   [temp],  eax
   mov   [temp2], ebx
   mov   [temp3], ecx
   finit                   ;initilize FPU
   fld   dword  [temp]     ;load value
   fdiv  dword  [temp2]    ;divide
   fmul  dword  [temp3]    ;multiply
   fst   dword  [temp]     ;store computed value
   mov   eax,   [temp]
ret

;**************************************************
;* End Slider Code
;**************************************************

; DATA AREA
title     db  'Color Slider',0
I_END: