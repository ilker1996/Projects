
import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.ArrayList;
import java.util.List;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author gorkem
 */
public class MovieManager implements IMovieManager {

    /* These values are read from databaseConfiguration.txt file. */
    private String _USERNAME;
    private String _PASSWORD;
    private String _HOST;
    private String _DATABASE_SCHEMA;
    private int _PORT_NUMBER;

    private static String _DB_CONFIG_FILE_NAME = "databaseConfiguration.txt";
    private static String _ACTOR_FILE_NAME = "actors.txt";
    private static String _MOVIE_FILE_NAME = "movies.txt";
    private static String _DIRECTOR_FILE = "directors.txt";
    private static String _USER_FILE = "users.txt";

    private Connection _connection;

    // DO NOT MODIFY THIS FUNCTION
    public void ReadDatabaseConfiguration() throws Exception {
        BufferedReader br = null;

        try {
            String currentLine;
            br = new BufferedReader(new FileReader(this._DB_CONFIG_FILE_NAME));

            while ((currentLine = br.readLine()) != null) {
                if (currentLine.startsWith("host")) {
                    this._HOST = currentLine.replace("host=", "").trim();
                }
                if (currentLine.startsWith("schema")) {
                    this._DATABASE_SCHEMA = currentLine.replace("schema=", "").trim();
                }
                if (currentLine.startsWith("username")) {
                    this._USERNAME = currentLine.replace("username=", "").trim();
                }
                if (currentLine.startsWith("password")) {
                    this._PASSWORD = currentLine.replace("password=", "").trim();
                }
                if (currentLine.startsWith("port")) {
                    this._PORT_NUMBER = Integer.parseInt(currentLine.replace("port=", "").trim());
                }
            }
        } catch (Exception ex) {
            System.out.println("Error while reading databaseConfiguration.txt file: " + ex.getMessage());
            throw ex;
        } finally {
            try {
                if (br != null) {
                    br.close();
                }
            } catch (IOException ex) {
                System.out.println("Error while reading databaseConfiguration.txt file: " + ex.getMessage());
                throw ex;
            }
        }

        System.out.println("Database configuration is read.");
    }

    // DO NOT MODIFY THIS FUNCTION
    public void InitializeConnection() {

        String url = "jdbc:mysql://" + this._HOST + ":" + this._PORT_NUMBER + "/"
                + this._DATABASE_SCHEMA + "?useUnicode=true&characterEncoding=UTF-8";

        try {
            Class.forName("com.mysql.jdbc.Driver");
            this._connection = DriverManager.getConnection(url, this._USERNAME,
                    this._PASSWORD);
        } catch (SQLException e) {
            e.printStackTrace();
        } catch (ClassNotFoundException e) {

            e.printStackTrace();
        }

        System.out.println("Database connection initialized.");
    }

