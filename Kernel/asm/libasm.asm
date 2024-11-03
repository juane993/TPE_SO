EXTERN copyRegisters
GLOBAL cpuVendor
GLOBAL getTime
GLOBAL getKeyPressed
GLOBAL startSound
GLOBAL stopSound
GLOBAL saveRegisters
GLOBAL initProcessStack
GLOBAL yield
GLOBAL acquire
GLOBAL release
GLOBAL disableInterrupts
GLOBAL enableInterrupts


section .text


%macro pushState 0
	push rax
	push rbx
	push rcx
	push rdx
	push rbp
	push rdi
	push rsi
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15
%endmacro
    
cpuVendor:
    push rbp
    mov rbp, rsp

    push rbx

    mov rax, 0
    cpuid


    mov [rdi], ebx
    mov [rdi + 4], edx
    mov [rdi + 8], ecx

    mov byte [rdi+13], 0

    mov rax, rdi

    pop rbx

    mov rsp, rbp
    pop rbp
    ret

getTime:
    push rbp
    mov rbp, rsp

    mov al, 0Bh
    out 70h, al
    in  al, 71h
    or al, 6h
    out 71h, al

    mov al, 0
    out 70h, al
    in al, 71h
    mov [rdx], al

    mov al, 2
    out 70h, al
    in al, 71h
    mov [rsi], al
    
    mov al, 4
    out 70h, al
    in al, 71h
    mov [rdi], al

    mov rsp, rbp
    pop rbp
    ret

getKeyPressed:
    push rbp
    mov rbp, rsp

    xor rax, rax
    in al, 60h

    mov rsp, rbp
    pop rbp
    ret


disableInterrupts:
    cli
    ret

enableInterrupts:
    sti
    ret


acquire:
    mov al, 0
.retry:
    xchg [rdi], al
    test al, al
    jz .retry
    ret


release:
    mov byte [rdi], 1
    ret

;acquire:
;    mov al, 1           ; Queremos bloquear con valor 1
;.retry:
;    xchg al, [rdi]       ; Intercambiamos al (1) con el lock
;    test al, al          ; Verificamos el valor anterior
;    jnz .retry           ; Si no era 0, significa que estaba bloqueado, reintentamos
;    ret                  ; Si era 0, hemos adquirido el lock, regresamos
;
;release:
;    mov byte [rdi], 0    ; Liberamos el lock escribiendo 0
;    ret

int_80:
    push rbp
    mov rbp, rsp

    xor rax, rax
    in al, 60h

    mov rsp, rbp
    pop rbp
    ret

startSound:
	push rbp
    push rdx
	mov rbp, rsp

	mov al, 0xB6
	out 43h, al

    mov rdx, 0
    mov rax, 1193180
    div rdi
    
	out 42h, al
	mov al, ah
	out 42h, al

	in al, 61h
 	or al, 03h
	out 61h, al

	mov rsp, rbp
    pop rdx
	pop rbp
	ret

stopSound:
  	in al, 61h
	and al, 0xFC
	out 61h, al
  	ret

yield:
    int 20h
    ret

saveRegisters:
    mov rdi, rbp 
    call copyRegisters
    ret

%macro pushStateNoRax 0
    push rbx
    push rcx
    push rdx
    push rbp
    push rdi
    push rsi
    push r8
    push r9
    push r10
    push r11
    push r12
    push r13
    push r14
    push r15
%endmacro

initProcessStack:
;; TODO: Alineamiento???
;	mov r8, rsp
;	mov rsp, rsi
;	push 0x0
;	push rdx
;	push 0x202
;	push 0x8
;	push rdi
;	mov rdi, rdi
;	mov rsi, rcx
;	push 0x0
;	pushStateNoRax
;	mov rax, rsp
;	mov rsp, r8
;	ret


    mov r10, rsp 	; Preservar rsp
    mov r9, rbp		; Preservar rbp
    mov rsp, rsi 	; Carga sp del proceso
    mov rbp, rsi
    push 0x0        ; Código de error (o 0 como valor inicial)
    push rsi        ; Dirección de pila
    push 0x202      ; Valor de FLAGS (0x202 habilita interrupciones)
    push 0x8        ; Selector de segmento de código
    push rdi        ; Dirección de retorno (RIP para el proceso)
    mov rdi, rdx 	; Primer argumento, argc
    mov rsi, rcx	; Segundo argumento, args
    mov rdx, r8     ; Tercer parametro, mainFunc
    pushState
    mov rax, rsp
    mov rsp, r10
    mov rbp, r9
    ret



