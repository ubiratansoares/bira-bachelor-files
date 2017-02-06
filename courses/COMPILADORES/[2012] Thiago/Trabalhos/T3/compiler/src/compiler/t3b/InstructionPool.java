package compiler.t3b;

import compiler.t3b.Instruction.Code;

import java.util.Deque;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.Map;

/**
 * Gerencia a criação de instruções.
 *
 */
public class InstructionPool {

	private Deque<Instruction> pool;
	private Map<Code, Instruction> flyweight;

	/**
	 * Cria um poço de objetos de instrução.
	 */
	public InstructionPool() {
		pool = new LinkedList<Instruction>();
		flyweight = new HashMap<Code, Instruction>();

		//instruções sem argumentos podem ser compartilhados

		//operações aritméticas e lógicas
		flyweight.put(Code.ADD, new Instruction(Code.ADD));
		flyweight.put(Code.SUB, new Instruction(Code.SUB));
		flyweight.put(Code.MLT, new Instruction(Code.MLT));
		flyweight.put(Code.DIV, new Instruction(Code.DIV));
		flyweight.put(Code.INV, new Instruction(Code.INV));
		flyweight.put(Code.NOT, new Instruction(Code.NOT));
		flyweight.put(Code.AND, new Instruction(Code.AND));
		flyweight.put(Code.OR, new Instruction(Code.OR));

		//comparações
		flyweight.put(Code.CPLT, new Instruction(Code.CPLT));
		flyweight.put(Code.CPGT, new Instruction(Code.CPGT));
		flyweight.put(Code.CPLE, new Instruction(Code.CPLE));
		flyweight.put(Code.CPGE, new Instruction(Code.CPGE));
		flyweight.put(Code.CPEQ, new Instruction(Code.CPEQ));
		flyweight.put(Code.CPNE, new Instruction(Code.CPNE));

		//outros
		flyweight.put(Code.JPR, new Instruction(Code.JPR));
		flyweight.put(Code.READ, new Instruction(Code.READ));
		flyweight.put(Code.WRITE, new Instruction(Code.WRITE));
		flyweight.put(Code.START, new Instruction(Code.START));
		flyweight.put(Code.STOP, new Instruction(Code.STOP));
		flyweight.put(Code.BIND, new Instruction(Code.BIND));
	}

	/**
	 * Obtém uma instrução do poço.
	 *
	 * @param code
	 * O código da instrução.
	 *
	 * @return
	 * Uma instância da instrução.
	 */
	public Instruction get(Code code) {
		return this.get(code, 0);
	}

	/**
	 * Obtém uma instrução do poço.
	 *
	 * @param code
	 * O código da instrução.
	 *
	 * @param arg
	 * O argumento da instrução.
	 *
	 * @return
	 * Uma instância da instrução.
	 */
	public Instruction get(Code code, Number arg) {
		if(flyweight.containsKey(code))
			return flyweight.get(code);
		else {
			if(pool.isEmpty()) {
				return new Instruction(code, arg);
			} else {
				Instruction i = pool.pop();
				i.reset(code, arg);
				return i;
			}
		}
	}

	/**
	 * Retorna uma instância ao poço.
	 *
	 * @param i
	 * A instância a ser devolvida.
	 */
	public void free(Instruction i) {
		if(!flyweight.containsKey(i.getCode()))
			pool.push(i);
		return;
	}

}
