#ifndef LIBRARY_H
#define LIBRARY_H

#include "Book.h"
#include "Member.h"
#include <string>
#include <vector>

// قلب النظام: بيدير الكتب والأعضاء والإعارة والحفظ في ملفات
class Library {
public:
    explicit Library(const std::string& dataDir = "data");

    // إضافة وحذف
    Book& addBook(const std::string& title, const std::string& author, int year);
    Member& addMember(const std::string& name);
    bool removeBook(int bookId);   // تفشل لو الكتاب معار
    bool removeMember(int memberId); // تفشل لو العضو عنده كتب

    // بحث
    Book* findBookById(int id);
    Member* findMemberById(int id);
    std::vector<Book*> searchBooks(const std::string& query); // بحث جزئي في العنوان أو المؤلف، مش حساس لحالة الحروف

    // إعارة وإرجاع. ترجع رسالة خطأ فاضية لو العملية نجحت
    std::string borrowBook(int memberId, int bookId);
    std::string returnBook(int memberId, int bookId);

    // حفظ وتحميل من ملفات نصية جوه dataDir
    bool save() const;
    bool load();

    const std::vector<Book>& getBooks() const;
    const std::vector<Member>& getMembers() const;

private:
    std::vector<Book> books_;
    std::vector<Member> members_;
    std::string dataDir_;
    int nextBookId_ = 1;
    int nextMemberId_ = 1;
};

#endif
