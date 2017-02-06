package br.usp.icmc.janela;

import java.awt.*;
import java.awt.event.*;

class Dialogo extends Frame implements ActionListener {
	private String label;

	public Dialogo() {
		super();
	}

	public Dialogo(String title) {
		super(title);
	}

	public Dialogo(String title, String label) {
		super(title);
		this.label = label;
	}

	public void actionPerformed(ActionEvent e) {
		this.dispose();
	}

	public void init() {
		this.setBounds(100, 100, 300, 100);
		this.setLayout(new FlowLayout());
		this.add(new Label(this.label));
		Button b1 = new Button("Fechar");
		b1.addActionListener(this);
		this.add(b1);
	}
}

public class Janela extends Frame implements ActionListener {
	public Janela() {
		super();
	}

	public Janela(String title) {
		super(title);
	}

	public void actionPerformed(ActionEvent e) {
		if (e.getActionCommand().equals("1")) {
			Dialogo d = new Dialogo("Caixa de Dialogo", "oi");
			d.init();
			d.show();
		} else if (e.getActionCommand().equals("2")) {

		} else if (e.getActionCommand().equals("3")) {

		}
	}

	public void init() {
		this.setBounds(0, 0, 400, 300);
		//this.setLayout(new GridLayout(3,2));
		this.setLayout(new FlowLayout());

		this.add(new Label("Calculadora"));
		this.add(new TextField("teste", 20));
		this.add(new TextArea("Hello", 5, 40));

		Button b1 = new Button("1");
		Button b2 = new Button("2");
		Button b3 = new Button("3");

		b1.setSize(50, 28);
		b2.setSize(50, 28);
		b3.setSize(50, 28);

		b1.addActionListener(this);
		b2.addActionListener(this);
		b3.addActionListener(this);

		this.add(b1);
		this.add(b2);
		this.add(b3);
	}

	public static void main(String args[]) {
		Janela j = new Janela("Titulo para a Janela");
		j.init();
		j.show();
	}

}
