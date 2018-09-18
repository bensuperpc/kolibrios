;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Hot Angles Config ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Compile with FASM
; Version 0.1: Sep 18, 2018

; Copyright (c) 2018, Efremenkov Sergey aka TheOnlyMirage
; All rights reserved.
; Redistribution and use in source and binary forms, with or without modification,
; are permitted provided that the following conditions are met:
;    * Redistributions of source code must retain the above copyright notice, this
;    list of conditions and the following disclaimer.
;    * Redistributions in binary form must reproduce the above copyright  notice,
;    this list of conditions and the following disclaimer in the documentation and/or
;    other materials provided with the distribution.
;    * Neither the name of the <organization> nor the names of its contributors may
;    be used to endorse or promote products derived from this software without
;    specific prior written permission.

; THE SOFTWARE IS PROVIDED �AS IS�, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
; INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
; PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
; HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
; OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
; SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
; --------------------------------------------------------------------------------------

;P.S. ��� �����, ��������� ����������� � ��������� (��. ���� ������)
format binary as ""     ; Binary file format without extension
use32                   ; Tell compiler to use 32 bit instructions
org 0                   ; the base address of code, always 0x0

; ��������� ������������ ����� ������� ��
db 'MENUET01'
dd 1
dd START
dd I_END
dd MEM
dd STACKTOP
dd 0, 0

; ������ ������� ����
include '../../../macros.inc'
include 'editbox.inc'
use_edit_box
include 'check.inc'      ;�������� ���� check.inc
version_ch1              ;����������� ������ ��� ��������� new ������ check_box1
use_check_box            ;��������� �������, ������ ��������� ��� ��������� ��� �����

Otstup = 30

START:
    call copyKill        ;���������� ������ �� ������� ������ ����� ����������
    mcall  68, 11        ;������������� ����

    mov  eax,48          ;�������� ��������� �����
    mov  ebx,3
    mov  ecx, sc
    mov  edx, sizeof.system_colors
    mcall

    mov  eax,40          ;���������� ����� ��� ��������� �������
    mov  ebx,0x27        ;������� ����� ����������� ������ �� ��������� � �����������, ������ ������, ����������� �����, ������� �� ���� (���-�� ��������� - ������� �� ������ ���� ��� �����������; ������������ ��� ���������)
    mcall

    call  loadConfig     ;��������� ������������ �� �����, ���� ��� - �� ���������� ��������� �� ���������
    call  draw_window    ;���������� ���� ����������

; ��������� ����� �������
event_wait:
    mov     eax, 10      ; function 10 : wait until event
    mcall                ; event type is returned in eax

    cmp     eax, 1       ; Event redraw request ?
    je      red          ; Expl.: there has been activity on screen and
                         ; parts of the applications has to be redrawn.

    cmp     eax, 2       ; Event key in buffer ?
    je      key          ; Expl.: User has pressed a key while the
                         ; app is at the top of the window stack.

    cmp     eax, 3       ; Event button in buffer ?
    je      button       ; Expl.: User has pressed one of the
                         ; applications buttons.

    mouse_edit_boxes editboxes,editboxes_end
    mouse_check_boxes check_boxes,check_boxes_end  ;�������� ��� �����
    jmp     event_wait


red:                     ;��������� ������� ����������� ����
   call    draw_window
   jmp     event_wait

key:                                    ; Keypress event handler
        mov     eax, 2                  ; The key is returned in ah. The key must be
        mcall                           ; read and cleared from the system queue.
        key_edit_boxes editboxes,editboxes_end
        jmp     event_wait              ; Just read the key, ignore it and jump to event_wait.

button:                                 ; Buttonpress event handler
    mov     eax,17                  ; The button number defined in window_draw
    mcall                           ; is returned to ah.

    cmp     ah,1                    ; button id=1 ?
    jne     noclose
    mov     eax,-1                  ; Function -1 : close this program
    mcall
noclose:
    cmp  ah, 2         ;������ "���������"
    jne  butend
    call applyButton   ;����� ������ ����������� ������� ������
butend:
    jmp  event_wait    ;this is for ignored events, useful at development



;��� ����������������� �����
fileName: db 'SETTINGS/HOTANGLES.CFG', 0

;��������� ��� ��������/���������� �����
createFile:
  .func     dd 2    ;����� ����������
  .re       dd 0, 0 ;���������������
  .size     dd 0    ;������� ���� ������
  .data_adr dd 0    ;��������� �� ������
  .path     db 0
  .path_adr dd fileName    ;����

