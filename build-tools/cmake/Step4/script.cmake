set(gcc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU,LCC>")
message("gcc_like_cxx evaluated: $<IF:${gcc_like_cxx},1,0>")
