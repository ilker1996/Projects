
/**
 * @author gorkem
 *
 */
public class User {

    private int uid;
    private String name;
    private int age;

    public User() {

    }

    public User(int uid, String name, int age) {
        this.setUid(uid);
        this.setName(name);
        this.setAge(age);
    }

    public int getUid() {
        return uid;
    }

    public void setUid(int uid) {
        this.uid = uid;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public int getAge() {
        return age;
    }

    public void setAge(int age) {
        this.age = age;
    }
}
