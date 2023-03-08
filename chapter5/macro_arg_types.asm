%macro print 1
  %ifid %1
    mov rdi, %1
    call print_string
  %else

    %ifnum %1
      mov rdi, %1
      call print_uint
    %else
      %error "String literals are not supposed yet"
    %endif
  %endif

%endmacro

myhello: db 'hello', 10, 0
_start:
  print myhello
  print 42
  mov rax, 60
  syscall
