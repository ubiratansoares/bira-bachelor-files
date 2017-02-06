package compiler.t3a;

import compiler.*;
import compiler.Compiler;

import java.util.List;

import java.io.FileNotFoundException;
import java.io.IOException;

/**
 * Interface em terminal de comandos do compilador.
 *
 * @version 3.0.0
 */
public class Main {

    /**
	 * Ponto de entrada da aplicação.
	 *
     * @param args the command line arguments
     */
    public static void main(String[] args) {

		SourceCode source = new SourceCode();
		Compiler compiler = new CompilerImpl();
		ObjectCode object = null;

		//leitura do arquivo de código-fonte
		try {
			source.load(args[0]);
		} catch(ArrayIndexOutOfBoundsException e) {
			System.out.println("Parametros: <arquivo fonte> [<arquivo objeto>]");
			return;
		} catch(FileNotFoundException e) {
			System.out.println("Arquivo \"" + args[0] + "\" nao encontrado.");
			return;
		} catch(IOException e) {
			System.out.println("Falha ao ler arquivo \"" + args[0] + "\".");
			return;
		}

		//compilação
		try {
			compiler.init(args);
			System.out.println("Compilando \"" + args[0] + "\"...");
			object = compiler.compile(source);
		} catch(LexicalException e) {
			int line = e.getToken().getLine() + 1;
			int column = e.getToken().getColumn() + 1;
			System.out.printf("Erro léxico na linha %d, coluna %d: %s%n",
					line, column, e.getMessage());
			System.out.println("Compilação abortada.");
			return;
		} catch(CompilerException e) {
			e.printStackTrace();
			return;
		}

		//listar erros sintáticos
		List<CompilationError> errors = ((CompilerImpl) compiler).getErrors();
		for(CompilationError e : errors) {
			int line = e.getToken().getLine() + 1;
			int column = e.getToken().getColumn() + 1;
			if(e instanceof SyntacticError) {
				System.out.printf(
						"Erro sintático na linha %d, coluna %d: %s%n",
						line, column, e.getMessage());
			} else if(e instanceof SemanticError) {
				System.out.printf(
						"Erro semântico na linha %d, coluna %d: %s%n",
						line, column, e.getMessage());
			}
		}

		if(errors.isEmpty())
			System.out.println("Compilação bem-sucedida.");
		else
			System.out.println("Compilação abortada.");
		
		//escrita do arquivo de "código-objeto" (fachada)
		try {
			if(args.length > 1 && object != null) {
				object.println();
				object.writeFile(args[1]);
				System.out.println("Escrito arquivo \"" + args[1] + "\".");
			}
		} catch(IOException e) {
			System.out.println(
					"Falha ao escrever arquivo \"" + args[1] + "\".");
			return;
		}

        return;
    }

}
