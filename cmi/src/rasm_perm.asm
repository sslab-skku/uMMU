_setup_mask_regs:
	mov rax, 1
%assign i 1
%rep 7
	kmovq k%+i, rax
	shl rax, 0x1
%assign i i+1
%endrep
	ret


global rasm_perm_load64
rasm_perm_load64:
	call _setup_mask_regs
;; rcx = zmm_v8_idx
	mov rcx, rdi
	shr rcx, 0x3
	vpbroadcastq zmm1, rcx
	vpbroadcastq zmm2, rcx
%if 1
%assign i1 4
%assign i2 i1+1
%assign ik 1
%rep 7
	vpermi2q zmm1{k%+ik}, zmm%+i1, zmm%+i2
%assign i1 i1+2
%assign i2 i2+2
%assign ik ik+1
%endrep
	kshiftlq k7, k7, 1
	vpermi2q zmm1{k7}, zmm%+i1, zmm%+i2
%assign i1 i1+2
%assign i2 i2+2
%assign ik 1
%rep 6
	vpermi2q zmm2{k%+ik}, zmm%+i1, zmm%+i2
%assign i1 i1+2
%assign i2 i2+2
%assign ik ik+1
%endrep


%else
;; Version 2 -> no performance difference
; %assign i1 4
; %assign i2 5
; %assign j1 20
; %assign j2 21
; %assign ik 1
; %rep 6
; 	vpermi2q zmm1{k%+ik}, zmm%+i1, zmm%+i2
;     vpermi2q zmm2{k%+ik}, zmm%+j1, zmm%+j2
; %assign i1 i1+2
; %assign i2 i2+2
; %assign j1 j1+2
; %assign j2 j2+2
; %assign ik ik+1
; %endrep
;     vpermi2q zmm1{k7}, zmm16, zmm17
;     kshiftlq k7, k7, 1
; 	vpermi2q zmm1{k7}, zmm18, zmm19
%endif
	mov rcx, rdi
	shr rcx, 0x7
	vmovq xmm0, rcx
	vpermi2q zmm0, zmm1, zmm2
	vmovq rax, xmm0
	ret
