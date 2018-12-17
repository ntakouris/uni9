/* Recruiter company and count of applicants for jobs with salary > 1900*/
SELECT `user`.`name`, `user`.surname, job.id, job.salary, etaireia.name,
 COUNT(applies.cand_usrname)  
FROM job WHERE salary > 1900
INNER JOIN `user` ON job.recruiter = `user`.username
INNER JOIN recruiter ON job.recruiter = recruiter.username
INNER JOIN etaireia ON recruiter.firm = etaireia.name
INNER JOIN applies ON job.id = applies.job_id;

SELECT candidate.username, candidate.certificates
COUNT(has_degree.degr_title), AVG(has_degree.grade) 
FROM candidate
INNER JOIN has_degree ON candidate.username = has_degree.cand_usrname
HAVING COUNT(has_degree.degr_title) > 1;

SELECT candidate.username, COUNT(applies.cand_usrname), AVG(job.salary)
FROM candidate
INNER JOIN applies ON candidate.username = applies.cand_usrname
INNER JOIN job ON applies.job_id = job.id 
WHERE job.salary > 1800;

/* Don't know if SUM is going to work properly */
SELECT etaireia.name, job.position, SUM(requires.antikeim_title)
FROM etaireia 
INNER JOIN recruiter ON etaireia.name = recruiter.firm
INNER JOIN job ON recruiter.username = job.recruiter
INNER JOIN requires ON job.id = requires.job_id
WHERE etaireia.city = 'Patras' AND job.position LIKE '%Program%';

SELECT recruiter.username, COUNT(interviews.id) as NumInterviews,
COUNT(job.id) as NumJobs, AVG(job.salary) as AvgSalary
FROM recruiter
INNER JOIN job ON recruiter.username = job.recruiter
INNER JOIN interviews ON recruiter.username = interviews = recruiter_name
WHERE COUNT(job.id) > 2 ORDER BY AVG(job.salary) DESC;