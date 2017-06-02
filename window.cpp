#include "window.h"
#include "rk4.h"

#define Q_MAX 2
#define W_MAX 2
#define A_MAX 2

float A, w, Q;
float g = 9.81;
float pi = 3.1415;

float dthetadt(float t,float theta, float v){
    return v;
}
float dvdt(float t, float theta, float v){
    return -1*v/Q - sin(theta) + A*cos(w*t);
}

window::window(QWidget *parent) :
    QWidget(parent)
{
    setWindowTitle("Chaotic Pendulum");

    A = 1, Q = 1, w = 1;

    tabWidget = new QTabWidget;
    tabWidget->addTab(new phaseTab(), "Phase Space");
    tabWidget->addTab(new poincareTab(), "Poincare Section");

    mainLayout = new QHBoxLayout();
    mainLayout->addWidget(tabWidget);

    setLayout(mainLayout);

    resize(800, 500);
}

phaseTab::phaseTab(QWidget *parent)
: QWidget(parent)
{
    createPhaseInputBox();
    createPhaseSpaceChart();
   
    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addWidget(chartView);
    mainLayout->addWidget(phaseInputBox);
    setLayout(mainLayout);

    setValueQ(250);
    setValueA(250);
    setValuew(250);
    updatePhaseSpace();
}


