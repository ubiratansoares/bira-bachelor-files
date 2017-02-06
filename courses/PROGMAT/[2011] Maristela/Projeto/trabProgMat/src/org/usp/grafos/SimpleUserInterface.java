package org.usp.grafos;

import org.jgrapht.graph.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.io.File;
import java.io.IOException;
//import java.util.Set;
import javax.swing.*;


public class SimpleUserInterface implements Runnable {

	
	private JFrame frame;
	private JLabel fileLabel;
	private JButton openButton;
	private JFileChooser fc;
	private JTextArea out;
		
	//@Override
	public void run() {

		frame = new JFrame("TSP Closest Neighbor");
		
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
			
//		frame.setPreferredSize(new Dimension(300, 200));
		frame.setLocation(200, 200);
			
		fileLabel = new JLabel();
		openButton = new JButton(new OpenFileAction());
		fc = new JFileChooser(".");
		out = new JTextArea(16, 80);
			
		fc.setFileSelectionMode(JFileChooser.FILES_ONLY);
		fc.setMultiSelectionEnabled(false);
		fc.setFileHidingEnabled(true);
			
		out.setFont(new Font(Font.MONOSPACED, Font.PLAIN, 16));
		out.setEditable(false);
		out.setLineWrap(true);
		out.setWrapStyleWord(true);
			
		JPanel topPanel = new JPanel();
		topPanel.setLayout(new BoxLayout(topPanel, BoxLayout.LINE_AXIS));
		topPanel.add(Box.createHorizontalStrut(10));
		topPanel.add(fileLabel);
		topPanel.add(Box.createHorizontalGlue());
		topPanel.add(openButton);
			
		JScrollPane scroll = new JScrollPane(out,
				JScrollPane.VERTICAL_SCROLLBAR_ALWAYS, 
				JScrollPane.HORIZONTAL_SCROLLBAR_NEVER);
			
		frame.getContentPane().add(topPanel, BorderLayout.PAGE_START);
		frame.getContentPane().add(scroll, BorderLayout.CENTER);
			
		frame.pack();
		frame.setVisible(true);
		return;
	}
		
	public void launch() {
		SwingUtilities.invokeLater(this);
	}
		
	public void exit() {
		System.exit(0);
	}
		
	private class OpenFileAction extends AbstractAction {

		private static final long serialVersionUID = 1L;
			
		public OpenFileAction() {
				this.putValue(Action.NAME, "Abrir");
		}

		@Override
		public void actionPerformed(ActionEvent arg0) {
			//selecionando o arquivo
			File file = null;
			switch(fc.showOpenDialog(frame)) {
				
				case JFileChooser.APPROVE_OPTION:
					file = fc.getSelectedFile();
					break;
					
				case JFileChooser.CANCEL_OPTION:
				case JFileChooser.ERROR_OPTION:
					return;
			}
				
			//lendo o arquivo
			
			SimpleWeightedGraph<String, DefaultWeightedEdge> graph = null;
				
			try {
					fileLabel.setText(file.getCanonicalPath());
					
					graph = new GraphFileParser().read(file);
					
			} catch(IOException e) {
				out.setText("N‹o foi poss’vel ler o arquivo.");
				return;
			}
				
			if(graph == null) {
				out.setText("N‹o h‡ grafo.");
				return;
			
			} 
				
				//exibindo o grafo
				out.setText("Encontrado o grafo:\n\n\n");
				out.append(graph.toString());
				
				TSPMininumSpanningTree tspcn = new TSPMininumSpanningTree(graph);
				
				//out.append(tspcn.getPath());
				
				
				//calculando o resultado
				out.append(tspcn.getPath());	
				
				out.append("\n\n[fim]\n\n");

				return;
			}
			
		}
		
	
	
	
}
