import jade.core.Agent;
import jade.core.behaviours.*;
import jade.domain.FIPAAgentManagement.DFAgentDescription;
import jade.domain.DFService;
import jade.domain.FIPAException;
import jade.lang.acl.*;
import java.util.Date;
import java.util.List;
import java.util.ArrayList;
import java.util.Comparator;
import java.util.Collections;

public class InitiatorAgent extends Agent {
  protected static int TIMEOUT = 10000;
  protected Date timeout_stamp;
  protected List<ACLMessage> proposals = new ArrayList<ACLMessage>();

  protected void setup() {
    System.out.println("Initiator " + getAID().getName() + ": ready");
    SequentialBehaviour seq = new SequentialBehaviour();

    // Initial delay on the initiator side
    seq.addSubBehaviour(new WakerBehaviour( this, 1000 ) {
        protected void handleElapsedTimeout() {
         System.out.println( "Initiator " + getAID().getName() + ": waited long enough" );
        }
    });
    // Send out a message that initiator is accepting proposals
    seq.addSubBehaviour(new AskForProposalBehaviour());
    // Collect proposals form participants
    seq.addSubBehaviour(new WaitForProposalsBehaviour());
    // Accept the best proposal
    seq.addSubBehaviour(new AcceptProposalBehaviour());
    // Wait for a complete project
    seq.addSubBehaviour(new WaitForProjectDoneBehaviour());

    addBehaviour(seq);
  }
}

class AskForProposalBehaviour extends OneShotBehaviour {
  public void action() {
    ACLMessage cfp = new ACLMessage(ACLMessage.CFP);

    // Search and collect all participant services
    DFAgentDescription template = new DFAgentDescription();
    try {
      DFAgentDescription[] results = DFService.search(myAgent, template, null);
      for (DFAgentDescription agent : results) cfp.addReceiver(agent.getName());
    } catch (FIPAException fe) {
      fe.printStackTrace();
    }

    // Set timeout
    cfp.setContent(Integer.toString(InitiatorAgent.TIMEOUT));
    ((InitiatorAgent) myAgent).timeout_stamp = new Date(System.currentTimeMillis() + InitiatorAgent.TIMEOUT);

    // Distribute proposal request
    myAgent.send(cfp);

    System.out.println("Initiator " + myAgent.getAID().getName() + ": Ask for proposal sent");
  }
}

class WaitForProposalsBehaviour extends SimpleBehaviour {
  public void action() {
    // Listen for message
    ACLMessage msg = myAgent.receive();
    if (msg == null) {
      block();
      return;
    }

    // If message contains a proposal, collect it
    if (msg.getPerformative() == ACLMessage.PROPOSE) {
      System.out.println("Initiator " + myAgent.getAID().getName() + ": Received proposal from " + msg.getSender().getName());
      ((InitiatorAgent) myAgent).proposals.add(msg);
    }
  }

  public boolean done() {
    // Stop when timeout is reached
    if (new Date().after(((InitiatorAgent) myAgent).timeout_stamp)) {
      System.out.println("Initiator " + myAgent.getAID().getName() + ": No longer accepting proposals");
      return true;
    }
    return false;
  }
}

class AcceptProposalBehaviour extends OneShotBehaviour {
  public void action() {
    // Select the best offer
    ACLMessage accept_offer = Collections.min(((InitiatorAgent) myAgent).proposals, Comparator.comparing(ACLMessage::getContent));
    if (accept_offer == null) return;

    // Accept the selected proposal
    ACLMessage accept_msg = accept_offer.createReply();
    accept_msg.setPerformative(ACLMessage.ACCEPT_PROPOSAL);
    myAgent.send(accept_msg);
    System.out.println("Initiator " + myAgent.getAID().getName() + ": Accepted proposal from " + accept_offer.getSender().getName());

    // Distribute a reject message to all other services
    ACLMessage reject_msg = new ACLMessage(ACLMessage.REJECT_PROPOSAL);
    for (ACLMessage reject_offer: ((InitiatorAgent) myAgent).proposals) {
      if (reject_offer != accept_offer)
        reject_msg.addReceiver(reject_offer.getSender());
    }
    myAgent.send(reject_msg);

    System.out.println("Initiator " + myAgent.getAID().getName() + ": Rejected offers from others");
  }
}

class WaitForProjectDoneBehaviour extends CyclicBehaviour {
  public void action() {
    // Listen for message
    ACLMessage msg = myAgent.receive();
    if (msg == null) {
      block();
      return;
    }

    // If message contains a information about complete project, collect it
    if (msg.getPerformative() == ACLMessage.INFORM) {
      System.out.println("Initiator " + myAgent.getAID().getName() + ": Received complated project with message: " + msg.getContent());
      System.out.println("Initiator " + myAgent.getAID().getName() + ": Exiting");
      myAgent.doDelete();
    }
  }
}
