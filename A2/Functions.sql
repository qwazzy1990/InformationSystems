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
    c2 cursor for SELECT amount from Grants;
    projId;
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
            RAISE NOTICE 'I do not like to share my income';
            fetch c2 into amnt where Grants.project_id = projId; 
        else
            RAISE NOTICE 'Not Applicable';
        end if;
    end loop;
    close c1;

END;

$$ LANGUAGE PLPGSQL;