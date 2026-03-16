#ifndef __UIDEDUPLIB_MAIN_FRAME_H__
#define __UIDEDUPLIB_MAIN_FRAME_H__

#include <wx/wx.h>

namespace Deduplicator::UiLib
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

}

#endif // __UIDEDUPLIB_MAIN_FRAME_H__
