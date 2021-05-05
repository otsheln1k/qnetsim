#ifndef _NS_STEPPABLE_HPP_
#define _NS_STEPPABLE_HPP_

/* Usage
 *
 * Stepping only happens in interfaces; the parts on upper levels are driven
 * either by the interfaces or by timers. To ensure steps are separated,
 * interfaces must buffer transmitted data.
 *
 * 1. All writes go to send-buffer (instead of executing immediately);
 * 2. We want to be able to review send-buffer contents, so replies to received
 *    messages must NOT be transmitted immediately. Thus, the order of sub-steps
 *    is send, then recv.
 *
 * Return value is true if any action was taken during the step. If all steps of
 * all elements of the model return false, the simulation can be stopped.
 */

class Steppable {
public:
    virtual bool stepSend() =0;
    virtual bool stepRecv() =0;

    bool step()
    {
        bool res = false;
        res = stepSend() || res;
        res = stepRecv() || res;
        return res;
    }
};

#endif
