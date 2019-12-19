#include "contex/analysis/word_window.hpp"
#include "contex/base/word.hpp"

#include <string>

int main(int argc, char** argv) {
    // get document
    contex::base::document d("tests/corpus/barn.txt");
    contex::base::query q;
    contex::analysis::word_window ww(q, 4, d);
}
