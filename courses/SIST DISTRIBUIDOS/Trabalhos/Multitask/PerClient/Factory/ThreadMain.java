import java.net.*; // for ServerSocket
import java.io.*; // for IOException

public class ThreadMain {
        
        public static void main(String[] args) throws Exception {
                
                if (args.length != 3)
                        throw new IllegalArgumentException("Parameter(s) [<Optional properties>]" +
                          " <Port> <Protocol> <Dispatcher>");

                int servPort = Integer.parseInt(args[0]);
                String protocolName = args[1];
                String dispatcherName = args[2];
                
                ServerSocket servSock = new ServerSocket(servPort);
                Logger logger = new ConsoleLogger();
                
                ProtocolFactory protoFactory = (ProtocolFactory) Class.forName(protocolName +
                  "ProtocolFactory").newInstance();
                  
                Dispatcher dispatcher = (Dispatcher) 
                  Class.forName(dispatcherName + "Dispatcher").newInstance();
                
                dispatcher.startDispatching(servSock, logger, protoFactory);

                /* NOT REACHED */
        }
}                