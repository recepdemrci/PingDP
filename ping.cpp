#include "ping.h"
#include "QtCore/qregularexpression.h"

Ping::Ping(QObject *parent) : QObject{parent}
{
    // Connect QProcess methods to the Ping::process
    connect (&process, &QProcess::readyReadStandardError, this, &Ping::readyReadStandardError);
    connect (&process, &QProcess::readyReadStandardOutput,this, &Ping::readyReadStandardOutput);
    connect (&process, &QProcess::stateChanged, this, &Ping::stateChanged);
    connect (&process, &QProcess::readyRead, this, &Ping::readyRead);

    // Init attributes
    count = 0;
    address = "";
    interval = 1.0;
    listening = false;
}

QString Ping::getAddress() const
{
    return address;
}

void Ping::setAddress(const QString &address)
{
    this->address = address;
    if (listening) {
        stop();
    }
}

double Ping::getInterval() const
{
    return interval;;
}

void Ping::setInterval(const double interval)
{
    this->interval = interval;
    if (listening) {
        stop();
    }
}

/*
 * Start the terminal process (cmd, bash ..)
 * Consider the OS
 */
void Ping::start()
{
    // If there is already running process
    // then stop previous and start new one
    if (listening) {
        stop();
    }

    // Check OS and run terminal based on that
    QString terminal;
    QString productType = QSysInfo::productType();
    if (productType == "windows") {
        terminal = "cmd";
    } else if (productType == "osx") {
        terminal = "/bin/zsh";
    } else {
        terminal = "bash";
    }

    listening = true;
    process.start(terminal);
}

/*
 * Stop the terminal process
 */
void Ping::stop()
{
    listening = false;
    process.close();
    count = 0;
    emit reset();
}

/*
 * When error occured on terminal,
 * display error message and stop the process
 */
void Ping::readyReadStandardError()
{
    if (listening) {
        emit output("ERROR: Invalid IP Address or URL");
        stop();
    }
}

/*
 * When output came on terminal, display error message
 */
void Ping::readyReadStandardOutput()
{
    if (listening) {
        QByteArray data = process.readAllStandardOutput();
        emit output(QString(data.trimmed()));
    }
}

/*
 * When the process of terminal is running state, start the ping
 */
void Ping::stateChanged(QProcess::ProcessState newState)
{
    if (newState == QProcess::Running) {
        sendPing();
    }
}

/*
 * When response came from the ping,
 * Display duration on the graph or Error
 */
void Ping::readyRead()
{
    if (listening) {
        QByteArray data = process.readAll().trimmed();
        emit output(data);

        // Extract request count and response duration from output
        QRegularExpression reTime("time=([\\d.]+)");
        QRegularExpressionMatchIterator matchesTime = reTime.globalMatch(data);
        if (!matchesTime.hasNext()) {
            emit draw(count);
            count += 1;
        }

        while (matchesTime.hasNext()) {
            double duration = matchesTime.next().captured(1).toDouble();
            emit draw(count, duration);
            count += 1;
        }
    }
}

/*
 * Set and run the ping command (ping 127.0.0.1 -i 0.5)
 */
void Ping::sendPing()
{
    QByteArray command;
    command.append("ping " + address.toUtf8());
    command.append(" -i " + QString::number(interval).toUtf8());
    if (QSysInfo::productType() == "windows") {
        command.append("\r");
    }
    command.append("\n");
    process.write(command);
}

