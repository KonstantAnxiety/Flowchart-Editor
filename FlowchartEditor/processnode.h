#ifndef PROCESSNODE_H
#define PROCESSNODE_H

#include "node.h"

class ProcessNode: public Node
{
public:
    ProcessNode();
    ProcessNode(double cx_, double cy_, double w_ = 100, double h_ = 50)
       : Node(cx_, cy_) { m_w = w_; m_h = h_; brush = QBrush(Qt::white); };
    virtual void setBrushColor(QColor c) override;
    virtual void setText(const QString &text) override;
    virtual const QString &getText() const override { return this->text; }
    virtual void draw(QGraphicsScene *scene) override;
    virtual void getBrush() const override;
    virtual bool hover(double x, double y) const override;

    virtual void write(std::ostream& out) override {
        out << "P" << cx() << ' ' << cy() << ' ' << text.toStdString() << '\n';
    }

    friend std::ostream& operator<<(std::ostream& out, const ProcessNode &obj) {
        out << "P" << obj.cx() << ' ' << obj.cy() << ' ' << obj.m_w << ' ' << obj.m_h << ' ';
        return out;
    }

private:
    double m_w{100}, m_h{50};
    QBrush brush;
    QString text;
};

#endif // PROCESSNODE_H
