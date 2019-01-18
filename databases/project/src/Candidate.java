import dto.PositionDto;
import util.LimitDocumentFilter;

import javax.swing.*;
import javax.swing.text.AbstractDocument;
import java.awt.*;
import java.util.function.Function;
import java.util.stream.Stream;

public class Candidate {
    public Candidate() {
        Util.showUserEditWindow();
        showCandidateProfileEditWindow();
        showProjectsEditWindow();
        showEditDegreesWindow();
//        showApplicationWizard();
    }

    private void showEditDegreesWindow() {
        var frame = new JFrame("Applications");

        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.getContentPane().setLayout(new FlowLayout());

        var list = new JList<>(Database.loadHasDegreeFor(User.name).stream().map(x -> x.title).toArray());

        Function refresh = (Object nothing) -> {
            list.removeAll();

            list.setListData(Database.loadHasDegreeFor(User.name).stream().map(x -> x.title).toArray());
            return true;
        };

        var addDegreeButton = new Button("Add Degree");

        list.addListSelectionListener(e -> showEditDegreeWindow(refresh, (String) list.getSelectedValue()));

        addDegreeButton.addActionListener(e -> showEditDegreeWindow(refresh, null));

        frame.getContentPane().add(list);
        frame.getContentPane().add(addDegreeButton);

        frame.pack();

        frame.setSize(800,600);
        frame.setVisible(true);
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
    }

    private void showEditDegreeWindow(Function refresh, String title){
        var frame = new JFrame(title == null ? "Add new degree" : title);

        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

        frame.getContentPane().setLayout(new FlowLayout());

        // list
        JComboBox nameslist;
        if (title == null) {
            nameslist = new JComboBox(Database.availableDegreesToAdd(User.name).stream().map(x -> String.join(" ", new String[]{x.titlos, x.idryma})).toArray());
        }else{
            nameslist = new JComboBox(new String[]{title});
        }

        var etosfield = new JTextField("0");
        var gradefield = new JTextField("5");

        var btn = new JButton(title == null ? "Add degree" : "Edit degree");
        var rmbtn = new JButton("Remove degree");
        rmbtn.setVisible(title != null);

        btn.addActionListener(e -> {
            //TODO: check year, grade
            String[] item = ((String)nameslist.getSelectedItem()).split(" ");

            if(title == null){
                Database.addHasDegree(User.name, item[0], item[1], etosfield.getText(), gradefield.getText());
            }else{
                Database.editHasDegree(User.name, item[0], item[1], etosfield.getText(), gradefield.getText());
            }


            refresh.apply(null);
            frame.dispose();
        });

        rmbtn.addActionListener(e -> {
            if (title == null) {
                return;
            }

            //TODO: Check year, grade
            String[] item = title.split(" ");

            Database.removeDegree(User.name, item[0], item[1]);

            refresh.apply(null);
            frame.dispose();
        });

        frame.getContentPane().add(nameslist);
        frame.getContentPane().add(etosfield);
        frame.getContentPane().add(btn);
        frame.getContentPane().add(rmbtn);

        frame.pack();

        frame.setSize(800, 600);
        frame.setVisible(true);
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
    }

    private void showApplicationWizard() {
        var frame = new JFrame("Applications");

        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.getContentPane().setLayout(new FlowLayout());

        var viewOpenPositionsBtn = new JButton("View positions to applyForJob");
        var editSubmittedApplicationsBtn = new JButton("View and edit submitted applications");

        viewOpenPositionsBtn.addActionListener(e -> {
            showPositions(x -> Database.loadOpenPositions(User.name));
        });

        editSubmittedApplicationsBtn.addActionListener(e -> {
            showPositions(x -> Database.loadSubmittedPositions(User.name));
        });

        frame.getContentPane().add(viewOpenPositionsBtn);
        frame.getContentPane().add(editSubmittedApplicationsBtn);

        frame.pack();

        frame.setSize(800, 600);
        frame.setVisible(true);
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
    }

