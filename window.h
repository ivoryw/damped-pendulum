#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QSlider>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QtCharts>
#include <cmath>

class phaseTab : public QWidget
{
    Q_OBJECT
public:
    explicit phaseTab(QWidget *parent=0);
private slots:
    void setValueQ(int);
    void setValueA(int);
    void setValuew(int);

private:
    void createPhaseInputBox();
    void createPhaseSpaceChart();
    void updatePhaseSpace();

    QGroupBox *phaseInputBox;
    QLabel *qualVal, *AVal, *wVal;
    QSlider *qualSlider,*wSlider, *ASlider;
    QChartView *chartView;
    QChart *phaseChart;
    QValueAxis *axisX, *axisY;
    QScatterSeries *phaseSeries;
};

class poincareTab : public QWidget{
    Q_OBJECT
public:
    explicit poincareTab(QWidget *parent=0);
private slots:
   void pressed();
   void setValuew(int);
   void setValueA(int);
private:
    void createPoincareChart();
    void createPoincareInputBox();
    void updatePoincareChart();

    QGroupBox *poincareInputBox;
    QLabel *wVal, *AVal;
    QSlider *wSlider, *ASlider;
    QChartView *chartView;
    QChart *poincareChart;
    QValueAxis *axisX, *axisY;
    QScatterSeries *poincareSeries;
};

class window : public QWidget
{
    Q_OBJECT
public:
    explicit window(QWidget *parent = 0);
    
private:
    QHBoxLayout *mainLayout;
    QTabWidget *tabWidget;
};

#endif // WINDOW_H