    @Override
    public void ParseFilesAndInsertData() {
        BufferedReader reader = null;
        try {
            reader = new BufferedReader(new FileReader(this._ACTOR_FILE_NAME));/* ACTOR FILE*/
            String currentLine;
            while((currentLine = reader.readLine()) != null){
                String[] pair = currentLine.split(";");
                this.InsertActor(new Actor(Integer.parseInt(pair[0]),pair[1]));
                
            }
            reader.close();
            reader = null;
            reader = new BufferedReader(new FileReader(this._DIRECTOR_FILE));/*DIRECTOR FILE*/
            while((currentLine = reader.readLine()) != null ){
                String [] pair = currentLine.split(";");
                this.InsertDirector(new Director(Integer.parseInt(pair[0]),pair[1]));
               
            }
            reader.close();
            reader = null;
            
            reader = new BufferedReader(new FileReader(this._MOVIE_FILE_NAME));/*MOVIE FILE*/
            while((currentLine = reader.readLine()) != null){
                String [] pair = currentLine.split(";");
                int mid = Integer.parseInt(pair[0]);
                String title = pair[1];
                int year = Integer.parseInt(pair[2]);
                String genre = pair[3];
                String directorName = pair[5];
                float rating = Float.parseFloat(pair[6]);
                String [] actors = pair[4].split(",");
                String findDirectorId = "SELECT D.did FROM director D WHERE D.name = '" + directorName + "'";/*FIND DIRECTOR ID*/
          
                try {
                    int did = 0;
                    int [] aid = new int[4];
                    Statement stmt;
                    stmt = this._connection.createStatement();
                    ResultSet rs = stmt.executeQuery(findDirectorId);
                    if(rs.next()){
                        did = rs.getInt("did");
                    }
                    this.InsertMovie(new Movie(mid,title,genre,rating,year,new Director(did,directorName)));
                    rs = stmt.executeQuery("SELECT A.aid,A.name FROM actor A");
                    String name;
                    while(rs.next()){
                        name = rs.getString("name").trim();
                        int temp = rs.getInt("aid");
                        if(name.equals(actors[0].trim())){
                            aid[0] = temp;
                        }
                        else if(name.equals((actors[1]).trim())){
                            aid[1] = temp;
                        }
                        else if (name.equals((actors[2]).trim())){
                            aid[2] = temp;
                        }
                        else if (name.equals(actors[3].trim())){
                            aid[3] = temp;
                        }
                    }
                    for(int i = 0; i < 4 ; i++){
                        
                        stmt.execute("INSERT INTO casted_in(aid,mid) VALUES ('" + Integer.toString(aid[i])+ "','" + Integer.toString(mid) + "')");
                    }
                } catch (SQLException ex) {
                    Logger.getLogger(MovieManager.class.getName()).log(Level.SEVERE, null, ex);
                }
            }
            reader.close();
            reader = null;
           
            reader = new BufferedReader(new FileReader(this._USER_FILE)); /*USER FILE*/
            while((currentLine = reader.readLine()) != null){
                String [] pair = currentLine.split(";");
                String [] movies = pair[3].split(",");
                int uid = Integer.parseInt(pair[0]);
                String name = pair[1];
                int age = Integer.parseInt(pair[2]);
                InsertUser(new User(uid,name,age));
                
                try {
                    Statement stmt;
                    stmt = this._connection.createStatement();
                    for(int i= 0;i < 10;i++){
                        stmt.execute("INSERT INTO watched(uid,mid) VALUES ('"+ Integer.toString(uid) + "','" + movies[i] + "')" ); 
                    }
                }catch (SQLException ex) {
                    Logger.getLogger(MovieManager.class.getName()).log(Level.SEVERE, null, ex);    
                }
            }
        } catch (IOException ex) {
            Logger.getLogger(MovieManager.class.getName()).log(Level.SEVERE, null, ex);
        }
    
    }

    @Override
    public void CreateTables() {
        try {
            String actorTable ="CREATE TABLE IF NOT EXISTS actor("
                    +"aid INT,"
                    +"name VARCHAR(50),"
                    +"PRIMARY KEY (aid)"
                    +")";
            String directorTable ="CREATE TABLE IF NOT EXISTS director("
                    +"did INT,"
                    +"name VARCHAR(50),"
                    +"PRIMARY KEY (did)"
                    +")";
            String movieTable ="CREATE TABLE IF NOT EXISTS movie("
                    +"mid INT,"
                    +"title VARCHAR(50),"
                    +"genre VARCHAR(50),"
                    +"year INT,"
                    +"did INT,"
                    +"rating DOUBLE,"
                    +"PRIMARY KEY(mid),"
                    +"FOREIGN KEY (did) REFERENCES director(did)"
                    +"ON DELETE CASCADE"
                    +")";
            String userTable = "CREATE TABLE IF NOT EXISTS user("
                    +"uid INT,"
                    +"name VARCHAR(50),"
                    +"age INT,"
                    +"PRIMARY KEY(uid)"
                    +")";
             String castedInTable="CREATE TABLE IF NOT EXISTS casted_in("
                    +"aid INT,"
                    +"mid INT,"
                    +"PRIMARY KEY (aid,mid),"
                    +"FOREIGN KEY (aid) REFERENCES actor(aid)"
                    +"ON DELETE CASCADE,"
                    +"FOREIGN KEY (mid) REFERENCES movie(mid)"
                    +"ON DELETE CASCADE"
                    +")";
                    
            String watchedTable ="CREATE TABLE IF NOT EXISTS watched("
                    +"uid INT,"
                    +"mid INT,"
                    +"PRIMARY KEY (uid,mid),"
                    +"FOREIGN KEY (uid) REFERENCES user(uid)"
                    + "ON DELETE CASCADE,"
                    +"FOREIGN KEY (mid) REFERENCES movie(mid)"
                    +"ON DELETE CASCADE"
                    +")";
            Statement stmt ;
            
            stmt = this._connection.createStatement();
            stmt.execute(actorTable);
            stmt.execute(directorTable);
            stmt.execute(movieTable);
            stmt.execute(userTable);
            stmt.execute(castedInTable);
            stmt.execute(watchedTable);
                  
        } catch (SQLException ex) {
            Logger.getLogger(MovieManager.class.getName()).log(Level.SEVERE, null, ex);
        }
        
    }   
   

