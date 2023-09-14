import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.net.InetAddress;
import java.net.ServerSocket;
import java.net.Socket;

public class Client {
  private static final int port = 4200;

  public static void main(String[] args) {
    try {
      InetAddress host = InetAddress.getLocalHost();
      int curr = 0;
      int numb = 1;
      Socket socket = new Socket(host.getHostName(), port);
      ObjectOutputStream oos = new ObjectOutputStream(socket.getOutputStream());
      ObjectInputStream ois = new ObjectInputStream(socket.getInputStream());
      ;

      while (true) {
        if (curr == 0) {
          System.out.println("Sending request to Socket Server");
          oos.writeObject(numb);
        }
        // convert ObjectInputStream object to integer
        numb = (Integer)ois.readObject();
        System.out.println("Message Received: " + numb);
        // create ObjectOutputStream object
        numb = nextFibonacci(numb);
        // write object to Socket
        oos.writeObject(numb);
        // terminate the server if client sends exit request
        if (numb > 10000)
          break;
        curr++;
      }
      // close resources
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