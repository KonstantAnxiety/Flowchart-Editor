#include "decisionnode.h"
#include <QDebug>
#include <QGraphicsItem>
#include <math.h>

DecisionNode::DecisionNode()
{

}

void DecisionNode::draw(QGraphicsScene *scene)
{
    double x = cx();
    double y = cy();
    QGraphicsTextItem *io = new QGraphicsTextItem;
    io->setFont(QFont("Courier New", 8));
    io->setPlainText(text);
    double xt = cx() - io->boundingRect().width()*.5;
    double yt = cy() - io->boundingRect().height()*.5;
    io->setPos(xt, yt);
    io->setZValue(7);
    scene->addItem(io);

    if (io->boundingRect().width() > m_w)
        m_w = round((io->boundingRect().width() + 50)/50)*50;
    double dw = m_w/2;
    double dh = m_h/2;
    QVector<QPoint> points;
    points.push_back(QPoint(x-dw, y));
    points.push_back(QPoint(x, y-dh));
    points.push_back(QPoint(x+dw, y));
    points.push_back(QPoint(x, y+dh));
    scene->addPolygon(QPolygon(points), QPen(Qt::black, 2), brush)->setZValue(6);
}

void DecisionNode::setBrushColor(QColor c)
{
    brush.setColor(c);
}

void DecisionNode::setText(const QString &text)
{
    this->text = text;
}

void DecisionNode::getBrush() const
{
    qDebug() << brush.color();
}

bool DecisionNode::hover(double x, double y) const
{
    return (x > cx() - m_w/2 &&
            x < cx() + m_w/2 &&
            y > cy() - m_h/2 &&
            y < cy() + m_h/2);
}