    @Override
    public void InsertActor(Actor actor) {
        try {
            String aid = "'" + Integer.toString(actor.getAid()) + "'";
            String name = "'" + actor.getName() + "'";
            String temp = "INSERT INTO actor(aid,name) VALUES (" + aid + "," + name + ")";
            Statement stmt;
            stmt = this._connection.createStatement();
            stmt.executeUpdate(temp);
        } catch (SQLException ex) {
            Logger.getLogger(MovieManager.class.getName()).log(Level.SEVERE, null, ex);
        }
    }

    @Override
    public void InsertDirector(Director director) {
        try {
            String did = "'" + Integer.toString(director.getDid()) + "'";
            String name = "'" + director.getName() + "'";
            String temp = "INSERT INTO director(did,name) VALUES (" + did + "," + name + ")";
            Statement stmt;
            stmt = this._connection.createStatement();
            stmt.executeUpdate(temp);
        } catch (SQLException ex) {
            Logger.getLogger(MovieManager.class.getName()).log(Level.SEVERE, null, ex);
        }
    }

    @Override
    public void InsertMovie(Movie movie) {
        try {
            String mid    = "'" + Integer.toString(movie.getMid()) + "'";
            String title  = "\"" + movie.getTitle() + "\"";
            String genre  = "\"" + movie.getGenre() + "\"";
            String year   = "'" + Integer.toString(movie.getYear()) + "'";
            String did    = "'" + Integer.toString(movie.getDirector().getDid())+ "'";
            String rating = "'" + Float.toString(movie.getRating()) + "'";
            String temp = "INSERT INTO movie(mid,title,genre,year,did,rating) VALUES (" + mid + "," + title + "," + genre
                          + "," + year + "," + did + "," + rating +  ")";
            Statement stmt;
            stmt = this._connection.createStatement();
            stmt.executeUpdate(temp);
        } catch (SQLException ex) {
            Logger.getLogger(MovieManager.class.getName()).log(Level.SEVERE, null, ex);
        }
    }

    @Override
    public void InsertUser(User user) {
        try {
            String uid  = "'" + Integer.toString(user.getUid()) + "'";
            String name = "\"" + user.getName() + "\"";
            String age  = "'" + Integer.toString(user.getAge())+"'";
            String temp = "INSERT INTO user(uid,name,age) VALUES (" + uid + "," + name + "," + age + ")";
            Statement stmt;
            stmt = this._connection.createStatement();
            stmt.executeUpdate(temp);
        } catch (SQLException ex) {
            Logger.getLogger(MovieManager.class.getName()).log(Level.SEVERE, null, ex);
        }
    }

    @Override
    public void InsertCastedIn(Actor actor, Movie movie) {
        try {
            String aid = "'" + Integer.toString(actor.getAid()) + "'";
            String mid = "'" + Integer.toString(movie.getMid()) + "'";
            String temp = "INSERT INTO casted_in(aid,mid) VALUES (" + aid + "," + mid + ")";
            Statement stmt;
            stmt = this._connection.createStatement();
            stmt.executeUpdate(temp);
        } catch (SQLException ex) {
            Logger.getLogger(MovieManager.class.getName()).log(Level.SEVERE, null, ex);
        }
    }

