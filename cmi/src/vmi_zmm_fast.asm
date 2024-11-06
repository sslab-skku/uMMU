global vmi_ZMMx28_indjmp_store
vmi_ZMMx28_indjmp_store:					; rdi: addr, rsi: value
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
	shl    rdi,0x3
	lea    rax,[rel .JMP_ENTRY]
	add    rax,rdi
	jmp    rax
.JMP_ENTRY:
%assign i 4
%rep 28
	vmovdqu64 zmm%+i{k1}, zmm0
	ret
	ret
%assign i i+1
%endrep


global vmi_ZMMx28_indjmp_load
vmi_ZMMx28_indjmp_load:
	mov rcx, rdi
	shr rdi, 0x6				; zmm(64byte) index
	call .LOAD_TO_ZMM0
	shr rcx, 0x3				; v8(8byte) index
	and rcx, 0b111
	mov rax, 0x1
	shl rax, cl
	kmovq k1, rax
	vpcompressq zmm0{k1}, zmm0
	vmovq rax, xmm0
	ret
.LOAD_TO_ZMM0:
	shl    rdi,0x3
	lea    rax,[rel .JMP_ENTRY]
	add    rax,rdi
	jmp    rax
.JMP_ENTRY:
%assign i 4
%rep 28
	vmovdqu64 zmm0, zmm%+i
	ret
	ret
%assign i i+1
%endrep
