import dto.ProjectDto;
import util.LimitDocumentFilter;

import javax.swing.*;
import javax.swing.event.ListSelectionEvent;
import javax.swing.event.ListSelectionListener;
import javax.swing.text.AbstractDocument;
import javax.xml.crypto.Data;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.function.Function;
import java.util.stream.Stream;

public class Candidate {
    public Candidate() {
        showUserEditWindow();
        showCandidateProfileEditWindow();
        showProjectsEditWindow();
    }

    private void showProjectsEditWindow(){
        var frame = new JFrame("Edit projects");

        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.getContentPane().setLayout(new FlowLayout());

        var list = new JList<>(Stream.of(Database.loadProjectsOf(User.name)).map(x -> x.url).toArray());

        Function refresh = (Object nothing) -> {
            DefaultListModel listModel = (DefaultListModel) list.getModel();
            listModel.removeAllElements();

            for(var dto : Database.loadProjectsOf(User.name)){
                listModel.addElement(dto.url);
            }
            return true;
        };

        var addProjectButton = new Button("Add project");

        list.addListSelectionListener(e -> showProjectEditWindow((String) list.getSelectedValue(), refresh));

        addProjectButton.addActionListener(e -> showProjectAddWindow(refresh));

        frame.getContentPane().add(list);
        frame.getContentPane().add(addProjectButton);

        frame.pack();

        frame.setVisible(true);
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
    }

    private void showProjectAddWindow(Function refresh){
        showProjectEditWindow(null, refresh);
    }

    private void showProjectEditWindow(String url, Function refresh){
        var frame = new JFrame(url == null ? "Add new project" : "Edit project");

        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

        frame.getContentPane().setLayout(new FlowLayout());

        var numfield = new JTextField("0",1);
        ((AbstractDocument)numfield.getDocument()).setDocumentFilter(new LimitDocumentFilter(1));


        var descrfield = new JTextField("descr",200);

        var urlfield = new JTextField("url", 35);
        ((AbstractDocument)urlfield.getDocument()).setDocumentFilter(new LimitDocumentFilter(60));

        var btn = new JButton(url == null ? "Add project" : "Edit");

        btn.addActionListener(e -> {
            //TODO: Check valididty of data
            if(url == null){
                Database.addProject(User.name, numfield.getText(), descrfield.getText(), urlfield.getText());
            }else{
                Database.updateProject(User.name, url, numfield.getText(), descrfield.getText(), urlfield.getText())
            }

            refresh.apply(null);
            frame.dispose();
        });

        frame.getContentPane().add(numfield);
        frame.getContentPane().add(descrfield);
        frame.getContentPane().add(urlfield);
        frame.getContentPane().add(btn);

        frame.pack();

        if(url != null){
            var dto = Database.loadProjectFor(User.name, url);

            numfield.setText(String.valueOf(dto.num));
            descrfield.setText(dto.desc);
            urlfield.setText(dto.url);
        }

        frame.setVisible(true);
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
    }

    private void showCandidateProfileEditWindow(){
        var frame = new JFrame("Edit candidate info");

        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

        frame.getContentPane().setLayout(new FlowLayout());

        var biofield = new JTextField("Bio",200);

        var sistatikesfield = new JTextField("passw",35);
        ((AbstractDocument)sistatikesfield.getDocument()).setDocumentFilter(new LimitDocumentFilter(35));

        var certificatesfield = new JTextField("name", 35);
        ((AbstractDocument)sistatikesfield.getDocument()).setDocumentFilter(new LimitDocumentFilter(35));

        var languagesfield = new JTextField("name", 35);

        var btn = new Button("Update candidate info");

        btn.addActionListener(e -> {
            Database.updateCandidate(User.name, biofield.getText(), sistatikesfield.getText(), certificatesfield.getText(), languagesfield.getText().split(" "));
        });

        frame.getContentPane().add(biofield);
        frame.getContentPane().add(sistatikesfield);
        frame.getContentPane().add(certificatesfield);
        frame.getContentPane().add(languagesfield);

        frame.getContentPane().add(btn);

        frame.pack();

        var dto = Database.loadCandidate(User.name);

        biofield.setText(dto.bio);
        sistatikesfield.setText(dto.sistatikes);
        certificatesfield.setText(dto.certificates);
        languagesfield.setText(String.join(" ", dto.languages));


        frame.setVisible(true);
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
    }

    private void showUserEditWindow() {
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

        frame.setVisible(true);
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
    }
}
