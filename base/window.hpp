#include <array>

namespace contex {
namespace base {
class window {
public:
    // getters
    std::size_t begin() const { return m_data[0]; }
    std::size_t centre() const { return m_data[1]; }
    std::size_t end() const { return m_data[2]; }
    // setters
    void begin(std::size_t b) { m_data[0] = b; }
    void centre(std::size_t c) { m_data[1] = c; }
    void end(std::size_t e) { m_data[2] = e; }

private:
    std::array<std::size_t, 3> m_data;
};

inline bool overlap(const window& w1, const window& w2) {
    if (w1.begin() >= w2.begin() && w1.begin() <= w2.end()) return true;
    if (w2.begin() >= w1.begin() && w2.end() <= w1.end()) return true;
    return false;
}

inline std::vector<window> intersection(std::vector<window>& ws) {
    std::vector<window> outs;
    if (!ws.size()) return outs;
    window out = ws[0];
    for (int i = 1; i < ws.size(); i++) {
        if (overlap(out, ws[i])) {
            out.end(ws[i].end());
        } else {
            outs.push_back(out);
            out = ws[i];
        }
        if (i == ws.size() - 1) outs.push_back(out);
    }

    for (auto& o : outs) {
        std::cout << o.begin() << " " << o.end() << std::endl;
    }

    return outs;
}
}  // namespace base
}  // namespace contex
