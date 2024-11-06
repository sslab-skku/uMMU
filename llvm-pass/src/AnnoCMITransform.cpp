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

using namespace llvm;

class AnnoCMITransformPass : public ModulePass {
public:
  static char ID;
  AnnoCMITransformPass() : ModulePass(ID) {}
  static void replaceIntoCloneOf(CallInst *dstInst, Function *srcFn) {
    ValueToValueMapTy VMap;
    Function *clonedFunction = CloneFunction(srcFn, VMap, NULL);
    clonedFunction->setLinkage(GlobalValue::InternalLinkage);
    dstInst->setCalledFunction(clonedFunction);
  }
  struct {
    Function *load8;
    Function *load16;
    Function *load32;
    Function *load64;
    Function *store8;
    Function *store16;
    Function *store32;
    Function *store64;
    Function *malloc;
    Function *calloc;
    Function *free;
  } cmiFunction;

  bool runOnModule(Module &M) {
    // Get cmi Functions
    cmiFunction.load8 = M.getFunction("cmi_load8");
    cmiFunction.load16 = M.getFunction("cmi_load16");
    cmiFunction.load32 = M.getFunction("cmi_load32");
    cmiFunction.load64 = M.getFunction("cmi_load64");
    cmiFunction.store8 = M.getFunction("cmi_store8");
    cmiFunction.store16 = M.getFunction("cmi_store16");
    cmiFunction.store32 = M.getFunction("cmi_store32");
    cmiFunction.store64 = M.getFunction("cmi_store64");
    cmiFunction.malloc = M.getFunction("cmi_malloc");
    cmiFunction.calloc = M.getFunction("cmi_calloc");
    cmiFunction.free = M.getFunction("cmi_free");

    for (auto &F : M) {
      for (auto &B : F) {
        for (auto &I : B) {
          if (auto CI = dyn_cast<CallInst>(&I)) {
            if (Function *CF = CI->getCalledFunction()) {
              // Maybe use if(find("xxx")==0 )?
              if (CF->getName().equals("anno_cmi_load8")) {
                CI->setCalledFunction(cmiFunction.load8);
              } else if (CF->getName().equals("anno_cmi_load16")) {
                CI->setCalledFunction(cmiFunction.load16);
              } else if (CF->getName().equals("anno_cmi_load32")) {
                CI->setCalledFunction(cmiFunction.load32);
              } else if (CF->getName().equals("anno_cmi_load64")) {
                CI->setCalledFunction(cmiFunction.load64);
              } else if (CF->getName().equals("anno_cmi_store8")) {
                CI->setCalledFunction(cmiFunction.store8);
              }
              // Store
              else if (CF->getName().equals("anno_cmi_store8")) {
                CI->setCalledFunction(cmiFunction.store8);
              } else if (CF->getName().equals("anno_cmi_store16")) {
                CI->setCalledFunction(cmiFunction.store16);
              } else if (CF->getName().equals("anno_cmi_store32")) {
                CI->setCalledFunction(cmiFunction.store32);
              } else if (CF->getName().equals("anno_cmi_store64")) {
                CI->setCalledFunction(cmiFunction.store64);
              }
              // malloc calloc free
              else if (CF->getName().equals("anno_cmi_malloc")) {
                CI->setCalledFunction(cmiFunction.malloc);
              } else if (CF->getName().equals("anno_cmi_calloc")) {
                CI->setCalledFunction(cmiFunction.calloc);
              } else if (CF->getName().equals("anno_cmi_free")) {
                CI->setCalledFunction(cmiFunction.free);
              }
              // check error
              else if (CF->getName().find("anno_cmi_") !=
                         StringRef::npos) {
                assert(0 && "find(anno_cmi_) != npos, but can't transform");
              }
            }
          }
        }
      }
    }

    return true;
  }
};

char AnnoCMITransformPass::ID = 0;

static RegisterPass<AnnoCMITransformPass>
    X("anno-cmi-transform",
      "anno_cmi_XXX -> cmi_XXX",
      false, false);
