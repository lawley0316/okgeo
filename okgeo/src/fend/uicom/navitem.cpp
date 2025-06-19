#include "navitem.h"

NavItem::NavItem(QWidget* parent)
    : QToolButton(parent) {
    setCursor(Qt::PointingHandCursor);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
}

void NavItem::SetDefaultIcon(const QIcon& icon) {
    default_icon_ = icon;
    UpdateUi();
}

void NavItem::SetActiveIcon(const QIcon& icon) {
    active_icon_ = icon;
    UpdateUi();
}

void NavItem::SetDefaultTextColor(const QColor& color) {
    default_text_color_ = color;
    UpdateUi();
}

void NavItem::SetActiveTextColor(const QColor& color) {
    active_text_color_ = color;
    UpdateUi();
}

void NavItem::SetDefaultBgColor(const QColor& color) {
    default_bg_color_ = color;
    UpdateUi();
}

void NavItem::SetActiveBgColor(const QColor& color) {
    active_bg_color_ = color;
    UpdateUi();
}

void NavItem::Activate() {
    is_active_ = true;
    UpdateUi();
}

void NavItem::Deactivate() {
    is_active_ = false;
    UpdateUi();
}

void NavItem::enterEvent(QEnterEvent* event) {
    is_hovering_ = true;
    UpdateUi();
    QToolButton::enterEvent(event);
}

void NavItem::leaveEvent(QEvent* event) {
    is_hovering_ = false;
    UpdateUi();
    QToolButton::leaveEvent(event);
}

void NavItem::UpdateUi() {
    bool ok = is_active_ || is_hovering_;

    // icon settings
    if (! default_icon_.isNull()) {  // omit icon settings when default icon is null
        if (active_icon_.isNull()) {
            setIcon(default_icon_);
        } else {
            setIcon(ok ? active_icon_ : default_icon_);
        }
    }

    QStringList style = {"QToolButton {"};

    // text color settings
    if (default_text_color_.isValid()) {  // omit text color settings when default text color is invalid
        QString text_color;
        if (active_text_color_.isValid()) {
            text_color = ok ? active_text_color_.name() : default_text_color_.name();
        } else {
            text_color = default_text_color_.name();
        }
        style.append("color: " + text_color + ";");
    }

    // background color settings
    if (default_bg_color_.isValid()) {  // omit background color settings when default background color is invalid
        QString bg_color;
        if (active_bg_color_.isValid()) {
            bg_color = ok ? active_bg_color_.name() : default_bg_color_.name();
        } else {
            bg_color = default_bg_color_.name();
        }
        style.append("background-color: " + bg_color + ";");
    }

    style.append("border: none;");
    style.append("}");

    setStyleSheet(style.join('\n'));
}
