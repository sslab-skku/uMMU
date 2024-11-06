global rasm_cmov_load64
rasm_cmov_load64:
	mov rcx, rdi
	;; rdi = zmm_idx
	shr rdi, 0x6
	;; rcx = zmm_v8_idx
	shr rcx, 0x3
	and cl, 0b111
	;; rax = idx_mask
	mov rax, 0x1
	shl rax, cl
	add rdi, 1
%assign i 4
%rep 28
	;; k1 = (zmm_idx==0)? idx_mask : 0
	xor r10, r10
	sub rdi, 1
	cmovz r10, rax
	kmovq k1, r10
	;; read the data into zmm0
	vmovdqu64 zmm0{k1}, zmm%+i
%assign i i+1
%endrep
	;; Now, zmm0 holds the data
	kmovq k1, rax
	vpcompressq zmm0{k1}, zmm0
	vmovq rax, xmm0
	ret


global rasm_cmov_store64
rasm_cmov_store64:
	mov rcx, rdi
	;; rdi = zmm_idx
	shr rdi, 0x6
	;; rcx = zmm_v8_idx
	shr rcx, 0x3
	and cl, 0b111
	;; rax = idx_mask
	mov rax, 0x1
	shl rax, cl
	vpbroadcastq zmm0, rsi
	add rdi, 1
%assign i 4
%rep 28
	xor r10,r10
	sub rdi, 1
	cmovz r10, rax
	kmovq k1, r10
	;; store the data into zmmXX
	vmovdqu64 zmm%+i{k1}, zmm0
%assign i i+1
%endrep
	ret

global rasm_cmov_store32
rasm_cmov_store32:
	mov rcx, rdi
	;; rdi = zmm_idx
	shr rdi, 0x6
	;; rcx = zmm_v4_idx
	shr rcx, 0x2
	and cl, 0b111
	;; rax = idx_mask
	mov rax, 0x1
	shl rax, cl
	vpbroadcastq zmm0, rsi
	add rdi, 1
%assign i 4
%rep 28
	xor r10,r10
	sub rdi, 1
	cmovz r10, rax
	kmovq k1, r10
	;; store the data into zmmXX
	vmovdqu64 zmm%+i{k1}, zmm0
%assign i i+1
%endrep
	ret

