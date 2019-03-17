package com.github.klangfarbe.statechart;

public class Condition extends PseudoState {
    public Condition(String name, Context parent, Guard guard, State whenTrue, State otherwise) throws StatechartException {
        super(name, parent, PseudoState.pseudostate_junction);
        new Transition(this, whenTrue, guard);
        new Transition(this, otherwise);
    }
}
