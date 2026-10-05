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

    void LoadImage(const wxString& imagePath);
};

} // namespace Deduplicator::Ui

#endif // __UIDEDUP_IMAGE_FILE_PREVIEW_H__