;������ ���������� �� ��������� � � ������
defaultDataForConfig db 121,13,10,'/rd/1/HACONFIG',13,10,'/rd/1/HACONFIG',13,10,'/rd/1/HACONFIG',13,10,'/rd/1/HACONFIG',13,10
defaultDataSize = 48+19


copyParam:  ;�������� ��������(������� �������) � ����� (��. ������)
   push esi ebx ecx
   mov ecx, dword[ebx] ;������ ������ ������
.copy:  ;�������� �������
   cmp ecx, 0
   je copyParam.copyEnd
   mov bl, byte[esi]
   mov byte[eax], bl
   dec ecx
   inc eax
   inc esi
   jmp copyParam.copy
.copyEnd:
   pop ecx ebx esi
   ret


addPerenos:  ;��������� ������� ������ � �����
   mov byte[eax], 13
   inc eax
   mov byte[eax], 10
   inc eax
   ret

;���������� ������ "���������"
applyButton:
    push eax ebx ecx
        ;mov dword[createFile.data_adr], defaultDataForConfig
        ;mov dword[createFile.size], defaultDataSize

        ;�������� ������ ����� ������������ � ������
        mov ecx, 5*2+1 ;1 ���� �� ��������� ���������� � 5 ��� ��������� ����� (������� 13, 10)
        add ecx, dword[editLU.size] ;dataBuffer.size1]      ;���� ������ ������ ������ � ������
        add ecx, dword[editLD.size] ;dataBuffer.size2]
        add ecx, dword[editRU.size] ;dataBuffer.size3]
        add ecx, dword[editRD.size] ;dataBuffer.size4]
        ;������ ������ ����� ������������ � ecx
        mov dword[createFile.size], ecx ;������� ������ ���� � ��������� ��������/���������� �����
        mcall 68, 12, ecx ;�������� ���� ������ ��� ����� �����������
        mov dword[createFile.data_adr], eax ;��������� ����� ������ � ���������

        ;�������� ����� �����������:
        mov byte[eax], 110; 'n' ;�� ��������� ���� ����
        cmp word[ch1.flags], ch_flag_en   ;��������� checkBox ��� ���?
        jne applyButton.no
        mov byte[eax], 121 ;����� ����� 'y'
.no:
        inc eax
        call addPerenos

        push esi ebx
        call UpdateEditsSizes ;�������� ������� ������ � edit'��

        mov esi, dataBuffer.1 ;editLU.text
        mov ebx, dataBuffer.size1 ;editLU.size
        call copyParam
        call addPerenos     ;��������� ������� ������

        mov esi, dataBuffer.2 ;editLD.text
        mov ebx, dataBuffer.size2 ;editLD.size
        call copyParam
        call addPerenos     ;��������� ������� ������

        mov esi, dataBuffer.3
        mov ebx, dataBuffer.size3
        call copyParam
        call addPerenos     ;��������� ������� ������

        mov esi, dataBuffer.4
        mov ebx, dataBuffer.size4
        call copyParam
        call addPerenos     ;��������� ������� ������

        pop ebx esi


    mov ebx, createFile
    mcall 70
        cmp eax, 0
        je .end
        ;����� ����� ����� ������� ���� � ������� !!!
.end:
        mcall 68, 13, dword[createFile.data_adr]  ;����������� ������ ��� �����
        call restartHotAnglesApp ;������������� @HOTANGLES
        pop ecx ebx eax
    ret






compareBytes2:
   push edi esi ecx ebx
   xor eax, eax
   mov ecx, 10  ;max ������ ����� 11
@@:
   mov bl, byte[edi]
   cmp bl, byte[esi]
   jne compareBytes.no
   inc edi
   inc esi
   cmp ecx, 0
   je @f
   dec ecx
   jmp @b
.no:
   mov eax, 1
@@:
   pop ebx ecx esi edi
   ret


restartHotAnglesApp:   ;!!!
   ;������� ��� ����� ���������� �� ����� � ��������� ��
   push eax ebx ecx esi edi
   ;���������, ������� ������� �� ��������:
   mov esi, startAppData.file_name       ;������ ������ - ������� ��� ����������
   mov edi, buf            ;������ ������ - ��� �������� �����
   add edi, 10

   mov ecx, 1
