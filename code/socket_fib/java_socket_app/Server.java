import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.net.ServerSocket;
import java.net.Socket;

public class Server {

  private static final int port = 4200;

  public static void main(String[] args) {
    try {
      ServerSocket server = new ServerSocket(port);
      System.out.println("Waiting for the client request");
      // creating socket and waiting for client connection
      Socket socket = server.accept();
      // read from socket to DataInputStream Data
      DataInputStream ois = new DataInputStream(socket.getInputStream());
      // create DataOutputStream Data
      DataOutputStream oos = new DataOutputStream(socket.getOutputStream());

      for (int i = 0; i<= 10; i++) {
        // convert DataInputStream Data to String
        int numb = ois.read();
        System.out.println("Message Received: " + numb);
        numb = nextFibonacci(numb);
        // write Data to Socket
        oos.write(numb);
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