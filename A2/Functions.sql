CREATE OR REPLACE FUNCTION queryOne(val integer) RETURNS TABLE(rn int, 
    tit varchar(300),
    topic int) AS $table1$
BEGIN
    RETURN QUERY 
    SELECT report_number, title, topic_id FROM Researchers INNER JOIN Reports on researcher_number = val AND report_id = report_number;
END; $table1$
LANGUAGE PLPGSQL;

CREATE OR REPLACE FUNCTION queryTwo() RETURNS void as $$

DECLARE 
    c1 cursor for select researcher_number, researcher_name, pi from Researchers;
    id integer;
    rname varchar(100);
    isPi boolean;
BEGIN
    open c1;
    loop
        fetch c1 into id, rname, isPi;
        exit when not found;
        if (isPi = true) then
            RAISE NOTICE 'ID: %', id;
            RAISE NOTICE 'Name: %', rname;
            RAISE NOTICE 'I do not like to share my income';
        end if;
    end loop;
    close c1;

END;

$$ LANGUAGE PLPGSQL;