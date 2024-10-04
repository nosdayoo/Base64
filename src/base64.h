#include <string>
#include <vector>
#include <algorithm>

namespace base64 {
    namespace map_data {
        static std::string encode = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

        static const char decode[] = {
            64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
            64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
            64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 62, 64, 64, 64, 63,
            52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 64, 64, 64, 64, 64, 64,
            64, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
            15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 64, 64, 64, 64, 64,
            64, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
            41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 64, 64, 64, 64, 64,
            64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
            64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
            64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
            64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
            64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
            64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
            64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
            64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64
        };
    }
    namespace utils {
        static uint32_t triple(const std::string& in, int i) {
            return (map_data::decode[in[i]] << 18) | (map_data::decode[in[i + 1]] << 12) | (map_data::decode[in[i + 2]] << 6) | map_data::decode[in[i + 3]];
        }

        static uint32_t octet(const std::string& in, int& index, int size) {
            return (index < size) ? (unsigned char)in[index++] : 0;
        }
    }

    static std::string encode(const std::string& in) {
        int size = in.size();
        std::vector<char> out((size + 2) / 3 * 4, '=');

        for (int i = 0, j = 0; i < size;) {
            auto t = (utils::octet(in, i, size) << 16) | (utils::octet(in, i, size) << 8) | utils::octet(in, i, size);
            out[j++] = map_data::encode[t >> 18];
            out[j++] = map_data::encode[t >> 12];
            out[j++] = (i + 1 < size) ? map_data::encode[(t >> 6) & 0x3F] : '=';
            out[j++] = (i + 2 < size) ? map_data::encode[t & 0x3F] : '=';
        }

        return std::string(out.begin(), out.end());
    }

    static std::string decode(const std::string& in) {
        if (in.size() % 4 != 0) return "Invalid!";
        std::vector<char> out;
        out.reserve(in.size() / 4 * 3);

        for (int i = 0; i < in.size(); i += 4) {
            auto triple = utils::triple(in, i);
            out.push_back((triple >> 16) & 0xFF);
            if (in[i + 2] != '=') out.push_back((triple >> 8) & 0xFF);
            if (in[i + 3] != '=') out.push_back(triple & 0xFF);
        }

        return std::string(out.begin(), out.end());
    }
}