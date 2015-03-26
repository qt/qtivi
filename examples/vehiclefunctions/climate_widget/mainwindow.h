#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtIVIVehicleFunctions/QtIVIClimateControl>

namespace Ui {
class MainWindow;
}

class QButtonGroup;
class QAbstractButton;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void setupUI();

private slots:
    void setupFlowDirectionRadioButtons(QtIVIClimateControl::AirflowDirection direction);
    void onFlowDirectionButtonToggled(QAbstractButton * button, bool checked);

private:

    Ui::MainWindow *ui;
    QButtonGroup *m_radioButtonGroup;
    QtIVIClimateControl* m_climateControl;
};

#endif // MAINWINDOW_H
