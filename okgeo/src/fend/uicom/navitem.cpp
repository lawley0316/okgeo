#include "navitem.h"

NavItem::NavItem(QWidget* parent)
    : QToolButton(parent)
{
    setCursor(Qt::PointingHandCursor);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
}

void NavItem::setDefaultIcon(const QIcon& icon)
{
    defaultIcon = icon;
    updateUi();
}

void NavItem::setActiveIcon(const QIcon& icon)
{
    activeIcon = icon;
    updateUi();
}

void NavItem::setDefaultTextColor(const QColor& color)
{
    defaultTextColor = color;
    updateUi();
}

void NavItem::setActiveTextColor(const QColor& color)
{
    activeTextColor = color;
    updateUi();
}

void NavItem::setDefaultBgColor(const QColor& color)
{
    defaultBgColor = color;
    updateUi();
}

void NavItem::setActiveBgColor(const QColor& color)
{
    activeBgColor = color;
    updateUi();
}

void NavItem::activate()
{
    isActive = true;
    updateUi();
}

void NavItem::deactivate()
{
    isActive = false;
    updateUi();
}

void NavItem::enterEvent(QEnterEvent* event)
{
    isHovering = true;
    updateUi();
    QToolButton::enterEvent(event);
}

void NavItem::leaveEvent(QEvent* event)
{
    isHovering = false;
    updateUi();
    QToolButton::leaveEvent(event);
}

void NavItem::updateUi()
{
    bool ok = isActive || isHovering;

    // icon settings
    if (! defaultIcon.isNull()) {  // omit icon settings when default icon is null
        if (activeIcon.isNull()) {
            setIcon(defaultIcon);
        } else {
            setIcon(ok ? activeIcon : defaultIcon);
        }
    }

    QStringList style = {"QToolButton {"};

    // text color settings
    if (defaultTextColor.isValid()) {  // omit text color settings when default text color is invalid
        QString textColor;
        if (activeTextColor.isValid()) {
            textColor = ok ? activeTextColor.name() : defaultTextColor.name();
        } else {
            textColor = defaultTextColor.name();
        }
        style.append("color: " + textColor + ";");
    }

    // background color settings
    if (defaultBgColor.isValid()) {  // omit background color settings when default background color is invalid
        QString bgColor;
        if (activeBgColor.isValid()) {
            bgColor = ok ? activeBgColor.name() : defaultBgColor.name();
        } else {
            bgColor = defaultBgColor.name();
        }
        style.append("background-color: " + bgColor + ";");
    }

    style.append("border: none;");
    style.append("}");

    setStyleSheet(style.join('\n'));
}
