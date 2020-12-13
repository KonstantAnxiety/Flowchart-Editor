#include "textnode.h"
#include <QDebug>
#include <QGraphicsItem>
#include <math.h>

TextNode::TextNode()
{

}

void TextNode::draw(QGraphicsScene *scene)
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
    scene->addRect(cx()-m_w/2, cy()-m_h/2, m_w, m_h, QPen(Qt::black, 1), brush)->setZValue(6);
}

void TextNode::setBrushColor(QColor c)
{
    brush.setColor(c);
}

void TextNode::setText(const QString &text)
{
    this->text = text;
}

void TextNode::getBrush() const
{
    qDebug() << brush.color();
}

bool TextNode::hover(double x, double y) const
{
    return (x > cx() - m_w/2 &&
            x < cx() + m_w/2 &&
            y > cy() - m_h/2 &&
            y < cy() + m_h/2);
}
