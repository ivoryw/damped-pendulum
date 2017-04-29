#include "window.h"
#include "rk4.h"

#define Q_MAX 2
#define W_MAX 2
#define A_MAX 2

float A, w, Q, h;
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
    createInputBox();
    createPhaseSpaceChart();

    mainBox = new QHBoxLayout();
    mainBox->addWidget(chartView);
    mainBox->addWidget(inputBox);

    setLayout(mainBox);
    resize(800, 500);

    setValueQ(250);
    setValueA(250);
    setValuew(250);
    updatePhaseSpace();
}

void window::setValueQ(int value){
    Q = float(value)/500*Q_MAX;
    QString s = QString::number(Q);
    qualVal->setText(s);
    updatePhaseSpace();
}
void window::setValueA(int value){
    A = float(value)/500*A_MAX;
    QString s = QString::number(A);
    AVal->setText(s);
    updatePhaseSpace();
}
void window::setValuew(int value){
    w = float(value)/500*W_MAX;
    QString s = QString::number(w);
    wVal->setText(s);
    updatePhaseSpace();
}

void window::createPhaseSpaceChart(){
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

void window::createInputBox(){
    QGridLayout *layout = new QGridLayout;
    inputBox = new QGroupBox();

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
    inputBox->setLayout(layout);
    inputBox->setFixedWidth(220);
}

void window::updatePhaseSpace(){
    int n = 1000;
    float* array = var3_rk4_fixed(dthetadt, dvdt, 0,0,0, 2*pi/(w*100),n);
    float *v_ptr, *theta_ptr;
    theta_ptr = &array[n];
    v_ptr = &array[2*n];

    delete phaseSeries;
    phaseSeries = new QScatterSeries;
    phaseSeries->setUseOpenGL(true);
    phaseSeries->setColor(QColor("black"));
    phaseSeries->setMarkerSize(2);

    for(int i = 0; i < n; i++){
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
