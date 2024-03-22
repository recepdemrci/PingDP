#ifndef PING_H
#define PING_H

#include <QObject>
#include <QDebug>
#include <QSysInfo>
#include <QProcess>

class Ping : public QObject
{
    Q_OBJECT
public:
    explicit Ping(QObject *parent = nullptr);

    QString getAddress() const;
    void setAddress(const QString &address);
    double getInterval() const;
    void setInterval(const double interval);

signals:
    void output(QString data);
    void draw(double x, double y = -0.5);
    void reset();

public slots:
    void start();
    void stop();

private slots:
    void readyReadStandardError();
    void readyReadStandardOutput();
    void stateChanged (QProcess:: ProcessState newState);
    void readyRead();

private:
    QProcess process;   // Ping process
    QString address;    // Ip address or url
    double interval;    // Time interval in second
    bool listening;     // State of the process
    int count;          // Number of response
    void sendPing();

};

#endif // PING_H
