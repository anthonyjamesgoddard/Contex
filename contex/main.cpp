#include <unistd.h>
#include <array>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <list>
#include <unordered_map>
#include <utility>
#include <vector>

#include "contex/base/window.hpp"

bool comp(const std::pair<int, int>& a, const std::pair<int, int>& b) {
    return (a.first < b.first);
}

std::string strip_punc(std::string& word) {
    auto word_nopunc = word;
    for (int i = 0, len = word_nopunc.size(); i < len; i++) {
        if (ispunct(word_nopunc[i])) {
            word_nopunc.erase(i--, 1);
            len = word_nopunc.size();
        }
    }
    return word_nopunc;
}

// this needs some work.
// it costs O(N_numberOfWordsInCorpus*log(N_numberOfQueries)
void tokenise(std::fstream& file, std::unordered_map<std::string, int>& query_map,
              std::vector<std::string>& split_string,
              std::vector<int>& full_stops, std::list<std::pair<int, int>>& l) {
    std::string line;
    int i = 0;
    while (file >> line) {
        auto full_stop_location = line.find_first_of(".");
        if (full_stop_location != string::npos) {
            // look at the substring to the left of the full_stop
            auto split = line.substr(0, full_stop_location);
            auto split_without_punc = strip_punc(split);
            auto it = query_map.find(split_without_punc);
            if (it != query_map.end()) {
                l.push_back(std::make_pair(i, it->second));
            }
            split_string.push_back(split);
            // add the full stop
            full_stops.push_back(++i);
            // look at the substring to the right of the full_stop
            split = line.substr(full_stop_location);
            split_without_punc = strip_punc(split);
            it = query_map.find(split_without_punc);
            if (it != query_map.end()) {
                l.push_back(std::make_pair(i, it->second));
            }
            split_string.push_back(split);
        } else {
            auto line_without_punc = strip_punc(line);
            auto it = query_map.find(line_without_punc);
            if (it != query_map.end()) {
                l.push_back(std::make_pair(i, it->second));
            }
            split_string.push_back(line);
            if (line == ".") {
                full_stops.push_back(i);
            }
        }
        i++;
    }
}

void write_context(std::fstream& file, std::unordered_map<std::string, int>& query_map) {
    // tokenise the document
    std::string line;
    std::vector<std::string> split_string;
    std::vector<int> full_stops = {-1};
    std::list<std::pair<int, int>> l;
    tokenise(file, query_map, split_string, full_stops, l);
    

    // at this point we have a list of pairs l
    // the first enrtry in the pair gives us the location of
    // an instance of a query and the second gives us
    // the number of words right of the query that we wish to
    // analyse

    // obtain windows that respect full stops
    int id_fs = 0;
    int log_id = 0;
    int full_stop = full_stops[id_fs];
    vector<window> windows;

    // O( N_INTERESTINGWORDS ) <--- the full stops get tracked along the way
    std::cout << "-----------------------" << std::endl;
    std::cout << "forming windows" << std::endl;
    std::cout << "-----------------" << std::endl;
    for (auto& p : l) {
        std::cout << p.first << " " << p.second << std::endl;
        window temp = {p.first - 5, p.first, p.first + p.second + 4};
        // we have gone too far ->> reset
        if (full_stop > temp[0]) {
            id_fs = log_id;
            full_stop = full_stops[id_fs];
        }
        // if full_stop is before window we dont care
        log_id = id_fs;
        while (full_stop < temp[0]) {
            full_stop = full_stops[++id_fs];
        }
        // is this in the window?
        int temp_id = id_fs;
        while (full_stop <= temp[2] && full_stop >= temp[0]) {
            if (full_stop < temp[1]) {
                temp[0] = full_stop + 1;
            } else {
                temp[2] = full_stop - 1;
            }
            full_stop = full_stops[++id_fs];
        }
        id_fs = temp_id;

        std::cout << temp[0] << " " << split_string[temp[0]] << " " << temp[1]
                  << " " << split_string[temp[1]] << " " << temp[2] << " "
                  << split_string[temp[2]] << " " << full_stop << std::endl;

        // otherwise it is outside the window
        windows.push_back(temp);
    }
    std::cout << "---------------" << std::endl;
    std::cout << "merging windows" << std::endl;
    std::cout << "-----------------" << std::endl;
    // the windows are already sored with respect to the first entry [0]
    // overlap windows O(numberofintervals) in time and O(1) memory
    auto outs = merge_sorted_winds(windows);

    // At this point we have overlapped windows and locations of
    // words of interest we can now print
    //
    //
    std::cout << "-----------------" << std::endl;
    std::cout << "merged query windows" << std::endl;
    std::cout << "-----------------" << std::endl;
    for (auto& o : outs) {
        std::cout << " ! " << std::endl;
        for (int i = o[0]; i <= o[2]; i++) {
            std::cout << split_string[i] << " ";
        }
        std::cout << "\n ! " << std::endl;
    }

    std::cout << "-----------------" << std::endl;
    std::cout << "output" << std::endl;
    std::cout << "-----------------" << std::endl;
    std::list<std::pair<int, int>>::iterator it = l.begin();
    for (auto& o : outs) {
        if (o != *outs.begin()) std::cout << " ";
        bool open = false;
        for (int i = o[0]; i <= o[2]; i++) {
            if (i == it->first) {
                std::cout << "\033[32m";
                open = true;
            }
            std::cout << split_string[i];
            if (i == it->first + it->second - 1 || (i == o[2] && open)) {
                std::cout << "\033[0m";
                it++;
                open = false;
            }
            std::cout << " ";
        }
        // if ... then words have been ommited
        if (!std::binary_search(full_stops.begin(), full_stops.end(),
                                o[2] + 1)) {
            std::cout << "...";
        } else {
            std::cout << "." << std::endl;
        }
    }
}

void read_pairs(std::fstream& file, std::unordered_map<std::string, int>& query_map) {
    int i;
    std::string s;
    char a;
    while (file >> s >> a >> i) {
        std::cout << s << " " << i << std::endl;
        query_map[s] = i;
    }
}

const std::string usage =
    R"(usage: ./write_context -f document_file_name -f list_file_name 

Make sure the files exist.
)";

int main(int argc, char** argv) {
    std::string list_file_name, doc_file_name;
    char c;
    std::cout << usage << std::endl;
    while ((c = getopt(argc, argv, "l:f:")) != -1) {
        switch (c) {
            case 'l':
                list_file_name = optarg;
                break;
            case 'f':
                doc_file_name = optarg;
                break;
        }
    }
    if (list_file_name != "" && doc_file_name != "") {
        std::cout << "File Context -------------- " << std::endl;
        // open a file that contains the list
        std::fstream listfile(list_file_name);
        // read the list into a std::list (doubly linked list)
        std::unordered_map<std::string, int> query_map;
        read_pairs(listfile, query_map);
        // open a file to access the document
        std::fstream docfile(doc_file_name);
        // write_context function extracts context from the document
        write_context(docfile, query_map);
    } else {
        std::cout << "read usage! : Missing arguments/empty filename!"
                  << std::endl;
    }
}
