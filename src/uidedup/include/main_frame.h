#ifndef __UIDEDUP_MAIN_FRAME_H__
#define __UIDEDUP_MAIN_FRAME_H__

#include <deduplib/files_map.h>
#include <filesystem>
#include <unordered_set>
#include <wx/image.h>
#include <wx/sizer.h>
#include <wx/splitter.h>
#include <wx/statbmp.h>
#include <wx/treectrl.h>
#include <wx/wx.h>

namespace Deduplicator::Ui
{

namespace fs = std::filesystem;

class MainFrame : public ::wxFrame
{
  public:
    MainFrame();
    void Initialize();

  private:
    void OnExit(::wxCommandEvent& event);
    void OnAbout(::wxCommandEvent& event);
    void OnItemSelected(::wxTreeEvent& event);

    void SetupMenuBar();
    void SetupMainLayout();
    void PopulateDuplicatesTree();
    void SetupImagePreview(const wxString& imagePath);

    wxTreeCtrl* m_treeCtrl;
    wxStaticText* m_previewLabel;
    wxStaticBitmap* m_previewImage;
    wxPanel* m_previewPanel;
    wxBoxSizer* m_previewSizer;

    fs::path _targetDirectory{"/Users/sashko/Documents/testbedduplicates"};
    Deduplicator::ChecksumFilesMap cfm;

    inline const static std::unordered_set<std::string> supportedImageExtensions{
        ".bmp",  ".jpg",  ".jpeg", ".png", ".gif", ".pcx", ".pnm",
        ".tiff", ".tgah", ".iff",  ".xpm", ".ico", ".cur", ".anih"};
};

} // namespace Deduplicator::Ui

#endif // __UIDEDUPLIB_MAIN_FRAME_H__UIDEDUPLIB_MAIN_FRAME_H__