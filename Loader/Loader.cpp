#include "substrate.h"
#include <llvm/Transforms/Obfuscation/Obfuscation.h>
#include <llvm/Support/CommandLine.h>
#include <string>
#include <libgen.h>
using namespace std;
int llvm::DisableABIBreakingChecks=1;
void (*old_pmb)(void* dis,legacy::PassManagerBase &MPM);
void* handle=nullptr;
static void new_pmb(void* dis,legacy::PassManagerBase &MPM){
  printf("Injecting Custom Obfuscation Pass\n");
  MPM.add(createObfuscationPass());
  printf("Added Obfuscation Pass\n");
  old_pmb(dis,MPM);
  printf("Done PMB Hijacking\n");
}
static __attribute__((__constructor__)) void Inj3c73d(int argc, char* argv[]){
  char* executablePath=argv[0];
  //char* folderPath=dirname(executablePath);
  //Initialize our own LLVM Library
  PassRegistry &Registry = *PassRegistry::getPassRegistry();
  initializeCore(Registry);
  MSHookFunction((void*)MSFindSymbol(MSGetImageByName(executablePath),"__ZN4llvm18PassManagerBuilder25populateModulePassManagerERNS_6legacy15PassManagerBaseE"),(void*)new_pmb,(void**)&old_pmb);

}
