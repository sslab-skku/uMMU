#include "Graphs/SVFG.h"
#include "Graphs/VFG.h"
#include "MemoryModel/PointerAnalysis.h"
#include "SVF-LLVM/BasicTypes.h"
#include "SVF-LLVM/LLVMModule.h"
#include "SVF-LLVM/LLVMUtil.h"
#include "SVF-LLVM/SVFIRBuilder.h"
#include "SVFIR/SVFModule.h"
#include "SVFIR/SVFType.h"
#include "Util/CallGraphBuilder.h"
#include "Util/Options.h"
#include "WPA/Andersen.h"
#include "WPA/Steensgaard.h"
#include "llvm/Pass.h"

#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/raw_ostream.h"
#include <llvm-14/llvm/IR/InstIterator.h>
#include <llvm-14/llvm/IR/InstVisitor.h>
#include <llvm-14/llvm/Support/Casting.h>

using namespace llvm;
using namespace SVF;

inline std::string printPts(PointerAnalysis *pta, Value *val) {
  auto LMS = LLVMModuleSet::getLLVMModuleSet();
  auto svfval = LMS->getSVFValue(val);
  std::string str;
  std::stringstream rawstr(str);

  NodeID pNodeId = pta->getPAG()->getValueNode(svfval);
  const PointsTo &pts = pta->getPts(pNodeId);
  for (PointsTo::iterator ii = pts.begin(), ie = pts.end(); ii != ie; ii++) {
    rawstr << " " << *ii << " ";
    PAGNode *targetObj = pta->getPAG()->getGNode(*ii);
    if (targetObj->hasValue()) {
      rawstr << "(" << targetObj->getValue()->toString() << ")\t ";
    }
  }

  return rawstr.str();
}

inline void setMetaData(Instruction *I, std::string str) {
  LLVMContext &C = I->getContext();
  MDNode *N = MDNode::get(C, MDString::get(C, str));
  I->setMetadata(str, N);
}

inline bool hasMetaData(const Instruction *I, std::string str) {
  LLVMContext &C = I->getContext();
  auto strRef = MDString::get(C, str)->getString();
  return I->hasMetadata(strRef);
}

inline StringRef extractAnnotation(CallInst *CI) {
  // assert(CI->getCalledFunction()->getName() == "llvm.var.annotation");
  errs() << "extract " << CI->getCalledFunction()->getName() << "\n";
  Value *annoVal = CI->getOperand(1);
  GlobalVariable *AnnoStr;
  if (auto CE = dyn_cast<ConstantExpr>(annoVal)) {
    AnnoStr = cast<GlobalVariable>(CE->getOperand(0));
  } else if (auto GEP = dyn_cast<GetElementPtrInst>(annoVal)) {
    AnnoStr = cast<GlobalVariable>(GEP->getOperand(0));
  }
  if (ConstantDataSequential *Data =
          dyn_cast<ConstantDataSequential>(AnnoStr->getInitializer())) {
    if (Data->isString()) {
      // errs() << CI->getParent()->getParent()->getName()
      //        << ":Found annotation: " << Data->getAsCString() << "\n";
      return Data->getAsCString();
    }
  }
  return "";
}

struct MDVisitor : public InstVisitor<MDVisitor> {

  MDVisitor() {}
  void visitInstruction(Instruction &I) {
    if (!I.hasMetadata(MDStrRef))
      return;
    result->insert(&cast<Value>(I));
  }
  void CollectValues(Set<Value *> *_result, Module &_M, StringRef str) {
    MDStrRef = MDString::get(_M.getContext(), str)->getString();
    result = _result;
    // Visit
    this->visit(_M);
  }

private:
  StringRef MDStrRef;
  Set<Value *> *result;
};

inline bool ptaQueryOverlap(PointerAnalysis *pta, Value* val, Set<Value*>* src){
  auto LMS = LLVMModuleSet::getLLVMModuleSet();
    auto sv1 = LMS->getSVFValue(val);
    for (auto s : *src) {
      auto sv2 = LMS->getSVFValue(s);
      auto AR = pta->alias(sv1, sv2);
      if (AR != SVF::NoAlias) {
        return true;
      }
    }
    return false;
}
