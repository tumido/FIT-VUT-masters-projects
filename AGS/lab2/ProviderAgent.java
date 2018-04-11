import jade.core.Agent;
import jade.core.behaviours.*;
import jade.domain.DFService;
import jade.domain.FIPAAgentManagement.DFAgentDescription;
import jade.domain.FIPAAgentManagement.ServiceDescription;
import jade.domain.FIPAException;
import jade.domain.FIPANames;
import jade.lang.acl.*;

public class ProviderAgent extends Agent {
  protected void setup() {
    try {
      ServiceDescription sd = new ServiceDescription();
      sd.setName(getAID().getName());
      sd.setType("ags-provider-type");
      sd.addLanguages(FIPANames.ContentLanguage.FIPA_SL);

      DFAgentDescription dfd = new DFAgentDescription();
      dfd.setName(getAID());
      dfd.addServices(sd);

      DFService.register(this, dfd);
    }
    catch (FIPAException fe) {
      fe.printStackTrace();
    }

    System.out.println("Provider " + getAID().getName() + " has created a service.");
    addBehaviour(new SubscribeAndWaitForMsgBehaviour());
  }
}

class SubscribeAndWaitForMsgBehaviour extends CyclicBehaviour{
  public void action() {
    ACLMessage msg = myAgent.receive();
    if (msg == null){
      block();
      return;
    }

    try {
      DFService.deregister(myAgent);
    }
    catch (FIPAException fe) {
      fe.printStackTrace();
    }

    System.out.println("Provider " + myAgent.getAID().getName() + " received message. Service consumed.");
    myAgent.doDelete();
  }
}
