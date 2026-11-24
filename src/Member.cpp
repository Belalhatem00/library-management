#include "Member.h"
#include <algorithm>
#include <sstream>
#include <stdexcept>
#include <vector>

Member::Member(int id, const std::string& name) : id_(id), name_(name) {}

int Member::getId() const { return id_; }
const std::string& Member::getName() const { return name_; }
const std::vector<int>& Member::getBorrowedBookIds() const { return borrowedBookIds_; }

void Member::borrowBook(int bookId) {
    if (!hasBook(bookId)) borrowedBookIds_.push_back(bookId);
}

bool Member::returnBook(int bookId) {
    auto it = std::find(borrowedBookIds_.begin(), borrowedBookIds_.end(), bookId);
    if (it == borrowedBookIds_.end()) return false;
    borrowedBookIds_.erase(it);
    return true;
}

bool Member::hasBook(int bookId) const {
    return std::find(borrowedBookIds_.begin(), borrowedBookIds_.end(), bookId) != borrowedBookIds_.end();
}

static std::string escapeName(const std::string& s) {
    std::string out;
    for (char c : s) {
        if (c == '|') out += "%7C";
        else if (c == '%') out += "%25";
        else out += c;
    }
    return out;
}

static std::string unescapeName(const std::string& s) {
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

// الشكل: id|name|book1,book2,book3
std::string Member::serialize() const {
    std::ostringstream oss;
    oss << id_ << '|' << escapeName(name_) << '|';
    for (size_t i = 0; i < borrowedBookIds_.size(); ++i) {
        if (i) oss << ',';
        oss << borrowedBookIds_[i];
    }
    return oss.str();
}

Member Member::deserialize(const std::string& line) {
    std::vector<std::string> parts;
    std::string cur;
    std::istringstream iss(line);
    while (std::getline(iss, cur, '|')) parts.push_back(cur);
    if (parts.size() < 2) throw std::invalid_argument("bad member line");
    Member m(std::stoi(parts[0]), unescapeName(parts[1]));
    if (parts.size() == 3 && !parts[2].empty()) {
        std::istringstream ids(parts[2]);
        std::string idStr;
        while (std::getline(ids, idStr, ',')) m.borrowBook(std::stoi(idStr));
    }
    return m;
}
