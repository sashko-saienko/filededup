#ifndef __UIDEDUP_MAIN_FRAME_H__
#define __UIDEDUP_MAIN_FRAME_H__

#include <deduplib/files_map.h>
#include <filesystem>
#include <unordered_set>
#include <wx/generic/statbmpg.h>
#include <wx/image.h>
#include <wx/sizer.h>
#include <wx/splitter.h>
#include <wx/statbmp.h>
#include <wx/treectrl.h>
#include <wx/wx.h>

namespace Deduplicator::Ui
{

namespace fs = std::filesystem;

enum
{
    ID_SELECTFOLDERTLBR = wxID_HIGHEST + 1,
    ID_REFRESHTLBR = wxID_HIGHEST + 2
};

class MainFrame : public ::wxFrame
{
  public:
    MainFrame();
    void Initialize();

  private:
    void OnExit(::wxCommandEvent& event);
    void OnAbout(::wxCommandEvent& event);
    void OnItemSelected(::wxTreeEvent& event);
    void OnSelectFolder(wxCommandEvent& event);
    void OnRefresh(wxCommandEvent& event);

    void SetupMenuBar();
    void SetupToolBar();
    void SetupMainLayout();
    void PopulateDuplicatesTree(bool resetRoot = false);
    void previewImage(const wxString& imagePath);
    bool IsFileItem(const wxTreeItemId& item) const;

    wxTreeCtrl* m_treeCtrl;
    wxToolBar* m_toolBar;
    wxGenericStaticBitmap* m_previewImage;
    wxPanel* m_previewPanel;
    wxBoxSizer* m_previewSizer;

    fs::path _targetDirectory{"/home/sashko/Documents/playground"};
    Deduplicator::ChecksumFilesMap cfm;
};

} // namespace Deduplicator::Ui

#endif // __UIDEDUPLIB_MAIN_FRAME_H__UIDEDUPLIB_MAIN_FRAME_H__