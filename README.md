# 夏恋花火
Hassle-free Obfuscator-Enabled Apple Clang without any sort of compromise. More to come later

# Must be this tall to ride
Due to its hackish nature (Which is why I don't want to do this in the first place), you should probably know some LLVM/macOS Hooking/Binary Patching and stuff to debug this thing

# Building

## Core
Create a folder called ``build/`` in project root, inside it build the attached LLVM with ``cmake ../LLVM -DCMAKE_BUILD_TYPE=Release -DLLVM_ABI_BREAKING_CHECKS=FORCE_OFF -G Ninja`` and ``ninja libLLVM.dylib``. Copy ``build/lib/libLLVM.dylib`` to ``/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/``

## Loader
Create a folder called ``build/`` in ``Loader/``, inside it build the Loader with `` cmake -DCMAKE_BUILD_TYPE=Release ../`` and ``ninja``.
Copy ``Loader/build/libInjector.dylib`` and ``Loader/libsubstitute.dylib`` to ``/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/``

# Patching

You need to build ``https://github.com/alexzielenski/optool`` and put it in your $PATH, then
``optool install -c load -p @executable_path/libInjector.dylib -t /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/clang`` (Remember to backup your original Clang first)

# Using

Due to many LLVM internal design choices, you can no longer pass options from command line and instead you'll have to use environment variables. Currently it supports the following:  

- SPLITOBF EnableBasicBlockSplit
- SUBOBF EnableSubstitution
- ALLOBF EnableAllObfuscation
- FCO EnableFunctionCallObfuscate
- STRCRY EnableStringEncryption
- INDIBRAN EnableIndirectBranching
- FUNCWRA EnableFunctionWrapper
- BCFOBF EnableBogusControlFlow
- ACDOBF EnableAntiClassDump
- CFFOBF EnableFlattening
