format ELF

include "__lib__.inc"

fun      equ con_get_cursor_height
fun_str  equ 'con_get_cursor_height'

section '.text'

fun_name db fun_str, 0

section '.data'

extrn lib_name
public fun

fun dd fun_name
lib dd lib_name
