section .text

exit:
  xor rdi, rdi
  mov rax, 60
  syscall

string_length:
  xor rax, rax
.loop:
  cmp byte [rdi + rax], 0
  je .end
  inc rax
  jmp .loop
.end:
  ret

print_string:
  push rdi
  call string_length          ; rax = len(string)
  pop rsi                     ; rsi = rdi (string)
  mov rdx, rax                ; rdx = rax
  mov rax, 1                  ; rax = 1 (write system call)
  mov rdi, 1                  ; rdi = 1 (stdout)
  syscall
  ret

print_char:
  push rdi
  mov rdi, rsp
  call print_string
  pop rdi
  ret

print_newline:
  mov rdi, 10
  jmp print_char

print_uint:
  mov rax, rdi                ; rax = rdi (number)
  mov rdi, rsp
  push 0                      ; null-terminator
  sub rsp, 16

  dec rdi
  mov r8, 10

.loop:
  xor rdx, rdx                ; rdx = 0
  div r8                      ; rax / 10
  or dl, 0x30                 ; converts the digit to ASCII character.
                              ; DL is the lowest bit of RDX register.
                              ; (0x30 is '0' in ASCII code)
  dec rdi                     ; --rdi
  mov [rdi], dl
  test rax, rax               ; rax AND rax
  jnz .loop

  call print_string

  add rsp, 24
  ret

print_int:
  test rdi, rdi
  jns print_uint
  push rdi
  mov rdi, '-'
  call print_char
  pop rdi
  neg rdi
  jmp print_uint

read_char:
  push 0
  xor rax, rax                ; rax = 0 (read system call)                                 
  xor rdi, rdi                ; rdi = 0 (stdin)
  mov rsi, rsp                ; rsi = rsp (character)
  mov rdx, 1                  ; rdx = 1 (number of bytes)
  syscall
  pop rax
  ret

read_word:
  push r14
  push r15
  xor r14, r14                ; r14 = 0
  mov r15, rsi                ; r15 = rsi (length)
  dec r15                     ; --r15 (length - null-terminator)

  .A:
  push rdi
  call read_char
  pop rdi
  cmp al, ' '                 ; if the character is a space
                              ; AL is the lowest byte of RAX.
  je .A
  cmp al, 10                  ; if the character is newline
  je .A
  cmp al, 13                  ; if the character is carriage return
  je .A 
  cmp al, 9                   ; if the character is tab
  je .A
  test al, al                 ; if the character is null-terminator
  jz .C

  .B:
  mov byte [rdi + r14], al  
  inc r14                     ; ++r14

  push rdi
  call read_char
  pop rdi
  cmp al, ' '
  je .C
  cmp al, 10
  je .C
  cmp al, 13
  je .C 
  cmp al, 9
  je .C
  test al, al
  jz .C
  cmp r14, r15                ; if r14 == 15
  je .D

  jmp .B

  .C:
  mov byte [rdi + r14], 0
  mov rax, rdi                ; rax = rdi
  
  mov rdx, r14                ; rdx = r14
  pop r15
  pop r14
  ret                         ; return buffer address

  .D:
  xor rax, rax                ; rax = 0
  pop r15
  pop r14
  ret                         ; return 0

; returns rax: number, rdx : length
parse_uint:
  mov r8, 10                  ; r8 = 10
  xor rax, rax                ; rax = 0
  xor rcx, rcx                ; rcx = 0
.loop:
  movzx r9, byte [rdi + rcx]   
  cmp r9b, '0'                ; r9b is the lowest byte of R9
  jb .end                     ; if the byte < 0 or the byte > 9, the byte is not digit
  cmp r9b, '9'
  ja .end
  xor rdx, rdx                ; rdx = 0 
  mul r8                      ; rax = rax * 10
  and r9b, 0x0f               ; the lower 4 bits of r9b is extracted
  add rax, r9                 ; rax += r9
  inc rcx                     ; ++rcx
  jmp .loop
  .end:
  mov rdx, rcx                ; rdx = rcx (size)
  ret

; returns rax: number, rdx : length
parse_int:
  mov al, byte [rdi]
  cmp al, '-'
  je .signed
  jmp parse_uint
.signed:
  inc rdi
  call parse_uint
  neg rax
  test rdx, rdx
  jz .error

  inc rdx
  ret

  .error:
  xor rax, rax
  ret 

string_equals:
  mov al, byte [rdi]
  cmp al, byte [rsi]
  jne .no
  inc rdi
  inc rsi
  test al, al
  jnz string_equals
  mov rax, 1
  ret
  .no:
  xor rax, rax
  ret 

; rdi = source
; rsi = dest
; rdx = dest length                    
string_copy:

  push rdi
  push rsi
  push rdx
  call string_length
  pop rdx
  pop rsi
  pop rdi

  cmp rax, rdx
  jae .too_long  ; we also need to store null-terminator
  
  push rsi 

    .loop: 
    mov dl, byte[rdi]
    mov byte[rsi], dl
    inc rdi
    inc rsi
    test dl, dl
    jnz .loop 

  pop rax 
  ret

  .too_long:
  xor rax, rax
  ret