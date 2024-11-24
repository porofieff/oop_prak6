#ifndef APPLICATION_H
#define APPLICATION_H

#include <QObject>
#include <QCoreApplication>

#include "communicator.h"
#include "polinome.h"

class Application : public QCoreApplication
{
    Q_OBJECT

    Communicator *comm;

    Polinome<complex> p_c;
    Polinome<double> p_d;

public:

    Application(int, char**);

signals:

public slots:

    void recieve(QByteArray);

};

#endif // APPLICATION_H
