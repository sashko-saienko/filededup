#ifndef __UIDEDUP_MAIN_FRAME_H__
#define __UIDEDUP_MAIN_FRAME_H__

#include <deduplib/files_map.h>
#include <filesystem>
#include <optional>
#include <unordered_set>
#include <wx/event.h>
#include <wx/generic/statbmpg.h>
#include <wx/image.h>
#include <wx/sizer.h>
#include <wx/splitter.h>
#include <wx/statbmp.h>
#include <wx/wx.h>

#include <duplicates_tree_ctrl.h>

namespace Deduplicator::Ui
{

namespace fs = std::filesystem;

enum
{
    ID_SELECTFOLDER = wxID_HIGHEST + 1,
    ID_REFRESH = wxID_HIGHEST + 2
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
    void OnItemActivated(::wxTreeEvent& event);
    void OnSelectFolder(wxCommandEvent& event);
    void OnRefresh(wxCommandEvent& event);

    void SetupMenuBar();
    void SetupToolBar();
    void SetupMainLayout();
    void SearchDuplicates();
    bool EnumerateTargetFolder(const std::string& targetDirectory);
    void previewImage(const wxString& imagePath);
    void UpdateRefreshButtonState();

    DuplicatesTreeCtrl* m_treeCtrl;
    wxToolBar* m_toolBar;
    wxGenericStaticBitmap* m_previewImage;
    wxPanel* m_previewPanel;
    wxBoxSizer* m_previewSizer;

    std::optional<fs::path> _targetFolder;
};

} // namespace Deduplicator::Ui

#endif // __UIDEDUPLIB_MAIN_FRAME_H__UIDEDUPLIB_MAIN_FRAME_H__