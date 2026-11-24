#ifndef MEMBER_H
#define MEMBER_H

#include <string>
#include <vector>

// يمثل عضو في المكتبة ويحتفظ بأرقام الكتب اللي مستعيرها
class Member {
public:
    Member() = default;
    Member(int id, const std::string& name);

    int getId() const;
    const std::string& getName() const;
    const std::vector<int>& getBorrowedBookIds() const;

    void borrowBook(int bookId);
    bool returnBook(int bookId); // ترجع false لو الكتاب مش عنده
    bool hasBook(int bookId) const;

    std::string serialize() const;
    static Member deserialize(const std::string& line);

private:
    int id_ = 0;
    std::string name_;
    std::vector<int> borrowedBookIds_;
};

#endif
