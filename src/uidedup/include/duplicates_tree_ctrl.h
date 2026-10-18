#ifndef __UIDEDUP_DUPLICATES_TREE_CTRL_H__
#define __UIDEDUP_DUPLICATES_TREE_CTRL_H__

#include <deduplib/files_map.h>
#include <thread>
#include <wx/treectrl.h>

namespace Deduplicator::Ui
{

class DuplicatesTreeCtrl : public wxTreeCtrl
{
  public:
    DuplicatesTreeCtrl(wxWindow* parent, wxWindowID id, const wxPoint& pos = wxDefaultPosition,
                       const wxSize& size = wxDefaultSize, long style = wxTR_DEFAULT_STYLE);

    void SetDuplicatesTree(std::stop_token st, const std::string& targetFolder);
    void DeleteChecked();
    void ClearChecked();

    bool HasCheckedItems() const
    {
        return checked > 0;
    }

  private:
    int m_uncheckedImg;
    int m_checkedImg;
    size_t checked;
    Deduplicator::ChecksumFilesMap checksumFilesMap;

    void OnItemActivated(::wxTreeEvent& event);
    void OnSelectionChanged(::wxTreeEvent& event);

    bool IsFileItem(const wxTreeItemId& item) const;
    bool canSelectWithinChecksumGroup(const wxTreeItemId& item) const;

    void Populate(const std::string& targetFolder);
    wxArrayTreeItemIds GetCheckedTreeItemIds() const;
    void RemoveFileFromChecksumMap(unsigned checksum, const std::string& filePath);
    void DeleteCheckedTreeItems(wxArrayTreeItemIds& checkedItems);
    void ActivateItem(const wxTreeItemId& item);
    void SendEvent(wxEventTypeTag<wxCommandEvent> event) const;

    using TreeItemIdCallback = std::function<void(wxTreeItemId)>;
    void EnumerateItems(wxTreeItemId parent, TreeItemIdCallback treeItemCallback) const;
};

} // namespace Deduplicator::Ui

#endif // __UIDEDUP_DUPLICATES_TREE_CTRL_H__