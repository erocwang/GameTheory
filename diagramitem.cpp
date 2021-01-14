#include "mainwindow.h"
#include "diagramitem.h"
#include "edge.h"

#include <QGraphicsScene>
#include <QGraphicsSceneContextMenuEvent>
#include <QStringList>
#include <QMenu>
#include <QPainter>
#include <QDebug>

DiagramItem::DiagramItem(DiagramType diagramType, QGraphicsItem *parent) : QGraphicsPolygonItem(parent), myDiagramType(diagramType) {
    QPainterPath path;
    parentNode = nullptr;
    parentEdge = nullptr;
    outEdges = {};
    player1 = true;
    itemColor = player1 ? Qt::black : Qt::blue;
    this->setBrush(itemColor);
    payoff = {0,0};
    payoffLabel = new QGraphicsTextItem(this);
    payoffLabel->setPos(-5,10);
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
    outEdges.removeAll(edge);
}

void DiagramItem::removeEdges() {
    const auto edgesCopy = edges;
    parentEdge = nullptr;
    for (Edge *edge : edgesCopy) {
        edge->startItem()->removeEdge(edge);
        edge->endItem()->removeEdge(edge);
        scene()->removeItem(edge);
        delete edge;
    }
}

void DiagramItem::addEdge(Edge *edge) {
    edges.append(edge);
    if (edge->getStartItem() == this) {
        outEdges.append(edge);
    }
    else {
        parentNode = edge->getStartItem();
        parentEdge = edge;
    }
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
void DiagramItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) {
    MainWindow * mw = MainWindow::getMainWinPtr();
    QStringList players;
    players << "Player 1 Action Node" << "Player 2 Action Node";
    QString player = QInputDialog::getItem(mw, "Node information", "Choose which player acts on this node (no impact on leaf nodes)",players,!player1);
    if (player == "Player 1 Action Node") {
        player1 = true;
        this->setBrush(Qt::black);
    }
    else {
        player1 = false;
        this->setBrush(Qt::blue);
    }
    QString temp = "0,0";
    if (payoff != make_pair(0,0)) {
        temp = QString::fromStdString(to_string(payoff.first) + "," + to_string(payoff.second));
    }
    QString text = QInputDialog::getText(mw, "Node information", "Enter payoffs separated by a comma (e.g. 20,20)", QLineEdit::Normal,temp);
    if(text != "0,0") {
        QStringList res = text.split(",");
        payoff = {res[0].toInt(),res[1].toInt()};
        qDebug() << payoff.first << " " << payoff.second;
        payoffLabel->setPlainText(text);
    }
}

