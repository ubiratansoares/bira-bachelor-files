package org.usp.grafos;

import org.jgrapht.graph.*;

import java.awt.*;
import java.awt.event.ActionEvent;
import java.io.File;
import java.io.IOException;
//import java.util.Set;
import javax.swing.*;
import java.util.*;
import java.text.SimpleDateFormat;


public class SimpleUserInterface implements Runnable {

	
	private JFrame frame;
	private JLabel fileLabel;
	private JButton openButton;
	private JFileChooser fc;
	private JTextArea out;
		
	//@Override
	public void run() {
		

		frame = new JFrame("Average Clustering Coefficient and Diameter");
		
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
			
			Pseudograph<String, DefaultEdge>graph = null;
				
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
			
				String DATE_FORMAT_NOW = "HH:mm:ss";
				
			    Calendar cal1 = Calendar.getInstance();
			    
			    SimpleDateFormat sdf = new SimpleDateFormat(DATE_FORMAT_NOW);
				
				//exibindo o grafo
				out.setText("START AT " + sdf.format(cal1.getTime()));
				
				out.append("\n\nFOUNDED GRAPH.");
				
				//out.append(graph.toString());
				
				ClusteringCoefficient cc = new ClusteringCoefficient(graph);			
				
				//calculando o resultado
				out.append("\n\nDIAMETER = " + cc.getDiameter() +"\n\n");
				
				out.append("\n\nAVERAGE CLUSTERING COEFFICIENT = " + cc.getAverageClusteringCoefficient());
				
			    Calendar cal2 = Calendar.getInstance();
				
				out.append("\n\nEND AT " + sdf.format(cal2.getTime()));
				
				out.append("\n\n[fim]\n\n");

				return;
			}
			
		}
		
	
	
	
}
