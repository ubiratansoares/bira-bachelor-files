package compiler.t2b;

import compiler.*;

import java.util.Collections;
import java.util.Deque;
import java.util.HashSet;
import java.util.List;
import java.util.LinkedList;
import java.util.Set;

/**
 * Analisador sintático da linguagem LALG.
 *
 * @version 2.1.1
 */
public class ParserImpl implements SyntacticAnalyzer {

	private LexicalAnalyzer lexer;
	private Token token;

	private List<CompilationError> errors;
	private Deque<Set<TokenType>> syncStack;
	private boolean panic;

	/**
	 * Construtor vazio.
	 */
	public ParserImpl() {
		errors = new LinkedList<CompilationError>();
		syncStack = new LinkedList<Set<TokenType>>();
		setPool = new LinkedList<Set<TokenType>>();
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
	 * A lista de erros detectada pela análise.
	 *
	 * @throws CompilerException
	 * Lançada para indicar qualquer erro de compilação.
	 */
	public ObjectCode parse(SourceCode source) throws CompilerException {

		//inicializar
		lexer.setSourceCode(source);
		errors.clear();
		panic = false;
		token = lexer.next();

		//sincronizar com {EOF}
		syncSet = createSyncSet();
		syncSet.add(TokenType.EOF);

		programa();
		terminal(TokenType.EOF);

		returnSyncSet();
		return null;
	}

	// 1. <programa> ::= program ident ; <corpo> .
	private void programa() throws CompilerException {

		//sincronizar com {program, ident, ;, .}
		syncSet = createSyncSet();
		syncSet.add(TokenType.KEYWORD_PROGRAM);
		syncSet.add(TokenType.IDENTIFIER);
		syncSet.add(TokenType.SYMBOL_SEMICOLON);
		syncSet.add(TokenType.SYMBOL_PERIOD);

		terminal(TokenType.KEYWORD_PROGRAM);

		//sincronizar com {ident, ;, .}
		syncSet = retrieveSyncSet();
		syncSet.remove(TokenType.KEYWORD_PROGRAM);

		terminal(TokenType.IDENTIFIER);
		
		//sincronizar com {;, .}
		syncSet = retrieveSyncSet();
		syncSet.remove(TokenType.IDENTIFIER);

		terminal(TokenType.SYMBOL_SEMICOLON);

		//sincronizar com {.}
		syncSet = retrieveSyncSet();
		syncSet.remove(TokenType.SYMBOL_SEMICOLON);

		corpo();

		terminal(TokenType.SYMBOL_PERIOD);

		returnSyncSet();
		return;
	}

	// 2. <corpo> ::= <dc> begin <comandos> end
	private void corpo() throws CompilerException {

		syncSet = createSyncSet();
		syncSet.add(TokenType.KEYWORD_BEGIN);
		syncSet.add(TokenType.KEYWORD_END);

		dc();

		terminal(TokenType.KEYWORD_BEGIN);

		//sincronizar com {end}
		syncSet = retrieveSyncSet();
		syncSet.remove(TokenType.KEYWORD_BEGIN);

		comandos(TokenType.KEYWORD_END);

		terminal(TokenType.KEYWORD_END);

		returnSyncSet();
		return;
	}

	// 3. <dc> ::= <dc_v> <dc_p>
	private void dc() throws CompilerException {

		//sincronizar com {var, procedure}
		syncSet = createSyncSet();
		syncSet.add(TokenType.KEYWORD_VAR);
		syncSet.add(TokenType.KEYWORD_PROCEDURE);

		dc_v();

		//sincronizar com {procedure}
		syncSet = retrieveSyncSet();
		syncSet.remove(TokenType.KEYWORD_VAR);

		dc_p();

		returnSyncSet();
		return;
	}

	// 4. <dc_v> ::= var <variaveis> : <tipo_var> ; <dc_v> | λ
	private void dc_v() throws CompilerException {

		// Produção substituída por (na notação de Wirth):
		// DC_V = {"var" VARIAVEIS ":" TIPO_VAR ";"} .

		//sincronizar com {var}
		syncSet = createSyncSet();
		syncSet.add(TokenType.KEYWORD_VAR);

		while(optional(TokenType.KEYWORD_VAR)) {

			//sincronizar com {var, :, ;}
			syncSet = retrieveSyncSet();
			syncSet.add(TokenType.SYMBOL_COLON);
			syncSet.add(TokenType.SYMBOL_SEMICOLON);

			variaveis();

			terminal(TokenType.SYMBOL_COLON);

			//sincronizar com {var, ;}
			syncSet = retrieveSyncSet();
			syncSet.remove(TokenType.SYMBOL_COLON);

			tipo_var();

			terminal(TokenType.SYMBOL_SEMICOLON);

			//sincronizar com {var}
			syncSet = retrieveSyncSet();
			syncSet.remove(TokenType.SYMBOL_SEMICOLON);
		}

		returnSyncSet();
		return;
	}

	// 5. <tipo_var> ::= real | integer
	private void tipo_var() throws CompilerException {

		//sincronizar com {:, ;}
		syncSet = createSyncSet();
		syncSet.add(TokenType.KEYWORD_INTEGER);
		syncSet.add(TokenType.KEYWORD_REAL);

		//verifica e trata o erro
		switch(token.getType()) {
			case KEYWORD_INTEGER:
			case KEYWORD_REAL:
				break; //token ok

			default:
				if(!panic) {
					handleError("Era esperado um tipo de variável.");
				}
		}

		returnSyncSet();

		//trata o símbolo atual
		switch(token.getType()) {
			case KEYWORD_INTEGER:
			case KEYWORD_REAL:
				panic = false;
				token = lexer.next();
				break;
		}

		return;
	}

	// 6. <variaveis> ::= ident <mais_var>
	// 7. <mais_var> ::= , <variaveis> | λ
	private void variaveis() throws CompilerException {

		// Produções substituídas por (na notação de Wirth):
		// VARIAVEIS = "ident" {"," "ident"} .

		//sincronizar com {ident, ','}
		syncSet = createSyncSet();
		syncSet.add(TokenType.IDENTIFIER);
		syncSet.add(TokenType.SYMBOL_COMMA);

		terminal(TokenType.IDENTIFIER);
		while(optional(TokenType.SYMBOL_COMMA))
			terminal(TokenType.IDENTIFIER);

		returnSyncSet();
		return;
	}

	// 8. <dc_p> ::= procedure ident <parametros> ; <corpo_p> <dc_p> | λ
	private void dc_p() throws CompilerException {

		//Produção substituída por (na notação de Wirth):
		//DC_P = {"procedure" "ident" PARAMETROS ";" CORPO_P} .

		//sincronizar com {procedure}
		syncSet = createSyncSet();
		syncSet.add(TokenType.KEYWORD_PROCEDURE);

		while(optional(TokenType.KEYWORD_PROCEDURE)) {

			//sincronizar com {procedure, ident, ;}
			syncSet = retrieveSyncSet();
			syncSet.add(TokenType.IDENTIFIER);
			syncSet.add(TokenType.SYMBOL_SEMICOLON);

			terminal(TokenType.IDENTIFIER);

			//sincronizar com {procedure, ;}
			syncSet = retrieveSyncSet();
			syncSet.remove(TokenType.IDENTIFIER);

			parametros();

			terminal(TokenType.SYMBOL_SEMICOLON);

			corpo_p();

			//sincronizar com {procedure}
			syncSet = retrieveSyncSet();
			syncSet.remove(TokenType.SYMBOL_SEMICOLON);
		}

		returnSyncSet();
		return;
	}

	// 9. <parametros> ::= ( <lista_par> ) | λ
	private void parametros() throws CompilerException {
		if(optional(TokenType.SYMBOL_LPAR)) {

			//sincronizar com {)}
			syncSet = createSyncSet();
			syncSet.add(TokenType.SYMBOL_RPAR);

			lista_par();

			terminal(TokenType.SYMBOL_RPAR);

			returnSyncSet();
		}
		return;
	}

	//10. <lista_par> ::= <variaveis> : <tipo_var> <mais_par>
	//11. <mais_par> ::= ; <lista_par> | λ
	private void lista_par() throws CompilerException {

		//Produções substituídas por (na notação de Wirth):
		//LISTA_PAR = VARIAVEIS ":" TIPO_VAR {";" VARIAVEIS ":" TIPO_VAR} .

		//sincronizar com {:, ;}
		syncSet = createSyncSet();
		syncSet.add(TokenType.SYMBOL_COLON);
		syncSet.add(TokenType.SYMBOL_SEMICOLON);

		variaveis();

		terminal(TokenType.SYMBOL_COLON);

		//sincronizar com {;}
		syncSet = retrieveSyncSet();
		syncSet.remove(TokenType.SYMBOL_COLON);

		tipo_var();
		
		while(optional(TokenType.SYMBOL_SEMICOLON)) {

			//sincronizar com {:, ;}
			syncSet = retrieveSyncSet();
			syncSet.add(TokenType.SYMBOL_SEMICOLON);

			variaveis();

			terminal(TokenType.SYMBOL_COLON);

			//sincronizar com {;}
			syncSet = retrieveSyncSet();
			syncSet.remove(TokenType.SYMBOL_SEMICOLON);

			tipo_var();
		}

		returnSyncSet();
		return;
	}

	//12. <corpo_p> ::= <dc_loc> begin <comandos> end ;
	//13. <dc_loc> ::= <dc_v>
	private void corpo_p() throws CompilerException {

		//Produções substituídas por (na notação de Wirth):
		//CORPO_P = DC_V "begin" COMANDOS "end" ";" .

		//sincronizar com {begin, end, ;}
		syncSet = createSyncSet();
		syncSet.add(TokenType.KEYWORD_BEGIN);
		syncSet.add(TokenType.KEYWORD_END);
		syncSet.add(TokenType.SYMBOL_SEMICOLON);

		dc_v();

		terminal(TokenType.KEYWORD_BEGIN);

		//sincronizar com {end, ;}
		syncSet = retrieveSyncSet();
		syncSet.remove(TokenType.KEYWORD_BEGIN);

		comandos(TokenType.KEYWORD_END);

		terminal(TokenType.KEYWORD_END);

		//sincronizar com {;}
		syncSet = retrieveSyncSet();
		syncSet.remove(TokenType.KEYWORD_END);

		terminal(TokenType.SYMBOL_SEMICOLON);

		returnSyncSet();
		return;
	}

	//14. <lista_arg> ::= ( <argumentos> ) | λ
	//30. <pos_ident> ::= := <expressao> | <lista_arg>
	private void pos_ident() throws CompilerException {

		//Produções substituídas por (na notação de Wirth):
		//POS_IDENT = ":=" EXPRESSAO | "(" ARGUMENTOS ")" | .

		switch(token.getType()) {
			case SYMBOL_ASSIGN:
				panic = false;
				token = lexer.next();
				expressao();
				return;

			case SYMBOL_LPAR:
				panic = false;
				token = lexer.next();

				//sincronizar com {)}
				syncSet = createSyncSet();
				syncSet.add(TokenType.SYMBOL_RPAR);

				argumentos();

				terminal(TokenType.SYMBOL_RPAR);

				returnSyncSet();
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

		//sincronizar com {ident, ;}
		syncSet = createSyncSet();
		syncSet.add(TokenType.IDENTIFIER);
		syncSet.add(TokenType.SYMBOL_SEMICOLON);

		terminal(TokenType.IDENTIFIER);
		while(optional(TokenType.SYMBOL_SEMICOLON))
			terminal(TokenType.IDENTIFIER);

		returnSyncSet();
		return;
	}

	//17. <pfalsa> ::= else <cmd> | λ
	private void pfalsa() throws CompilerException {
		if(optional(TokenType.KEYWORD_ELSE))
			cmd();
		return;
	}

	//18. <comandos> ::= <cmd> ; <comandos> | λ
	private void comandos(TokenType follow) throws CompilerException {

		//Produção substituída por (na notação de Wirth):
		//COMANDOS = {CMD ";"} .

		//sincronizar com {;}
		syncSet = createSyncSet();
		syncSet.add(TokenType.SYMBOL_SEMICOLON);

		while(token.getType() != follow && !panic) {
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

		//sincronizar com {read, write, while, if, ident, begin, repeat}
		syncSet = createSyncSet();
		syncSet.add(TokenType.KEYWORD_READ);
		syncSet.add(TokenType.KEYWORD_WRITE);
		syncSet.add(TokenType.KEYWORD_IF);
		syncSet.add(TokenType.KEYWORD_WHILE);
		syncSet.add(TokenType.KEYWORD_REPEAT);
		syncSet.add(TokenType.KEYWORD_BEGIN);
		syncSet.add(TokenType.IDENTIFIER);

		//trata o primeiro símbolo
		Token first = token;
		if(inFirstSetOfCMD()) {
			panic = false;
			token = lexer.next();
		} else if(!panic) {
			handleError("Era esperado um comando.");
			if(inFirstSetOfCMD()) {
				panic = false;
				token = lexer.next();
			}
		}

		returnSyncSet();
		syncSet = createSyncSet();

		//trata os próximos símbolos
		switch(first.getType()) {

			// <cmd> ::= read ( <variaveis> ) | write ( <variaveis> )
			case KEYWORD_READ:
			case KEYWORD_WRITE:
				//sincronizar com {(, )}
				syncSet.add(TokenType.SYMBOL_LPAR);
				syncSet.add(TokenType.SYMBOL_RPAR);

				terminal(TokenType.SYMBOL_LPAR);

				//sincronizar com {)}
				syncSet = retrieveSyncSet();
				syncSet.remove(TokenType.SYMBOL_LPAR);

				variaveis();

				terminal(TokenType.SYMBOL_RPAR);

				break;

			// <cmd> ::= while <condicao> do <cmd>
			case KEYWORD_WHILE:
				//sincronizar com {do}
				syncSet.add(TokenType.KEYWORD_DO);

				condicao();

				terminal(TokenType.KEYWORD_DO);

				syncSet.remove(TokenType.KEYWORD_DO);

				cmd();

				break;

			// <cmd> ::= if <condicao> then <cmd> <pfalsa>
			case KEYWORD_IF:
				//sincronizar com {then}
				syncSet.add(TokenType.KEYWORD_THEN);

				condicao();

				terminal(TokenType.KEYWORD_THEN);

				syncSet.remove(TokenType.KEYWORD_THEN);

				cmd();

				pfalsa();

				break;

			// <cmd> ::= ident <pos_ident>
			case IDENTIFIER:
				pos_ident();
				break;

			// <cmd> ::= begin <comandos> end
			case KEYWORD_BEGIN:
				//sincronizar com {end}
				syncSet.add(TokenType.KEYWORD_END);

				comandos(TokenType.KEYWORD_END);

				terminal(TokenType.KEYWORD_END);

				break;

			// <cmd> ::= repeat <comandos> until <condicao>
			case KEYWORD_REPEAT:
				//sincronizar com {until}
				syncSet.add(TokenType.KEYWORD_UNTIL);

				comandos(TokenType.KEYWORD_UNTIL);

				terminal(TokenType.KEYWORD_UNTIL);

				syncSet.remove(TokenType.KEYWORD_UNTIL);

				condicao();

				break;

			default: //já tratado
				break;
		}

		returnSyncSet();
		return;
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
		//sincronizar com {=, <>, >=, <=, >, <}
		syncSet = createSyncSet();
		syncSet.add(TokenType.SYMBOL_EQ);
		syncSet.add(TokenType.SYMBOL_NE);
		syncSet.add(TokenType.SYMBOL_GE);
		syncSet.add(TokenType.SYMBOL_LE);
		syncSet.add(TokenType.SYMBOL_GT);
		syncSet.add(TokenType.SYMBOL_LT);

		//verifica e trata o erro
		switch(token.getType()) {
			case SYMBOL_EQ: //símbolo terminal "="
			case SYMBOL_NE: //símbolo terminal "<>"
			case SYMBOL_GE: //símbolo terminal ">="
			case SYMBOL_LE: //símbolo terminal "<="
			case SYMBOL_GT: //símbolo terminal ">"
			case SYMBOL_LT: //símbolo terminal "<"
				break;

			default:
				if(!panic) {
					handleError("Era esperado um operador relacional.");
				}
		}

		returnSyncSet();

		//trata o símbolo atual
		switch(token.getType()) {
			case SYMBOL_EQ: //símbolo terminal "="
			case SYMBOL_NE: //símbolo terminal "<>"
			case SYMBOL_GE: //símbolo terminal ">="
			case SYMBOL_LE: //símbolo terminal "<="
			case SYMBOL_GT: //símbolo terminal ">"
			case SYMBOL_LT: //símbolo terminal "<"
				panic = false;
				token = lexer.next();
				break;
		}

		return;
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

	//23. <op_un> ::= + | - | λ
	private void op_un() throws CompilerException {
		switch(token.getType()) {
			case SYMBOL_PLUS: //símbolo terminal "+"
			case SYMBOL_MINUS: //símbolo terminal "-"
				token = lexer.next();
				return;

			default:
				return; //sinal omitido
		}
	}

	//verifica se o token é um operador aditivo
	private boolean inFirstSetOfOP_AD() {
		switch(token.getType()) {
			case SYMBOL_PLUS:
			case SYMBOL_MINUS:
				return true;
			default:
				return false;
		}
	}

	//25. <op_ad> ::= + | -
	private void op_ad() throws CompilerException {
		//sincronizar com {+, -}
		syncSet = createSyncSet();
		syncSet.add(TokenType.SYMBOL_PLUS);
		syncSet.add(TokenType.SYMBOL_MINUS);

		//verifica e trata o erro
		switch(token.getType()) {
			case SYMBOL_PLUS:
			case SYMBOL_MINUS:
				break;

			default:
				if(!panic) {
					handleError("Era esperado um operador aditivo.");
				}
		}

		//trata o símbolo atual
		switch(token.getType()) {
			case SYMBOL_PLUS:
			case SYMBOL_MINUS:
				panic = false;
				token = lexer.next();
				break;
		}

		returnSyncSet();
		return;
	}

	//26. <termo> ::= <op_un> <fator> <mais_fatores>
	//27. <mais_fatores> ::= <op_mul> <fator> <mais_fatores> | λ
	private void termo() throws CompilerException {
		//Produção substituída por (na notação de Wirth):
		//TERMO = OP_UN FATOR {OP_MUL FATOR} .
		op_un();
		fator();
		while(inFirstSetOifOP_MUL()) {
			op_mul();
			fator();
		}
		return;
	}

	//verifica se o token é um operador multiplicativo
	private boolean inFirstSetOifOP_MUL() {
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
		//sincronizar com {*, /}
		syncSet = createSyncSet();
		syncSet.add(TokenType.SYMBOL_MULT);
		syncSet.add(TokenType.SYMBOL_DIV);

		//verifica e trata o erro
		switch(token.getType()) {
			case SYMBOL_MULT:
			case SYMBOL_DIV:
				break;

			default:
				if(!panic) {
					handleError("Era esperado um operador multiplicativo.");
				}
		}

		//trata o símbolo atual
		switch(token.getType()) {
			case SYMBOL_MULT:
			case SYMBOL_DIV:
				panic = false;
				token = lexer.next();
				break;
		}

		returnSyncSet();
		return;
	}

	//29. <fator> ::= ident | numero_int | numero_real | ( <expressao> )
	private void fator() throws CompilerException {
		//sincronizar com {ident, numero_int, numero_real, (}
		syncSet = createSyncSet();
		syncSet.add(TokenType.IDENTIFIER);
		syncSet.add(TokenType.CONST_INTEGER);
		syncSet.add(TokenType.CONST_REAL);
		syncSet.add(TokenType.SYMBOL_LPAR);

		//verifica e trata o erro
		switch(token.getType()) {
			case IDENTIFIER:
			case CONST_INTEGER:
			case CONST_REAL:
			case SYMBOL_LPAR:
				break;

			default:
				if(!panic) {
					handleError("Era esperado um operando.");
				}
		}

		returnSyncSet();

		//trata o símbolo atual
		switch(token.getType()) {
			case IDENTIFIER:
			case CONST_INTEGER:
			case CONST_REAL:
				panic = false;
				token = lexer.next();
				break;

			case SYMBOL_LPAR:
				panic = false;
				token = lexer.next();

				syncSet = createSyncSet();
				syncSet.add(TokenType.SYMBOL_RPAR);

				expressao();

				terminal(TokenType.SYMBOL_RPAR);

				returnSyncSet();
				break;
		}

		return;
	}

	//avalia um símbolo terminal
	private void terminal(TokenType expectedType) throws CompilerException {
		if(token.getType() == expectedType) {
			panic = false;
			token = lexer.next();
		} else if(!panic) {
			//se modo de pânico
			//então ignorar análise para sincronizar
			//senão tratar o erro e sincronizar
			if(token.getType() == TokenType.EOF) {
				panic = true;
				errors.add(new CompilationError(token,
						"Não era esperado o fim do arquivo."));
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
					default: message = "Desconhecido.";
				}

				//tentar sincronizar com o símbolo atual
				syncSet = createSyncSet();
				syncSet.add(expectedType);

				handleError(message);

				returnSyncSet();

				//caso consiga, prosseguir normalmente
				if(token.getType() == expectedType) {
					panic = false;
					token = lexer.next();
				}
			}
		}
		return;
	}

	//avalia um símbolo terminal opcional
	//retorna true, se o símbolo for encontrado
	private boolean optional(TokenType expectedType) throws CompilerException {
		if(token.getType() == expectedType) {
			panic = false;
			token = lexer.next();
			return true;
		} else
			return false;
	}

	//tratamento de erros
	private void handleError(String message) throws CompilerException {
		errors.add(new CompilationError(token, message));

		syncSet = createSet();
		for(Set<TokenType> s : syncStack)
			syncSet.addAll(s);

		while(!syncSet.contains(token.getType()))
			token = lexer.next();

		returnSet(syncSet);
		panic = true;
		return;
	}

	private Set<TokenType> syncSet; //usar como local

	//poço de conjuntos vazios
	private Deque<Set<TokenType>> setPool;

	//cria um conjunto de sincronização vazio
	private Set<TokenType> createSyncSet() {
		Set<TokenType> set;
		set = createSet();
		syncStack.push(set);
		return set;
	}

	//devolve um conjunto de sincronização
	private void returnSyncSet() {
		returnSet(syncStack.pop());
		return;
	}

	//recupera um conjunto de sincronização do topo da pilha
	private Set<TokenType> retrieveSyncSet() {
		return syncStack.peek();
	}

	//cria um conjunto vazio
	private Set<TokenType> createSet() {
		return (setPool.isEmpty())?
			new HashSet<TokenType>():
			setPool.pop();
	}

	//devolve um conjunto ao poço
	private void returnSet(Set<TokenType> set) {
		set.clear();
		setPool.push(set);
		return;
	}

	/**
	 * {@inheritDoc }
	 */
	public List<CompilationError> getErrors() {
		return Collections.unmodifiableList(errors);
	}

}
