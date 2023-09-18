import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.net.InetAddress;
import java.net.Socket;

public class Client {
  private static final int port = 4200;

  public static void main(String[] args) {
    try {
      InetAddress host = InetAddress.getByName("10.1.135.93");
      int numb = 1;
      Socket socket = new Socket(host.getHostName(), port);

      DataOutputStream oos = new DataOutputStream(socket.getOutputStream());
      DataInputStream ois = new DataInputStream(socket.getInputStream());

      for (int i = 0; i <10; i++) {
        if (i == 0) {
          System.out.println("Sending request to Socket Server");
          oos.write(numb);
        }
        // convert DataInputStream object to integer
        numb = ois.read();
        System.out.println("Message Received: " + numb);
        // create DataOutputStream object
        numb = nextFibonacci(numb);
        // write object to Socket
        oos.write(numb);
        // terminate the server if client sends exit request
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
