.intel_syntax noprefix
.global v_assembly

.text

.align 16

v_assembly:
    cmp rdi, 0
    jle end

    xor rax, rax                //segment degree

    mov qword ptr [rsi], 0      //x[0]
    mov qword ptr [rdx], 0      //y[0]

    mov qword ptr [rsi + 4], 0      //x[1]
    mov qword ptr [rdx + 4], 1      //y[1]

    mov qword ptr [rsi + 8], 1      //x[2]
    mov qword ptr [rdx + 8], 1      //y[2]

    mov qword ptr [rsi + 12], 1      //x[3]
    mov qword ptr [rdx + 12], 0      //y[3]

degrees_loop:
    inc rax

    cmp rax, rdi
    je end
    
    mov r8, 1                  //segment length
    mov rcx, rax
    imul rcx, 2                  //2 * segment degree
    shl r8, cl                  //1 << (2 * (segment degree))
    
    mov rcx, rax
    mov r9, 1                   //segment max coordinate
    shl r9, cl                  //1 << segment degree
    dec r9                      //(1 << segment_degree) - 1

    xor rcx, rcx                //counter

segments_loop:
    cmp rcx, r8
    je degrees_loop

    //left upper segment
    /*mov r11, r8                        //segment length + i
    add r11, rcx
    imul r11, 4
    
    mov r10, qword ptr [rsi + 4*rcx]       //x[i]
    mov qword ptr [rsi + r11], r10

    mov r10, qword ptr [rdx + 4*rcx]       //y[i] + segment max coordinate + 1
    add r10, rcx
    inc r10
    mov qword ptr [rdx + r11], r10

    //right upper segment
    mov r11, r8                        //2 * segment length + i
    imul r11, 2
    add r11, rcx
    imul r11, 4

    mov r10, qword ptr [rsi + 4*rcx]       //x[i] + segment max coordinate + 1
    add r10, rcx
    inc r10
    mov qword ptr [rsi + r11], r10

    mov r10, qword ptr [rdx + 4*rcx]       //y[i] + segment max coordinate + 1
    add r10, rcx
    inc r10
    mov qword ptr [rdx + r11], r10*/

    //left lower segment
    mov r10, qword ptr [rsi + 4*rcx]
    mov r11, qword ptr [rdx + 4*rcx]
    mov qword ptr [rsi + 4*rcx], r11
    mov qword ptr [rdx + 4*rcx], r10

    /*//right lower segment
    mov r11, r8                        //3 * segment length + i
    imul r11, 3
    add r11, rcx
    imul r11, 4

    mov r10, rcx                        //2 * segment max coordinate + 1 - x[i]
    imul r10, 2
    inc r10
    sub r10, qword ptr [rsi + 4*rcx]
    mov qword ptr [rsi + r11], r10

    mov r10, rcx                        //segment max coordinate + 1 - y[i]
    sub r10, qword ptr [rdx + 4*rcx]
    mov qword ptr [rdx + r11], r10*/

    inc rcx
    
    jmp segments_loop

end:
    ret
