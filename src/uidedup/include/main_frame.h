#ifndef __UIDEDUP_MAIN_FRAME_H__
#define __UIDEDUP_MAIN_FRAME_H__

#include <wx/listbase.h>
#include <wx/listctrl.h>
#include <wx/splitter.h>
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
    void OnItemSelected(::wxListEvent& event);
    
    void SetupMenuBar();
    void SetupMainLayout();
    void SetupListControl();
    void PopulateSampleData();
    
    wxListCtrl* m_listCtrl;
    wxSplitterWindow* m_splitter;
    wxPanel* m_leftPanel;
    wxPanel* m_rightPanel;
    wxTextCtrl* m_previewText;
};

} // namespace Deduplicator::Ui

#endif // __UIDEDUPLIB_MAIN_FRAME_H__