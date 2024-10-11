#include <cstdio>
#include "Request.h"  
#include "EStore.h"
/*
 * ------------------------------------------------------------------
 * add_item_handler --
 *
 *      Handle an AddItemReq.
 *
 *      Delete the request object when done.
 *
 * Results:
 *      None.
 *
 * ------------------------------------------------------------------
 */
void 
add_item_handler(void *args)
{
    AddItemReq *req = static_cast<AddItemReq *>(args);
    printf("add_item_handler: item_id=%d, quantity=%d, price=%.2f, discount=%.2f\n",
           req->item_id, req->quantity, req->price, req->discount);

    req->store->addItem(req->item_id, req->quantity, req->price, req->discount);

    delete req;
}

/*
 * ------------------------------------------------------------------
 * remove_item_handler --
 *
 *      Handle a RemoveItemReq.
 *
 *      Delete the request object when done.
 *
 * Results:
 *      None.
 *
 * ------------------------------------------------------------------
 */
void 
remove_item_handler(void *args)
{
    RemoveItemReq *req = static_cast<RemoveItemReq *>(args);
    printf("remove_item_handler: item_id=%d\n", req->item_id);

    req->store->removeItem(req->item_id);

    delete req;
}

/*
 * ------------------------------------------------------------------
 * add_stock_handler --
 *
 *      Handle an AddStockReq.
 *
 *      Delete the request object when done.
 *
 * Results:
 *      None.
 *
 * ------------------------------------------------------------------
 */
void 
add_stock_handler(void *args)
{
    AddStockReq *req = static_cast<AddStockReq *>(args);
    printf("add_stock_handler: item_id=%d, additional_stock=%d\n", req->item_id, req->additional_stock);

    req->store->addStock(req->item_id, req->additional_stock);

    delete req;
}

/*
 * ------------------------------------------------------------------
 * change_item_price_handler --
 *
 *      Handle a ChangeItemPriceReq.
 *
 *      Delete the request object when done.
 *
 * Results:
 *      None.
 *
 * ------------------------------------------------------------------
 */
void 
change_item_price_handler(void *args)
{
    ChangeItemPriceReq *req = static_cast<ChangeItemPriceReq *>(args);
    printf("change_item_price_handler: item_id=%d, new_price=%.2f\n", req->item_id, req->new_price);

    req->store->priceItem(req->item_id, req->new_price);

    delete req;
}

/*
 * ------------------------------------------------------------------
 * change_item_discount_handler --
 *
 *      Handle a ChangeItemDiscountReq.
 *
 *      Delete the request object when done.
 *
 * Results:
 *      None.
 *
 * ------------------------------------------------------------------
 */
void 
change_item_discount_handler(void *args)
{
    ChangeItemDiscountReq *req = static_cast<ChangeItemDiscountReq *>(args);
    printf("change_item_discount_handler: item_id=%d, new_discount=%.2f\n", req->item_id, req->new_discount);

    req->store->discountItem(req->item_id, req->new_discount);

    delete req;
}

/*
 * ------------------------------------------------------------------
 * set_shipping_cost_handler --
 *
 *      Handle a SetShippingCostReq.
 *
 *      Delete the request object when done.
 *
 * Results:
 *      None.
 *
 * ------------------------------------------------------------------
 */
void 
set_shipping_cost_handler(void *args)
{
    SetShippingCostReq *req = static_cast<SetShippingCostReq *>(args);
    printf("set_shipping_cost_handler: new_shipping_cost=%.2f\n", req->new_cost);

    req->store->setShippingCost(req->new_cost);

    delete req;
}

/*
 * ------------------------------------------------------------------
 * set_store_discount_handler --
 *
 *      Handle a SetStoreDiscountReq.
 *
 *      Delete the request object when done.
 *
 * Results:
 *      None.
 *
 * ------------------------------------------------------------------
 */
void
set_store_discount_handler(void *args)
{
    SetStoreDiscountReq *req = static_cast<SetStoreDiscountReq *>(args);
    printf("set_store_discount_handler: new_discount=%.2f\n", req->new_discount);

    req->store->setStoreDiscount(req->new_discount);

    delete req;
}

/*
 * ------------------------------------------------------------------
 * buy_item_handler --
 *
 *      Handle a BuyItemReq.
 *
 *      Delete the request object when done.
 *
 * Results:
 *      None.
 *
 * ------------------------------------------------------------------
 */
void
buy_item_handler(void *args)
{
    BuyItemReq *req = static_cast<BuyItemReq *>(args);
    printf("buy_item_handler: item_id=%d, budget=%.2f\n", req->item_id, req->budget);

    req->store->buyItem(req->item_id, req->budget);

    delete req;
}

/*
 * ------------------------------------------------------------------
 * buy_many_items_handler --
 *
 *      Handle a BuyManyItemsReq.
 *
 *      Delete the request object when done.
 *
 * Results:
 *      None.
 *
 * ------------------------------------------------------------------
 */
void
buy_many_items_handler(void *args)
{
    BuyManyItemsReq *req = static_cast<BuyManyItemsReq *>(args);
    printf("buy_many_items_handler: budget=%.2f\n", req->budget);

    req->store->buyManyItems(&req->item_ids, req->budget);

    delete req;
}

/*
 * ------------------------------------------------------------------
 * stop_handler --
 *
 *      The thread should exit.
 *
 * Results:
 *      None.
 *
 * ------------------------------------------------------------------
 */
void 
stop_handler(void* args)
{
    printf("stop_handler: Stopping the thread\n");
}
