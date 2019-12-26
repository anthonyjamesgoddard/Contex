#pragma once

#include "contex/base/document.hpp"
#include "contex/base/window.hpp"
#include "contex/base/word.hpp"

#include <vector>
#include <iostream>

namespace contex {
namespace analysis {
// clang-format off

//! This analysis object can be used to obtain a word window analysis.
class word_window {
    using word              =   contex::base::word;
    using window            =   contex::base::window;
    using document          =   contex::base::document;
    using word_list         =   contex::base::word_list;
    using word_instances    =   contex::base::word_instances;
public:
    using size = std::size_t;
    explicit word_window(const word_list& context_words,
                         const word_list& target_words,
                         const size sz,
                         const document& d);
};
//clang-format on

word_window::word_window(const word_list& context_words,
                         const word_list& target_words,
                         const size sz,
                         const document& d) {

//    auto wi = contex::base::init_word_instances(context_words);

}

}  // namespace analysis
}  // namespace contex
