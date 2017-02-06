import java.net.*; // for Socket, ServerSocket, and InetAddress
import java.io.*; // for IOException and Input/OutputStream

public class TCPEchoServerThread {
        
        public static void main (String[] args) throws IOException {
                if (args.length != 1)
                        throw new IllegalArgumentException ("Parameter(s): <Port>");
                
                int echoServPort = Integer.parseInt(args[0]); // Server port

                // Create a server socket to accept client connection requests
                ServerSocket servSock = new ServerSocket(echoServPort);
                
                Logger logger = new ConsoleLogger(); // Log messages to console
                
                // Run forever, accepting and spawing threads to service each connection
                for (;;) { 
                        try {
                                Socket clntSock = servSock.accept(); // Block waiting for connection
                                
                                EchoProtocol protocol = new EchoProtocol(clntSock, logger);
                                Thread thread = new Thread(protocol);
                                thread.start();
                                logger.writeEntry("Created and started Thread = " +
                                  thread.getName());
                        } catch (IOException e) {
                                logger.writeEntry("Exception = " + e.getMessage());
                        }
                }
                /* NOT REACHED */
        }
}                