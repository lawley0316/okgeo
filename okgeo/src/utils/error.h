#ifndef ERROR_H
#define ERROR_H

#include <QString>
#include <QHash>
#include <QCoreApplication>

static const QHash<QString, QString> kErrors = {
    {"111", QCoreApplication::tr("failed to parse probe file")},
    {"121", QCoreApplication::tr("failed to parse annotaiton file")},
    {"131", QCoreApplication::tr("failed to map probe to gene")},
    {"211", QCoreApplication::tr("failed to parse series matrix file")},
    {"221", QCoreApplication::tr("failed to write phenotype to file")},
};

static const QList<QString> kErrorDetails = {
    QCoreApplication::tr("malformed probe file"),
    QCoreApplication::tr("inconsistent number of columns across rows"),
    QCoreApplication::tr("malformed annotatinon file"),
    QCoreApplication::tr("invalid gene symbol column"),
    QCoreApplication::tr("invalid probe aggregate method"),
    QCoreApplication::tr("failed to open file"),
    QCoreApplication::tr("malformed series matrix file"),
    QCoreApplication::tr("inconsistent number of rows across columns"),
    QCoreApplication::tr("failed to open file")
};

class Error : public std::exception {
public:
    Error() = default;
    Error(const QString& code, const QString& title)
        : code_(code),
        title_(title) {}
    Error(const QString& code, const QString& title, const QString detail)
        : code_(code)
        , title_(title)
        , detail_(detail) {}
    QString GetCode() const { return code_; }
    QString GetTitle() const { return title_; }
    QString GetDetail() const { return detail_; }
    void SetDetail(const QString& detail) { detail_ = detail; }
    QString Message() const {
        if (detail_.isEmpty()) {
            return code_ + " - " + title_;
        } else {
            return code_ + " - " + title_ + ": " + detail_;
        }
    }
    static Error Get(const QString& code, const QString& detail="") {
        return {
            code,
            QCoreApplication::tr(kErrors[code].toUtf8()),
            QCoreApplication::tr(detail.toUtf8())
        };
    }
private:
    QString code_;
    QString title_;
    QString detail_;
};

#endif // ERROR_H