    @Override
    public void InsertWatched(User user, Movie movie) {
        try {
            String uid = "'" + Integer.toString(user.getUid()) + "'";
            String mid = "'" + Integer.toString(movie.getMid()) + "'";
            String temp = "INSERT INTO watched(uid,mid) VALUES (" + uid + "," + mid + ")";
            Statement stmt;
            stmt = this._connection.createStatement();
            stmt.executeUpdate(temp);
        } catch (SQLException ex) {
            Logger.getLogger(MovieManager.class.getName()).log(Level.SEVERE, null, ex);
        }
    }

    @Override
    public List<Movie> GetMoviesOfDirector(int did) {
        try {
            List<Movie> movies = new ArrayList<Movie>();
            String d = "'" + Integer.toString(did) + "'";
            String temp = "SELECT * FROM movie M,director D  WHERE D.did = M.did AND D.did =" + d + " ORDER BY D.did";
            Statement stmt;
            stmt = this._connection.createStatement();
            ResultSet rs = stmt.executeQuery(temp);
            while(rs.next()){
                movies.add(new Movie(rs.getInt("mid"),rs.getString("title")
                        ,rs.getString("genre"),rs.getFloat("rating")
                        ,rs.getInt("year"),new Director(rs.getInt("did"),rs.getString("name"))));
            }
            return movies;
        } catch (SQLException ex) {
            Logger.getLogger(MovieManager.class.getName()).log(Level.SEVERE, null, ex);
        }
        return null;
    }

    @Override
    public List<Movie> GetMoviesOfActor(int aid) {
        try {
            List<Movie> movies = new ArrayList<Movie>();
            String a = "'" + Integer.toString(aid) + "'";
            String temp = "SELECT  M.mid,M.title,M.genre,M.rating,M.year,D.did,D.name"
                    + " FROM casted_in C,movie M,actor A,director D WHERE"
                    + " C.aid = A.aid AND D.did = M.did AND M.mid = C.mid AND C.aid = " + a
                    + " ORDER BY A.aid";
            Statement stmt;
            stmt = this._connection.createStatement();
            ResultSet rs = stmt.executeQuery(temp);
            while(rs.next()){
                movies.add(new Movie(rs.getInt("mid"),rs.getString("title")
                        ,rs.getString("genre"),rs.getFloat("rating")
                        ,rs.getInt("year"),new Director(rs.getInt("did"),rs.getString("name"))));
            }
            return movies;
        } catch (SQLException ex) {
            Logger.getLogger(MovieManager.class.getName()).log(Level.SEVERE, null, ex);
        }
        return null;
    }
    

    @Override
    public List<Movie> GetUserWatchlist(int uid) {
        try {
            List<Movie> movies = new ArrayList<Movie>();
            String a = "'" + Integer.toString(uid) + "'";
            String temp = "SELECT  M.mid,M.title,M.genre,M.rating,M.year,D.did,D.name"
                    + " FROM watched W,movie M,director D WHERE"
                    + " W.mid = M.mid AND D.did = M.did AND W.uid = " + a
                    + " ORDER BY W.uid";
            Statement stmt;
            stmt = this._connection.createStatement();
            ResultSet rs = stmt.executeQuery(temp);
            while(rs.next()){
                movies.add(new Movie(rs.getInt("mid"),rs.getString("title")
                        ,rs.getString("genre"),rs.getFloat("rating")
                        ,rs.getInt("year"),new Director(rs.getInt("did"),rs.getString("name"))));
            }
            return movies;
        } catch (SQLException ex) {
            Logger.getLogger(MovieManager.class.getName()).log(Level.SEVERE, null, ex);
        }
        return null;
    }

