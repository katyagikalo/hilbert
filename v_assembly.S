.intel_syntax noprefix
.global v_assembly

.text

.align 16

v_assembly:
    cmp edi, 0
    jle end

    xor eax, eax                //segment degree

    mov dword ptr [esi], 0      //x[0]
    mov dword ptr [edx], 0      //y[0]

    mov dword ptr [esi + 4], 0      //x[1]
    mov dword ptr [edx + 4], 1      //y[1]

    mov dword ptr [esi + 8], 1      //x[2]
    mov dword ptr [edx + 8], 1      //y[2]

    mov dword ptr [esi + 12], 1      //x[3]
    mov dword ptr [edx + 12], 0      //y[3]

degrees_loop:
    inc eax

    cmp eax, edi
    je end
    
    mov r8d, 1                  //segment length
    mov ecx, eax
    imul ecx, 2                  //2 * segment degree
    shl r8d, cl                  //1 << (2 * (segment degree))
    
    mov ecx, eax
    mov r9d, 1                   //segment max coordinate
    shl ecx, cl                  //1 << segment degree
    dec r9d                      //(1 << segment_degree) - 1

    xor ecx, ecx                //counter

segments_loop:
    cmp ecx, r8d
    je degrees_loop

    //left upper segment
    mov r11d, r8d                        //segment length + i
    add r11d, ecx
    imul r11d, 4
    
    mov r10d, dword ptr [esi + 4*ecx]       //x[i]
    mov dword ptr [esi + r11d], r10d

    mov r10d, dword ptr [edx + 4*ecx]       //y[i] + segment max coordinate + 1
    add r10d, ecx
    inc r10d
    mov dword ptr [edx + r11d], r10d

    //right upper segment
    mov r11d, r8d                        //2 * segment length + i
    imul r11d, 2
    add r11d, ecx
    imul r11d, 4

    mov r10d, dword ptr [esi + 4*ecx]       //x[i] + segment max coordinate + 1
    add r10d, ecx
    inc r10d
    mov dword ptr [esi + r11d], r10d

    mov r10d, dword ptr [edx + 4*ecx]       //y[i] + segment max coordinate + 1
    add r10d, ecx
    inc r10d
    mov dword ptr [edx + r11d], r10d

    //left lower segment
    mov r10d, dword ptr [esi + 4*ecx]
    mov r11d, dword ptr [edx + 4*ecx]
    mov dword ptr [esi + 4*ecx], r11d
    mov dword ptr [edx + 4*ecx], r10d

    //right lower segment
    mov r11d, r8d                        //3 * segment length + i
    imul r11d, 3
    add r11d, ecx
    imul r11d, 4

    mov r10d, ecx                        //2 * segment max coordinate + 1 - x[i]
    imul r10d, 2
    inc r10d
    sub r10d, dword ptr [esi + 4*ecx]
    mov dword ptr [esi + r11d], r10d

    mov r10d, ecx                        //segment max coordinate + 1 - y[i]
    sub r10d, dword ptr [edx + 4*ecx]
    mov dword ptr [edx + r11d], r10d

    inc r9d
    
    jmp segments_loop

end:
    ret
