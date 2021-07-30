/*++
Copyright (c) Microsoft Corporation
Licensed under the MIT license.

Module Name:
- conimeinfo.h

Abstract:
- This module contains the structures for the console IME entrypoints
  for overall control

Author:
- Michael Niksa (MiNiksa) 10-May-2018

Revision History:
- From pieces of convarea.cpp originally authored by KazuM
--*/

#pragma once

#include "../inc/conime.h"
#include "../buffer/out/OutputCell.hpp"
#include "../buffer/out/TextAttribute.hpp"
#include "../renderer/inc/FontInfo.hpp"
#include "../buffer/out/Row.hpp"
#include "../types/inc/viewport.hpp"

#include "conareainfo.h"

class SCREEN_INFORMATION;

class ConsoleImeInfo final
{
public:
    // IME composition string information
    // There is one "composition string" per line that must be rendered on the screen
    std::vector<ConversionAreaInfo> ConvAreaCompStr;

    ConsoleImeInfo();
    ~ConsoleImeInfo() = default;
    ConsoleImeInfo(const ConsoleImeInfo&) = delete;
    ConsoleImeInfo(ConsoleImeInfo&&) = delete;
    ConsoleImeInfo& operator=(const ConsoleImeInfo&) & = delete;
    ConsoleImeInfo& operator=(ConsoleImeInfo&&) & = delete;

    void RefreshAreaAttributes();
    void ClearAllAreas();

    [[nodiscard]] HRESULT ResizeAllAreas(const COORD newSize);

    void WriteCompMessage(const std::wstring_view text,
                          const gsl::span<const BYTE> attributes,
                          const gsl::span<const WORD> colorArray);

    void WriteResultMessage(const std::wstring_view text);

    void RedrawCompMessage();

    void SaveCursorVisibility();
    void RestoreCursorVisibility();

private:
    [[nodiscard]] HRESULT _AddConversionArea();

    void _ClearComposition();

    void _WriteUndeterminedChars(const std::wstring_view text,
                                 const gsl::span<const BYTE> attributes,
                                 const gsl::span<const WORD> colorArray);

    void _InsertConvertedString(const std::wstring_view text);

    static TextAttribute s_RetrieveAttributeAt(const size_t pos,
                                               const gsl::span<const BYTE> attributes,
                                               const gsl::span<const WORD> colorArray);

    static RowImage s_ConvertToCells(std::wstring_view text,
                                     const gsl::span<const BYTE> attributes,
                                     const gsl::span<const WORD> colorArray);

    void _WriteConversionArea(const RowImage& cells,
                              COORD pos,
                              const Microsoft::Console::Types::Viewport view,
                              SCREEN_INFORMATION& screenInfo);

    bool _isSavedCursorVisible;

    std::wstring _text;
    std::vector<BYTE> _attributes;
    std::vector<WORD> _colorArray;
};
