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
    
    System.out.println("Participant " + getAID().getName() + " has created a service."); 
    addBehaviour(new WaitAndRespondBehaviour());
  } 
}

class WaitAndRespondBehaviour extends CyclicBehaviour{
  public void action() {
    ACLMessage msg = myAgent.receive();  
    if (msg == null){ 
      block();
    }
    else{
      System.out.println("Participant " + myAgent.getAID().getName() + " received message.");
    }
  }
}
  

  
