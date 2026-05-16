#ifndef __UIDEDUP_MAIN_FRAME_H__
#define __UIDEDUP_MAIN_FRAME_H__

#include <wx/splitter.h>
#include <wx/treectrl.h>
#include <wx/wx.h>

namespace Deduplicator::Ui
{

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
    void PopulateSampleData();

    wxTreeCtrl* m_treeCtrl;
    wxTextCtrl* m_previewText;
};

} // namespace Deduplicator::Ui

#endif // __UIDEDUPLIB_MAIN_FRAME_H__UIDEDUPLIB_MAIN_FRAME_H__