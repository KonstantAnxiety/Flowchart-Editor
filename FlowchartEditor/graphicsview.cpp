#include <QScrollBar>
#include <QMouseEvent>
#include <QGraphicsTextItem>
#include "graphicsview.h"
#include <QDebug>
#include <QTimer>
#include <QFont>
#include <math.h>

GraphicsView::GraphicsView(QWidget *parent)
    : QGraphicsView(parent)
{
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    this->setMinimumHeight(100);
    this->setMinimumWidth(300);

    scene = new QGraphicsScene();
    this->setScene(scene);

    scene->setSceneRect(-1000, -1000, 1000, 1000);

    nodeToAdd = 0;

    timer = new QTimer();               // Инициализируем Таймер
    //timer->setSingleShot(true);
    // Подключаем СЛОТ для отрисовки к таймеру
    connect(timer, SIGNAL(timeout()), this, SLOT(slotAlarmTimer()));
    timer->start(20);                   // Стартуем таймер на 50 миллисекунд

    grid = new QGraphicsItemGroup();
    addGrid();
    grid->setZValue(1);
    scene->addItem(grid);

    connections = new QGraphicsItemGroup;
    connections->setZValue(1);
    scene->addItem(connections);
    nowMoving = -1;
    _pan = false;
    _line = false;
    saved = true;
}

GraphicsView::~GraphicsView()
{

}

void GraphicsView::addNode(int id)
{
    nodeToAdd = id;
    if (nodeToAdd < 5) {
        tmp.reset();
        switch (nodeToAdd) {
        case 0:
            break;
        case 1:
            tmp = std::shared_ptr<Node>(new Terminal);
            break;
        case 2:
            tmp = std::shared_ptr<Node>(new ProcessNode);
            break;
        case 3:
            tmp = std::shared_ptr<Node>(new DecisionNode);
            break;
        case 4:
            tmp = std::shared_ptr<Node>(new TextNode);
            break;
        }
    }
}

void GraphicsView::save()
{
    if (!path.isEmpty()) {
        saveAs(path);
        saved = true;
    }
}

void GraphicsView::saveAs(QString path)
{
    std::ofstream file(path.toStdString());
    file << nodes.getSize() << '\n';
    for (size_t i = 0; i < nodes.getSize(); ++i) {
        nodes[i].get()->write(file);
    }
    file << connections->childItems().size() << '\n';
    foreach(QGraphicsItem *item, scene->items()) {
        if (item->group() == connections) {
            QGraphicsLineItem *line = dynamic_cast<QGraphicsLineItem*>(item);
            QLineF l = line->line();
            file << l.p1().x() << ' ' <<
                    l.p1().y() << ' ' <<
                    l.p2().x() << ' ' <<
                    l.p2().y() << '\n';
        }
    }
    file.close();
    this->path = path;
    saved = true;
}

void GraphicsView::open(QString path)
{
    std::ifstream file(path.toStdString());
    std::shared_ptr<Node> t;
    size_t n;
    file >> n;
    double x, y;
    char c;
    std::string qq;
    nodes.clear();
    for (size_t i = 0; i < n; ++i) {
        file >> c >> x >> y;
        std::getline(file, qq);
        qq = qq.substr(1, qq.size()-1);
        switch (c){
        case 'P':
            t = std::shared_ptr<Node>(new ProcessNode(x, y));
            break;
        case 'T':
            t = std::shared_ptr<Node>(new Terminal(x, y));
            break;
        case 'D':
            t = std::shared_ptr<Node>(new DecisionNode(x, y));
            break;
        case 't':
            t = std::shared_ptr<Node>(new TextNode(x, y));
            break;
        default:
            t = std::shared_ptr<Node>(new Terminal(x, y));
            break;
        }
        t->setText(QString::fromStdString(qq));
        nodes.push_back(t);
    }
    file >> n;

    foreach(QGraphicsItem *item, scene->items()) {
        if (item->group() == connections) {
            delete item;
        }
    }
    double x1, x2, y1, y2;
    for (size_t i = 0; i < n; ++i) {
        file >> x1 >> y1 >> x2 >> y2;
        connections->addToGroup(scene->addLine(x1, y1, x2, y2, QPen(Qt::black, 2)));
    }
}

