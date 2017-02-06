package compiler.t3a;

import compiler.*;

import compiler.t3a.Attributes.Operator;
import compiler.t3a.Symbol.Category;

import java.util.Collections;
import java.util.Deque;
import java.util.HashSet;
import java.util.List;
import java.util.LinkedList;
import java.util.Set;

/**
 * Analisador sintático da linguagem LALG.
 *
 * @version 3.0.3
 */
public class ParserImpl implements SyntacticAnalyzer {

	private LexicalAnalyzer lexer;
	private Token token;
	private boolean localErrorFlag;

	private SymbolsTable symbols;
	private List<CompilationError> errors;
	private Deque<Set<TokenType>> syncStack;

	/**
	 * Construtor vazio.
	 */
	public ParserImpl() {
		errors = new LinkedList<CompilationError>();
		syncStack = new LinkedList<Set<TokenType>>();
		setPool = new LinkedList<Set<TokenType>>();
		attPool = new LinkedList<Attributes>();
		symbols = new SymbolsTable();
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
		token = lexer.next();
		localErrorFlag = false;
		symbols.clear();

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

		Token programId;

		//sincronizar com {var, procedure, begin}
		syncSet = createSyncSet(); //extra
		syncSet.add(TokenType.KEYWORD_VAR);
		syncSet.add(TokenType.KEYWORD_PROCEDURE);
		syncSet.add(TokenType.KEYWORD_BEGIN);

		//sincronizar com {ident}
		syncSet = createSyncSet();
		syncSet.add(TokenType.IDENTIFIER);
		terminal(TokenType.KEYWORD_PROGRAM);
		returnSyncSet();

		//sincronizar com {;}
		syncSet = createSyncSet();
		syncSet.add(TokenType.SYMBOL_SEMICOLON);
		programId = terminal(TokenType.IDENTIFIER);
		returnSyncSet();

		//sincronizar com firstSet(<corpo>) =
		//{var, procedure, begin}
		syncSet = createSyncSet();
		syncSet.add(TokenType.KEYWORD_VAR);
		syncSet.add(TokenType.KEYWORD_PROCEDURE);
		syncSet.add(TokenType.KEYWORD_BEGIN);
		terminal(TokenType.SYMBOL_SEMICOLON);
		returnSyncSet();

		Attributes att = createAttributes();

		//declaração do programa
		Symbol ident;
		if(localErrorFlag == false) {
			ident = new Symbol(programId);
		} else {
			ident = symbols.createNullIdentifier();
		}

		ident.setCategory(Category.PROGRAM);
		try {
			symbols.declare(ident);
		} catch(SemanticException e) {
			errors.add(new SemanticError(programId,
					"O nome do programa já foi declarado."));
		}
		att.setScope(ident);

		returnSyncSet(); //extra
		localErrorFlag = false;

		//sincronizar com {.}
		syncSet = createSyncSet();
		syncSet.add(TokenType.SYMBOL_PERIOD);
		corpo(att);
		returnSyncSet();

		terminal(TokenType.SYMBOL_PERIOD);

		returnAttributes(att);
		return;
	}

	// 2. <corpo> ::= <dc> begin <comandos> end
	private void corpo(Attributes att) throws CompilerException {

		//sincronizar com {begin}
		syncSet = createSyncSet();
		syncSet.add(TokenType.KEYWORD_BEGIN);
		dc(att);
		returnSyncSet();

		//sincronizar com firstSet(<comandos>) U followSet(<comandos>) =
		//{read, write, while, if, ident, begin, repeat, λ} U {end, until}
		syncSet = createSyncSet();
		syncSet.add(TokenType.KEYWORD_READ);
		syncSet.add(TokenType.KEYWORD_WRITE);
		syncSet.add(TokenType.KEYWORD_WHILE);
		syncSet.add(TokenType.KEYWORD_IF);
		syncSet.add(TokenType.IDENTIFIER);
		syncSet.add(TokenType.KEYWORD_BEGIN);
		syncSet.add(TokenType.KEYWORD_REPEAT);
		syncSet.add(TokenType.KEYWORD_END);
		syncSet.add(TokenType.KEYWORD_UNTIL);
		terminal(TokenType.KEYWORD_BEGIN);
		returnSyncSet();

		//sincronizar com {end}
		syncSet = createSyncSet();
		syncSet.add(TokenType.KEYWORD_END);
		comandos(att);
		returnSyncSet();

		terminal(TokenType.KEYWORD_END);
		return;
	}

	// 3. <dc> ::= <dc_v> <dc_p>
	private void dc(Attributes att) throws CompilerException {

		//sincronizar com firstSet(<dc_p>) U followSet(<dc_p>) =
		//{procedure, λ} U {begin}
		syncSet = createSyncSet();
		syncSet.add(TokenType.KEYWORD_PROCEDURE);
		syncSet.add(TokenType.KEYWORD_BEGIN);
		dc_v(att);
		returnSyncSet();

		dc_p(att);
		return;
	}

