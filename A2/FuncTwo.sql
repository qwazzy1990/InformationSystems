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
            RAISE NOTICE 'Name: %', rname;
        end if;
    end loop;
    close c1;

END;

$$ LANGUAGE PLPGSQL;