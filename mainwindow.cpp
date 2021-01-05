#include "diagramarea.h"
#include "diagramitem.h"
#include "edge.h"
#include "mainwindow.h"

#include <QtWidgets>

MainWindow::MainWindow() {
    createActions();

    scene = new DiagramArea(this);
    scene->setSceneRect(QRectF(0, 0, 5000, 5000));
    connect(scene, &DiagramArea::itemInserted, this, &MainWindow::itemInserted);
    connect(scene, &DiagramArea::itemSelected, this, &MainWindow::itemSelected);
    createToolbars();

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(toolBox);
    view = new QGraphicsView(scene);
    layout->addWidget(view);

    QWidget *widget = new QWidget;
    widget->setLayout(layout);

    setCentralWidget(widget);
    setWindowTitle(tr("Diagramarea"));
    setUnifiedTitleAndToolBarOnMac(true);
}
void MainWindow::backgroundButtonGroupClicked(QAbstractButton *button) {
    const QList<QAbstractButton *> buttons = backgroundButtonGroup->buttons();
    for (QAbstractButton *myButton : buttons) {
        if (myButton != button) {
            button->setChecked(false);
        }
    }
    QString text = button->text();

    // maybe add rest later?

    scene->update();
    view->update();
}
void MainWindow::buttonGroupClicked(QAbstractButton *button) {
    const QList<QAbstractButton *> buttons = buttonGroup->buttons();
    for (QAbstractButton *myButton : buttons) {
        if (myButton != button) {
            button->setChecked(false);
        }
    }
    const int id = buttonGroup->id(button);
    scene->setItemType(DiagramItem::DiagramType(id));
    scene->setMode(DiagramArea::InsertItem);
}
void MainWindow::deleteItem() {
    QList<QGraphicsItem *> selectedItems = scene->selectedItems();
    for (QGraphicsItem *item : qAsConst(selectedItems)) {
        if (item->type() == Edge::Type) {
            scene->removeItem(item);
            Edge *edge = qgraphicsitem_cast<Edge *>(item);
            edge->startItem()->removeEdge(edge);
            edge->endItem()->removeEdge(edge);
            delete item;
        }
    }

    selectedItems = scene->selectedItems();
    for (QGraphicsItem *item : qAsConst(selectedItems)) {
        if (item->type() == DiagramItem::Type) {
            qgraphicsitem_cast<DiagramItem *>(item)->removeEdges();
            scene->removeItem(item);
            delete item;
        }
    }
}
void MainWindow::pointerGroupClicked() {
    scene->setMode(DiagramArea::Mode(pointerTypeGroup->checkedId()));
}
void MainWindow::itemInserted(DiagramItem *item) {
    pointerTypeGroup->button(int(DiagramArea::MoveItem))->setChecked(true);
    scene->setMode(DiagramArea::Mode(pointerTypeGroup->checkedId()));
    //buttonGroup->button(int(item->diagramType()))->setChecked(false);
}
void MainWindow::itemSelected(QGraphicsItem *item) {

}
void MainWindow::sceneScaleChanged(const QString &scale) {
    double newScale = scale.left(scale.indexOf(tr("%"))).toDouble() / 100.0;
    QTransform oldMatrix = view->transform();
    view->resetTransform();
    view->translate(oldMatrix.dx(), oldMatrix.dy());
    view->scale(newScale, newScale);
}
void MainWindow::lineButtonTriggered() {
    scene->setLineColor(qvariant_cast<QColor>(lineAction->data()));
}
void MainWindow::createActions() {

    deleteAction = new QAction(QIcon(":/images/delete.png"), tr("&Delete"), this);
    deleteAction->setShortcut(tr("Delete"));
    deleteAction->setStatusTip(tr("Delete item from diagram"));
    connect(deleteAction, &QAction::triggered, this, &MainWindow::deleteItem);
}
void MainWindow::createToolbars() {
    editToolBar = addToolBar(tr("Edit"));
    editToolBar->addAction(deleteAction);

    QToolButton *pointerButton = new QToolButton;
    pointerButton->setCheckable(true);
    pointerButton->setChecked(true);
    pointerButton->setIcon(QIcon(":/images/pointer.png"));
    QToolButton *linePointerButton = new QToolButton;
    linePointerButton->setCheckable(true);
    linePointerButton->setIcon(QIcon(":/images/linepointer.png"));

    QToolButton *nodeButton = new QToolButton;
    nodeButton->setCheckable(true);
    nodeButton->setIcon(QIcon(":/images/node.png"));

    pointerTypeGroup = new QButtonGroup(this);
    pointerTypeGroup->addButton(pointerButton, int(DiagramArea::MoveItem));
    pointerTypeGroup->addButton(linePointerButton, int(DiagramArea::InsertLine));
    pointerTypeGroup->addButton(nodeButton, int(DiagramArea::InsertItem));
    connect(pointerTypeGroup, QOverload<QAbstractButton *>::of(&QButtonGroup::buttonClicked),
            this, &MainWindow::pointerGroupClicked);

    sceneScaleCombo = new QComboBox;
    QStringList scales;
    scales << tr("50%") << tr("75%") << tr("100%") << tr("125%") << tr("150%");
    sceneScaleCombo->addItems(scales);
    sceneScaleCombo->setCurrentIndex(2);
    connect(sceneScaleCombo, &QComboBox::currentTextChanged,
            this, &MainWindow::sceneScaleChanged);

    pointerToolBar = addToolBar(tr("Pointer type"));
    pointerToolBar->addWidget(pointerButton);
    pointerToolBar->addWidget(linePointerButton);
    pointerToolBar->addWidget(nodeButton);
    pointerToolBar->addWidget(sceneScaleCombo);
}


