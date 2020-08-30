#include "widget.h"
#include "ui_widget.h"

#include <QTableView>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QPaintEvent>
#include <QRandomGenerator>
#include <QMenu>
#include <QAction>
#include <QColorDialog>
#include <QDebug>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    init();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::processModelIndx(const QModelIndex &index)
{
    qDebug() << "Column: " << index.column() << "Row: " << index.row();
}

void Widget::popupMenu(const QPoint &pos)
{
    QModelIndex index = tableView->indexAt(pos);
    if (!index.isValid()) {
        qDebug() << "Current QModelIndex is not valid!";
        return;
    }

    if (menu) {
        menu->popup(QCursor::pos());
    }
}

void Widget::selectBackground()
{
    QColor color = QColorDialog::getColor(Qt::red, this, "选择背景颜色", QColorDialog::ShowAlphaChannel);
    qDebug() << color;
    QModelIndexList indexs = tableView->selectionModel()->selectedIndexes();
    for (auto& index : indexs) {
        QStandardItemModel* it = static_cast<QStandardItemModel*>(tableView->model());
        QBrush brush = it->item(index.row(), index.column())->foreground();
        QStandardItem* item = new QStandardItem(index.data().toString());

        item->setBackground(QBrush(color));
        item->setForeground(brush);
        it->setItem(index.row(), index.column(), item);
    }
}

void Widget::selectForeground()
{
    QColor color = QColorDialog::getColor(Qt::white, this, "选择背景颜色", QColorDialog::ShowAlphaChannel);
    QModelIndexList indexs = tableView->selectionModel()->selectedIndexes();
    for (auto& index : indexs) {
        QStandardItemModel* it = static_cast<QStandardItemModel*>(tableView->model());
        QBrush brush = it->item(index.row(), index.column())->background();
        QStandardItem* item = new QStandardItem(index.data().toString());

        item->setBackground(brush);
        item->setForeground(QBrush(color));
        it->setItem(index.row(), index.column(), item);
    }
}

void Widget::init()
{
    tableView = new QTableView(this);
    tableView->resize(width(), height());
    tableView->setContextMenuPolicy(Qt::CustomContextMenu);
    tableView->setSelectionMode(QAbstractItemView::ContiguousSelection);

    connect(tableView, &QTableView::clicked, this, &Widget::processModelIndx);
    connect(tableView, &QTableView::customContextMenuRequested, this, &Widget::popupMenu);

    QStandardItemModel* model = new QStandardItemModel(tableView);
    model->setColumnCount(20);
    model->setRowCount(20);
    for (int i = 0; i < 20; ++i) {
        for (int j = 0; j < 20; ++j) {
            model->setData(model->index(i, j), QVariant(QRandomGenerator::global()->bounded(10, 100)));
        }
    }

    tableView->setModel(model);

    menu = new QMenu(this);
    menu->addAction("test", [] () {
        qDebug() << "Just a test.";
    });
    menu->addAction("test2", [] () {
        qDebug() << "Just a test2.";
    });
    menu->addAction("背景颜色", this, &Widget::selectBackground, QKeySequence(Qt::Key_B));
    menu->addAction("前景颜色", this, &Widget::selectForeground, QKeySequence(Qt::Key_Up));
}
