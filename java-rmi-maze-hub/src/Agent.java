public class Agent extends MazeObject {

    private static final long serialVersionUID = 1L;

    private final int id;
    private int collectedGold;

    public Agent(Position position, int id) {
        super(position, MazeObjectType.AGENT);
        this.collectedGold = 0;
        this.id = id;
    }

    public int getId() {
        return id;
    }

    public void incrGold() {
        collectedGold++;
    }

    // Stays in the same position
    @Override
    public Position moveInto(Agent agent, MazeObject[][] gridMap) {
        return agent.getPosition();
    }

    public void print() {
        System.out.println(
                "Agent" + id + " at " + super.getPosition() + "." + " Gold collected : " + collectedGold + ".");
    }
}
