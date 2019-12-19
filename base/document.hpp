#include "contex/base/word.hpp"

#include <fstream>
#include <vector>

namespace contex {
namespace base {
class document {
public:
    document() = delete;
    document(std::string file_name) { m_doc_stream = std::fstream(file_name); }

    bool operator>>(contex::base::word& w) const;

private:
    mutable std::fstream m_doc_stream;
};

bool document::operator>>(contex::base::word& w) const {
    std::string temp;
    if (m_doc_stream >> temp) {
        w = temp;
        return true;
    }
    return false;
}

}  // namespace base
}  // namespace contex
