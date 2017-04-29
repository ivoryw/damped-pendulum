#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QSlider>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QtCharts>
#include <cmath>

class QSlider; 
class window : public QWidget
{
    Q_OBJECT
public:
    explicit window(QWidget *parent = 0);
private slots:
    void setValueQ(int);
    void setValueA(int);
    void setValuew(int);
private:
    void createInputBox();
    void createPhaseSpaceChart();
    void updatePhaseSpace();

    QHBoxLayout *mainBox;
    QGroupBox *inputBox;
    QLabel *qualVal, *AVal, *wVal;
    QSlider *qualSlider,*wSlider, *ASlider;
    QChartView *chartView;
    QChart *phaseChart;
    QValueAxis *axisX, *axisY;
    QScatterSeries *phaseSeries;

signals:

public slots:
};

#endif // WINDOW_H
