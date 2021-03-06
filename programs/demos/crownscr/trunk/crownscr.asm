;
;    Crown_s Soft Screensaver v1.13m
;       WWW: http://www.crown-s-soft.com
;
;    You may add you own figures. See file FIGURES.INC
;
;    Compile with FASM v1.73 for Kolibri or hier
;
;    Copyright(c) 2002-2004 Crown_s Soft. All rights reserved.
;

fullscreen = 1
n_points = 0x1800
delay = 2
const480 = 480
speed equ 0.004


macro align value { rb (value-1) - ($ + value-1) mod value }

use32
	       org    0x0
	       db     'MENUET01'	      ; 8 byte id
	       dd     0x01		      ; header version
	       dd     start		      ; start of code
	       dd     i_end		      ; size of image
	       dd     i_end+0x1000	      ; memory for app
	       dd     i_end+0x1000	      ; esp
	       dd     params		      ; I_Param
	       dd     0x0		      ; I_Icon

copyright db   'Crown_s Soft(c) Screensaver - www.crown-s-soft.com',0
copyrightlen:

include "lang.inc"
include "figuresi.inc"
include "..\..\..\macros.inc"
include "..\..\..\KOSfuncs.inc"
start:
    cmp     dword[params], '@ss'
    setz    [screensaver]
    mov     ebx, EVM_REDRAW + EVM_KEY + EVM_BUTTON
    cmovz   ebx, EVM_REDRAW + EVM_KEY + EVM_BUTTON + EVM_MOUSE
    mcall   SF_SET_EVENTS_MASK

    cmp [flscr],0
    jz @f
    mov     edi, transparent_cursor
    xor     eax, eax
    mov     ecx, 32*32
    rep     stosd
    mcall   SF_MOUSE_GET, SSF_LOAD_CURSOR, transparent_cursor, 2
    mov     ecx, eax
    mcall   SF_MOUSE_GET, SSF_SET_CURSOR
@@:
    cld
    finit
    call filling_alfbet ; fill table alfbet by casual numbers

    mov eax,[tabl_calls]
    mov [pp1adr],eax


    cmp [flscr],0
    jz	nofullscreen
      mcall SF_GET_SCREEN_SIZE

      mov  [maxy],ax
      sub  ax,const480
      jnc m5
	xor ax,ax
      m5:
      shr  ax,1
      mov  [posy],ax

      shr  eax,16
      mov  [maxx],ax
      sub  ax,const480
      jnc m6
	xor ax,ax
      m6:
      shr  ax,1
      mov  [posx],ax

      mov  [outsize],const480+65536*const480
    jmp m4
    nofullscreen:
      mov  [posx],75
      mov  [posy],20

      mov  [outsize],const480+65536*const480
    m4:

red:
    call draw_window

still:
    mcall SF_WAIT_EVENT_TIMEOUT, delay			; wait here for event
	
    cmp  eax,1			; redraw request ?
    je	 red
    cmp  eax,2			; key in buffer ?
    je	 key
    cmp  eax,3			; button in buffer ?
    je	 close
    cmp  eax,6			; mouse moved ?
    je	 close

    call calcframe
    mov  eax,SF_PUT_IMAGE
    mov  ebx,scr	      ; ebx pointer to image in memory
    mov  ecx,[outsize]	      ; ecx=image position in window [w]*65536+[h]
    mov  edx,dword [posy]     ; edx=image position in window [x]*65536+[y]
    mcall
jmp  still

key:
	cmp  [screensaver], 0
	jnz  close
	
    mcall SF_GET_KEY

    cmp  al,1			; is key in buffer ?
    jz	 still
    cmp  ah,0x1B		; is key ESC ?
    jz	 close
jmp  still

close:
    cmp     [screensaver], 0
    jz      @f
    mcall   SF_FILE, f70
  @@:
    mcall   SF_TERMINATE_PROCESS


;   *********************************************
;   *******  WINDOW DEFINITIONS AND DRAW ********
;   *********************************************


