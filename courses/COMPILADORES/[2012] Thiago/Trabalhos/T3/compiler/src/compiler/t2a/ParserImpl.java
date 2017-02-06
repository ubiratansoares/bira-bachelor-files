package compiler.t2a;

import compiler.*;
import java.util.Collections;
import java.util.List;

/**
 * Analisador sintático da linguagem LALG.
 *
 * @version 2.0.2
 */
public class ParserImpl implements SyntacticAnalyzer {

	private LexicalAnalyzer lexer;
	private Token token;

	/**
	 * Construtor vazio.
	 */
	public ParserImpl() {
	}

	/**
	 * Define o analisador léxico usado por este analisador.
	 *
	 * @param lexer
	 * O analisador léxico a ser usado.
	 */
	public void setLexer(LexicalAnalyzer lexer) {
		this.lexer = lexer;
		return;
	}

	/**
	 * Efetua a análise sintática do código-fonte. A análise é
	 * descendente, preditiva e recursiva.
	 *
	 * @param source
	 * O código-fonte do programa a ser analisado.
	 *
	 * @return
	 * {@code null}, caso a análise termine bem-sucedida.
	 *
	 * @throws CompilerException
	 * Lançada para indicar qualquer erro de compilação.
	 */
	public ObjectCode parse(SourceCode source) throws CompilerException {
		lexer.setSourceCode(source);
		token = lexer.next();
		programa();
		terminal(TokenType.EOF);
		return null; //fim de arquivo esperado
	}

	//avalia um símbolo terminal
	private void terminal(TokenType expectedType) throws CompilerException {
		if(token.getType() == expectedType) {
			token = lexer.next();
		} else if(token.getType() == TokenType.EOF) {
			throw new SyntacticException(token,
					"Não era esperado o fim do arquivo.");
		} else {
			String message = null;
			switch(expectedType) {

				//tokens genéricos
				case IDENTIFIER:
					message = "Era esperado um identificador.";
					break;

				case CONST_INTEGER:
					message = "Era esperada uma constante inteira.";
					break;

				case CONST_REAL:
					message = "Era esperada uma constante real.";
					break;

				//tokens de símbolos especiais
				case SYMBOL_COLON:		case SYMBOL_PERIOD:
				case SYMBOL_SEMICOLON:	case SYMBOL_COMMA:
				case SYMBOL_LPAR:		case SYMBOL_RPAR:
				case SYMBOL_ASSIGN:
				case SYMBOL_EQ:			case SYMBOL_NE:
				case SYMBOL_GE:			case SYMBOL_LE:
				case SYMBOL_GT:			case SYMBOL_LT:
				case SYMBOL_PLUS:		case SYMBOL_MINUS:
				case SYMBOL_MULT:		case SYMBOL_DIV:
					message = "Era esperado o símbolo \"" + 
							expectedType + "\".";
					break;
				
				//tokens de palavras reservadas
				case KEYWORD_PROGRAM:	case KEYWORD_PROCEDURE:
				case KEYWORD_VAR:
				case KEYWORD_BEGIN:		case KEYWORD_END:
				case KEYWORD_IF:		case KEYWORD_THEN:
				case KEYWORD_ELSE:
				case KEYWORD_WHILE:		case KEYWORD_DO:
				case KEYWORD_REPEAT:	case KEYWORD_UNTIL:
				case KEYWORD_READ:		case KEYWORD_WRITE:
				case KEYWORD_INTEGER:	case KEYWORD_REAL:
					message = "Era esperada a palavra reservada \"" +
							expectedType + "\".";
					break;

				//fim de arquivo
				case EOF:
					message = "Era esperado o fim do arquivo.";
					break;

				//outros
				case ERROR:
				default:
					message = "Desconhecido.";
			}
			throw new SyntacticException(token, message);
		}
	}

	//avalia um símbolo terminal opcional
	private boolean optional(TokenType expectedType) throws CompilerException {
		if(token.getType() == expectedType) {
			token = lexer.next();
			return true;
		} else
			return false;
	}

