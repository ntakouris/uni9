import util.LimitDocumentFilter;

import javax.swing.*;
import javax.swing.text.AbstractDocument;
import java.awt.*;

public class Login {

    public Login(){
        var frame = new JFrame("FrameDemo");

        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

        frame.getContentPane().setLayout(new FlowLayout());
        var unamefield = new JTextField("Username",12);
        ((AbstractDocument)unamefield.getDocument()).setDocumentFilter(new LimitDocumentFilter(12));

        var pwfield = new JTextField("passw",10);
        ((AbstractDocument)pwfield.getDocument()).setDocumentFilter(new LimitDocumentFilter(10));

        var errfield = new JTextField("", 50);
        errfield.setVisible(false);

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

            frame.dispose();
        });

        frame.getContentPane().add(unamefield);
        frame.getContentPane().add(pwfield);
        frame.getContentPane().add(btn);
        frame.getContentPane().add(errfield);
        frame.pack();

        frame.setVisible(true);
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
    }
}
