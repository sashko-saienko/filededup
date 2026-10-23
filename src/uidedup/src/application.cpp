//
// Created by sashko on 13.03.26.
//
#include <Poco/AutoPtr.h>
#include <Poco/Logger.h>
#include <Poco/Util/LoggingConfigurator.h>
#include <Poco/Util/PropertyFileConfiguration.h>
#include <application.h>
#include <main_frame.h>
#include <wx/filename.h>
#include <wx/stdpaths.h>

namespace Deduplicator::Ui
{

using Poco::makeAuto;
using Poco::Util::LoggingConfigurator;
using Poco::Util::PropertyFileConfiguration;

wxString Application::GetResourcesPath()
{
    wxFileName execFileName{wxStandardPaths::Get().GetExecutablePath()};
    return wxString::Format("%s/resources/", execFileName.GetPath());
}

wxImage Application::GetCheckedImage()
{
    wxImage checkedImg;
    checkedImg.LoadFile(GetResourcesPath() + "checked.png", wxBITMAP_TYPE_PNG);
    return checkedImg;
}

wxImage Application::GetUncheckedImage()
{
    wxImage uncheckedImg;
    uncheckedImg.LoadFile(GetResourcesPath() + "unchecked.png", wxBITMAP_TYPE_PNG);
    return uncheckedImg;
}

wxIcon Application::GetApplicationIcon()
{
    wxIcon icon;
    wxString resourcesPath = GetResourcesPath();

#ifdef __WXMSW__
    icon.LoadFile(resourcesPath + "appicon.ico", wxBITMAP_TYPE_ICO);
#elif __WXMAC__
    icon.LoadFile(resourcesPath + "appicon.icns", wxBITMAP_TYPE_ICNS);
#else
    wxImage img;
    if (img.LoadFile(resourcesPath + "appicon.png", wxBITMAP_TYPE_PNG))
    {
        icon.CopyFromBitmap(wxBitmap(img));
    }
#endif
    return icon;
}

bool Application::OnInit()
{
    fs::path props = fs::path{argv[0].ToStdString()}.parent_path() / "logging.properties";
    ConfigureLogging(props);
    wxInitAllImageHandlers();
    auto* mainFrame = new MainFrame();
    mainFrame->Show();
    return true;
}

void Application::ConfigureLogging(const fs::path& propsPath)
{
    PropertyFileConfiguration::Ptr pConfig = makeAuto<PropertyFileConfiguration>(propsPath);
    LoggingConfigurator lc;
    lc.configure(pConfig);
}

} // namespace Deduplicator::Ui