
// source https://github.com/metacall/core/blob/433e3107112b6d8362aa03057dae20b96ff5b1a8/source/loaders/node_loader/source/node_loader_impl.cpp#L3413

#include <Windows.h>
#include <MinHook.h>

#include "Winfixes.hpp"

/* Required for the DelayLoad hook interposition, solves bug of NodeJS extensions requiring node.exe instead of node.dll */
static HMODULE (*get_module_handle_a_ptr)(_In_opt_ LPCSTR) = NULL;

#include <intrin.h>
#pragma intrinsic(_ReturnAddress)

_Ret_maybenull_ HMODULE WINAPI get_module_handle_a_hook(_In_opt_ LPCSTR lpModuleName)
{
    /* This hooks GetModuleHandle, which is called as DelayLoad hook inside NodeJS
     * extensions in order to retrieve the executable handle, which is supposed
     * to have all N-API symbols. This trick is used because the design of NodeJS forces
     * to compile statically node.dll into the executable, but this does not happen on
     * MetaCall as it is embedded. We cannot change this behavior because it depends on
     * NodeJS extension build system, which relies on DelayLoad mechanism. So what we are
     * doing here is intercepting the GetModuleHandle call inside the DelayLoad hook, then
     * getting the address from where this Win32 API was called, and if it commes from a
     * NodeJS extension, then we return the node.dll module, otherwise we call to the original
     * GetModuleHandle funciton. This method successfully hooks into the NodeJS mechanism and
     * redirects properly the linker resolver system to the node.dll where symbols are located.
     */
    if (lpModuleName == NULL)
    {
        HMODULE mod = NULL;

        if (GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
                                  GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, /* Behave like GetModuleHandle */
                (LPCTSTR)_ReturnAddress(),
                &mod) == TRUE)
        {
            static const char node_ext[] = ".node";
            char              mod_name[MAX_PATH];
            size_t            length = GetModuleFileName(mod, mod_name, MAX_PATH);

            /* It must contain a letter a part from the .node extension */
            if (length > sizeof(node_ext))
            {
                char* ext = &mod_name[length - sizeof(node_ext) + 1];

                if (strncmp(ext, node_ext, sizeof(node_ext)) == 0)
                {
                    return get_module_handle_a_ptr("libnode.dll");
                }
            }
        }
    }

    return get_module_handle_a_ptr(lpModuleName);
}

bool DoWinFixes()
{
    if (MH_Initialize() != MH_OK)
    {
        return false;
    }

    if (MH_CreateHook(&GetModuleHandleA, &get_module_handle_a_hook, reinterpret_cast<LPVOID*>(&get_module_handle_a_ptr)) != MH_OK)
    {
        return false;
    }

    MH_EnableHook(&GetModuleHandleA);

    return true;
}
