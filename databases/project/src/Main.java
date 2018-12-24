import java.sql.SQLException;

public class Main {

    public static void main(String[] args) {
        try {
            Database.init();
        } catch (ClassNotFoundException | SQLException e) {
            e.printStackTrace();
        }

        new Login();

        try {
            Database.conn.close();
        } catch (SQLException e) {
            e.printStackTrace();
        }
    }
}
