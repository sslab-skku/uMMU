; %macro cmp_and_vmov_ret 5
;     mov r10, %1
;     cmp %2, r10
;     jne %%exit
; 	vmovdqu64 %3{%4}, %5
; 	ret
;     %%exit:
; %endmacro

global rasm_dirjmp_store64
rasm_dirjmp_store64:
	;; set zmm0
	vpbroadcastq zmm0, rsi
	;; Calculate mask register
	mov rcx, rdi
	shr rcx, 0x3				; v8(8byte) index
	and rcx, 0b111
	mov rax, 0x1
	shl rax, cl
	kmovq k1, rax

	shr rdi, 0x6				; zmm(64byte) index

%assign i 4
%assign j 0
%rep 28
    mov r10, %+j
    cmp rdi, r10
    jne .LABEL%+j
	vmovdqu64 zmm%+i{k1}, zmm0
    jmp .exit
.LABEL%+j:
%assign i i+1
%assign j j+1
%endrep
.exit:
    ret

global rasm_dirjmp_load64
rasm_dirjmp_load64:
	mov rcx, rdi
	shr rdi, 0x6				; zmm(64byte) index

%assign i 4
%assign j 0
%rep 28
    mov r10, %+j
    cmp rdi, r10
    jne .LABEL%+j
	vmovdqu64 zmm0, zmm%+i
    jmp .exit
.LABEL%+j:
%assign i i+1
%assign j j+1
%endrep

.exit:

	shr rcx, 0x3				; v8(8byte) index
	and rcx, 0b111
	mov rax, 0x1
	shl rax, cl
	kmovq k1, rax
	vpcompressq zmm0{k1}, zmm0
	vmovq rax, xmm0
	ret
