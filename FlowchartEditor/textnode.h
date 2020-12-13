#ifndef TEXTNODE_H
#define TEXTNODE_H

#include "node.h"

class TextNode: public Node
{
public:
    TextNode();
    TextNode(double cx_, double cy_, double w_ = 50, double h_ = 25)
       : Node(cx_, cy_) { m_w = w_; m_h = h_; brush = QBrush(Qt::white); };
    virtual void draw(QGraphicsScene *scene) override;
    virtual void setBrushColor(QColor c) override;
    virtual void setText(const QString &text) override;
    virtual const QString &getText() const override { return this->text; }
    virtual void getBrush() const override;
    virtual bool hover(double x, double y) const override;

    virtual void write(std::ostream& out) override {
        out << "t" << cx() << ' ' << cy() << ' ' << text.toStdString() << '\n';
    }

    friend std::ostream& operator<<(std::ostream& out, const TextNode &obj) {
        out << "t" << obj.cx() << ' ' << obj.cy() << ' ' << obj.m_w << ' ' << obj.m_h << ' ';
        return out;
    }

private:
    double m_w{50}, m_h{25};
    QBrush brush;
    QString text;
};

#endif // TEXTNODE_H
