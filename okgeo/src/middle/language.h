#ifndef LANGUAGE_H
#define LANGUAGE_H

#include <QString>
#include <QObject>
#include <QTranslator>
#include <QLocale>

class Language : public QObject
{
    Q_OBJECT
public:
    explicit Language(QObject *parent = nullptr);
    void Initialize();

public slots:
    void SetLanguage(QLocale::Language language);

signals:
    void LanguageChanged();

private:
    void SetQtLanguage(QLocale::Language language);
    void SetAppLanguage(QLocale::Language language);

private:
    QString qt_translations_dir_;
    QString app_translations_dir_;
    QTranslator qt_translator_;
    QTranslator app_translator_;
};

#endif // LANGUAGE_H
