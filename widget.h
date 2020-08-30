#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
class QTableView;
class QMenu;
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

protected:

private slots:
    void processModelIndx(const QModelIndex& index);
    void popupMenu(const QPoint &pos);
    void selectBackground();
    void selectForeground();

private:
    Ui::Widget *ui;

    QTableView* tableView;
    QMenu* menu;

private:
    void init();
};
#endif // WIDGET_H
