#include "diagramitem.h"
#include "edge.h"

#include <QGraphicsScene>
#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>
#include <QPainter>

DiagramItem::DiagramItem(DiagramType diagramType, QGraphicsItem *parent) : QGraphicsPolygonItem(parent), myDiagramType(diagramType) {
    QPainterPath path;
    switch (myDiagramType) {
        case StartEnd:
            path.moveTo(200, 50);
            path.arcTo(150, 0, 50, 50, 0, 90);
            path.arcTo(50, 0, 50, 50, 90, 90);
            path.arcTo(50, 50, 50, 50, 180, 90);
            path.arcTo(150, 50, 50, 50, 270, 90);
            path.lineTo(200, 25);
            myPolygon = path.toFillPolygon();
            break;
        case Node:
            myPolygon << QPointF(-10, -10) << QPointF(10, -10)
                      << QPointF(10, 10) << QPointF(-10, 10)
                      << QPointF(-10, -10);
            break;
        default:
            break;
    }
    setPolygon(myPolygon);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
}

void DiagramItem::removeEdge(Edge *edge) {
    edges.removeAll(edge);
}

void DiagramItem::removeEdges() {
    const auto edgesCopy = edges;
    for (Edge *edge : edgesCopy) {
        edge->startItem()->removeEdge(edge);
        edge->endItem()->removeEdge(edge);
        scene()->removeItem(edge);
        delete edge;
    }
}

void DiagramItem::addEdge(Edge *edge) {
    edges.append(edge);
}

QPixmap DiagramItem::image() const {
    QPixmap pixmap(250,250);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.setPen(QPen(Qt::black, 8));
    painter.translate(125,125); \
    painter.drawPolyline(myPolygon);

    return pixmap;
}

QVariant DiagramItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == QGraphicsItem::ItemPositionChange) {
        for (Edge *edge : qAsConst(edges))
            edge->updatePosition();
    }

    return value;
}

