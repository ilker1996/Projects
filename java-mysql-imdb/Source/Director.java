
/**
 * @author gorkem
 *
 */
public class Director {

    private int did;
    private String name;

    public Director() {

    }

    public Director(int did, String name) {
        this.setDid(did);
        this.setName(name);
    }

    public int getDid() {
        return did;
    }

    public void setDid(int did) {
        this.did = did;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

}
