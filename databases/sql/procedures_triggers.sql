DELIMITER //
CREATE PROCEDURE CANDIDATES_FOR
(IN j_id int(4))
BEGIN
    DECLARE uname VARCHAR(12);

    DECLARE iid INT;
    DECLARE personality TINYINT;
    DECLARE education TINYINT;
    DECLARE experience TINYINT;

    DECLARE reason VARCHAR(100) DEFAULT "";
    DECLARE bad_candidate INT DEFAULT FALSE;

    DECLARE done INT DEFAULT FALSE;
    DECLARE crsr_cand CURSOR FOR 
    SELECT cand_usrname FROM applies WHERE job_id = j_id; 

    DECLARE CONTINUE HANDLER FOR NOT FOUND SET done = TRUE;

    OPEN crsr_cand;

    cand_loop: LOOP
    FETCH crsr_cand INTO uname;

    IF done THEN
    LEAVE cand_loop;
    END IF;
    
    SELECT id, per, ed, exp INTO iid, personality, education, experience
    FROM interviews 
    WHERE target_job = j_id AND cand_usrname = uname 
    AND (personality = 0 OR education = 0 OR experience = 0) LIMIT 1;

    IF iid IS NOT NULL THEN

    SET bad_candidate = 0;
    SET reason = "";

    IF personality = 0 THEN
    SET reason = CONCAT(reason, "Failed the interview");
    SET bad_candidate = 1;
    END IF;

    IF education = 0 THEN
    SET reason = CONCAT(reason, "Inadequate education");
    SET bad_candidate = 1;
    END IF;

    IF exp = 0 THEN
    SET reason = CONCAT(reason, "No prior experience");
    SET bad_candidate = 1;
    END IF;

    IF bad_candidate = 1 THEN
    SELECT uname, reason;
    ELSE

    SELECT cand_usrname, (AVG(interviews.personality) + SUM(interviews.education) + SUM(interviews.experience))
    FROM applies
    INNER JOIN interviews ON 
    (applies.job_id = j_id AND applies.job_id = interviews.target_job AND applies.cand_usrname = interviews.cand_usrname)
    GROUP BY cand_usrname
    ORDER BY (AVG(interviews.personality)+SUM(interviews.education)+SUM(interviews.experience)) DESC;
    END IF;

    END IF;
    END LOOP;
    CLOSE crsr_cand;
END //
DELIMITER ;


DELIMITER //
CREATE PROCEDURE BEST_CANDIDATES
(IN j_id int(4))
BEGIN
    DECLARE uname VARCHAR(12);
    DECLARE iid INT;

    DECLARE done INT DEFAULT FALSE;
    DECLARE not_all_applicants_are_validated INT DEFAULT FALSE;
    DECLARE crsr_cand CURSOR FOR 
    SELECT cand_usrname FROM applies WHERE job_id = j_id; 

    DECLARE CONTINUE HANDLER FOR NOT FOUND SET done = TRUE;

    OPEN crsr_cand;

    cand_loop: LOOP
    FETCH crsr_cand INTO uname;

    IF done THEN
    LEAVE cand_loop;
    END IF;
    
    SELECT id INTO iid FROM interviews 
    WHERE target_job = j_id AND cand_usrname = uname;

    IF id IS NULL THEN
    SET not_all_applicants_are_validated = TRUE;
    LEAVE cand_loop;
    END IF;

    END LOOP;
    CLOSE crsr_cand;

    IF not_all_applicants_are_validated THEN
    SELECT "Not all applicants are validated/ranked with interview";
    ELSE

    CALL CANDIDATES_FOR(j_id);

    END IF;
END //
DELIMITER ;

DELIMITER //
CREATE TRIGGER erecruit.cand_ins AFTER INSERT ON candidate
FOR EACH ROW
BEGIN 
INSERT INTO audit_logs VALUES (NULL, username ,"insert", NOW(), 'candidate', 1);
END //
DELIMITER ;

DELIMITER //
CREATE TRIGGER cand_upd AFTER UPDATE ON candidate
FOR EACH ROW
BEGIN 
INSERT INTO audit_logs VALUES (NULL, username ,"update", NOW(), 'candidate', 1);
END //
DELIMITER ;

