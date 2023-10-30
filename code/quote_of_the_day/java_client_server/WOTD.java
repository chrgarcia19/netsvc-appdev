import java.io.*;
import java.net.InetAddress;
import java.net.ServerSocket;
import java.net.Socket;
import java.net.UnknownHostException;
import java.util.*;
import java.util.stream.Stream;

public class WOTD {
    static InetAddress address;
    static int port = 4200;
    static HashMap<Integer, String[]> quotes = new HashMap<>();
    static String[] qotd = new String[3];

    private static final char DEFAULT_SEPARATOR = ',';
    private static final char DOUBLE_QUOTES = '"';
    private static final char DEFAULT_QUOTE_CHAR = DOUBLE_QUOTES;
    private static final String NEW_LINE = "\n";

    private static boolean isMultiLine = false;
    private static String pendingField = "";
    private static String[] pendingFieldLine = new String[]{};

    public WOTD() {
    }

    public static void main(String[] args) {
        try {
            File file = new File("../quotes.csv");
            List<String[]> result = readFile(file);
            int listIndex = 0;
            for (String[] arrays : result) {
                quotes.put(listIndex, arrays);
                listIndex++;
            }
        } catch (IOException e) {
            throw new RuntimeException(e);
        } catch (Exception e) {
            throw new RuntimeException(e);
        }

        Random var2 = new Random();
        int var3 = var2.nextInt(1660);
        qotd = quotes.get(var3);

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
                ServerMain();
            } else {
                Client();
            }

        } catch (IOException e) {
            System.out.println(e);
        }
    }

    public static void Client() {
        try {
            Socket socket = new Socket(address, port);
            System.out.println("Connected");
            DataInputStream input = new DataInputStream(socket.getInputStream());
            DataOutputStream output = new DataOutputStream(socket.getOutputStream());

            String response = "";
            while (!response.equals("E")) {
                try {
                    System.out.println("\nPrivde an input to the server:" +
                            "\n Q - Request the Quote" +
                            "\n D - Request the date of the quote" +
                            "\n A - Request the author of the quote" +
                            "\n R - Request Everything" +
                            "\n E - Exit Program" +
                            "\n\t");
                    response = System.console().readLine();
                    switch (response) {
                        case "Q" -> {
                            output.writeUTF("Q");
                            System.out.println("\nWaiting for Quote...\n");
                        }
                        case "D" -> {
                            output.writeUTF("D");
                            System.out.println("\nWaiting for Date...\n");
                        }
                        case "A" -> {
                            output.writeUTF("A");
                            System.out.println("\nWaiting for Author...\n");
                        }
                        case "R" -> {
                            output.writeUTF("R");
                            System.out.println("\nWaiting for Response...\n");
                        }
                        case "E" -> {
                            output.writeUTF("E");
                            System.out.println("\nTerminating program...\n");
                            continue;
                        }
                        default -> System.out.println("Invalid Input, Try Again!");
                    }
                    String result = input.readUTF();
                    System.out.println(result);
                } catch (IOException var4) {
                    System.out.println(var4);
                }
            }

            try {
                input.close();
                output.close();
                socket.close();
            } catch (IOException e) {
                System.out.println(e);
            }
        } catch (IOException e) {
            System.out.println(e);
        }
    }

    public static void ServerMain() {
        int curr = 0;
        try {
            ServerSocket server = new ServerSocket(port);
            while (curr <= 2) {

                System.out.println("[MAIN] Server started");
                System.out.println("[MAIN] Waiting for a client ...");
                Socket socket = server.accept();
                System.out.println("[MAIN] Client accepted");
                ServerWorker(socket);
                curr++;
            }
        } catch (IOException var5) {
            System.out.println(var5);
        }
    }


    public static void ServerWorker(Socket localSocket) {
        new Thread(() -> {
            try {
                DataInputStream input = new DataInputStream(localSocket.getInputStream());
                DataOutputStream output = new DataOutputStream(localSocket.getOutputStream());
                System.out.println("[Worker " + localSocket.getPort() + "] Created New Server Thread");

                String response = "";

                while (!response.equals("E")) {
                    try {
                        System.out.println("[Worker " + localSocket.getPort() + "] Waiting for Client Request");
                        response = input.readUTF();
                        System.out.println("[Worker " + localSocket.getPort() + "] Received value: " + response);
                        switch (response) {
                            case "Q" -> {
                                output.writeUTF(qotd[1]);
                                System.out.println("[Worker " + localSocket.getPort() + "] Sent Quote");
                            }
                            case "D" -> {
                                output.writeUTF(qotd[2]);
                                System.out.println("[Worker " + localSocket.getPort() + "] Sent Date");
                            }
                            case "A" -> {
                                output.writeUTF(qotd[0]);
                                System.out.println("[Worker " + localSocket.getPort() + "] Sent Author");
                            }
                            case "R" -> {
                                output.writeUTF("Quote: " + qotd[1] + " Author: " + qotd[0] + " Date: " + qotd[2]);
                                System.out.println("[Worker " + localSocket.getPort() + "] Sent Response");
                            }
                            case "E" -> {
                                System.out.println("\n[Worker " + localSocket.getPort() + "] Terminating program...\n");
                            }
                            default -> System.out.println("[Worker " + localSocket.getPort() + "] Invalid Input!");
                        }
                    } catch (IOException var4) {
                        System.out.println(var4);
                    }
                }
                System.out.println("[Worker] Closing connection");
                localSocket.close();
                input.close();
                output.close();
            } catch (IOException e) {
                throw new RuntimeException(e);
            }
        }).start();
    }

    public static List<String[]> readFile(File csvFile) throws Exception {
        return readFile(csvFile, 0);
    }

    public static List<String[]> readFile(File csvFile, int skipLine) throws Exception {

        List<String[]> result = new ArrayList<>();
        int indexLine = 1;

        try (BufferedReader br = new BufferedReader(new FileReader(csvFile))) {

            String line;
            while ((line = br.readLine()) != null) {

                if (indexLine++ <= skipLine) {
                    continue;
                }

                String[] csvLineInArray = parseLine(line);

                if (isMultiLine) {
                    pendingFieldLine = joinArrays(pendingFieldLine, csvLineInArray);
                } else {

                    if (pendingFieldLine != null && pendingFieldLine.length > 0) {
                        // joins all fields and add to list
                        result.add(joinArrays(pendingFieldLine, csvLineInArray));
                        pendingFieldLine = new String[]{};
                    } else {
                        // don't want to support multiline, only this line is required.
                        result.add(csvLineInArray);
                    }

                }
            }
        }

        return result;
    }

    public static String[] parseLine(String line) throws Exception {
        return parseLine(line, DEFAULT_SEPARATOR);
    }

    public static String[] parseLine(String line, char separator) throws Exception {
        return parse(line, separator, DEFAULT_QUOTE_CHAR).toArray(String[]::new);
    }

    private static List<String> parse(String line, char separator, char quoteChar) throws Exception {

        List<String> result = new ArrayList<>();

        boolean inQuotes = false;
        boolean isFieldWithEmbeddedDoubleQuotes = false;

        StringBuilder field = new StringBuilder();

        for (char c : line.toCharArray()) {

            if (c == DOUBLE_QUOTES) {               // handle embedded double quotes ""
                if (isFieldWithEmbeddedDoubleQuotes) {

                    if (field.length() > 0) {       // handle for empty field like "",""
                        field.append(DOUBLE_QUOTES);
                        isFieldWithEmbeddedDoubleQuotes = false;
                    }

                } else {
                    isFieldWithEmbeddedDoubleQuotes = true;
                }
            } else {
                isFieldWithEmbeddedDoubleQuotes = false;
            }

            if (isMultiLine) {                      // multiline, add pending from the previous field
                field.append(pendingField).append(NEW_LINE);
                pendingField = "";
                inQuotes = true;
                isMultiLine = false;
            }

            if (c == quoteChar) {
                inQuotes = !inQuotes;
            } else {
                if (c == separator && !inQuotes) {  // if find separator and not in quotes, add field to the list
                    result.add(field.toString());
                    field.setLength(0);             // empty the field and ready for the next
                } else {
                    field.append(c);                // else append the char into a field
                }
            }

        }

        //line done, what to do next?
        if (inQuotes) {
            pendingField = field.toString();        // multiline
            isMultiLine = true;
        } else {
            result.add(field.toString());           // this is the last field
        }

        return result;

    }

    private static String[] joinArrays(String[] array1, String[] array2) {
        return Stream.concat(Arrays.stream(array1), Arrays.stream(array2)).toArray(String[]::new);
    }
}


