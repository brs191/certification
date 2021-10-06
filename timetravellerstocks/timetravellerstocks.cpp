#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <iostream>
#include <set>
#include <stdio.h>

#define CMD_INIT 1
#define CMD_BUY 2
#define CMD_SELL 3
#define CMD_CANCEL 4
#define CMD_BEST_PROFIT 5

using namespace std;
#define MAX_STOCKS 6
#define MAX_ORDERS 200001
struct Order
{
    int orderId;
    int quantity;
    int price;
    bool status; // true (bought), false (cancelled or not initialized)
};
Order orders[MAX_ORDERS];

struct compareBuy
{
    bool operator()(Order *a, Order *b)
    {
        if (a->price > b->price)
        {
            return true;
        }
        if (a->price < b->price)
        {
            return false;
        }
        if (a->orderId < b->orderId)
        {
            return true;
        }
        return false;
    }
};

struct compareSell
{
    bool operator()(Order *a, Order *b)
    {
        if (a->price < b->price)
        {
            return true;
        }
        if (a->price > b->price)
        {
            return false;
        }
        if (a->orderId < b->orderId)
        {
            return true;
        }
        return false;
    }
};

struct Stock
{
    int maxProfit;
    int minPrice;

    int buyListSize;
    int sellListSize;
    set<Order *, compareBuy> buyList;
    set<Order *, compareSell> sellList;
};
Stock stocks[MAX_STOCKS];

void init()
{
    for (register int i = 0; i < MAX_ORDERS; i++)
    {
        orders[i].status = false;
    }
    for (register int i = 0; i < MAX_STOCKS; i++)
    {
        stocks[i].minPrice = 10000001;
        stocks[i].maxProfit = 0;
        stocks[i].buyListSize = 0;
        stocks[i].sellListSize = 0;
    }
}

int buy(int mNumber, int mStock, int mQuantity, int mPrice)
{
    while (mQuantity > 0 && stocks[mStock].sellListSize > 0)
    {
        // auto it = stocks[mStock].sellList.begin();
        set<Order *, compareSell>::iterator itr = stocks[mStock].sellList.begin();
        Order *order = *itr;
        if (order->status == false) {
            stocks[mStock].sellList.erase(itr);
            stocks[mStock].sellListSize--;
            continue;
        }
        if (mPrice < order->price) {
            break;
        }
        if (mQuantity >= order->quantity) {
            mQuantity = mQuantity - order->quantity;
            order->quantity = 0;
            order->status = false;
            stocks[mStock].sellList.erase(itr);
            stocks[mStock].sellListSize--;
        } else {
            order->quantity = order->quantity - mQuantity;
            mQuantity = 0;
        }
        stocks[mStock].minPrice = min(stocks[mStock].minPrice, order->price);
        stocks[mStock].maxProfit = max(stocks[mStock].maxProfit, order->price - stocks[mStock].minPrice);
    }

    // buy list is empty; nothing to buy; add to buyList;
    if (mQuantity > 0)
    {
        orders[mNumber].status = true;
        orders[mNumber].orderId = mNumber;
        orders[mNumber].price = mPrice;
        orders[mNumber].quantity = mQuantity;
        stocks[mStock].buyList.insert(&orders[mNumber]);
        stocks[mStock].buyListSize++;
    }
    return mQuantity;
}

int sell(int mNumber, int mStock, int mQuantity, int mPrice)
{
    while (mQuantity > 0 && stocks[mStock].buyListSize > 0) {
        set<Order *, compareBuy>::iterator itr = stocks[mStock].buyList.begin();
        Order *order = *itr;
        if (order->status) {
            stocks[mStock].buyList.erase(itr);
            stocks[mStock].buyListSize--;
            continue;
        }
        if (mPrice > order->price) {
            break;
        }
        if (mQuantity >= order->quantity) {
            mQuantity = mQuantity - order->quantity;
            order->quantity = 0;
            order->status = false;
            stocks[mStock].buyList.erase(itr);
            stocks[mStock].buyListSize--;
        } else {
            order->quantity = order->quantity - mQuantity;
            mQuantity = 0;
        }
        stocks[mStock].minPrice = min(stocks[mStock].minPrice, order->price);
        stocks[mStock].maxProfit = max(stocks[mStock].maxProfit, order->price - stocks[mStock].minPrice);
    }

    if (mQuantity > 0) {
        orders[mNumber].status = true;
        orders[mNumber].orderId = mNumber;
        orders[mNumber].price = mPrice;
        orders[mNumber].quantity = mQuantity;
        stocks[mStock].sellList.insert(&orders[mNumber]);
        stocks[mStock].sellListSize++;
    }
    return mQuantity;
}

void cancel(int mNumber)
{
    orders[mNumber].status = false;
}

int bestProfit(int mStock)
{
    return stocks[mStock].maxProfit;
}

/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////

static bool run()
{
    int numQuery;

    int mNumber, mStock, mQuantity, mPrice;

    int userAns, ans;

    bool isCorrect = false;

    scanf("%d", &numQuery);

    for (int i = 0; i < numQuery; ++i)
    {
        int cmd;
        scanf("%d", &cmd);
        switch (cmd)
        {
        case CMD_INIT:
            init();
            isCorrect = true;
            break;
        case CMD_BUY:
            scanf("%d %d %d %d", &mNumber, &mStock, &mQuantity, &mPrice);
            userAns = buy(mNumber, mStock, mQuantity, mPrice);
            scanf("%d", &ans);
            if (userAns != ans)
            {
                isCorrect = false;
            }
            break;
        case CMD_SELL:
            scanf("%d %d %d %d", &mNumber, &mStock, &mQuantity, &mPrice);
            userAns = sell(mNumber, mStock, mQuantity, mPrice);
            scanf("%d", &ans);
            if (userAns != ans)
            {
                isCorrect = false;
            }
            break;
        case CMD_CANCEL:
            scanf("%d", &mNumber);
            cancel(mNumber);
            break;
        case CMD_BEST_PROFIT:
            scanf("%d", &mStock);
            userAns = bestProfit(mStock);
            scanf("%d", &ans);
            if (userAns != ans)
            {
                isCorrect = false;
            }
            break;
        default:
            isCorrect = false;
            break;
        }
    }

    return isCorrect;
}

int main()
{
    setbuf(stdout, NULL);
    freopen("sample_input.txt", "r", stdin);

    int T, MARK;
    scanf("%d %d", &T, &MARK);

    for (int tc = 1; tc <= T; tc++)
    {
        int score = run() ? MARK : 0;
        printf("#%d %d\n", tc, score);
    }

    return 0;
}