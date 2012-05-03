#include <queue>
#include <boost/thread/mutex.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>

/**
* Its just std::queue with mutex on every operation
* MBO Whole class ;-)
*/
template < class T, class Container = std::deque<T> >
class SafeQueue : protected std::queue<T,Container>
{
    typedef std::queue<T,Container> baseQueue;
    boost::mutex m;

public:
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
        boost::interprocess::scoped_lock<boost::mutex>(m);
        return baseQueue::front();
    }
    const T& front ( ) const
    {
        boost::interprocess::scoped_lock<boost::mutex>(m);
        return baseQueue::front();
    }
    T& back ( )
    {
        boost::interprocess::scoped_lock<boost::mutex>(m);
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
        return baseQueue::push(x);
    }
    void pop ( )
    {
        boost::interprocess::scoped_lock<boost::mutex>(m);
        return baseQueue::pop();
    }
};

