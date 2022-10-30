// For open-source license, please refer to [License](https://github.com/HikariObfuscator/Hikari/wiki/License).
#include "dobby.h"
#include <llvm/Transforms/Obfuscation/Obfuscation.h>
#include <llvm/IR/PassManager.h>
#include <llvm/Passes/OptimizationLevel.h>
#include <llvm/Config/abi-breaking.h>
#include <string>
#include <mach-o/dyld.h>
#if LLVM_VERSION_MAJOR != 14
#warning "This fork of Naville's Hanabi only tested in Xcode 14.0, other versions of Xcode may have problems"
#endif
#if LLVM_ENABLE_ABI_BREAKING_CHECKS==1
#error "Configure LLVM with -DLLVM_ABI_BREAKING_CHECKS=FORCE_OFF"
#endif
using namespace std;
void (*old_pmb)(void* dis,legacy::PassManagerBase &MPM);
static void new_pmb(void* dis,legacy::PassManagerBase &MPM){
  MPM.add(createObfuscationLegacyPass());
  old_pmb(dis,MPM);
}

static ModulePassManager buildObfuscationPipeline(void) {
  ModulePassManager MPM;
  MPM.addPass(ObfuscationPass());
  return MPM;
}
Error (*old_pmp)(ModulePassManager &MPM, int E);
static Error new_pmp(ModulePassManager &MPM, int E) {
  MPM.addPass(buildObfuscationPipeline());
  return old_pmp(MPM, E);
}

static __attribute__((__constructor__)) void Inj3c73d(int argc, char* argv[]){
  char* executablePath=argv[0];
  //Initialize our own LLVM Library
  if (strstr(executablePath, "swift-frontend"))
    errs() << "Applying Apple SwiftC Hooks...\n";
  else
    errs() << "Applying Apple Clang Hooks...\n";
  DobbyHook(DobbySymbolResolver(executablePath, "__ZN4llvm18PassManagerBuilder25populateModulePassManagerERNS_6legacy15PassManagerBaseE"), (dobby_dummy_func_t)new_pmb, (dobby_dummy_func_t *)&old_pmb);
  DobbyHook(DobbySymbolResolver(executablePath, "__ZN4llvm11PassBuilder15parseModulePassERNS_11PassManagerINS_6ModuleENS_15AnalysisManagerIS2_JEEEJEEERKNS0_15PipelineElementE"), (dobby_dummy_func_t)new_pmp, (dobby_dummy_func_t *)&old_pmp);
}
