#include <gtest/gtest.h>

using namespace std;

class byte {
public:
    explicit byte(char c) { m_byte = (uint8_t )c;}
    friend std::ostream& operator << (std::ostream& os, byte b) {
        const char lut[] = {'0','1','2','3','4','5','6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E','F'};
        os << "0x" << lut[b.m_byte/16] << lut[b.m_byte%16];
        return os;
    }
private:
    uint8_t m_byte;
};

template<class Facet>
struct deletable_facet : Facet
{
    template<class ...Args>
    deletable_facet(Args&& ...args) : Facet(std::forward<Args>(args)...) {}
    ~deletable_facet() {}
};

void print_as_wide(const char* mbstr)
{
    std::mbstate_t state = std::mbstate_t();
    std::size_t len = 1 + std::mbsrtowcs(NULL, &mbstr, 0, &state);
    std::vector<wchar_t> wstr(len);
    std::mbsrtowcs(&wstr[0], &mbstr, wstr.size(), &state);
    std::wcout << "Wide string: " << &wstr[0] << '\n'
               << "The length, including '\\0': " << wstr.size() << '\n';
}

TEST(codecvt, case1) {
    std::string data = "caption \"£ª£¤120 Stories£ªGGMºº»¯£ª\"";
    for (auto c : data) {
        std::cout << byte(c) << " ";
    }
    std::cout << "\n";
    std::cout << data << "\n";

    setlocale(LC_ALL, "");
    print_as_wide(data.c_str());

    const char* mbstr = u8"z\u00df\u6c34\U0001f34c";
    print_as_wide(mbstr);
}
