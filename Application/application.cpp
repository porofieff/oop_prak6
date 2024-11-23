#include "application.h"
#include "common.h"
#include "communicator.h"
#include <iostream>
using namespace std;


Application::Application(int argc, char *argv[])
    : QCoreApplication(argc,argv)
{
    CommParams pars = { QHostAddress("127.0.0.1"), 9998,
                       QHostAddress("127.0.0.1"), 9999};
    comm = new Communicator(pars, this);

    connect(comm,SIGNAL(recieved(QByteArray)),this,SLOT(recieve(QByteArray)));

    p.change_size(2);
    p.change_an(number(2, 0));
    p.change_roots(number(1, 0), 0);
    p.change_roots(number(3, 0), 1);
}

void Application::recieve(QByteArray msg)
{
    QString answer, s;
    complex an, x, v, root;
    int size, root_pos, pol_size;
    int pos = msg.indexOf(separator);
    int t = msg.left(pos).toInt();
    cout << "есть контакт"<< endl;
    switch (t)
    {
    case VALUE_REQUEST:
        msg = msg.right(msg.length()-pos-1);
        msg >> x;
        v = p.count_val(x);
        s << (QString)x << (QString)v;
        answer<<QString().setNum(VALUE_ANSWER);
        answer += s;
        cout << answer.toStdString() << endl;
        break;
    case PRINT_CLASSIC_REQUEST:
        p.change_print_mode(PrintModeClassic);
        s<<p;
        answer<<QString().setNum(PRINT_ANSWER)<<s;
        cout << answer.toStdString() << endl;
        break;
    case PRINT_CANONIC_REQUEST:
        p.change_print_mode(PrintModeCanonic);
        s << p;
        answer<<QString().setNum(PRINT_ANSWER)<<s;
        cout << answer.toStdString() << endl;
        break;
    case CHANGE_AN_REQUEST:
        msg = msg.right(msg.length()-pos-1);
        msg >> an;
        p.change_an(an);
        answer << QString().setNum(CHANGE_LAST_ANSWER);
        cout << answer.toStdString() << endl;
        break;
    case CHANGE_POL_AN_REQUEST:
        msg = msg.right(msg.length()-pos-1);
        msg >> an;
        p.change_an(an);
        answer << QString().setNum(CHANGE_POL_ANSWER);
        cout << answer.toStdString() << endl;
        break;
    case CHANGE_SIZE_REQUEST:
        msg = msg.right(msg.length()-pos-1);
        pos = msg.indexOf(separator);
        size = msg.left(pos).toInt();
        p.change_size(size);
        answer << QString().setNum(CHANGE_LAST_ANSWER);
        cout << answer.toStdString() << endl;
        break;
    case CHANGE_ROOT_REQUEST:
        msg = msg.right(msg.length()-pos-1);
        msg >> root;
        pos = msg.indexOf(separator);
        root_pos = msg.left(pos).toInt();
        p.change_roots(root, root_pos);
        answer << QString().setNum(CHANGE_LAST_ANSWER);
        cout << answer.toStdString() << endl;
        break;
    case CHANGE_POL_ROOTS_REQUEST:
        msg = msg.right(msg.length()-pos-1);
        msg >> root;
        pos = msg.indexOf(separator);
        root_pos = msg.left(pos).toInt();
        p.change_roots(root, root_pos);
        if (root_pos == p.get_size() - 1)
            answer << QString().setNum(CHANGE_LAST_ANSWER);
        else
            answer << QString().setNum(CHANGE_POL_ANSWER);
        cout << answer.toStdString() << endl;
        break;
    case GET_POLIN_SIZE:
        pol_size = p.get_size();
        answer<<QString().setNum(SIZE_ANSWER);
        answer += pol_size;
        cout << answer.toStdString() << endl;
    default: return;
    }
    comm->send(QByteArray().append(answer));
}
