;; rdi=pgn: 0~23  0|0000
global ustor7_30_perm_load_page:
ustor_30_perm_load_page:
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

