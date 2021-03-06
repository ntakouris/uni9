/* Recruiter company and count of applicants for jobs with salary > 1900*/
SELECT `user`.`name`, `user`.surname, job.id, job.salary, etaireia.name AS 'company', COUNT(applies.cand_usrname) AS 'applicants'  
FROM job
INNER JOIN `user` ON job.recruiter = `user`.username
INNER JOIN recruiter ON job.recruiter = recruiter.username
INNER JOIN etaireia ON recruiter.firm = etaireia.AFM
INNER JOIN applies ON job.id = applies.job_id WHERE job.salary > 1900 
GROUP BY `user`.`name`,`user`.surname, job.id, job.salary, etaireia.name;

SELECT candidate.username, candidate.certificates,
COUNT(has_degree.degr_title) AS 'degrees', AVG(has_degree.grade) AS 'average grade' 
FROM candidate
INNER JOIN has_degree ON candidate.username = has_degree.cand_usrname
GROUP BY candidate.username, candidate.certificates
HAVING COUNT(has_degree.degr_title) > 1 ;

SELECT candidate.username, COUNT(applies.cand_usrname) AS 'applications', AVG(job.salary) AS 'average salary'
FROM candidate
INNER JOIN applies ON candidate.username = applies.cand_usrname
INNER JOIN job ON applies.job_id = job.id 
WHERE job.salary > 1800 GROUP BY candidate.username;

SELECT etaireia.name AS company, job.position, GROUP_CONCAT(requires.antikeim_title SEPARATOR ' ')
FROM etaireia 
INNER JOIN recruiter ON etaireia.AFM = recruiter.firm
INNER JOIN job ON recruiter.username = job.recruiter
INNER JOIN requires ON job.id = requires.job_id
WHERE (etaireia.city = 'Patra') AND requires.antikeim_title LIKE '%Program%' GROUP BY etaireia.name, job.position;

SELECT recruiter.username, COUNT(DISTINCT interviews.id) as Interviews,
COUNT(DISTINCT job.id) as Jobs, AVG(job.salary) as AvgSalary
FROM recruiter
INNER JOIN job ON recruiter.username = job.recruiter
INNER JOIN interviews ON recruiter.username = interviews.recruiter_name
GROUP BY recruiter.username
HAVING COUNT(job.id) > 2 
ORDER BY AVG(job.salary) DESC;