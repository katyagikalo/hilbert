.intel_syntax noprefix
.global v_assembly_multithreaded

.text

.align 16

v_assembly_multithreaded:
    mov dword ptr [rsp], 0
    ret




    /*cmp rdi, 0                              //degree
    jle end

    xor rax, rax                            //segment degree

    mov dword ptr [rsi], 0                  //x[0]
    mov dword ptr [rdx], 0                  //y[0]

    mov dword ptr [rsi + 4], 0              //x[1]
    mov dword ptr [rdx + 4], 1              //y[1]

    mov dword ptr [rsi + 8], 1              //x[2]
    mov dword ptr [rdx + 8], 1              //y[2]

    mov dword ptr [rsi + 12], 1             //x[3]
    mov dword ptr [rdx + 12], 0             //y[3]

    mov r10, 1
    movd xmm2, r10
    pshufd xmm2, xmm2, 0x00                 //1 1 1 1

degrees_loop:
    inc rax

    cmp rax, rdi
    je end
    
    mov rcx, rax

    mov r9, 1
    shl r9, cl                              //1 << segment degree
    movd xmm3, r9
    pshufd xmm3, xmm3, 0x00                 //segment coordinate .. segment coordinate

    mov r8, 1                               //segment length
    add rcx, rcx
    add rcx, 2
    shl r8, cl                              //1 << (2 * (segment degree) + 2)

    xor rcx, rcx                            //loop counter

segments_loop:
    cmp rcx, r8
    je degrees_loop

    movups xmm0, [rsi + rcx]                //x[i] .. x[i+3]
    movups xmm1, [rdx + rcx]                //y[i] .. y[i+3]

    //left lower segment
    movups [rsi + rcx], xmm1
    movups [rdx + rcx], xmm0

    mov r9, r8
    add r9, rcx                             //segment length + i

    //left upper segment
    movups [rsi + r9], xmm0

    paddd xmm1, xmm3                        //y[i] + segment coordinate .. y[i+3] + segment coordinate
    movups [rdx + r9], xmm1

    //right upper segment
    add r9, r8                              //2 * segment length + i

    paddd xmm0, xmm3                        //x[i] + segment coordinate .. x[i+3] + segment coordinate
    movups [rsi + r9], xmm0

    movups [rdx + r9], xmm1

    //right lower segment
    add r9, r8                              //3 * segment length + i

    movups xmm0, xmm3
    paddd xmm0, xmm0
    psubd xmm0, xmm2
    psubd xmm0, [rsi + rcx]                 //2 * segment coordinate - 1 - x[i] .. 2 * segment coordinate - 1 - x[i+3]
    movups [rsi + r9], xmm0

    movups xmm0, xmm3
    psubd xmm0, xmm2
    psubd xmm0, [rdx + rcx]                 //segment coordinate - 1 - y[i] .. segment coordinate - 1 - y[i+3]
    movups [rdx + r9], xmm0

    add rcx, 16
    
    jmp segments_loop

end:
    ret*/
