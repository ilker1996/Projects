
/**
 * @author gorkem
 *
 */
public class Movie {

    private int mid;
    private String title;
    private String genre;
    private float rating;
    private int year;
    private Director director;

    public Movie() {

    }

    public Movie(int mid, String title, String genre, float rating, int year,
            Director director) {
        this.setMid(mid);
        this.setTitle(title);
        this.setGenre(genre);
        this.setRating(rating);
        this.setYear(year);
        this.setDirector(director);
    }

    public int getMid() {
        return mid;
    }

    public void setMid(int mid) {
        this.mid = mid;
    }

    public String getTitle() {
        return title;
    }

    public void setTitle(String title) {
        this.title = title;
    }

    public String getGenre() {
        return genre;
    }

    public void setGenre(String genre) {
        this.genre = genre;
    }

    public float getRating() {
        return rating;
    }

    public void setRating(float rating) {
        this.rating = rating;
    }

    public int getYear() {
        return year;
    }

    public void setYear(int year) {
        this.year = year;
    }

    public Director getDirector() {
        return director;
    }

    public void setDirector(Director director) {
        this.director = director;
    }

}
