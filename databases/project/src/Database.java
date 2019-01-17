import dto.*;

import java.sql.*;
import java.util.ArrayList;
import java.util.List;

public class Database {

    public static Connection conn;

    public static void init() throws ClassNotFoundException, SQLException {
        Class.forName("com.mysql.jdbc.Driver");
        conn = DriverManager.getConnection("jdbc:mysql://localhost:3306/erecruit", "doot", "doot");
    }

    // return null for bad credentials
    // else return 'candidate' or 'recruiter' or 'admin'
    public static String authenticate(String username, String password) {
        if(username.equals("admin1") && password.equals("password1")){
            return "admin";
        }

        Statement stmt = null;
        boolean found = false;

        try {
            stmt = conn.createStatement();
            String sql;
            sql = "SELECT * FROM `user` WHERE username='" + username + "' AND `password`='" + password + "'";
            ResultSet rs = stmt.executeQuery(sql);

            while (rs.next()) {
                found = true;
            }
            rs.close();
            stmt.close();
        } catch (Exception se) {
            found = false;
            se.printStackTrace();
        } finally {
            try {
                if (stmt != null) {
                    stmt.close();
                }
            } catch (SQLException se2) {
            }
        }

        if(found){
            return roleOf(username);
        }

        return null;
    }

    private static boolean isCandidate(String username){
        Statement stmt = null;
        boolean found = false;

        try {
            stmt = conn.createStatement();
            String sql;
            sql = "SELECT * FROM candidate WHERE username='" + username + "'";
            ResultSet rs = stmt.executeQuery(sql);

            while (rs.next()) {
                found = true;
            }
            rs.close();
            stmt.close();
        } catch (Exception se) {
            se.printStackTrace();
        } finally {
            try {
                if (stmt != null) {
                    stmt.close();
                }
            } catch (SQLException se2) {
            }
        }

        return found;
    }

    private static boolean isRecruiter(String username){
        Statement stmt = null;
        boolean found = false;

        try {
            stmt = conn.createStatement();
            String sql;
            sql = "SELECT * FROM recruiter WHERE username='" + username + "'";
            ResultSet rs = stmt.executeQuery(sql);

            while (rs.next()) {
                found = true;
            }
            rs.close();
            stmt.close();
        } catch (Exception se) {
            se.printStackTrace();
        } finally {
            try {
                if (stmt != null) {
                    stmt.close();
                }
            } catch (SQLException se2) {
            }
        }

        return found;
    }

    private static String roleOf(String username){
        if(isRecruiter(username)){
            return "recruiter";
        }

        if(isCandidate(username)){
            return "candidate";
        }

        return "candidate"; // just to be safe
    }

    public static UserDto loadUser(String username) {
        Statement stmt = null;

        UserDto dto = new UserDto();

        try {
            stmt = conn.createStatement();
            String sql;
            sql = "SELECT * FROM `user` WHERE username='" + username + "'";
            ResultSet rs = stmt.executeQuery(sql);

            while (rs.next()) {
                dto.username = username;
                dto.password = rs.getString("password");
                dto.email = rs.getString("email");
                dto.name = rs.getString("name");
                dto.surname = rs.getString("surname");
                dto.reg_date = rs.getDate("reg_date").toString();
            }
            rs.close();
            stmt.close();
        } catch (Exception se) {
            se.printStackTrace();
        } finally {
            try {
                if (stmt != null) {
                    stmt.close();
                }
            } catch (SQLException se2) {
            }
        }
        return dto;
    }

    public static void updateProfile(String uname, String pw, String name, String surname, String date, String mail) {
        Statement stmt = null;
        try {
            stmt = conn.createStatement();
            String sql;
            sql = "UPDATE `user` SET `password`='" + pw + "'" + ", `name`='" + name  + "'" + ", surname='" + surname + "', email='" + mail + "' WHERE username='" + uname + "'";
            stmt.executeUpdate(sql);
            stmt.close();
        } catch (Exception se) {
            se.printStackTrace();
        } finally {
            try {
                if (stmt != null) {
                    stmt.close();
                }
            } catch (SQLException se2) {
            }
        }
    }

    public static List<ProjectDto> loadProjectsOf(String username) {
        Statement stmt = null;

        List<ProjectDto> list = new ArrayList<>();
        try {
            stmt = conn.createStatement();
            String sql;
            sql = "SELECT * FROM project WHERE candid='" + username + "'";
            ResultSet rs = stmt.executeQuery(sql);

            while (rs.next()) {
                ProjectDto dto = new ProjectDto();

                dto.url = rs.getString("url");
                dto.num = rs.getInt("num");
                dto.desc = rs.getString("descr");

                list.add(dto);
            }
            rs.close();
            stmt.close();
        } catch (Exception se) {
            se.printStackTrace();
        } finally {
            try {
                if (stmt != null) {
                    stmt.close();
                }
            } catch (SQLException se2) {
            }
        }

        return list;
    }

