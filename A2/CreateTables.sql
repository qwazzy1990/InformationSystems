
/*
    Table for topics
    topic number is primary key
    topic is the name of the topic
*/
CREATE TABLE Topics(topic_number integer NOT NULL PRIMARY KEY, topic varchar(50));

-- /*Table for the projects, primary key is project number plus topic_id which is foreign key to a topic*/
CREATE TABLE Projects(project_number integer NOT NULL, 
    project_name varchar(300), 
    topic_id integer, 
    PRIMARY KEY(project_number, topic_id));

-- Table for reports. primary key is the report_number + topic_id
CREATE TABLE Reports(report_number integer NOT NULL, 
    title varchar(50), 
    topic_id integer, 
    PRIMARY KEY(report_number, topic_id)
);

-- Table for funding agencies. The fund_name is the name of the agency and is primary key
CREATE TABLE FundingAgencies(fund_name varchar(50) NOT NULL PRIMARY KEY, address varchar(300));

-- Table for grants. The primary key is the grant_number
CREATE TABLE Grants(grant_number integer NOT NULL PRIMARY KEY, 
    amount integer, 
    period varchar(50), 
    agency_id varchar(50), 
    project_id integer
);

-- Table for researchers. The primary key will be the researchers number + project_id + report_id
CREATE TABLE Researchers(researcher_number integer NOT NULL,
    researcher_name varchar(50),
    phone varchar(50),
    room varchar(50), 
    email varchar(50),
    project_id integer,
    report_id integer,
    PRIMARY KEY(researcher_number, project_id, report_id)
);
