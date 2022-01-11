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
    je rnd
    
    mov r8d, 1                  //segment length
    mov rcx, rax
    imul rcx, 2                  //2 * segment degree
    shl r8d, cl                  //1 << (2 * (segment degree))
    
    mov rcx, rax
    mov r9d, 1                   //segment max coordinate
    shl r9d, cl                  //1 << segment degree
    dec r9d                      //(1 << segment_degree) - 1

    xor rcx, rcx                //counter

segments_loop:
    cmp rcx, r8d
    je degrees_loop

    //left upper segment
    mov r11d, r8d                        //segment length + i
    add r11d, rcx
    imul r11d, 4
    
    mov r10d, dword ptr [rsi + 4*rcx]       //x[i]
    mov dword ptr [rsi + r11d], r10d

    mov r10d, dword ptr [rdx + 4*rcx]       //y[i] + segment max coordinate + 1
    add r10d, rcx
    inc r10d
    mov dword ptr [rdx + r11d], r10d

    //right upper segment
    mov r11d, r8d                        //2 * segment length + i
    imul r11d, 2
    add r11d, rcx
    imul r11d, 4

    mov r10d, dword ptr [rsi + 4*rcx]       //x[i] + segment max coordinate + 1
    add r10d, rcx
    inc r10d
    mov dword ptr [rsi + r11d], r10d

    mov r10d, dword ptr [rdx + 4*rcx]       //y[i] + segment max coordinate + 1
    add r10d, rcx
    inc r10d
    mov dword ptr [rdx + r11d], r10d

    //left lower segment
    mov r10d, dword ptr [rsi + 4*rcx]
    mov r11d, dword ptr [rdx + 4*rcx]
    mov dword ptr [rsi + 4*rcx], r11d
    mov dword ptr [rdx + 4*rcx], r10d

    //right lower segment
    mov r11d, r8d                        //3 * segment length + i
    imul r11d, 3
    add r11d, rcx
    imul r11d, 4

    mov r10d, rcx                        //2 * segment max coordinate + 1 - x[i]
    imul r10d, 2
    inc r10d
    sub r10d, dword ptr [rsi + 4*rcx]
    mov dword ptr [rsi + r11d], r10d

    mov r10d, rcx                        //segment max coordinate + 1 - y[i]
    sub r10d, dword ptr [rdx + 4*rcx]
    mov dword ptr [rdx + r11d], r10d

    inc r9d
    
    jmp segments_loop

end:
    ret
