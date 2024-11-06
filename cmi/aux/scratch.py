# -*- compile-command: "python3 scratch.py"; -*-


for i in range(1,10):
    print(f"%xmm0 %xmm{i}\n"+
          f"%xmm1 %xmm{i}\n")