    private void showPositions(Function<Object, PositionDto[]> source){
        var frame = new JFrame("Apply for new position");

        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.getContentPane().setLayout(new FlowLayout());

        var list = new JList<>(Stream.of(source.apply(null)).map(x -> x.position).toArray());

        Function refresh = (Object nothing) -> {
            DefaultListModel listModel = (DefaultListModel) list.getModel();
            listModel.removeAllElements();

            for(var dto : source.apply(null)){
                listModel.addElement(dto.position);
            }
            return true;
        };

        var addProjectButton = new Button("Add project");

        list.addListSelectionListener(e -> showPositionDetail((String) list.getSelectedValue(), refresh));

        addProjectButton.addActionListener(e -> showProjectAddWindow(refresh));

        frame.getContentPane().add(list);
        frame.getContentPane().add(addProjectButton);

        frame.pack();

        frame.setSize(800, 600);
        frame.setVisible(true);
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
    }

    private void showPositionDetail(String id, Function refresh){
        var frame = new JFrame("Position details");

        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

        frame.getContentPane().setLayout(new FlowLayout());

        var positionfield = new JTextField("descriptions",40);

        var salaryfield = new JTextField("10000",200);

        var edrafield = new JTextField("edra", 45);

        var rankfield = new JTextField("", 20);

        var statusfield = new JTextField("status", 50);
        //TODO: Add status of application based on dates

        var canRevokeApplication = Database.canCandidateRemoveApplication(User.name, id);

        var btn = new JButton(canRevokeApplication ? "Remove application" : "Apply now");

        btn.addActionListener(e -> {
            if (canRevokeApplication) {
                Database.removeApplicationForJob(User.name, id);
            }else{
                Database.applyForJob(User.name, id);
            }

            refresh.apply(null);
            frame.dispose();
        });

        frame.getContentPane().add(positionfield);
        frame.getContentPane().add(salaryfield);
        frame.getContentPane().add(edrafield);
        frame.getContentPane().add(rankfield);
        frame.getContentPane().add(statusfield);
        frame.getContentPane().add(btn);

        frame.setSize(800,600);
        frame.pack();

        var dto = Database.loadOpenPosition(id);

        positionfield.setText(String.valueOf(dto.position));
        salaryfield.setText(String.valueOf(dto.salary));
        edrafield.setText(dto.edra);

        if(dto.rank != -1){
            rankfield.setText(String.format("Your rank is %d out of %d candidates", dto.rank, dto.totalcands));
        }

        frame.setVisible(true);
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
    }

    private void showProjectsEditWindow(){
        var frame = new JFrame("Edit projects");

        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.getContentPane().setLayout(new FlowLayout());

        var list = new JList<>(Database.loadProjectsOf(User.name).stream().map(x -> x.url).toArray());

        Function refresh = (Object nothing) -> {
            list.removeAll();

            list.setListData(Database.loadProjectsOf(User.name).stream().map(x -> x.url).toArray());
            return true;
        };

        var addProjectButton = new Button("Add project");

        list.addListSelectionListener(e -> showProjectEditWindow((String) list.getSelectedValue(), refresh));

        addProjectButton.addActionListener(e -> showProjectAddWindow(refresh));

        frame.getContentPane().add(list);
        frame.getContentPane().add(addProjectButton);

        frame.pack();

        frame.setSize(800, 600);
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

        var descrfield = new TextArea();
        descrfield.setText("descr");

        var urlfield = new JTextField("url", 35);
        ((AbstractDocument)urlfield.getDocument()).setDocumentFilter(new LimitDocumentFilter(60));

        var btn = new JButton(url == null ? "Add project" : "Edit");

        btn.addActionListener(e -> {
            //TODO: Check valididty of data
            if(url == null){
                Database.addProject(User.name, numfield.getText(), descrfield.getText(), urlfield.getText());
            }else{
                Database.updateProject(User.name, url, numfield.getText(), descrfield.getText(), urlfield.getText());
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

        var biofield = new JTextArea();
        biofield.setText("bio");

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

        frame.setSize(800, 600);
        frame.setVisible(true);
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
    }

}
