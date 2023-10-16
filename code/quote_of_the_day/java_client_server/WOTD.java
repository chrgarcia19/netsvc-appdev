import java.io.*;
import java.net.InetAddress;
import java.net.ServerSocket;
import java.net.Socket;
import java.net.UnknownHostException;
import java.util.*;
import java.util.stream.Stream;

public class WOTD {
    static Socket socket;
    static DataInputStream input;
    static DataOutputStream output;
    static InetAddress address;
    static int port = 4200;
    static HashMap<Integer, String[]> quotes = new HashMap<>();

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

        } catch (IOException e) {
            System.out.println(e);
        }
    }

    public static void Client() {
        try {
            socket = new Socket(address, port);
            System.out.println("Connected");
            input = new DataInputStream(socket.getInputStream());
            output = new DataOutputStream(socket.getOutputStream());
        } catch (UnknownHostException e) {
            System.out.println(e);
            return;
        } catch (IOException e) {
            System.out.println(e);
            return;
        }

        try {
            output.writeUTF("Send Quote");
            String var1 = "";
            while (!var1.equals("Over")) {
                try {
                    System.out.println("Waiting for Quote...");
                    var1 = input.readUTF();
                    System.out.println(var1);
                    var1 = "Over";
                    output.writeUTF(var1);
                } catch (IOException var4) {
                    System.out.println(var4);
                }
            }
        } catch (IOException var2) {
            throw new RuntimeException(var2);
        }

        try {
            input.close();
            output.close();
            socket.close();
        } catch (IOException e) {
            System.out.println(e);
        }

    }

    public static void Server() {
        try {
            ServerSocket var0 = new ServerSocket(port);
            System.out.println("Server started");
            System.out.println("Waiting for a client ...");
            socket = var0.accept();
            System.out.println("Client accepted");
            input = new DataInputStream(socket.getInputStream());
            output = new DataOutputStream(socket.getOutputStream());
            String var1 = "";

            while (!var1.equals("Over")) {
                try {
                    var1 = input.readUTF();
                    System.out.println(var1);
                    if (var1.equals("Send Quote")) {
                        Random var2 = new Random();
                        int var3 = var2.nextInt(1660);
                        String quote = ((String[]) quotes.get(var3))[1];
                        System.out.println(quote);
                        output.writeUTF(quote);
                    }
                } catch (IOException var4) {
                    System.out.println(var4);
                }
            }

            System.out.println("Closing connection");
            socket.close();
            input.close();
        } catch (IOException var5) {
            System.out.println(var5);
        }

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


