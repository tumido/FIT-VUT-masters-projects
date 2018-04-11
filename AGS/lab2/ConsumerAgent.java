import jade.core.*;
import jade.core.behaviours.*;
import jade.domain.DFService;
import jade.domain.FIPAAgentManagement.DFAgentDescription;
import jade.domain.FIPAAgentManagement.SearchConstraints;
import jade.domain.FIPAAgentManagement.ServiceDescription;
import jade.domain.FIPAException;
import jade.lang.acl.*;

public class ConsumerAgent extends Agent {
  protected void setup() {
    System.out.println("Consumer agent " + getAID().getName() + " is ready.");

    SequentialBehaviour seq = new SequentialBehaviour();

    seq.addSubBehaviour(new WakerBehaviour( this, 1000 ) {
      protected void handleElapsedTimeout() {
        System.out.println( "Consumer agent " + getAID().getName() + " waited long enough." );
      }
    });

    seq.addSubBehaviour(new TickerBehaviour(this, 1000) {
      protected void onTick() {
        try {
          DFAgentDescription template = new DFAgentDescription();
          DFAgentDescription[] results = DFService.search(myAgent, template, null);
          if (results.length > 0) {
            ACLMessage msg = new ACLMessage(ACLMessage.INFORM);
            DFAgentDescription dfd = results[0];
            msg.addReceiver(dfd.getName());
            msg.setContent("Message #"+getTickCount());
            myAgent.send(msg);
            System.out.println("Consumer " + myAgent.getAID().getName() + " sent msg.");
          }
          else {
            System.out.println("Consumer " + myAgent.getAID().getName() + " consumed all services.");
            myAgent.doDelete();
          }
        }
      	catch (FIPAException fe) {
      		fe.printStackTrace();
      	}
      }
    });

    addBehaviour(seq);
  }
}
