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
#include "llvm/Support/Format.h"
#include <iomanip>
#include <llvm-14/llvm/IR/InstVisitor.h>
#include <llvm-14/llvm/Support/Casting.h>

using namespace llvm;
using namespace SVF;

struct StatVisitor : public InstVisitor<StatVisitor> {
  int loadInstCount = 0;
  int storeInstCount = 0;
  int mdInstCount = 0;
  int unknownInstCount = 0;
  int totalInstCount = 0;
  int allocInstCount = 0;
  StringMap<int> instCntMap;
  StringMap<int> allocCntMap;
  StringRef MDStrRef;

  void collect(Module *_M, StringRef str) {
    MDStrRef = MDString::get(_M->getContext(), str)->getString();
    this->visit(_M);
  }

  void visitInstruction(Instruction &I) {
    auto PFName = I.getFunction()->getName();
    if (PFName.find("cmi_malloc") == 0 || PFName.find("cmi_calloc") == 0 ||
        PFName.find("cmi_load") == 0 || PFName.find("cmi_store") == 0) {
      return;
    }
    totalInstCount++;

    mdInstCount++;

    if (auto *CI = dyn_cast<CallInst>(&I)) {
      // Allocator
      if (auto *CF = CI->getCalledFunction()) {
        auto CFName = CF->getName();

        if (I.hasMetadata(MDStrRef)) {
        }
        // if (CFName.equals("malloc")) {
        //   allocInstCount++;
        // }
        // if (CFName.equals("calloc")) {
        //   allocInstCount++;
        // }
        // if (CFName.equals("internal_malloc")) {
        //   allocInstCount++;
        // }
        // if (CFName.equals("internal_calloc")) {
        //   allocInstCount++;
        // }

        // Alloc
        if (CFName.find("cmi_malloc") == 0 || CFName.find("cmi_calloc") == 0) {
          auto it = allocCntMap.find(PFName);
          int Cnt = (allocCntMap.count(PFName) != 0) ? it->second : 0;
          Cnt++;
          allocCntMap.insert({PFName, Cnt});
        }

        // Find Instrumented Load Stores
        if (CFName.find("cmi_load") == 0 || CFName.find("cmi_store") == 0) {
          instCntMap.try_emplace(PFName, 0);
          auto kv = instCntMap.find(PFName);
          kv->second++;
          // int Cnt = (instCntMap.count(PFName) != 0) ? it->second : 0;
          // errs() << "find cmi load/store" << PFName << " Cnt was:" << Cnt << "\n";
          // Cnt++;
          // instCntMap.insert({PFName, Cnt});
        }
      }
    }
    // Memory Access
    else if (auto *LI = dyn_cast<LoadInst>(&I)) {
      loadInstCount++;
    } else if (auto *SI = dyn_cast<StoreInst>(&I)) {
      storeInstCount++;
    }
    // Unknown
    else {
      unknownInstCount++;
    }
  }

  void printStats() {
    errs() << "load:" << loadInstCount << "\n"
           << "store:\t" << storeInstCount << "\n"
           << "alloc:\t" << allocInstCount << "\n"
           << "unknown:\t" << unknownInstCount << "\n"
           << "metadata:\t" << mdInstCount << "\n"
           << "total:" << totalInstCount << "\n";
    errs() << "--- FUNC : #cmi_load/stores ---"
           << "\n";
    for (auto &it : instCntMap) {
      errs() << it.first() << ":" <<  it.second << "\n";
    }
  }
};

class Stats : public ModulePass {
public:
  static char ID;
  PointerAnalysis *pta;
  SVFIR *pag;
  SVFModule *SVFM;
  LLVMModuleSet *LMS;

  Stats() : ModulePass(ID) {}
  bool runOnModule(Module &M) override {
    // Stats
    StatVisitor stat;
    stat.collect(&M, "sensitive");
    stat.printStats();

    return true;
  }
};

char Stats::ID = 0;

static RegisterPass<Stats> X("vmi-stats", "vmi stats", false, true);
