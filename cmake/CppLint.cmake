function(AddCppLint target)
    find_program(CPPLINT_PATH cpplint REQUIRED)
    set_target_properties(${target} PROPERTIES CXX_CPPLINT "${CPPLINT_PATH}")
endfunction()