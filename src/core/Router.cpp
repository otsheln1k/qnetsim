#include "Router.h"

Router::Router()
{
    auto router = NodeRoute();
    router.getIP4Node()->setForwardingEnabled(true);
}
