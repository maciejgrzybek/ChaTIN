#define BOOST_TEST_MODULE const_string test
#include <boost/test/unit_test.hpp>
#include <boost/test/execution_monitor.hpp> 
#include "../src/Socket.hpp"

BOOST_AUTO_TEST_CASE( connection_test )
{
/*    Socket::ServerSocket s1; // default arguments for constructor - should be binded on ::
//    BOOST_REQUIRE(s1.socket != 0);
    BOOST_CHECK(s1.getHostAddress() == "::");
    Socket::ServerSocket s2;
    BOOST_CHECK(s1.getHostAddress() == s2.getHostAddress());
    s1.listen();
    Socket::ClientSocket c1; // default arguments for constructor - should get ephemeral port from system
//    BOOST_REQUIRE(c1.socket != 0);
    c1.setHostAddress("::1");
    BOOST_CHECK(c1.getHostAddress() == "::1");
    BOOST_CHECK(s1.isClientEnqueued() == false); // there is no client connected, should be false
    c1.connect(s1.getHostAddress(),s1.getHostPort());
    usleep(2000); // wait 2s
    BOOST_REQUIRE(s1.isClientEnqueued()); // now, client should be connected and enqueued.
    Socket::ServerSocket::ClientIncomeSocket *ci1 = NULL;
    ci1 = s1.pickClient();
    BOOST_CHECK(ci1); // instance of ClientIncomeSocket exists*/
    Socket::ServerSocket s1("::1",1024);
    s1.listen();
    Socket::ClientSocket c1;
    BOOST_CHECK_NO_THROW(c1.connect("::1",1024)); // should not throw, otherwise connection failure
}

BOOST_AUTO_TEST_CASE( port_obtained_test )
{
    Socket::ServerSocket s1("::1",1025);
//    BOOST_REQUIRE_THROW(Socket::ServerSocket s2("::1",1025), Socket::SocketCreationFailureException); // the same port as above
}

BOOST_AUTO_TEST_CASE( communication_test )
{
    Socket::ServerSocket s1("::1",1026);
    Socket::ClientSocket c1;
    BOOST_REQUIRE_NO_THROW(s1.listen());
    BOOST_REQUIRE_NO_THROW(c1.connect("::1",1026)); // can't throw, otherwise end of test and failure
    std::string msg = "test message";
    BOOST_REQUIRE_NO_THROW(c1.send(msg)); // if thrown - sending does not work
    BOOST_REQUIRE(s1.isClientEnqueued() == true);
    Socket::ServerSocket::ClientIncomeSocket* inCli = NULL;
    BOOST_REQUIRE_NO_THROW(inCli = s1.pickClient()); // client picked
    BOOST_CHECK_EQUAL(inCli->receive(),msg);
}

BOOST_AUTO_TEST_CASE( sending_connectionless_test )
{
    Socket::ClientSocket c1;
    std::string msg = "test message";
    // send data without being connected
    BOOST_REQUIRE_THROW(c1.send(msg),Socket::SendFailureException);
}

BOOST_AUTO_TEST_CASE( receiving_connectionless_test )
{
    Socket::ClientSocket c1;
    // receive without being connected
    BOOST_REQUIRE_THROW(c1.receive(),Socket::SocketException);
}
