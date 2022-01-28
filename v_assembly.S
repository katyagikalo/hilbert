.intel_syntax noprefix
.global v_assembly

.text

.align 16

v_assembly:
    mov rax, qword ptr [rsp]                //segment_length
    mov rcx, qword ptr [rsp + 8]            //segment_coord
    mov r8, qword ptr [rsp + 12]            //start
    mov r9, qword ptr [rsp + 16]            //end
    mov rdx, qword ptr [rsp + 20]           //x
    mov rsi, qword ptr [rsp + 24]           //y

    cmp rax, 0
    jle end

    movd xmm2, rcx
    pshufd xmm2, xmm2, 0x00                 //segment_coord .. segment_coord

    mov r10, 1
    movd xmm3, r10
    pshufd xmm3, xmm3, 0x00                 //1 .. 1

segments_loop:
    cmp r8, r9
    je end

    movups xmm0, [rdx + r8]                 //x[i] .. x[i+3]
    movups xmm1, [rsi + r8]                 //y[i] .. y[i+3]
    
    mov rdi, rax
    add rdi, r8                             //segment_length + i

    //left lower segment
    movups [rdx + r8], xmm1
    movups [rsi + r8], xmm0

    //left upper segment
    movups [rdx + rdi], xmm0

    paddd xmm1, xmm2                        //y[i] + segment_coord .. y[i+3] + segment_coord
    movups [rsi + rdi], xmm1

    //right upper segment
    add rdi, rax                            //2*segment_length + i

    paddd xmm0, xmm2                        //x[i] + segment_coord .. x[i+3] + segment_coord
    movups [rdx + rdi], xmm0

    movups [rsi + rdi], xmm1

    //right lower segment
    add rdi, rax                            //3*segment_length + i
    
    movups xmm0, xmm2
    paddd xmm0, xmm0
    psubd xmm0, xmm3
    psubd xmm0, [rdx + r8]                  //2*segment_coord - 1 - x[i] .. 2*segment_coord - 1 - x[i+4]
    movups [rdx + rdi], xmm0

    movups xmm0, xmm2
    psubd xmm0, xmm3
    psubd xmm0, [rsi + r8]                  //segment_coord - 1 - y[i] .. segment_coord - 1 - y[i+4]
    movups [rsi + rdi], xmm0

    add rcx, 16
    
    jmp segments_loop

end:
    ret
