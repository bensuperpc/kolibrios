format ELF
include "__lib__.inc"
fun      equ __func@clearerr
fun_str  equ 'clearerr'
section '.text'
fun_name db fun_str, 0
section '.data'
extrn lib_name
public fun as fun_str
fun dd fun_name
lib dd lib_name
