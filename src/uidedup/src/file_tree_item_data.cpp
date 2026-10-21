//
// Created by sashko on 21.10.26.
//
#include <file_tree_item_data.h>

namespace Deduplicator::Ui
{

FileTreeItemData::FileTreeItemData(Poco::File&& file) : m_file(std::move(file))
{
}

const Poco::File& FileTreeItemData::GetFile() const
{
    return m_file;
}

} // namespace Deduplicator::Ui
