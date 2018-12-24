import dto.CandidateDto;
import dto.ProjectDto;
import dto.UserDto;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.SQLException;

public class Database {

    public static Connection conn;

    public static void init() throws ClassNotFoundException, SQLException {
        Class.forName("com.mysql.jdbc.Driver");
        conn = DriverManager.getConnection("jdbc:mysql://localhost:3306/erecruit","yeet", "yeet");
    }

    // return null for bad credentials
    // else return 'candidate' or 'recruiter' or 'Admin'
    public static String authenticate(String username, String password){
        return null;
    }

    public static UserDto loadUser(String username){
        return null;
    }

    public static void updateProfile(String uname, String pw, String name, String surname, String date, String mail) {

    }

    public static ProjectDto[] loadProjectsOf(String username){
        return null;
    }

    public static ProjectDto loadProjectFor(String username, String url){
        return null;
    }

    public static CandidateDto loadCandidate(String uname) {
        return null;
    }

    public static void updateCandidate(String uname, String biofield, String sistatikes, String certificates, String[] langs) {

    }

    public static void addProject(String uname, String num, String desc, String url) {

    }

    public static void updateProject(String uname, String url, String num, String desc, String newurl) {
    }
}
