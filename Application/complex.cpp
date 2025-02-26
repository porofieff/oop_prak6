#include "complex.h"
#include <cmath>
#include <common.h>

QChar complex::SEPARATOR = separator;

complex::complex()
{

}

complex::complex(const int& r)
{
    im = 0;
    re = r;
}

double arg(double re, double im)
{
    return atan(im/re);
}

complex::complex(double r, double i)
{
    re = r;
    im = i;
}

ostream& operator<<(ostream& os, complex c)
{
    os << c.re << "+" << c.im << "i";
    return os;
}

istream& operator>>(istream& is, complex& c)
{
    is >> c.re >> c.im;
    return is;
}

QString& operator<< (QString& s, complex c)
{
    s += "(";
    s += QString().setNum(c.re);
    s += "+";
    s += QString().setNum(c.im);
    s += "i)";
    return s;
}

QByteArray& operator>>(QByteArray& arr, complex& c)
{
    int p = arr.indexOf(complex::SEPARATOR);
    p = arr.indexOf(complex::SEPARATOR,p+1);
    if (p > 0)
    {
        c = complex(arr.left(p));
        arr = arr.right(arr.length()-p-1);
    }
    return arr;
}

complex::complex(const QByteArray& arr)
{
    int p = arr.indexOf(SEPARATOR);
    re = arr.left(p).toDouble();
    im = arr.right(arr.length()-p-1).toDouble();
}

complex complex::operator*(complex c)
{
    complex t;
    t.re = re * c.re - im * c.im;
    t.im = re * c.im + im * c.re;
    return t;
}

complex complex::operator+(complex c)
{
    complex t;
    t.re = re + c.re;
    t.im = im + c.im;
    return t;
}

complex complex::operator-(complex c)
{
    complex t;
    t.re = re - c.re;
    t.im = im - c.im;
    return t;
}

complex complex::operator/(int c)
{
    complex t;
    t.re = re / c;
    t.im = im / c;
    return t;
}

complex complex::operator=(int a)
{
    complex t;
    t.re = a;
    t.im = 0;
    return t;
}

complex complex::operator- ()
{
    complex temp;
    temp.re = -re;
    if (temp.re == -0) temp.re = 0;
    temp.im = -im;
    if (temp.im == -0) temp.im = 0;
    return temp;
}

complex::operator QString ()
{
    QString s = "(";
    s += QString().setNum(re);
    s += "+";
    s += QString().setNum(im);
    s += "i)";
    return s;
}

/*complex complex:: operator-(complex)
{
    complex t;
    t.re = re;
    t.im = -im;
    return t;
};*/
bool complex::operator<(complex c)
{
    bool res = false;
    int m1;
    int m2;
    m1 = sqrt(re * re + im * im);
    m2 = sqrt(c.re * c.re + c.im * c.im);
    if (m1 < m2)
    {
        res = true;
    }
    else if (m1 == m2)
    {
        if (arg(re, im) < arg(c.re, c.im))
        {
            res = true;
        }
    }
    return res;
}

bool complex::operator>(complex c)
{
    bool res = false;
    int m1;
    int m2;
    m1 = sqrt(re * re + im * im);
    m2 = sqrt(c.re * c.re + c.im * c.im);
    if (m1 > m2)
    {
        res = true;
    }
    else if (m1 == m2)
    {
        if (arg(re, im) > arg(c.re, c.im))
        {
            res = true;
        }
    }
    return res;
}

complex sqrt(complex c)
{
    complex t;
    t.re = sqrt((sqrt(c.re*c.re + c.im*c.im) + c.re) / 2);
    t.im = sqrt((sqrt(c.re*c.re + c.im*c.im) - c.re) / 2);

    if(c.im < 0)
    {
        t.im = t.im * -1;
    }

    return t;
}