DELIMITER //
CREATE TRIGGER cand_del AFTER DELETE ON candidate
FOR EACH ROW
BEGIN 
INSERT INTO audit_logs VALUES (NULL, username ,"delete", NOW(), 'candidate', 1);
END //
DELIMITER ;

DELIMITER //
CREATE TRIGGER rec_ins AFTER INSERT ON recruiter
FOR EACH ROW
BEGIN 
INSERT INTO audit_logs VALUES (NULL, username,"insert", NOW(), 'recruiter', 1);
END //
DELIMITER ;

DELIMITER //
CREATE TRIGGER rec_upd AFTER UPDATE ON recruiter
FOR EACH ROW
BEGIN 
INSERT INTO audit_logs VALUES (NULL, username,"update", NOW(), 'recruiter', 1);
END //
DELIMITER ;

DELIMITER //
CREATE TRIGGER rec_del AFTER DELETE ON recruiter
FOR EACH ROW
BEGIN 
INSERT INTO audit_logs VALUES (NULL, username,"delete", NOW(), 'recruiter', 1);
END //
DELIMITER ;

DELIMITER //
CREATE TRIGGER usr_ins AFTER INSERT ON user
FOR EACH ROW
BEGIN 
INSERT INTO audit_logs VALUES (NULL, username,"insert", NOW(), 'user', 1);
END //
DELIMITER ;

DELIMITER //
CREATE TRIGGER usr_upd AFTER UPDATE ON user
FOR EACH ROW
BEGIN 
INSERT INTO audit_logs VALUES (NULL, username,"update", NOW(), 'user', 1);
END //
DELIMITER ;

DELIMITER //
CREATE TRIGGER usr_del AFTER DELETE ON user
FOR EACH ROW
BEGIN 
INSERT INTO audit_logs VALUES (NULL, username,"delete", NOW(), 'user', 1);
END //
DELIMITER ;

DELIMITER //
CREATE TRIGGER job_ins AFTER INSERT ON job
FOR EACH ROW
BEGIN 
INSERT INTO audit_logs VALUES (NULL, recruiter,"insert", NOW(), 'jos', 1);
END //
DELIMITER ;

DELIMITER //
CREATE TRIGGER job_upd AFTER UPDATE ON job
FOR EACH ROW
BEGIN 
INSERT INTO audit_logs VALUES (NULL, recruiter,"update", NOW(), 'jos', 1);
END //
DELIMITER ;

DELIMITER //
CREATE TRIGGER job_del AFTER DELETE ON job
FOR EACH ROW
BEGIN 
INSERT INTO audit_logs VALUES (NULL, recruiter,"delete", NOW(), 'jos', 1);
END //
DELIMITER ;

DELIMITER //
CREATE TRIGGER et_ins AFTER INSERT ON etaireia
FOR EACH ROW
BEGIN 
INSERT INTO audit_logs VALUES (NULL, USER(),"insert", NOW(), 'etaireia', 1);
END //
DELIMITER ;

DELIMITER //
CREATE TRIGGER et_upd BEFORE UPDATE ON etaireia
FOR EACH ROW
BEGIN 
SET NEW.AFM = OLD.AFM;
SET NEW.DOY = OLD.DOY;
SET NEW.name = OLD.name;
INSERT INTO audit_logs VALUES (NULL, USER(),"update", NOW(), 'etaireia', 1);
END //
DELIMITER ;

DELIMITER //
CREATE TRIGGER et_del AFTER DELETE ON etaireia
FOR EACH ROW
BEGIN 
INSERT INTO audit_logs VALUES (NULL, USER(),"delete", NOW(), 'etaireia', 1);
END //
DELIMITER ;


DELIMITER //
CREATE TRIGGER prevent_application_del BEFORE DELETE ON applies
    FOR EACH ROW
    BEGIN
    IF (SELECT submission_date from job WHERE job.id = job_id) < NOW() THEN
    INSERT INTO audit_logs VALUES (NULL, cand_usrname ,"update", NOW(), 'candidate', 0);
    raise_application_error(-20001,'Application can not be deleted; submission date has passed');
    END IF;
    END //
DELIMITER ;