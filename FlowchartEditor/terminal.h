#ifndef TERMINAL_H
#define TERMINAL_H

#include "node.h"

class Terminal: public Node
{
public:
    Terminal();
    Terminal(double cx_, double cy_, double w_ = 100, double h_ = 50)
       : Node(cx_, cy_) { m_w = w_; m_h = h_; brush = QBrush(Qt::white); };
    virtual void setBrushColor(QColor c) override;
    virtual void setText(const QString &text) override;
    virtual const QString &getText() const override { return this->text; }
    virtual void draw(QGraphicsScene *scene) override;
    virtual void getBrush() const override;
    virtual bool hover(double x, double y) const override;

    virtual void write(std::ostream& out) override {
        out << "T" << cx() << ' ' << cy() << ' ' << text.toStdString() << '\n';
    }

    friend std::ostream& operator<<(std::ostream& out, const Terminal &obj) {
        out << "T" << obj.cx() << ' ' << obj.cy() << ' ' << obj.m_w << ' ' << obj.m_h << ' ';
        return out;
    }

private:
    double m_w{100}, m_h{50};
    QBrush brush;
    QString text;
};

#endif // TERMINAL_H