	// 4. <dc_v> ::= var <variaveis> : <tipo_var> ; <dc_v> | λ
	private void dc_v(Attributes att) throws CompilerException {

		// Produção substituída por (na notação de Wirth):
		// DC_V = {"var" VARIAVEIS ":" TIPO_VAR ";"} .

		while(optional(TokenType.KEYWORD_VAR)) {
			localErrorFlag = false;

			Attributes dcvAtt = createAttributes();

			//sincronizar com {:}
			syncSet = createSyncSet();
			syncSet.add(TokenType.SYMBOL_COLON);
			variaveis(dcvAtt);
			returnSyncSet();

			//sincronizar com firstSet(<tipo_var>) =
			//{real, integer}
			syncSet = createSyncSet();
			syncSet.add(TokenType.KEYWORD_REAL);
			syncSet.add(TokenType.KEYWORD_INTEGER);
			terminal(TokenType.SYMBOL_COLON);
			returnSyncSet();

			//sincronizar com {;}
			syncSet = createSyncSet();
			syncSet.remove(TokenType.SYMBOL_SEMICOLON);
			tipo_var(dcvAtt);
			returnSyncSet();

			//sincronizar com {var}
			syncSet = createSyncSet();
			syncSet.add(TokenType.KEYWORD_VAR);
			terminal(TokenType.SYMBOL_SEMICOLON);
			returnSyncSet();

			//declarar todas as variáveis da lista
			if(localErrorFlag == false) {
				for(Symbol ident : dcvAtt.getVariables()) {
					ident.setScope(att.getScope());
					ident.setType(dcvAtt.getType());
					ident.setCategory(Category.VARIABLE);

					try {
						symbols.declare(ident);
					} catch(SemanticException e) {
						errors.add(new SemanticError(
								ident.getToken(), e.getMessage()));
					}
				}
			}

			returnAttributes(dcvAtt);
		}

		return;
	}

	// 5. <tipo_var> ::= real | integer
	private void tipo_var(Attributes att) throws CompilerException {
		switch(token.getType()) {
			case KEYWORD_INTEGER:
				token = lexer.next();
				att.setType(DataType.INTEGER);
				break;

			case KEYWORD_REAL:
				token = lexer.next();
				att.setType(DataType.REAL);
				break;

			default:
				handleError("Era esperado um tipo de variável.");
		}
		return;
	}

	// 6. <variaveis> ::= ident <mais_var>
	// 7. <mais_var> ::= , <variaveis> | λ
	private void variaveis(Attributes att) throws CompilerException {

		// Produções substituídas por (na notação de Wirth):
		// VARIAVEIS = "ident" {"," "ident"} .

		Token t; Symbol ident;

		syncSet = createSyncSet();
		syncSet.add(TokenType.SYMBOL_COMMA);
		t = terminal(TokenType.IDENTIFIER);
		returnSyncSet();

		ident = new Symbol(t);
		att.addVariable(ident);

		while(optional(TokenType.SYMBOL_COMMA)) {
			syncSet = createSyncSet();
			syncSet.add(TokenType.SYMBOL_COMMA);
			t = terminal(TokenType.IDENTIFIER);
			returnSyncSet();

			ident = new Symbol(t);
			att.addVariable(ident);
		}

		return;
	}

	// 8. <dc_p> ::= procedure ident <parametros> ; <corpo_p> <dc_p> | λ
	private void dc_p(Attributes att) throws CompilerException {

		//Produção substituída por (na notação de Wirth):
		//DC_P = {"procedure" "ident" PARAMETROS ";" CORPO_P} .

		Token procId; Symbol ident; Attributes dcpAtt;

		while(optional(TokenType.KEYWORD_PROCEDURE)) {

			localErrorFlag = false;
			dcpAtt = createAttributes();

			//sincronizar com firstSet(<parametros>) U 
			//followSet(<parametros>) = {(, λ} U {;}
			syncSet = createSyncSet();
			syncSet.add(TokenType.SYMBOL_LPAR);
			syncSet.add(TokenType.SYMBOL_SEMICOLON);
			procId = terminal(TokenType.IDENTIFIER);
			returnSyncSet();

			//declarar procedimento
			ident = (localErrorFlag == false)? 
				new Symbol(procId): symbols.createNullIdentifier();
			ident.setScope(att.getScope());
			ident.setCategory(Category.PROCEDURE);
			try {
				symbols.declare(ident);
			} catch(SemanticException e) {
				errors.add(new SemanticError(procId, e.getMessage()));
			}
			dcpAtt.clear();
			dcpAtt.setScope(ident);

			//sincronizar com {;}
			syncSet = createSyncSet();
			syncSet.add(TokenType.SYMBOL_SEMICOLON);
			parametros(dcpAtt);
			returnSyncSet();

			//sincronizar com firstSet(<corpo_p>) =
			//{var, begin}
			syncSet = createSyncSet();
			syncSet.add(TokenType.KEYWORD_VAR);
			syncSet.add(TokenType.KEYWORD_BEGIN);
			terminal(TokenType.SYMBOL_SEMICOLON);
			returnSyncSet();

			//sincronizar com {procedure}
			syncSet = createSyncSet();
			syncSet.add(TokenType.KEYWORD_PROCEDURE);
			corpo_p(dcpAtt);
			returnSyncSet();

			returnAttributes(dcpAtt);
		}
		return;
	}

