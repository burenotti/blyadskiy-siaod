#include <iostream>
#include <bitset>

using namespace std;

void coutp(unsigned int x)
{
    int n = sizeof(int) * 8;
    unsigned maska = (1 << (n - 1));
    for (int i = 1; i <= n; i++)
    {
        cout << ((x & maska) >> (n - i));
        maska = maska >> 1;
    }
    cout << endl;
}

void fun1() {
    int x = 0xD1A; //110100011010
    int maska = 0xAAA; //101010101010

    int answer = x | maska;
    cout << "Изначальное число: " << endl;
    coutp(x);
    coutp(answer);
}

void fun2() {
    int x = 0xBC6; //101111000110
    int maska = 0x1E0; //000111100000
    //111000011111 = E1F

    int answer = x & (~maska);
    cout << "Изначальное число: " << endl;
    coutp(x);
    coutp(answer);
}

void fun3() {
    int x;
    cout << "Введите число: ";
    cin >> x;
    int answer = x << 5;

    cout << "Изначальное число: " << endl;
    coutp(x);
    coutp(answer);
}

void fun4() {
    int x;
    cout << "Введите число: ";
    cin >> x;
    int answer = x >> 5;

    cout << "Изначальное число: " << endl;
    coutp(x);
    coutp(answer);
}

void fun5() {
    int x, n;
    cout << "Введите число и номер бита: ";
    cin >> x;
    cin >> n;

    int maska = 1 << n;
    int answer = x | maska;

    cout << "Изначальное число: " << endl;
    coutp(x);
    coutp(answer);
}

int main() {
    setlocale(LC_ALL, "ru");
    //fun1();
    //fun2();
    //fun3();
    //fun4();
    fun5();

}