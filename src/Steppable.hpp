#ifndef _NS_STEPPABLE_HPP_
#define _NS_STEPPABLE_HPP_

/* Usage
 *
 * To implement layer-at-a-time stepping, first step the sending parts from
 * lower to higher layers, then step the receiving parts from higher to lower
 * layer. Only start receiving after all nodes in the model have completed the
 * sending part.
 */

class Steppable {
public:
    virtual void stepSend() =0;
    virtual void stepRecv() =0;
};

#endif
