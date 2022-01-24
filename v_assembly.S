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
    //inc rcx
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

    /*mov r9, r8
    add r9, rcx                             //segment length + i

    //left upper segment
    movups xmm0, [rsi + rcx]                 //x[i] .. x[i+3]
    movups [rsi + r9], xmm0

    movups xmm0, [rdx + rcx]                 //y[i] .. y[i+3]
    paddd xmm0, xmm2                        //y[i] + segment_coord .. y[i+3] + segment_coord
    movups [rdx + r9], xmm0

    //right upper segment
    add r9, r8                             //2 * segment length + i

    movups xmm0, [rsi + rcx]                 //x[i] .. x[i+3]
    paddd xmm0, xmm2
    movups [rsi + r9], xmm0

    movups xmm0, [rdx + rcx]                 //y[i] .. y[i+3]
    paddd xmm0, xmm2                        //y[i] + segment_coord .. y[i+3] + segment_coord
    movups [rdx + r9], xmm0

    //left lower segment
    movups xmm0, [rsi + rcx]                 //x[i] .. x[i+3]
    movups xmm1, [rdx + rcx]                 //y[i] .. y[i+3]

    movups [rsi + rcx], xmm1
    movups [rdx + rcx], xmm0

    //right lower segment
    add r9, r8                             //3 * segment length + i

    mov r10, 1
    movd xmm1, r10
    pshufd xmm1, xmm1, 0x00

    movups xmm0, xmm2
    paddd xmm0, xmm0
    psubd xmm0, xmm1
    psubd xmm0, [rsi + rcx]
    movups [rsi + r9], xmm0

    movups xmm0, xmm2
    psubd xmm0, xmm1
    psubd xmm0, [rdx + rcx]
    movups [rdx + r9], xmm0*/

    add rcx, 4
    
    jmp segments_loop

end:
    ret
