#include <codecvt>
#include <string>
#include <vector>
#include <locale>
#include <istream> 

std::u32string to_UTF32(const std::string& string) {
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> convert;
    return convert.from_bytes(string);
}

std::string to_UTF8(const std::u32string& u32string) {
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> convert;
    return convert.to_bytes(u32string);
}

void getline_u32(std::istream& is, std::u32string& u32string) {
    std::string string;
    std::getline(is, string);
    u32string = to_UTF32(string); 
}