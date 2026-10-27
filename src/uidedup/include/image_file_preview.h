// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Sashko

#ifndef __UIDEDUP_IMAGE_FILE_PREVIEW_H__
#define __UIDEDUP_IMAGE_FILE_PREVIEW_H__

#include <wx/generic/statbmpg.h>
#include <wx/string.h>

namespace Deduplicator::Ui
{

class ImageFilePreview : public wxGenericStaticBitmap
{
  public:
    ImageFilePreview(wxWindow* parent, wxWindowID id, const wxBitmap& bitmap = wxBitmap())
        : wxGenericStaticBitmap(parent, id, bitmap)
    {
    }

    bool LoadImage(const wxString& imagePath);
};

} // namespace Deduplicator::Ui

#endif // __UIDEDUP_IMAGE_FILE_PREVIEW_H__