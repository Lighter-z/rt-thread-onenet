#ifndef MYGAUGE2_H
#define MYGAUGE2_H

#define LONG1 10
#define OK1 7
#define SHORT1 5
#define SPACE1 3
#define ANGLE1 10

#include <QWidget>
#include <QtGui>

class myGauge2 : public QWidget
{
    Q_OBJECT
public:
    explicit myGauge2(QWidget *parent = 0);

public slots:
    void setValue(qreal value);
    void setAnimating(bool enable)
    {
        m_bAnimating=enable;
        update();
    }

public:
    bool isAnimating() const
    {
        return m_bAnimating;
    }

    int scale_value_;//最小刻度值

protected:
    void paintEvent(QPaintEvent *);

    QSize sizeHint() const
    {
        return QSize(300,300);
    }

    QSize minimumSizeHint() const
    {
        return QSize(200,200);
    }

private:
    void drawOuterCircle(QPainter* painter);//外圆
    void drawInnerCircle(QPainter* painter);//内圆
    void drawMark(QPainter* painter);//刻度值
    void drawCoverCircle(QPainter* painter);
    void drawColorPie(QPainter* painter);
    void drawIndicator(QPainter* painter);//指针
    void drawCoverBall(QPainter* painter);
    void resetVariables(QPainter* painter);
    void drawTextRect(QPainter* painter);//文本

private:
    qreal m_outerRadius;//外径
    qreal m_innerRadius;//内径
    qreal m_coverCircleRadius;//覆盖 园 半径
    qreal m_colorCircleRadius;
    qreal m_coverBallRadius;//覆盖 球 半径
    QPointF m_center;//中心
    QRectF m_colorCircleRect;//颜色 矩形  颜色环
    qreal m_value;
    QTimer* updateTimer;
    bool m_bReverse;//相反
    qreal m_currentValue;//当前值
    QTimer* singleTimer;
    bool m_bAnimating;

private:
    void initVariables();

private slots:
    void UpdateGraph();
};

#endif // MYGAUGE2_H