void phaseTab::createPhaseSpaceChart(){
    axisX = new QValueAxis;
    axisX->setRange(-1,1);
    axisX->setTitleText("θ / π");
    axisX->setMinorTickCount(2);
    axisY = new QValueAxis;
    axisY->setRange(-3,3);
    axisY->setTitleText("v");
    axisY->setTickCount(13);
    axisY->setMinorTickCount(1);
    
    phaseChart = new QChart();
    phaseChart->addAxis(axisX,Qt::AlignBottom);
    phaseChart->addAxis(axisY,Qt::AlignLeft);
    phaseChart->legend()->setVisible(false);

    chartView = new QChartView(phaseChart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setMinimumWidth(500);
}

void phaseTab::createPhaseInputBox(){
    QGridLayout *layout = new QGridLayout;
    phaseInputBox = new QGroupBox();

    QLabel *qualLabel = new QLabel("Quality Factor (Q): ");
    QLabel *ALabel = new QLabel("Normalized Amp (Â): ");
    QLabel *wLabel = new QLabel("Normalized AngFreq (ῶ): ");

    AVal = new QLabel();
    wVal = new QLabel();
    qualVal = new QLabel();

    qualSlider = new QSlider(Qt::Horizontal);
    qualSlider->setRange(1, 500);
    qualSlider->setValue(250);
    ASlider = new QSlider(Qt::Horizontal);
    ASlider->setRange(1, 500);
    ASlider->setValue(250);
    wSlider = new QSlider(Qt::Horizontal);
    wSlider->setRange(1, 500);
    wSlider->setValue(250);

    connect(qualSlider, SIGNAL(valueChanged(int)), this, SLOT(setValueQ(int)));
    connect(ASlider, SIGNAL(valueChanged(int)), this, SLOT(setValueA(int)));
    connect(wSlider, SIGNAL(valueChanged(int)), this, SLOT(setValuew(int)));

    layout->addWidget(qualLabel,0,0,Qt::AlignLeft);        
    layout->addWidget(qualVal,0,0, Qt::AlignRight);
    layout->addWidget(qualSlider,1,0,Qt::AlignTop);
    layout->addWidget(ALabel,2,0, Qt::AlignLeft);
    layout->addWidget(AVal,2,0, Qt::AlignRight);
    layout->addWidget(ASlider,3,0, Qt::AlignTop);
    layout->addWidget(wLabel,4,0, Qt:: AlignLeft);
    layout->addWidget(wVal,4,0, Qt::AlignRight);
    layout->addWidget(wSlider,5,0, Qt::AlignTop);
    phaseInputBox->setLayout(layout);
    phaseInputBox->setFixedWidth(220);
}


void phaseTab::setValueQ(int value){
    Q = float(value)/500*Q_MAX;
    QString s = QString::number(Q);
    qualVal->setText(s);
    updatePhaseSpace();
}
void phaseTab::setValueA(int value){
    A = float(value)/500*A_MAX;
    QString s = QString::number(A);
    AVal->setText(s);
    updatePhaseSpace();
}
void phaseTab::setValuew(int value){
    w = float(value)/500*W_MAX;
    QString s = QString::number(w);
    wVal->setText(s);
    updatePhaseSpace();
}

void phaseTab::updatePhaseSpace(){
    int n = 1000;
    float* array = var3_rk4_fixed(dthetadt, dvdt, 0,0,0, 2*pi/(w*100),n);
    float *v_ptr, *theta_ptr;
    theta_ptr = &array[n];
    v_ptr = &array[2*n];
    
    static int phase_flag = 0;
    if(phase_flag == 1){
        delete phaseSeries;
    }
    else{
        phase_flag = 1;
    }

    phaseSeries = new QScatterSeries;
    phaseSeries->setUseOpenGL(true);
    phaseSeries->setColor(QColor("black"));
    phaseSeries->setMarkerSize(2);

    for(int i = 1; i < n; i++){
        *theta_ptr /= pi;
        if(*theta_ptr > 1)
            *theta_ptr = fmod(*theta_ptr+1,2) - 1;
        else if(*theta_ptr < -1)
            *theta_ptr = fmod(*theta_ptr-1,2) + 1;
        phaseSeries->append(*theta_ptr, *v_ptr);
        ++v_ptr;
        ++theta_ptr;
    }
    phaseChart->addSeries(phaseSeries);
    phaseSeries->attachAxis(axisX);
    phaseSeries->attachAxis(axisY);
    delete(array);
}

poincareTab::poincareTab(QWidget *parent)
: QWidget(parent)
{
    createPoincareChart();
    createPoincareInputBox();

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addWidget(chartView);
    mainLayout->addWidget(poincareInputBox);
    setLayout(mainLayout);
    poincareSeries = new QScatterSeries;
}

void poincareTab::createPoincareChart(){
    axisX = new QValueAxis;
    axisX->setRange(1,1.5);
    axisX->setTitleText("Q");
    axisX->setMinorTickCount(2);
    axisY = new QValueAxis;
    axisY->setRange(0,3);
    axisY->setTitleText("v");
    axisY->setTickCount(13);
    axisY->setMinorTickCount(1);

    poincareChart = new QChart();
    poincareChart->addAxis(axisX,Qt::AlignBottom);
    poincareChart->addAxis(axisY,Qt::AlignLeft);
    poincareChart->legend()->setVisible(false);

    chartView = new QChartView(poincareChart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setMinimumWidth(500);
}

void poincareTab::createPoincareInputBox(){
    QGridLayout *layout = new QGridLayout;
    poincareInputBox = new QGroupBox();

    QLabel *ALabel = new QLabel("Normalized Amp");
    QLabel *wLabel = new QLabel("Normalized AngFreq");

    QPushButton *goButton = new QPushButton("Go!");
    connect(goButton, SIGNAL(clicked()), this, SLOT(pressed()));

    AVal = new QLabel;
    wVal = new QLabel;
    ASlider = new QSlider(Qt::Horizontal);
    ASlider->setRange(1,500);
    ASlider->setValue(250);
    wSlider = new QSlider(Qt::Horizontal);
    wSlider->setRange(1,500);
    wSlider->setValue(250);

    connect(ASlider, SIGNAL(valueChanged(int)), this, SLOT(setValueA(int)));
    connect(wSlider, SIGNAL(valueChanged(int)), this, SLOT(setValuew(int)));

    layout->addWidget(ALabel,0,0);
    layout->addWidget(AVal,0,0,Qt::AlignRight);
    layout->addWidget(ASlider,1,0);
    layout->addWidget(wLabel,2,0);
    layout->addWidget(wVal,2,0,Qt::AlignRight);
    layout->addWidget(wSlider,3,0);
    layout->addWidget(goButton,5,0);
    poincareInputBox->setLayout(layout);
    poincareInputBox->setFixedWidth(220);
}

void poincareTab::updatePoincareChart(){
    int n = 5000;
    float *array, *v;
    static int poincare_flag = 0;
    
    if(poincare_flag == 1){
        delete poincareSeries;
    }
    else{
        poincare_flag = 1;
    }

    poincareSeries = new QScatterSeries;
    poincareSeries->setUseOpenGL(true);
    poincareSeries->setColor(QColor("black"));
    poincareSeries->setMarkerSize(2);
    for(int i = 0; i< 500; i++){
        Q = i*(0.5)/500 + 1;
        array = var3_rk4_fixed(dthetadt, dvdt, 0,0,0, 2*pi/(w*100),n);
        v = &array[2*n+300];
        for(int j = 300; j < n; j++){
            if(j%100 == 0)
                poincareSeries->append(Q, *v);
            ++v;
        }
        delete(array);
    }
    poincareChart->addSeries(poincareSeries);
    poincareSeries->attachAxis(axisX);
    poincareSeries->attachAxis(axisY);
}
void poincareTab::pressed(){
    updatePoincareChart();
}

void poincareTab::setValueA(int value){
    A = float(value)*A_MAX/500;
    QString s = QString::number(A);
    AVal->setText(s);
}

void poincareTab::setValuew(int value){
    w = float(value)*W_MAX/500;
    QString s = QString::number(w);
    wVal->setText(s);
}
