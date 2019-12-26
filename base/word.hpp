#pragma once

#include <algorithm>
#include <list>
#include <map>
#include <string>

namespace contex {
namespace base {
class word {
public:
    explicit word(){};
    explicit word(std::string& s) { m_word = s; };
    explicit word(std::string&& s) { m_word = std::move(s); }

    word& operator=(std::string s) {
        m_word = s;
        return *this;
    }
    word& operator=(word& w) {
        m_word = w.m_word;
        return *this;
    }

    const std::string& as_string() const { return m_word; }

    word rmv_punc() const;

private:
    std::string m_word;
};

inline word word::rmv_punc() const {
    std::string result;
    std::remove_copy_if(m_word.begin(), m_word.end(),
                        std::back_inserter(result),
                        [](unsigned char i) { return std::ispunct(i); });
    return word(result);
}

using word_list = std::list<contex::base::word>;

using word_instances = std::map<std::list<contex::base::word>::iterator,
                                std::vector<std::size_t>>;

//! This is a conv. function that enables us to just start pushing back
//! a word instance structure
/*
inline word_instances init_word_instances(const word_list& wl) {
    word_instances wi;
    for (auto it = wl.begin(); it != wl.end(); ++it) {
        wi[it] = {};
    }
    return wi;
}
*/
}  // namespace base
}  // namespace contex
