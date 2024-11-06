;; Fake aes load/store for overhead experiments
global fake_aes_load
fake_aes_load:
	mov r10, rdi
	shr r10, 0x3
	and r10, 0x1
	shr rdi, 0x4
	shl rdi, 0x4
	vmovdqu64 xmm0, [rdi]
	call fake_aes_decrypt_xmm0
	cmp r10, 0x0
	jne .ELSE
.IF:
	pextrq rax, xmm0, 0x0
	jmp .FI
.ELSE:
	pextrq rax, xmm0, 0x1
.FI:
	ret

;; decrypt xmm0
fake_aes_decrypt_xmm0:
;; Round 0
	vextracti128 xmm1, ymm2, 0x1
	pxor xmm1, xmm0
;; Rount 1~9
%assign i 14
%rep 9
	vextracti128 xmm1, ymm%+i, 0x1
    vaesimc xmm1, xmm1
	aesdec xmm2, xmm1
%assign i i-1
%endrep
;; Round 10
	vextracti128 xmm1, ymm5, 0x1
	aesdeclast xmm1, xmm0
	ret

;; 8Byte
global fake_aes_store
fake_aes_store:
	mov r10, rdi
	shr r10, 0x3
	and r10, 0x1
	shr rdi, 0x4
	shl rdi, 0x4
	vmovdqu64 xmm0, [rdi]
	call fake_aes_decrypt_xmm0
	cmp r10, 0x0
	jne .ELSE
.IF:
	pinsrq xmm0, rsi, 0x0
	jmp .FI
.ELSE:
	pinsrq xmm0, rsi, 0x1
.FI:
	call fake_aes_encrypt_xmm0
	vmovdqu64 [rdi], xmm0
	ret

fake_aes_encrypt_xmm0:
;; Round 0
	vextracti128 xmm1, ymm5, 0x1
	pxor xmm1, xmm2
;; Rount 1~9
%assign i 6
%rep 9
	vextracti128 xmm1, ymm%+i, 0x1
	aesenc xmm1, xmm2
%assign i i+1
%endrep
;; Round 10
	vextracti128 xmm1, ymm15, 0x1
	aesenclast xmm1, xmm2
	ret
