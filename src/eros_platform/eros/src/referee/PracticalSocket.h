#ifndef __PRACTICALSOCKET_INCLUDED__
#define __PRACTICALSOCKET_INCLUDED__

#include <string>            // For string
#include <exception>         // For exception class

using namespace std;

class SocketException : public exception {
public:
  SocketException(const string &message, bool inclSysMsg = false) throw();

  ~SocketException() throw();

  const char *what() const throw();

private:
  string userMessage;  // Exception message
};

class Socket {
public:
  ~Socket();

  string getLocalAddress() throw(SocketException);

  unsigned short getLocalPort() throw(SocketException);

  void setLocalPort(unsigned short localPort) throw(SocketException);

  void setLocalAddressAndPort(const string &localAddress, 
    unsigned short localPort = 0) throw(SocketException);

  static void cleanUp() throw(SocketException);

  static unsigned short resolveService(const string &service,
                                       const string &protocol = "tcp");

private:
  Socket(const Socket &sock);
  void operator=(const Socket &sock);

protected:
  int sockDesc;              // Socket descriptor
  Socket(int type, int protocol) throw(SocketException);
  Socket(int sockDesc);
};

class CommunicatingSocket : public Socket {
public:
  void connect(const string &foreignAddress, unsigned short foreignPort)
    throw(SocketException);

  void send(const void *buffer, int bufferLen) throw(SocketException);

  int recv(void *buffer, int bufferLen) throw(SocketException);

  string getForeignAddress() throw(SocketException);

  unsigned short getForeignPort() throw(SocketException);

protected:
  CommunicatingSocket(int type, int protocol) throw(SocketException);
  CommunicatingSocket(int newConnSD);
};

class TCPSocket : public CommunicatingSocket {
public:
  TCPSocket() throw(SocketException);

  TCPSocket(const string &foreignAddress, unsigned short foreignPort) 
      throw(SocketException);

private:
  friend class TCPServerSocket;
  TCPSocket(int newConnSD);
};

class TCPServerSocket : public Socket {
public:
  TCPServerSocket(unsigned short localPort, int queueLen = 5) 
      throw(SocketException);

  TCPServerSocket(const string &localAddress, unsigned short localPort,
      int queueLen = 5) throw(SocketException);

  TCPSocket *accept() throw(SocketException);

private:
  void setListen(int queueLen) throw(SocketException);
};

class UDPSocket : public CommunicatingSocket {
public:
  UDPSocket() throw(SocketException);

  UDPSocket(unsigned short localPort) throw(SocketException);

  UDPSocket(const string &localAddress, unsigned short localPort) 
      throw(SocketException);

  void disconnect() throw(SocketException);

  void sendTo(const void *buffer, int bufferLen, const string &foreignAddress,
            unsigned short foreignPort) throw(SocketException);

  int recvFrom(void *buffer, int bufferLen, string &sourceAddress, 
               unsigned short &sourcePort) throw(SocketException);

  void setMulticastTTL(unsigned char multicastTTL) throw(SocketException);

  void joinGroup(const string &multicastGroup) throw(SocketException);

  void leaveGroup(const string &multicastGroup) throw(SocketException);

private:
  void setBroadcast();
};

#endif
