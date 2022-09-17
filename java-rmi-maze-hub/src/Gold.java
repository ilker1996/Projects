public class Gold extends MazeObject {

    private static final long serialVersionUID = 1L;

    public Gold(Position position) {
        super(position, MazeObjectType.GOLD);
    }

    @Override
    public Position moveInto(Agent agent, MazeObject[][] gridMap) {
        Position agentPos = agent.getPosition();
        gridMap[getPosition().getY()][getPosition().getX()] = agent;
        gridMap[agentPos.getY()][agentPos.getX()] = null;
        agent.incrGold();

        return getPosition();
    }
}
