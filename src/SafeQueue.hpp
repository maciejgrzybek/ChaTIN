#include <queue>
#include <boost/thread/mutex.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <boost/interprocess/sync/interprocess_semaphore.hpp>

/**
* Its just std::queue with mutex on every operation
* MBO Whole class ;-)
*/
template < class T, class Container = std::deque<T> >
class SafeQueue : protected std::queue<T,Container>
{
    typedef std::queue<T,Container> baseQueue;
    boost::mutex m;
    /* used to avoid active wait for something in queue */
    boost::interprocess::interprocess_semaphore sizeSem;

public:
    SafeQueue() : sizeSem(0)
    {}

    bool empty() const
    {
        boost::interprocess::scoped_lock<boost::mutex>(m);
        return baseQueue::empty();
    }
    size_t size() const
    {
        boost::interprocess::scoped_lock<boost::mutex>(m);
        return baseQueue::size();
    }
    T& front ( )
    {
        sizeSem.wait(); //must be in this order
        boost::interprocess::scoped_lock<boost::mutex>(m);
        sizeSem.post();
        return baseQueue::front();
    }
    const T& front ( ) const
    {
        boost::interprocess::scoped_lock<boost::mutex>(m);
        return baseQueue::front();
    }
    T& back ( )
    {
        sizeSem.wait(); //must be in this order
        boost::interprocess::scoped_lock<boost::mutex>(m);
        sizeSem.post();
        return baseQueue::back();
    }
    const T& back ( ) const
    {
        boost::interprocess::scoped_lock<boost::mutex>(m);
        return baseQueue::back();
    }
    void push ( const T& x )
    {
        boost::interprocess::scoped_lock<boost::mutex>(m);
        sizeSem.post();
        return baseQueue::push(x);
    }

    /**
     * This method will hang process if queue is empty
     */
    void pop ( )
    {
        sizeSem.wait(); //must be in this order
        boost::interprocess::scoped_lock<boost::mutex>(m);
        return baseQueue::pop();
    }
};

