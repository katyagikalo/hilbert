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
    
    push rbx
    
    mov rbx, 1                  //segment length
    mov r8, rax
    imul r8, 2                  //2 * segment degree
    shl rbx, r8                 //1 << (2 * (segment degree))
    
    mov rcx, 1                   //segment max coordinate
    shl rcx, rax                 //1 << segment degree
    dec rcx                      //(1 << segment_degree) - 1

    xor r9, r9                //counter

segments_loop:
    cmp r9, rbx
    je degrees_loop

    //left upper segment
    mov r11, rbx                        //segment length + i
    add r11, r9
    imul r11, 4
    
    mov r10, dword ptr [rsi + 4*r9]       //x[i]
    mov dword ptr [rsi + r11], r10

    mov r10, dword ptr [rdx + 4*r9]       //y[i] + segment max coordinate + 1
    add r10, rcx
    inc r10
    mov dword ptr [rdx + r11], r10

    //right upper segment
    mov r11, rbx                        //2 * segment length + i
    imul r11, 2
    add r11, r9
    imul r11, 4

    mov r10, dword ptr [rsi + 4*r9]       //x[i] + segment max coordinate + 1
    add r10, rcx
    inc r10
    mov dword ptr [rsi + r11], r10

    mov r10, dword ptr [rdx + 4*r9]       //y[i] + segment max coordinate + 1
    add r10, rcx
    inc r10
    mov dword ptr [rdx + r11], r10

    //left lower segment
    mov r10, dword ptr [rsi + 4*r9]
    mov r11, dword ptr [rdx + 4*r9]
    mov dword ptr [rsi + 4*r9], r11
    mov dword ptr [rdx + 4*r9], r10

    //right lower segment
    mov r11, rbx                        //3 * segment length + i
    imul r11, 3
    add r11, r9
    imul r11, 4

    mov r10, rcx                        //2 * segment max coordinate + 1 - x[i]
    imul r10, 2
    inc r10
    sub r10, dword ptr [rsi + 4*r9]
    mov dword ptr [rsi + r11], r10

    mov r10, rcx                        //segment max coordinate + 1 - y[i]
    sub r10, dword ptr [rdx + 4*r9]
    mov dword ptr [rdx + r11], r10

    inc r9
    
    pop rbx
    jmp segments_loop

end:
    ret