	// 9. <parametros> ::= ( <lista_par> ) | λ
	private void parametros(Attributes att) throws CompilerException {
		int npar = 0;

		if(optional(TokenType.SYMBOL_LPAR)) {
			localErrorFlag = false;
			Attributes parsAtt = createAttributes();

			//sincronizar com {)}
			syncSet = createSyncSet();
			syncSet.add(TokenType.SYMBOL_RPAR);
			lista_par(parsAtt);
			returnSyncSet();

			terminal(TokenType.SYMBOL_RPAR);

			if(localErrorFlag == false) {
				//declarar parâmetros
				Symbol param;
				List<Symbol> parlist = parsAtt.getVariables();
				for(int i = 0; i < parlist.size(); i++) {
					param = parlist.get(i);
					param.setScope(att.getScope());
					param.setCategory(Category.PARAMETER);
					param.setIndex(i);

					try {
						symbols.declare(param); npar++;
					} catch(SemanticException e) {
						errors.add(new SemanticError(
								param.getToken(), e.getMessage()));
					}
				}

			}

			returnAttributes(parsAtt);
		}

		//registrar número de parâmetros
		Symbol proc = att.getScope();
		try {
			proc = symbols.getElement(proc);
			proc.setIndex(npar);
		} catch(SemanticException e) {
			//não deve ocorrer
		}
		return;
	}

	//10. <lista_par> ::= <variaveis> : <tipo_var> <mais_par>
	//11. <mais_par> ::= ; <lista_par> | λ
	private void lista_par(Attributes att) throws CompilerException {

		//Produções substituídas por (na notação de Wirth):
		//LISTA_PAR = VARIAVEIS ":" TIPO_VAR {";" VARIAVEIS ":" TIPO_VAR} .

		Attributes parsAtt;

		parsAtt = createAttributes();

		//sincronizar com {:}
		syncSet = createSyncSet();
		syncSet.add(TokenType.SYMBOL_COLON);
		variaveis(parsAtt);
		returnSyncSet();

		//sincronizar com firstSet(<tipo_var>) =
		//{real, integer}
		syncSet = createSyncSet();
		syncSet.add(TokenType.KEYWORD_REAL);
		syncSet.add(TokenType.KEYWORD_INTEGER);
		terminal(TokenType.SYMBOL_COLON);
		returnSyncSet();

		//sincronizar com {;}
		syncSet = createSyncSet();
		syncSet.add(TokenType.SYMBOL_SEMICOLON);
		tipo_var(parsAtt);
		returnSyncSet();

		if(localErrorFlag == false) {
			//copiar lista
			for(Symbol ident : parsAtt.getVariables()) {
				ident.setType(parsAtt.getType());
				att.addVariable(ident);
			}
		}

		while(optional(TokenType.SYMBOL_SEMICOLON)) {
			localErrorFlag = false;
			parsAtt.clear();

			//sincronizar com {:}
			syncSet = createSyncSet();
			syncSet.add(TokenType.SYMBOL_COLON);
			variaveis(parsAtt);
			returnSyncSet();

			//sincronizar com firstSet(<tipo_var>) =
			//{real, integer}
			syncSet = createSyncSet();
			syncSet.add(TokenType.KEYWORD_REAL);
			syncSet.add(TokenType.KEYWORD_INTEGER);
			terminal(TokenType.SYMBOL_COLON);
			returnSyncSet();

			//sincronizar com {;}
			syncSet = createSyncSet();
			syncSet.add(TokenType.SYMBOL_SEMICOLON);
			tipo_var(parsAtt);
			returnSyncSet();

			if(localErrorFlag == false) {
				//copiar lista
				for(Symbol ident : parsAtt.getVariables()) {
					ident.setType(parsAtt.getType());
					att.addVariable(ident);
				}
			}
		}

		returnAttributes(parsAtt);
		return;
	}

	//12. <corpo_p> ::= <dc_loc> begin <comandos> end ;
	//13. <dc_loc> ::= <dc_v>
	private void corpo_p(Attributes att) throws CompilerException {

		//Produções substituídas por (na notação de Wirth):
		//CORPO_P = DC_V "begin" COMANDOS "end" ";" .

		//sincronizar com {begin, end, ;}
		syncSet = createSyncSet();
		syncSet.add(TokenType.KEYWORD_BEGIN);
		dc_v(att);
		returnSyncSet();

		//sincronizar com firstSet(<comandos>) U followSet(<comandos>) =
		//{read, write, while, if, ident, begin, repeat, λ} U {end, until}
		syncSet = createSyncSet();
		syncSet.add(TokenType.KEYWORD_READ);
		syncSet.add(TokenType.KEYWORD_WRITE);
		syncSet.add(TokenType.KEYWORD_WHILE);
		syncSet.add(TokenType.KEYWORD_IF);
		syncSet.add(TokenType.IDENTIFIER);
		syncSet.add(TokenType.KEYWORD_BEGIN);
		syncSet.add(TokenType.KEYWORD_REPEAT);
		syncSet.add(TokenType.KEYWORD_END);
		syncSet.add(TokenType.KEYWORD_UNTIL);
		terminal(TokenType.KEYWORD_BEGIN);
		returnSyncSet();

		//sincronizar com {end}
		syncSet = createSyncSet();
		syncSet.add(TokenType.KEYWORD_END);
		comandos(att);
		returnSyncSet();

		//sincronizar com {end}
		syncSet = createSyncSet();
		syncSet.add(TokenType.SYMBOL_SEMICOLON);
		terminal(TokenType.KEYWORD_END);
		returnSyncSet();

		terminal(TokenType.SYMBOL_SEMICOLON);
		return;
	}

