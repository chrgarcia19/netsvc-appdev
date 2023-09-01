// A C# program for Client
using System;
using System.Net;
using System.Net.Sockets;
using System.Text;

namespace Client
{

    class FibSocket2
    {

        // Main Method
        static void Main(string[] args)
        {
            try
            {

                // Establish the remote endpoint
                // for the socket. This example
                // uses port 11111 on the local
                // computer.
                IPHostEntry ipHost = Dns.GetHostEntry(Dns.GetHostName());
                IPAddress ipAddr = ipHost.AddressList[0];
                IPEndPoint localEndPoint = new IPEndPoint(ipAddr, 4200);

                // Creation TCP/IP Socket using
                // Socket Class Constructor
                Socket sender = new Socket(ipAddr.AddressFamily,
                           SocketType.Stream, ProtocolType.Tcp);

                try
                {

                    while (true)
                    {
                        try
                        {
                            // Connect Socket to the remote
                            // endpoint using method Connect()
                            sender.Connect(localEndPoint);

                            if (sender.Connected)
                            {
                                // We print EndPoint information
                                // that we are connected
                                Console.WriteLine("Socket connected to -> {0} ",
                                              sender.RemoteEndPoint.ToString());
                                break;
                            }
                        }
                        catch
                        {
                            Console.WriteLine("Sender Not Yet Connected");
                        }

                    }


                    // Creation of message that
                    // we will send to Server
                    byte[] messageSent = BitConverter.GetBytes(1);

                    // Send a message to Client
                    // using Send() method
                    int byteSent = sender.Send(messageSent);

                    while (true)
                    {

                        // Data buffer
                        byte[] bytes = new Byte[1024];
                        int i = 0;
                        while (i < 4)
                        {
                            i = sender.Receive(bytes);
                        }

                        int numb = BitConverter.ToInt32(bytes, 0);
                        Console.WriteLine("Text received -> {0} ", numb);
                        int nextNumb = nextFibonacci(numb);
                        Console.WriteLine("Text Sending -> {0} ", nextNumb);
                        byte[] message = BitConverter.GetBytes(nextNumb);

                        // Send a message to Client
                        // using Send() method
                        sender.Send(message);

                        if (nextNumb > 10000) break;

                        if (!sender.Connected) break;
                    }

                    // Close client Socket using the
                    // Close() method. After closing,
                    // we can use the closed Socket
                    // for a new Client Connection
                    sender.Shutdown(SocketShutdown.Both);
                    sender.Close();
                }

                // Manage of Socket's Exceptions
                catch (ArgumentNullException ane)
                {

                    Console.WriteLine("ArgumentNullException : {0}", ane.ToString());
                }

                catch (SocketException se)
                {

                    Console.WriteLine("SocketException : {0}", se.ToString());
                }

                catch (Exception e)
                {
                    Console.WriteLine("Unexpected exception : {0}", e.ToString());
                }
            }

            catch (Exception e)
            {

                Console.WriteLine(e.ToString());
            }
        }

        // fibonacci number
        static int nextFibonacci(int n)
        {
            double a = n * (1 + Math.Sqrt(5)) / 2.0;
            return (int)Math.Round(a);
        }
    }
}
