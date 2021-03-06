pb equ [esp + 4 + 8*4]
align 16
progressbar_draw:
        pushad
; draw frame
        mov    eax, pb
        mov    edx, [eax + PB_FRAME_COLOR]
        mov    ebx, [eax + PB_LEFT]
        mov    edi, [eax + PB_TOP]
        mov    ebp, [eax + PB_WIDTH]
        mov    esi, [eax + PB_HEIGHT]
        add    ebp, ebx
        add    esi, edi
        dec    ebp
        dec    esi
        mov    eax, SF_DRAW_LINE
        shl    ebx, 16
        mov    bx, bp
        shrd   ecx, edi, 16
        mov    cx, di
        int    64
        shrd   ecx, esi, 16
        mov    cx, si
        int    64
        shld   esi, ebx, 16
        mov    bx, si
        shrd   ecx, edi, 16
        int    64
        shrd   ebx, ebp, 16
        mov    bx, bp
        int    64
; if min > max then .skip
        mov    eax, pb
        mov    edx, [eax + PB_MAX]
        mov    ecx, [eax + PB_MIN]
        cmp    ecx, edx
        ; jg     .skip
        jne    .min_no_eq_max
        dec    edx
.min_no_eq_max:
; draw all progress rectangle
        call   get_progress_width
        mov    edi, eax
        mov    eax, pb
        mov    ebx, [eax + PB_LEFT]
        mov    ecx, [eax + PB_TOP]
        mov    edx, [eax + PB_PROGRESS_COLOR]
        inc    ebx
        inc    ecx
        shl    ebx, 16
        shl    ecx, 16
        add    ebx, edi
        add    ecx, [eax + PB_HEIGHT]
        dec    ecx
        mov    eax, SF_DRAW_RECT
        dec    ecx
        int    64
; draw last part of non-progress rectangle
; edi = pW, esi = W - 2
        sub    esi, edi ; width
        shr    ebx, 16
        add    ebx, edi
        shl    ebx, 16
        add    ebx, esi
        mov    esi, pb
        mov    edx, [esi + PB_BACK_COLOR]
        int    64
; .skip:
        popad
        ret    4

align 16
get_progress_width:
; pW = (W-2)(value - min) / (max - min)
        mov    esi, [eax + PB_WIDTH]
        mov    eax, [eax + PB_VALUE]
        dec    esi
        sub    eax, ecx
        dec    esi
        neg    ecx
        add    ecx, edx
        mul    esi
        div    ecx
        ret

align 16
progressbar_progress:
        pushad
; if min > max then .skip
        mov    eax, pb
        mov    edx, [eax + PB_MAX]
        mov    ecx, [eax + PB_MIN]
        cmp    ecx, edx
        ; jg     .skip
        jne    .min_no_eq_max1
        dec    edx
.min_no_eq_max1:
        call   get_progress_width
        mov    edi, eax
; increase value
        mov    eax, pb
        mov    ecx, [eax + PB_VALUE]
        mov    edx, [eax + PB_MAX]
        inc    ecx
; if value > max then value = max
        cmp    ecx, edx
        jng    .next
        mov    ecx, edx
.next:
        mov    [eax + PB_VALUE], ecx
; draw new part of progress rectangle
        mov    eax, pb
        mov    ecx, [eax + PB_MIN]
        cmp    ecx, edx
        jne    .min_no_eq_max2
        dec    edx
.min_no_eq_max2:
        call   get_progress_width
        mov    esi, eax
; edi = last pW, esi = new pW
        mov    eax, pb
        mov    ebx, [eax + PB_LEFT]
        mov    ecx, [eax + PB_TOP]
        mov    edx, [eax + PB_PROGRESS_COLOR]
        inc    ebx
        inc    ecx
        add    ebx, edi
        shl    ecx, 16
        shl    ebx, 16
        add    ecx, [eax + PB_HEIGHT]
        add    ebx, esi
        dec    ecx
        sub    ebx, edi
        mov    eax, SF_DRAW_RECT
        dec    ecx
        int    64
; .skip:
        popad
        ret    4
restore pb
