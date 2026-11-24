#ifndef BOOK_H
#define BOOK_H

#include <string>

// يمثل كتاب واحد في المكتبة
class Book {
public:
    Book() = default;
    Book(int id, const std::string& title, const std::string& author, int year);

    int getId() const;
    const std::string& getTitle() const;
    const std::string& getAuthor() const;
    int getYear() const;
    bool isBorrowed() const;

    void borrow();   // تسجيل إعارة الكتاب
    void giveBack(); // تسجيل رجوع الكتاب

    // تحويل الكتاب لسطر نصي عشان يتحفظ في ملف
    std::string serialize() const;
    // قراءة كتاب من سطر نصي (ترمي std::invalid_argument لو السطر بايظ)
    static Book deserialize(const std::string& line);

private:
    int id_ = 0;
    std::string title_;
    std::string author_;
    int year_ = 0;
    bool borrowed_ = false;
};

#endif
