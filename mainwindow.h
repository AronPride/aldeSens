/***************************************************************************
**                                                                        **
**  QCustomPlot, an easy to use, modern plotting widget for Qt            **
**  Copyright (C) 2011, 2012, 2013, 2014 Emanuel Eichhammer               **
**                                                                        **
**  This program is free software: you can redistribute it and/or modify  **
**  it under the terms of the GNU General Public License as published by  **
**  the Free Software Foundation, either version 3 of the License, or     **
**  (at your option) any later version.                                   **
**                                                                        **
**  This program is distributed in the hope that it will be useful,       **
**  but WITHOUT ANY WARRANTY; without even the implied warranty of        **
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         **
**  GNU General Public License for more details.                          **
**                                                                        **
**  You should have received a copy of the GNU General Public License     **
**  along with this program.  If not, see http://www.gnu.org/licenses/.   **
**                                                                        **
****************************************************************************
**           Author: Emanuel Eichhammer                                   **
**  Website/Contact: http://www.qcustomplot.com/                          **
**             Date: 07.04.14                                             **
**          Version: 1.2.1                                                **
****************************************************************************/


#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "qcustomplot.h" // the header file of QCustomPlot

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
    void setupWaveTypes();
    void waveType();
    void setupAldeSensGraph(QCustomPlot *customPlot);
    void setUpComPort();

private slots:
    void fillPortsInfo();
    void parseAndPlot();
    void sampleSetup();
    void mouseWheel();
    void mousePress();
    void removeSelectedGraph();
    void removeAllGraphs();
    void contextMenuRequest(QPoint pos);
    void selectionChanged();
    void sampASPressed();
    void sampPAPressed();
    void sampCVPressed();
    void resetSelected();
    void clearAllSelected();
    void closeSelected();
    void disconnectSelected();
    void graphClicked(QCPAbstractPlottable *plottable);

    void res10ASelected();
    void res10nASelected();
    void res100nASelected();
    void res1000nASelected();

    void rate2000Selected();
    void rate5000Selected();
    void rate10000Selected();

private:
    Ui::MainWindow *ui;

    int samples;
    int sampleRate;

    int graphMemory;        // used in parseAndPlot()
    int sampleNumber;       // used in parseAndPlot()
    //double xVal;             // used in parseAndPlot()
    
    int waveNum;


};

#endif // MAINWINDOW_H

