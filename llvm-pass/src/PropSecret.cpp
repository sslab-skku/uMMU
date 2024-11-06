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
#include <llvm-14/llvm/IR/InstVisitor.h>
#include <llvm-14/llvm/IR/Intrinsics.h>
#include <llvm-14/llvm/Support/Casting.h>

#include "Utils.h"

using namespace llvm;
using namespace SVF;

class PropSecret : public ModulePass {
public:
  static char ID;
  PointerAnalysis *pta;
  SVFIR *pag;
  SVFModule *SVFM;
  LLVMModuleSet *LMS;

  PropSecret() : ModulePass(ID) {}
  bool runOnModule(Module &M) override {
    MDVisitor MDV;
    Set<Value *> secret_ptr_ptr_set;
    Set<Value *> secret_ptr_set;

    // Initialize SVF
    LMS = LLVMModuleSet::getLLVMModuleSet();
    SVFM = LMS->buildSVFModule(M);
    SVFIRBuilder builder(SVFM);
    pag = builder.build();
    pta = AndersenWaveDiff::createAndersenWaveDiff(pag);

    #if 0
    // Anlaysis & Transformations
    // MDV.CollectValues(&secret_ptr_ptr_set, M, "secret_ptr_ptr");
    // MDV.CollectValues(&secret_ptr_ptr_set, M, "secret_ptr_ptr");

    // secret_ptr_ptr -> secret_ptr
    taintPtr(&secret_ptr_set, &secret_ptr_ptr_set);
    #endif

    MDV.CollectValues(&secret_ptr_set, M, "secret_ptr");

    Set<std::string> logSensitiveEmptyFuncSet;

    // taint every memory accesses that use secret_ptr
    for (auto &F : M.functions()) {
      for (auto &B : F) {
        for (auto &I : B) {
          bool isSensitive = false;
          // ex) %10 = LOAD secret_ptr
          if (auto LI = dyn_cast<LoadInst>(&I)) {
            if (ptaQueryOverlap(pta, LI->getPointerOperand(),
                                &secret_ptr_set)) {
              isSensitive = true;
            }
            // ex) STORE secret_ptr, %10
          } else if (auto SI = dyn_cast<StoreInst>(&I)) {
            if (ptaQueryOverlap(pta, SI->getPointerOperand(),
                                &secret_ptr_set)) {
              isSensitive = true;
            }
          } else {
            bool maySensitive = false;
            for (auto &oi : I.operands()) {
              // if (oi.getOperandNo() == 0)
              //   continue;
              if (oi.get()->getType()->isPointerTy()) {
                if (ptaQueryOverlap(pta, oi.get(), &secret_ptr_set)) {
                  maySensitive = true;
                  break;
                }
              }
            }
            if (maySensitive) {
              if (auto *CI = dyn_cast<CallInst>(&I)) {
                if (auto *CF = CI->getCalledFunction()) {
                  isSensitive=true;
                  if(CF->empty()){
                    logSensitiveEmptyFuncSet.insert(CF->getName().str());
                  }
                }
              }
            }
          }

          if (isSensitive)
            setMetaData(&I, "sensitive");
        }
      }
    }

    errs() << "+++sensitive_funcs\n";
    for(auto fnstr:logSensitiveEmptyFuncSet){
      errs()<< fnstr << "\n";
    }
    errs() << "---sensitive_funcs\n";

    // Release
    AndersenWaveDiff::releaseAndersenWaveDiff();
    SVFIR::releaseSVFIR();
    SVF::LLVMModuleSet::releaseLLVMModuleSet();
    return true;
  }

  void taintPtr(Set<Value *> *dst, const Set<Value *> *ptrSet) {
    auto WL = new SetVector<Value *>();
    auto VL = new SetVector<Value *>();

    // 1. Intra-procedure analysis
    for (auto p : *ptrSet) {
      WL->insert(p);
    }

    // 1.1 secret_ptr_ptr tracking
    while (!WL->empty()) {
      auto V = WL->pop_back_val();
      if (VL->contains(V)) {
        continue;
      }

      VL->insert(V);

      for (auto U : V->users()) {
        if (auto *GEP = dyn_cast<GetElementPtrInst>(U)) {
          if (GEP->getResultElementType()->isPointerTy()) {
            WL->insert(GEP);
          }
        } else if (auto *AI = dyn_cast<AllocaInst>(U)) {
          if (AI->getAllocatedType()->isPointerTy()) {
            WL->insert(AI);
          }
        } else if (auto *BCI = dyn_cast<BitCastInst>(U)) {
          if (BCI->getType()->isPointerTy()) {
            WL->insert(BCI);
          }
        }
      }
    }

    // 1.2 Find users that Load/Store, secret_ptr_ptr, the value is
    // secret_ptr
    std::swap(WL, VL);
    while (!WL->empty()) {
      auto V = WL->pop_back_val();
      for (auto U : V->users()) {
        Value *SV = nullptr;
        // The secret_ptr_ptr used as ptr operand
        // ex) STORE secret_ptr_ptr,  secret_ptr
        if (auto *SI = dyn_cast<StoreInst>(U)) {
          if (SI->getPointerOperand() == V) {
            SV = SI->getValueOperand();
          }
        }
        // ex) secret_ptr = LOAD secret_ptr_ptr
        else if (auto *LI = dyn_cast<LoadInst>(U)) {
          SV = LI;
        }

        if (SV != nullptr) {
          if (!VL->contains(SV)) {
            VL->insert(SV);
          }
        }
      }
    }

    // Points-to anlaysis based propagation
    std::swap(WL, VL);
    auto LMS = LLVMModuleSet::getLLVMModuleSet();
    auto pag = pta->getPAG();
    // errs() << "Taint 3\n";
    while (!WL->empty()) {
      auto V = WL->pop_back_val();
      // errs() << *V << "\n";
      auto svfV = LMS->getSVFValue(V);
      // Iterate points-to set
      for (auto pi : pta->getPts(pag->getValueNode(svfV))) {
        auto gn = pag->getGNode(pi);
        if (gn->hasValue()) {
          Value *v = (Value *)LMS->getLLVMValue(gn->getValue());
          if (!VL->contains(v))
            VL->insert(v);
        } else {
          errs() << "gn->hasValue == false\n";
        }
      }
    }

    // Set MetaData
    std::swap(WL, VL);
    while (!WL->empty()) {
      auto V = WL->pop_back_val();
      if (auto I = dyn_cast<Instruction>(V)) {
        dst->insert(I);
        setMetaData(I, "secret_ptr");
      } else {
        errs() << "invalid I\n";
      }
    }
  }
};

char PropSecret::ID = 0;

static RegisterPass<PropSecret>
    X("prop-secret", "prop secret with andersen PTA(WPA)", false, false);
