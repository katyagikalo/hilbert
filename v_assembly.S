.intel_syntax noprefix
.global v_assembly

.text

.align 16

v_assembly:
    cmp rdi, 0
    jle end

    mov dword ptr [rsi], 0      //x[0]
    mov dword ptr [rdx], 0      //y[0]

    inc rsi
    inc rdx

    mov dword ptr [rsi], 0      //x[1]
    mov dword ptr [rdx], 1      //y[1]

    inc rsi
    inc rdx

    mov dword ptr [rsi], 1      //x[2]
    mov dword ptr [rdx], 1      //y[2]

    inc rsi
    inc rdx

    mov dword ptr [rsi], 1      //x[3]
    mov dword ptr [rdx], 0      //y[3]
    
    inc rsi
    inc rdx

end:
    ret
