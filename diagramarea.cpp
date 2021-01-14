#include "diagramarea.h"
#include "edge.h"

#include <QGraphicsSceneMouseEvent>
#include <QTextCursor>
#include <QDebug>

#include <bits/stdc++.h>

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
            nodes.append(item);
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
QVector<Edge*> DiagramArea::solve() {
    QVector<Edge *> res;
    unordered_map<DiagramItem*,pair<int,int>> nodeValues;
    unordered_map<DiagramItem*,int> outGoingEdges;
    unordered_set<DiagramItem*> visited;
    for (DiagramItem* node : nodes) {
        nodeValues[node] = node->payoff;
        outGoingEdges[node] = node->outEdges.size();
    }
    while(true) {
        QList<DiagramItem *> toProcess;
        unordered_map<DiagramItem*,Edge*> chosenEdges;
        for(DiagramItem * node : nodes) {
            if(!visited.count(node) && node->parentNode!=nullptr && outGoingEdges[node] == 0) {
                visited.insert(node);
                toProcess.append(node);
            }
        }
        if(toProcess.size()==0) {
            break;
        }
        else {
            qDebug() << "process nodes " << toProcess.size();
            for (DiagramItem * node : toProcess) {
                outGoingEdges[node->parentNode]--;
                pair<int,int> temp = node->payoff;
                qDebug() << "payoff " << temp.first << " " << temp.second;
                if(node->parentNode->player1) {
                    qDebug("parent is p1");
                    if (temp.first > node->parentNode->payoff.first) {
                        qDebug("found edge");
                        nodeValues[node->parentNode] = node->payoff;
                        chosenEdges[node->parentNode] = node->parentEdge;
                    }
                }
                else {
                    if (temp.second > node->parentNode->payoff.second) {
                        nodeValues[node->parentNode] = node->payoff;
                        chosenEdges[node->parentNode] = node->parentEdge;
                    }
                }
            }
        }
        for(auto& p : chosenEdges) {
            res.push_back(p.second);
        }
    }
    qDebug() << "result size " << res.size();
    return res;
}

