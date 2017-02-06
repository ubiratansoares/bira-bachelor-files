import java.util.*;
public class LittleWindow {
	private Vector v;
	public LittleWindow() { v = new Vector(); }
	public void addConnection(NetworkingConnection nc) { v.add(nc); }
	public void restart() {
		for (int i = 0; i < v.size(); i++) {
			NetworkingConnection nc = 
				(NetworkingConnection) v.elementAt(i);
			nc.disconnect();
			nc.connect();
		}
	}
	public static void main(String args[]) {
		LittleWindow lw = new LittleWindow();
		/*DialUp up = new DialUp();
		lw.addConnection(up);
		lw.addConnection(new Lan());*/
		lw.addConnection(new ADSL());
		lw.restart();
	}
}
