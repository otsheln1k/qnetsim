#ifndef _NS_STEPPABLE_HPP_
#define _NS_STEPPABLE_HPP_

/* Usage
 *
 * To implement layer-at-a-time stepping, first step the sending parts from
 * lower to higher layers, then step the receiving parts from higher to lower
 * layer. Only start receiving after all nodes in the model have completed the
 * sending part.
 *
 * Return value is true if any action was taken during the step. If all steps of
 * all elements of the model return false, the simulation can be stopped.
 */

class Steppable {
public:
    virtual bool stepSend() =0;
    virtual bool stepRecv() =0;
};

#endif
