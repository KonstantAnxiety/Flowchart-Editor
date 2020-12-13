#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <memory>
#include "node.h"
#include "terminal.h"
#include "processnode.h"
#include "decisionnode.h"
#include "textnode.h"
#include "textdialog.h"
#include "vector.h"

class GraphicsView: public QGraphicsView
{
    Q_OBJECT
public:
    GraphicsView(QWidget *parent);
    ~GraphicsView();
    void addNode(int id);
    void save();
    void saveAs(QString path);
    void open(QString path);
    const QString &getPath() const { return this->path; };
    bool isSaved() const { return saved; };
    void addGrid();
    void removeGrid();
    void addLabel(const QString &str, double x, double y,
                  const QColor &color = Qt::black, double scale = 1,
                  int align = 0, int valign = 0);
                  // -1:0:1 = left:center:right = top:middle:bottom

private slots:
    void slotAlarmTimer();

private:
    QTimer *timer;
    QGraphicsScene *scene;
    Vector<std::shared_ptr<Node>> nodes;
    QGraphicsItemGroup *grid;
    QGraphicsItemGroup *connections;
    int nodeToAdd, nowMoving, lineDir;
    bool _pan, _line, saved;
    QString path;
    QPoint lastPoint;
    int _panStartX, _panStartY;
    std::shared_ptr<Node> tmp;

    void snapToGrid(QPointF *pt) const;
    int hoveredNode(QMouseEvent *event) const;
    virtual void mouseDoubleClickEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void wheelEvent(QWheelEvent *event) override;
};

#endif // GRAPHICSVIEW_H
