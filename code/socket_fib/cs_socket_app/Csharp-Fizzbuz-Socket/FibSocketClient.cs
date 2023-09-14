using System;
using System.Collections.Generic;
// A C# Program for Server
using System;
using System.Net;
using System.Net.Sockets;
using System.Text;
using static System.Runtime.InteropServices.JavaScript.JSType;
using System.Reflection;

namespace Server {

class FibSocketClienClient {

  // Main Method
  static void Main(string[] args) {
    // Establish the local endpoint
    // for the socket. Dns.GetHostName
    // returns the name of the host
    // running the application.
    IPHostEntry ipHost = Dns.GetHostEntry(Dns.GetHostName());
    IPAddress ipAddr = ipHost.AddressList[0];
    IPEndPoint localEndPoint = new IPEndPoint(ipAddr, 4200);

    // Creation TCP/IP Socket using
    // Socket Class Constructor
    Socket listener =
        new Socket(ipAddr.AddressFamily, SocketType.Stream, ProtocolType.Tcp);

    try {

      // Using Bind() method we associate a
      // network address to the Server Socket
      // All client that will connect to this
      // Server Socket must know this network
      // Address
      listener.Bind(localEndPoint);

      // Using Listen() method we create
      // the Client list that will want
      // to connect to Server
      listener.Listen(10);

      Console.WriteLine("Waiting connection ... ");

      // Suspend while waiting for
      // incoming connection Using
      // Accept() method the server
      // will accept connection of client
      Socket clientSocket = listener.Accept();

      while (true) {

        // Data buffer
        byte[] bytes = new Byte[1024];
        int i = 0;
        while (i < 4) {
          i = clientSocket.Receive(bytes);
        }

        int numb = BitConverter.ToInt32(bytes, 0);
        Console.WriteLine("Text received -> {0} ", numb);
        int nextNumb = nextFibonacci(numb);
        Console.WriteLine("Text Sending -> {0} ", nextNumb);
        byte[] message = BitConverter.GetBytes(nextNumb);

        // Send a message to Client
        // using Send() method
        clientSocket.Send(message);

        if (nextNumb > 10000)
          break;

        if (!clientSocket.Connected)
          break;
      }

      // Close client Socket using the
      // Close() method. After closing,
      // we can use the closed Socket
      // for a new Client Connection
      clientSocket.Shutdown(SocketShutdown.Both);
      clientSocket.Close();
    }

    catch (Exception e) {
      Console.WriteLine(e.ToString());
    }
  }

  // fibonacci number
  static int nextFibonacci(int n) {
    double a = n * (1 + Math.Sqrt(5)) / 2.0;
    return (int)Math.Round(a);
  }
}
}
