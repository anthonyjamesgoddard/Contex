#include "contex/base/document.hpp"
#include "contex/base/window.hpp"
#include "contex/base/query.hpp"

#include <vector>

#include <iostream>

namespace contex {
namespace analysis {
// clang-format off

//! This analysis object can be used to obtain a word window analysis.
//! Current functionality is primitive. At the moment we only look at
//! words and does not consider full stops. See blog post for implem-
//! enting more complex functionality.
class word_window {
    using word      =   contex::base::word;
    using window    =   contex::base::window;
    using document  =   contex::base::document;
    using query     =   contex::base::query;
public:
    using size = std::size_t;
    explicit word_window(const query& q,
                         const size sz,
                         const document& d);
private:
    std::vector<window> m_windows;
};
//clang-format on

word_window::word_window(const query& q,
                         const size sz,
                         const document& d) {
    word current_word;
    std::size_t i = 0;
    while(d >> current_word) {
        i++;
        std::cout << i << std::endl;
    }
}

}  // namespace analysis
}  // namespace contex
