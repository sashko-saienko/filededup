#ifndef __UIDEDUP_MAIN_FRAME_H__
#define __UIDEDUP_MAIN_FRAME_H__

#include <wx/wx.h>

namespace Deduplicator::Ui
{

class MainFrame : public ::wxFrame
{
  public:
    MainFrame();

    enum ControlId
    {
        ID_HELLO = 0,
    };

  private:
    void OnHello(::wxCommandEvent& event);
    void OnExit(::wxCommandEvent& event);
    void OnAbout(::wxCommandEvent& event);
};

} // namespace Deduplicator::Ui

#endif // __UIDEDUPLIB_MAIN_FRAME_H__