	//14. <lista_arg> ::= ( <argumentos> ) | λ
	//30. <pos_ident> ::= := <expressao> | <lista_arg>
	private void pos_ident(Attributes att) throws CompilerException {

		//Produções substituídas por (na notação de Wirth):
		//POS_IDENT = ":=" EXPRESSAO | "(" ARGUMENTOS ")" | .

		Attributes posAtt = createAttributes();
		posAtt.setScope(att.getScope());
		posAtt.setToken(att.getToken());

		//não sincronizar o primeiro símbolo

		switch(token.getType()) {
			case SYMBOL_ASSIGN:
				token = lexer.next();
				expressao(posAtt);
				assignmentSemanticCheck(posAtt);
				break;

			case SYMBOL_LPAR:
				token = lexer.next();

				//sincronizar com {)}
				syncSet = createSyncSet();
				syncSet.add(TokenType.SYMBOL_RPAR);
				argumentos(posAtt);
				returnSyncSet();

				terminal(TokenType.SYMBOL_RPAR);

				procedureCallSemanticCheck(posAtt);
				break;

			default:
				//procedimento sem argumentos
				procedureCallSemanticCheck(posAtt);
				break;
		}


		returnAttributes(posAtt);
		return;
	}

	//análise semântica da atribuição
	private void assignmentSemanticCheck(Attributes att) {
		if(localErrorFlag == false) {
			try {
				//verificar declaração da variável
				Symbol ident = symbols.getElement(
						att.getScope(), att.getToken());
				switch(ident.getCategory()) {
					case PROGRAM:
						errors.add(new SemanticError(att.getToken(),
								"O programa não é uma variável."));
						break;

					case PROCEDURE:
						errors.add(new SemanticError(att.getToken(),
								"Um procedimento não é uma variável."));
						break;

					case PARAMETER:
					case VARIABLE:
						//verificar tipagem da atribuição
						if((ident.getType() == DataType.INTEGER) &&
								(att.getType() == DataType.REAL)) {
							errors.add(new SemanticError(att.getToken(),
									"Não é permitido atribuir um valor " +
									"real a uma variável inteira."));
						}
						break;
				}
			} catch(SemanticException e) {
				errors.add(new SemanticError(att.getToken(),
						"Variável não declarada."));
			}
		}
		return;
	}

	//análise semântica da chamada de procedimento
	private void procedureCallSemanticCheck(Attributes att) {
		if(localErrorFlag == false) {
			try {
				//verificar declaração do procedimento
				Symbol proc = symbols.getElement(
						att.getScope(), att.getToken());
				switch(proc.getCategory()) {
					case PROGRAM:
						errors.add(new SemanticError(att.getToken(),
								"O programa não é um procedimento."));
						break;

					case PARAMETER:
						errors.add(new SemanticError(att.getToken(),
								"Um parâmetro não é um procedimento."));
						break;

					case VARIABLE:
						errors.add(new SemanticError(att.getToken(),
								"Uma variável não é um procedimento."));
						break;

					case PROCEDURE:
						List<Symbol> args = att.getVariables();
						if(args.size() == proc.getIndex()) {
							argumentTypeSemanticCheck(att);
						} else if(args.size() < proc.getIndex()) {
							errors.add(new SemanticError(att.getToken(),
									"Número insuficiente de argumentos."));
						} else if(args.size() > proc.getIndex()) {
							errors.add(new SemanticError(att.getToken(),
									"Há argumentos demais."));
						}
						break;
				}
			} catch(SemanticException e) {
				errors.add(new SemanticError(att.getToken(),
						"Procedimento não declarado."));
			}
		}
		return;
	}

	//verifica a assinatura do procedimento
	private void argumentTypeSemanticCheck(Attributes att) {
		Symbol arg, param;
		List<Symbol> args = att.getVariables();
		for(int i = 0; i < args.size(); i++) {
			try {
				param = symbols.getParameter(
						att.getScope(), att.getToken(), i);
				arg = args.get(i);
				if(arg.getType() != param.getType()) {
					errors.add(new SemanticError(arg.getToken(),
							"Incompatibilidade de tipos na " +
							"chamada de procedimento."));
				}
			} catch(SemanticException e) {
				errors.add(new SemanticError(att.getToken(),
						"Parâmetro não declarado."));
			}
		}
		return;
	}

	//15. <argumentos> ::= ident <mais_ident>
	//16. <mais_ident> ::= ; <argumentos> | λ
	private void argumentos(Attributes att) throws CompilerException {

		//Produções substituídas por (na notação de Wirth):
		//ARGUMENTOS = "ident" {";" "ident"} .

		Token t; int index = 0;
		Attributes a = createAttributes();
		a.setScope(att.getScope());
		localErrorFlag = false;

		//sincronizar com {;}
		syncSet = createSyncSet();
		syncSet.add(TokenType.SYMBOL_SEMICOLON);
		t = terminal(TokenType.IDENTIFIER);
		returnSyncSet();

		//verificar declaração
		a.setToken(t); a.setIndex(index);
		argumentDeclarationSemanticCheck(a);
		index++;

		while(optional(TokenType.SYMBOL_SEMICOLON)) {
			//sincronizar com {;}
			syncSet = createSyncSet();
			syncSet.add(TokenType.SYMBOL_SEMICOLON);
			t = terminal(TokenType.IDENTIFIER);
			returnSyncSet();

			//verificar declaração
			a.setToken(t); a.setIndex(index);
			argumentDeclarationSemanticCheck(a);
			index++;
		}

		if(localErrorFlag == false) {
			for(Symbol arg : a.getVariables())
				att.addVariable(arg);
		}
		returnAttributes(a);
		return;
	}

