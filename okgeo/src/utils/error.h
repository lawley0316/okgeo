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

class Error : public std::exception
{
public:
    Error() = default;
    Error(const QString& code, const QString& title)
        : mCode(code)
        , mTitle(title)
    {
    }
    Error(const QString& code, const QString& title, const QString detail)
        : mCode(code)
        , mTitle(title)
        , mDetail(detail)
    {
    }
    QString GetCode() const
    {
        return mCode;
    }
    QString GetTitle() const
    {
        return mTitle;
    }
    QString GetDetail() const
    {
        return mDetail;
    }
    void SetDetail(const QString& detail)
    {
        mDetail = detail;
    }
    QString Message() const
    {
        if (mDetail.isEmpty())
        {
            return mCode + " - " + mTitle;
        }
        else
        {
            return mCode + " - " + mTitle + ": " + mDetail;
        }
    }
    static Error Get(const QString& code, const QString& detail="")
    {
        return {code, kErrors[code], detail};
    }
private:
    QString mCode;
    QString mTitle;
    QString mDetail;
};

#endif // ERROR_H
