#include "Library.h"
#include <iostream>
#include <limits>
#include <string>

// نقطة البداية: قائمة تفاعلية بسيطة في الترمينال

static int readInt(const std::string& prompt) {
    while (true) {
        std::cout << prompt;
        int value;
        if (std::cin >> value) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        }
        std::cout << "من فضلك اكتب رقم صحيح.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

static std::string readLine(const std::string& prompt) {
    std::cout << prompt;
    std::string line;
    std::getline(std::cin, line);
    return line;
}

static void printBook(const Book& b) {
    std::cout << "  [" << b.getId() << "] " << b.getTitle()
              << " - " << b.getAuthor() << " (" << b.getYear() << ")"
              << (b.isBorrowed() ? "  [معار]" : "  [متاح]") << '\n';
}

int main() {
    Library lib("data");
    if (lib.load()) std::cout << "تم تحميل البيانات المحفوظة.\n";
    else std::cout << "أول تشغيل، هيتعمل ملفات بيانات جديدة في مجلد data.\n";

    while (true) {
        std::cout << "\n===== نظام إدارة المكتبة =====\n"
                  << "1) إضافة كتاب\n"
                  << "2) إضافة عضو\n"
                  << "3) عرض كل الكتب\n"
                  << "4) عرض كل الأعضاء\n"
                  << "5) بحث عن كتاب\n"
                  << "6) إعارة كتاب\n"
                  << "7) إرجاع كتاب\n"
                  << "8) حذف كتاب\n"
                  << "9) حذف عضو\n"
                  << "0) خروج\n";

        switch (readInt("اختيارك: ")) {
        case 1: {
            std::string title = readLine("عنوان الكتاب: ");
            std::string author = readLine("المؤلف: ");
            int year = readInt("سنة النشر: ");
            Book& b = lib.addBook(title, author, year);
            lib.save(); // حفظ تلقائي بعد أي تعديل
            std::cout << "اتضاف الكتاب برقم " << b.getId() << ".\n";
            break;
        }
        case 2: {
            std::string name = readLine("اسم العضو: ");
            Member& m = lib.addMember(name);
            lib.save();
            std::cout << "اتضاف العضو برقم " << m.getId() << ".\n";
            break;
        }
        case 3:
            if (lib.getBooks().empty()) std::cout << "مفيش كتب لسه.\n";
            for (const auto& b : lib.getBooks()) printBook(b);
            break;
        case 4:
            if (lib.getMembers().empty()) std::cout << "مفيش أعضاء لسه.\n";
            for (const auto& m : lib.getMembers()) {
                std::cout << "  [" << m.getId() << "] " << m.getName()
                          << "، مستعير " << m.getBorrowedBookIds().size() << " كتاب\n";
            }
            break;
        case 5: {
            std::string q = readLine("اكتب جزء من العنوان أو اسم المؤلف: ");
            auto results = lib.searchBooks(q);
            if (results.empty()) std::cout << "مفيش نتائج.\n";
            for (const Book* b : results) printBook(*b);
            break;
        }
        case 6: {
            int memberId = readInt("رقم العضو: ");
            int bookId = readInt("رقم الكتاب: ");
            std::string err = lib.borrowBook(memberId, bookId);
            if (err.empty()) lib.save();
            std::cout << (err.empty() ? "تمت الإعارة بنجاح.\n" : "فشلت الإعارة: " + err + "\n");
            break;
        }
        case 7: {
            int memberId = readInt("رقم العضو: ");
            int bookId = readInt("رقم الكتاب: ");
            std::string err = lib.returnBook(memberId, bookId);
            if (err.empty()) lib.save();
            std::cout << (err.empty() ? "تم الإرجاع بنجاح.\n" : "فشل الإرجاع: " + err + "\n");
            break;
        }
        case 8: {
            int id = readInt("رقم الكتاب: ");
            bool ok = lib.removeBook(id);
            if (ok) lib.save();
            std::cout << (ok ? "اتحذف الكتاب.\n"
                             : "مقدرتش أحذف (غير موجود أو معار حالياً).\n");
            break;
        }
        case 9: {
            int id = readInt("رقم العضو: ");
            bool ok = lib.removeMember(id);
            if (ok) lib.save();
            std::cout << (ok ? "اتحذف العضو.\n"
                             : "مقدرتش أحذف (غير موجود أو عنده كتب).\n");
            break;
        }
        case 0:
            if (lib.save()) std::cout << "البيانات محفوظة. مع السلامة!\n";
            else std::cout << "حصل خطأ أثناء الحفظ!\n";
            return 0;
        default:
            std::cout << "اختيار غير معروف.\n";
        }
    }
}
