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
    imul rcx, 2                             //2 * segment degree
    shl r8, cl                              //1 << (2 * (segment degree))
    
    mov rcx, rax
    mov r9, 1                               //segment max coordinate
    shl r9, cl                              //1 << segment degree
    //dec r9                                  //(1 << segment_degree) - 1

    xor rcx, rcx                            //loop counter
    
segments_loop:
    cmp rcx, r8
    je degrees_loop

    jmp left_upper

left_upper_end:

    jmp right_upper

right_upper_end:
    
    jmp left_lower

left_lower_end:
    
    jmp right_lower

right_lower_end:

    add rcx, 4
    
    jmp segments_loop

left_upper:
    mov r11, r8                             //segment length + i
    add r11, rcx
    imul r11, 4
    
    movups xmm0, [rsi + 4*rcx]              //x[i] .. x[i+3]
    movups [rsi + r11], xmm0

    movups xmm0, [rdx + 4*rcx]                   //y[i] .. y[i+3]
    movd xmm1, r9
    pshufd xmm1, xmm1, 0x00
    paddd xmm0, xmm1                            //y[i] + segment_coord .. y[i+3] + segment_coord
    movups [rdx + r11], xmm0
    
    jmp left_upper_end

right_upper:
    mov r11, r8                             //2 * segment length + i
    imul r11, 2
    add r11, rcx
    imul r11, 4

    movups xmm0, [rsi + 4*rcx]              //x[i] .. x[i+3]
    movd xmm1, r9
    pshufd xmm1, xmm1, 0x00
    paddd xmm0, xmm1
    movups [rsi + r11], xmm0

    movups xmm0, [rdx + 4*rcx]                   //y[i] .. y[i+3]
    movd xmm1, r9
    pshufd xmm1, xmm1, 0x00
    paddd xmm0, xmm1                            //y[i] + segment_coord .. y[i+3] + segment_coord
    movups [rdx + r11], xmm0
    
    jmp right_upper_end

left_lower:
    movups xmm0, [rsi + 4*rcx]              //x[i] .. x[i+3]
    movups xmm1, [rdx + 4*rcx]                   //y[i] .. y[i+3]
    
    movups [rsi + 4*rcx], xmm1
    movups [rdx + 4*rcx], xmm0
    
    jmp left_lower_end

right_lower:
    mov r11, r8                        //3 * segment length + i
    imul r11, 3
    add r11, rcx
    imul r11, 4

    movd xmm1, 1
    pshufd xmm1, xmm1, 0x00

    movups xmm0, r9
    pshufd xmm0, xmm0, 0x00
    pmuld xmm0, 2
    psubd xmm0, xmm1
    psubd xmm0, [rsi + 4*rcx]
    movups [rsi + r11], xmm0

    movups xmm0, r9
    pshufd xmm0, xmm0, 0x00
    psubd xmm0, xmm1
    psubd xmm0, [rdx + 4*rcx]
    movups [rdx + r11], xmm0
    
    jmp right_lower_end

end:
    ret
