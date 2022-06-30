#include "TileMapComponent.h"

size_t read_data(char const* const filename, std::function<char* (size_t const)> allocator) {
    size_t readbytes{ 0 };
    std::ifstream ifile(filename, std::ios::ate | std::ios::binary);
    if (ifile.is_open()) {
        auto length{ static_cast<size_t> (ifile.tellg()) };
        ifile.seekg(0, std::ios_base::beg);

        auto buffer{ allocator(length) };
        try {
            ifile.read(buffer, length);
            readbytes = static_cast<size_t> (ifile.gcount());
        }
        catch (std::ios_base::failure&) {
            // handle error
        }
        ifile.close();
    }
    return readbytes;
}

void read_csv(char const* const filename, std::vector<std::vector<int>>& v) {
    auto input{ std::make_unique<std::vector<unsigned char>>() };
    if (read_data(filename, [&input](size_t const length) {
        input->resize(length);
        return reinterpret_cast<char*> (input->data()); })
        ) {
        // successfully read file to input vector
        int tmp{ 0 };
        int negative{ 1 };
        std::vector<int> tmp_v{};
        for (auto& c : *input) {
            switch (c) {
            case ',':
                tmp *= negative;
                negative = 1;
                tmp_v.push_back(tmp);
                tmp = 0;
                break;
            case '\r':
                tmp *= negative;
                negative = 1;
                tmp_v.push_back(tmp);
                tmp = 0;
                v.emplace_back(tmp_v);
                tmp_v.clear();
                break;
            case '\n':
                continue;
            case '-':
                negative = -1;
                break;
            default:
                tmp *= 10 * (tmp != 0) + 1 * (tmp == 0);
                tmp += static_cast<int>(c - '0');
            }
        }
    }
}