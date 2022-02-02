.intel_syntax noprefix
.global v_assembly_multithreaded

.text

.align 16

v_assembly_multithreaded:

    mov rsi, qword ptr [rdi]                //x
    mov rdx, qword ptr [rdi + 8]            //y
    mov rax, qword ptr [rdi + 16]           //segment length
    mov rcx, qword ptr [rdi + 24]           //segment coordinate
    mov r8, qword ptr [rdi + 32]            //start
    mov r9, qword ptr [rdi + 40]            //end

    add r8, r8
    add r8, r8                              //start * 4

    add r9, r9
    add r9, r9                              //end * 4

    add rsi, r8
    add rdx, r8

    movd xmm3, rcx
    pshufd xmm3, xmm3, 0x00                 //segment coordinate .. segment coordinate
     
    mov r10, 1
    movd xmm2, r10
    pshufd xmm2, xmm2, 0x00                 //1 1 1 1

segments_loop:
    cmp r8, r9
    je end

    movups xmm0, [rsi]                      //x[i] .. x[i+3]
    movups xmm1, [rdx]                      //y[i] .. y[i+3]

    //left lower segment
    movups [rsi], xmm1
    movups [rdx], xmm0

    mov r10, rax                            //offset = segment length

    //left upper segment
    movups [rsi + r10], xmm0

    paddd xmm1, xmm3
    movups [rdx + r10], xmm1

    //right upper segment
    add r10, r10                            //2 * segment length

    paddd xmm0, xmm3
    movups [rsi + r10], xmm0

    movups [rdx + r10], xmm1

    //right lower segment
    add r10, r10                            //3 * segment length

    movups xmm0, xmm3
    paddd xmm0, xmm0
    psubd xmm0, xmm2
    psubd xmm0, [rsi]
    movups [rsi + r10], xmm0

    movups xmm0, xmm3
    psubd xmm0, xmm2
    psubd xmm0, [rdx]
    movups [rdx + r10], xmm0

    add rsi, 16
    add rdx, 16
    add r8, 16
    
    jmp segments_loop

end:
    ret
