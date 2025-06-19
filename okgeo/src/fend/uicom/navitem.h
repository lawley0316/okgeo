#ifndef NAVITEM_H
#define NAVITEM_H

#include <QToolButton>

class NavItem : public QToolButton
{
    Q_OBJECT
public:
    explicit NavItem(QWidget* parent = nullptr);
    void SetDefaultIcon(const QIcon& icon);
    void SetActiveIcon(const QIcon& icon);
    void SetDefaultTextColor(const QColor& color);
    void SetActiveTextColor(const QColor& color);
    void SetDefaultBgColor(const QColor& color);
    void SetActiveBgColor(const QColor& color);
    void Activate();
    void Deactivate();

protected:
    void enterEvent(QEnterEvent* event) override;
    void leaveEvent(QEvent* event) override;
    void UpdateUi();

private:
    QIcon default_icon_;
    QIcon active_icon_;
    QColor default_text_color_;
    QColor active_text_color_;
    QColor default_bg_color_;
    QColor active_bg_color_;

    bool is_active_ = false;
    bool is_hovering_ = false;
};

#endif // NAVITEM_H
