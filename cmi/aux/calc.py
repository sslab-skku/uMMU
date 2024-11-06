import math

def clog2(x):
    return math.ceil(math.log2(x))

M=2048*8                          # Bit
print(f"Total Mem: {M//8}B")
frn_w=5
pdn=1
frn=2**frn_w

pg_sz=M/frn

print(f"page(size): {pg_sz/8}B")
print(f"page(N): {frn}")
print(f"page dir(N):-{pdn}")

aux=3 # present bit, clock page swap bit, reserved bit

pd_h=pdn*pg_sz/(frn_w+aux)
pd2_h=pg_sz/(frn_w+aux)
pd3_h=1#pg_sz/(frn_w+aux)
pt_h=pg_sz/(frn_w+aux)

virt=pd_h*pd2_h*pd3_h*pt_h*pg_sz
print(f"pd3_h: {pd3_h}")
print(f"virt : {virt//1000//1000}MB")

# Local Variables:
# compile-command: "python3 calc.py"
# End:
