#include <QApplication>
#include <QWidget>
#include <QBrush>
#include <QColor>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsRectItem>

#include "MatchEngine.cpp"

const qint64 PIXEL = 48;
const qint64 BORDER = 4;
const QBrush BLACK(QColor("black"));
const char* const Color[] = {"purple", "red", "green", "blue", "yellow"};

class QColorRect : public QGraphicsRectItem {
    public:
        QColorRect( const QRectF&,
                    const QPoint&,
                    void*,
                    QGraphicsItem* parent = nullptr);
        void mark(void);
        void undo(void);
        QPoint point() { return m_point; };
    private:
        QGraphicsEllipseItem* m_icon;
        QRectF m_rect;
        QPoint m_point;
        void* m_master;
    protected:
        void mousePressEvent(QGraphicsSceneMouseEvent*);
};


class DisplayFrame : public QWidget {
    public:
        DisplayFrame(qint64, QWidget* parent = nullptr);
        void reg(bool, QColorRect&);
        void draw(void);
        void ready(QGraphicsScene&);
    private:
        Board* m_board;
        QColorRect* m_cur;
        vector<vector<QColorRect*>> m_rects;
        qint64 m_sideNum;
        qint64 m_score;
};
