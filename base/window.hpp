#include <array>

namespace contex {
namespace base {
class window {
public:
    std::size_t begin() { return m_data[0] };
    std::size_t middle();
    std::size_t end();

    inline bool overlap(const window& w1, const window& w2) {
        if (w1[0] >= w2[0] && w1[0] <= w2[2]) return true;
        if (w2[0] >= w1[0] && w2[0] <= w1[2]) return true;
        return false;
    }

    inline std::vector<window> intersection(std::vector<window>& ws) {
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

private:
    std::optional<std::array<std::size_t, 3>> m_data;
};

}  // namespace base
}  // namespace contex