void GraphicsView::addLabel(const QString &str, double x, double y,
                            const QColor &color, double scale,
                            int align, int valign)
{
    QGraphicsTextItem *io = new QGraphicsTextItem;
    io->setFont(QFont("Courier New", 8));
    io->setScale(scale);
    io->setPlainText(str);
    if (align > 0)
        x -= io->boundingRect().width()*scale;
    else if (align == 0)
        x -= io->boundingRect().width()*scale/2;
    if (valign > 0)
        y -= io->boundingRect().height()*scale;
    else if (valign == 0)
        y -= io->boundingRect().height()*scale/2;
    io->setPos(x, y);
    io->setDefaultTextColor(color);
    io->setZValue(7);
    scene->addItem(io);
    labels.push_back(io);
}

void GraphicsView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::MiddleButton) {
        _pan = true;
        _panStartX = event->x();
        _panStartY = event->y();
        setCursor(Qt::ClosedHandCursor);
        event->accept();
        return;
    } else if (event->button() == Qt::LeftButton && nodeToAdd == 0
               && nowMoving == -1) {
        int i = hoveredNode(event);
        if (i > -1) {
            nowMoving = i;
            lastPoint.setX(nodes[i]->cx());
            lastPoint.setY(nodes[i]->cy());
            tmp = nodes[i];
        }
    }
    event->ignore();
}

void GraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
    std::shared_ptr<Node> t;
    TextDialog td;
    QPointF pt = mapToScene(event->pos());
    snapToGrid(&pt);

    if (event->button() == Qt::MiddleButton) {
        _pan = false;
        setCursor(Qt::ArrowCursor);
        event->accept();
        return;
    } else if (event->button() == Qt::LeftButton) {
        if (nowMoving > -1) {
            nodes[nowMoving].get()->setPos(tmp.get()->cx(), tmp.get()->cy());
            nodes[nowMoving].get()->setText(tmp.get()->getText());
            nowMoving = -1;
            tmp.reset();
            saved = false;
        } else {
            bool node = false;
            switch (nodeToAdd) {
            case 0:
                break;
            case 1:
                t = std::shared_ptr<Node>(new Terminal(pt.x(), pt.y()));
                node = true;
                break;
            case 2:
                t = std::shared_ptr<Node>(new ProcessNode(pt.x(), pt.y()));
                node = true;
                break;
            case 3:
                t = std::shared_ptr<Node>(new DecisionNode(pt.x(), pt.y()));
                node = true;
                break;
            case 4:
                t = std::shared_ptr<Node>(new TextNode(pt.x(), pt.y()));
                node = true;
                break;
            case 5:
                QPoint newPoint;
                if (!_line) {
                    newPoint = QPoint(pt.x(), pt.y());
                    _line = true;
                } else {
                    double dx = abs(lastPoint.x()-pt.x());
                    double dy = abs(lastPoint.y()-pt.y());
                    if (dx < dy)
                        newPoint = QPoint(lastPoint.x(), pt.y());
                    else {
                        newPoint = QPoint(pt.x(), lastPoint.y());
                    }
                    connections->addToGroup(scene->addLine(lastPoint.x(), lastPoint.y(),
                                   newPoint.x(), newPoint.y(), QPen(Qt::black, 2)));
                    dx = newPoint.x() - lastPoint.x();
                    dy = newPoint.y() - lastPoint.y();
                    if (dx == 0) {
                        if (dy < 0)
                            lineDir = 0;
                        else
                            lineDir = 2;
                    } else if (dy == 0) {
                        if (dx < 0)
                            lineDir = 3;
                        else
                            lineDir = 1;
                    }
                }
                lastPoint = newPoint;
            }
            if (node) {
                if (td.exec() == QDialog::Accepted) {
                    QString text;
                    text = td.getText();
                    t->setText(text);
                }
                nodes.push_back(t);
                saved = false;
            }
            if (nodeToAdd != 5)
                nodeToAdd = 0;
        }
    } else if (event->button() == Qt::RightButton) {
        if (nowMoving < 0) {
            if (nodeToAdd == 0) {
                foreach(QGraphicsItem *item, scene->items()) {
                    double x = item->boundingRect().topLeft().x()-10,
                           y = item->boundingRect().topLeft().y()-10,
                           w = item->boundingRect().width()+20,
                           h = item->boundingRect().height()+20;
                    if (item->group() == connections && pt.x() > x &&
                            pt.y() > y && pt.x() < (x+w) && pt.y() < (y+h)) {
                        delete item;
                        saved = false;
                    }
                }
                int i = hoveredNode(event);
                if (i > -1) {
                    nodes.del(i);
                    saved = false;
                }
            } else if (nodeToAdd == 5) {
                double x = lastPoint.x(),
                       y = lastPoint.y();
                switch (lineDir) {
                case 0:
                    connections->addToGroup(scene->addLine(x-5, y+20, x, y, QPen(Qt::black, 2)));
                    connections->addToGroup(scene->addLine(x+5, y+20, x, y, QPen(Qt::black, 2)));
                    break;
                case 1:
                    connections->addToGroup(scene->addLine(x-20, y-5, x, y, QPen(Qt::black, 2)));
                    connections->addToGroup(scene->addLine(x-20, y+5, x, y, QPen(Qt::black, 2)));
                    break;
                case 2:
                    connections->addToGroup(scene->addLine(x-5, y-20, x, y, QPen(Qt::black, 2)));
                    connections->addToGroup(scene->addLine(x+5, y-20, x, y, QPen(Qt::black, 2)));
                    break;
                case 3:
                    connections->addToGroup(scene->addLine(x+20, y-5, x, y, QPen(Qt::black, 2)));
                    connections->addToGroup(scene->addLine(x+20, y+5, x, y, QPen(Qt::black, 2)));
                    break;
                }
                _line = false;
                saved = false;
            }
            nodeToAdd = 0;
        } else {
            nodes[nowMoving].get()->setPos(lastPoint.x(), lastPoint.y());
            nodes[nowMoving].get()->setText(tmp.get()->getText());
            nowMoving = -1;
            tmp.reset();
        }
    }
    event->ignore();
}

