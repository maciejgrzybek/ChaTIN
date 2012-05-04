#pragma once
/**
 * Class which should be used to add elements to waiting queue
 */
template <class QueueType>
class QueueAdder
{
    ChaTIN::IncomingMassage income;
    SafeQueue<QueueType>& q;
public:
    /**
     * Create functor with given argumeters
     * @param const QueueType& massage that came  from DialogManager
     * @param SafeQueue<QueueType>& q queue to which we will add our income
     */
    QueueAdder( const QueueType& income, SafeQueue<QueueType>& q )
        : income(income), q(q)
    {}
    
    /** 
     * Ads income to SafeQueue (designed to run in new thread).
     */
    void operator()()
    {
        q.push(income); //this may hang process for a while
    }
};