	// 1. <programa> ::= program ident ; <corpo> .
	private void programa() throws CompilerException {
		terminal(TokenType.KEYWORD_PROGRAM);
		terminal(TokenType.IDENTIFIER);
		terminal(TokenType.SYMBOL_SEMICOLON);
		corpo();
		terminal(TokenType.SYMBOL_PERIOD);
		return;
	}

	// 2. <corpo> ::= <dc> begin <comandos> end
	private void corpo() throws CompilerException {
		dc();
		terminal(TokenType.KEYWORD_BEGIN);
		comandos();
		terminal(TokenType.KEYWORD_END);
		return;
	}

	// 3. <dc> ::= <dc_v> <dc_p>
	private void dc() throws CompilerException {
		dc_v();
		dc_p();
		return;
	}

	// 4. <dc_v> ::= var <variaveis> : <tipo_var> ; <dc_v> | λ
	private void dc_v() throws CompilerException {
		// Produção substituída por (na notação de Wirth):
		// DC_V = {"var" VARIAVEIS ":" TIPO_VAR ";"} .
		while(optional(TokenType.KEYWORD_VAR)) {
			variaveis();
			terminal(TokenType.SYMBOL_COLON);
			tipo_var();
			terminal(TokenType.SYMBOL_SEMICOLON);
		}
		return;
	}

	// 5. <tipo_var> ::= real | integer
	private void tipo_var() throws CompilerException {
		switch(token.getType()) {
			case KEYWORD_INTEGER:
			case KEYWORD_REAL:
				token = lexer.next();
				return;
			default:
				throw new SyntacticException(token,
						"Era esperado um tipo de variável.");
		}
	}

	// 6. <variaveis> ::= ident <mais_var>
	// 7. <mais_var> ::= , <variaveis> | λ
	private void variaveis() throws CompilerException {
		// Produções substituídas por (na notação de Wirth):
		// VARIAVEIS = "ident" {"," "ident"} .
		terminal(TokenType.IDENTIFIER);
		while(optional(TokenType.SYMBOL_COMMA))
			terminal(TokenType.IDENTIFIER);
		return;
	}

	// 8. <dc_p> ::= procedure ident <parametros> ; <corpo_p> <dc_p> | λ
	private void dc_p() throws CompilerException {
		//Produção substituída por (na notação de Wirth):
		//DC_P = {"procedure" "ident" PARAMETROS ";" CORPO_P} .
		while(optional(TokenType.KEYWORD_PROCEDURE)) {
			terminal(TokenType.IDENTIFIER);
			parametros();
			terminal(TokenType.SYMBOL_SEMICOLON);
			corpo_p();
		}
		return;
	}

	// 9. <parametros> ::= ( <lista_par> ) | λ
	private void parametros() throws CompilerException {
		if(optional(TokenType.SYMBOL_LPAR)) {
			lista_par();
			terminal(TokenType.SYMBOL_RPAR);
		}
		return;
	}

	//10. <lista_par> ::= <variaveis> : <tipo_var> <mais_par>
	//11. <mais_par> ::= ; <lista_par> | λ
	private void lista_par() throws CompilerException {
		//Produções substituídas por (na notação de Wirth):
		//LISTA_PAR = VARIAVEIS ":" TIPO_VAR {";" VARIAVEIS ":" TIPO_VAR} .
		variaveis();
		terminal(TokenType.SYMBOL_COLON);
		tipo_var();
		while(optional(TokenType.SYMBOL_SEMICOLON)) {
			variaveis();
			terminal(TokenType.SYMBOL_COLON);
			tipo_var();
		}
		return;
	}

	//12. <corpo_p> ::= <dc_loc> begin <comandos> end ;
	//13. <dc_loc> ::= <dc_v>
	private void corpo_p() throws CompilerException {
		//Produções substituídas por (na notação de Wirth):
		//CORPO_P = DC_V "begin" COMANDOS "end" ";" .
		dc_v();
		terminal(TokenType.KEYWORD_BEGIN);
		comandos();
		terminal(TokenType.KEYWORD_END);
		terminal(TokenType.SYMBOL_SEMICOLON);
		return;
	}

