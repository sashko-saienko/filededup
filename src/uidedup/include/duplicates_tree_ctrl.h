#ifndef __UIDEDUP_DUPLICATES_TREE_CTRL_H__
#define __UIDEDUP_DUPLICATES_TREE_CTRL_H__

#include <deduplib/files_map.h>
#include <filesystem>
#include <wx/treectrl.h>

namespace Deduplicator::Ui
{

namespace fs = std::filesystem;

class DuplicatesTreeCtrl : public wxTreeCtrl
{
  public:
    DuplicatesTreeCtrl(wxWindow* parent, wxWindowID id, const wxPoint& pos = wxDefaultPosition,
                       const wxSize& size = wxDefaultSize, long style = wxTR_DEFAULT_STYLE);

    void SetDuplicatesTree(const std::string& targetFolder);

  private:
    int m_uncheckedImg;
    int m_checkedImg;
    Deduplicator::ChecksumFilesMap checksumFilesMap;

    void OnItemActivated(::wxTreeEvent& event);
    void OnSelectionChanged(::wxTreeEvent& event);
    bool IsFileItem(const wxTreeItemId& item) const;
    bool IsCheckable(const wxTreeItemId& item) const;
    void Populate(const std::string& targetFolder);
};

} // namespace Deduplicator::Ui

#endif // __UIDEDUP_DUPLICATES_TREE_CTRL_H__