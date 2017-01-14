#include <QDebug>
#include "DisplayFrame.h"

void QColorRect::mousePressEvent(QGraphicsSceneMouseEvent*) {
    if (m_icon) undo();
        else mark();
    auto p_master = static_cast<DisplayFrame*>(m_master);
    p_master->reg(m_icon, *this);
};

DisplayFrame::DisplayFrame(qint64 sideNum, QWidget* parent) : QWidget(parent) {
    m_board = new Board(sideNum, sideNum, 328);
    m_cur = nullptr;
    m_rects.resize(sideNum);
    m_score = 0;
    for (auto& v : m_rects) v.resize(sideNum);
    m_sideNum = sideNum;
    
    qint64 sideLength = (PIXEL + BORDER) * sideNum - BORDER;
    QSize windowSize(sideLength, sideLength);
    this->setMinimumSize(windowSize);
};

void DisplayFrame::ready(QGraphicsScene& scene) {
    for (qint64 x = 0; x < m_sideNum; ++x) for (qint64 y = 0; y < m_sideNum; ++y) {
        QRectF rectf(x * (PIXEL + BORDER), (m_sideNum - 1 - y) * (PIXEL + BORDER), PIXEL, PIXEL);
        QPoint point(x, y);
        QColorRect* rectc = new QColorRect(rectf, point, this);
        m_rects[x][y] = rectc;
        scene.addItem(rectc);
    }
    draw();
};

void DisplayFrame::draw() {
    for (qint64 x = 0; x < m_sideNum; ++x) for (qint64 y = 0; y < m_sideNum; ++y) {
        QColor color(Color[static_cast<int>(m_board->at(x, y))]);
        m_rects[x][y]->setBrush(QBrush(color));
    }
    this->setWindowTitle(QString("Score: %1").arg(m_score));
};

void DisplayFrame::reg(bool select, QColorRect& rect) {
    if (select) {
        if (m_cur) {
            QPoint pcur = m_cur->point();
            QPoint from = rect.point();
            QPoint diff = pcur - from;
            m_cur->undo();
            rect.undo();
            if (diff.manhattanLength() == 1) {
                int score = m_board->scoreIt(pcur.x(), pcur.y(), from.x(), from.y());
                if (score) {
                    m_score += score;
                    draw();
                }
                m_cur = nullptr;
            } else m_cur = &rect;
        } else m_cur = &rect;
    } else m_cur = nullptr;
}

QColorRect::QColorRect(
    const QRectF& rect,
    const QPoint& point,
    void* master,
    QGraphicsItem* parent
) : QGraphicsRectItem(rect, parent) {
    this->m_icon = nullptr;
    this->m_rect = rect;
    this->m_point = point;
    this->m_master = master;
};

void QColorRect::mark() {
    QPointF p = this->m_rect.topLeft();
    this->m_icon = new QGraphicsEllipseItem(
            p.x() + PIXEL * 3 / 8, 
            p.y() + PIXEL * 3 / 8, 
            PIXEL / 4, 
            PIXEL / 4);
    this->m_icon->setBrush(BLACK);
    this->scene()->addItem(this->m_icon);
};

void QColorRect::undo() {
    this->scene()->removeItem(this->m_icon);
    this->m_icon = nullptr;
};