	//14. <lista_arg> ::= ( <argumentos> ) | λ
	//30. <pos_ident> ::= := <expressao> | <lista_arg>
	private void pos_ident() throws CompilerException {
		//Produções substituídas por (na notação de Wirth):
		//POS_IDENT = ":=" EXPRESSAO | "(" ARGUMENTOS ")" | .
		switch(token.getType()) {

			case SYMBOL_ASSIGN:
				token = lexer.next();
				expressao();
				return;

			case SYMBOL_LPAR:
				token = lexer.next();
				argumentos();
				terminal(TokenType.SYMBOL_RPAR);
				return;

			default:
				//procedimento sem argumentos
				return;
		}
	}

	//15. <argumentos> ::= ident <mais_ident>
	//16. <mais_ident> ::= ; <argumentos> | λ
	private void argumentos() throws CompilerException {
		//Produções substituídas por (na notação de Wirth):
		//ARGUMENTOS = "ident" {";" "ident"} .
		terminal(TokenType.IDENTIFIER);
		while(optional(TokenType.SYMBOL_SEMICOLON))
			terminal(TokenType.IDENTIFIER);
		return;
	}

	//17. <pfalsa> ::= else <cmd> | λ
	private void pfalsa() throws CompilerException {
		if(optional(TokenType.KEYWORD_ELSE))
			cmd();
		return;
	}

	//18. <comandos> ::= <cmd> ; <comandos> | λ
	private void comandos() throws CompilerException {
		//Produção substituída por (na notação de Wirth):
		//COMANDOS = {CMD ";"} .
		while(inFirstSetOfCMD()) {
			cmd();
			terminal(TokenType.SYMBOL_SEMICOLON);
		}
		return;
	}

	//avalia se o token está no firstSet(CMD)
	private boolean inFirstSetOfCMD() {
		switch(token.getType()) {
			case KEYWORD_READ:
			case KEYWORD_WRITE:
			case KEYWORD_WHILE:
			case KEYWORD_IF:
			case IDENTIFIER:
			case KEYWORD_BEGIN:
			case KEYWORD_REPEAT:
				return true;
			default:
				return false;
		}
	}

	/* 19. <cmd> ::= read ( <variaveis> ) |
	 * write ( <variaveis> ) |
	 * while <condicao> do <cmd> |
	 * if <condicao> then <cmd> <pfalsa> |
	 * ident <pos_ident> |
	 * begin <comandos> end |
	 * repeat <comandos> until <condicao>
	 */
	private void cmd() throws CompilerException {
		switch(token.getType()) {

			// <cmd> ::= read ( <variaveis> ) | write ( <variaveis> )
			case KEYWORD_READ:
			case KEYWORD_WRITE:
				token = lexer.next();
				terminal(TokenType.SYMBOL_LPAR);
				variaveis();
				terminal(TokenType.SYMBOL_RPAR);
				return;

			// <cmd> ::= while <condicao> do <cmd>
			case KEYWORD_WHILE:
				token = lexer.next();
				condicao();
				terminal(TokenType.KEYWORD_DO);
				cmd();
				return;

			// <cmd> ::= if <condicao> then <cmd> <pfalsa>
			case KEYWORD_IF:
				token = lexer.next();
				condicao();
				terminal(TokenType.KEYWORD_THEN);
				cmd();
				pfalsa();
				return;

			// <cmd> ::= ident <pos_ident>
			case IDENTIFIER:
				token = lexer.next();
				pos_ident();
				return;

			// <cmd> ::= begin <comandos> end
			case KEYWORD_BEGIN:
				token = lexer.next();
				comandos();
				terminal(TokenType.KEYWORD_END);
				return;

			// <cmd> ::= repeat <comandos> until <condicao>
			case KEYWORD_REPEAT:
				token = lexer.next();
				comandos();
				terminal(TokenType.KEYWORD_UNTIL);
				condicao();
				return;

			default:
				throw new SyntacticException(token,
						"Era esperado um comando.");
		}
	}

