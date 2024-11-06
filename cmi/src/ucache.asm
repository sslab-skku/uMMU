%macro varShiftLeft 2
    push rcx
    mov rcx, %2
    shl %1, cl
    pop rcx
%endmacro
;; ex)
;; ucache64x16_store_64 %dst, %val
%macro ucache64x16_store_64 2
    push rcx
    push rax
;; calculate mask
    mov rcx, %1
    mov rax, 0x1
    shl rax, cl
    kmovq k1, rax
    kshiftrq k2, k1

;; store
    vmovq xmm0, %2
    vpexpandq zmm2{k1}, zmm0
    vpexpandq zmm3{k2}, zmm0
    pop rax
    pop rcx
%endmacro

;; ucache64x16_load_64 %dst, %val
%macro ucache64x16_load_64 2
    push rcx
    push rax
;; calculate mask
    mov rcx, %1
    mov rax, 0x1
    shl rax, cl
    kmovq k1, rax
    kshiftrq k2, k1

;; store
    vmovq xmm0, %2
    vpexpandq zmm2{k1}, zmm0
    vpexpandq zmm3{k2}, zmm0
    pop rax
    pop rcx
%endmacro

%macro uStorage1024x12_load_page 2

%endmacro

%macro uStorage_zmm4_11_load_512 1
    mov rax, 0b01010101
    kmovq k1, rax
    kshiftlq k2, k1
    vpbroadcastq zmm0{k1}, %1
%endmacro

global uCache_load_page:
uCache_load_page:
    ; zmm4[3:1] -> xmm5~7
    vextracti64x2 xmm5, zmm4, 1
    vextracti64x2 xmm6, zmm4, 2
    vextracti64x2 xmm7, zmm4, 3

    ; zmm4[3:1] <- xmm5~7
    vinserti64x2 zmm4, xmm5, 1
    vinserti64x2 zmm4, xmm6, 2
    vinserti64x2 zmm4, xmm7, 3
    ret

;; Cache the 512 page at pgn(rdi) into uCache(zmm2)
;; rdi: pgn
global uStorage512x28_cache_page
uStorage512x28_cache_page:
    mov rax, 0b11
    kmovq k1, rax               ; k1: 00|00|00|11
    shl rax, 2
    kmovq k2, rax               ; k2: 00|00|11|00
    shl rax, 2
    kmovq k3, rax               ; k3: 00|11|00|00
    shl rax, 2
    kmovq k4, rax               ; k4: 11|00|00|00

    ; ex) rdi=1
    ;  => zmm0: 23|23|23|23 perm indices
    mov rsi, rdi
    shl rsi, 1
    mov rax, 0b01010101
    kmovq k6, rax               ; k6: 01|01|01|01
    kshiftlq k7, k1             ; k7: 10|10|10|10
    vpbroadcastq zmm0{k6}, rsi
    inc rsi
    vpbroadcastq zmm0{k7}, rsi

    ; rsi: pgn>>3 -> 00,01,10
    mov rsi, rdi
    shr rsi, 3
    mov r10, 0b11111111

    ; ex) if rsi==01 --> k5=0, k6=0b11..11, k7=0
%assign i 0
%assign j 5
%rep 3
    mov rax, 0
    cmp rsi, %i
    cmove rax, r10
    kmovq k%+j, rax
%assign i i+1
%assign j j+1
%endrep

    ; pgn 0~7
    vpermi2q zmm0{k1}, zmm7, zmm9
    vpermi2q zmm0{k2}, zmm10, zmm11
    vpermi2q zmm0{k3}, zmm12, zmm13
    vpermi2q zmm0{k4}, zmm14, zmm15
    vmovdqu64 zmm2{k5}, zmm0

    ; pgn 8~15
    vpermi2q zmm0{k1}, zmm16, zmm17
    vpermi2q zmm0{k2}, zmm18, zmm19
    vpermi2q zmm0{k3}, zmm20, zmm21
    vpermi2q zmm0{k4}, zmm22, zmm23
    vmovdqu64 zmm2{k6}, zmm0

    ; pgn 16~23
    vpermi2q zmm0{k1}, zmm24, zmm25
    vpermi2q zmm0{k2}, zmm26, zmm27
    vpermi2q zmm0{k3}, zmm28, zmm29
    vpermi2q zmm0{k4}, zmm30, zmm31
    vmovdqu64 zmm2{k7}, zmm0
    ret

;; uCache(zmm2) -> uStorage
global uStorage512x28_flush_page
uStorage512x28_flush_page:
    mov rax, 0b01010101
    kmovq k6, rax               ; k6: 01|01|01|01
    kshiftlq k7, k1             ; k7: 10|10|10|10
    mov rax, 0b11
    kmovq k1, rax               ; k1: 00|00|00|11
    shl rax, 2
    kmovq k2, rax               ; k2: 00|00|11|00
    shl rax, 2
    kmovq k3, rax               ; k3: 00|11|00|00
    shl rax, 2
    kmovq k4, rax               ; k4: 11|00|00|00

    ; rsi: 2*pgn
    mov rsi, rdi
    shl rsi, 1

    ; zmm0: 23|23|23|23
    vpbroadcastq zmm0{k6}, rsi
    inc rsi
    vpbroadcastq zmm0{k7}, rsi

    ; Calc k5,k6,k7
    ; rsi: pgn>>3 -> 00,01,10
    mov rsi, rdi
    shr rsi, 3
    mov r10, 0b11111111

    ; ex) if rsi==01 --> k5=0, k6=0b11..11, k7=0
