package compiler;

import java.util.HashSet;
import java.util.Set;

/**
 * Tabela de palavras reservadas da LALG.
 *
 */
public class KeywordsTable {

	private Set<String> table;

	/**
	 * Cria uma tabela vazia.
	 */
	public KeywordsTable() {
		table = new HashSet<String>();
	}

	/**
	 * Preenche a tabela com as palavras e os símbolos reservados.
	 */
	public void init() {
		table.clear();

		//1. <programa> ::= program ident ; <corpo> .
		table.add("program");

		//2. <corpo> ::= <dc> begin <comandos> end
		table.add("begin");
		table.add("end");

		//3. <dc> ::= <dc_v> <dc_p>
		//4. <dc_v> ::= var <variaveis> : <tipo_var> ; <dc_v> | λ
		table.add("var");

		//5. <tipo_var> ::= real | integer
		table.add("real");
		table.add("integer");

		//6. <variaveis> ::= ident <mais_var>
		//7. <mais_var> ::= , <variaveis> | λ
		//8. <dc_p> ::= procedure ident <parametros> ; <corpo_p> <dc_p> | λ
		table.add("procedure");

		//9. <parametros> ::= ( <lista_par> ) | λ
		//10. <lista_par> ::= <variaveis> : <tipo_var> <mais_par>
		//11. <mais_par> ::= ; <lista_par> | λ
		//12. <corpo_p> ::= <dc_loc> begin <comandos> end ;
		//13. <dc_loc> ::= <dc_v>
		//14. <lista_arg> ::= ( <argumentos> ) | λ
		//15. <argumentos> ::= ident <mais_ident>
		//16. <mais_ident> ::= ; <argumentos> | λ

		//17. <pfalsa> ::= else <cmd> | λ
		table.add("else");

		//18. <comandos> ::= <cmd> ; <comandos> | λ
		/* 19. <cmd> ::= read ( <variaveis> ) |
		 *		write ( <variaveis> ) |
		 *		while <condicao> do <cmd> |
		 * 		if <condicao> then <cmd> <pfalsa> |
		 * 		ident := <expressao> |
		 * 		ident <lista_arg> |
		 * 		begin <comandos> end |
		 * 		repeat <comandos> until <condicao>
		 */
		table.add("readln");
		table.add("writeln");
		table.add("while");
		table.add("do");
		table.add("if");
		table.add("then");
		table.add("repeat");
		table.add("until");

		//20. <condicao> ::= <expressao> <relacao> <expressao>
		//21. <relacao> ::= = | <> | >= | <= | > | <
		//22. <expressao> ::= <termo> <outros_termos>
		//23. <op_un> ::= + | - | λ
		//24. <outros_termos> ::= <op_ad> <termo> <outros_termos> | λ
		//25. <op_ad> ::= + | -
		//26. <termo> ::= <op_un> <fator> <mais_fatores>
		//27. <mais_fatores> ::= <op_mul> <fator> <mais_fatores> | λ
		//28. <op_mul> ::= * | /
		//29. <fator> ::= ident | numero_int | numero_real | ( <expressao> )
		return;
	}

	/**
	 * Verifica se a cadeia está na tabela.
	 *
	 * @param string
	 * A cadeia a ser verificada.
	 *
	 * @return
	 * {@code true} caso a cadeia esteja presente na tabela.
	 */
	public boolean contains(String string) {
		return table.contains(string);
	}

}
