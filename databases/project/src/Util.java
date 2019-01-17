import util.LimitDocumentFilter;

import javax.swing.*;
import javax.swing.text.AbstractDocument;
import java.awt.*;

public class Util {
    public static void showUserEditWindow() {
        var frame = new JFrame("Edit user info");

        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

        frame.getContentPane().setLayout(new FlowLayout());

        var unamefield = new JTextField("Username",12);
        unamefield.setEditable(false);
        ((AbstractDocument)unamefield.getDocument()).setDocumentFilter(new LimitDocumentFilter(12));

        var pwfield = new JTextField("passw",10);
        ((AbstractDocument)pwfield.getDocument()).setDocumentFilter(new LimitDocumentFilter(10));

        var namefield = new JTextField("name", 25);
        ((AbstractDocument)pwfield.getDocument()).setDocumentFilter(new LimitDocumentFilter(25));

        var surnamefield = new JTextField("name", 35);
        ((AbstractDocument)pwfield.getDocument()).setDocumentFilter(new LimitDocumentFilter(35));

        var datefield = new JTextField("date", 10);
        ((AbstractDocument)pwfield.getDocument()).setDocumentFilter(new LimitDocumentFilter(10));

        var emailfield = new JTextField("email@somewhere.com", 30);
        ((AbstractDocument)pwfield.getDocument()).setDocumentFilter(new LimitDocumentFilter(30));

        var btn = new Button("Update user info");

        btn.addActionListener(e -> {
            //TODO: check mail and date
            Database.updateProfile(User.name, pwfield.getText(), namefield.getText(), surnamefield.getText(), datefield.getText(), emailfield.getText());
        });

        frame.getContentPane().add(unamefield);
        frame.getContentPane().add(pwfield);
        frame.getContentPane().add(namefield);
        frame.getContentPane().add(surnamefield);
        frame.getContentPane().add(datefield);
        frame.getContentPane().add(emailfield);

        frame.getContentPane().add(btn);

        frame.pack();

        var dto = Database.loadUser(User.name);

        unamefield.setText(dto.username);
        pwfield.setText(dto.password);
        namefield.setText(dto.name);
        surnamefield.setText(dto.surname);
        datefield.setText(dto.reg_date);
        emailfield.setText(dto.email);

        frame.setSize(800, 600);
        frame.setVisible(true);
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
    }
}
