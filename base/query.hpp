#pragma once
#include<unordered_map>

namespace contex {
namespace base {
//! Each entry of the map is a word and how many words
//! after the word we want to define a query
using query = std::unordered_map<word, std::size_t>;
}  // namespace base
}  // namespace contex

