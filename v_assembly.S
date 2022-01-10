.intel_syntax noprefix
.global v_assembly

.text

.align 16

v_assembly:
    cmp rdi, 0
    jle end

    xor rax, rax                //segment degree

    mov dword ptr [rsi], 0      //x[0]
    mov dword ptr [rdx], 0      //y[0]

    mov dword ptr [rsi + 4], 0      //x[1]
    mov dword ptr [rdx + 4], 1      //y[1]

    mov dword ptr [rsi + 8], 1      //x[2]
    mov dword ptr [rdx + 8], 1      //y[2]

    mov dword ptr [rsi + 12], 1      //x[3]
    mov dword ptr [rdx + 12], 0      //y[3]

degrees_loop:
    inc rax

    cmp rax, rdi
    je end
    
    mov rcx, 1                  //segment length
    mov r8, rax
    mul r8, 2                   //2 * segment degree
    shl rcx, r8                 //1 << (2 * (segment degree))
    
    mov r9, 1                   //segment max coordinate
    shl r9, rax                 //1 << segment degree
    dec r9                      //(1 << segment_degree) - 1

    xor r10, r10                //counter

segments_loop:
    cmp r10, rcx
    je degrees_loop
    
    mov r11, dword ptr [rsi + r10]
    mov dword ptr [rsi + r9 + r10], r11

    inc r10
    jmp segments_loop
end:
    ret
