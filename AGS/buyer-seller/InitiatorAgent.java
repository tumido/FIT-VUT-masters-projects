import jade.core.Agent;
import jade.core.behaviours.*;
import jade.lang.acl.*;

public class InitiatorAgent extends Agent {
  protected void setup() { 
    System.out.println("Initiator " + getAID().getName() + " is ready."); 
    SequentialBehaviour seq = new SequentialBehaviour();
    seq.addSubBehaviour(new WakerBehaviour( this, 1000 ) {
        protected void handleElapsedTimeout() {
         System.out.println( "Initiator " + getAID().getName() + " waited long enough." );
        }
    });
    seq.addSubBehaviour(new AskForProposalBehaviour());
    addBehaviour(seq);
  } 
}

class AskForProposalBehaviour extends OneShotBehaviour{
  public void action() {
    
  }
}