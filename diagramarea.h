#ifndef DIAGRAMAREA_H
#define DIAGRAMAREA_H

#include "diagramitem.h"

#include <QGraphicsScene>

QT_BEGIN_NAMESPACE
class QGraphicsSceneMouseEvent;
class QMenu;
class QPointF;
class QGraphicsLineItem;
class QFont;
class QGraphicsTextItem;
class QColor;
QT_END_NAMESPACE

class DiagramArea : public QGraphicsScene {
    Q_OBJECT
public:
    enum Mode {InsertItem, InsertLine, MoveItem};

    explicit DiagramArea(QObject *parent = nullptr);

    QVector<Edge *> solve();

public slots:
    void setMode(Mode mode);
    void setItemType(DiagramItem::DiagramType type);
    void setLineColor(const QColor &color);

signals:
    void itemInserted(DiagramItem *item);
    void itemSelected(QGraphicsItem *item);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) override;

private:
    bool isItemChange(int type) const;

    DiagramItem::DiagramType myItemType;
    Mode myMode;
    bool leftButtonDown;
    QPointF startPoint;
    QGraphicsLineItem *line;
    QColor myItemColor;
    QColor myLineColor;
    QVector<DiagramItem *> nodes;
};

#endif // DIAGRAMAREA_H
