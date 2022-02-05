.intel_syntax noprefix
.global v_assembly_multithreaded

.text

.align 16

v_assembly_multithreaded:

    mov rsi, qword ptr [rdi]                //x
    mov rdx, qword ptr [rdi + 8]            //y
    mov rax, qword ptr [rdi + 16]           //segment length
    mov r8, qword ptr [rdi + 24]            //start
    mov r9, qword ptr [rdi + 32]            //end
    mov rcx, qword ptr [rdi + 40]           //segment coordinate

    add r8, r8
    add r8, r8                              //start * 4

    add r9, r9
    add r9, r9                              //end * 4

    add rax, rax
    add rax, rax                            //segment length * 4

    movd xmm3, rcx
    pshufd xmm3, xmm3, 0x00                 //segment coordinate .. segment coordinate
     
    mov r10, 1
    movd xmm2, r10
    pshufd xmm2, xmm2, 0x00                 //1 1 1 1

segments_loop:
    cmp r8, r9
    je end

    /*movups xmm0, [rsi + r8]                 //x[i] .. x[i+3]
    movups xmm1, [rdx + r8]                 //y[i] .. y[i+3]

    //left lower segment
    movups [rsi + r8], xmm1
    movups [rdx + r8], xmm0

    mov r10, rax
    add r10, r8                             //segment length + i

    //left upper segment
    movups [rsi + r10], xmm0

    paddd xmm1, xmm3                        //y[i] + segment coordinate .. y[i+3] + segment coordinate
    movups [rdx + r10], xmm1

    //right upper segment
    add r10, rax                            //2 * segment length + i

    paddd xmm0, xmm3                        //x[i] + segment coordinate .. x[i+3] + segment coordinate
    movups [rsi + r10], xmm0

    movups [rdx + r10], xmm1

    //right lower segment
    add r10, rax                            //3 * segment length + i

    movups xmm0, xmm3
    paddd xmm0, xmm0
    psubd xmm0, xmm2
    psubd xmm0, [rsi + r8]                  //2 * segment coordinate - 1 - x[i] .. 2 * segment coordinate - 1 - x[i+3]
    movups [rsi + r10], xmm0

    movups xmm0, xmm3
    psubd xmm0, xmm2
    psubd xmm0, [rdx + r8]                  //segment coordinate - 1 - y[i] .. segment coordinate - 1 - y[i+3]
    movups [rdx + r10], xmm0*/

    add r8, 16
    
    jmp segments_loop

end:
    ret