@@:
   mcall 9, buf, ecx              ;�������� ������ ���������� �����
   mov dword[slotMax], eax

   call compareBytes2 ;���������� ���
   cmp eax, 0
   jne restartHotAnglesApp.next   ;���� �� �������, �� ��������� �� ��������� ����

   ;push eax ebx ecx
   mcall 18, 2, ecx               ;����� ��������� ����
   ;pop ecx ebx eax

.next:
   inc  ecx
   cmp ecx, dword[slotMax]
   ja @f
   jmp @b
@@:
   pop edi esi ecx ebx eax

   ;��������� ���������� ������
   push eax ebx
   mcall 70, startAppData
   cmp eax, 0
   ja @f       ;���� eax>0, �� ������� ��������
   ;!!! ����� ��� �������� ������
@@:
   pop ebx eax
   ret

startAppData:
    .subfunction dd   7
    .flags       dd   0
    .param       dd   0
    .rezerv      dd   0, 0
    .full_name   db   '/rd/1/'
    .file_name   db   '@HOTANGLES', 0
    ;db 0
    ;.name: dd HotAnglesAppName
;HotAnglesAppName db '@HOTANGLES', 0


UpdateEditsSizes:
   push eax
   mov eax, dword[editLU.size]
   mov dword[dataBuffer.size1], eax

   mov eax, dword[editLD.size]
   mov dword[dataBuffer.size2], eax

   mov eax, dword[editRU.size]
   mov dword[dataBuffer.size3], eax

   mov eax, dword[editRD.size]
   mov dword[dataBuffer.size4], eax

   pop eax
   ret

fileAdr dd 0

loadConfig:
   push eax ebx ecx edx
   mcall 68, 27, fileName   ;��������� ���������������� ���� � ���
   mov dword[fileAdr], eax  ;��������� ����� ����� ������ �����
   cmp eax, 0
   je loadConfig.default       ;���� ����� ������������ ���, �� ������ ��������� �� ���������
   ;����� ������ ��������� � ���, ������ � edx
   cmp edx, 0               ;���� ���� ����, � ��� ��� ������, �� ������ ��������� �� ���������
   je loadConfig.default
.start:
   add edx, eax             ;����� � edx - ����� ����� ������.������

.chStart:
   cmp byte[eax], 121 ;'y'  ;���������� checkBox - �������� ���������� ����������
   jne loadConfig.no
.yes:
   mov word[ch1.flags], ch_flag_en
   jmp loadConfig.chEnd
.no:
   mov word[ch1.flags], 0
.chEnd:
   ;jmp loadConfig.end   ;������� ��� �������� ������� � ������ ������

   ;�������� ���������� ����� � �����
   push edi esi ecx ebx eax
   ;�������������� ��������� �� ������ ������ ������ � � �������
   mov edi, dataBuffer.1     ;dword[dataBuffer.1]
   mov esi, dataBuffer.size1 ;dword[dataBuffer.size1]
   ;ecx - ������� ���� � ������
;   mov ecx, 0 ;xor ecx, ecx
.block:
   xor ecx, ecx ;�������� �������
   inc eax
   cmp byte[eax], 10         ;���� ��������� ��� ������� 10 ��� 13, �� ���������� �������
   je loadConfig.block ;propusk     ;�� ������� ��������� �� ���
   cmp byte[eax], 13
   je loadConfig.block ;propusk

   ; ������ ������� �� �������� ������ � �������� ������� - �������� ���������� ������ � �����
   mov bl, byte[eax]
   mov byte[edi], bl
   inc edi
   inc eax
   inc ecx ;����������� ������� �������� � ������

   ;��� �� ����� ���� ������: ���� �� �������� ��������� 10, 13, 0 ��� file end
.while:     ;� �������� �� � �����
   cmp eax, edx           ;��������� ���������� �� ����, � ����� ������ �� ���� !!!
   ja loadConfig.ura
   cmp byte[eax], 10
   je loadConfig.ura
   cmp byte[eax], 0
   je loadConfig.ura
   cmp byte[eax], 13
   je loadConfig.ura

   mov bl, byte[eax]
   mov byte[edi], bl
   inc edi
   inc eax
   inc ecx
   cmp ecx, 511
   jae loadConfig.ura
   jmp loadConfig.while
.ura:
   mov byte[edi], 0      ;����� ����������� 0 � ������
   mov dword[esi], ecx   ;��������� ������ ������
   add esi, 4            ;��������� �� ������ ���������� ������
   sub edi, ecx          ;��������� �� ��������� �����
   add edi, 512
