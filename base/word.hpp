#include <algorithm>
#include <string>
//! A word in contex is a string that is endowed with
//! convienience methods
namespace contex {
namespace base {
class word {
public:
    explicit word() {};
    explicit word(std::string& s) {};

    word& operator=(std::string& s) {
        m_word = s;
        return *this;
    }
    word& operator=(word& w) {
        m_word = w.m_word;
        return *this;
    }
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

}  // namespace base
}  // namespace contex
