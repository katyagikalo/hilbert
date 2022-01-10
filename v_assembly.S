.intel_syntax noprefix
.global v_assembly

.text

.align 16

v_assembly:
    cmp rdi, 0
    jle end

    mov dword ptr [rsi], 0      //x[0]
    mov dword ptr [rdx], 0      //y[0]

    add rsi, 4
    add rdx, 4

    mov dword ptr [rsi], 0      //x[1]
    mov dword ptr [rdx], 1      //y[1]

    add rsi, 4
    add rdx, 4

    mov dword ptr [rsi], 1      //x[2]
    mov dword ptr [rdx], 1      //y[2]

    add rsi, 4
    add rdx, 4

    mov dword ptr [rsi], 1      //x[3]
    mov dword ptr [rdx], 0      //y[3]
    
    add rsi, 4
    add rdx, 4

end:
    ret
