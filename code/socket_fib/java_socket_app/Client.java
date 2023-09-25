import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.net.Socket;
import java.net.SocketAddress;
import java.util.Random;
import java.util.Timer;
import java.util.TimerTask;

public class Client {
  private static int port = 4200;

  static Timer timer = new Timer();
  
  private static final Socket socket = new Socket();

  static class Task extends TimerTask {
    private static DataOutputStream dos;
    private static DataInputStream dis;

    public Task(DataOutputStream dosIN, DataInputStream disIN){
      dos = dosIN;
      dis = disIN;
    }
    @Override
    public void run() {
      int delay = (2 + new Random().nextInt(8)) * 1000;
      timer.schedule(new Task(dos, dis), delay);
      try {
        System.out.println("\nSending Message");
        dos.write(new Random().nextInt(50));
        int numb = dis.read();
        System.out.println("Received Fib Number: " + numb);
        System.out.println("Setting new task in " + delay/1000 + " seconds.");
      } catch (IOException e) {
        System.out.println("Ended Timer Thread");
        timer.cancel();
      }
    }
  }

  public static void main(String[] args) {
    try {
      InetAddress host = InetAddress.getLocalHost();
      if (args.length > 0){
        port = Integer.parseInt(args[0]);
        if (args.length > 1){
          host = InetAddress.getByName(args[1]);
        }
      }
      SocketAddress address = new InetSocketAddress(host, port);
      socket.bind(address);
      socket.connect(address);

      DataOutputStream dos = new DataOutputStream(socket.getOutputStream());
      DataInputStream dis = new DataInputStream(socket.getInputStream());

     new Task(dos, dis).run();

      for (int i = 0; i <= 10; i++) {
        // convert DataInputStream object to integer
        int numb = dis.read();
        System.out.println("Received Index: " + numb + "\tNumber " + i + "/10");

        numb = fib(numb);

        System.out.println("Sending Result, fib: " + numb);
        // write object to Socket
        dos.write(numb);
        // terminate the server if client sends exit request
      }
      
      dis.close();
      dos.close();
      socket.close();
    } catch (Exception ex) {
      System.out.println(ex);
      System.exit(1);
    }
  }

  // fibonacci number
  static int fib(int n)
  {
    int a = 0, b = 1, c;
    if (n == 0)
      return a;
    for (int i = 2; i <= n; i++) {
      c = a + b;
      a = b;
      b = c;
    }
    return b;
  }
}
