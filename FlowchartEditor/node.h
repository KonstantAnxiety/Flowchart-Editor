#ifndef NODE_H
#define NODE_H

#include <QGraphicsScene>
#include <QDebug>

class Node
{
public:
    Node();
    Node(double cx_, double cy_) : m_cx{cx_}, m_cy{cy_} { };
    double cx() const { return m_cx; };
    double cy() const { return m_cy; };
    virtual const QString &getText() const = 0;
    virtual void setPos(double cx, double cy) { m_cx = cx; m_cy = cy; };
    virtual void draw(QGraphicsScene *scene) = 0;
    virtual void setBrushColor(QColor c) = 0;
    virtual void setText(const QString &label) = 0;
    virtual void getBrush() const = 0;
    virtual bool hover(double x, double y) const = 0;

    virtual void write(std::ostream& out) {
        out << 'N' << cx() << ' ' << cy();
    }

    friend std::ostream& operator<<(std::ostream& out, const Node &obj) {
        out << 'N' << obj.cx() << ' ' << obj.cy();
        return out;
    }

private:
    double m_cx{0.0}, m_cy{0.0};
    QBrush brush;
    QString text;
};

#endif // NODE_H
