#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected slots:
    void processSerialData();
private slots:
    void on_pushButtonStart_clicked();

    void on_horizontalSliderScaleValue_valueChanged(int value);
    void timerEvent(QTimerEvent *event);
    void on_pushButtonIntegration_toggled(bool checked);

private:
    Ui::MainWindow *ui;
    bool isIntegrate;
    void paintEvent(QPaintEvent *event);
    int scaleValue;

};

#endif // MAINWINDOW_H
