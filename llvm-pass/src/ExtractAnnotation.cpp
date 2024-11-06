#include "Utils.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/raw_ostream.h"
#include <llvm-14/llvm/ADT/StringRef.h>
#include <llvm-14/llvm/IR/InstVisitor.h>
#include <llvm-14/llvm/Support/Casting.h>

using namespace llvm;

struct AnnotationExtractor : public InstVisitor<AnnotationExtractor> {
public:
  int secret_obj_count=0;
  AnnotationExtractor() {}

  void visitCall(CallInst &CI) {
    if (!CI.getCalledFunction())
      return;
    auto funcName = CI.getCalledFunction()->getName();
    if (funcName.find("anno_secret") != StringRef::npos) {
      setMetaData(&CI, "secret_ptr");
      errs() << "find secret_obj"  << ++secret_obj_count << " in " << CI.getFunction()->getName()<< "\n";
      errs() << CI << "\n";
    }
    // Local variable annotation
    else if (funcName == "llvm.var.annotation") {
      auto annoStr = extractAnnotation(&CI);
      Value *annoVal = CI.getOperand(0);
      if (auto *BC = dyn_cast<BitCastInst>(annoVal)) {
        annoVal = BC->getOperand(0);
      }
      setMetaData(cast<Instruction>(annoVal), annoStr.str());
    } else if(funcName.find("llvm.ptr.annotation") != StringRef::npos) {
      auto annoStr = extractAnnotation(&CI);
      Value *annoVal = CI.getOperand(0);
      if (auto *BC = dyn_cast<BitCastInst>(annoVal)) {
        annoVal = BC->getOperand(0);
      }
      setMetaData(cast<Instruction>(annoVal), annoStr.str());
      return;
    }
  }
};

class ExtractAnnotation : public ModulePass {
public:
  static char ID;

  ExtractAnnotation() : ModulePass(ID) {}
  bool runOnModule(Module &M) override {
    AnnotationExtractor().visit(M);
    return true;
  }
};

char ExtractAnnotation::ID = 0;

static RegisterPass<ExtractAnnotation> X("extract-annotation",
                                         "extract annotations", false, false);
