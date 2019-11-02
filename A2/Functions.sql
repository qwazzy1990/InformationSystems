-- function for question 1

CREATE OR REPLACE FUNCTION findReport(val integer) RETURNS TABLE(rn int, 
    tit varchar(300),
    topic int) AS $table1$
    -- returns a table that contains the report number, title and topic id of a report of a researcher if the researchers number is the same as val
BEGIN
    RETURN QUERY 
    SELECT report_number, title, topic_id FROM Researchers INNER JOIN Reports on researcher_number = val AND report_id = report_number;
END; $table1$
LANGUAGE PLPGSQL;


-- Function to find all the pis, display their name, id and amount of funding
CREATE OR REPLACE FUNCTION findPI() RETURNS void as $$

DECLARE 
    --check if the researcher is a pi and put their info into id, rname and isPi
    c1 cursor for SELECT DISTINCT researcher_number, researcher_name, pi, project_id from Researchers;
    c2 cursor (id integer) for SELECT amount from Grants where project_id = id;
    projId integer;
    id integer;
    rname varchar(100);
    amnt integer;
    isPi boolean;
BEGIN
    open c1;
    loop
        fetch c1 into id, rname, isPi, projId;
        exit when not found;
        if (isPi = true) then
            RAISE NOTICE 'ID: %', id;
            RAISE NOTICE 'Name: %', rname;
            open c2(id:=projId);
            RAISE NOTICE 'Amount %', amnt;
            close c2;


        else
            RAISE NOTICE 'Not Applicable';
        end if;
    end loop;
    close c1;

END;

$$ LANGUAGE PLPGSQL;

CREATE OR REPLACE FUNCTION addNewProject(projectNumber integer,
     projectName varchar(200), 
    topicCode integer, 
    piId integer, 
    grantNumber integer, 
    amount integer,
    priod varchar(50),
    fundingAgency varchar(50)
) 
RETURNS void as $$
DECLARE
    c1 cursor (id integer) for SELECT DISTINCT researcher_number, researcher_name, phone, room, email, pi from Researchers where id = projectNumber;
    researcherNumber integer;
    researcherName varchar(300);
    researcherPhone varchar(30);
    researcherEmail varchar(100);
    researcherRoom varchar(30);
    researcherPi boolean;
BEGIN
    INSERT INTO Projects VALUES(projectNumber, projectName,topicCode);
    INSERT INTO Grants VALUES(grantNumber, amount, priod, fundingAgecny, projectNumber);

    open c1(id:=projectNumber);
    close c1;

END;

$$ LANGUAGE PLPGSQL;