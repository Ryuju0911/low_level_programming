mov rax, -1
mov rdx, 2

cmp rax, rdx
jg location   ; if rax > rdx
ja location   ; if |rax| > |rdx|

cmp rax, rdx
je location   ; if rax equals rdx
jne location  ; if rax is not equal to rdx