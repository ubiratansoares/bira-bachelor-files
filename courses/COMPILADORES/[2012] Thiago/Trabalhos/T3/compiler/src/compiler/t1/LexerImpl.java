package compiler.t1;

import compiler.LexicalAnalyzer;
import compiler.SourceCode;
import compiler.Token;

import compiler.lexer.*;

import compiler.LexicalException;

/**
 * Analisador léxico do compilador.
 *
 * @version 1.0.2
 */
public class LexerImpl extends LexicalAnalyzer {

	/**
	 * Construtor vazio.
	 */
	public LexerImpl() {
		super();
	}

	/**
	 * {@inheritDoc} Não define a posição do token ou do erro.
	 */
	public Token next() throws LexicalException {
		SourceCode source = this.getSourceCode();
		StringBuilder sb = new StringBuilder();
		char c; State state; boolean stop;

		//executa o autômato
		stop = false;
		state = StartState.getInstance();
		while(source.hasNext() && !stop) {
			c = source.getchar();
			state.updateString(c, sb);
			state = state.next(c);
			if(state instanceof StopState)
				stop = true;
		}

		//devolve um caracter ao buffer, caso o
		//autômato tenha efetuado forward-looking
		if(state.hasForwardLooked())
			source.ungetchar();

		Token token = state.getToken(sb.toString());
		token = this.checkKeyword(token);

		return token;
	}

}