    @Override
    public List<Movie> GetMoviesWithTwoActors(String actorName1, String actorName2) {
        try {
            List<Movie> movies = new ArrayList<Movie>();
            String a1 = "\"" + actorName1 + "\"";
            String a2 = "\"" + actorName2 + "\"";
            String temp = "SELECT  M.mid,M.title,M.genre,M.rating,M.year,D.did,D.name"
                    + " FROM actor A,movie M,director D,casted_in C WHERE"
                    + " A.aid = C.aid AND D.did = M.did AND A.name = " + a1
                    + " AND M.mid = C.mid AND M.mid IN("
                    + " SELECT  M2.mid"
                    + " FROM actor A2,movie M2,casted_in C2 WHERE"
                    + " A2.aid = C2.aid AND C2.mid = M2.mid AND "
                    + " A2.name = " + a2 + ")";
            Statement stmt;
            stmt = this._connection.createStatement();
            ResultSet rs = stmt.executeQuery(temp);
            while(rs.next()){
                movies.add(new Movie(rs.getInt("mid"),rs.getString("title")
                        ,rs.getString("genre"),rs.getFloat("rating")
                        ,rs.getInt("year"),new Director(rs.getInt("did"),rs.getString("name"))));
            }
            return movies;
        } catch (SQLException ex) {
            Logger.getLogger(MovieManager.class.getName()).log(Level.SEVERE, null, ex);
        }
        return null;
    }

    @Override
    public List<Movie> GetMoviesAboveRating(float rating) {
        try {
            List<Movie> movies = new ArrayList<Movie>();
            String r = "'" + Float.toString(rating) + "'";
            String temp = "SELECT M.mid,M.title,M.genre,M.rating,M.year,D.did,D.name"
                    + " FROM movie M,director D"
                    + " WHERE D.did = M.did AND M.rating > " + r;
            System.out.println(temp);
            Statement stmt;
            stmt = this._connection.createStatement();
            ResultSet rs = stmt.executeQuery(temp);
            while(rs.next()){
                movies.add(new Movie(rs.getInt("mid"),rs.getString("title")
                        ,rs.getString("genre"),rs.getFloat("rating")
                        ,rs.getInt("year"),new Director(rs.getInt("did"),rs.getString("name"))));
            }
            return movies;
        } catch (SQLException ex) {
            Logger.getLogger(MovieManager.class.getName()).log(Level.SEVERE, null, ex);
        }
        return null;
    }

    @Override
    public void ChangeRatingsOfMoviesLike(String pattern, float value) {
        try {
            String p = "LOWER(\"%" + pattern + "%\")";
            String v = "'" + Float.toString(value) + "'";
            String temp = "UPDATE movie M"
                    + " SET M.rating = " + v
                    + " WHERE LOWER(M.title) LIKE " + p;
      
            Statement stmt;
            stmt = this._connection.createStatement();
            stmt.executeUpdate(temp);
        } catch (SQLException ex) {
            Logger.getLogger(MovieManager.class.getName()).log(Level.SEVERE, null, ex);
        }
    }

    @Override
    public void DeleteDirectorWithId(int did) {
        try {
            String d = "'" + Integer.toString(did) + "'";
            String temp = "DELETE FROM director"
                    + " WHERE did = " + d;
            System.out.println(temp);
            Statement stmt;
            stmt = this._connection.createStatement();
            stmt.executeUpdate(temp);
        } catch (SQLException ex) {
            Logger.getLogger(MovieManager.class.getName()).log(Level.SEVERE, null, ex);
        }

    }

    @Override
    public void DeleteUserWithId(int uid) {
        try {
            String u = "'" + Integer.toString(uid) + "'";
            String temp = "DELETE FROM user"
                    + " WHERE uid = " + u;
            Statement stmt;
            stmt = this._connection.createStatement();
            stmt.executeUpdate(temp);
        } catch (SQLException ex) {
            Logger.getLogger(MovieManager.class.getName()).log(Level.SEVERE, null, ex);
        }
    }

