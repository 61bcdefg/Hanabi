// For open-source license, please refer to [License](https://github.com/HikariObfuscator/Hikari/wiki/License).
#include "substrate.h"
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
ModulePassManager (*old_bo0dp)(OptimizationLevel Level, bool LTOPreLink);
static ModulePassManager new_bo0dp(OptimizationLevel Level, bool LTOPreLink) {
  ModulePassManager MPM = old_bo0dp(Level, LTOPreLink);
  MPM.addPass(ObfuscationPass());
  return MPM;
}
ModulePassManager (*old_bpmdp)(OptimizationLevel Level, bool LTOPreLink);
static ModulePassManager new_bpmdp(OptimizationLevel Level, bool LTOPreLink) {
  ModulePassManager MPM = old_bpmdp(Level, LTOPreLink);
  MPM.addPass(ObfuscationPass());
  return MPM;
}
ModulePassManager (*old_btlpldp)(OptimizationLevel Level);
static ModulePassManager new_btlpldp(OptimizationLevel Level) {
  ModulePassManager MPM = old_btlpldp(Level);
  MPM.addPass(ObfuscationPass());
  return MPM;
}
ModulePassManager (*old_btldp)(OptimizationLevel Level, void *ImportSummary);
static ModulePassManager new_btldp(OptimizationLevel Level, void *ImportSummary) {
  ModulePassManager MPM = old_btldp(Level, ImportSummary);
  MPM.addPass(ObfuscationPass());
  return MPM;
}
ModulePassManager (*old_bldp)(OptimizationLevel Level, void *ImportSummary);
static ModulePassManager new_bldp(OptimizationLevel Level, void *ImportSummary) {
  ModulePassManager MPM = old_bldp(Level, ImportSummary);
  MPM.addPass(ObfuscationPass());
  return MPM;
}

static __attribute__((__constructor__)) void Inj3c73d(int argc, char* argv[]){
  char* executablePath=argv[0];
  //Initialize our own LLVM Library
  MSImageRef exeImagemage = MSGetImageByName(executablePath);
  if (strstr(executablePath, "swift-frontend"))
    errs() << "Applying Apple SwiftC Hooks...\n";
  else
    errs() << "Applying Apple Clang Hooks...\n";
  MSHookFunction(MSFindSymbol(exeImagemage, "__ZN4llvm18PassManagerBuilder25populateModulePassManagerERNS_6legacy15PassManagerBaseE"), (void *)new_pmb, (void **)&old_pmb);
  MSHookFunction(MSFindSymbol(exeImagemage, "__ZN4llvm11PassBuilder22buildO0DefaultPipelineENS_17OptimizationLevelEb"), (void *)new_bo0dp, (void **)&old_bo0dp);
  MSHookFunction(MSFindSymbol(exeImagemage, "__ZN4llvm11PassBuilder29buildPerModuleDefaultPipelineENS_17OptimizationLevelEb"), (void *)new_bpmdp, (void **)&old_bpmdp);
  MSHookFunction(MSFindSymbol(exeImagemage, "__ZN4llvm11PassBuilder34buildThinLTOPreLinkDefaultPipelineENS_17OptimizationLevelE"), (void *)new_btlpldp, (void **)&old_btlpldp);
  MSHookFunction(MSFindSymbol(exeImagemage, "__ZN4llvm11PassBuilder27buildThinLTODefaultPipelineENS_17OptimizationLevelEPKNS_18ModuleSummaryIndexE"), (void *)new_btldp, (void **)&old_btldp);
  MSHookFunction(MSFindSymbol(exeImagemage, "__ZN4llvm11PassBuilder23buildLTODefaultPipelineENS_17OptimizationLevelEPNS_18ModuleSummaryIndexE"), (void *)new_bldp, (void **)&old_bldp);
}
