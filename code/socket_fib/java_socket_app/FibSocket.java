import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.*;
import java.util.Random;
import java.util.Timer;
import java.util.TimerTask;

public class FibSocket {
  private static int port = 4200;

  static Timer timer = new Timer();
  
  private static Socket socket = new Socket();

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
      boolean IsServer = false;
      InetAddress host = InetAddress.getLocalHost();
      if (args.length > 0){
        if (args[0].equalsIgnoreCase("server")) IsServer = true;
        if (args.length > 1){
          port = Integer.parseInt(args[1]);
          if (args.length > 2){
            host = InetAddress.getByName(args[2]);
          }
        }
      }

      if (IsServer){
        ServerSocket server = new ServerSocket(port);
        System.out.println("Waiting for incoming connection...");
        socket = server.accept();
      }
      else{
        SocketAddress address = new InetSocketAddress(host, port);
        socket.bind(address);
        socket.connect(address, 10000);
      }


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