	//verifica a declaração do argumento
	private void argumentDeclarationSemanticCheck(Attributes att) {
		if(localErrorFlag == false) {
			Symbol var, arg;
			arg = new Symbol(att.getToken());
			try {
				var = symbols.getElement(
						att.getScope(), att.getToken());
				switch(var.getCategory()) {
					case PROGRAM:
						errors.add(new SemanticError(att.getToken(),
								"O programa não é uma variável."));
						break;

					case PROCEDURE:
						errors.add(new SemanticError(att.getToken(),
								"Um procedimento não é uma variável."));
						break;

					case VARIABLE:
					case PARAMETER:
						arg.setType(var.getType());
						break;
				}
			} catch(SemanticException e) {
				errors.add(new SemanticError(att.getToken(),
						"Variável não declarada."));
			}
			att.addVariable(arg);
		}
		return;
	}

	//17. <pfalsa> ::= else <cmd> | λ
	private void pfalsa(Attributes att) throws CompilerException {
		if(optional(TokenType.KEYWORD_ELSE))
			cmd(att);
		return;
	}

	//18. <comandos> ::= <cmd> ; <comandos> | λ
	private void comandos(Attributes att) throws CompilerException {

		//Produção substituída por (na notação de Wirth):
		//COMANDOS = [CMD ";"] {CMD ";"} .

		//sincronizar com firstSet(<cmd>) =
		//{read, write, while, if, ident, begin, repeat}
		syncSet = createSyncSet(); //extra
		syncSet.add(TokenType.KEYWORD_READ);
		syncSet.add(TokenType.KEYWORD_WRITE);
		syncSet.add(TokenType.KEYWORD_IF);
		syncSet.add(TokenType.KEYWORD_WHILE);
		syncSet.add(TokenType.KEYWORD_REPEAT);
		syncSet.add(TokenType.KEYWORD_BEGIN);
		syncSet.add(TokenType.IDENTIFIER);

		//verificar o primeiro comando à parte
		//followSet(<comandos>) = {end, until}
		switch(token.getType()) {
			case KEYWORD_END:
			case KEYWORD_UNTIL:
				break;

			default:
				//sincronizar com {;}
				syncSet = createSyncSet();
				syncSet.add(TokenType.SYMBOL_SEMICOLON);
				cmd(att);
				returnSyncSet();

				terminal(TokenType.SYMBOL_SEMICOLON);
		}

		while(inFirstSetOfCMD()) {
			//sincronizar com {;}
			syncSet = createSyncSet();
			syncSet.add(TokenType.SYMBOL_SEMICOLON);
			cmd(att);
			returnSyncSet();

			terminal(TokenType.SYMBOL_SEMICOLON);
		}

		returnSyncSet();
		return;
	}

	//avalia se o token está no firstSet(<cmd>) =
	//{read, write, while, if, ident, begin, repeat}
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
	private void cmd(Attributes att) throws CompilerException {

		//não sincronizar o primeiro símbolo
		Attributes cmdAtt = createAttributes();

		switch(token.getType()) {

			// <cmd> ::= read ( <variaveis> ) | write ( <variaveis> )
			case KEYWORD_READ:
			case KEYWORD_WRITE: {
				cmdAtt.setToken(token);

				token = lexer.next();

				//sincronizar com firstSet(<variaveis>) = {ident}
				syncSet = createSyncSet();
				syncSet.add(TokenType.IDENTIFIER);
				terminal(TokenType.SYMBOL_LPAR);
				returnSyncSet();

				//sincronizar com {)}
				syncSet = createSyncSet();
				syncSet.add(TokenType.SYMBOL_RPAR);
				variaveis(cmdAtt);
				returnSyncSet();

				//verificar as declarações dos argumentos
				cmdAtt.setScope(att.getScope());
				rwSemanticCheck(cmdAtt);

				terminal(TokenType.SYMBOL_RPAR);
			} break;

			// <cmd> ::= while <condicao> do <cmd>
			case KEYWORD_WHILE: {
				token = lexer.next();

				//sincronizar com {do}
				syncSet = createSyncSet();
				syncSet.add(TokenType.KEYWORD_DO);
				condicao(att);
				returnSyncSet();

				//sincronizar com firstSet(<cmd>) =
				//{read, write, while, if, ident, begin, repeat}
				syncSet = createSyncSet();
				syncSet.add(TokenType.KEYWORD_READ);
				syncSet.add(TokenType.KEYWORD_WRITE);
				syncSet.add(TokenType.KEYWORD_WHILE);
				syncSet.add(TokenType.KEYWORD_IF);
				syncSet.add(TokenType.IDENTIFIER);
				syncSet.add(TokenType.KEYWORD_BEGIN);
				syncSet.add(TokenType.KEYWORD_REPEAT);
				terminal(TokenType.KEYWORD_DO);
				returnSyncSet();

				cmd(att);
			} break;

			// <cmd> ::= if <condicao> then <cmd> <pfalsa>
			case KEYWORD_IF: {
				token = lexer.next();

				//sincronizar com {then}
				syncSet = createSyncSet();
				syncSet.add(TokenType.KEYWORD_THEN);
				condicao(att);
				returnSyncSet();

				//sincronizar com firstSet(<cmd>) =
				//{read, write, while, if, ident, begin, repeat}
				syncSet = createSyncSet();
				syncSet.add(TokenType.KEYWORD_READ);
				syncSet.add(TokenType.KEYWORD_WRITE);
				syncSet.add(TokenType.KEYWORD_WHILE);
				syncSet.add(TokenType.KEYWORD_IF);
				syncSet.add(TokenType.IDENTIFIER);
				syncSet.add(TokenType.KEYWORD_BEGIN);
				syncSet.add(TokenType.KEYWORD_REPEAT);
				terminal(TokenType.KEYWORD_THEN);
				returnSyncSet();

				//sincronizar com firstSet(<p_falsa>) U
				//followSet(<p_falsa>) = {else, λ} U {else, ;}
				syncSet = createSyncSet();
				syncSet.add(TokenType.KEYWORD_ELSE);
				syncSet.add(TokenType.SYMBOL_SEMICOLON);
				cmd(att);
				returnSyncSet();

				pfalsa(att);
			} break;

			// <cmd> ::= ident <pos_ident>
			case IDENTIFIER: {
				Token t = token;
				token = lexer.next();

				cmdAtt.setToken(t);
				cmdAtt.setScope(att.getScope());
				pos_ident(cmdAtt);
			} break;

			// <cmd> ::= begin <comandos> end
			case KEYWORD_BEGIN: {
				token = lexer.next();

				//sincronizar com {end}
				syncSet = createSyncSet();
				syncSet.add(TokenType.KEYWORD_END);
				comandos(att);
				returnSyncSet();

				terminal(TokenType.KEYWORD_END);
			} break;

			// <cmd> ::= repeat <comandos> until <condicao>
			case KEYWORD_REPEAT: {
				token = lexer.next();

				//sincronizar com {until}
				syncSet = createSyncSet();
				syncSet.add(TokenType.KEYWORD_UNTIL);
				comandos(att);
				returnSyncSet();

				//sincronizar com firstSet(<condicao>) =
				//{+, -, ident, numero_int, numero_real, (}
				syncSet = createSyncSet();
				syncSet.add(TokenType.SYMBOL_PLUS);
				syncSet.add(TokenType.SYMBOL_MINUS);
				syncSet.add(TokenType.IDENTIFIER);
				syncSet.add(TokenType.CONST_INTEGER);
				syncSet.add(TokenType.CONST_REAL);
				syncSet.add(TokenType.SYMBOL_LPAR);
				terminal(TokenType.KEYWORD_UNTIL);
				returnSyncSet();

				condicao(att);
			} break;

			default:
				handleError("Era esperado um comando.");
				break;
		}

		returnAttributes(cmdAtt);
		return;
	}

