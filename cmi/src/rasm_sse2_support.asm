global rasm_sse2_support_copy_from_shelter_regs:
rasm_sse2_support_copy_from_shelter_regs:
    mov rax, 0b1100
    kmovq k1,rax
    kshiftlq k2,k1,2
    kshiftlq k3,k1,4
    vpcompressq zmm4{k1}, zmm0
    vpcompressq zmm5{k2}, zmm0
    vpcompressq zmm6{k3}, zmm0
    vpcompressq zmm7{k1}, zmm1
    vpcompressq zmm8{k2}, zmm1
    vpcompressq zmm9{k3}, zmm1
    vpcompressq zmm10{k1}, zmm2
    vpcompressq zmm11{k2}, zmm2
    vpcompressq zmm12{k3}, zmm2
    vpcompressq zmm13{k1}, zmm3
    vpcompressq zmm14{k2}, zmm3
    vpcompressq zmm15{k3}, zmm3
    ret

global rasm_sse2_support_copy_to_shelter_regs:
rasm_sse2_support_copy_to_shelter_regs:
    vinserti64x2 zmm0, zmm0, xmm4, 1
    vinserti64x2 zmm0, zmm0, xmm5, 2
    vinserti64x2 zmm0, zmm0, xmm6, 3
    vinserti64x2 zmm1, zmm1, xmm7, 1
    vinserti64x2 zmm1, zmm1, xmm8, 2
    vinserti64x2 zmm1, zmm1, xmm9, 3
    vinserti64x2 zmm2, zmm2, xmm10, 1
    vinserti64x2 zmm2, zmm2, xmm11, 2
    vinserti64x2 zmm2, zmm2, xmm12, 3
    vinserti64x2 zmm3, zmm3, xmm13, 1
    vinserti64x2 zmm3, zmm3, xmm14, 2
    vinserti64x2 zmm3, zmm3, xmm15, 3
    ret

global rasm_sse_support_copy_from_shelter_regs:
rasm_sse_support_copy_from_shelter_regs:
    mov rax, 0b1100
    kmovq k1,rax
    kshiftlq k2,k1,2
    kshiftlq k3,k1,4
    vpcompressq zmm4{k1}, zmm0
    vpcompressq zmm5{k2}, zmm0
    vpcompressq zmm6{k3}, zmm0
    vpcompressq zmm7{k1}, zmm1
    vpcompressq zmm8{k2}, zmm1
    ret

global rasm_sse_support_copy_to_shelter_regs:
rasm_sse_support_copy_to_shelter_regs:
    vinserti64x2 zmm0, zmm0, xmm4, 1
    vinserti64x2 zmm0, zmm0, xmm5, 2
    vinserti64x2 zmm0, zmm0, xmm6, 3
    vinserti64x2 zmm1, zmm1, xmm7, 1
    vinserti64x2 zmm1, zmm1, xmm8, 2
    ret

