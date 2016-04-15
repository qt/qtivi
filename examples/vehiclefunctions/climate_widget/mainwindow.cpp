/****************************************************************************
**
** Copyright (C) 2016 Pelagicore AG
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtIVI module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD-QTAS$
** Commercial License Usage
** Licensees holding valid commercial Qt Automotive Suite licenses may use
** this file in accordance with the commercial license agreement provided
** with the Software or, alternatively, in accordance with the terms
** contained in a written agreement between you and The Qt Company.  For
** licensing terms and conditions see https://www.qt.io/terms-conditions.
** For further information use the contact form at https://www.qt.io/contact-us.
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
**   * Neither the name of The Qt Company Ltd nor the names of its
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
    m_buttonGroup(new QButtonGroup(this)),
    m_climateControl(0)
{
    ui->setupUi(this);

    m_buttonGroup->setExclusive(false);
    m_buttonGroup->addButton(ui->cb_windshield);
    m_buttonGroup->addButton(ui->cb_dashboard);
    m_buttonGroup->addButton(ui->cb_floor);

//![1]
    m_climateControl = new QtIVIClimateControl(QString(), this);
    m_climateControl->startAutoDiscovery();

    if (!m_climateControl->isValid())
        QMessageBox::critical(this, tr("Auto Discovery Failed !"), tr("No Climate Backend available"));
//![1]

//![2]
    //Air Flow Direction
    setupFlowDirectionRadioButtons(m_climateControl->airflowDirections());
    setupFlowDirectionAttribute(m_climateControl->airflowDirectionsAttribute());
    connect(m_buttonGroup, static_cast<void (QButtonGroup::*)(QAbstractButton *, bool)>(&QButtonGroup::buttonToggled),
            this, &MainWindow::onFlowDirectionButtonToggled);

    connect(m_climateControl, &QtIVIClimateControl::airflowDirectionsChanged,
            this, &MainWindow::setupFlowDirectionRadioButtons);
    connect(m_climateControl, &QtIVIClimateControl::airflowDirectionsAttributeChanged,
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
void MainWindow::setupFlowDirectionRadioButtons(QtIVIClimateControl::AirflowDirections direction)
{
    ui->cb_windshield->setChecked(direction.testFlag(QtIVIClimateControl::Windshield));
    ui->cb_dashboard->setChecked(direction.testFlag(QtIVIClimateControl::Dashboard));
    ui->cb_floor->setChecked(direction.testFlag(QtIVIClimateControl::Floor));
}

void MainWindow::setupFlowDirectionAttribute(const QtIVIPropertyAttribute<QtIVIClimateControl::AirflowDirections> & attribute)
{
    ui->cb_windshield->setEnabled(attribute.availableValues().contains(QtIVIClimateControl::Windshield));
    ui->cb_dashboard->setEnabled(attribute.availableValues().contains(QtIVIClimateControl::Dashboard));
    ui->cb_floor->setEnabled(attribute.availableValues().contains(QtIVIClimateControl::Floor));
}

void MainWindow::onFlowDirectionButtonToggled(QAbstractButton *button, bool checked)
{
    Q_UNUSED(button)
    Q_UNUSED(checked)

    QtIVIClimateControl::AirflowDirections direction;

    if (ui->cb_windshield->isChecked())
        direction |= QtIVIClimateControl::Windshield;
    if (ui->cb_dashboard->isChecked())
        direction |= QtIVIClimateControl::Dashboard;
    if (ui->cb_floor->isChecked())
        direction |= QtIVIClimateControl::Floor;

    m_climateControl->setAirflowDirections(direction);
}
//![3]
