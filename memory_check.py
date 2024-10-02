# usage:
# sudo apt install python-lldb
# command script import ./memory_check.py
# command script add -f memory_check.check_memory_and_continue checkit
# breakpoint command add 1
# >checkit
# >DONE

import lldb

def check_memory_and_continue(debugger, command, result, internal_dict):
    # Define memory range (start and end address)
    target_variable = "buf"
    
    # Get the memory range (you can replace this with actual addresses if needed)
    frame = debugger.GetSelectedTarget().GetProcess().GetSelectedThread().GetSelectedFrame()
    buf_addr = frame.FindVariable(target_variable).GetValueAsUnsigned()  # Get the address of 'buf'
    search_range = 4096
    end_addr = buf_addr + search_range
    
    # The pattern to search for
    #target_string = "define linkonce_odr hidden noundef nonnull align 8 dereferenceable(88) ptr @_ZNK5clang12FileEntryRef12getFileEntryEv"
    # Perform the memory search
    #command_string = 'memory find -s "{target_string}" {buf_addr} {end_addr}'
    command_string = 'memory find -s "%8 = call noundef ptr @_ZNK4llvm12PointerUnionIJPN5clang9FileEntryEPKNS_14StringMapEntryINS_7ErrorOrINS1_12FileEntryRef8MapVa" buf buf+4096'
    #command_string = 'mem find -s "class" buf buf+4096'
    result_obj = lldb.SBCommandReturnObject()
    debugger.GetCommandInterpreter().HandleCommand(command_string, result_obj)
    
    # Parse the result
    output = result_obj.GetOutput()
    
    if "data not" in output:
        # Pattern is not found, continue execution
        print("Pattern not found, continuing execution.")
        #debugger.GetSelectedTarget().GetProcess().Continue()
    else:
        # Pattern found, stop here
        print(f"Pattern found in memory range {hex(buf_addr)} - {hex(end_addr)}.")
        print(output)
        debugger.GetSelectedTarget().GetProcess().Continue() 
