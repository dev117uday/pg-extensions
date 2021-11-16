CREATE OR REPLACE FUNCTION
addme(int,int) RETURNS int AS 'MODULE_PATHNAME','addme'
LANGUAGE C STRICT;

create extension addme;

select addme(2,3);