draw_window:
    mcall SF_REDRAW, SSF_BEGIN_DRAW ;start draw

    cmp  [flscr],0
    jnz  m2
      mcall SF_STYLE_SETTINGS, SSF_GET_SKIN_HEIGHT
      mov  esi, eax
                                   ; DRAW WINDOW
      mov  ecx,100*65536+const480+4         ; [y start] *65536 + [y size]
      add  ecx, esi
      mcall SF_CREATE_WINDOW, 100*65536+const480+9,,0x74000000,,copyright

    jmp m3
    m2:
      movzx  ebx,[maxx] 	     ; [x start] *65536 + [x size]
      movzx  ecx,[maxy] 	     ; [y start] *65536 + [y size]

      mov  edx,0x01000000	     ; color of work area RRGGBB,8->color gl
      mov  esi,0x805080d0	     ; color of grab bar  RRGGBB,8->color gl
      mov  edi,0x005080d0	     ; color of frames    RRGGBB
      xor  eax,eax		     ; function 0 : define and draw window
      mcall

      inc bx
      inc cx
      mcall SF_DRAW_RECT
    m3:

    mcall SF_REDRAW, SSF_END_DRAW ;end draw
ret


calcframe:
     cld
     mov  edi,scr
     mov  ecx,const480*const480*3/4
     xor  eax,eax
     rep stosd	   ; CLS


     mov  ebx,[frame]
     not  bh
     test bh,03h
     not  bh
     jnz  lb1
       ; ebx=xxxx xxxx  xxxx xxxx  xxxx xx11  xxxx xxxxb
       mov  byte [p],bl
     lb1:

     test  bx,03ffh
     jnz  lb2
       ; ebx=xxxx xxxx  xxxx xxxx  xxxx xx00  0000 0000b
       mov [p],0

       mov  eax,[pp1adr]
       mov  [pp0adr],eax

       inc [pp1]
       cmp [pp1],num_tabl_calls
       jnz lb3
	 mov [pp1],0
       lb3:

       movzx eax,[pp1]
       mov  eax,[tabl_calls+eax*4]
       mov  [pp1adr],eax
     lb2:


     fild  [frame]    ; st0=STime
     fmul  [speed1]   ; st0=STime*Speed
     fst   [bt_r]     ; al_rSTime*Speed
     fadd  st0,st0
     fstp  [al_r]     ; al_rSTime*Speed*2


     mov   [Fl],0
     mov ecx,[mFl]
     mov esi,alfbet
     ckl1:
       call [pp0adr]
       cmp  [p],0
       jz  lb4
	 fstp [x1]
	 fstp [y1]
	 fstp [z1]
	 call [pp1adr]
	 call mix
       lb4:

       call turn

       add  esi,4
       inc  [Fl]
     loop ckl1

     inc ebx
     mov [frame],ebx
ret


; turn coordinate system
turn:
  ; around Y
  ;  x= x*cos(a)-z*sin(a)
  ;  y= y
  ;  z= x*sin(a)+z*cos(a)
  fld  st2	; st0=z  st1=x  st2=y  st3=z
  fld  st1	; st0=x  st1=z  st2=x  st3=y  st4=z
  fld  [al_r]	; st0=a  st1=x  st2=z  st3=x  st4=y  st5=z
  fsincos	; st0=cos(a)  st1=sin(a)  st2=x  st3=z  st4=x  st5=y  st6=z
  fmul	st4,st0
  fmulp st6,st0 ; st0=sin(a)  st1=x  st2=z  st3=x*cos(a)  st4=y  st5=z*cos(a)
  fmul	st2,st0
  fmulp st1,st0 ; st0=x*sin(a)  st1=z*sin(a)  st2=x*cos(a) st3=y st4=z*c
  faddp st4,st0
  fsubp st1,st0


  ; around X
  ;  x=x
  ;  y= y*cos(b)+z*sin(b)
  ;  z=-y*sin(b)+z*cos(b)
  fld  st2	; st0=z  st1=x  st2=y  st3=z
  fld  st2	; st0=y  st1=z  st2=x  st3=y  st4=z
  fld  [bt_r]	; st0=b  st1=y  st2=z  st3=x  st4=y  st5=z
  fsincos	; st0=cos(b)  st1=sin(b)  st2=y  st3=z  st4=x  st5=y  st6=z
  fmul	st5,st0
  fmulp st6,st0 ; st0=sin(b)  st1=y  st2=z  st3=x  st4=y*cos(b)  st5=z*cos(b)
  fmul	st2,st0
  fmulp st1,st0 ; st0=y*sin(b) st1=z*sin(b) st2=x st3=y*cos(b) st4=z*cos(b)
  fsubp st4,st0 ; st0=z*sin(b)  st1=x  st2=y*cos(b)  st3=z*cos(b)-y*sin(b)
  faddp st2,st0

  ; st0=x  st1=y  st2=z
  fistp [x1]
  fistp [y1]
  fmul	  [Zdepth]   ; st0=z*Zdepth
  fiadd   [Zcolor]   ; st0=z*Zdepth+Zcolor
  fistp   [z_w]      ; st0z*Zdepth+Zcolor


  push edx

  mov  eax,[x1]
  add  eax,[mid]
  mul  [consts]
  add  eax,[y1]
  add  eax,[mid]
  mul  [const3]

  mov  dl,byte [z_w]	  ; al=ZZ
  mov  [scr+0+eax],dl
  mov  [scr+1+eax],dl
  mov  [scr+2+eax],dl

  pop  edx
