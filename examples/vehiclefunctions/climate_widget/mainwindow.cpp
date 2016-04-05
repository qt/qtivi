/****************************************************************************
**
** Copyright (C) 2016 Pelagicore AG
** Contact: http://www.qt.io/ or http://www.pelagicore.com/
**
** This file is part of the QtIVI module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD-PELAGICORE$
** Commercial License Usage
** Licensees holding valid commercial Qt IVI licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Pelagicore. For licensing terms
** and conditions, contact us at http://www.pelagicore.com.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Pelagicore AG nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
** SPDX-License-Identifier: BSD-3-Clause
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
    m_climateControl = new QtIVIClimateControl(QString(), this);
    m_climateControl->startAutoDiscovery();

    if (!m_climateControl->isValid())
        QMessageBox::critical(this, tr("Auto Discovery Failed !"), tr("No Climate Backend available"));
//![1]

//![2]
    //Air Flow Direction
    setupFlowDirectionRadioButtons(m_climateControl->airflowDirection());
    setupFlowDirectionAttribute(m_climateControl->airflowDirectionAttribute());
    connect(m_radioButtonGroup, static_cast<void (QButtonGroup::*)(QAbstractButton *, bool)>(&QButtonGroup::buttonToggled),
            this, &MainWindow::onFlowDirectionButtonToggled);
    connect(m_climateControl, &QtIVIClimateControl::airflowDirectionChanged,
            this, &MainWindow::setupFlowDirectionRadioButtons);
    connect(m_climateControl, &QtIVIClimateControl::airflowDirectionAttributeChanged,
            this, &MainWindow::setupFlowDirectionAttribute);

    //Air Condition
    ui->cb_airCondition->setChecked(m_climateControl->isAirConditioningEnabled());
    ui->cb_airCondition->setEnabled(m_climateControl->airConditioningAttribute().isAvailable());
    connect(m_climateControl, &QtIVIClimateControl::airConditioningEnabledChanged,
            ui->cb_airCondition, &QCheckBox::setChecked);
    connect(m_climateControl, &QtIVIClimateControl::airConditioningAttributeChanged,
            this, &MainWindow::onAirConditioningAttributeChanged);
    connect(ui->cb_airCondition, &QCheckBox::clicked,
            m_climateControl, &QtIVIClimateControl::setAirConditioningEnabled);

    //Air Recirculation
    ui->cb_airRecirculation->setChecked(m_climateControl->isAirRecirculationEnabled());
    ui->cb_airRecirculation->setEnabled(m_climateControl->airRecirculationAttribute().isAvailable());
    connect(m_climateControl, &QtIVIClimateControl::airRecirculationEnabledChanged,
            ui->cb_airRecirculation, &QCheckBox::setChecked);
    connect(m_climateControl, &QtIVIClimateControl::airRecirculationAttributeChanged,
            this, &MainWindow::onAirRecirculationAttributeChanged);
    connect(ui->cb_airRecirculation, &QCheckBox::clicked,
            m_climateControl, &QtIVIClimateControl::setAirRecirculationEnabled);

    //Heater
    ui->cb_heater->setChecked(m_climateControl->isHeaterEnabled());
    ui->cb_heater->setEnabled(m_climateControl->heaterAttribute().isAvailable());
    connect(m_climateControl, &QtIVIClimateControl::heaterEnabledChanged,
            ui->cb_heater, &QCheckBox::setChecked);
    connect(m_climateControl, &QtIVIClimateControl::heaterAttributeChanged,
            this, &MainWindow::onHeaterAttributeChanged);
    connect(ui->cb_heater, &QCheckBox::clicked,
            m_climateControl, &QtIVIClimateControl::setHeaterEnabled);
}
//![2]

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onAirRecirculationAttributeChanged(const QtIVIPropertyAttribute<bool> & attribute)
{
    ui->cb_airRecirculation->setEnabled(attribute.isAvailable());
}

void MainWindow::onHeaterAttributeChanged(const QtIVIPropertyAttribute<bool> & attribute)
{
    ui->cb_heater->setEnabled(attribute.isAvailable());
}

void MainWindow::onAirConditioningAttributeChanged(const QtIVIPropertyAttribute<bool> & attribute)
{
    ui->cb_airCondition->setEnabled(attribute.isAvailable());
}

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

void MainWindow::setupFlowDirectionAttribute(const QtIVIPropertyAttribute<QtIVIClimateControl::AirflowDirection> & attribute)
{
    ui->rb_BiLevel->setEnabled(attribute.availableValues().contains(QtIVIClimateControl::BiLevel));
    ui->rb_DefrostFloor->setEnabled(attribute.availableValues().contains(QtIVIClimateControl::DefrostFloor));
    ui->rb_FloorDuct->setEnabled(attribute.availableValues().contains(QtIVIClimateControl::FloorDuct));
    ui->rb_FloorPanel->setEnabled(attribute.availableValues().contains(QtIVIClimateControl::FloorPanel));
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
