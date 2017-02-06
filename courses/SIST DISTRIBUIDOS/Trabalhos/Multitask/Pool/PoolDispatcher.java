import java.net.*; // for Socket and ServerSocket
import java.io.*;  // for IOException

class PoolDispatcher implements Dispatcher {
        
        static final String NUMTHREADS = "8";  // Default thread-pool size
        static final String THREADPROP = "Threads"; // Name of thread property
        
        private int numThreads;  // Number of threads in pool
        
        public PoolDispatcher() {
                // Get the number of threads from the System properties or take the default
                numThreads = Integer.parseInt(System.getProperty(THREADPROP, NUMTHREADS));
        }
        
        public void startDispatching(final ServerSocket servSock, final Logger logger,
                                    final ProtocolFactory protoFactory) {
                // Create N-1 threads, each running an iterative server
                for (int i=0; i < (numThreads - 1); i++) {
                        Thread thread = new Thread() {
                                public void run() {
                                        dispatchLoop(servSock, logger, protoFactory);
                                }
                        };
                        thread.start();
                        logger.writeEntry("Created and started Thread = " + thread.getName());
                }
                logger.writeEntry("Iterative server starting in main thread " +
                  Thread.currentThread().getName());
                // Use main thread as Nth iterative server
                dispatchLoop(servSock, logger, protoFactory);
                /* NOT REACHEAD */
        }
        
        private void dispatchLoop(ServerSocket servSock, Logger logger,
                                  ProtocolFactory protoFactory) {
                // Run forever, accepting and handling each connection
                for (;;) {
                        try {
                                Socket clntSock = servSock.accept(); // Block waiting for connection
                                Runnable protocol = protoFactory.createProtocol(clntSock, logger);
                                protocol.run();
                        } catch (IOException e) {
                                logger.writeEntry("Exception = " + e.getMessage());
                        }
                }
        }
}
                
                