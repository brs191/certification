#include <iostream>

using namespace std;

#define MAX_STACK 100
typedef struct _stack
{
    int top;
    int stack[MAX_STACK];

    void init()
    {
        top = 0;
    }
    bool isEmpty()
    {
        return (top == 0);
    }
    int TOP()
    {
        if (top > 0)
            return stack[top - 1];
        return -1;
    }
    void PUSH(int val)
    {
        stack[top++] = val;
    }
    int POP()
    {
        return stack[--top];
    }

    void PRINT()
    {
        for (int i = 0; i < top; i++)
        {
            cout << (char)stack[i];
        }
        cout << endl;
    }
} STACK;

STACK operators;
STACK values;

int precedence(char op)
{
    if (op == '+' || op == '-')
        return 1;
    if (op == '*' || op == '/')
        return 2;
    return 0;
}

bool isDigit(int ch)
{
    if (ch - '0' > 0 && ch - '0' < 10)
        return true;
    return false;
}

int applyOp(int val1, int val2, int ch)
{
    switch (ch)
    {
    case '+':
        return val1 + val2;
    case '-':
        return val1 - val2;
    case '*':
        return val1 * val2;
    }
}

int evalExpr(char expr[])
{
    int i = 0;
    int c = 0;
    while ((c = expr[i]) != '\0')
    {

        if (c == '(')
        {
            operators.PUSH(c);
        }
        else if (isDigit(c))
        {
            values.PUSH(c - '0');
        }
        else if (c == ')')
        {
            while (!operators.isEmpty() && operators.TOP() != '(')
            {
                int val2 = values.TOP();
                values.POP();

                int val1 = values.TOP();
                values.POP();

                int op = operators.TOP();
                operators.POP();

                int out = applyOp(val1, val2, op);
                values.PUSH(out);
            }
        }
        else
        {
        }

        i++;
    }
    operators.PRINT();
    cout << expr << endl;
    return 1;
}

int main()
{
    char expr[] = "3+5*(1-5+4)+2";

    int val = evalExpr(expr);
    return 0;
}