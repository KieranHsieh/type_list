function(get_latest_git_tag)
    set(ONE_VAL_ARGS DEFAULT OUTPUT)
    cmake_parse_arguments(INFO "" "${ONE_VAL_ARGS}" "" ${ARGN})
    
    find_program(GIT_BIN git)
    if(NOT GIT_BIN)
        set(${INFO_OUTPUT} ${INFO_DEFAULT} PARENT_SCOPE)
        return()
    endif()
    # Put tags into a list
    execute_process(COMMAND ${GIT_BIN} tag -l --sort=version:refname
        RESULT_VARIABLE GIT_RETURN
        OUTPUT_VARIABLE GIT_TAG
        ERROR_VARIABLE  GIT_ERR_MSG
    )
    # Check for errors when listing tags
    if(NOT GIT_RETURN EQUAL 0)
        message(WARNING "Failed to get latest Git tag: ${GIT_ERR_MSG}")
        set(${INFO_OUTPUT} ${INFO_DEFAULT} PARENT_SCOPE)
        return()
    # Check if there are no tags
    elseif(NOT GIT_TAG)
        message(WARNING "Failed to get latest Git tag: No git tag for current repository")
        set(${INFO_OUTPUT} ${INFO_DEFAULT} PARENT_SCOPE)
        return()
    endif()
    # Convert command output to a CMake compatible list
    string(REPLACE "\n" ";" GIT_TAG "${GIT_TAG}")
    list(GET GIT_TAG 0 GIT_TAG)

    # Check tag format of "vX.X.X"
    string(REGEX MATCH "^v[0-9]+\.[0-9]+\.[0-9]+" GIT_TAG_PREFIXED "${GIT_TAG}")
    # If tag isn't prefixed by "v"
    if(NOT GIT_TAG_PREFIXED)
        set(${INFO_OUTPUT} ${GIT_TAG} PARENT_SCOPE)
    endif()
    
    # Check tag format of "X.X.X"
    string(REGEX_MATCH "^[0-9]+\.[0-9]+\.[0-9]+" GIT_TAG_CORRECT_FORMAT "${GIT_TAG}")
    if(NOT GIT_TAG_CORRECT_FORMAT)
        set(${INFO_OUTPUT} ${INFO_DEFAULT} PARENT_SCOPE)
        return()
    endif()

    string(SUBSTRING "${GIT_TAG}" 1 -1 GIT_TAG)
    set(${INFO_OUTPUT} ${GIT_TAG} PARENT_SCOPE)
endfunction()