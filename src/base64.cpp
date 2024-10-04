#include "base64.h"

std::string base64::encode(const std::string& in) {
	int value = 0, value_bit = -6;
	std::string ret;

	for (auto& c : in) {
		value = (value << 8) + c;
		for (value_bit += 8; value_bit >= 0; value_bit -= 6) ret.push_back(map_data::encode[(value >> value_bit) & 0x3F]);
	}

	if (value_bit > -6) ret.push_back(map_data::encode[(value << 8) >> value_bit & 0x3F]);
	ret.append((4 - ret.size() % 4) % 4, '=');
	return ret;
}

std::string base64::decode(const std::string& in) {
    int value = 0, value_bit = -8;
    std::string ret;

    for (auto& c : in) {
        if (std::isspace(c) || c == '=') continue;
        if (map_data::decode[c] == 64) return "invalid!";
        value = (value << 6) + map_data::decode[c];
        if ((value_bit += 6) >= 0) {
            ret.push_back((value >> value_bit) & 0xFF);
            value_bit -= 8;
        }
    }
    return ret;
}