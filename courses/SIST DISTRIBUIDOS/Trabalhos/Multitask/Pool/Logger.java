import java.util.*;  // for Collection

public interface Logger {
        public void writeEntry(Collection entry); // write list of lines
        public void writeEntry(String entry);
}
