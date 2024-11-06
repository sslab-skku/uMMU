%macro ROUND_INIT 2
    vextracti64x2 xmm0, %1, %2
%endmacro

%macro DEC_ROUND 2
    vextracti64x2 xmm0, %1, %2
    aesimc xmm0, xmm0
    aesdec xmm3, xmm0
%endmacro

%macro DEC_ROUND_LAST 2
    vextracti64x2 xmm0, %1, %2
    aesdeclast xmm3, xmm0
%endmacro

%macro ENC_ROUND 2
    vextracti64x2 xmm0, %1, %2
    aesenc xmm3, xmm0
%endmacro

%macro ENC_ROUND_LAST 2
    vextracti64x2 xmm0, %1, %2
    aesenclast xmm3, xmm0
%endmacro

global aes_xmm3_load
aes_xmm3_load:
	mov r10, rdi
	shr r10, 0x3
	and r10, 0x1
	shr rdi, 0x4
	shl rdi, 0x4
	vmovdqu64 xmm3, [rdi]
	call aes_decrypt_xmm3
	cmp r10, 0x0
	jne .ELSE
.IF:
	pextrq rax, xmm3, 0x0
	jmp .FI
.ELSE:
	pextrq rax, xmm3, 0x1
.FI:
	ret

;; 8Byte
global aes_xmm3_store
aes_xmm3_store:
	mov r10, rdi
	shr r10, 0x3
	and r10, 0x1
	shr rdi, 0x4
	shl rdi, 0x4
	vmovdqu64 xmm3, [rdi]
	call aes_decrypt_xmm3
	cmp r10, 0x0
	jne .ELSE
.IF:
	pinsrq xmm3, rsi, 0x0
	jmp .FI
.ELSE:
	pinsrq xmm3, rsi, 0x1
.FI:
	call aes_encrypt_xmm3
	vmovdqu64 [rdi], xmm3
	ret


global aes_decrypt_xmm3
aes_decrypt_xmm3:
    ROUND_INIT zmm6, 0x2
    DEC_ROUND zmm6, 0x1

    DEC_ROUND zmm6, 0x0
    DEC_ROUND zmm5, 0x3
    DEC_ROUND zmm5, 0x2
    DEC_ROUND zmm5, 0x1

    DEC_ROUND zmm5, 0x0
    DEC_ROUND zmm4, 0x3
    DEC_ROUND zmm4, 0x2
    DEC_ROUND zmm4, 0x1
    DEC_ROUND_LAST zmm4, 0x0
	ret

global aes_encrypt_xmm3
aes_encrypt_xmm3:
    ROUND_INIT zmm4, 0x0
    ENC_ROUND zmm4, 0x1
    ENC_ROUND zmm4, 0x2
    ENC_ROUND zmm4, 0x3

    ENC_ROUND zmm5, 0x0
    ENC_ROUND zmm5, 0x1
    ENC_ROUND zmm5, 0x2
    ENC_ROUND zmm5, 0x3

    ENC_ROUND zmm6, 0x0
    ENC_ROUND zmm6, 0x1
    ENC_ROUND_LAST zmm6, 0x2
	ret

global aes_populate_key_zmm456
aes_populate_key_zmm456:
    vpbroadcastq zmm4, 0xfcb3bf52ab70db11
    vpbroadcastq zmm5, 0xea7972a48c12d064
    vpbroadcastq zmm6, 0xf2fa111491866d6a
    ret
