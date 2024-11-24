#include "application.h"
#include "common.h"
#include "communicator.h"
#include <iostream>

using namespace std;

QByteArray& operator>>(QByteArray& arr, double& d)
{
    int p = arr.indexOf(separator);
    p = arr.indexOf(separator);
    if (p > 0)
    {
        d = atof(arr.left(p));
        arr = arr.right(arr.length()-p-1);
    }
    return arr;
}

QString& operator<< (QString& s, double d)
{
    s += "(";
    s += QString().setNum(d);
    s += ")";
    return s;
}


Application::Application(int argc, char *argv[])
    : QCoreApplication(argc,argv)
{
    CommParams pars = { QHostAddress("127.0.0.1"), 10000,
                       QHostAddress("127.0.0.1"), 10001};
    comm = new Communicator(pars, this);

    connect(comm,SIGNAL(recieved(QByteArray)),this,SLOT(recieve(QByteArray)));

    p_c.change_size(2);
    p_c.change_an(number(2, 0));
    p_c.change_roots(number(1, 0), 0);
    p_c.change_roots(number(3, 0), 1);

    p_d.change_size(2);
    p_d.change_an(2);
    p_d.change_roots(1, 0);
    p_d.change_roots(3, 1);

}

void Application::recieve(QByteArray msg)
{
    QString answer, s;
    int size, root_pos;
    int pos = msg.indexOf(separator);
    int mode = msg.left(pos).toInt();
    msg = msg.right(msg.length() - pos - 1);
    if (mode == COMPLEX_MODE)
    {
        complex an, x, v, root;
        int pos = msg.indexOf(separator);
        int t = msg.left(pos).toInt();
        switch (t)
        {
        case VALUE_REQUEST:
            msg = msg.right(msg.length()-pos-1);
            msg>>x;
            v = p_c.count_val(x);
            s<<(QString)x<<(QString)v;
            answer<<QString().setNum(VALUE_ANSWER);
            answer += s;
            break;
        case PRINT_CLASSIC_REQUEST:
            p_c.change_print_mode(PrintModeClassic);
            s<<p_c;
            answer<<QString().setNum(PRINT_ANSWER)<<s;
            break;
        case PRINT_CANONIC_REQUEST:
            p_c.change_print_mode(PrintModeCanonic);
            s << p_c;
            answer<<QString().setNum(PRINT_ANSWER)<<s;
            break;
        case CHANGE_AN_REQUEST:
            msg = msg.right(msg.length()-pos-1);
            msg >> an;
            p_c.change_an(an);
            answer << QString().setNum(CHANGE_LAST_ANSWER);
            break;
        case CHANGE_POL_AN_REQUEST:
            msg = msg.right(msg.length()-pos-1);
            msg >> an;
            p_c.change_an(an);
            answer << QString().setNum(CHANGE_POL_ANSWER);
            break;
        case CHANGE_SIZE_REQUEST:
            msg = msg.right(msg.length()-pos-1);
            pos = msg.indexOf(separator);
            size = msg.left(pos).toInt();
            p_c.change_size(size);
            answer << QString().setNum(CHANGE_LAST_ANSWER);
            break;
        case CHANGE_ROOT_REQUEST:
            msg = msg.right(msg.length()-pos-1);
            msg >> root;
            pos = msg.indexOf(separator);
            root_pos = msg.left(pos).toInt();
            p_c.change_roots(root, root_pos);
            answer << QString().setNum(CHANGE_LAST_ANSWER);
            break;
        case CHANGE_POL_ROOTS_REQUEST:
            msg = msg.right(msg.length()-pos-1);
            msg >> root;
            pos = msg.indexOf(separator);
            root_pos = msg.left(pos).toInt();
            p_c.change_roots(root, root_pos);
            if (root_pos == p_c.get_size() - 1)
                answer << QString().setNum(CHANGE_LAST_ANSWER);
            else
                answer << QString().setNum(CHANGE_POL_ANSWER);
            break;
        default: return;
        }
    }
    if (mode == DOUBLE_MODE)
    {
        double an, x, v, root;
        int pos = msg.indexOf(separator);
        int t = msg.left(pos).toInt();
        switch (t)
        {
        case VALUE_REQUEST:
            msg = msg.right(msg.length()-pos-1);
            msg>>x;
            v = p_d.count_val(x);
            s<<QString::number(x)<<QString::number(v);
            answer<<QString().setNum(VALUE_ANSWER);
            answer += s;
            break;
        case PRINT_CLASSIC_REQUEST:
            p_d.change_print_mode(PrintModeClassic);
            s<<p_d;
            answer<<QString().setNum(PRINT_ANSWER)<<s;
            break;
        case PRINT_CANONIC_REQUEST:
            p_d.change_print_mode(PrintModeCanonic);
            s << p_d;
            answer<<QString().setNum(PRINT_ANSWER)<<s;
            break;
        case CHANGE_AN_REQUEST:
            msg = msg.right(msg.length()-pos-1);
            msg >> an;
            p_d.change_an(an);
            answer << QString().setNum(CHANGE_LAST_ANSWER);
            break;
        case CHANGE_POL_AN_REQUEST:
            msg = msg.right(msg.length()-pos-1);
            msg >> an;
            p_d.change_an(an);
            answer << QString().setNum(CHANGE_POL_ANSWER);
            break;
        case CHANGE_SIZE_REQUEST:
            msg = msg.right(msg.length()-pos-1);
            pos = msg.indexOf(separator);
            size = msg.left(pos).toInt();
            p_d.change_size(size);
            answer << QString().setNum(CHANGE_LAST_ANSWER);
            break;
        case CHANGE_ROOT_REQUEST:
            msg = msg.right(msg.length()-pos-1);
            msg >> root;
            pos = msg.indexOf(separator);
            root_pos = msg.left(pos).toInt();
            p_d.change_roots(root, root_pos);
            answer << QString().setNum(CHANGE_LAST_ANSWER);
            break;
        case CHANGE_POL_ROOTS_REQUEST:
            msg = msg.right(msg.length()-pos-1);
            msg >> root;
            pos = msg.indexOf(separator);
            root_pos = msg.left(pos).toInt();
            p_d.change_roots(root, root_pos);
            if (root_pos == p_d.get_size() - 1)
                answer << QString().setNum(CHANGE_LAST_ANSWER);
            else
                answer << QString().setNum(CHANGE_POL_ANSWER);
            break;
        default: return;
        }
    }
    comm->send(QByteArray().append(answer));
}
