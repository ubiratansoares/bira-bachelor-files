package org.generator;

import java.util.*;
import java.io.*;

public class JavaBean {

	private String packageName;
	private String className;

	private Vector name;
	private Vector jtype;
	private Vector stype;

	public JavaBean(String packageName, String className) {
		this.packageName = packageName;
		this.className = className;
		this.name = new Vector();
		this.jtype = new Vector();
		this.stype = new Vector();
	}

	public void add(String name, String jtype, String stype) {
		this.name.add(name);
		this.jtype.add(jtype);
		this.stype.add(stype);
	}

	public void process() {
		StringBuffer sb = new StringBuffer();

		sb.append("package "+packageName+";\n\n");
		sb.append("public class "+className+" {\n");
		for (int i = 0; i < this.name.size(); i++) {
			sb.append("\tprivate "+((String) jtype.elementAt(i))+" "+((String) name.elementAt(i))+";\n");
		}

		// construtores
		sb.append("\n\tpublic "+className+"() {}\n");
		sb.append("\tpublic "+className+"(");
		for (int i = 0; i < this.name.size(); i++) {
			sb.append(((String) jtype.elementAt(i))+" "+((String) name.elementAt(i))+", ");
		}
		sb.append(") {\n");
		for (int i = 0; i < this.name.size(); i++) {
			sb.append("\t\tthis."+((String) name.elementAt(i))+" = "+((String) name.elementAt(i))+";\n");
		}
		sb.append("\n\t}\n\n");

		// sets
		for (int i = 0; i < this.name.size(); i++) {
			char first[] = new char[1];
		        first[0] = ((String) name.elementAt(i)).charAt(0);
			String firstLetter = new String(first);
			String rest = ((String) name.elementAt(i)).substring(1);
			sb.append("\tpublic void set"+firstLetter.toUpperCase()+rest+"("+((String) jtype.elementAt(i))+" "+
					((String) name.elementAt(i))+") {\n");
			sb.append("\t\tthis."+((String) name.elementAt(i))+" = "+((String) name.elementAt(i))+";\n");
			sb.append("\t}\n\n");
		}

		// gets
		for (int i = 0; i < this.name.size(); i++) {
			char first[] = new char[1];
			first[0] = ((String) name.elementAt(i)).charAt(0);
			String firstLetter = new String(first);
			String rest = ((String) name.elementAt(i)).substring(1);
			sb.append("\tpublic "+((String) jtype.elementAt(i))+" get"+firstLetter.toUpperCase()+rest+"() {\n");
			sb.append("\t\treturn this."+((String) name.elementAt(i))+";\n");
			sb.append("\t}\n\n");
		}

		// insert
		sb.append("\tpublic int insert() throws Exception {\n");
		sb.append("\t\tDBConnection db = DBFactory.getConnection();\n");
		sb.append("\t\tint ret = db.update(\"insert into _"+className+" (");
		for (int i = 0; i < this.name.size(); i++) {
			sb.append("_"+(String) name.elementAt(i));
			if (i < this.name.size() - 1) { sb.append(", "); }
		}
		sb.append(") values (");
		for (int i = 0; i < this.name.size(); i++) {
			if (((String) jtype.elementAt(i)).equals("String")) { sb.append("'"); }
			sb.append("\"+this."+((String) name.elementAt(i))+"+\"");
			if (((String) jtype.elementAt(i)).equals("String")) { sb.append("'"); }
			if (i < this.name.size() - 1) { sb.append(", "); }
		}
		sb.append(");\");\n");
		sb.append("\t\tDBFactory.closeConnection(db);\n");
		sb.append("\t\treturn ret;\n");
		sb.append("\t}\n\n");

		System.out.println(sb);
	}

	public static void main(String args[]) throws Exception {
	
		BufferedReader br = new BufferedReader(new InputStreamReader(System.in));

		System.out.print("Digite o nome do pacote: ");
		String packageName = br.readLine();
		System.out.print("Digite o nome da classe: ");
		String className = br.readLine();

		JavaBean jb = new JavaBean(packageName, className);

		while (true) {
			System.out.print("Digite o nome do argumento (ou 1 para sair): ");
			String name = br.readLine();

			if (name.equals("1")) {
				break;
			}

			System.out.print("Digite o tipo java do argumento: ");
			String jtype = br.readLine();
			System.out.print("Digite o tipo sql do argumento: ");
			String stype = br.readLine();
			jb.add(name, jtype, stype);
		}

		jb.process();
	}
}
