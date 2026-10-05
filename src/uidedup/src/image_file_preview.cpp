//
// Created by sashko on 05.10.26.
//
#include <wx/filefn.h>
#include <wx/generic/statbmpg.h>
#include <wx/image.h>
#include <wx/msgdlg.h>
#include <wx/wx.h>

#include <image_file_preview.h>

namespace Deduplicator::Ui
{

void ImageFilePreview::LoadImage(const wxString& imagePath)
{
    // Load and display the image
    if (wxFileExists(imagePath) == false)
    {
        auto errorMessage = wxString::Format("Image file does not exist: %s", imagePath);
        wxMessageBox(errorMessage, "Error finding image to open it for preview", wxOK | wxCENTER,
                     wxTheApp->GetTopWindow());
        return;
    }

    wxImage image;
    if (image.LoadFile(imagePath) == false)
    {
        auto errorMessage = wxString::Format("Image file not supported: %s", imagePath);
        wxMessageBox(errorMessage, "Error opening image for preview", wxOK | wxCENTER,
                     wxTheApp->GetTopWindow());
        return;
    }

    wxBitmap bitmap(image);
    SetScaleMode(wxStaticBitmapBase::Scale_AspectFit);
    SetBitmap(bitmap);
    Show();
}

} // namespace Deduplicator::Ui