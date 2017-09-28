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
typedef union
{
  float value;
  unsigned char binary[4];
  }binary_float;
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
    serial.setPortName("COM6");
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
    binary_float value;
    int i = data.size()-1;
    if(i<13)return;
    while(i>=13)
    {
        if(pData[i]==0xCC&&pData[i-13]==0xAA)
        {
            i-=12;
            graphIndex++;
            int offset=i;
            if(isIntegrate)offset+=6;
            value.binary[0] = pData[offset];
            value.binary[1] = pData[offset+1];
            value.binary[2] = pData[offset+2];
            value.binary[3] = pData[offset+3];
            if(graphIndex>=MAX_GRAPH_INDEX)graphIndex = 0;
            gx=value.value*100;
            gyroX[graphIndex] =gx;
            /*
            //read x
            int value=  (pData[offset]<<8)|pData[offset+1];
            if(value>>15)value = value-0xFFFF;
            gx=value;
            gyroX[graphIndex] =gx;
            //read y
            value =  (pData[offset+2]<<8)|pData[offset+3];
            if(value>>15)value = value-0xFFFF;
            gy=value;
            gyroY[graphIndex] =gy;
            //read z
            value =  (pData[offset+4]<<8)|pData[offset+5];
            if(value>>15)value = value-0xFFFF;
            gz=value;
            gyroZ[graphIndex] =gz;
*/
            update();
            mesCount++;

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
