.intel_syntax noprefix
.global v_assembly

.text

.align 16

v_assembly:
    cmp rdi, 0
    jle end

    xor rax, rax                            //segment degree

    mov qword ptr [rsi], 0                  //x[0]
    mov qword ptr [rdx], 0                  //y[0]

    mov qword ptr [rsi + 4], 0              //x[1]
    mov qword ptr [rdx + 4], 1              //y[1]

    mov qword ptr [rsi + 8], 1              //x[2]
    mov qword ptr [rdx + 8], 1              //y[2]

    mov qword ptr [rsi + 12], 1             //x[3]
    mov qword ptr [rdx + 12], 0             //y[3]

degrees_loop:
    inc rax

    cmp rax, rdi
    je end
    
    mov r8, 1                               //segment length
    mov rcx, rax
    add rcx, rcx
    shl r8, cl                              //1 << (2 * (segment degree))
    
    mov rcx, rax
    mov r9, 1                               //segment max coordinate
    shl r9, cl                              //1 << segment degree
    movd xmm2, r9
    pshufd xmm2, xmm2, 0x00

    xor rcx, rcx                            //loop counter
    
segments_loop:
    cmp rcx, r8
    je degrees_loop

    //left upper segment
    mov r11, r8                             //segment length + i
    add r11, rcx
    imul r11, 4

    movups xmm0, [rsi + r9]              //x[i] .. x[i+3]
    movups [rsi + r11], xmm0

    movups xmm0, [rdx + r9]                   //y[i] .. y[i+3]
    paddd xmm0, xmm2                            //y[i] + segment_coord .. y[i+3] + segment_coord
    movups [rdx + r11], xmm0

    //right upper segment
    mov r11, r8                             //2 * segment length + i
    imul r11, 2
    add r11, rcx
    imul r11, 4

    movups xmm0, [rsi + r9]              //x[i] .. x[i+3]
    paddd xmm0, xmm2
    movups [rsi + r11], xmm0

    movups xmm0, [rdx + r9]                   //y[i] .. y[i+3]
    paddd xmm0, xmm2                            //y[i] + segment_coord .. y[i+3] + segment_coord
    movups [rdx + r11], xmm0

    //left lower segment
    movups xmm0, [rsi + r9]              //x[i] .. x[i+3]
    movups xmm1, [rdx + r9]                   //y[i] .. y[i+3]

    movups [rsi + r9], xmm1
    movups [rdx + r9], xmm0

    //right lower segment
    mov r11, r8                        //3 * segment length + i
    imul r11, 3
    add r11, rcx
    imul r11, 4

    mov r10, 1
    movd xmm1, r10
    pshufd xmm1, xmm1, 0x00

    movss xmm0, xmm2
    paddd xmm0, xmm0
    psubd xmm0, xmm1
    psubd xmm0, [rsi + r9]
    movups [rsi + r11], xmm0

    movss xmm0, xmm2
    psubd xmm0, xmm1
    psubd xmm0, [rdx + r9]
    movups [rdx + r11], xmm0

    add r9, 16
    add rcx, 4
    
    jmp segments_loop

end:
    ret
