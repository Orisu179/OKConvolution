/*******************************************************************************
The content of this file includes portions of the AUDIOKINETIC Wwise Technology
released in source code form as part of the SDK installer package.

Commercial License Usage

Licensees holding valid commercial licenses to the AUDIOKINETIC Wwise Technology
may use this file in accordance with the end user license agreement provided
with the software or, alternatively, in accordance with the terms contained in a
written agreement between you and Audiokinetic Inc.

Apache License Usage

Alternatively, this file may be used under the Apache License, Version 2.0 (the
"Apache License"); you may not use this file except in compliance with the
Apache License. You may obtain a copy of the Apache License at
http://www.apache.org/licenses/LICENSE-2.0.

Unless required by applicable law or agreed to in writing, software distributed
under the Apache License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES
OR CONDITIONS OF ANY KIND, either express or implied. See the Apache License for
the specific language governing permissions and limitations under the License.

  Copyright (c) 2026 Audiokinetic Inc.
*******************************************************************************/

#include "OKConvolutionPluginGUI.h"
#include <commdlg.h> // For GetOpenFileName
#define IDC_BUTTON_BROWSE_IR 1001

OKConvolutionPluginGUI::OKConvolutionPluginGUI()
{
}

OKConvolutionPluginGUI::~OKConvolutionPluginGUI()
{
}

HINSTANCE OKConvolutionPluginGUI::GetResourceHandle() const
{
    return GUIWindows::GetResourceHandle();
}

bool OKConvolutionPluginGUI::GetDialog(AK::Wwise::Plugin::eDialog in_eDialog, uint32_t& out_uiDialogID,
    AK::Wwise::Plugin::PopulateTableItem*& out_pTable) const
{
    return true;
}

bool OKConvolutionPluginGUI::WindowProc(AK::Wwise::Plugin::eDialog in_eDialog, HWND in_hWnd, uint32_t in_message,
    WPARAM in_wParam, LPARAM in_lParam, LRESULT& out_lResult)
{
    if (in_message == WM_COMMAND && LOWORD(in_wParam) == IDC_BUTTON_BROWSE_IR)
    {
        wchar_t szFilePath[MAX_PATH] = L"";

        OPENFILENAMEW ofn = { sizeof(ofn) };
        ofn.hwndOwner = in_hWnd;
        ofn.lpstrFilter = L"WAV Files (*.wav)\0*.wav\0All Files (*.*)\0*.*\0";
        ofn.lpstrFile = szFilePath;
        ofn.nMaxFile = MAX_PATH;
        ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

        if (GetOpenFileNameW(&ofn))
        {
            // Index 0 represents the first media slot for this plugin instance.
            // If you support multiple IR slots, use the appropriate index (e.g. ActiveIRIndex).
            const unsigned int uMediaIndex = 0;

            // Assign the WAV file as the media source.
            // Setting in_bUseAsOriginal to true copies or references the file into the project's Originals.
            m_objectMedia.SetMediaSource(szFilePath, uMediaIndex, true);
        }

        out_lResult = 0;
        return true;
    }

    return false;
}

bool OKConvolutionPluginGUI::Help(HWND in_hWnd, AK::Wwise::Plugin::eDialog in_eDialog,
    const char* in_szLanguageCode) const
{
    return GUIWindows::Help(in_hWnd, in_eDialog, in_szLanguageCode);
}

void OKConvolutionPluginGUI::NotifyPluginMediaChanged()
{
    RequestedHostInterface<AK::Wwise::Plugin::ObjectMedia>::NotifyPluginMediaChanged();
}

AK_ADD_PLUGIN_CLASS_TO_CONTAINER(
    OKConvolution, // Name of the plug-in container for this shared library
    OKConvolutionPluginGUI, // Authoring plug-in class to add to the plug-in container
    OKConvolutionFX // Corresponding Sound Engine plug-in class
);
