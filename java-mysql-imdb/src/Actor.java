
/**
 * @author gorkem
 *
 */
public class Actor {

    private int aid;
    private String name;

    public Actor() {

    }

    public Actor(int aid, String name) {
        this.setAid(aid);
        this.setName(name);
    }

    public int getAid() {
        return aid;
    }

    public void setAid(int aid) {
        this.aid = aid;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

}
