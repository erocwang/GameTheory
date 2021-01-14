#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <diagramitem.h>

#include <QMainWindow>

class DiagramArea;

QT_BEGIN_NAMESPACE
class QAction;
class QToolBox;
class QSpinBox;
class QComboBox;
class QFontComboBox;
class QButtonGroup;
class QLineEdit;
class QGraphicsTextItem;
class QFont;
class QToolButton;
class QAbstractButton;
class QGraphicsView;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    static MainWindow * getMainWinPtr();

private slots:
    void backgroundButtonGroupClicked(QAbstractButton *button);
    void buttonGroupClicked(QAbstractButton *button);
    void deleteItem();
    void pointerGroupClicked();
    void itemInserted(DiagramItem *item);
    void sceneScaleChanged(const QString &scale);
    void lineButtonTriggered();
    void itemSelected(QGraphicsItem *item);

private:
    static MainWindow * pMainWindow;

    void createActions();
    void createToolbars();

    QWidget *createBackgroundCellWidget(const QString &text, const QString &image);
    QWidget *createCellWidget(const QString &text, DiagramItem::DiagramType type);

    DiagramArea *scene;
    QGraphicsView *view;

    QAction *exitAction;
    QAction *addAction;
    QAction *deleteAction;
    QAction *solveAction;

    QToolBar *editToolBar;
    QToolBar *pointerToolBar;

    QComboBox *sceneScaleCombo;

    QToolBox *toolBox;
    QButtonGroup *buttonGroup;
    QButtonGroup *pointerTypeGroup;
    QButtonGroup *backgroundButtonGroup;
    QAction *lineAction;

    void solve();

};
#endif // MAINWINDOW_H
