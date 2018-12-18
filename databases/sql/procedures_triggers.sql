DELIMITER //
CREATE PROCEDURE CANDIDATES_FOR
(IN j_id int(4))
BEGIN
    DECLARE uname VARCHAR(12);

    DECLARE iid INT;
    DECLARE per TINYINT;
    DECLARE ed TINYINT;
    DECLARE exp TINYINT;

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
    AND (personality = 0 OR education = 0, OR experience = 0) LIMIT 1;

    IF iid IS NOT NULL

    SET bad_candidate = FALSE;
    SET reason = "";

    IF personality = 0 THEN
    SET reason = CONCAT(reason, "Failed the interview");
    SET bad_candidate = TRUE;
    END IF;

    IF education = 0 THEN
    SET reason = CONCAT(reason, "Inadequate education");
    SET bad_candidate = TRUE;
    END IF;

    IF exp = 0 THEN
    SET reason = CONCAT(reason, "No prior experience");
    SET bad_candidate = TRUE;
    END IF;

    IF bad_candidate THEN
    SELECT uname, reason;
    ELSE

    SELECT cand_usrname, (AVG(interviews.personality)+SUM(interviews.education)+SUM(interviews.experience))
    FROM applies WHERE job_id = j_id
    INNER JOIN interviews ON 
    (applies.job_id = interviews.target_job AND applies.cand_usrname = interviews.cand_usrname)
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
    not_all_applicants_are_validated = TRUE;
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