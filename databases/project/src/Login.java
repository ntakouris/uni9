import util.LimitDocumentFilter;

import javax.swing.*;
import javax.swing.text.AbstractDocument;
import java.awt.*;

public class Login {

    public Login(){
        var frame = new JFrame("Login");

        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

        frame.getContentPane().setLayout(new FlowLayout());
        var unamefield = new JTextField("Username",12);
        ((AbstractDocument)unamefield.getDocument()).setDocumentFilter(new LimitDocumentFilter(12));
        unamefield.setText("zazahir23");

        var pwfield = new JTextField("passw",10);
        ((AbstractDocument)pwfield.getDocument()).setDocumentFilter(new LimitDocumentFilter(10));
        pwfield.setText("zoolhger");

        var errfield = new JTextField("no errors", 50);
        errfield.setVisible(true);

        var btn = new Button("Login");

        btn.addActionListener(e -> {
            var uname = unamefield.getText();
            var pw = pwfield.getText();

            var result = Database.authenticate(uname, pw);

            if (result == null) {
                errfield.setText("Invalid credentials");
                errfield.setVisible(true);
                return;
            }

            User.name = uname;
            User.role = result;

            switch (result){
                case "admin":
                    new Admin();
                    break;
                case "recruiter":
                    new Recruiter();
                    break;
                case "candidate":
                    new Candidate();
                    break;
            }

            frame.setVisible(false);
            frame.dispose();
        });

        frame.getContentPane().add(unamefield);
        frame.getContentPane().add(pwfield);
        frame.getContentPane().add(btn);
        frame.getContentPane().add(errfield);
        frame.pack();

        frame.setVisible(true);
        frame.setSize(800, 600);
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
    }
}
