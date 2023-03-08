%line 16+1 macro_arg_types.asm

myhello: db 'hello', 10, 0
_start:
 mov rdi, myhello
%line 19+0 macro_arg_types.asm
 call print_string

%line 20+1 macro_arg_types.asm

%line 20+0 macro_arg_types.asm
 mov rdi, 42
 call print_uint

%line 21+1 macro_arg_types.asm
 mov rax, 60
 syscall
