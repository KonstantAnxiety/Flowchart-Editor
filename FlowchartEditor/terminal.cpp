#include "terminal.h"
#include <QPainterPath>
#include <QDebug>
#include <QGraphicsPathItem>
#include <math.h>

Terminal::Terminal()
{

}

void Terminal::setBrushColor(QColor c)
{
    brush.setColor(c);
}

void Terminal::setText(const QString &text)
{
    this->text = text;
}

void Terminal::draw(QGraphicsScene *scene)
{
    QGraphicsTextItem *io = new QGraphicsTextItem;
    io->setFont(QFont("Courier New", 8));
    io->setPlainText(text);
    double x = cx() - io->boundingRect().width()*.5;
    double y = cy() - io->boundingRect().height()*.5;
    io->setPos(x, y);
    io->setZValue(7);
    scene->addItem(io);
    if (io->boundingRect().width() > m_w)
        m_w = round((io->boundingRect().width() + 50)/50)*50;
    QPainterPath *arc = new QPainterPath();
    arc->addRoundedRect(cx()-m_w/2, cy()-m_h/2, m_w, m_h, m_h/2, m_h/2);
    scene->addPath(*arc, QPen(Qt::black, 2), brush)->setZValue(6);
}

void Terminal::getBrush() const
{
    qDebug() << brush.color();
}

bool Terminal::hover(double x, double y) const
{
    return (x > cx() - m_w/2 &&
            x < cx() + m_w/2 &&
            y > cy() - m_h/2 &&
            y < cy() + m_h/2);
}