    public static ProjectDto loadProjectFor(String username, String url) {
        Statement stmt = null;
        ProjectDto dto = new ProjectDto();

        try {
            stmt = conn.createStatement();
            String sql;
            sql = "SELECT * FROM project WHERE candid='" + username + "' AND url='" + url + "'";
            ResultSet rs = stmt.executeQuery(sql);

            while (rs.next()) {
                dto.url = url;
                dto.num = rs.getInt("num");
                dto.desc = rs.getString("descr");
            }
            rs.close();
            stmt.close();
        } catch (Exception se) {
            se.printStackTrace();
        } finally {
            try {
                if (stmt != null) {
                    stmt.close();
                }
            } catch (SQLException se2) {
            }
        }
        return dto;
    }

    public static CandidateDto loadCandidate(String uname) {
        Statement stmt = null;
        CandidateDto dto = new CandidateDto();

        try {
            stmt = conn.createStatement();
            String sql;
            sql = "SELECT * FROM candidate WHERE username='" + uname + "'";
            ResultSet rs = stmt.executeQuery(sql);

            while (rs.next()) {
                dto.bio = rs.getString("bio");
                dto.certificates = rs.getString("certificates");
                dto.sistatikes = rs.getString("sistatikes");
            }
            rs.close();
            stmt.close();
        } catch (Exception se) {
            se.printStackTrace();
        } finally {
            try {
                if (stmt != null) {
                    stmt.close();
                }
            } catch (SQLException se2) {
            }
        }

        dto = injectLanguages(uname, dto);

        return dto;
    }

    private static CandidateDto injectLanguages(String uname, CandidateDto dto){
        Statement stmt = null;
        try {
            stmt = conn.createStatement();
            String sql;
            sql = "SELECT * FROM languages WHERE candid='" + uname + "'";
            ResultSet rs = stmt.executeQuery(sql);

            while (rs.next()) {
                dto.languages = rs.getString("lang").split(",");
            }
            rs.close();
            stmt.close();
        } catch (Exception se) {
            se.printStackTrace();
        } finally {
            try {
                if (stmt != null) {
                    stmt.close();
                }
            } catch (SQLException se2) {
            }
        }
        return dto;
    }

    public static void updateCandidate(String uname, String biofield, String sistatikes, String certificates, String[] langs) {
        Statement stmt = null;
        try {
            stmt = conn.createStatement();
            String sql;
            sql = "UPDATE candidate SET bio='" + biofield + "'" + ", sistatikes='" + sistatikes  + "'" + ", certificates='" + certificates + "'" + " WHERE username='" + uname + "'";
            stmt.executeUpdate(sql);
            stmt.close();
        } catch (Exception se) {
            se.printStackTrace();
        } finally {
            try {
                if (stmt != null) {
                    stmt.close();
                }
            } catch (SQLException se2) {
            }
        }

        cascadeUpdateLanguages(uname, langs);
    }

    private static void deleteAllLanguagesFor(String uname){
        Statement stmt = null;
        try {
            stmt = conn.createStatement();
            String sql;
            sql = "DELETE * FROM languages WHERE candid='" + uname + "'";
            stmt.executeUpdate(sql);
            stmt.close();
        } catch (Exception se) {
            se.printStackTrace();
        } finally {
            try {
                if (stmt != null) {
                    stmt.close();
                }
            } catch (SQLException se2) {
            }
        }
    }

    private static void cascadeUpdateLanguages(String uname, String[] langs){
        deleteAllLanguagesFor(uname);

        for(String lang: langs){
            Statement stmt = null;
            try {
                stmt = conn.createStatement();
                String sql;
                sql = "INSERT INTO languages VALUES('" + uname + "','" + lang + "')";
                stmt.executeUpdate(sql);
                stmt.close();
            } catch (Exception se) {
                se.printStackTrace();
            } finally {
                try {
                    if (stmt != null) {
                        stmt.close();
                    }
                } catch (SQLException se2) {
                }
            }
        }
    }

