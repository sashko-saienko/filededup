//
// Created by sashko on 13.03.26.
//

#ifndef UIDEDUP_APPLICATION_H
#define UIDEDUP_APPLICATION_H

#include <wx/wx.h>

class UidedupApplication : public wxApp
{
  public:
    bool OnInit() override;

protected:
    static void ConfigureLogging();
};

#endif // UIDEDUP_APPLICATION_H
