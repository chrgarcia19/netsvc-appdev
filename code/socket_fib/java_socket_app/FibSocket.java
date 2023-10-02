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
  private static boolean readIndex = true;

  static class Task extends TimerTask {
    private static DataOutputStream dos;
    private static DataInputStream dis;

    public Task(DataOutputStream dosIN, DataInputStream disIN) {
      dos = dosIN;
      dis = disIN;
    }

    @Override
    public synchronized void run() {
      int delay = (1 + new Random().nextInt(4)) * 1000;
      timer.schedule(new Task(dos, dis), delay);
      System.out.println("[TIMER THREAD] Setting new task in " + delay / 1000 + " seconds.");
      try {
        int temp = new Random().nextInt(30);
        System.out.println("\n [TIMER THREAD] Sending Message: " + temp);
        dos.write(temp);
        readIndex = false;
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
        if (args.length > 0) {
          if (args[0].equalsIgnoreCase("server")) IsServer = true;
          if (args.length > 1) {
            port = Integer.parseInt(args[1]);
            if (args.length > 2) {
              host = InetAddress.getByName(args[2]);
            }
          }
        }

        Socket socket;
        if (IsServer) {
          ServerSocket server = new ServerSocket(port);
          System.out.println("Waiting for incoming connection...");
          socket = server.accept();
        } else {
          socket = new Socket(host, port);
        }


        DataOutputStream dos = new DataOutputStream(socket.getOutputStream());
        DataInputStream dis = new DataInputStream(socket.getInputStream());

        if (IsServer) new Task(dos, dis).run();

        int curr = 1;

        while (curr <= 10) {
          // convert DataInputStream object to integer
          int numb = dis.read();
          if (readIndex){
            System.out.println("[MAIN THREAD] Received Index: " + numb + " \tNumber " + curr + "/10");

            numb = fib(numb);

            //System.out.println("[MAIN THREAD] Sending Result, fib: " + numb);
            // write object to Socket
            dos.write(numb);
            curr++;
          }
          else{
            System.out.println("\t[TIMER THREAD] Received Fib Number: " + numb);
            readIndex = true;
          }

          // terminate the server if client sends exit request
        }

        dis.close();
        dos.close();
        socket.close();
      } catch (Exception ex) {
        System.out.println(ex);
        System.exit(1);
      }
      finally {
        System.exit(0);
      }
    }

    // fibonacci number
    static int fib(int n) {
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