    public static void addProject(String uname, String num, String desc, String url) {
        Statement stmt = null;
        try {
            stmt = conn.createStatement();
            String sql;
            sql = "INSERT INTO project VALUES('" + uname + "'," + num + ",'" + desc + "','" + url + "')";
            stmt.executeUpdate(sql);
            stmt.close();
        } catch (Exception se) {
            se.printStackTrace();
        } finally {
            try {
                if (stmt != null) {
                    stmt.close();
                }
            } catch (SQLException se2) {
            }
        }
    }

    public static void updateProject(String uname, String url, String num, String desc, String newurl) {
        Statement stmt = null;
        try {
            stmt = conn.createStatement();
            String sql;
            sql = "UPDATE project SET url='" + newurl + "', num=" + num + ", descr='" + desc + "' WHERE candid='" + uname + "' AND url='" + url + "'";
            stmt.executeUpdate(sql);
            stmt.close();
        } catch (Exception se) {
            se.printStackTrace();
        } finally {
            try {
                if (stmt != null) {
                    stmt.close();
                }
            } catch (SQLException se2) {
            }
        }
    }

    public static PositionDto[] loadOpenPositions(String uname) {
        return null;
    }

    public static void applyForJob(String name, String jobPosId) {
        Statement stmt = null;
        try {
            stmt = conn.createStatement();
            String sql;
            sql = "INSERT INTO applies VALUES('" + name + "','" + jobPosId + "')";
            stmt.executeUpdate(sql);
            stmt.close();
        } catch (Exception se) {
            se.printStackTrace();
        } finally {
            try {
                if (stmt != null) {
                    stmt.close();
                }
            } catch (SQLException se2) {
            }
        }
    }

    public static PositionDto loadOpenPosition(String jobId) {
        return null;
    }

    public static boolean canCandidateRemoveApplication(String name, String jobid) {
        return false;
    }

    public static void removeApplicationForJob(String name, String jobid) {
        Statement stmt = null;
        try {
            stmt = conn.createStatement();
            String sql;
            sql = "DELETE * FROM applies WHERE job_id='" + jobid + "' AND cand_usrname='" + name + "'";
            stmt.executeUpdate(sql);
            stmt.close();
        } catch (Exception se) {
            se.printStackTrace();
        } finally {
            try {
                if (stmt != null) {
                    stmt.close();
                }
            } catch (SQLException se2) {
            }
        }
    }

    public static PositionDto[] loadSubmittedPositions(String name) {
        return null;
    }

    public static DegreeDto[] availableDegreesToAdd(String name) {
        return null;
    }

    public static List<HasDegreeDto> loadHasDegreeFor(String name) {
        Statement stmt = null;
        List<HasDegreeDto> dto = new ArrayList<>();

        try {
            stmt = conn.createStatement();
            String sql;
            sql = "SELECT * FROM has_degree WHERE cand_usrname='" + name + "'";
            ResultSet rs = stmt.executeQuery(sql);

            while (rs.next()) {
                HasDegreeDto deg = new HasDegreeDto();

                deg.title = rs.getString("degr_title");
                deg.idryma = rs.getString("degr_idryma");
                deg.etos = rs.getString("etos");
                deg.grade = rs.getFloat("grade");

                dto.add(deg);
            }
            rs.close();
            stmt.close();
        } catch (Exception se) {
            se.printStackTrace();
        } finally {
            try {
                if (stmt != null) {
                    stmt.close();
                }
            } catch (SQLException se2) {
            }
        }
        return dto;
    }

    public static void addHasDegree(String name, String title, String idryma, String year, String grade) {
        Statement stmt = null;
        try {
            stmt = conn.createStatement();
            String sql;
            sql = "UPDATE has_degree SET etos=" + year + ", grade=" + grade + " has_degree WHERE name='" + name + "' AND degr_title='" + title + "' AND degr_idryma='" + idryma + "'";
            stmt.executeUpdate(sql);
            stmt.close();
        } catch (Exception se) {
            se.printStackTrace();
        } finally {
            try {
                if (stmt != null) {
                    stmt.close();
                }
            } catch (SQLException se2) {
            }
        }
    }

    public static void editHasDegree(String name, String title, String idryma, String year, String grade) {
        Statement stmt = null;
        try {
            stmt = conn.createStatement();
            String sql;
            sql = "INSERT INTO has_degree VALUES('" + title + "','" + idryma + "','" + name + "'," + year + "," + grade + ")";
            stmt.executeUpdate(sql);
            stmt.close();
        } catch (Exception se) {
            se.printStackTrace();
        } finally {
            try {
                if (stmt != null) {
                    stmt.close();
                }
            } catch (SQLException se2) {
            }
        }
    }
}
