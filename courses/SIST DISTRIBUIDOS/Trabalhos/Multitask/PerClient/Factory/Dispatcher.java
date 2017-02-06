import java.net.*; // for Socket

public interface Dispatcher {
        public void startDispatching(ServerSocket servSock, Logger logger,
          ProtocolFactory protoFactory);
}
