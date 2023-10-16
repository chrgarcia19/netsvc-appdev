import com.opencsv.CSVReader;

import java.io.*;
import java.net.InetAddress;
import java.net.ServerSocket;
import java.net.Socket;
import java.net.UnknownHostException;
import java.util.HashMap;
import java.util.List;
import java.util.Objects;
import java.util.Random;

public class WOTD {

    static Socket socket;
    static DataInputStream input;
    static DataOutputStream out;
    static InetAddress address;
    static int port = 4200;

    static File data = new File("../../../../../quotes.csv");

    static HashMap<Integer, String[]> quotes;

    public static void Client() {
        // establish a connection
        try {
            socket = new Socket(address, port);
            System.out.println("Connected");

            // takes input from terminal
            input = new DataInputStream(socket.getInputStream());

            // sends output to the socket
            out = new DataOutputStream(
                    socket.getOutputStream());
        } catch (UnknownHostException u) {
            System.out.println(u);
            return;
        } catch (IOException i) {
            System.out.println(i);
            return;
        }

        try {
            out.writeUTF("Send Quote");
            String quote = input.readUTF();
            System.out.println(quote);
        } catch (IOException e) {
            throw new RuntimeException(e);
        }

        // close the connection
        try {
            input.close();
            out.close();
            socket.close();
        } catch (IOException i) {
            System.out.println(i);
        }
    }

    static public void Server()
    {
        // starts server and waits for a connection
        try
        {
            ServerSocket server = new ServerSocket(port);
            System.out.println("Server started");

            System.out.println("Waiting for a client ...");

            socket = server.accept();
            System.out.println("Client accepted");

            // takes input from the client socket
            input = new DataInputStream(new BufferedInputStream(socket.getInputStream()));
            out = new DataOutputStream(new BufferedOutputStream(socket.getOutputStream()));

            String line = "";

            // reads message from client until "Over" is sent
            while (!line.equals("Over"))
            {
                try
                {
                    line = input.readUTF();
                    System.out.println(line);
                    if (line.equals("Send Quote")){
                        Random random = new Random();
                        int row = random.nextInt(1660);
                        out.writeUTF(quotes.get(row)[1]);
                    }
                }
                catch(IOException i)
                {
                    System.out.println(i);
                }
            }
            System.out.println("Closing connection");

            // close connection
            socket.close();
            input.close();
        }
        catch(IOException i)
        {
            System.out.println(i);
        }
    }

    public static void main(String args[]) {

        try {
            FileReader reader1 = new FileReader(data);
            CSVReader reader2 = new CSVReader(reader1);
            String[] nextRecord;
            int curr = 0;
            while ((nextRecord = reader2.readNext()) != null) {
                quotes.put(curr, nextRecord);
                curr++;
            }
        }
        catch (IOException e) {
            throw new RuntimeException(e);
        }

        try {
            boolean IsServer = false;
            address = InetAddress.getLocalHost();
            if (args.length > 0) {
                if (args[0].equalsIgnoreCase("server")) IsServer = true;
                if (args.length > 1) {
                    port = Integer.parseInt(args[1]);
                    if (args.length > 2) {
                        address = InetAddress.getByName(args[2]);
                    }
                }
            }

            if (IsServer) {
                Server();
            }
            else{
                Client();
            }
        } catch (IOException u) {
            System.out.println(u);
            return;
        }
    }
}