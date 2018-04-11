import jade.core.Agent;
import jade.core.behaviours.*;
import jade.domain.DFService;
import jade.domain.FIPAAgentManagement.DFAgentDescription;
import jade.domain.FIPAAgentManagement.ServiceDescription;
import jade.domain.FIPAException;
import jade.domain.FIPANames;
import jade.lang.acl.*;

public class ParticipantAgent extends Agent {
  protected void setup() {
    System.out.println("Initiator " + getAID().getName() + ": ready");
    try {
        DFAgentDescription dfd = new DFAgentDescription();
        dfd.setName(getAID());
        ServiceDescription sd = new ServiceDescription();
        sd.setName(getAID().getName());
        sd.setType("ags-participant-type");
        sd.addLanguages(FIPANames.ContentLanguage.FIPA_SL);
        dfd.addServices(sd);
        DFService.register(this, dfd);
    }
    catch (FIPAException fe) {
        fe.printStackTrace();
    }

    System.out.println("Participant " + getAID().getName() + ": Service created");
    addBehaviour(new WaitAndRespondBehaviour());
  }
}

class WaitAndRespondBehaviour extends CyclicBehaviour{
  public void action() {
    ACLMessage msg = myAgent.receive();
    if (msg == null) {
      block();
      return;
    }

    System.out.println("Participant " + myAgent.getAID().getName() + ": Message received");
    switch (msg.getPerformative()) {
      case ACLMessage.CFP:
        respond_to_cfp(msg); break;
      case ACLMessage.ACCEPT_PROPOSAL:
        respond_to_accepted(msg); break;
      case ACLMessage.REJECT_PROPOSAL:
        System.out.println("Participant " + myAgent.getAID().getName() + ": Our proposal was rejected");
        kill_service();
        break;
      default:
        System.out.println("Participant " + myAgent.getAID().getName() + ": MSG type not recognised");
        kill_service();
    }
  }

  protected void respond_to_cfp(ACLMessage msg) {
    System.out.println("Participant " + myAgent.getAID().getName() + ": Received CFP");
    double dice = Math.random();

    ACLMessage response = msg.createReply();

    // Decide whether and what to respond
    if (dice < 0.4) {
      System.out.println("Participant " + myAgent.getAID().getName() + ": Decided to accept CFP");
      response.setPerformative(ACLMessage.PROPOSE);
    } else if (dice < 0.8) {
      System.out.println("Participant " + myAgent.getAID().getName() + ": Decided to reject CFP");
      response.setPerformative(ACLMessage.REFUSE);
    } else {
      System.out.println("Participant " + myAgent.getAID().getName() + ": Decided to ignore CFP");
      kill_service();
      return;
    }

    // Set price and delay
    int price = (int) Math.ceil(Math.random() * 100);
    int delay = (int) Math.ceil(Math.random() * 15000);

    System.out.println("Participant " + myAgent.getAID().getName() + ": Offering $" + Integer.toString(price) + ", after a delay of " + Integer.toString(delay));
    response.setContent(Integer.toString(price));
    try { Thread.sleep(delay); } catch (InterruptedException ex) {}

    myAgent.send(response);

    if (response.getPerformative() == ACLMessage.REFUSE) kill_service();
  }

  protected void respond_to_accepted(ACLMessage msg) {
    System.out.println("Participant " + myAgent.getAID().getName() + ": Our proposal was accepted");

    ACLMessage response = msg.createReply();
    response.setPerformative(ACLMessage.INFORM);

    System.out.println("Participant " + myAgent.getAID().getName() + ": Working on project");
    try { Thread.sleep(5000); } catch (InterruptedException ex) {}
    System.out.println("Participant " + myAgent.getAID().getName() + ": Project done");
    response.setContent("DONE");

    myAgent.send(response);
    kill_service();
  }

  protected void kill_service() {
    System.out.println("Participant " + myAgent.getAID().getName() + ": Exiting");
    try { DFService.deregister(myAgent); } catch (FIPAException fe) { fe.printStackTrace(); }
    myAgent.doDelete();
  }
}
