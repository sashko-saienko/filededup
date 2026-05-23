#ifndef __UIDEDUP_MAIN_FRAME_H__
#define __UIDEDUP_MAIN_FRAME_H__

#include <deduplib/files_map.h>
#include <filesystem>
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
};

} // namespace Deduplicator::Ui

#endif // __UIDEDUPLIB_MAIN_FRAME_H__UIDEDUPLIB_MAIN_FRAME_H__