	//20. <condicao> ::= <expressao> <relacao> <expressao>
	private void condicao() throws CompilerException {
		expressao();
		relacao();
		expressao();
		return;
	}

	//21. <relacao> ::= = | <> | >= | <= | > | <
	private void relacao() throws CompilerException {
		switch(token.getType()) {
			case SYMBOL_EQ: //símbolo terminal "="
			case SYMBOL_NE: //símbolo terminal "<>"
			case SYMBOL_GE: //símbolo terminal ">="
			case SYMBOL_LE: //símbolo terminal "<="
			case SYMBOL_GT: //símbolo terminal ">"
			case SYMBOL_LT: //símbolo terminal "<"
				token = lexer.next();
				return;

			default:
				throw new SyntacticException(token, 
						"Era esperado um operador relacional.");
		}
	}

	//22. <expressao> ::= <termo> <outros_termos>
	//24. <outros_termos> ::= <op_ad> <termo> <outros_termos> | λ
	private void expressao() throws CompilerException {
		//Produção substituída por (na notação de Wirth):
		//EXPRESSAO = TERMO {OP_AD TERMO} .
		termo();
		while(inFirstSetOfOP_AD()) {
			op_ad();
			termo();
		}
		return;
	}

	//avalia se o token está no firstSet(OP_AD)
	private boolean inFirstSetOfOP_AD() {
		switch(token.getType()) {
			case SYMBOL_PLUS:
			case SYMBOL_MINUS:
				return true;

			default:
				return false;
		}
	}

	//23. <op_un> ::= + | - | λ
	private void op_un() throws CompilerException {
		switch(token.getType()) {
			case SYMBOL_PLUS: //símbolo terminal "+"
			case SYMBOL_MINUS: //símbolo terminal "-"
				token = lexer.next();
				return;

			default: 
				//sinal omitido
				return;
		}
	}

	//25. <op_ad> ::= + | -
	private void op_ad() throws CompilerException {
		switch(token.getType()) {
			case SYMBOL_PLUS: //símbolo terminal "+"
			case SYMBOL_MINUS: //símbolo terminal "-"
				token = lexer.next();
				return;

			default:
				throw new SyntacticException(token,
						"Era esperado um operador aditivo.");
		}
	}

	//26. <termo> ::= <op_un> <fator> <mais_fatores>
	//27. <mais_fatores> ::= <op_mul> <fator> <mais_fatores> | λ
	private void termo() throws CompilerException {
		//Produção substituída por (na notação de Wirth):
		//TERMO = OP_UN FATOR {OP_MUL FATOR} .
		op_un();
		fator();
		while(inFirstSetOfOP_MUL()) {
			op_mul();
			fator();
		}
		return;
	}

	//avalia se o token está no firstSet(OP_AD)
	private boolean inFirstSetOfOP_MUL() {
		switch(token.getType()) {
			case SYMBOL_MULT:
			case SYMBOL_DIV:
				return true;

			default:
				return false;
		}
	}

	//28. <op_mul> ::= * | /
	private void op_mul() throws CompilerException {
		switch(token.getType()) {
			case SYMBOL_MULT: //símbolo terminal "*"
			case SYMBOL_DIV: //símbolo terminal "/"
				token = lexer.next();
				return;

			default:
				throw new SyntacticException(token,
						"Era esperado um operador multiplicativo.");
		}
	}

	//29. <fator> ::= ident | numero_int | numero_real | ( <expressao> )
	private void fator() throws CompilerException {
		switch(token.getType()) {
			case IDENTIFIER:
			case CONST_INTEGER:
			case CONST_REAL:
				token = lexer.next();
				break;

			case SYMBOL_LPAR:
				token = lexer.next();
				expressao();
				terminal(TokenType.SYMBOL_RPAR);
				break;

			default:
				throw new SyntacticException(token,
						"Era esperado um operando.");
		}
		return;
	}

	/**
	 * {@inheritDoc }
	 */
	public List<CompilationError> getErrors() {
		return Collections.emptyList();
	}

}
