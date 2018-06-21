#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
 #include "qcustomplot.h"

namespace Ui {
class MainWindow;
}
class QSerialPort; //Definimos la nueva clase para el puerto serial

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_7_clicked();
    void on_pushButton_9_clicked();
    void fTimer();

    void on_verticalSlider_valueChanged(int value);

private:
    Ui::MainWindow *ui;

    QSerialPort *hw; //Creamos el objeto
    QString hw_port_name;
    bool hw_is_available;
    QTimer *cronometro=new QTimer(this);

};

#endif // MAINWINDOW_H
