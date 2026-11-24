#include "Book.h"
#include <sstream>
#include <stdexcept>
#include <vector>

Book::Book(int id, const std::string& title, const std::string& author, int year)
    : id_(id), title_(title), author_(author), year_(year) {}

int Book::getId() const { return id_; }
const std::string& Book::getTitle() const { return title_; }
const std::string& Book::getAuthor() const { return author_; }
int Book::getYear() const { return year_; }
bool Book::isBorrowed() const { return borrowed_; }

void Book::borrow() { borrowed_ = true; }
void Book::giveBack() { borrowed_ = false; }

// بنستخدم | كفاصل، وبنستبدلها لو موجودة جوه النص
static std::string escapeField(const std::string& s) {
    std::string out;
    for (char c : s) {
        if (c == '|') out += "%7C";
        else if (c == '%') out += "%25";
        else out += c;
    }
    return out;
}

static std::string unescapeField(const std::string& s) {
    std::string out;
    for (size_t i = 0; i < s.size(); ++i) {
        if (s[i] == '%' && i + 2 < s.size()) {
            std::string code = s.substr(i, 3);
            if (code == "%7C") { out += '|'; i += 2; continue; }
            if (code == "%25") { out += '%'; i += 2; continue; }
        }
        out += s[i];
    }
    return out;
}

std::string Book::serialize() const {
    std::ostringstream oss;
    oss << id_ << '|' << escapeField(title_) << '|' << escapeField(author_)
        << '|' << year_ << '|' << (borrowed_ ? 1 : 0);
    return oss.str();
}

static std::vector<std::string> split(const std::string& line, char delim) {
    std::vector<std::string> parts;
    std::string cur;
    std::istringstream iss(line);
    while (std::getline(iss, cur, delim)) parts.push_back(cur);
    return parts;
}

Book Book::deserialize(const std::string& line) {
    auto parts = split(line, '|');
    if (parts.size() != 5) throw std::invalid_argument("bad book line");
    Book b(std::stoi(parts[0]), unescapeField(parts[1]), unescapeField(parts[2]), std::stoi(parts[3]));
    if (parts[4] == "1") b.borrow();
    return b;
}
