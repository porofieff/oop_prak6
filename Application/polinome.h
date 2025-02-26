#ifndef POLINOME_H
#define POLINOME_H

#include "array.h"
#include <math.h>

enum PrintMode
{
    PrintModeClassic,
    PrintModeCanonic,
};


template <class number>
class Polinome
{
    PrintMode mode;
    Array<number> roots;
    Array<number> coefficents;
    int N;
private:
    void count_coefficents(number);
public:
    Polinome();
    Polinome(number, number[], int);

    template <class T>
    friend ostream& operator<< (ostream&, Polinome<T>&);
    template <class T>
    friend istream& operator>> (istream&, Polinome<T>&);
    template <class T>
    friend QString& operator<< (QString&, Polinome<T>&);

    void change_print_mode(PrintMode);
    void change_an(number);
    void change_roots(number, int);
    void change_size(int);

    int get_size();

    number count_val(number);

};

template <class number>
Polinome<number>::Polinome()
{
    N = 0;
    mode = PrintModeClassic;
}

template <class number>
Polinome<number>::Polinome(number new_an, number new_roots[], int new_size)
{
    N = new_size;
    roots.change_size(N);
    coefficents.change_size(N + 1);

    for (int i = 0; i < N; i++)
    {
        roots[i] = new_roots[i];
    }

    count_coefficents(new_an);
    mode = PrintModeClassic;
}

template <class number>
void Polinome<number>::count_coefficents(number new_an)
{
    for (int i = 0; i < N; i++)
        coefficents[i] = 0;
    coefficents[N] = new_an;
    coefficents[1] = 1;
    coefficents[0] = -roots[0];

    for (int k = 2; k <= N; k++)
    {
        coefficents[k] = coefficents[k-1];
        for (int i = k-1; i > 0; i--)
        {
            coefficents[i] = coefficents[i-1] - coefficents[i] * roots[k-1];
        }
        coefficents[0] = -coefficents[0] * roots[k-1];
    }
    for (int i = 0; i <= N; i++)
        coefficents[i] = coefficents[i] * new_an;
}

template <class number>
void Polinome<number>::change_print_mode (PrintMode new_mode)
{
    mode = new_mode;
}

template <class number>
ostream& operator<< (ostream& os, Polinome<number>& pol)
{
    if (pol.mode == PrintModeCanonic)
    {
        for (int i = pol.N; i > 0; i--)
            os << pol.coefficents[i] << " x^" << i << " + ";
        os << pol.coefficents[0] << std::endl;
    }
    else
    {
        os << pol.coefficents[pol.N] << " * ";
        for (int i = 0; i < pol.N - 1; i++)
            os << "(x - " << pol.roots[i] << ") * ";
        os << "(x - " << pol.roots[pol.N - 1] << ")" << std::endl;
    }
    return os;
}

template <class number>
QString& operator<< (QString& os, Polinome<number>& pol)
{
    if (pol.mode == PrintModeCanonic)
    {
        for (int i = pol.N; i > 0; i--)
        {
            os << pol.coefficents[i];
            os += " x^";
            os += QString().setNum(i);
            os += " + ";
        }
        os << pol.coefficents[0];
        os += "\n";
    }
    else
    {
        os << pol.coefficents[pol.N];
        os += " * ";
        for (int i = 0; i < pol.N - 1; i++)
        {
            os += "(x - ";
            os << pol.roots[i];
            os +=  ") * ";
        }
        os += "(x - ";
        os << pol.roots[pol.N - 1];
        os += ")";
        os += "\n";
    }
    return os;
}

template <class number>
istream& operator>> (istream& is, Polinome<number>& pol)
{
    is >> pol.coefficents[pol.N];
    for (int i = 0; i < pol.N; i++)
        is >> pol.roots[i];
    pol.count_coefficents(pol.coefficents[pol.N]);
    return is;
}

template <class number>
number Polinome<number>::count_val (number new_point)
{
    number val = coefficents[N];
    for (int i = 0; i < N; i++)
        val = (new_point - roots[i]) * val;
    return val;
}

template <class number>
void Polinome<number>::change_an(number new_an)
{
    coefficents[N] = new_an;
    count_coefficents(coefficents[N]);
}

template <class number>
void Polinome<number>::change_roots(number new_root, int new_index)
{
    roots[new_index] = new_root;
    count_coefficents(coefficents[N]);
}

template <class number>
void Polinome<number>::change_size(int new_size)
{
    //number an = coefficents[N];
    roots.change_size(new_size);
    coefficents.change_size(new_size + 1);
    N = new_size;
    //change_an(an);
}

template <class number>
int Polinome<number>::get_size()
{
    return N;
}

#endif // POLINOME_H
