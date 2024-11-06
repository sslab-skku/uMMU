#include "llvm/Pass.h"

#include "SVF-LLVM/BasicTypes.h"
#include "Utils.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

class CMIInstrumentation : public ModulePass {
public:
  static char ID;
  struct {
    Function *load8;
    Function *load16;
    Function *load32;
    Function *load64;
    Function *load128;
    Function *store8;
    Function *store16;
    Function *store32;
    Function *store64;
    Function *store128;
    Function *malloc;
    Function *calloc;
    Function *free;
  } cmiFunction;

  CMIInstrumentation() : ModulePass(ID) {}
  Value *buildCmiLoad(LoadInst *orig);
  Value *buildCmiStore(StoreInst *orig);
  bool runOnModule(Module &M) override {
    // Get cmi Functions
    cmiFunction.load8 = M.getFunction("cmi_load8");
    cmiFunction.load16 = M.getFunction("cmi_load16");
    cmiFunction.load32 = M.getFunction("cmi_load32");
    cmiFunction.load64 = M.getFunction("cmi_load64");
    cmiFunction.load128 = M.getFunction("cmi_load128");
    cmiFunction.store8 = M.getFunction("cmi_store8");
    cmiFunction.store16 = M.getFunction("cmi_store16");
    cmiFunction.store32 = M.getFunction("cmi_store32");
    cmiFunction.store64 = M.getFunction("cmi_store64");
    cmiFunction.store128 = M.getFunction("cmi_store128");
    cmiFunction.malloc = M.getFunction("cmi_malloc");
    cmiFunction.calloc = M.getFunction("cmi_calloc");
    cmiFunction.free = M.getFunction("cmi_free");

    // Instrumentation
    std::vector<Instruction *> replaced;
    for (auto &F : M) {
      for (auto &B : F) {
        for (auto &I : B) {
          // 1. Allocator
          if (hasMetaData(&I, "secret_ptr")) {
            if (auto *CI = dyn_cast<CallInst>(&I)) {
              if (auto *CF = CI->getCalledFunction()) {
                if (CF->getName().find("anno_secret_malloc")==0) {
                  CI->setCalledFunction(cmiFunction.malloc);
                }
                if (CF->getName().find("anno_secret_calloc")==0) {
                  CI->setCalledFunction(cmiFunction.calloc);
                }
              }
            }
          }
          // 2. May uses secret_ptr
          if (hasMetaData(&I, "sensitive")) {
            // Memory Access
            // Load
            if (auto *LI = dyn_cast<LoadInst>(&I)) {
              Value *cmiLoad = buildCmiLoad(LI);
              LI->replaceAllUsesWith(cmiLoad);
              replaced.push_back(LI);
            }
            // Store
            else if (auto *SI = dyn_cast<StoreInst>(&I)) {
              buildCmiStore(SI);
              replaced.push_back(SI);
            }
            // Function call
            else if (auto *CI = dyn_cast<CallInst>(&I)) {
              if (auto *CF = CI->getCalledFunction()) {
                if (CF->getName().find("free")==0) {
                  CI->setCalledFunction(cmiFunction.free);
                }
              }
            }
            // Unknown
            else {
            }
          }
        }
      }
    }
    // Erase original load/store
    for (auto *I : replaced) {
      I->eraseFromParent();
    }
    return true;
  }
};