%assign i 0
%assign j 5
%rep 3
    mov rax, 0
    cmp rsi, %i
    cmove rax, r10
    kmovq k%+j, rax
%assign i i+1
%assign j j+1
%endrep

    ; pgn 0~7
    vpermi2q zmm0{k1}, zmm8, zmm9
    vpermi2q zmm0{k2}, zmm10, zmm11
    vpermi2q zmm0{k3}, zmm12, zmm13
    vpermi2q zmm0{k4}, zmm14, zmm15
    vmovdqu64 zmm2{k5}, zmm0

    ; pgn 8~15
    vpermi2q zmm0{k1}, zmm16, zmm17
    vpermi2q zmm0{k2}, zmm18, zmm19
    vpermi2q zmm0{k3}, zmm20, zmm21
    vpermi2q zmm0{k4}, zmm22, zmm23
    vmovdqu64 zmm2{k6}, zmm0

    ; pgn 16~23
    vpermi2q zmm0{k1}, zmm24, zmm25
    vpermi2q zmm0{k2}, zmm26, zmm27
    vpermi2q zmm0{k3}, zmm28, zmm29
    vpermi2q zmm0{k4}, zmm30, zmm31
    vmovdqu64 zmm2{k7}, zmm0
    ret


;; pgn: |bn(Bank Num)|rn(Reg Num)|
;; pgn: |010|10|
uStorage512x28_cache:
    ; r8: reg index(2bit)
    mov r8, rdi
    and r8, 0b11
    ; r9: bank index(3bit)
    mov r9, rdi
    shr r9, 2
    ; r10: 0b11..11
    mov r10, 0b11111111

    ; k(pgn+1) = 0b1111111
%assign i 0
%assign j 1
%rep 4
    mov rax, 0
    cmp r8, %i
    cmove rax, r10
    kmovq k%+j, rax
%assign i i+1
%assign j j+1
%endrep

    ; k5,k6,k7 bank number(0~2)
%assign i 0
%assign k 5
%rep 3
    mov rax, 0
    cmp r9, %i
    cmove rax, r10
    kmovq k%+k, rax
%assign i i+1
%assign k k+1
%endrep

    vmovdqu64 zmm0{k1},
    ; TODO
    ret

;; pgn: |bn(Bank Num)|rn(Reg Num)|
;; pgn: |01|10|
uStorage1024x12_cache:
    ; r8: reg index(2bit)
    mov r8, rdi
    and r8, 0b11
    ; r9: bank index(3bit)
    mov r9, rdi
    shr r9, 2
    ; r10: 0b11..11
    mov r10, 0b11111111

    ; k(pgn+1) = 0b1111111
%assign i 0
%assign j 1
%rep 4
    mov rax, 0
    cmp r8, %i
    cmove rax, r10
    kmovq k%+j, rax
%assign i i+1
%assign j j+1
%endrep

    ; k5,k6,k7 bank number(0~2)
%assign i 0
%assign k 5
%rep 3
    mov rax, 0
    cmp r9, %i
    cmove rax, r10
    kmovq k%+k, rax
%assign i i+1
%assign k k+1
%endrep

    vmovdqu64 zmm0{k1},



;; ex) ucache_extract_i128 xmm0, 7
%macro ucache_extract_i128 2
%if %2 < 4
    vextracti64x2 %1, zmm2, %2
%else
    vextracti64x2 %1, zmm3, %2
%endif
%endmacro

;; ex) ucache_insert_i128 xmm0, 7
%macro ucache_insert_i128 2
%if %2 < 4
    vinserti64x2 zmm2, zmm2, %1, %2
%else
    vinserti64x2 zmm3, zmm3, %1, %2
%endif
%endmacro

%macro ucache_mov 2
    vpermi2q zmm0, zmm2, zmm3
%endmacro


;; ex)
;; mov rdi, 0x4 ;; 0~7
;; ucache_load_i128 rdi
%macro ucache_mov_xmm0_idx 1
    inc %1
    vpbroadcastq xmm0, %1
    dec %1
    vmovq xmm0, %1
    vpermi2q zmm0, zmm2, zmm3
%endmacro


;; ex)
;; mov rdi, 0x4 ;; 0~7
;; ucache_store_i128 rdi
%macro ucache_mov_idx_xmm0 1
    inc %1
    vpbroadcastq xmm0, %1
    dec %1
    vmovq xmm0, %1
    vpermi2q zmm0, zmm2, zmm3
%endmacro
