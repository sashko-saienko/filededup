#ifndef __UIDEDUP_MAIN_FRAME_H__
#define __UIDEDUP_MAIN_FRAME_H__

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
#include <image_file_preview.h>

namespace Deduplicator::Ui
{

namespace fs = std::filesystem;

enum
{
    ID_SELECTFOLDER = wxID_HIGHEST + 1,
    ID_REFRESH = wxID_HIGHEST + 2,
    ID_DUPLICATES_TREE = wxID_HIGHEST + 3,
    ID_DELETE_SELECTED = wxID_HIGHEST + 4,
    ID_CLEAR_SELECTED = wxID_HIGHEST + 5
};

class MainFrame : public ::wxFrame
{
  public:
    MainFrame();

  private:
    void OnExit(::wxCommandEvent& event);
    void OnAbout(::wxCommandEvent& event);
    void OnItemSelected(::wxTreeEvent& event);
    void OnSelectFolder(wxCommandEvent& event);
    void OnRefresh(wxCommandEvent& event);
    void OnDeleteChecked(wxCommandEvent& event);
    void OnClearSelected(wxCommandEvent& event);
    void ToggleActionsOverSelectedItems(wxCommandEvent& event);

    void SetupMenuBar();
    void SetupToolBar();
    void SetupMainLayout();
    void SetupEventBinds();
    void SetDuplicatesTree();
    void SetRefreshOptionState();
    void SetActionsOverSelectedOptionsState(bool isEnabled);

    DuplicatesTreeCtrl* m_treeCtrl;
    wxToolBar* m_toolBar;
    ImageFilePreview* m_previewImage;
    wxPanel* m_previewPanel;
    wxBoxSizer* m_previewSizer;

    std::optional<fs::path> _targetFolder;
};

} // namespace Deduplicator::Ui

#endif // __UIDEDUPLIB_MAIN_FRAME_H__UIDEDUPLIB_MAIN_FRAME_H__