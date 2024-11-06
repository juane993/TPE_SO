GLOBAL write
GLOBAL read
GLOBAL getSeconds
GLOBAL clear
GLOBAL getInfoReg
GLOBAL setFontSize
GLOBAL getScreenResolution
GLOBAL drawRect
GLOBAL getTicks
GLOBAL getMemory
GLOBAL playSound
GLOBAL kaboom
GLOBAL setFontColor
GLOBAL getFontColor
GLOBAL allocMemory
GLOBAL my_free
GLOBAL my_create_process
GLOBAL my_kill
GLOBAL my_block
GLOBAL my_unblock
GLOBAL my_nice
GLOBAL my_exit
GLOBAL my_yield
GLOBAL my_ps
GLOBAL my_getPid
GLOBAL my_semWait
GLOBAL my_semPost
GLOBAL my_semInit
GLOBAL my_semClose
GLOBAL my_waitPid
GLOBAL my_waitPidNoHang


read:
    mov rax, 0
    int 80h
    test rax, rax
    jnz .finish
    call my_yield
    jmp read

.finish:
    ret

write:
    mov rax, 1
    int 80h
    ret

clear:
    mov rax, 2
    int 80h
    ret

getSeconds:
    mov rax, 3
    int 80h
    ret

getInfoReg:
    mov rax, 4
    int 80h
    ret

setFontSize:
    mov rax, 5
    int 80h
    ret

getScreenResolution:
    mov rax, 6
    int 80h
    ret

drawRect:
    mov rax, 7
    mov r10, rcx
    int 80h
    ret

getTicks:
    mov rax, 8
    int 80h
    ret

getMemory:
    mov rax, 9
    int 80h
    ret

playSound:
    mov rax, 10
    int 80h
    ret

setFontColor:
    mov rax, 11
    int 80h
    ret

getFontColor:
    mov rax, 12
    int 80h
    ret

allocMemory:
    mov rax, 13
    int 80h
    ret

my_free:
    mov rax, 14
    int 80h
    ret

my_create_process:
    mov rax, 15
    int 80h
    ret

my_kill:
    mov rax, 16
    int 80h
    ret

my_block:
    mov rax, 17
    int 80h
    ret

my_unblock:
    mov rax, 18
    int 80h
    ret

my_nice:
    mov rax, 19
    int 80h
    ret

my_exit:
    mov rax, 20
    int 80h
    ret

my_yield:
    mov rax, 21
    int 80h
    ret

my_ps:
    mov rax, 22
    int 80h
    ret

my_getPid:
    mov rax, 23
    int 80h
    ret

my_semWait:
    mov rax, 24
    int 80h
    call my_yield
    ret

my_semPost:
    mov rax, 25
    int 80h
    ret

my_semInit:
    mov rax, 26
    int 80h
    ret

my_semClose:
    mov rax,27
    int 80h
    ret

my_waitPid:
    mov rax, 28
    int 80h
    call my_yield
    ret

my_waitPidNoHang:
    mov rax, 29
    int 80h
    ret