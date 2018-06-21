#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <QMessageBox>
#include <QTimer>
#include <QDesktopWidget>
#include <QScreen>
#include <QMetaEnum>
#include "qcustomplot.h"

int segundero=0;
QVector<double> xx(51), yy(51), zz(51);
int TiempoMuestreo = 100; //ms

void MainWindow::fTimer(){
    QCustomPlot *ObjetoGrafica = ui->Grafica;
    segundero++;
    if(segundero>50){ segundero=0;//limpia buffer

     for(int i=0; i<=50; i++){
         xx[i] = 0;
         yy[i] = 0;
         zz[i] = 0;
     }

    }
    ui->lcdNumber_3->display(segundero);
    if(segundero == 0) xx[segundero] = segundero;
    else if(segundero > 0){
      xx[segundero] = segundero*((float)TiempoMuestreo/1000);
}
  if(hw_is_available){
      if(hw->isWritable()){
          hw->write("a");
          if(hw->isReadable()){
              QByteArray datosTotales = hw->read(4);//se pueden leer 2 datos de 16 bytes o de 8 bytes
              QByteArray canalA = datosTotales.mid(0,2); //, partida y longitud
              QByteArray canalB = datosTotales.mid(2,2);
              int ADC_A = canalA.toHex().toInt(0,16);//un entero de 16 bits
              int ADC_B = canalB.toHex().toInt(0,16);
              float voltajeA = (5*(float)ADC_A/1023);
              float voltajeB = (5*(float)ADC_B/1023);
              ui->progressBar->setValue(ADC_A);
              ui->progressBar_4->setValue(ADC_B);
              ui->lcdNumber->display(voltajeA);
              ui->lcdNumber_10->display(voltajeB);
              yy[segundero] = voltajeA;
              zz[segundero] = voltajeB;

          }
      }

      else yy[segundero] = -1;

      ObjetoGrafica->addGraph();
      ObjetoGrafica->graph(0)->setPen(QPen(Qt::green));
      ObjetoGrafica->graph(0)->setData(xx, yy);
      ObjetoGrafica->addGraph();
      ObjetoGrafica->graph(1)->setPen(QPen(Qt::red));
      ObjetoGrafica->graph(1)->setData(xx, zz);
      /*
       *   customPlot->graph(1)->setPen(QPen(Qt::red)); // line color red for second graph
            en plots, ejemplo 2, línea
        */

      ObjetoGrafica->xAxis->setLabel("Tiempo (s)");
       ObjetoGrafica->yAxis->setLabel("Voltaje (V)");
       ObjetoGrafica->xAxis->setRange(0, TiempoMuestreo*50/1000);
        ObjetoGrafica->yAxis->setRange(0, 5);
      ui->Grafica->replot();

  }
}


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //QTimer *cronometro=new QTimer(this);
     connect(cronometro, SIGNAL(timeout()), this, SLOT(fTimer()));
     cronometro->start(TiempoMuestreo);

    hw_is_available = false;
    hw_port_name = "";
    hw = new QSerialPort;
    ui->lineEdit3->setText("9600");
    ui->lineEdit1->setText(QString::number(QSerialPortInfo::availablePorts().length()));
    foreach (const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts()) {
        if(serialPortInfo.hasVendorIdentifier() && serialPortInfo.hasProductIdentifier()){
            ui->lineEdit2->setText(QString::number(serialPortInfo.vendorIdentifier()));
            ui->lineEdit5->setText(QString::number(serialPortInfo.productIdentifier()));
            hw_port_name = serialPortInfo.portName();
            hw_is_available = true;
        }
    }

    foreach (const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts())
        {
        ui->comboBox1->addItem(serialPortInfo.portName());
        }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_7_clicked()
{
    QString bauds = ui->lineEdit3->text();
    int vbauds = bauds.toInt();
    if(vbauds == 9600){
        hw ->setBaudRate(QSerialPort::Baud9600);
        ui->lineEdit4->setText(bauds);
    }
    else{
    }

    if(hw_is_available){
        hw ->setPortName(hw_port_name);
        hw->open(QIODevice::ReadWrite);
        hw->setDataBits(QSerialPort::Data8);
        hw->setParity(QSerialPort::NoParity);
        hw->setStopBits(QSerialPort::OneStop);
        hw->setFlowControl(QSerialPort::NoFlowControl);
    }
    else{
        QMessageBox::information(this,"Error","No esta disponible el puerto serial");
    }
}

void MainWindow::on_pushButton_9_clicked()
{
    hw->close();
    QMessageBox::information(this,"Fin de Comunicación","Comunicación Serial Terminada!");
    close();
}

void MainWindow::on_verticalSlider_valueChanged(int value)
{
    TiempoMuestreo = value*100;
    cronometro->start(TiempoMuestreo);
    ui->lcdNumber_2->display(TiempoMuestreo);
}
