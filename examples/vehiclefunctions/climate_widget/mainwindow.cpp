/****************************************************************************
**
** Copyright (C) 2015 Pelagicore AG
** Contact: http://www.qt.io/ or http://www.pelagicore.com/
**
** This file is part of the QtIVI module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL3-PELAGICORE$
** Commercial License Usage
** Licensees holding valid commercial Qt IVI licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Pelagicore. For licensing terms
** and conditions, contact us at http://www.pelagicore.com.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPLv3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** $QT_END_LICENSE$
**
** SPDX-License-Identifier: LGPL-3.0
**
****************************************************************************/

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

//![1]
    m_climateControl = new QtIVIClimateControl(this);
    m_climateControl->startAutoDiscovery();

    if (!m_climateControl->isValid())
        QMessageBox::critical(this, "Auto Discovery Failed !", "No Climate Backend available");

    setupUI();
//![1]
}

MainWindow::~MainWindow()
{
    delete ui;
}

//![2]
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
}
//![2]

//![3]
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
//![3]
