/************************************************************************************************
 * Copyright (c) 2012-2015 Pelagicore AG. All rights reserved.
 *
 * This software, including documentation, is protected by copyright controlled by Pelagicore AG.
 * All rights reserved. Copying, including reproducing, storing, adapting or translating, any or
 * all of this material requires prior written consent of Pelagicore AG Corporation. This material
 * also contains confidential information which may not be disclosed to others without the prior
 * written consent of Pelagicore AG.
 ************************************************************************************************/

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
