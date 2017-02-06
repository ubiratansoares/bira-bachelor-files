import java.net.*;  // for Socket

public class EchoProtocolFactory implements ProtocolFactory {
        public Runnable createProtocol(Socket clntSock, Logger logger) {
                return new EchoProtocol(clntSock, logger);
        }
}