void GraphicsView::wheelEvent(QWheelEvent *event)
{
    if(event->angleDelta().y() > 0)
      scale(1.25, 1.25);
    else
      scale(0.8, 0.8);
}

void GraphicsView::mouseMoveEvent(QMouseEvent *event)
{
    if (_pan) {
        horizontalScrollBar()->setValue(horizontalScrollBar()->value() - (event->x() - _panStartX));
        verticalScrollBar()->setValue(verticalScrollBar()->value() - (event->y() - _panStartY));
        _panStartX = event->x();
        _panStartY = event->y();
        event->accept();
        return;
    } else if (nodeToAdd == 0 && nowMoving < 0) {
        int hov = hoveredNode(event);
        for (int j = 0; j < (int)nodes.getSize(); ++j) {
            if (j == hov) {
                nodes[j].get()->setBrushColor(QColor("#D6F9FD"));
                break;
            } else {
                nodes[j].get()->setBrushColor(Qt::white);
            }
        }
    } else if (nodeToAdd != 5) {
        QPointF pt = mapToScene(event->pos());
        snapToGrid(&pt);
        tmp.get()->setPos(pt.x(), pt.y());
    }
    event->ignore();
}

void GraphicsView::slotAlarmTimer()
{
    //qDebug() << "node " << nodeToAdd << " line " << _line << " move " << nowMoving;
    foreach(QGraphicsItem *item, scene->items()) {
        if (item != grid && item->group() != grid &&
                item != connections && item->group() != connections)
            delete item;
    }
    for (size_t i = 0; i < nodes.getSize(); ++i) {
        nodes[i].get()->draw(scene);
    }
    if ((nodeToAdd > 0 && nodeToAdd < 5) || nowMoving > -1)
        tmp->draw(scene);
}

void GraphicsView::addGrid()
{
    QPointF bl = scene->sceneRect().bottomLeft(),
            tr = scene->sceneRect().topRight();
    for (double x = bl.x(); x <= tr.x(); x += 25) {
        grid->addToGroup(scene->addLine(x, bl.y(), x, tr.y(), QPen(Qt::lightGray)));
    }
    for (double y = tr.y(); y <= bl.y(); y += 25) {
        grid->addToGroup(scene->addLine(bl.x(), y, tr.x(), y, QPen(Qt::lightGray)));
    }
}

void GraphicsView::removeGrid()
{
    foreach(QGraphicsItem *item, scene->items()) {
        if (item->group() == grid)
            delete item;
    }
}

void GraphicsView::snapToGrid(QPointF *pt) const
{
    double grid = 25;
    pt->setX(round(pt->x()/grid)*grid);
    pt->setY(round(pt->y()/grid)*grid);
}

int GraphicsView::hoveredNode(QMouseEvent *event) const
{
    QPointF pt = mapToScene(event->pos());
    for (size_t i = 0; i < nodes.getSize(); ++i) {
        if (nodes[i].get()->hover(pt.x(), pt.y()))
            return i;
    }
    return -1;
}

void GraphicsView::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        int i = hoveredNode(event);
        TextDialog td;
        if (td.exec() == QDialog::Accepted) {
            QString text = td.getText();
            nodes[i].get()->setText(text);
            saved = false;
        }
    }
}

