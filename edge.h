#ifndef EDGE_H
#define EDGE_H

#include <QGraphicsLineItem>
#include <QInputDialog>
#include <QGraphicsTextItem>

class DiagramItem;

class DiagramTextItem;

class Edge : public QGraphicsLineItem{
public:
    enum {Type = UserType + 4};

    Edge(DiagramItem *startItem, DiagramItem *endItem, QGraphicsItem *parent = nullptr);

    int type() const override {
        return Type;
    }
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void setColor(const QColor &color) {
        myColor=color;
    }
    DiagramItem *startItem() const {
        return myStartItem;
    }
    DiagramItem *endItem() const {
        return myEndItem;
    }
    void updatePosition();
protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
private:
    DiagramItem *myStartItem;
    DiagramItem *myEndItem;
    QPolygonF arrowHead;
    QColor myColor = Qt::black;
    QString name;
    QGraphicsTextItem * edgeLabel;
};

#endif // EDGE_H
