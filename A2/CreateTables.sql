CREATE TABLE Projects(id integer NOT NULL, project_name varchar(50) NOT NULL, pi boolean NOT NULL, grant_id integer, topic_id integer, researcher_id);
CREATE TABLE Topics(id integer NOT NULL, topic_name varchar(50) NOT NULL, project_id integer, report_id integer);
CREATE TABLE Reports(id integer NOT NULL, title varchar(50), researcher_id integer);
CREATE TABLE FundingAgencies(id varchar(50) NOT NULL, address varchar(300), grant_id integer);
CREATE TABLE Grants(id integer NOT NULL, amount integer, period varchar(50), project_id integer, agency_id varchar(50));
CREATE TABLE Researchers(id integer, first_name varchar(50), last_name varchar(50), office varchar(50), is_pi boolean, email varchar(50), project_id integer, report_id integer);
