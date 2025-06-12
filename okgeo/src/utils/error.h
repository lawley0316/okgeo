#ifndef ERROR_H
#define ERROR_H

#include <QString>
#include <QHash>
#include <QCoreApplication>

class Error : public std::exception
{
public:
    Error() = default;
    Error(const QString& code, const QString& title) : code(code) , title(title) {}
    Error(const QString& code, const QString& title, const QString detail) : code(code) , title(title), detail(detail) {}
    QString getCode() const { return code; }
    QString getTitle() const { return title; }
    QString getDetail() const { return detail; }
    void setDetail(const QString& detail) { this->detail = detail; }
    QString message() const {
        if (detail.isEmpty()) {
            return code + " - " + title;
        } else {
            return code + " - " + title + ": " + detail;
        }
    }
private:
    QString code;
    QString title;
    QString detail;
};

namespace errors {
    namespace convert {
        namespace probeFile {
            static Error parseFailed(const QString& detail="") { return {"111", QCoreApplication::tr("failed to parse probe file"), detail}; }
        }
        namespace annoFile {
            static Error parseFailed(const QString& detail="") { return {"121", QCoreApplication::tr("failed to parse annotaiton file"), detail}; }
        }
        namespace map {
            static Error mapFailed(const QString& detail="") { return {"131", QCoreApplication::tr("failed to map probe to gene"), detail}; }
        }
    }
}

#endif // ERROR_H
