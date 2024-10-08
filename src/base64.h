#pragma once
#include <string>
#include <vector>
#include <algorithm>

/*
 _______
/       \                                /      \ /  |  /  |
$$$$$$$  |  ______    _______   ______  /$$$$$$  |$$ |  $$ |
$$ |__$$ | /      \  /       | /      \ $$ \__$$/ $$ |__$$ |
$$    $$<  $$$$$$  |/$$$$$$$/ /$$$$$$  |$$      \ $$    $$ |
$$$$$$$  | /    $$ |$$      \ $$    $$ |$$$$$$$  |$$$$$$$$ |
$$ |__$$ |/$$$$$$$ | $$$$$$  |$$$$$$$$/ $$ \__$$ |      $$ |
$$    $$/ $$    $$ |/     $$/ $$       |$$    $$/       $$ |
$$$$$$$/   $$$$$$$/ $$$$$$$/   $$$$$$$/  $$$$$$/        $$/

dev    : nosdayoo
github : https://github.com/nosdayoo/Base64

: mini lib o_O :
*/

namespace base64 {
    namespace map_data {
        static std::string_view encode = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

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
        static size_t triple(const std::string_view& in, size_t index) {
            return (map_data::decode[in[index]] << 18) | (map_data::decode[in[index + 1]] << 12) | (map_data::decode[in[index + 2]] << 6) | map_data::decode[in[index + 3]];
        }
    }

    static std::string encode(const std::string_view input) {
        const size_t size = input.size();
        const size_t out_size = (size + 2) / 3 * 4;
        std::string out(out_size, '\0');

        size_t j = 0;
        for (size_t i = 0; i < size;) {
            size_t buffer = 0;
            for (size_t k = 0; k < 3; ++k) {
                if (i < size) buffer |= (size_t)input[i++] << (16 - k * 8);
            }
            out[j++] = map_data::encode[(buffer >> 18) & 0x3F];
            out[j++] = map_data::encode[(buffer >> 12) & 0x3F];
            out[j++] = (i > size + 1) ? '=' : map_data::encode[(buffer >> 6) & 0x3F];
            out[j++] = (i > size) ? '=' : map_data::encode[buffer & 0x3F];
        }
        out.resize(j);
        return out;
    }

    static std::string decode(const std::string_view& in) {
        size_t size = in.size();
        size_t out_size = (size / 4) * 3;
        if (size % 4 != 0) return "Invalid!";
        if (in[size - 2] == '=') out_size -= 2;
        else if (in[size - 1] == '=') out_size -= 1;
        std::string out(out_size, '\0');
        size_t out_index = 0;
        for (size_t i = 0; i < size; i += 4) {
            auto triple = utils::triple(in, i);
            out[out_index++] = (triple >> 16) & 0xFF;
            if (in[i + 2] != '=') out[out_index++] = (triple >> 8) & 0xFF;
            if (in[i + 3] != '=') out[out_index++] = triple & 0xFF;
        }
        return out;
    }
}