    @Override
    public void PrintViewStatsByMovie() {
        try {
            String temp = "SELECT M.mid,M.title,COUNT(*) AS count"
                    + " FROM movie M,watched W"
                    + " WHERE M.mid = W.mid"
                    + " GROUP BY M.mid"
                    + " ORDER BY count DESC,"
                    + " M.title ASC";
            Statement stmt;
            stmt = this._connection.createStatement();
            ResultSet rs = stmt.executeQuery(temp);
            while(rs.next()){
                System.out.println(rs.getInt("mid")+ "|"+ rs.getString("title") + "|" + rs.getInt("count"));
            }
        } catch (SQLException ex) {
            Logger.getLogger(MovieManager.class.getName()).log(Level.SEVERE, null, ex);
        }
    }

    @Override
    public void PrintViewStatsByGenre() {
         try {
            String temp = "SELECT M.genre,COUNT(*) AS count"
                    + " FROM movie M,watched W"
                    + " WHERE M.mid = W.mid"
                    + " GROUP BY M.genre"
                    + " ORDER BY count DESC,"
                    + " M.genre ASC";
            Statement stmt;
            stmt = this._connection.createStatement();
            ResultSet rs = stmt.executeQuery(temp);
            while(rs.next()){
                System.out.println(rs.getString("genre") + "|" + rs.getInt("count"));
            }
        } catch (SQLException ex) {
            Logger.getLogger(MovieManager.class.getName()).log(Level.SEVERE, null, ex);
        }
    }

    @Override
    public void PrintAverageMovieRatingOfDirectors() { 
        try {
            String temp = "SELECT D.name,AVG(M.rating) AS avg,COUNT(*) AS count"
                    + " FROM director D,movie M,watched W"
                    + " WHERE D.did = M.did AND W.mid = M.mid"
                    + " GROUP BY D.did"
                    + " ORDER BY avg DESC,"
                    + " D.name ASC";
            Statement stmt;
            stmt = this._connection.createStatement();
            ResultSet rs = stmt.executeQuery(temp);
            while(rs.next()){
                System.out.println(rs.getString("name") + "|" + rs.getFloat("avg") + "|" + rs.getInt("count"));
            }
        } catch (SQLException ex) {
            Logger.getLogger(MovieManager.class.getName()).log(Level.SEVERE, null, ex);
        }
    }

    @Override
    public void DropTables() {
        try {
            String dropActorTable ="DROP TABLE IF EXISTS actor"  ;
            String dropDirectorTable ="DROP TABLE IF EXISTS director";
            String dropMovieTable ="DROP TABLE IF EXISTS movie";
            String dropUserTable ="DROP TABLE IF EXISTS user";
            String dropCastedInTable = "DROP TABLE IF EXISTS casted_in";
            String dropWatchedTable = "DROP TABLE IF EXISTS watched";
            Statement stmt;
            
            stmt = this._connection.createStatement();
            
            stmt.executeUpdate(dropCastedInTable);
            stmt.executeUpdate(dropWatchedTable);
            stmt.executeUpdate(dropMovieTable);
            stmt.executeUpdate(dropActorTable);
            stmt.executeUpdate(dropDirectorTable);
            stmt.executeUpdate(dropUserTable);
           
            
            
        } catch (SQLException ex) {
            Logger.getLogger(MovieManager.class.getName()).log(Level.SEVERE, null, ex);
        }
       
    }

    /* 
     - This main function is for helping you
     - You don't have to send your main function when you submit your code.
        
     - Use this template to continue coding. 
     - Just implement a function above, and call it with example parameters
     in main function below.
     */
    public static void main(String[] args) throws Exception {

        /* ***** DO NOT CHANGE THIS PART FOR YOUR OWN FAVOUR - BEGIN ***** */
		
        // Create new movie manager object
        MovieManager manager = new MovieManager();

        // Read databaseConfiguration.txt file for database settings
        manager.ReadDatabaseConfiguration();

        // Initialize connection with database and
        // Create connection object
        manager.InitializeConnection();

        /* ***** DO NOT CHANGE THIS PART FOR YOUR OWN FAVOUR - END ***** */

        // Rest of the functions can be called here for testing
     
        
        
        
        
     
        
        
    }
}


