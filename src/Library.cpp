#include "Library.h"
#include <algorithm>
#include <cctype>
#include <filesystem>
#include <fstream>

Library::Library(const std::string& dataDir) : dataDir_(dataDir) {}

Book& Library::addBook(const std::string& title, const std::string& author, int year) {
    books_.emplace_back(nextBookId_++, title, author, year);
    return books_.back();
}

Member& Library::addMember(const std::string& name) {
    members_.emplace_back(nextMemberId_++, name);
    return members_.back();
}

bool Library::removeBook(int bookId) {
    auto it = std::find_if(books_.begin(), books_.end(),
                           [&](const Book& b) { return b.getId() == bookId; });
    if (it == books_.end() || it->isBorrowed()) return false;
    books_.erase(it);
    return true;
}

bool Library::removeMember(int memberId) {
    auto it = std::find_if(members_.begin(), members_.end(),
                           [&](const Member& m) { return m.getId() == memberId; });
    if (it == members_.end() || !it->getBorrowedBookIds().empty()) return false;
    members_.erase(it);
    return true;
}

Book* Library::findBookById(int id) {
    for (auto& b : books_) if (b.getId() == id) return &b;
    return nullptr;
}

Member* Library::findMemberById(int id) {
    for (auto& m : members_) if (m.getId() == id) return &m;
    return nullptr;
}

static std::string toLower(const std::string& s) {
    std::string out = s;
    std::transform(out.begin(), out.end(), out.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    return out;
}

std::vector<Book*> Library::searchBooks(const std::string& query) {
    std::vector<Book*> results;
    std::string q = toLower(query);
    for (auto& b : books_) {
        if (toLower(b.getTitle()).find(q) != std::string::npos ||
            toLower(b.getAuthor()).find(q) != std::string::npos) {
            results.push_back(&b);
        }
    }
    return results;
}

std::string Library::borrowBook(int memberId, int bookId) {
    Member* m = findMemberById(memberId);
    if (!m) return "member not found";
    Book* b = findBookById(bookId);
    if (!b) return "book not found";
    if (b->isBorrowed()) return "book already borrowed";
    b->borrow();
    m->borrowBook(bookId);
    return "";
}

std::string Library::returnBook(int memberId, int bookId) {
    Member* m = findMemberById(memberId);
    if (!m) return "member not found";
    Book* b = findBookById(bookId);
    if (!b) return "book not found";
    if (!m->hasBook(bookId)) return "member does not have this book";
    b->giveBack();
    m->returnBook(bookId);
    return "";
}

bool Library::save() const {
    std::filesystem::create_directories(dataDir_);
    std::ofstream booksFile(dataDir_ + "/books.txt", std::ios::trunc);
    std::ofstream membersFile(dataDir_ + "/members.txt", std::ios::trunc);
    if (!booksFile || !membersFile) return false;
    booksFile << "next_id " << nextBookId_ << '\n';
    for (const auto& b : books_) booksFile << b.serialize() << '\n';
    membersFile << "next_id " << nextMemberId_ << '\n';
    for (const auto& m : members_) membersFile << m.serialize() << '\n';
    return true;
}

bool Library::load() {
    std::ifstream booksFile(dataDir_ + "/books.txt");
    std::ifstream membersFile(dataDir_ + "/members.txt");
    if (!booksFile || !membersFile) return false; // أول تشغيل: مفيش ملفات لسه

    books_.clear();
    members_.clear();

    std::string line;
    if (std::getline(booksFile, line) && line.rfind("next_id ", 0) == 0)
        nextBookId_ = std::stoi(line.substr(8));
    while (std::getline(booksFile, line))
        if (!line.empty()) books_.push_back(Book::deserialize(line));

    if (std::getline(membersFile, line) && line.rfind("next_id ", 0) == 0)
        nextMemberId_ = std::stoi(line.substr(8));
    while (std::getline(membersFile, line))
        if (!line.empty()) members_.push_back(Member::deserialize(line));
    return true;
}

const std::vector<Book>& Library::getBooks() const { return books_; }
const std::vector<Member>& Library::getMembers() const { return members_; }
