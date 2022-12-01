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
        pFileOpen->SetTitle(L"Open examination");
        pFileOpen->SetOkButtonLabel(L"load");

        auto filterVec = GetFilters(fileExtentions);
        pFileOpen->SetFileTypes(filterVec.size(), filterVec.data());

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

void Ull::CreateDataFromDicom(std::wstring filePath)
{
    constexpr auto output = L" Assets/VolumetricData/DICOM.dat";
    std::wstring pathCopy = filePath;
    filePath.clear();

    for (int i = 0; i < pathCopy.size(); ++i)
    {
        if (pathCopy[i] == L'\\')
        {
            filePath += '/';
        }
        else
        {
            filePath += pathCopy[i];
        }
    }

    std::wstring args = filePath + output;

    SHELLEXECUTEINFO ShExecInfo = { 0 };
    ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
    ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
    ShExecInfo.hwnd = NULL;
    ShExecInfo.lpVerb = NULL;
    ShExecInfo.lpFile = L"Assets\\DicomConverter\\DicomConverter.exe";
    ShExecInfo.lpParameters = args.c_str();
    ShExecInfo.lpDirectory = NULL;
    ShExecInfo.nShow = SW_SHOW;
    ShExecInfo.hInstApp = NULL;
    ShellExecuteEx(&ShExecInfo);
    WaitForSingleObject(ShExecInfo.hProcess, INFINITE);
}
