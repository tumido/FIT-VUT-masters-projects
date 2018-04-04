import jade.core.*; 
import jade.core.behaviours.*;
import jade.lang.acl.*;

public class SenderAgent extends Agent { 
  protected void setup() { 
    System.out.println("Sender agent " + getAID().getName() + " is ready."); 
    addBehaviour(new TickerBehaviour(this, 1000) {
      protected void onTick() {
        ACLMessage msg = new ACLMessage(ACLMessage.INFORM); 
        msg.addReceiver(new AID("Receiver", AID.ISLOCALNAME)); 
        msg.setContent("Message #"+getTickCount()); 
        myAgent.send(msg);  
        System.out.println("Sender agent " + myAgent.getAID().getName() + " sent msg.");
        if (getTickCount() >= 100) myAgent.doDelete();
     }   
    });
  } 
}
