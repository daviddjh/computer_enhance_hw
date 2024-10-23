#ifndef _D_OS_WIN32
#define _D_OS_WIN32

#include "../d_os.h"
#include "../d_string.h"
#include "../d_memory.h"

// Windows.h
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include "windows.h"

namespace d_std {

    /////////////////////////////////
    // Memory
    /////////////////////////////////

    u_ptr os_reserve_memory (u64 size){

        u_ptr  memory = (u_ptr)VirtualAlloc(nullptr, size, MEM_RESERVE, PAGE_READWRITE);
        return memory;

    }

    void os_commit_memory (u_ptr memory, u64 size){

        VirtualAlloc((LPVOID)memory, size, MEM_COMMIT, PAGE_READWRITE);
        return;

    }

    void os_decommit_memory(u_ptr memory, u64 size){

        VirtualFree((LPVOID)memory, size, MEM_DECOMMIT);

    }

    void os_release_memory (u_ptr memory){

        VirtualFree((LPVOID)memory, 0, MEM_RELEASE);

    }

    void os_debug_print(const char * string)
    {
        OutputDebugStringA(string);
    }
    
    void os_debug_print(d_string string)
    {

        char c_str[501]; 

        if(string.size > 500){

            OutputDebugStringA("(os_debug_print) Error: string size is more than 500 characters. Printing first 500.\n");

            for(int i = 0; i < 500; i++){
                c_str[i] = string.string[i];
            }

            c_str[500] = '\0';

            OutputDebugStringA(c_str);
            

        } else {

            for(int i = 0; i < string.size; i++){
                c_str[i] = string.string[i];
            }

            c_str[string.size] = '\0';

            OutputDebugStringA(c_str);

        }

        return;

    }

    /////////////////////////////////
    // Print
    /////////////////////////////////

    void __cdecl os_debug_printf(Memory_Arena *arena, char* lit_string, ...){

        // va_list
        u_ptr va_args;

        // va_start
        va_args = (u_ptr)(&lit_string + 1);

        char c_str[501]; 
        c_str[500] = '\0';

        d_string return_string = _format_lit_string(arena, lit_string, va_args);

        os_debug_print(return_string);

        return;

    }

    void __cdecl os_debug_printf(const char* lit_string, ...){

        // va_list
        u_ptr va_args;

        // va_start
        va_args = (u_ptr)(&lit_string + 1);

        char c_str[501] = {0}; 

        _format_lit_string(c_str, 500, lit_string, va_args);

        c_str[500] = '\0';

        OutputDebugStringA(c_str);

        return;

    }

    /////////////////////////////////
    // Debug
    /////////////////////////////////

    bool is_debugger_present(){
        return  (bool) IsDebuggerPresent();
    }

    /////////////////////////////////
    // Files
    /////////////////////////////////

    struct File {
        HANDLE handle;
        u32 size;
    };

    File open_file(const char* file_name, const char* permissions){

        DWORD dwDesiredAccess = 0;

        if(permissions != NULL){
            // First Char
            if(*permissions == 'r'){
                dwDesiredAccess |= GENERIC_READ;
            } else if (*permissions == 'w') {
                dwDesiredAccess |= GENERIC_WRITE;
            }
            permissions++;
            // Second Char
            if(*permissions == 'r'){
                dwDesiredAccess |= GENERIC_READ;
            } else if (*permissions == 'w') {
                dwDesiredAccess |= GENERIC_WRITE;
            }
        }


        HANDLE os_file_handle = CreateFileA(
            file_name,
            dwDesiredAccess,
            0,
            NULL,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            NULL
        );

        if(os_file_handle == INVALID_HANDLE_VALUE){
            os_debug_printf("open_file failed to create file handle! last error: %u", GetLastError());
            File error_file;
            error_file.handle = INVALID_HANDLE_VALUE;
            error_file.size   = INVALID_FILE_SIZE;
            return error_file;
        }

        DWORD file_size = GetFileSize(os_file_handle, NULL);
        if(file_size == INVALID_FILE_SIZE){
            os_debug_printf("open_file failed to get file size! last error: %u", GetLastError());
            File error_file;
            error_file.handle = INVALID_HANDLE_VALUE;
            error_file.size   = INVALID_FILE_SIZE;
            return error_file;
        }

        File return_file;
        return_file.handle = os_file_handle;
        return_file.size   = file_size;

        return return_file;
    }

    d_string read_file(Memory_Arena* arena, File file, u32 bytes_to_read){
        d_string return_string;
        return_string.string = arena->allocate_array<char>(bytes_to_read);

        DWORD num_bytes_read = 0;

        BOOL result = ReadFile(
            file.handle,
            return_string.string,
            bytes_to_read,
            &num_bytes_read,
            NULL
        );

        if(result == 0){
            arena->deallocate_array(return_string.string);
            return_string.string = NULL;
            return_string.size   = 0;
            return return_string;
        }

        return_string.size = num_bytes_read;

        arena->deallocate((u_ptr)return_string.string + num_bytes_read);

        return return_string;
    }

    bool close_file(File file){
        if(CloseHandle(file.handle) == 0){
            os_debug_printf("close_file failed!", GetLastError());
            return false;
        }
        return true;
    }

}

#endif // _D_OS_WIN32