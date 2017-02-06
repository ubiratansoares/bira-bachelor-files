import java.io.*;    // for PrintWriter and FileWriter
import java.util.*;  // for Collection and Iterator

class FileLogger implements Logger {
        
        PrintWriter out; // Log file
        
        public FileLogger(String filename) throws IOException {
                out = new PrintWriter(new FileWriter(filename), true); // Create log file
        }
        
        public synchronized void writeEntry(Collection entry) {
                for (Iterator line = entry.iterator(); line.hasNext();)
                        out.println(line.next());
                out.println();
        }
        
        public synchronized void writeEntry(String entry) {
                out.println(entry);
                out.println();
        }
}