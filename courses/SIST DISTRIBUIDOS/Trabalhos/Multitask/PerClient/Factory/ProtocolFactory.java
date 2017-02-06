import java.net.*; // for Socket

public interface ProtocolFactory {
        public Runnable createProtocol (Socket clntSock, Logger logger);
}
