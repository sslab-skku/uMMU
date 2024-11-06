global vmi_ZMMx28_vmov_oload
vmi_ZMMx28_vmov_oload:
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

global vmi_ZMMx14_vmov_oload
vmi_ZMMx14_vmov_oload:
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
%rep 14
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


global vmi_ZMMx28_vmov_ostore
vmi_ZMMx28_vmov_ostore:
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

;; Setup mask register
global vmi_setup_mask_regs
vmi_setup_mask_regs:
	mov rax, 1
%assign i 1
%rep 7
	kmovq k%+i, rax
	shl rax, 0x1
%assign i i+1
%endrep
	ret

%macro SetMaskRegs 1
    push %1
%assign i 1
%rep 7
	kmovq k%+i, %1
	shl %1, 0x1
%assign i i+1
%endrep
    pop %1
%endmacro

;; rdi=pgn: 0~23  0|0000
global zmm7_30_perm_load_page:
zmm7_30_perm_load_page:
    call vmi_setup_mask_regs
    ; zmm31 <- perm2iq zmm[7:22]
    vpbroadcastq zmm31, rdi
    vpbroadcastq zmm1, rdi
%assign i1 7
%assign i2 15
%assign ik 1
%rep 7
	vpermi2q zmm31{k%+ik}, zmm%+i1, zmm%+i2
%assign i1 i1+1
%assign i2 i2+1
%assign ik ik+1
%endrep
	kshiftlq k7, k7, 1
	vpermi2q zmm31{k7}, zmm%+i1, zmm%+i2
    kshiftrq k7, k7, 1

    ; zmm1 <- permq zmm[23:30]
%assign i1 23
%assign ik 1
%rep 7
	vpermq zmm1{k%+ik}, zmm1, zmm%+i1
%assign i1 i1+1
%assign ik ik+1
%endrep
	kshiftlq k7, k7, 1
	vpermq zmm1{k7}, zmm1, zmm%+i1

    mov rax, 0b11111111
    xor r10, r10
    bt rdi, 4
    cmovc r10, rax
    kmovq k1, r10
    vmovdqu64 zmm31{k1}, zmm1
    ret

;; Store zmm31
;; rdi=pgn 00|000
global zmm7_30_perm_store_page:
zmm7_30_perm_store_page:
    mov rcx, rdi
    and rcx, 0b11111
    mov r10, 1
    shl r10, cl
    kmovq k1, r10

    ; i1 7 ~ 30
%assign i1 7
%rep 3

%assign i2 0
%rep 8
    mov r10, i2
    vpbroadcastq zmm1, r10
    vpermq zmm%+i1{k1}, zmm1, zmm31
%assign i1 i1+1
%assign i2 i2+1
%endrep
    kshiftrq k1, k1, 8

%endrep
    ret

global zmm7_30_perm_load:
zmm7_30_perm_load:
    push rdi
    shr rdi, 6
    call zmm7_30_perm_load_page
    pop rdi
    mov rcx, rdi
    and rcx, 0b111111           ; rcx=64byte offset
    shr rcx, 0x3                ; rcx=8bytex8 idx
    ; mov rax, 0x1
    ; shl rax, cl
    ; kmovq k1, rax
    vpbroadcastq zmm0, rcx
    vpermq zmm0, zmm0, zmm31
    ; vpcompressq zmm0{k1}, zmm31
	vmovq rax, xmm0
    ret

global zmm7_30_perm_store:
zmm7_30_perm_store:
    push rdi
    push rsi
    shr rdi, 6
    call zmm7_30_perm_load_page
    pop rsi
    pop rdi

    vpbroadcastq zmm1, rsi
    push rdi
    mov rcx, rdi
    and rcx, 0b111111           ; rcx=64byte offset
    shr rcx, 0x3                ; rcx=8x8 idx
    mov rax, 0x1
    shl rax, cl
    kmovq k1, rax
    vmovdqu64 zmm31{k1}, zmm1
    pop rdi
    shr rdi, 6

    call zmm7_30_perm_store_page
    ret


global vmi_ZMMx28_vpermi2q_oload
vmi_ZMMx28_vpermi2q_oload:
	call vmi_setup_mask_regs
;; rcx = zmm_v8_idx
	mov rcx, rdi
	shr rcx, 0x3
	vpbroadcastq zmm1, rcx
	vpbroadcastq zmm2, rcx
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
	mov rcx, rdi
	shr rcx, 0x7
	vmovq xmm0, rcx
	vpermi2q zmm0, zmm1, zmm2
	vmovq rax, xmm0
	ret

global vmi_ZMMx14_vpermi2q_oload
vmi_ZMMx14_vpermi2q_oload:
	call vmi_setup_mask_regs
    ;; rcx = zmm_v8_idx
	mov rcx, rdi
	shr rcx, 0x3
	vpbroadcastq zmm1, rcx
%assign i1 4
%assign i2 i1+1
%assign ik 1
%rep 7
	vpermi2q zmm1{k%+ik}, zmm%+i1, zmm%+i2
%assign i1 i1+2
%assign i2 i2+2
%assign ik ik+1
%endrep
	;; rdi = zmm_and_zmm_idx
	shr rdi, 0x7
	vpbroadcastq zmm2, rdi
	vpermq zmm0, zmm2, zmm1
	vmovq rax, xmm0
	ret

global vmi_ZMMx14_vpermq_oload
vmi_ZMMx14_vpermq_oload:
	call vmi_setup_mask_regs
    ;; rcx = zmm_v8_idx
	mov rcx, rdi
	shr rcx, 0x3
	vpbroadcastq zmm0, rcx
%assign i1 4
%assign ik 1
%rep 7
	vpermq zmm1{k%+ik}, zmm0, zmm%+i1
%assign i1 i1+1
%assign ik ik+1
%endrep
	kshiftlq k7,k7,1
	vpermq zmm1{k7}, zmm0, zmm%+i1
	kshiftrq k7,k7,1
%assign i1 i1+1

%assign ik 1
%rep 6
	vpermq zmm2{k%+ik}, zmm0, zmm%+i1
%assign i1 i1+1
%assign ik ik+1
%endrep

	;; rdi = zmm_idx
	shr rdi, 0x6
	vpbroadcastq zmm0, rdi
	vpermi2q zmm0, zmm1, zmm2
	vmovq rax, xmm0
	ret
