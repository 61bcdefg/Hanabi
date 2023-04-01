// For open-source license, please refer to
// [License](https://github.com/HikariObfuscator/Hikari/wiki/License).
#include "dobby.h"
#include <llvm/Config/abi-breaking.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/IR/PassManager.h>
#include <llvm/Transforms/Obfuscation/Obfuscation.h>
#include <mach-o/dyld.h>
#include <string>
#include <sys/sysctl.h>

#if LLVM_ENABLE_ABI_BREAKING_CHECKS
#error "Configure LLVM with -DLLVM_ABI_BREAKING_CHECKS=FORCE_OFF"
#endif

using namespace llvm;

void (*old_pmb)(void *dis, legacy::PassManagerBase &MPM);
static void new_pmb(void *dis, legacy::PassManagerBase &MPM) {
  MPM.add(createObfuscationLegacyPass());
  old_pmb(dis, MPM);
}

ModulePassManager (*old_bo0dp)(void *Level, bool LTOPreLink);
static ModulePassManager new_bo0dp(void *Level, bool LTOPreLink) {
  ModulePassManager MPM = old_bo0dp(Level, LTOPreLink);
  MPM.addPass(ObfuscationPass());
  return MPM;
}

ModulePassManager (*old_bpmdp)(void *Level, bool LTOPreLink);
static ModulePassManager new_bpmdp(void *Level, bool LTOPreLink) {
  ModulePassManager MPM = old_bpmdp(Level, LTOPreLink);
  MPM.addPass(ObfuscationPass());
  return MPM;
}

static __attribute__((__constructor__)) void Inj3c73d(int argc, char *argv[]) {
  char *executablePath = argv[0];
  // Initialize our own LLVM Library
  if (strstr(executablePath, "swift-frontend"))
    errs() << "Applying Apple SwiftC Hooks...\n";
  else
    errs() << "Applying Apple Clang Hooks...\n";
#if defined(__x86_64__)
  int ret = 0;
  size_t size = sizeof(ret);
  if (sysctlbyname("sysctl.proc_translated", &ret, &size, NULL, 0) != -1 &&
      ret == 1)
    errs() << "[Hanabi] Looks like you are currently running the process in "
              "Rosetta 2 mode, which will prevent DobbyHook from "
              "working.\nPlease close it.\n";
#endif
  DobbyHook(DobbySymbolResolver(
                executablePath,
                "__ZN4llvm18PassManagerBuilder25populateModulePassManagerERNS_"
                "6legacy15PassManagerBaseE"),
            (dobby_dummy_func_t)new_pmb, (dobby_dummy_func_t *)&old_pmb);
  DobbyHook(
      DobbySymbolResolver(executablePath,
                          "__ZN4llvm11PassBuilder22buildO0DefaultPipelineENS_"
                          "17OptimizationLevelEb"),
      (dobby_dummy_func_t)new_bo0dp, (dobby_dummy_func_t *)&old_bo0dp);
  DobbyHook(DobbySymbolResolver(
                executablePath,
                "__ZN4llvm11PassBuilder29buildPerModuleDefaultPipelineENS_"
                "17OptimizationLevelEb"),
            (dobby_dummy_func_t)new_bpmdp, (dobby_dummy_func_t *)&old_bpmdp);
}
