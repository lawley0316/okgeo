#include "language.h"

#include <QStringList>
#include <QDir>
#include <QLocale>
#include <QLibraryInfo>
#include <QCoreApplication>

Language::Language(QObject *parent) : QObject{parent} {
    QStringList qt_translation_dirs = {
        QLibraryInfo::path(QLibraryInfo::TranslationsPath),
        QCoreApplication::applicationDirPath() + "/translations"
    };
    for (const QString& qt_translation_dir : qt_translation_dirs) {
        if (QDir(qt_translation_dir).exists()) {
            qt_translations_dir_ = qt_translation_dir;
            break;
        }
    }
    app_translations_dir_ = ":/static/translations";
}

void Language::SetLanguage(QLocale::Language language) {
    SetQtLanguage(language);
    SetAppLanguage(language);
    emit LanguageChanged();
}

void Language::Initialize() {
    SetLanguage(QLocale::system().language());
}

void Language::SetQtLanguage(QLocale::Language language) {
    qApp->removeTranslator(&qt_translator_);
    QString code = language == QLocale::Chinese ? "zh_CN" : "en";
    QString file = qt_translations_dir_ + QString("/qtbase_%1.qm").arg(code);
    if (qt_translator_.load(file)) {
        qApp->installTranslator(&qt_translator_);
    }
}

void Language::SetAppLanguage(QLocale::Language language) {
    qApp->removeTranslator(&app_translator_);
    QString code = language == QLocale::Chinese ? "zh_CN" : "en";
    QString file = app_translations_dir_ + QString("/app_%1.qm").arg(code);
    if (app_translator_.load(file)) {
        qApp->installTranslator(&app_translator_);
    }
}