ret



mix:
  fild	[p]	  ; st0=p
  fmul	[mp]	  ; st0=p=p*mp
  fld	st0	  ; st0=p  st1=p
  fmul	st4,st0
  fmul	st3,st0
  fmulp st2,st0   ; st0=p    st1=x*p  st2=y*p  st3=z*p

  fld1
  fsubrp st1,st0  ; st0=1-p  st1=x*p  st2=y*p  st3=z*p
  fld	 [z1]	  ; st0=z1   st1=1-p  st2=x*p  st3=y*p  st4=z*p
  fmul	st0,st1
  faddp st4,st0
  fld	 [y1]	  ; st0=y1   st1=1-p  st2=x*p  st3=y*p  st4=
  fmul	st0,st1
  faddp st3,st0
  fld	 [x1]	  ; st0=x1   st1=1-p  st2=x*p  st3=y*p+y1*(1-p)  st4=
  fmulp st1,st0   ; st0=x1*(1-p)      st1=x*p  st2=y*p+y1*(1-p)  st3=
  faddp st1,st0   ; st0=x=x*p+x1*(1-p)  st1=y=y*p+y1*(1-p)  st2=z
ret


filling_alfbet:
     ; Initialize RND
     mcall SF_GET_SYS_TIME
     ; eax - fist random number

     mov   ecx,n_points
     mov   edi,alfbet
     mov   ebx,8088405h
     ck2:
       stosd
       ; Compute next random number
       ; New := 8088405H * Old + 1
       mul   ebx
       inc   eax
     loop ck2
ret


; DATA AREA
align 2

  frame   dd	 0

  mp	  dd	 0.00390625
  n_r	  dd	 0.00390625

  mal_r   dd	 6.28318530717958648
  mbt_r   dd	 6.28318530717958648

  const3   dd	 3
  const6   dw	 6
  consts   dd    const480

  mFl	  dd	 n_points
  pp1	  dw	 0

  Zdepth  dd	 0.3
  Zcolor  dw	 140

  mid	  dd	 240   ; centre of screen

  speed1  dd	 speed
  flscr   db	 fullscreen

  align 4

  outsize dd	 ?
  posy	  dw	 ?
  posx	  dw	 ?
  maxy	  dw	 ?
  maxx	  dw	 ?

  Fl	  dd	 ?

  p	  dd	 ?
  al_r	  dd	 ?
  bt_r	  dd	 ?


  pp0adr  dd	 ?
  pp1adr  dd	 ?

  z_w	  dw	 ?

  x1	  dd	 ?
  y1	  dd	 ?
  z1	  dd	 ?

f70:    ; run
        dd SSF_START_APP, 0, 0, 0, 0
        db '/sys/@SS',0

screensaver db ?
params rb 4
transparent_cursor rd 32*32

align 16
  alfbet:		  ; alfbet  db  n_points*4  dup (?)
  scr = alfbet+n_points*4   ; scr     db  480*480*3+1 dup (?)
  i_end = scr+const480*const480*3+1 ; i_param db  256         dup (?)
