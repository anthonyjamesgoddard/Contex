#include "contex/base/word.hpp"

#include <fstream>
#include <vector>

namespace contex {
namespace base {
class document {
public:
    document() = delete;
    document(std::string file_name) { m_doc_stream = std::fstream(file_name); }

private:
    mutable std::fstream m_doc_stream;
};

std::istream& operator>>(const document& is, contex::base::word& w) {
    std::string temp;
    is >> temp;
    w = temp;
    return is;
}

}  // namespace base
}  // namespace contex
