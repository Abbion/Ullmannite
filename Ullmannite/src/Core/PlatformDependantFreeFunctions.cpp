#include "Ullpch.h"
#include "PlatformDependantFreeFunctions.h"
#include <Windows.h>
#include <shobjidl.h> 

using namespace Ull;

namespace
{
    std::vector<COMDLG_FILTERSPEC> GetFilters(FileExtentions fileExtentions)
    {
        std::vector<COMDLG_FILTERSPEC> fileFilters;

        if (fileExtentions & FileExtentions::DAT)
            fileFilters.push_back(COMDLG_FILTERSPEC{ L"Raw Data", L"*.dat" });
        if (fileExtentions & FileExtentions::DICOM)
            fileFilters.push_back(COMDLG_FILTERSPEC{ L"Dicom", L"*.dcm" });
        return fileFilters;
    }
}

std::optional<std::wstring> Ull::CreateFileOpenDialog(FileExtentions fileExtentions)
{
    std::optional<std::wstring> filePath;

    HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED |
        COINIT_DISABLE_OLE1DDE);

    if (SUCCEEDED(hr))
    {
        IFileOpenDialog* pFileOpen;

        hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));
        pFileOpen->SetOkButtonLabel(L"load");

        if (fileExtentions & FileExtentions::FOLDER) 
        {
            pFileOpen->SetTitle(L"Select examination folder");

            DWORD dwFlags;
            hr = pFileOpen->GetOptions(&dwFlags);
            if (SUCCEEDED(hr))
                pFileOpen->SetOptions(dwFlags | FOS_PICKFOLDERS | FOS_FORCEFILESYSTEM);
            else
            {
                CoUninitialize();
                pFileOpen->Release();
                return std::nullopt;
            }
        }
        else
        {
            pFileOpen->SetTitle(L"Select examination file");
            const auto filterVec = GetFilters(fileExtentions);
            pFileOpen->SetFileTypes(static_cast<UINT>(filterVec.size()), filterVec.data());
        }

        if (SUCCEEDED(hr))
        {
            hr = pFileOpen->Show(NULL);

            if (SUCCEEDED(hr))
            {
                IShellItem* pItem;
                hr = pFileOpen->GetResult(&pItem);
                if (SUCCEEDED(hr))
                {
                    PWSTR pszFilePath;
                    hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

                    if (SUCCEEDED(hr))
                    {
                        filePath = std::wstring(pszFilePath);
                        CoTaskMemFree(pszFilePath);
                    }
                    pItem->Release();
                }
            }
            pFileOpen->Release();
        }
        CoUninitialize();
    }

    return filePath;
}
