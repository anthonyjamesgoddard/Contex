#pragma once

#include "contex/base/word.hpp"

#include <unordered_map>

namespace contex {
namespace base {
//! Each entry of the map is a word and how many words
//! after the word we want to define a query
using query = std::unordered_map<contex::base::word, std::size_t>;
}  // namespace base
}  // namespace contex

