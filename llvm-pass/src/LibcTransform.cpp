#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Pass.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Transforms/Utils/Cloning.h"
#include <llvm-14/llvm/IR/InstVisitor.h>
#include <llvm-14/llvm/Support/Casting.h>

using namespace std;
using namespace llvm;

class LibcTransformPass : public ModulePass {
public:
  static char ID;
  LibcTransformPass() : ModulePass(ID) {}
  static void replaceIntoCloneOf(CallInst *dstInst, Function *srcFn) {
    ValueToValueMapTy VMap;
    Function *clonedFunction = CloneFunction(srcFn, VMap, NULL);
    clonedFunction->setLinkage(GlobalValue::InternalLinkage);
    dstInst->setCalledFunction(clonedFunction);
  }
  static Function *internalMemcpyFn;
  static Function *internalMemsetFn;
  static Function *internalMemcmpFn;
  static Function *internalStrlenFn;
  static Function *internalStrcatFn;
  static Function *internalStrcpyFn;
  static Function *internalStrcmpFn;
  static Function *internalStrstrFn;
  struct {
    Function *explicit_bzero;
    Function *memcpy;
    Function *memmove;
    Function *memset;
    Function *memcmp;
    Function *strlen;
    Function *strcat;
    Function *strcpy;
    Function *strcmp;
    Function *strstr;
    Function *read;
    Function *recvfrom;
    Function *recv;
  } tlibcFn;

  bool runOnModule(Module &M) {
    tlibcFn.explicit_bzero = M.getFunction("tlibc_internal_explicit_bzero");
    tlibcFn.memcpy = M.getFunction("tlibc_internal_memcpy");
    tlibcFn.memmove = M.getFunction("tlibc_internal_memmove");
    tlibcFn.memset = M.getFunction("tlibc_internal_memset");
    tlibcFn.memcmp = M.getFunction("tlibc_internal_memcmp");
    tlibcFn.strlen = M.getFunction("tlibc_internal_strlen");
    tlibcFn.strcat = M.getFunction("tlibc_internal_strcat");
    tlibcFn.strcpy = M.getFunction("tlibc_internal_strcpy");
    tlibcFn.strcmp = M.getFunction("tlibc_internal_strcmp");
    tlibcFn.strstr = M.getFunction("tlibc_internal_strstr");
    tlibcFn.read = M.getFunction("tlibc_internal_read");
    tlibcFn.recv = M.getFunction("tlibc_recv");
    tlibcFn.recvfrom = M.getFunction("tlibc_recvfrom");

    for (auto &F : M) {
      for (auto &B : F) {
        for (auto &I : B) {
          if (auto CI = dyn_cast<CallInst>(&I)) {

            if (Function *calledFunction = CI->getCalledFunction()) {
              if (calledFunction->getName().find("llvm.memset") !=
                  StringRef::npos) {
                replaceIntoCloneOf(CI, tlibcFn.memset);
              } else if (calledFunction->getName().find("llvm.memcpy") !=
                         StringRef::npos) {
                replaceIntoCloneOf(CI, tlibcFn.memcpy);
              } else if (calledFunction->getName().find("llvm.memmove") !=
                         StringRef::npos) {
                replaceIntoCloneOf(CI, tlibcFn.memmove);
              } else if (calledFunction->getName().equals("explicit_bzero")) {
                replaceIntoCloneOf(CI, tlibcFn.explicit_bzero);
              } else if (calledFunction->getName().equals("memcmp")) {
                replaceIntoCloneOf(CI, tlibcFn.memcmp);
              } else if (calledFunction->getName().equals("strlen")) {
                replaceIntoCloneOf(CI, tlibcFn.strlen);
              } else if (calledFunction->getName().equals("strcat")) {
                replaceIntoCloneOf(CI, tlibcFn.strcat);
              } else if (calledFunction->getName().equals("strcpy")) {
                replaceIntoCloneOf(CI, tlibcFn.strcpy);
              } else if (calledFunction->getName().equals("strcmp")) {
                replaceIntoCloneOf(CI, tlibcFn.strcmp);
              } else if (calledFunction->getName().equals("strstr")) {
                replaceIntoCloneOf(CI, tlibcFn.strstr);
              } else if (calledFunction->getName().equals("read")) {
                replaceIntoCloneOf(CI, tlibcFn.read);
              } else if (calledFunction->getName().equals("recvfrom")) {
                replaceIntoCloneOf(CI, tlibcFn.recvfrom);
              } else if (calledFunction->getName().equals("recv")) {
                replaceIntoCloneOf(CI, tlibcFn.recv);
              }
            }
          }
        }
      }
    }

    return true;
  }
};

char LibcTransformPass::ID = 0;

static RegisterPass<LibcTransformPass>
    X("libc-transform",
      "Transform calls to memcpy, memset, memcmp and their string equivalents",
      false, false);