;   mov ecx, 0 ;xor ecx, ecx ;�������� ������ ������

   cmp edi, dataBuffer.size1 ;���� ��� ��� ��������� ����� (� �� ����� �� ������ �������� �������), �� �������
   jb loadConfig.block

   pop eax ebx ecx esi edi

   jmp loadConfig.end
.default:
   mov eax, defaultDataForConfig
   mov edx, defaultDataSize
   jmp loadConfig.start
.end:
   mov ecx, dword[fileAdr]    ;���� ���� ��� ��������, ��
   cmp ecx, 0
   je @f
   mcall 68, 13, ecx  ;��������� ���� �� ������
@@:
   call updateParams ;��������� ��������� ������� ������
   pop edx ecx ebx eax
   ret


updateParams: ;max, size, pos
   push eax
   mov eax, dword[dataBuffer.size1]
   mov dword[editLU.size], eax
   mov dword[editLU.pos], eax

   mov eax, dword[dataBuffer.size2]
   mov dword[editLD.size], eax
   mov dword[editLD.pos], eax

   mov eax, dword[dataBuffer.size3]
   mov dword[editRU.size], eax
   mov dword[editRU.pos], eax

   mov eax, dword[dataBuffer.size4]
   mov dword[editRD.size], eax
   mov dword[editRD.pos], eax
   pop eax
   ret



selfName db 'HACONFIG', 0 ;'@HOTANGLES',0
selfNameSize = 8;9 ;�� 11 byte
;
compareBytes:
   push edi esi ecx ebx
   mov eax, 0 ;xor eax, eax
   mov ecx, selfNameSize ;11    ;max ������ ����� 11
@@:
   mov bl, byte[edi]
   cmp bl, byte[esi]
   jne compareBytes.no
   inc edi
   inc esi
   cmp ecx, 0
   je @f
   dec ecx
   jmp @b
.no:
   mov eax, 1
@@:
   pop ebx ecx esi edi
   ret
;
slotMax dd 0
selfPID dd 0
buf db 1024 dup(0)
copyKill:
   push eax ebx ecx esi edi

   ;������ ������ ���� ����������
   mcall 9, buf, -1
   mov eax, dword[buf+30]
   mov dword[selfPID], eax

   ;���������, ������� ������� �� ��������:
   mov esi, selfName       ;������ ������ - ��� �������� ����������
   mov edi, buf            ;������ ������ - ��� �������� �����
   add edi, 10

   mov ecx, 1
@@:
   mcall 9, buf, ecx
   mov dword[slotMax], eax

   ;���� ��� �� ����, �� ���������� ��������
   mov eax, dword[buf+30]
   cmp eax, dword[selfPID]
   je copyKill.propusk

   call compareBytes   ;���������� 11 ���� �����, ��������� � eax

   cmp eax, 0
   je copyKill.selfKill

.propusk:
   inc ecx
   cmp ecx, dword[slotMax]
   ja @f
   jmp @b

.selfKill:
   pop edi esi ecx ebx eax
   mcall -1
   ret
@@:
   pop edi esi ecx ebx eax
   ret

 WindowsWidth  = 450
 WindowsHeight = 195
coord:      ;���������� ���� ����������
 .y dw 0
 .x dw 0

