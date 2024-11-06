global vmi_m2048B_oload
vmi_m2048B_oload:
	mov rcx, rdi
	shr rcx, 0x10
	and rcx, 0x1
	push rcx
	push rdi
	call vmi_m1024B_oload
	pop rdi
	push rax
	add rdi,1024
	call vmi_m1024B_oload
	pop rdi
	pop rcx
	test rcx,rcx
	cmovz rax, rdi
	ret

global vmi_m1024B_oload
vmi_m1024B_oload:
	kxnorq k1,k0,k0
	kmovq k2,k1
	mov rax, rdi
	shr rax, 0x3
	and rax, 0b111				;u64_v8_idx
	vpbroadcastq zmm2, rax
	mov rax, idx_base_arr
	vmovdqu64 zmm1, [rax]
	vpaddq zmm2, zmm2, zmm1
	mov rax, rdi
	and rax, ~0x3ff
	; The vpgatherqq inst resets k1 register, why?
	vpgatherqq zmm0{k1}, [rax + zmm2*8]
	add rax, 512
	vpgatherqq zmm1{k2}, [rax + zmm2*8]

	mov rax, rdi
	shr rax, 6
	; and rax, 0b1111
	vpbroadcastq zmm2, rax
	vpermi2q zmm2, zmm0, zmm1
	vmovq rax, xmm2
	ret

global vmi_m512B_oload
vmi_m512B_oload:
	mov rax, rdi
	and rax, 0b111111
	vpbroadcastq zmm2, rax
	mov rax, idx_base_arr2
	vmovdqu64 zmm1, [rax]
	vpaddq zmm2, zmm2, zmm1
	mov rax, rdi
	and rax, ~0x3ff
	kxnorq k1,k0,k0
	; The vpgatherqq inst resets k1 register, why?
	vpgatherqq zmm0{k1}, [rax + zmm2*1]
	mov rax, rdi
	shr rax, 6
	vpbroadcastq zmm1, rax
	vpermq zmm1, zmm1, zmm0
	vmovq rax, xmm1
	ret

global vmi_m256B_oload
vmi_m256B_oload:
	mov rax, rdi
	and rax, 0b111111
	vpbroadcastq zmm2, rax
	mov rax, idx_base_arr2
	vmovdqu64 zmm1, [rax]
	vpaddq zmm2, zmm2, zmm1
	mov rax, rdi
	and rax, ~0x3ff
	kxnorq k1,k0,k0
	; The vpgatherqq inst resets k1 register, why?
	vpgatherqq zmm0{k1}, [rax + zmm2*1]
	mov rax, rdi
	shr rax, 6
	vpbroadcastq zmm1, rax
	vpermq zmm1, zmm1, zmm0
	vmovq rax, xmm1
	ret

section .bss:
align 64
idx_base_arr:
	dq 0x0
	dq 0x8
	dq 0x10
	dq 0x18
	dq 0x20
	dq 0x28
	dq 0x30
	dq 0x38

align 64
idx_base_arr2:
	dq 0x0
	dq 0x40
	dq 0x80
	dq 0xc0
	dq 0x100
	dq 0x140
	dq 0x180
	dq 0x1c0
