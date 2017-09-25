#include "mainwindow.h"
#include "ui_mainwindow.h"
#define MAX_GRAPH_INDEX 1000
#include <QPainter>
#include <QSerialPort>
QSerialPort serial;
int freqencyMes;
int mesCount = 0;
int gyroX[MAX_GRAPH_INDEX],gyroY[MAX_GRAPH_INDEX],gyroZ[MAX_GRAPH_INDEX];
int graphIndex = 0;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    scaleValue =1;
    freqencyMes = startTimer(1000);

}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::timerEvent(QTimerEvent *event)
{
    ui->labelFreq->setText(QString::fromUtf8("Tần số đo:")+QString::number(mesCount));
    mesCount = 0;
}

void MainWindow::on_pushButtonStart_clicked()
{
    serial.setPortName("COM12");
    serial.setBaudRate(500000);
    serial.setDataBits(QSerialPort::Data8);
    serial.setParity(QSerialPort::NoParity);
    serial.setStopBits(QSerialPort::OneStop);
    serial.open(QIODevice::ReadWrite);
    if (serial.isOpen() && serial.isWritable())
    {
        connect(&serial,SIGNAL(readyRead()),this,SLOT(processSerialData()));
    }
}

void MainWindow::paintEvent(QPaintEvent *event)
{

    QPainter p(this);
    p.fillRect(this->rect(),Qt::black);
    //draw x graph
    p.setPen(QPen(QColor(255,0,0,150)));
    int zeroY = height()/2;
    for(int i = 1;i<MAX_GRAPH_INDEX;i++)
    {
        p.drawLine((i-1),gyroX[i-1]/scaleValue+zeroY,i,gyroX[i]/scaleValue+zeroY);
    }
    //draw y graph
    p.setPen(QPen(QColor(0,255,0,150)));
    for(int i = 1;i<MAX_GRAPH_INDEX;i++)
    {
        p.drawLine((i-1),gyroY[i-1]/scaleValue+zeroY,i,gyroY[i]/scaleValue+zeroY);
    }
    //draw z graph
    p.setPen(QPen(QColor(0,0,255,150)));
    for(int i = 1;i<MAX_GRAPH_INDEX;i++)
    {
        p.drawLine((i-1),gyroZ[i-1]/scaleValue+zeroY,i,gyroZ[i]/scaleValue+zeroY);
    }
    //draw grid
    p.setPen(QPen(QColor(255,255,255,150),3));
    p.drawLine(0,zeroY,MAX_GRAPH_INDEX,zeroY);
    p.setPen(QPen(QColor(255,255,255,150),1));
    for(int i=-10;i<=10;i++)
    {
        p.drawLine(0,zeroY+i*100,MAX_GRAPH_INDEX,zeroY+i*100);
    }
}
int gx=0,gy=0,gz=0;
void MainWindow::processSerialData()
{
    QByteArray data = serial.readAll();
    unsigned char* pData = (unsigned char *)data.data();

    int i = data.size()-1;
    if(i<7)return;
    while(i>=7)
    {
        if(pData[i]==0xCC&&pData[i-7]==0xAA)
        {
            graphIndex++;
            if(isIntegrate)
            {
                if(graphIndex>=MAX_GRAPH_INDEX)graphIndex = 0;
                //read x
                int value=  (pData[i-6]<<8)|pData[i-5];
                if(value>>15)value = value-0xFFFF;
                gx+=value;
                gyroX[graphIndex] =gx;
                //read y
                value =  (pData[i-4]<<8)|pData[i-3];
                if(value>>15)value = value-0xFFFF;
                gy+=value;
                gyroY[graphIndex] =gy;
                //read z
                value =  (pData[i-2]<<8)|pData[i-1];
                if(value>>15)value = value-0xFFFF;
                gz+=value;
                gyroZ[graphIndex] =gz;
            }
            else
            {
                if(graphIndex>=MAX_GRAPH_INDEX)graphIndex = 0;

                gx =  (pData[i-6]<<8)|pData[i-5];
                if(gx>>15)gx = gx-0xFFFF;
                gyroX[graphIndex] =gx;
                gy =  (pData[i-4]<<8)|pData[i-3];
                if(gy>>15)gy = gy-0xFFFF;
                gyroY[graphIndex] =gy;
                gz =  (pData[i-2]<<8)|pData[i-1];
                if(gz>>15)gz = gz-0xFFFF;
                gyroZ[graphIndex] =gz;
            }
            update();
            mesCount++;
            i-=6;
        }
        i--;
    }
    update();
}

void MainWindow::on_horizontalSliderScaleValue_valueChanged(int value)
{
    scaleValue = value;
}

void MainWindow::on_pushButtonIntegration_toggled(bool checked)
{
    isIntegrate = checked;
}
