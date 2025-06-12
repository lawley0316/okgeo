#ifndef NAVITEM_H
#define NAVITEM_H

#include <QToolButton>

class NavItem : public QToolButton
{
    Q_OBJECT
public:
    explicit NavItem(QWidget* parent = nullptr);
    void setDefaultIcon(const QIcon& icon);
    void setActiveIcon(const QIcon& icon);
    void setDefaultTextColor(const QColor& color);
    void setActiveTextColor(const QColor& color);
    void setDefaultBgColor(const QColor& color);
    void setActiveBgColor(const QColor& color);
    void activate();
    void deactivate();

protected:
    void enterEvent(QEnterEvent* event) override;
    void leaveEvent(QEvent* event) override;
    void updateUi();

private:
    QIcon defaultIcon;
    QIcon activeIcon;
    QColor defaultTextColor;
    QColor activeTextColor;
    QColor defaultBgColor;
    QColor activeBgColor;

    bool isActive = false;
    bool isHovering = false;
};

#endif // NAVITEM_H
