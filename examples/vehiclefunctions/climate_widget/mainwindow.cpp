/************************************************************************************************
 * Copyright (c) 2012-2015 Pelagicore AG. All rights reserved.
 *
 * This software, including documentation, is protected by copyright controlled by Pelagicore AG.
 * All rights reserved. Copying, including reproducing, storing, adapting or translating, any or
 * all of this material requires prior written consent of Pelagicore AG Corporation. This material
 * also contains confidential information which may not be disclosed to others without the prior
 * written consent of Pelagicore AG.
 ************************************************************************************************/

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QSpinBox>
#include <QButtonGroup>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_radioButtonGroup(new QButtonGroup(this)),
    m_climateControl(0)
{
    ui->setupUi(this);

    m_radioButtonGroup->addButton(ui->rb_BiLevel);
    m_radioButtonGroup->addButton(ui->rb_DefrostFloor);
    m_radioButtonGroup->addButton(ui->rb_FloorDuct);
    m_radioButtonGroup->addButton(ui->rb_FloorPanel);

    m_climateControl = new QtIVIClimateControl(this);
    m_climateControl->startAutoDiscovery();

    if (!m_climateControl->isValid())
        QMessageBox::critical(this, "Auto Discovery Failed !", "No Climate Backend available");

    setupUI();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupUI()
{
    //Air Flow Direction
    setupFlowDirectionRadioButtons(m_climateControl->airflowDirection());
    connect(m_radioButtonGroup, static_cast<void (QButtonGroup::*)(QAbstractButton *, bool)>(&QButtonGroup::buttonToggled), this, &MainWindow::onFlowDirectionButtonToggled);
    connect(m_climateControl, &QtIVIClimateControl::airflowDirectionChanged, this, &MainWindow::setupFlowDirectionRadioButtons);

    //Air Condition
    ui->cb_airCondition->setChecked(m_climateControl->isAirConditioningEnabled());
    connect(m_climateControl, &QtIVIClimateControl::airConditioningEnabledChanged, ui->cb_airCondition, &QCheckBox::setChecked);
    connect(ui->cb_airCondition, &QCheckBox::clicked, m_climateControl, &QtIVIClimateControl::setAirConditioningEnabled);

    //Air Recirculation
    ui->cb_airRecirculation->setChecked(m_climateControl->isAirRecirculationEnabled());
    connect(m_climateControl, &QtIVIClimateControl::airRecirculationEnabledChanged, ui->cb_airRecirculation, &QCheckBox::setChecked);
    connect(ui->cb_airRecirculation, &QCheckBox::clicked, m_climateControl, &QtIVIClimateControl::setAirRecirculationEnabled);

    //Heater
    ui->cb_heater->setChecked(m_climateControl->isHeaterEnabled());
    connect(m_climateControl, &QtIVIClimateControl::heaterEnabledChanged, ui->cb_heater, &QCheckBox::setChecked);
    connect(ui->cb_heater, &QCheckBox::clicked, m_climateControl, &QtIVIClimateControl::setHeaterEnabled);

    //Heater
    ui->cb_steeringWheelHeater->setChecked(m_climateControl->isSteeringWheelHeaterEnabled());
    connect(m_climateControl, &QtIVIClimateControl::steeringWheelHeaterEnabledChanged, ui->cb_steeringWheelHeater, &QCheckBox::setChecked);
    connect(ui->cb_steeringWheelHeater, &QCheckBox::clicked, m_climateControl, &QtIVIClimateControl::setSteeringWheelHeaterEnabled);

    //Fan Speed
    ui->sb_fanSpeed->setValue(m_climateControl->fanSpeedLevel());
    connect(m_climateControl, &QtIVIClimateControl::fanSpeedLevelChanged, ui->sb_fanSpeed, &QSpinBox::setValue);
    connect(ui->sb_fanSpeed, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), m_climateControl, &QtIVIClimateControl::setFanSpeedLevel);
}

void MainWindow::setupFlowDirectionRadioButtons(QtIVIClimateControl::AirflowDirection direction)
{
    QAbstractButton* button = ui->rb_BiLevel;

    if (direction == QtIVIClimateControl::BiLevel)
        button = ui->rb_BiLevel;
    else if (direction == QtIVIClimateControl::DefrostFloor)
        button = ui->rb_DefrostFloor;
    else if (direction == QtIVIClimateControl::FloorDuct)
        button = ui->rb_FloorDuct;
    else if (direction == QtIVIClimateControl::FloorPanel)
        button = ui->rb_FloorPanel;

    button->setChecked(true);
}

void MainWindow::onFlowDirectionButtonToggled(QAbstractButton *button, bool checked)
{
    if (!checked)
        return;

    QtIVIClimateControl::AirflowDirection direction = QtIVIClimateControl::BiLevel;

    if (button == ui->rb_BiLevel)
        direction = QtIVIClimateControl::BiLevel;
    else if (button == ui->rb_DefrostFloor)
        direction = QtIVIClimateControl::DefrostFloor;
    else if (button == ui->rb_FloorDuct)
        direction = QtIVIClimateControl::FloorDuct;
    else if (button == ui->rb_FloorPanel)
        direction = QtIVIClimateControl::FloorPanel;

    m_climateControl->setAirflowDirection(direction);
}
