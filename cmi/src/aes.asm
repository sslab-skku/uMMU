;; rax <--(Decrypt)-- [rdi]
global aes_load
aes_load:
	mov r10, rdi
	shr r10, 0x3
	and r10, 0x1
	shr rdi, 0x4
	shl rdi, 0x4
	vmovdqu64 xmm14, [rdi]
	call aes_decrypt_xmm14
	cmp r10, 0x0
	jne .ELSE
.IF:
	pextrq rax, xmm14, 0x0
	jmp .FI
.ELSE:
	pextrq rax, xmm14, 0x1
.FI:
	ret

;; decrypt xmm14
aes_decrypt_xmm14:
;; Round 0
	vextracti128 xmm0, ymm15, 0x1
	pxor xmm14, xmm0
;; Rount 1~9
%assign i 14
%rep 9
	vextracti128 xmm0, ymm%+i, 0x1
    vaesimc xmm0, xmm0
	aesdec xmm14, xmm0
%assign i i-1
%endrep
;; Round 10
	vextracti128 xmm0, ymm5, 0x1
	aesdeclast xmm14, xmm0
	ret
;; 8Byte
global aes_store
aes_store:
	mov r10, rdi
	shr r10, 0x3
	and r10, 0x1
	shr rdi, 0x4
	shl rdi, 0x4
	vmovdqu64 xmm14, [rdi]
	call aes_decrypt_xmm14
	cmp r10, 0x0
	jne .ELSE
.IF:
	pinsrq xmm14, rsi, 0x0
	jmp .FI
.ELSE:
	pinsrq xmm14, rsi, 0x1
.FI:
	call aes_encrypt_xmm14
	vmovdqu64 [rdi], xmm14
	ret

aes_encrypt_xmm14:
;; Round 0
	vextracti128 xmm0, ymm5, 0x1
	pxor xmm14, xmm0
;; Rount 1~9
%assign i 6
%rep 9
	vextracti128 xmm0, ymm%+i, 0x1
	aesenc xmm14, xmm0
%assign i i+1
%endrep
;; Round 10
	vextracti128 xmm0, ymm15, 0x1
	aesenclast xmm14, xmm0
	ret

global aes_populate_key
aes_populate_key:
%assign i 5
%rep 11
	mov r10, 0xfcb3bf52ab70db11
	pinsrq xmm15, r10, 0x0
	mov r10, 0xea7972a48c12d064
	pinsrq xmm15, r10, 0x1
    vinserti128 ymm%+i, ymm%+i, xmm15, 0x1
%assign i i+1
%endrep
	ret