	//efetua a análise semântica sobre os comandos "read" e "write"
	private void rwSemanticCheck(Attributes att) {
		Symbol arg; DataType type = null;
		boolean sameType = true;

		for(Symbol argument : att.getVariables()) {
			argument.setScope(att.getScope());
			try {
				arg = symbols.getElement(argument);
				if(type == null)
					type = arg.getType();

				switch(arg.getCategory()) {
					case PROGRAM:
						errors.add(new SemanticError(arg.getToken(),
								"O programa não é uma variável."));
						break;

					case PROCEDURE:
						errors.add(new SemanticError(arg.getToken(),
								"Um procedimento não é uma variável."));
						break;

					case VARIABLE:
					case PARAMETER:
						if(!type.equals(arg.getType()))
							sameType = false;
						break;
				}
			} catch(SemanticException e) {
				errors.add(new SemanticError(argument.getToken(),
						"Argumento não declarado."));
			}
		}

		if(!sameType) {
			errors.add(new SemanticError(att.getToken(),
					"Este procedimento requer argumentos do mesmo tipo."));
		}
		return;
	}

	//20. <condicao> ::= <expressao> <relacao> <expressao>
	private void condicao(Attributes att) throws CompilerException {

		DataType leftType, rightType, evalType;
		Operator operator;

		Attributes condAtt = createAttributes();
		condAtt.setScope(att.getScope());

		//sincronizar com firstSet(<relacao>) =
		//{=, <>, >=, <=, >, <}
		syncSet = createSyncSet();
		syncSet.add(TokenType.SYMBOL_EQ);
		syncSet.add(TokenType.SYMBOL_NE);
		syncSet.add(TokenType.SYMBOL_GE);
		syncSet.add(TokenType.SYMBOL_LE);
		syncSet.add(TokenType.SYMBOL_GT);
		syncSet.add(TokenType.SYMBOL_LT);
		expressao(condAtt);
		returnSyncSet();

		leftType = condAtt.getType();
		condAtt.clear();

		//sincronizar com firstSet(<expressao>) =
		//{+, -, ident, numero_int, numero_real, (}
		syncSet = createSyncSet();
		syncSet.add(TokenType.SYMBOL_PLUS);
		syncSet.add(TokenType.SYMBOL_MINUS);
		syncSet.add(TokenType.IDENTIFIER);
		syncSet.add(TokenType.CONST_INTEGER);
		syncSet.add(TokenType.CONST_REAL);
		syncSet.add(TokenType.SYMBOL_LPAR);
		relacao(condAtt);
		returnSyncSet();

		operator = condAtt.getOperator();
		condAtt.clear();
		condAtt.setScope(att.getScope());

		expressao(condAtt);

		rightType = condAtt.getType();

		//resolução de tipos
		if(localErrorFlag == false) {
			evalType = (leftType == rightType)?
				leftType: DataType.REAL;
		}

		returnAttributes(condAtt);
		return;
	}

