import com.sun.management.OperatingSystemMXBean;

import java.io.*;
import java.lang.management.ManagementFactory;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.text.DecimalFormat;
import java.util.Scanner;

public class CpuBroadcast {
    static InetAddress address;
    static int port = 4200;

    public static void main (String[] args){
        try {
            boolean isServer = false;
            address = InetAddress.getLocalHost();
            if (args.length > 0) {
                if (args[0].equalsIgnoreCase("server")) {
                    isServer = true;
                }

                if (args.length > 1) {
                    port = Integer.parseInt(args[1]);
                    if (args.length > 2) {
                        address = InetAddress.getByName(args[2]);
                    }
                }
            }

            if (isServer) {
                Server();
            } else {
                Client();
            }

        } catch (IOException | InterruptedException e) {
            System.out.println(e);
        }
    }

    public static void Client() throws IOException, InterruptedException {
        Scanner sc = new Scanner(System.in);

        // Step 1:Create the socket object for
        // carrying the data.
        DatagramSocket ds = new DatagramSocket();

        byte[] buf = null;

        // loop while user not enters "bye"
        while (true)
        {
            String temp = GetLoad();

            // convert the String input into the byte array.
            buf = temp.getBytes();

            // Step 2 : Create the datagramPacket for sending
            // the data.
            DatagramPacket DpSend =
                    new DatagramPacket(buf, buf.length, address, port);

            // Step 3 : invoke the send call to actually send
            // the data.
            ds.send(DpSend);

            Thread.sleep(1000);
        }
    }

    public static void Server() throws IOException
    {
        // Step 1 : Create a socket to listen at port 1234
        DatagramSocket ds = new DatagramSocket(port);
        byte[] receive = new byte[65535];

        DatagramPacket DpReceive = null;
        while (true)
        {

            // Step 2 : create a DatgramPacket to receive the data.
            DpReceive = new DatagramPacket(receive, receive.length);

            // Step 3 : revieve the data in byte buffer.
            ds.receive(DpReceive);

            System.out.println("Client: " + data(receive) + "%");

            // Clear the buffer after every message.
            receive = new byte[65535];
        }
    }

    public static String GetLoad() throws IOException {
        OperatingSystemMXBean osBean = ManagementFactory.getPlatformMXBean(OperatingSystemMXBean.class);
        double load = osBean.getCpuLoad() * 100;
        return new DecimalFormat("#0.00").format(load);
    }

    // A utility method to convert the byte array
    // data into a string representation.
    public static StringBuilder data(byte[] a)
    {
        if (a == null)
            return null;
        StringBuilder ret = new StringBuilder();
        int i = 0;
        while (a[i] != 0)
        {
            ret.append((char) a[i]);
            i++;
        }
        return ret;
    }

    private static int convertByteArrayToInt(byte[] data) {
        if (data == null || data.length != 4) return 0x0;
        // ----------
        return (int)( // NOTE: type cast not necessary for int
                (0xff & data[0]) << 24  |
                        (0xff & data[1]) << 16  |
                        (0xff & data[2]) << 8   |
                        (0xff & data[3]) << 0
        );
    }

    private static byte[] intToBytes(final int data) {
        return new byte[] {
                (byte)((data >> 24) & 0xff),
                (byte)((data >> 16) & 0xff),
                (byte)((data >> 8) & 0xff),
                (byte)((data >> 0) & 0xff),
        };
    }
}
