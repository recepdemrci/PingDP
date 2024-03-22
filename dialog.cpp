#include "dialog.h"
#include "ui_dialog.h"

#define MINY -1.0
#define MAXY 1.0

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);

    // Connect UI items to the Ping func
    connect(ui->btnStart, &QPushButton::clicked, &ping, &Ping::start);
    connect(ui->btnStop, &QPushButton::clicked, &ping, &Ping::stop);
    connect(&ping, &Ping::output, this, &Dialog::output);
    connect(&ping, &Ping::draw, this, &Dialog::draw);
    connect(&ping, &Ping::reset, this, &Dialog::reset);

    // Init ping attr
    ping.setAddress(ui->txtIP->text());
    ping.setInterval(ui->interval->value());
    // Init graph
    ui->customPlot->setObjectName("Network Heartbeat");
    ui->customPlot->xAxis->setLabel("Count");
    ui->customPlot->yAxis->setLabel("Duration [ms]");
    ui->customPlot->addGraph();
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::output(QString data)
{
    ui->plainTextEdit->appendPlainText(data);
}

/*
 * Draw the response duration graph
 */
void Dialog::draw(double x, double y)
{
    // Add data to the graph
    ui->customPlot->graph(0)->addData(x, y);

    // Set the graph axis and plot
    ui->customPlot->rescaleAxes();
    double maxY = ui->customPlot->yAxis->range().upper;
    if (maxY < MAXY) {
        maxY = MAXY;
    }
    ui->customPlot->yAxis->setRange(MINY, maxY);
    ui->customPlot->replot();
}

void Dialog::reset()
{
    ui->customPlot->graph(0)->data()->clear();
    ui->customPlot->replot();
}

/*
 * IP adress or url change handler
 */
void Dialog::on_txtIP_textChanged(const QString &address)
{
    ping.setAddress(ui->txtIP->text());
}


/*
 * Time interval change handler
 */
void Dialog::on_interval_valueChanged(double interval)
{
    ping.setInterval(ui->interval->value());
}
