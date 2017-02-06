package br.usp.icmc.swing;

import javax.swing.*;
import java.awt.*;
import java.awt.event.*;

public class Janela extends JFrame implements ActionListener {

	private JTextField jtf01;
	private StringBuffer buffer;
	private StringBuffer oldbuffer;
	private String op;

	public Janela(String title) {
		super(title);
		this.buffer = new StringBuffer();
		this.op = null;
	}

	public void init() {
		this.setBounds(0, 0, 180, 300);
		this.setLayout(new FlowLayout());

		jtf01 = new JTextField("0", 13);
		jtf01.setHorizontalAlignment(JTextField.RIGHT);
		
		JButton jb01 = new JButton("1");
		JButton jb02 = new JButton("2");
		JButton jb03 = new JButton("3");
		JButton jb04 = new JButton("4");
		JButton jb05 = new JButton("5");
		JButton jb06 = new JButton("6");
		JButton jb07 = new JButton("7");
		JButton jb08 = new JButton("8");
		JButton jb09 = new JButton("9");
		JButton jb10 = new JButton("0");
		JButton jb11 = new JButton("+");
		JButton jb12 = new JButton("-");
		JButton jb13 = new JButton("*");
		JButton jb14 = new JButton("/");
		JButton jb15 = new JButton("=");
		JButton jb16 = new JButton("C");
		JButton jb17 = new JButton("E");

		jb01.addActionListener(this);
		jb02.addActionListener(this);
		jb03.addActionListener(this);
		jb04.addActionListener(this);
		jb05.addActionListener(this);
		jb06.addActionListener(this);
		jb07.addActionListener(this);
		jb08.addActionListener(this);
		jb09.addActionListener(this);
		jb10.addActionListener(this);
		jb11.addActionListener(this);
		jb12.addActionListener(this);
		jb13.addActionListener(this);
		jb14.addActionListener(this);
		jb15.addActionListener(this);
		jb16.addActionListener(this);
		jb17.addActionListener(this);
                    
		this.add(jtf01);
		this.add(jb01);
		this.add(jb02);
		this.add(jb03);
		this.add(jb04);
		this.add(jb05);
		this.add(jb06);
		this.add(jb07);
		this.add(jb08);
		this.add(jb09);
		this.add(jb10);
		this.add(jb11);
		this.add(jb12);
		this.add(jb13);
		this.add(jb14);
		this.add(jb15);
		this.add(jb16);
		this.add(jb17);
	}

	public void actionPerformed(ActionEvent e) {
		if (e.getActionCommand().equals("1") || 
		e.getActionCommand().equals("2") ||
		e.getActionCommand().equals("3") ||
		e.getActionCommand().equals("4") ||
		e.getActionCommand().equals("5") ||
		e.getActionCommand().equals("6") ||
		e.getActionCommand().equals("7") ||
		e.getActionCommand().equals("8") ||
		e.getActionCommand().equals("9") ||
		e.getActionCommand().equals("0")) {
			
			buffer.append(e.getActionCommand());
			jtf01.setText(buffer.toString());

		} else if (e.getActionCommand().equals("+") ||
		e.getActionCommand().equals("-") ||
		e.getActionCommand().equals("/") ||
		e.getActionCommand().equals("*")) {
			this.oldbuffer = new StringBuffer(buffer.toString());
			//this.oldbuffer = buffer;
			this.op = e.getActionCommand();
			this.buffer.setLength(0);
		} else if (e.getActionCommand().equals("=")) {
			String old = this.oldbuffer.toString();
			String current = this.buffer.toString();
			double oldv = Double.parseDouble(old);
			double currv = Double.parseDouble(current);

			this.buffer.setLength(0);
			Double result = null;

			if (this.op.equals("+")) {
				result = new Double(oldv + currv);
			} else if (this.op.equals("-")) {
				result = new Double(oldv - currv);
			} else if (this.op.equals("/")) {
				result = new Double(oldv / currv);
			} else if (this.op.equals("*")) {
				result = new Double(oldv * currv);
			}

			this.buffer.append(result.toString());
			jtf01.setText(this.buffer.toString());

		} else if (e.getActionCommand().equals("C")) {
			this.buffer.setLength(0);
			this.buffer.append("0");
			this.oldbuffer = new StringBuffer();
			this.op = null;

			jtf01.setText(this.buffer.toString());
		} else if (e.getActionCommand().equals("E")) {
			this.dispose();
		}
	}

	public static void main(String args[]) {
		Janela j = new Janela("Calculadora");
		j.init();
		j.show();
	}
}
