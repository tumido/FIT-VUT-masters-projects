import jade.core.Agent;
import jade.core.behaviours.*;
import jade.lang.acl.*;
import java.util.Date;
import java.text.SimpleDateFormat;

public class ReceiverAgent extends Agent {
  protected void setup() {
    System.out.println("Receiver agent " + getAID().getName() + " is ready.");
    addBehaviour(new ReceiveMsgBehaviour());
  }
}


class ReceiveMsgBehaviour extends CyclicBehaviour{
  private int counter = 0;

  public void action() {
    ACLMessage msg = myAgent.receive();
    if (msg == null) {
      block();
      return;

    System.out.println(new SimpleDateFormat("yyyy/MM/dd HH:mm:ss").format(new Date()) + " | " + msg.getSender().getName() + " | "+  msg.getContent());
    counter++;

    if (counter >= 100) myAgent.doDelete();
  }
}
