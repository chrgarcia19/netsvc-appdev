import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.net.ServerSocket;
import java.net.Socket;

public class Server {

  private static ServerSocket server;

  private static final int port = 4200;

  public static void main(String[] args) {
    try {
      server = new ServerSocket(port);
      System.out.println("Waiting for the client request");
      // creating socket and waiting for client connection
      Socket socket = server.accept();
      // read from socket to ObjectInputStream object
      ObjectInputStream ois = new ObjectInputStream(socket.getInputStream());
      // create ObjectOutputStream object
      ObjectOutputStream oos = new ObjectOutputStream(socket.getOutputStream());

      while (true) {
        // convert ObjectInputStream object to String
        Integer numb = (Integer)ois.readObject();
        System.out.println("Message Received: " + numb);
        numb = nextFibonacci(numb);
        // write object to Socket
        oos.writeObject(numb);
        if (numb > 10000)
          break;
      }
      ois.close();
      oos.close();
      socket.close();

    } catch (Exception ex) {
      System.out.println(ex);
    }
  }

  // fibonacci number
  static int nextFibonacci(int n) {
    double a = n * (1 + Math.sqrt(5)) / 2.0;
    return (int)Math.round(a);
  }
}