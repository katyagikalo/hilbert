.intel_syntax noprefix
.global v_assembly_multithreaded

.text

.align 16

v_assembly_multithreaded:

    mov rsi, qword ptr [rdi]                //x
    mov rdx, qword ptr [rdi + 8]            //y
    mov rax, qword ptr [rdi + 16]           //segment length
    mov rcx, qword ptr [rdi + 24]            //start
    mov r8, qword ptr [rdi + 32]            //end
    mov r10, qword ptr [rdi + 40]           //segment coordinate

    add rcx, rcx
    add rcx, rcx                              //start * 4

    add r8, r8
    add r8, r8                              //end * 4

    add rax, rax
    add rax, rax                            //segment length * 4

    movd xmm3, r10
    pshufd xmm3, xmm3, 0x00                 //segment coordinate .. segment coordinate
     
    mov r11, 1
    movd xmm2, r11
    pshufd xmm2, xmm2, 0x00                 //1 1 1 1

segments_loop:
    cmp rcx, r8
    je end

    movups xmm0, [rsi + rcx]                 //x[i] .. x[i+3]
    movups xmm1, [rdx + rcx]                 //y[i] .. y[i+3]

    //left lower segment
    movups [rsi + rcx], xmm1
    movups [rdx + rcx], xmm0

    mov r9, rax
    add r9, rcx                             //segment length + i

    //left upper segment
    movups [rsi + r9], xmm0

    paddd xmm1, xmm3                        //y[i] + segment coordinate .. y[i+3] + segment coordinate
    movups [rdx + r9], xmm1

    //right upper segment
    add r9, rax                            //2 * segment length + i

    paddd xmm0, xmm3                        //x[i] + segment coordinate .. x[i+3] + segment coordinate
    movups [rsi + r9], xmm0

    movups [rdx + r9], xmm1

    //right lower segment
    add r9, rax                            //3 * segment length + i

    movups xmm0, xmm3
    paddd xmm0, xmm0
    psubd xmm0, xmm2
    psubd xmm0, [rsi + rcx]                  //2 * segment coordinate - 1 - x[i] .. 2 * segment coordinate - 1 - x[i+3]
    movups [rsi + r9], xmm0

    movups xmm0, xmm3
    psubd xmm0, xmm2
    psubd xmm0, [rdx + rcx]                  //segment coordinate - 1 - y[i] .. segment coordinate - 1 - y[i+3]
    movups [rdx + r9], xmm0

    add r8, 16
    
    jmp segments_loop

end:
    ret
