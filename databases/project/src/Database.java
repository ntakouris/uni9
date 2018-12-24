import dto.*;

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

    public static PositionDto[] loadOpenPositions(String uname) {
        return null;
    }

    public static void applyForJob(String name, String jobPosId) {

    }

    public static PositionDto loadOpenPosition(String jobId) {
        return null;
    }

    public static boolean canCandidateRemoveApplication(String name, String jobid) {
        return false;
    }

    public static void removeApplicationForJob(String name, String jobid) {

    }

    public static PositionDto[] loadSubmittedPositions(String name) {
        return null;
    }

    public static DegreeDto[] availableDegreesToAdd(String name){
        return null;
    }

    public static HasDegreeDto[] loadHasDegreeFor(String name) {
        return null;
    }

    public static void addHasDegree(String name, String title, String idryma, String year, String grade) {

    }

    public static void editHasDegree(String name, String title, String idryma, String year, String grade) {

    }
}