Value *CMIInstrumentation::buildCmiStore(StoreInst *orig) {
  Value *ret = nullptr;
  int width = 0;
  llvm::IRBuilder<> Builder(orig);

  Value *ptrOp = orig->getPointerOperand();
  Type *ptrOpType = ptrOp->getType()->getPointerElementType();

  assert(!ptrOpType->isDoubleTy() && "Unsupported prtType!");
  // assert(!ptrOpType->isVectorTy() && "Unsupported prtType!");

  if (auto *PT = dyn_cast<PointerType>(ptrOpType)) {
    width = orig->getModule()->getDataLayout().getPointerSizeInBits();
  } else if (auto *IT = dyn_cast<IntegerType>(ptrOpType)) {
    width = IT->getBitWidth();
  } else if (auto *VT = dyn_cast<VectorType>(ptrOpType)) {
    width = VT->getScalarSizeInBits();
  }

  // Get Function
  Function *func = nullptr;
  switch (width) {
  case 8:
    func = cmiFunction.store8;
    break;
  case 16:
    func = cmiFunction.store16;
    break;
  case 32:
    func = cmiFunction.store32;
    break;
  case 64:
    func = cmiFunction.store64;
    break;
  case 128:
    func = cmiFunction.store64;
    break;
  default:
    errs() << *orig << "\n";
    assert(0 && "Unsupported bitWidth");
    break;
  }
  auto *funcType = func->getFunctionType();
  if (width == 128) {
    Type *i64PtrTy = funcType->getParamType(0);
    Type *i64Ty = i64PtrTy->getPointerElementType();
    auto *val = orig->getValueOperand();
    // dst0: low 64bit
    auto *dst0 = Builder.CreateBitCast(orig->getPointerOperand(), i64PtrTy);

    Value *idxList = {ConstantInt::get(i64Ty, 1)};
    // dst1: high 64bit(dst0+1)
    auto *dst1 = Builder.CreateInBoundsGEP(i64Ty, dst0, idxList);

    auto *val0 = Builder.CreateTrunc(val, i64Ty);
    auto *val1 = Builder.CreateTrunc(Builder.CreateLShr(val, 64), i64Ty);
    Builder.CreateCall(func, {dst0, val0});
    return Builder.CreateCall(func, {dst1, val1});
  } else {
    auto *arg0 = Builder.CreateBitCast(orig->getPointerOperand(),
                                       funcType->getParamType(0));
    Value *arg1 = orig->getValueOperand();
    if (arg1->getType()->isPointerTy()) {
      arg1 = Builder.CreatePtrToInt(arg1, funcType->getParamType(1));
    }
    arg1 = Builder.CreateBitCast(arg1, funcType->getParamType(1));

    return Builder.CreateCall(func, {arg0, arg1});
  }

  return nullptr;
}

Value *CMIInstrumentation::buildCmiLoad(LoadInst *orig) {
  Value *ret = nullptr;
  int width = 0;
  llvm::IRBuilder<> Builder(orig);

  Value *ptrOp = orig->getPointerOperand();
  Type *ptrOpType = ptrOp->getType()->getPointerElementType();

  assert(!ptrOpType->isDoubleTy() && "Unsupported prtType!");
  // assert(!ptrOpType->isVectorTy() && "Unsupported prtType!");
  assert(!ptrOpType->isVoidTy() && "Unsupported prtType!");

  if (auto *PT = dyn_cast<PointerType>(ptrOpType)) {
    width = orig->getModule()->getDataLayout().getPointerSizeInBits();
  } else if (auto *IT = dyn_cast<IntegerType>(ptrOpType)) {
    width = IT->getBitWidth();
  } else if (auto *VT = dyn_cast<VectorType>(ptrOpType)) {
    width = VT->getScalarSizeInBits();
  }

  // Get Function
  Function *func = nullptr;
  std::vector<Value *> args;

  args.push_back(ptrOp);
  switch (width) {
  case 8:
    func = cmiFunction.load8;
    break;
  case 16:
    func = cmiFunction.load16;
    break;
  case 32:
    func = cmiFunction.load32;
    break;
  case 64:
    func = cmiFunction.load64;
    break;
  case 128:
    func = cmiFunction.load64;
    break;
  default:
    errs() << *orig << "\n";
    assert(0 && "Unsupported bitWidth");
    break;
  }

  auto *funcType = func->getFunctionType();
  auto *arg0 = Builder.CreateBitCast(orig->getPointerOperand(),
                                     funcType->getParamType(0));
  Value *retVal = Builder.CreateCall(func, {arg0});
  if (orig->getType()->isPointerTy()) {
    retVal = Builder.CreateIntToPtr(retVal, orig->getType());
  }

  // Create additional call for 128bit load
  if (width == 128) {
    Type *gepType = arg0->getType()->getPointerElementType();
    Value *idxList = {ConstantInt::get(gepType, 1)};
    auto *arg0_2 = Builder.CreateInBoundsGEP(
        gepType, arg0,
        idxList); // %arg0_2 = getelementptr inbounds i64, i64* %arg0, i64 1
    Value *retVal2 = Builder.CreateCall(func, {arg0_2});

    retVal =
        Builder.CreateZExt(retVal, orig->getType()); // zext i64 $retval to i128
    retVal = Builder.CreateShl(retVal, 64, "", true);
    retVal2 = Builder.CreateZExt(retVal2,
                                 orig->getType()); // zext i64 $retval to i128
    retVal = Builder.CreateOr(retVal, retVal2);
  }

  // Cast return
  retVal = Builder.CreateBitCast(retVal, orig->getType());
  return retVal;
}

char CMIInstrumentation::ID = 0;

static RegisterPass<CMIInstrumentation>
    X("inst-secret", "instrument secret related instructions", false, false);
