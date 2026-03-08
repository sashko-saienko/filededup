function(AddClangTidy target)
    find_program(CLANG_TIDY_PATH clang-tidy REQUIRED)
    set_target_properties(${target} PROPERTIES CXX_LANG_TIDY "${CLANG_TIDY_PATH};-checks=*;--warnings-as-errors=*")
endfunction()