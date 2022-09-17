
import java.util.List;

/**
 * @author gorkem
 *
 */
public interface IMovieManager {

    /**
     * Function for parsing input files: actors.txt, directors.txt, movies.txt, users.txt
     * Do your insertions inside this function.
     */
    public void ParseFilesAndInsertData();

    /**
     * Function for creating tables in the database.
     */
    public void CreateTables();

    /**
     * Function for inserting Actor object into the database.
     *
     * @param actor Actor object to be inserted.
     */
    public void InsertActor(Actor actor);

    /**
     * Function for inserting Director object into the database.
     *
     * @param director Director object to be inserted.
     */
    public void InsertDirector(Director director);

    /**
     * Function for inserting Movie object into the database.
     *
     * @param movie Movie object to be inserted.
     */
    public void InsertMovie(Movie movie);

    /**
     * Function for inserting User object into the database.
     *
     * @param user User object to be inserted.
     */
    public void InsertUser(User user);

    /**
     * Function for inserting record for Actor-Movie relationship.
     *
     * @param actor Actor that is casted in the movie.
     * @param movie Movie information that actor participated.
     */
    public void InsertCastedIn(Actor actor, Movie movie);

    /**
     * Function for inserting record for User-Movie relationship.
     *
     * @param user User that watched the movie.
     * @param movie Movie information that user watched.
     */
    public void InsertWatched(User user, Movie movie);

    /**
     * Function for getting movies that director directed.
     *
     * @param did Director id of the director.
     * @return List of movies that director is directed.
     */
    public List<Movie> GetMoviesOfDirector(int did);

    /**
     * Function for getting movies that actor casted in.
     *
     * @param aid Actor id of the actor.
     * @return List of movies that actor is casted in.
     */
    public List<Movie> GetMoviesOfActor(int aid);

    /**
     * Function for getting movies that user watched.
     *
     * @param uid User id of the user.
     * @return List of movies that user is watched.
     */
    public List<Movie> GetUserWatchlist(int uid);

    /**
     * Function for getting movies that have two actors casted in together.
     *
     * @param actorName1 Name of the first actor.
     * @param actorName2 Name of the second actor.
     * @return List of movies that two actors casted in together.
     */
    public List<Movie> GetMoviesWithTwoActors(String actorName1, String actorName2);

    /**
     * Function for getting movies that have higher rating than given value.
     *
     * @param rating A float value.
     * @return List of movies that have higher rating than given value.
     */
    public List<Movie> GetMoviesAboveRating(float rating);

    /**
     * Update ratings of movies with given pattern.
     * Ratings of movies that match pattern will be updated with given value.
     * This method should work in CASE INSENSITIVE manner.
	 * For example, giving either ”Rings”, ”rings”, ”rIngs” or ”RiNgS” as pattern should have the same effect.
     * 
	 * @param pattern Search criteria for updating movies
     * @param value New rating value for movies with title like given text.
     */
    public void ChangeRatingsOfMoviesLike(String pattern, float value);

    /**
     * Function for deleting director with given id. Note that when a director
     * is deleted, movies that director directed are also deleted. Since some of
     * the movies are deleted, watchlists of some of users are changed.
     *
     * @param did Director id of the director to be deleted.
     */
    public void DeleteDirectorWithId(int did);

    /**
     * Function for deleting user with given id. Note that when a user is
     * deleted, user watchlist changes, rows from user-movie relationship should
     * be deleted too.
     *
     * @param uid User id of the user to be deleted.
     */
    public void DeleteUserWithId(int uid);

    /**
     * Prints how many times each movie is watched, with movie details. View
     * counts are calculated according to movie id (or name). For each movie;
     * print movie id, movie title and view count separated by "|":
     * movie_id|movie_title|view_count
     */
    public void PrintViewStatsByMovie();

    /**
     * Prints how many times movies from each genre is watched. View counts are
     * calculated according to movie genre. For each genre; print genre and view
     * count separated by "|":
     * genre|view_count
     */
    public void PrintViewStatsByGenre();

    /**
     * Prints average rating of movies that directors directed. Average rating
     * is calculated by total_rating/directed_movie_count for each director. For
     * each director; print director name, average rating and view count
     * separated by "|": 
     * director_name|average_rating|view_count
     */
    public void PrintAverageMovieRatingOfDirectors();

    /**
     * Function to drop tables.
     */
    public void DropTables();
}
