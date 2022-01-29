.intel_syntax noprefix
.global v_assembly

.text

.align 16

v_assembly:
    mov rdx, qword ptr [rsp]                //x
    //mov edi, dword ptr [rsp + 4]            //y
    //mov rax, qword ptr [rsp + 16]           //segment_length
    //mov rcx, qword ptr [rsp + 24]           //segment_coord
    //mov r8, qword ptr [rsp + 32]            //start
    //mov r9, qword ptr [rsp + 40]            //end

    mov qword ptr [rdx], 0
   // mov qword ptr [edi], 1

    /*cmp rax, 0
    jle end

    movd xmm2, rcx
    pshufd xmm2, xmm2, 0x00                 //segment_coord .. segment_coord

    mov r10, 1
    movd xmm3, r10
    pshufd xmm3, xmm3, 0x00                 //1 .. 1

    add r8, r8
    add r8, r8                              //start*4
    add r9, r9
    add r9, r9                              //end*4

segments_loop:
    cmp r8, r9
    je end

    movups xmm0, [rdx + r8]                 //x[i] .. x[i+3]
    movups xmm1, [rdi + r8]                 //y[i] .. y[i+3]
    
    mov rsi, rax
    add rsi, r8                             //segment_length + i

    //left lower segment
    movups [rdx + r8], xmm1
    movups [rdi + r8], xmm0

    //left upper segment
    movups [rdx + rsi], xmm0

    paddd xmm1, xmm2                        //y[i] + segment_coord .. y[i+3] + segment_coord
    movups [rdi + rsi], xmm1

    //right upper segment
    add rsi, rax                            //2*segment_length + i

    paddd xmm0, xmm2                        //x[i] + segment_coord .. x[i+3] + segment_coord
    movups [rdx + rsi], xmm0

    movups [rdi + rsi], xmm1

    //right lower segment
    add rsi, rax                            //3*segment_length + i
    
    movups xmm0, xmm2
    paddd xmm0, xmm0
    psubd xmm0, xmm3
    psubd xmm0, [rdx + r8]                  //2*segment_coord - 1 - x[i] .. 2*segment_coord - 1 - x[i+4]
    movups [rdx + rsi], xmm0

    movups xmm0, xmm2
    psubd xmm0, xmm3
    psubd xmm0, [rsi + r8]                  //segment_coord - 1 - y[i] .. segment_coord - 1 - y[i+4]
    movups [rdi + rsi], xmm0

    add r8, 16
    
    jmp segments_loop*/

end:
    ret
