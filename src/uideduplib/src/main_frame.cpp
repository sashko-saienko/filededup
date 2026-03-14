//
// Created by sashko on 14.03.26.
//
#include "uideduplib/main_frame.h"
#include <wx/wx.h>

MainFrame::MainFrame(): wxFrame(NULL, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize)
{
    wxMenu* menuFile = new wxMenu();
    menuFile->Append(ID_HELLO, "&Hello...\tCtrl+H", "Hello world example!");
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);

    wxMenu* menuHelp = new wxMenu();
    menuHelp->Append(wxID_ABOUT);

    wxMenuBar* menuBar = new wxMenuBar();
    menuBar->Append(menuFile, "&File");
    menuBar->Append(menuHelp, "&Help");

    SetMenuBar(menuBar);

    CreateStatusBar();
    SetStatusText("Hello, world!");
}