	//21. <relacao> ::= = | <> | >= | <= | > | <
	private void relacao(Attributes att) throws CompilerException {

		//análise semântica
		switch(token.getType()) {
			case SYMBOL_EQ: att.setOperator(Operator.REL_EQ); break;
			case SYMBOL_NE: att.setOperator(Operator.REL_NE); break;
			case SYMBOL_GE: att.setOperator(Operator.REL_GE); break;
			case SYMBOL_LE: att.setOperator(Operator.REL_LE); break;
			case SYMBOL_GT: att.setOperator(Operator.REL_GT); break;
			case SYMBOL_LT: att.setOperator(Operator.REL_LT); break;
		}

		//análise sintática
		switch(token.getType()) {
			case SYMBOL_EQ: case SYMBOL_NE:
			case SYMBOL_GE: case SYMBOL_LE:
			case SYMBOL_GT: case SYMBOL_LT:
				token = lexer.next();
				break;

			default:
				handleError("Era esperado um operador relacional.");
		}
		return;
	}

	//22. <expressao> ::= <termo> <outros_termos>
	//24. <outros_termos> ::= <op_ad> <termo> <outros_termos> | λ
	private void expressao(Attributes att) throws CompilerException {
		//Produção substituída por (na notação de Wirth):
		//EXPRESSAO = TERMO {OP_AD TERMO} .

		DataType evalType, nextType;
		Operator operator;

		Attributes expAtt = createAttributes();
		expAtt.setScope(att.getScope());

		//sincronizar com firstSet(<op_ad>) = {+, -}
		syncSet = createSyncSet();
		syncSet.add(TokenType.SYMBOL_PLUS);
		syncSet.add(TokenType.SYMBOL_MINUS);
		termo(expAtt);
		returnSyncSet();

		evalType = expAtt.getType();

		while(inFirstSetOfOP_AD()) {
			expAtt.clear();

			//sincronizar com firstSet(<termo>) = 
			//{+, -, ident, numero_int, numero_real, (}
			syncSet = createSyncSet();
			syncSet.add(TokenType.SYMBOL_PLUS);
			syncSet.add(TokenType.SYMBOL_MINUS);
			syncSet.add(TokenType.IDENTIFIER);
			syncSet.add(TokenType.CONST_INTEGER);
			syncSet.add(TokenType.CONST_REAL);
			syncSet.add(TokenType.SYMBOL_LPAR);
			op_ad(expAtt);
			returnSyncSet();

			operator = expAtt.getOperator();
			expAtt.clear();
			expAtt.setScope(att.getScope());

			//sincronizar com firstSet(<op_ad>) = {+, -}
			syncSet = createSyncSet();
			syncSet.add(TokenType.SYMBOL_PLUS);
			syncSet.add(TokenType.SYMBOL_MINUS);
			termo(expAtt);
			returnSyncSet();

			nextType = expAtt.getType();

			if(localErrorFlag == false) {
				if(evalType != nextType)
					evalType = DataType.REAL;
			}
		}

		att.setType(evalType);
		returnAttributes(expAtt);
		return;
	}

