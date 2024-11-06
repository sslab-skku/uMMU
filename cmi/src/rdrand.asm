section .text
global rdrand
rdrand:
    ; Execute rdrand instruction
    rdrand rax
    
    ret