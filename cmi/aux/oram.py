#!/bin/python3
import random

ORAM_Z=4

class oram_block:

class oram_block_arr:
    data=[]
    bid=[]
    length=0

class oram_ctx:
    L=0
    Z=0
    S=oram_block_arr()
    B=oram_block_arr()

def alloc_oram_block_arr(size):
    b=oram_block_arr() # malloc
    b.data=[0]*size # calloc()
    b.bid=[0]*size #
    return b

def alloc_oram_ctx(Z,L):
    ctx=oram_ctx() # malloc
    ctx.L=L
    ctx.S=alloc_oram_block_arr(200)
    ctx.Z=Z
    return ctx

def P(L, x, l):
    return (x>>(L-l)) + (1<<l) - 1

def oram_access(ctx, op, a, data):
    S=ctx.S
    St=[]
    L=ctx.L
    posmap=L.posmap
    x=posmap[a]
    posmap[a] = random.randrange(0, 1<<L - 1)

    for l in range(0,L+1):
        S.