	//23. <op_un> ::= + | - | λ
	private void op_un(Attributes att) throws CompilerException {

		//análise semântica
		switch(token.getType()) {
			case SYMBOL_MINUS:
				att.setOperator(Operator.UNARY_MINUS);
				break;
			default:
				att.setOperator(Operator.UNARY_PLUS);
				break;
		}

		//análise sintática
		switch(token.getType()) {
			case SYMBOL_PLUS:
			case SYMBOL_MINUS:
				token = lexer.next();
				break;
			default: //sinal omitido
				break;
		}

		return;
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
	private void op_ad(Attributes att) throws CompilerException {

		//análise semântica
		switch(token.getType()) {
			case SYMBOL_PLUS:
				att.setOperator(Operator.ADDITION);
				break;

			case SYMBOL_MINUS:
				att.setOperator(Operator.SUBTRACTION);
				break;

			default:
		}

		//análise sintática
		switch(token.getType()) {
			case SYMBOL_PLUS:
			case SYMBOL_MINUS:
				token = lexer.next();
				break;
			default:
				handleError("Era esperado um operador aditivo.");
		}

		return;
	}

	//26. <termo> ::= <op_un> <fator> <mais_fatores>
	//27. <mais_fatores> ::= <op_mul> <fator> <mais_fatores> | λ
	private void termo(Attributes att) throws CompilerException {
		//Produção substituída por (na notação de Wirth):
		//TERMO = OP_UN FATOR {OP_MUL FATOR} .

		DataType evalType, nextType;
		Operator unaryOperator, operator;

		Attributes a = createAttributes();

		//sincronizar com firstSet(<fator>) =
		//{ident, numero_int, numero_real, (}
		syncSet = createSyncSet();
		syncSet.add(TokenType.IDENTIFIER);
		syncSet.add(TokenType.CONST_INTEGER);
		syncSet.add(TokenType.CONST_REAL);
		syncSet.add(TokenType.SYMBOL_LPAR);
		op_un(a);
		returnSyncSet();

		unaryOperator = a.getOperator();
		a.clear();
		a.setScope(att.getScope());

		//sincronizar com firstSet(<op_mul>) =
		//{ident, numero_int, numero_real, (}
		syncSet = createSyncSet();
		syncSet.add(TokenType.SYMBOL_MULT);
		syncSet.add(TokenType.SYMBOL_DIV);
		fator(a);
		returnSyncSet();

		evalType = a.getType();

		while(inFirstSetOifOP_MUL()) {
			a.clear();
			a.setScope(att.getScope());

			//sincronizar com firstSet(<fator>) =
			//{ident, numero_int, numero_real, (}
			syncSet = createSyncSet();
			syncSet.add(TokenType.IDENTIFIER);
			syncSet.add(TokenType.CONST_INTEGER);
			syncSet.add(TokenType.CONST_REAL);
			syncSet.add(TokenType.SYMBOL_LPAR);
			op_mul(a);
			returnSyncSet();

			operator = a.getOperator();

			//sincronizar com firstSet(<op_mul>) =
			//{ident, numero_int, numero_real, (}
			syncSet = createSyncSet();
			syncSet.add(TokenType.SYMBOL_MULT);
			syncSet.add(TokenType.SYMBOL_DIV);
			fator(a);
			returnSyncSet();

			nextType = a.getType();

			//resolução de tipos
			if(localErrorFlag == false) {
				switch(operator) {
					case MULTIPLICATION:
						if(evalType != nextType)
							evalType = DataType.REAL;
						break;

					case DIVISION:
						if(evalType == DataType.INTEGER &&
								nextType == DataType.INTEGER) {
							evalType = DataType.INTEGER;
						} else {
							errors.add(new SemanticError(a.getToken(),
									"A divisão é permitida apenas " +
									"entre valores inteiros."));
						}
						break;
				}
			}
		}

		att.setType(evalType);
		returnAttributes(a);
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
	private void op_mul(Attributes att) throws CompilerException {

		//análise semântica
		switch(token.getType()) {
			case SYMBOL_MULT:
				att.setOperator(Operator.MULTIPLICATION);
				break;

			case SYMBOL_DIV:
				att.setOperator(Operator.DIVISION);
				break;

			default:
		}

		//análise sintática
		switch(token.getType()) {
			case SYMBOL_MULT:
			case SYMBOL_DIV:
				att.setToken(token);
				token = lexer.next();
				break;
			default:
				handleError("Era esperado um operador multiplicativo.");
		}

		return;
	}

	//29. <fator> ::= ident | numero_int | numero_real | ( <expressao> )
	private void fator(Attributes att) throws CompilerException {

		//não sincronizar o primeiro símbolo

		switch(token.getType()) {
			case IDENTIFIER:
				//verificar declaração da variável
				try {
					Symbol ident = symbols.getElement(att.getScope(), token);
					switch(ident.getCategory()) {
						case PROGRAM:
							errors.add(new SemanticError(token,
									"O programa não é uma variável."));
							break;

						case PROCEDURE:
							errors.add(new SemanticError(token,
									"Um procedimento não é uma variável."));
							break;

						case PARAMETER:
						case VARIABLE:
							att.setType(ident.getType());
							break;
					}
				} catch(SemanticException e) {
					errors.add(new SemanticError(token,
							"Variável não declarada."));
				}
				token = lexer.next();
				break;

			case CONST_INTEGER:
				token = lexer.next();
				att.setType(DataType.INTEGER);
				break;

			case CONST_REAL:
				token = lexer.next();
				att.setType(DataType.REAL);
				break;

			case SYMBOL_LPAR:
				token = lexer.next();

				Attributes a = createAttributes();
				a.setScope(att.getScope());

				//sincronizar com firstSet(<expressao>) =
				//{+, -, ident, numero_int, numero_real, (}
				syncSet = createSyncSet();
				syncSet.add(TokenType.SYMBOL_PLUS);
				syncSet.add(TokenType.SYMBOL_MINUS);
				syncSet.add(TokenType.IDENTIFIER);
				syncSet.add(TokenType.CONST_INTEGER);
				syncSet.add(TokenType.CONST_REAL);
				syncSet.add(TokenType.SYMBOL_LPAR);
				expressao(a);
				returnSyncSet();

				terminal(TokenType.SYMBOL_RPAR);

				if(localErrorFlag == false) {
					att.setType(a.getType());
				}

				returnAttributes(a);
				break;

			default:
				handleError("Era esperado um operando.");
		}
		return;
	}

	//avalia um símbolo terminal
	private Token terminal(TokenType expectedType) throws CompilerException {
		if(token.getType() == expectedType) {
			Token accepted = token;
			token = lexer.next();
			return accepted;
		} else {
			if(token.getType() == TokenType.EOF) {
				errors.add(new SyntacticError(token,
						"Não era esperado o fim do arquivo."));
				localErrorFlag = true;
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

				handleError(message);
			}
			return null;
		}
	}

	//avalia um símbolo terminal opcional
	//retorna true, se o símbolo for encontrado
	private boolean optional(TokenType expectedType) throws CompilerException {
		if(token.getType() == expectedType) {
			token = lexer.next();
			return true;
		} else
			return false;
	}

	//tratamento de erros
	private void handleError(String message) throws CompilerException {
		errors.add(new SyntacticError(token, message));

		syncSet = createSet();
		for(Set<TokenType> s : syncStack)
			syncSet.addAll(s);

		while(!syncSet.contains(token.getType())) {
			token = lexer.next();
		}

		returnSet(syncSet);
		localErrorFlag = true;
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

	//poço de atributos
	private Deque<Attributes> attPool;

	//cria um mapeamento de atributos vazio
	private Attributes createAttributes() {
		return attPool.isEmpty()?
			new Attributes(): attPool.pop();
	}

	//retorna um mapeamento de atributos ao poço
	private void returnAttributes(Attributes att) {
		att.clear();
		attPool.push(att);
		return;
	}

	/**
	 * {@inheritDoc }
	 */
	public List<CompilationError> getErrors() {
		return Collections.unmodifiableList(errors);
	}

}
