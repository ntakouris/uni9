use erecruit;

CREATE TABLE interviews (
    id INT NOT NULL AUTO_INCREMENT,
    recruiter_name VARCHAR(12) NOT NULL,
    cand_usrname VARCHAR(12) NOT NULL,
    target_job INT(4) NOT NULL,
    duration_hours INT(3) NOT NULL, 
    start_date DATETIME DEFAULT CURRENT_TIMESTAMP,
    personality TINYINT DEFAULT 0,
    education TINYINT DEFAULT 0,
    experience TINYINT DEFAULT 0,
    comments VARCHAR(255),

    PRIMARY KEY(id),
    CONSTRAINT INTERV_RECR FOREIGN KEY (recruiter_name) REFERENCES recruiter(username) ON DELETE CASCADE ON UPDATE CASCADE,
    CONSTRAINT INTERV_JOB FOREIGN KEY (target_job) REFERENCES job(id) ON DELETE CASCADE ON UPDATE CASCADE,
    CONSTRAINT INTERV_CAND FOREIGN KEY (cand_usrname) REFERENCES candidate(username) ON DELETE CASCADE ON UPDATE CASCADE
);

CREATE TABLE sections(
    title VARCHAR(25) NOT NULL,
    descr VARCHAR(255) NOT NULL,
    parent VARCHAR(25) DEFAULT NULL,

    PRIMARY KEY(title),
    CONSTRAINT SEC_PARENT FOREIGN KEY (parent) REFERENCES sections(title) ON DELETE CASCADE ON UPDATE CASCADE
);

INSERT INTO sections VALUES
("Computer Engineering", "Computer Science, Informatics", NULL),
("Software", "NOT Hadrdware", "Computer Engineering"),
("Hardware", "NOT Software", "Computer Engineering"),
("Web Apps", "REST APIs", "Software"),
("Computer Vision", "Multiview Geometry and stuff", "Software"),
("Logic Design", "Vergos :)", "Hardware"),
("Quantum Computers", "1 - 0.5 - 0", "Hardware")
;

CREATE TABLE sections_per_company(
    company_afm char(9) NOT NULL, 
    section VARCHAR(25) NOT NULL,

    PRIMARY KEY(company_afm, section),
    CONSTRAINT SEC_PER_COMP_COMP FOREIGN KEY (company_afm) REFERENCES etaireia(AFM) ON DELETE CASCADE ON UPDATE CASCADE,
    CONSTRAINT SEC_PER_COMP_SEC FOREIGN KEY (section) REFERENCES sections(title) ON DELETE CASCADE ON UPDATE CASCADE
);

CREATE TABLE audit_logs(
    id INT NOT NULL AUTO_INCREMENT,
    username varchar(12) NOT NULL,
    action_type varchar(25) NOT NULL,
    happened_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    table_name VARCHAR(45) NOT NULL,
    success INT(1) NOT NULL,

    PRIMARY KEY(id)
);