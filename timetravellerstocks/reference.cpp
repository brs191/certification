#include <vector>
#include <algorithm>
#include <unordered_map>

using namespace std;
const int MAX_ORDERS = 200001;
const int MAX_STOCKS = 6;

struct Order
{
    int order_id;
    //int stock_id;
    int qty;
    int price;
    bool status; // true (bought) : false (cancelled or not initied)
};

Order orders[MAX_ORDERS];

struct CompareBuy
{
    bool operator()(Order *a, Order *b)
    {
        if (a->price > b->price)
            return true;
        if (a->price < b->price)
            return false;
        if (a->order_id < b->order_id)
            return true;
        return false;
    }
};
struct CompareSell
{
    bool operator()(Order *a, Order *b)
    {
        if (a->price < b->price)
            return true;
        if (a->price > b->price)
            return false;
        if (a->order_id < b->order_id)
            return true;
        return false;
    }
};

template <typename C>
struct StockHeap
{
    Order *heap[MAX_ORDERS];
    int heapSize = 0;

    C comp;

    void heapInit(void)
    {
        heapSize = 0;
    }

    int heapPush(Order *value)
    {
        if (heapSize + 1 > MAX_ORDERS)
        {
            return 0;
        }

        heap[heapSize] = value;

        int current = heapSize;
        while (current > 0 && comp(heap[current], heap[(current - 1) / 2]))
        {
            Order *temp = heap[(current - 1) / 2];
            heap[(current - 1) / 2] = heap[current];
            heap[current] = temp;
            current = (current - 1) / 2;
        }

        heapSize = heapSize + 1;

        return 1;
    }

    Order *heapPop()
    {
        if (heapSize <= 0)
        {
            return 0;
        }

        Order *value = heap[0];
        heapSize = heapSize - 1;

        heap[0] = heap[heapSize];

        int current = 0;
        while (current * 2 + 1 < heapSize)
        {
            int child;
            if (current * 2 + 2 == heapSize)
            {
                child = current * 2 + 1;
            }
            else
            {
                child = comp(heap[current * 2 + 1], heap[current * 2 + 2]) ? current * 2 + 1 : current * 2 + 2;
            }

            if (comp(heap[current], heap[child]))
            {
                break;
            }

            Order *temp = heap[current];
            heap[current] = heap[child];
            heap[child] = temp;

            current = child;
        }
        return value;
    }
};

struct Stock
{
    StockHeap<CompareBuy> buyHeap;
    StockHeap<CompareSell> sellHeap;
    int max_profit;
    int min_price;
};
Stock stocks[MAX_STOCKS];

void init()
{
    for (register int i = 1; i < MAX_ORDERS; i++)
    {
        orders[i].status = false;
    }
    for (register int i = 1; i < 6; i++)
    {
        stocks[i].min_price = 10000001;
        stocks[i].max_profit = 0;
        stocks[i].buyHeap.heapInit();
        stocks[i].sellHeap.heapInit();
    }
}

int buy(int mNumber, int mStock, int mQuantity, int mPrice)
{
    while (mQuantity > 0 && stocks[mStock].sellHeap.heapSize > 0)
    {
        Order *order = stocks[mStock].sellHeap.heap[0];
        if (order->status == false)
        {
            stocks[mStock].sellHeap.heapPop();
            continue;
        }
        if (mPrice < order->price)
            break;
        if (mQuantity >= order->qty)
        {
            mQuantity = mQuantity - order->qty;
            order->qty = 0;
            order->status = false;
            stocks[mStock].sellHeap.heapPop();
        }
        else
        {
            order->qty -= mQuantity;
            mQuantity = 0;
        }

        stocks[mStock].min_price = min(stocks[mStock].min_price, order->price);
        stocks[mStock].max_profit = max(stocks[mStock].max_profit, order->price - stocks[mStock].min_price);
    }

    if (mQuantity > 0)
    {
        orders[mNumber].status = true;
        orders[mNumber].order_id = mNumber;
        orders[mNumber].price = mPrice;
        orders[mNumber].qty = mQuantity;
        stocks[mStock].buyHeap.heapPush(&orders[mNumber]);
    }
    return mQuantity;
}

int sell(int mNumber, int mStock, int mQuantity, int mPrice)
{
    while (mQuantity > 0 && stocks[mStock].buyHeap.heapSize > 0)
    {
        Order *order = stocks[mStock].buyHeap.heap[0];
        if (order->status == false)
        {
            stocks[mStock].buyHeap.heapPop();
            continue;
        }
        if (mPrice > order->price)
            break;
        if (mQuantity >= order->qty)
        {
            mQuantity = mQuantity - order->qty;
            order->qty = 0;
            order->status = false;
            stocks[mStock].buyHeap.heapPop();
        }
        else
        {
            order->qty -= mQuantity;
            mQuantity = 0;
        }

        stocks[mStock].min_price = min(stocks[mStock].min_price, order->price);
        stocks[mStock].max_profit = max(stocks[mStock].max_profit, order->price - stocks[mStock].min_price);
    }

    if (mQuantity > 0)
    {
        orders[mNumber].status = true;
        orders[mNumber].order_id = mNumber;
        orders[mNumber].price = mPrice;
        orders[mNumber].qty = mQuantity;
        stocks[mStock].sellHeap.heapPush(&orders[mNumber]);
    }
    return mQuantity;
}

void cancel(int mNumber)
{
    orders[mNumber].status = false;
}

int bestProfit(int mStock)
{
    return stocks[mStock].max_profit;
}