draw_window:
        ;������� ������, ���� ����
        mov edx, 0x80000002
        mcall 8

        mov     eax, 12                 ; function 12: tell os about windowdraw
        mov     ebx, 1                  ; 1, start of draw
        mcall

        ;��������� ���� �� ������ ������ ��� �����:
        mcall 14 ;������� ������� ������
        mov dword[coord], eax       ;������� ������� ������ � ����������
        shr word[coord.x], 1        ;������ ������� �� ��������
        shr word[coord.y], 1

        ;��������� ���������� ����, ����� ���� ���� �� �������� ������
        sub word[coord.x], WindowsWidth/2
        sub word[coord.y], WindowsHeight/2

        mov     eax, 0                  ; function 0 : define and draw window
        mov     ebx, 0 ;* 65536 + WindowsWidth   ; [x start] *65536 + [x size]
        mov bx, word[coord.x]
        shl  ebx, 16
        mov bx, WindowsWidth

        mov ecx, 0
        mov     cx, word[coord.y] ;* 65536 + WindowsHeight  ; [y start] *65536 + [y size]
        shl ecx, 16
        mov cx, WindowsHeight

        mov     edx, 0x14ffffff         ; color of work area RRGGBB
                                        ; 0x02000000 = window type 4 (fixed size, skinned window)
        mov     esi, 0x808899ff         ; color of grab bar  RRGGBB
                                        ; 0x80000000 = color glide
        mov     edi, title
        mcall

        draw_check_boxes check_boxes,check_boxes_end ;��������� ���������
        draw_edit_boxes editboxes,editboxes_end      ;��������� edit box'��

        ;������ ������
        mov ebx, (WindowsWidth-80-Otstup)*65536 + 80 ;���������� X, ������ ������
        mov ecx, 160*65536 + 20   ;���������� Y, ������ ������
        mov edx, 0x00000002 ;2    ;����� ������
        mov esi, 0x00AABBCC       ;���� ������
        mcall 8
        ;������ ����� ������
        mov     ebx, (WindowsWidth-80-Otstup+25) * 65536 + 166    ;���������� ������ (x, y)
        mov     ecx, 0xFFFFFF     ;����� ���� ������
        mov     edx, buttonText
        mov     esi, 5            ;������ ������ � ��������
                mcall 4

        call draw_super_text

        LineLength = 62 ;42
        mov     ebx, 40 * 65536 + 36    ; draw info text with function 4 (x, y)
        mov     ecx, 0x224466
        mov     edx, text
        mov     esi, LineLength
        mov     eax, 4

  .newline:                             ; text from the DATA AREA
        mcall
        add     ebx, 12
        add     edx, LineLength
        cmp     byte[edx], 0
        jne     .newline

        mov     eax, 12                 ; function 12:tell os about windowdraw
        mov     ebx, 2                  ; 2, end of draw
        mcall

        ret


text    db  "This is a Hot Angles Configurator.                            "
        db  "Set the parameters bellow for yourself or disable this option."
        db  "                                                              ", 0
 hed:
title   db  "Hot Angles Configurator", 0
buttonText db "Apply", 0

align 4
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
;DATA ������  ��� check_box1 - ���������� ��������� �����
;������ ������ ��� �����:
;   ���������� �, ���������� �, ��������� �� ������ ������, ����� ������
;��������� ������ �������: �������/���
check_boxes:
   ch1 check_box1 30,160,ch_text.1,ch_text.e1-ch_text.1,ch_flag_en
check_boxes_end:

ch_text:        ;�������������� ����� ��� ��� ������
.1 db 'Activate "Hot Angles"'
.e1:

superText:
  .lu db 'Left up angle command:', 0
  .ld db 'Left down angle command:', 0
  .ru db "Right up angle command:", 0
  .rd db 'Right down angle command:', 0

draw_super_text:
   push eax ebx ecx edx edi esi
   mov     ebx, Otstup * 65536 + 74    ; draw info text with function 4 (x, y)
   mov     ecx, 0x224466
   mov eax, superText.lu
   mov     edx, eax
   mov     esi, 22
   mcall 4

   mov     ebx, Otstup * 65536 + 109
   mov     edx, superText.ld
   mov     esi, 24
   mcall 4

   mov     ebx, 240 * 65536 + 74
   mov     edx, superText.ru
   mov     esi, 23
   mcall 4

   mov     ebx, 240 * 65536 + 109
   mov     edx, superText.rd
   mov     esi, 25
   mcall 4

   pop esi edi edx ecx ebx eax
   ret


edMaxSize = 510
edMax = 0 ;max, size, pos

editboxes:
editLU edit_box 180,Otstup,85,0xffffff,0x6a9480,0,0xAABBCC,0,edMaxSize,dataBuffer.1,ed_focus,edMax,edMax
editLD edit_box 180,Otstup,120,0xffffff,0x6a9480,0,0xAABBCC,0,edMaxSize,dataBuffer.2,ed_focus,edMax,edMax
editRU edit_box 180,240,85,0xffffff,0x6a9480,0,0xAABBCC,0,edMaxSize,dataBuffer.3,ed_focus,edMax,edMax
editRD edit_box 180,240,120,0xffffff,0x6a9480,0,0xAABBCC,0,edMaxSize,dataBuffer.4,ed_focus,edMax,edMax
editboxes_end:

data_of_code dd 0
mouse_flag dd 0x0

dataBuffer:
.1: rb 512
.2: rb 512
.3: rb 512
.4: rb 512
.size1: rd 1
.size2: rd 1
.size3: rd 1
.size4: rd 1
;255+255+2
;��� �������� ����� ���������� ��� ���� ��� �� �� ������������ ���������� �����, � ����� ������ 0
dataBufferEnd:

sc  system_colors

I_END:
        rb 4096
align 16
STACKTOP:

MEM: