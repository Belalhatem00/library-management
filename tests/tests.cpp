// اختبارات بسيطة من غير مكتبات خارجية: كل دالة بتختبر جزء من النظام
#include "../src/Library.h"
#include <cassert>
#include <cstdio>
#include <iostream>

static void testAddAndFind() {
    Library lib("/tmp/libtest1");
    Book& b = lib.addBook("1984", "George Orwell", 1949);
    assert(b.getId() == 1);
    assert(lib.findBookById(1) != nullptr);
    assert(lib.findBookById(99) == nullptr);
    assert(lib.getBooks().size() == 1);
    std::cout << "testAddAndFind OK\n";
}

static void testSearch() {
    Library lib("/tmp/libtest2");
    lib.addBook("The Hobbit", "Tolkien", 1937);
    lib.addBook("The Lord of the Rings", "Tolkien", 1954);
    lib.addBook("Clean Code", "Robert Martin", 2008);
    assert(lib.searchBooks("hobbit").size() == 1);      // مش حساس لحالة الحروف
    assert(lib.searchBooks("tolkien").size() == 2);     // بحث بالمؤلف
    assert(lib.searchBooks("the").size() == 2);         // بحث جزئي
    assert(lib.searchBooks("xyz").empty());
    std::cout << "testSearch OK\n";
}

static void testBorrowReturn() {
    Library lib("/tmp/libtest3");
    Book& b = lib.addBook("Dune", "Frank Herbert", 1965);
    Member& m = lib.addMember("Belal");
    assert(lib.borrowBook(m.getId(), b.getId()).empty());
    assert(b.isBorrowed());
    assert(!lib.borrowBook(m.getId(), b.getId()).empty()); // معار بالفعل
    assert(!lib.borrowBook(999, b.getId()).empty());       // عضو غير موجود
    assert(lib.returnBook(m.getId(), b.getId()).empty());
    assert(!b.isBorrowed());
    assert(!lib.returnBook(m.getId(), b.getId()).empty()); // مش معاه الكتاب
    std::cout << "testBorrowReturn OK\n";
}

static void testRemoveRules() {
    Library lib("/tmp/libtest4");
    Book& b = lib.addBook("Dune", "Frank Herbert", 1965);
    Member& m = lib.addMember("Belal");
    assert(lib.borrowBook(m.getId(), b.getId()).empty());
    assert(!lib.removeBook(b.getId()));   // معار، ممنوع الحذف
    assert(!lib.removeMember(m.getId())); // عنده كتاب، ممنوع الحذف
    lib.returnBook(m.getId(), b.getId());
    assert(lib.removeBook(b.getId()));
    assert(lib.removeMember(m.getId()));
    std::cout << "testRemoveRules OK\n";
}

static void testPersistence() {
    {
        Library lib("/tmp/libtest5");
        Book& b = lib.addBook("The Pragmatic|Programmer", "Hunt & Thomas", 1999);
        Member& m = lib.addMember("Belal%Hatem");
        lib.borrowBook(m.getId(), b.getId());
        assert(lib.save());
    }
    {
        Library lib("/tmp/libtest5");
        assert(lib.load());
        assert(lib.getBooks().size() == 1);
        assert(lib.getMembers().size() == 1);
        // الرموز الخاصة لازم ترجع زي ما هي
        assert(lib.getBooks()[0].getTitle() == "The Pragmatic|Programmer");
        assert(lib.getMembers()[0].getName() == "Belal%Hatem");
        // حالة الإعارة محفوظة
        assert(lib.getBooks()[0].isBorrowed());
        assert(lib.getMembers()[0].hasBook(lib.getBooks()[0].getId()));
        // الأرقام الجديدة ماتتكررش
        Book& b2 = lib.addBook("New", "New", 2024);
        assert(b2.getId() == 2);
    }
    std::cout << "testPersistence OK\n";
}


static void testIdNeverReusedAfterDeletingHighest() {
    // أعلى رقم اتحذف قبل الحفظ: الرقم لازم ميتكررش بعد إعادة التحميل
    {
        Library lib("/tmp/libtest6");
        lib.addBook("A", "X", 2000);        // id 1
        Book& b2 = lib.addBook("B", "Y", 2001); // id 2
        lib.removeBook(b2.getId());         // حذف أعلى رقم
        assert(lib.save());
    }
    {
        Library lib("/tmp/libtest6");
        assert(lib.load());
        assert(lib.getBooks().size() == 1);
        Book& b3 = lib.addBook("C", "Z", 2002);
        assert(b3.getId() == 3); // مش 2
    }
    std::cout << "testIdNeverReusedAfterDeletingHighest OK\n";
}

int main() {
    testAddAndFind();
    testSearch();
    testBorrowReturn();
    testRemoveRules();
    testPersistence();
    testIdNeverReusedAfterDeletingHighest();
    std::cout << "كل الاختبارات نجحت ✅\n";
    return 0;
}
