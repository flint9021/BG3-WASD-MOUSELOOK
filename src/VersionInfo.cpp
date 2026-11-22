#include <codecvt>
#include <locale>

#include "VersionInfo.hpp"

void VersionInfo::Print(HMODULE a_hModule)
{
    PrintGameVersion();
    PrintModVersion(a_hModule);
}

void VersionInfo::PrintModVersion(HMODULE a_hModule)
{
    WCHAR mod_dll_file_name[_MAX_PATH];
    GetModuleFileName(a_hModule, mod_dll_file_name, _MAX_PATH);
    VersionInfo::PrintModuleVersion(mod_dll_file_name);
}

void VersionInfo::PrintGameVersion()
{
    WCHAR game_exe_file_name[_MAX_PATH];
    GetModuleFileName(nullptr, game_exe_file_name, _MAX_PATH);
    VersionInfo::PrintModuleVersion(game_exe_file_name);
}

void VersionInfo::PrintModuleVersion(std::wstring file_name)
{
    DWORD size = 0;
    DWORD handle = 0;
    size = GetFileVersionInfoSize(file_name.c_str(), &handle);
    BYTE* versionInfo = new BYTE[size];
    if (!GetFileVersionInfo(file_name.c_str(), handle, size, versionInfo))
    {
        delete[] versionInfo;
        return;
    }
    UINT len = 0;
    VS_FIXEDFILEINFO* vsfi = NULL;
    VerQueryValue(versionInfo, L"\\", (void**)&vsfi, &len);
    int aVersion[4];
    aVersion[0] = HIWORD(vsfi->dwFileVersionMS);
    aVersion[1] = LOWORD(vsfi->dwFileVersionMS);
    aVersion[2] = HIWORD(vsfi->dwFileVersionLS);
    aVersion[3] = LOWORD(vsfi->dwFileVersionLS);
    using convert_typeX = std::codecvt_utf8<wchar_t>;
    std::wstring_convert<convert_typeX, wchar_t> converterX;
    std::string file_name_narrow = converterX.to_bytes(file_name);
    INFO("{} Version: {}.{}.{}.{}", file_name_narrow, aVersion[0], aVersion[1], aVersion[2],
        aVersion[3]);
    delete[] versionInfo;
}
