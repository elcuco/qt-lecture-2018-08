#include <QCoreApplication>
#include <QTcpServer>
#include <QAbstractSocket>
#include <QIODevice>
#include <QTcpSocket>
#include <QObject>
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QTcpServer *server = new QTcpServer();

    server->listen(QHostAddress::Any, 8888);
    QObject::connect(server, &QTcpServer::newConnection, [server](){
        QTcpSocket *socket = server->nextPendingConnection();
        QObject::connect(socket, &QIODevice::readyRead, [socket](){
            QByteArray data = socket->readAll();
            QString input = QString(data);
            QString yes = "Yes\n";
            socket->write(yes.toLatin1());
            socket->flush();
            socket->close();

            qDebug() << "Read from socket:" << input;
            qDebug() << "Write to socket:" << yes;
        });
    });

    return a.exec();
}
