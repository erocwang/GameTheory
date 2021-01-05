#ifndef DIAGRAMITEM_H
#define DIAGRAMITEM_H

#include <QGraphicsPixmapItem>
#include <QVector>

QT_BEGIN_NAMESPACE
class QPixmap;
class QGraphicsSceneContextMenuEvent;
class QMenu;
class QPolygonF;
QT_END_NAMESPACE

class Edge;

class DiagramItem : public QGraphicsPolygonItem {
public:
    enum DiagramType {StartEnd,Node};
    DiagramItem(DiagramType diagramType, QGraphicsItem *parent = nullptr);
    void removeEdge(Edge *edge);
    void removeEdges();
    DiagramType diagramType() const {
        return myDiagramType;
    }
    QPolygonF polygon() const {
        return myPolygon;
    }
    void addEdge(Edge *edge);
    QPixmap image() const;
    int type() const override {
        return Type;
    }

protected:


private:
    DiagramType myDiagramType;
    QPolygonF myPolygon;
    QVector<Edge*> edges;
};

#endif // DIAGRAMITEM_H
