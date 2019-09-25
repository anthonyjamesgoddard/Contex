#include <unistd.h>
#include <array>
#include <fstream>
#include <iostream>
#include <list>
#include <utility>
#include <vector>

using namespace std;

typedef std::array<int, 3> window;

bool overlap(window& w1, window& w2) {
    if (w1[0] >= w2[0] && w1[0] <= w2[2]) return true;
    if (w2[0] >= w1[0] && w2[0] <= w1[2]) return true;
    return false;
}

std::vector<window> merge_sorted_winds(std::vector<window>& ws) {
    std::vector<window> outs;
    if (!ws.size()) return outs;
    window out = ws[0];
    for (int i = 1; i < ws.size(); i++) {
        if (overlap(out, ws[i])) {
            out[2] = ws[i][2];
        } else {
            outs.push_back(out);
            out = ws[i];
        }
        if (i == ws.size() - 1) outs.push_back(out);
    }

    for (auto& o : outs) {
        std::cout << o[0] << " " << o[2] << std::endl;
    }

    return outs;
}

bool comp(const std::pair<int, int>& a, const std::pair<int, int>& b) {
    return (a.first < b.first);
}

void write_context(std::fstream& file, std::list<std::pair<int, int>>& l) {
    // sort the list
    l.sort(comp);

    // tokenise the document
    std::string line;
    std::vector<std::string> split_string;
    std::vector<int> full_stops = {-1};
    int i = 0;
    while (file >> line) {
        auto full_stop_location = line.find_first_of(".");
        if (full_stop_location != string::npos) {
            split_string.push_back(line.substr(0, full_stop_location));
            full_stops.push_back(++i);
            split_string.push_back(line.substr(full_stop_location));
        } else {
            split_string.push_back(line);
            if (line == ".") {
                full_stops.push_back(i);
            }
        }
        i++;
    }
    /* 
    for(auto& fs: full_stops) {
        std::cout << fs << std::endl;
    }
    */
    /* tokenised output
    for (auto& s : split_string) {
        std::cout << s << std::endl;
    }*/
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
        if (!std::binary_search(full_stops.begin(), full_stops.end(), o[2]+1)) {
            std::cout << "...";
        } else {
            std::cout << "." << std::endl;
        }
    }
}

std::list<std::pair<int, int>> read_pairs(std::fstream& file) {
    std::list<std::pair<int, int>> l;
    int i, j;
    char a;
    while (file >> i >> a >> j) {
        l.emplace_back(std::make_pair(i, j));
    }
    return l;
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
        auto l = read_pairs(listfile);
        // open a file to access the document
        std::fstream docfile(doc_file_name);
        // write_context function extracts context from the document
        write_context(docfile, l);
    } else {
        std::cout << "read usage! : Missing arguments/empty filename!"
                  << std::endl;
    }
}
