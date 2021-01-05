#include "diagramarea.h"
#include "edge.h"

#include <QGraphicsSceneMouseEvent>
#include <QTextCursor>

DiagramArea::DiagramArea(QObject *parent) : QGraphicsScene(parent) {
    myMode = MoveItem;
    myItemType = DiagramItem::Node;
    line = nullptr;
    myItemColor = Qt::black;
    myLineColor = Qt::black;
}
void DiagramArea::setMode(Mode mode) {
    myMode = mode;
}
void DiagramArea::setItemType(DiagramItem::DiagramType type) {
    myItemType = type;
}
void DiagramArea::setLineColor(const QColor &color) {
    myLineColor = color;
    if (isItemChange(Edge::Type)) {
        Edge *item = qgraphicsitem_cast<Edge *>(selectedItems().first());
        item->setColor(myLineColor);
        update();
    }
}
void DiagramArea::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) {
    if (mouseEvent->button() != Qt::LeftButton) {
        return;
    }

    DiagramItem *item;
    switch (myMode) {
        case InsertItem:
            item = new DiagramItem(myItemType);
            item->setBrush(myItemColor);
            addItem(item);
            item->setPos(mouseEvent->scenePos());
            emit itemInserted(item);
            break;
        case InsertLine:
            line = new QGraphicsLineItem(QLineF(mouseEvent->scenePos(),mouseEvent->scenePos()));
            line->setPen(QPen(myLineColor,2));
            addItem(line);
            break;
        default:
            break;
    }
    QGraphicsScene::mousePressEvent(mouseEvent);
}
void DiagramArea::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) {
    if (myMode == InsertLine && line != nullptr) {
        QLineF newLine(line->line().p1(), mouseEvent->scenePos());
        line->setLine(newLine);
    }
    else if (myMode == MoveItem) {
        QGraphicsScene::mouseMoveEvent(mouseEvent);
    }
}
void DiagramArea::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) {
    if (line != nullptr && myMode == InsertLine) {
        QList<QGraphicsItem *> startItems = items(line->line().p1());
        if (startItems.count() && startItems.first() == line) {
            startItems.removeFirst();
        }
        QList<QGraphicsItem *> endItems = items(line->line().p2());
        if (endItems.count() && endItems.first() == line) {
            endItems.removeFirst();
        }
        removeItem(line);
        delete line;

        if (startItems.count() > 0 && endItems.count() > 0 &&
            startItems.first()->type() == DiagramItem::Type &&
            endItems.first()->type() == DiagramItem::Type &&
            startItems.first() != endItems.first()) {
            DiagramItem *startItem = qgraphicsitem_cast<DiagramItem *>(startItems.first());
            DiagramItem *endItem = qgraphicsitem_cast<DiagramItem *>(endItems.first());
            Edge *edge = new Edge(startItem,endItem);
            edge->setColor(myLineColor);
            startItem->addEdge(edge);
            endItem->addEdge(edge);
            edge->setZValue(-1000.0);
            addItem(edge);
            edge->updatePosition();
        }
    }
    line = nullptr;
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}
bool DiagramArea::isItemChange(int type) const {
    const QList<QGraphicsItem *> items = selectedItems();
    const auto cb = [type](const QGraphicsItem *item) {
        return item->type() == type;
    };
    return std::find_if(items.begin(), items.end(), cb) != items.end();
}
