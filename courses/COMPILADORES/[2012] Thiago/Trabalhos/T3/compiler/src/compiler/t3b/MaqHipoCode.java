package compiler.t3b;

import compiler.*;
import compiler.t3b.Instruction.Code;

import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.PrintWriter;
import java.io.Writer;
import java.io.IOException;

import java.util.List;
import java.util.LinkedList;

/**
 * Código-Objeto para a MaqHipo.
 *
 */
public class MaqHipoCode implements ObjectCode {

	private List<Instruction> code;
	private InstructionPool pool;
	private boolean halted;

	private int stackPointer;

	private static final int DATA_START_ADDRESS = 0;

	/**
	 * Cria um código-objeto vazio.
	 */
	public MaqHipoCode() {
		code = new LinkedList<Instruction>();
		pool = new InstructionPool();
		halted = false;

		stackPointer = DATA_START_ADDRESS;
	}

	/**
	 * Escreve o conteúdo do objeto na saída padrão.
	 */
	public void println() {
		if(halted) {
			System.out.println("Não foi gerado um código-objeto.");
		} else {
			System.out.println("[início do código-objeto]");
			for(int i = 0; i < code.size(); i++)
				System.out.printf("%4d  %s%n", i, code.get(i));
			System.out.println("[fim do código-objeto]");
		}
		return;
	}

	/**
	 * Escreve o conteúdo do objeto no arquivo especificado.
	 *
	 * @param filename
	 * O nome do arquivo onde escrever.
	 *
	 * @throws IOException
	 * Lançada caso ocorra qualquer problema na escrita.
	 */
	public void writeFile(String filename) throws IOException {
		Writer stream = new BufferedWriter(new FileWriter(filename));
		PrintWriter out = new PrintWriter(stream);

		for(int i = 0; i < code.size(); i++)
			out.printf("%4d  %s%n", i, code.get(i));

		out.flush();
		out.close();
		return;
	}

	/**
	 * Apaga todo o código-objeto e metadados.
	 */
	public void clear() {
		code.clear();
		halted = false;
		stackPointer = DATA_START_ADDRESS;
		return;
	}

	/**
	 * Interrompe a geração de código-objeto.
	 */
	public void cancel() {
		halted = true;
	}

	/**
	 * Verifica se a geração de código está parada.
	 *
	 * @return
	 * {@code true} se a geração de código foi interrompida.
	 */
	public boolean isCancelled() {
		return halted;
	}

	/**
	 * Insere uma instrução no fim do código-objeto.
	 *
	 * @param inst
	 * A instrução a ser inserida.
	 */
	public Instruction append(Code inst) {
		return this.append(inst, 0);
	}

	/**
	 * Insere uma instrução no fim do código-objeto.
	 *
	 * @param inst
	 * A instrução a ser inserida.
	 *
	 * @param arg
	 * O argumento da instrução a ser inserida.
	 */
	public Instruction append(Code inst, Number arg) {
		if(halted) return pool.get(Code.STOP);
		Instruction i = pool.get(inst, arg);
		code.add(i);
		return i;
	}

	/**
	 * Define a posição do ponteiro da pilha de dados.
	 *
	 * @param address
	 * O novo endereço apontado pelo ponteiro da pilha.
	 */
	public void setStackPointer(int address) {
		stackPointer = address;
		return;
	}

	/**
	 * Obtém o endereço do topo da pilha.
	 *
	 * @return
	 * O endereço atual do topo da pilha.
	 */
	public int getStackPointer() {
		return stackPointer;
	}

	/**
	 * Move o ponteiro da pilha de algumas posições.
	 *
	 * @param delta
	 * O número de palavras a mover.
	 */
	public void moveStackPointer(int delta) {
		stackPointer += delta;
		return;
	}

	/**
	 * Obtém o endereço da próxima instrução que
	 * será inserida no código-objeto.
	 *
	 * @return
	 * O tamanho atual do código-objeto.
	 */
	public int getNextInstructionAddress() {
		return code.size();
	}

}
