#include <algorithm>
#include <string>
//! A word in contex is a string that is endowed with
//! convienience methods
namespace contex {
namespace base {
class word {
public:
    word(std::string s);
    word strip_punctuation() const; 
private:
    std::string m_word;
};

inline word word::strip_punctuation() const {
    std::string result;
//    std::remove_copy_if(m_word.begin(), m_word.end(), std::back_inserter(result), );
}

}  // namespace base
}  // namespace contex
