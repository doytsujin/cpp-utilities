if(NOT DEFINED LIST_TO_STRING_EXISTS)
    set(LIST_TO_STRING_EXISTS true)
    function(list_to_string separator prefix input_list output_string_var)
        set(res "")
        # get list length
        list(LENGTH input_list list_length)
        # if the list has 0 or 1 element, there is no need to loop over
        if(list_length LESS 2)
            set(res "${input_list}")
        else()
            math(EXPR last_element_index "${list_length} - 1")
            foreach(index RANGE ${last_element_index})
                # get current item_value
                list(GET input_list ${index} item_value)
                if(NOT item_value STREQUAL "")
                    # .. and append non-empty value to output string
                    set(res "${res}${prefix}${item_value}")
                    # append separator if current element is NOT the last one.
                    if(NOT index EQUAL last_element_index)
                        set(res "${res}${separator}")
                    endif()
                endif()
            endforeach()
        endif()
        set(${output_string_var} "${res}" PARENT_SCOPE)
    endfunction()
endif()