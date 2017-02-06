import java.util.*;  // for Collection and Iterator

class ConsoleLogger implements Logger {
        public synchronized void writeEntry(Collection entry) {
                for (Iterator line = entry.iterator(); line.hasNext();)
                        System.out.println(line.next());
                System.out.println();
        }
        
        public synchronized void writeEntry(String entry) {
                System.out.println(entry);
                System.out.println();
        }
}